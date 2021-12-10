/**********************************************************************
 *  Project App6
 *
 *  biped.cpp
 *
 *  Author: Vladimir Baranov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
**********************************************************************/


#include <string>
#include <stdio.h>
#include "biped.h"
#include "../include/drawings2d.h"
#include "../include/group.h"
#include "../include/var_parse.h"
#include "../include/print.h"
#include "../include/camera.h"
#include "../include/vector3d.h"
#include "../include/cinput.h"
#include "../include/cobjregistry.h"
#include "../include/tranform.h"
#include "../include/vpnt.h"


static CObjRegistry& MREG = CObjRegistry::getInstance2();
extern void  SeekNext2(FILE* f, unsigned char sig, int Ntimes);
VPnt* np;
VPnt* A;
VPnt* B;
using namespace std;

Biped::Biped()
{
    ctrl_objs = " ";
    t_sim = rand()%10;
    cur_frame = 0;
    speed = 24  ;
    kurs = 0;
    head = 0;
    delta2 = Pnt(80,-110,0);
    delta = Pnt(20,0,-5);
    basis_mat.rotate(180,1,0,0);
    basis_mat = basis_mat.inverted();
    control.SetControlShema(0x69,0x6B,0x6C,0x6A);
    timer.start();
    CInput* iinput = CInput::getInstance();
    iinput->add_delegat(this);
    np = new VPnt();
    np->x = rand()%10; np->y = rand()%10; np->z = rand()%10;
    np->SetName("neck_p");
    np->ShowBounds = false;
    MREG.AddObj(np);
    A = new VPnt();
    A->SetName("A");
    A->ShowBounds = false;
    //A->color[0] = 1.0;
    MREG.AddObj(A);
    B = new VPnt();
    //B->color[0] = 1.0;
    B->SetName("B");
    B->ShowBounds = false;
    MREG.AddObj(B);

    isWalking = false;
    isStrafeLeft = false;
    isStrafeRight = false;
    isWalkingBackwards = false;

    /*
    weapon = new Weapon();
    weapon->type = Weapon::RAILGUN;
    weapon->Init();
    MREG.AddObj(weapon);
    */

}

void Biped::Load(string& s)
{


    /*
      if(s == "") {
        FramesLoad(keys_file, false,"","");
        return;
    }
    vector<string> args = StrSplitE(s,",",false);    
    if(args.size() == 3) { FramesLoad(keys_file, args[0]=="true",args[1],args[2]); }
    */
}

void Biped::Play(int from_frame, int N_frames_to_play, bool loop)
{
    t_sim = from_frame * 30;
    N_frames =  from_frame + N_frames_to_play;
    isCanPlay = true;
    isLoop = loop;
}

void Biped::FramesLoad(string fn, int N_fr)
{

    FILE* f = fopen(fn.c_str(), "rb");
    if(f == NULL) return;

    isCanPlay = false;

    N_frames = N_fr;

    char gstr[512];

    states_data.clear();

    for(int j=0;  j<n; j++)
    {
        fread(gstr, 1, 256, f);
        string name = gstr;
        GemObject* go = (GemObject*)MREG.DeepFindObj(name);
        if(go!=0)
        {
            scale = go->ei.scale.x;
            ctrl_objs+= name + " ";
        }
        for(int i=0;  i<N_frames; i++)
        {
            BipState* s = new BipState();
            fread(s, sizeof(BipState),1,f);            
            if( (go!=0) &&( go->classname == "GemObject"))
                states_data[go].push_back(*s);               
        }
        SeekNext2(f, 0xFA, 8);
    }
    fclose(f);

}

void Biped::CreateDefaultTransformStack(GemObject* go)
{
    /*
    Transform& transform = go->transform_new;
    go->bTransformReady = false;
    transform.Clear();
    transform.AddTranslate(go->ei.coord0.x, go->ei.coord0.y, go->ei.coord0.z);
    transform.AddRotate(go->ei.rot.y,0,1,0);
    Pnt tv = Pnt(go->ei.coord.x - go->C0.x,  go->ei.coord.y - go->C0.y, go->ei.coord.z - go->C0.z);
    transform.AddTranslate(tv.x, tv.y, tv.z);
    transform.AddTranslate(go->C0.x, go->C0.y, go->C0.z);
    transform.AddRotate(go->ei.rot.x, go->rot.x, go->rot.y, go->rot.z);
    transform.AddTranslateMinus(go->C0.x, go->C0.y, go->C0.z);
    go->bTransformReady = true;
    */
    go->model_matrix.setToIdentity();
    go->model_matrix.scale(go->ei.scale.x, go->ei.scale.y, go->ei.scale.z);
    go->model_matrix.translate(go->ei.coord0.x, go->ei.coord0.y, go->ei.coord0.z);
    go->model_matrix.rotate(go->ei.rot.y,0,1,0);
    Pnt tv = Pnt(go->ei.coord.x - go->C0.x,  go->ei.coord.y - go->C0.y, go->ei.coord.z - go->C0.z);
    go->model_matrix.translate(tv.x, tv.y, tv.z);
    go->model_matrix.translate(go->C0.x, go->C0.y, go->C0.z);
    go->model_matrix.rotate(go->ei.rot.x, go->rot.x, go->rot.y, go->rot.z);
    go->model_matrix.translate(-go->C0.x, -go->C0.y, -go->C0.z);

}

void Biped::DoShot()
{
    GemObject* Head = (GemObject*)MREG.DeepFindObj("Bip001 Head");
    GemObject* Neck  = (GemObject*)MREG.DeepFindObj("Bip001 Neck");
    CVector3 neckv = CVector3(Head->ei.coord) - CVector3(Neck->ei.coord);
    double t;
    t = 1.7;
    CVector3 plechov = CVector3(Neck->ei.coord) + t*neckv.rotateAboutZ((float)M_PI_2);
    plechov = plechov + 1.3*neckv.rotateAboutZ((float)M_PI_2).rotateAboutY((float)M_PI_2);
    CVector3 plechov2 = plechov + 4*neckv.rotateAboutZ((float)M_PI_2).rotateAboutY((float)M_PI_2);
    plechov = plechov.rotateAboutY((Neck->ei.rot.y-45)*GR);
    plechov2 = plechov2.rotateAboutY((Neck->ei.rot.y-45)*GR);
    //weapon->Shot((plechov2 + pos)* Neck->ei.scale.x, lookAt2.normalize());
    //weapon->Shot((plechov2 + pos)* Neck->ei.scale.x, lookAt);
    return; 
}

void Biped::CreateCopy()
{
//    map<GemObject*,vector<BipState>>::iterator it;
//    for(it = states_data.begin(); it!=states_data.end(); it++)
//    {
//        GemObject* go = it->first;
//        GemObject* go_copy = new GemObject(go);
//        Group* TTT = (Group*)MREG.DeepFindObj(go->groupname);
//        TTT->AddObj(go_copy);
//    }
}

void Biped::Slot1()
{
    // Делаем цикл симуляции
    dt=(double)(timer.elapsed())/(double)(1000.);

    step = speed * dt;
    t_sim+=dt;

    cur_frame = t_sim * 30;

    if(cur_frame > N_frames)
    {
        if( isLoop )
        {
            t_sim = 0;
        }
        else
            t_sim = (double)(N_frames-1) / 30.0;
    }
    ProcessInput();
    if( isCanPlay )
        SimpleAnimate(cur_frame);
    SetFPCamera();
    //prn("cur_frame=%d",cur_frame);
    //prn("Biped pos is %f",pos.x);
    timer.restart();
}


void Biped::SimpleAnimate(int cur_frame)
{

    map<GemObject*,vector<BipState>>::iterator it;

    //walk states
    for(it = states_data.begin(); it!=states_data.end(); it++)
    {
        if( !isCanPlay ) return;
        GemObject* go = it->first;
        vector<BipState>& states = it->second;
        if(cur_frame < (int)states.size())
        {
            BipState& s = states[cur_frame];
            go->ei.coord.x = -s.pos[0]; go->ei.coord.y = s.pos[2]; go->ei.coord.z = s.pos[1];
            go->ei.coord= go->ei.coord + Pnt(44.000000,17.000000,-14.000000);
            go->ei.coord0 = this->pos;
            go->ei.rot.y = kurs ;

            QQuaternion q(s.q[3],s.q[0],s.q[2],s.q[1]);
            float angle;
            QVector3D axis;
            q.getAxisAndAngle(&axis, &angle);
            axis = axis * basis_mat;
            go->rot = Pnt(axis.x(),axis.y(),axis.z());
            go->ei.rot.x = angle;
            CreateDefaultTransformStack(go);

        }
    }

/*
    //torso aim states
    int aim_frame = 25 - (80+lookAt.y)/1.5;//lookAt.y / 60;
    if(aim_frame > 59) aim_frame = 59;
    if(aim_frame < 0) aim_frame = 0;
    //prn("aim_frame=%d",aim_frame);
    for(it = states_aim.begin(); it!=states_aim.end(); it++)
    {
        GemObject* go = it->first;
        vector<BipState>& states = it->second;
        if( go->name.find("Pelvis")!=string::npos) continue;
        if( go->name.find("Thigh")!=string::npos) continue;
        if( go->name.find("Calf")!=string::npos) continue;
        if( go->name.find("Foot")!=string::npos) continue;
        if( go->name.find("Toe0")!=string::npos) continue;
        if( go->name.find("Toe01")!=string::npos) continue;
        if( go->name.find("Toe02")!=string::npos) continue;
        BipState& s = states[aim_frame];
        go->ei.coord.x = -s.pos[0]; go->ei.coord.y = s.pos[2]; go->ei.coord.z = s.pos[1];
        go->ei.coord= go->ei.coord + Pnt(44.000000,17.000000,-14.000000);
        go->ei.coord0 = this->pos;
        go->ei.rot.y = kurs ;
        Quaternion<float> q(s.q[3],s.q[0],s.q[2],s.q[1]);
        float angle;
        Vector3<float> axis;
        q.ToAxisAngle(axis,angle);
        axis = axis * basis_mat;
        go->rot = Pnt(axis.X(),axis.Y(),axis.Z());
        go->ei.rot.x = angle / GR;     
    }
    for(it = states_aim.begin(); it!=states_aim.end(); it++)
    {
        GemObject* go = it->first;
        CreateDefaultTransformStack(go);
    }
    */
}


//--------------------------------- Методы Player

//inline Vector3<float> Pnt2Vec(Pnt& p){return Vector3<float>(p.x,p.y,p.z); }
//inline Pnt Vec2Pnt(Vector3<float>& v){return Pnt(v.X(),v.Y(),v.Z());}

void Biped::SetFPCamera()
{
    static Camera* cam = 0;
    static Camera* cam3rd = 0;
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    if(head == 0)
    {
        map<GemObject*,vector<BipState>>::iterator it;
        for(it = states_data.begin(); it!=states_data.end(); it++)
        {
            GemObject* go = it->first;
            if(go->name.find("Head")!=string::npos)
            {
                head = go;
                break;
            }
        }

    }

    if(head == 0) return;

    if(cam == 0 || cam3rd == 0)
    {
        //cam = (Camera*)MREG.FindObj("cam02");
        cam3rd = (Camera*)MREG.FindObj("cam_3person");
        cam = cam3rd;
        if(cam == 0 || cam3rd == 0)
        {
            print("BIPED: cant find cam3rd and cam object");
        }
    }
    else
    {
        Pnt visLineOrigin;

        {
            cam->ei.coord = head->ei.coord * head->ei.scale.x + pos* head->ei.scale.x;
            cam->ei.coord.y+=0.3f;
            //cam->isReceiveInput = true;
            visLineOrigin = cam->ei.coord;
            cam->UpdateLookAt();
            rot = (cam->lookAt - cam->ei.coord);
            if( !isWalkingBackwards )
                kurs = 180 - cam->orient.B.x;
            else
                kurs = 360 - cam->orient.B.x;

            float t = -0.5;
            CVector3 P3 = CVector3(cam->ei.coord)+t*CVector3(rot);
            lookAt = Pnt(cam->orient.B.x,cam->orient.B.z,0);
            lookAt2 = cam->lookAt;
            if(cam3rd!=0)
            {
                cam3rd->ei.coord = P3.toPnt();
                cam3rd->lookAt = cam->lookAt;
                //cam3rd->isReceiveInput = false;
            }
        }

        //CreateLineP(visLineOrigin, rot, "test_line" + name);
        //CreateLineP(cam->ei.coord, Vperp.toPnt(), "test_line_perp");
        return;



        //prn("kurs=%f",kurs/GR);


    }

}

void Biped::click_lmb(int state)
{
    if(state == 1)
        DoShot();
}

void Biped::key_upressed(int k)
{
    if(control.isControlKey(k))
    {
        //FramesLoad("Biped/biped_aim_60.bip", 60);
        //states_aim = states_data;
        if( !isStrafeLeft && !isStrafeRight )
        {
            FramesLoad("Biped/biped_walk_stop_12.bip", 12);
            states_walk = states_data;
            Play(0,12,false);
        }
        if( isWalking )
            isLoop = false;

        isWalking = false;
        isStrafeLeft = false;
        isStrafeRight = false;
        isWalkingBackwards = false;

        prn("biped_walk_stop_12.bip");
    }
}

void Biped::key_pressed(int k)
{
    //prn("press %d isWalking=%d", rand(), isWalking);
    if(control.isControlKey(k))
    {
        //pos.y = pos.y - step;
        if(k==control.ForwardKey)
        {
            if( isWalking==false )
            {
                FramesLoad("Biped/biped_walk_40.bip", 40);
                Play(0,40,true);
                prn("biped_walk_40.bip");
            }
            isWalking = true;
        }
        if(k==control.RightKey)
        {
            if(isStrafeRight==false)
            {
                FramesLoad("Biped/biped_strafe_r_24.bip", 24);
                Play(0,24,true);
                prn("biped_strafe_r_24.bip");
            }
            isStrafeRight = true;
        }
        if(k==control.LeftKey)
        {
            if(isStrafeLeft==false)
            {
                FramesLoad("Biped/biped_strafe_r_24.bip", 24);
                Play(0,24,true);
                prn("biped_strafe_r_24.bip");
            }
            isStrafeLeft = true;
        }
        if(k==control.BackwardKey)
        {
            if(isWalkingBackwards==false)
            {
                FramesLoad("Biped/biped_walk_40.bip", 40);
                Play(0,40,true);
                prn("biped_walk_40.bip");
            }
            isWalkingBackwards = true;
        }


    }
/*
    if (k == 0x31)
    {
        //pos.y = pos.y - step;
        FramesLoad("Biped/biped_walk_40.bip", 40);
        Play(0,40,true);
        prn("biped_walk_40.bip");

    }
*/

    if (k == 0x32)
    {


    }
}

void Biped::ProcessInput()
{
    //if(!isReceiveInput) return;
    //if(isSelected == 0) return;

    CInput* iinput = CInput::getInstance();

     //prn("%f %f",axisX.Dot(dir),axisZ.Dot(dir));
    step = step;
    //prn("scale =%f", scale);



    if(iinput->keystates[control.ForwardKey]) //i
    {
        CVector3 pos_new = CVector3(pos)+step*CVector3(rot);
        pos.x = pos_new.x;
        pos.z = pos_new.z;
    }
    if(iinput->keystates[control.BackwardKey]) //k
    {
        CVector3 pos_new = CVector3(pos)-step*CVector3(rot);
        pos.x = pos_new.x;
        pos.z = pos_new.z;
    }

    if(iinput->keystates[control.LeftKey]) //j
    {
        CVector3 pos_new = CVector3(pos)+step*CVector3(rot).rotateAboutY((float)M_PI_2);
        pos.x = pos_new.x;
        pos.z = pos_new.z;
        //kurs+=5*GR;

        //pos.x  = pos.x - step*sin((kurs+1.5707));
        //pos.z  =  pos.z + step*cos((kurs+1.5707));
    }
    if(iinput->keystates[control.RightKey])  //l
     {
        CVector3 pos_new = CVector3(pos)-step*CVector3(rot).rotateAboutY((float)M_PI_2);
        pos.x = pos_new.x;
        pos.z = pos_new.z;
        //kurs-=5*GR;
    }

}
