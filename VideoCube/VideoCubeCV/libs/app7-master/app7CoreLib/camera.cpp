/**********************************************************************
 *  Project App6
 *
 *  camera.cpp
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

#include <QApplication>
#include <QDesktopWidget>
#include "geometry.h"
#include "cinput.h"
#include "selection.h"
#include "cmaininterface.h"
#include "print.h"
#include "flags.h"
#include "param_screen_bcb.h"
#include "cobjregistry.h"
#include "camera.h"
#include "timer.h"
#include "group.h"
#include "cmaininterface.h"
#include "glcode.h"

extern char scripttext[100000];
//#define RGB(r,g,b)      ((((uchar)(r)|((uchar)((uchar)(g))<<8))|(((uint)(uchar)(b))<<16)))
static ParamScreen& pscr = ParamScreen::getInstance();

Camera::Camera()
{

    num = 1;

    char gstr[255];
    sprintf_s(gstr, "cam%02d", num);
    name = gstr;
    isEntety = true;
    isMouseEnabled = false;
    isReceiveInput = false;
    isCanRender = true;
    drag_speed = 1;


    //собственные
    //isVisible = false;
    orient = Vector(Pnt(0,0,0),Pnt(169.00, 90.00, 267.00));
    ei.coord = Pnt(1,10,16);
    lookAt = Pnt(0,0,0);
    dist = 10;
    fovy = 45;
    zFar = 30;
    colx = 255;
    zNear = 1;
    speed = 1;
    sens  = 1;
    blink = 0;
    tlastmove = 0;
    prev_x = 0;
    prev_y = 0;
    bBreakLookAt = 0;
    pscr.AddVar("camPos");
    pscr.AddVar("teta");
    pscr.AddVar("phi");
    pscr.AddVar("cp");
    view = 0;
    looktarget = 0;
    followtarget = 0;
    target = 0;
    views = "freecamera top left right bottom front back";
    //main_if.selobj = NULL;
    dt = 0; k=1; tm=0;
    //if(num == 1)
    CInput* iinput = CInput::getInstance();
    iinput->add_delegat(this);

    // запрос на апдейт
    Connect();

    RES_X = QApplication::desktop()->width();
    RES_Y = QApplication::desktop()->height();
    aspect = QApplication::desktop()->width() / QApplication::desktop()->height();


}


//object inspector event
void Camera::OnAttrChange(t_atr& a)
{
    //mu_printf("Camera::OnAttrChange %s data=%d", a.name.c_str(), a.data);
    if(  (a.name == "view")  )
    {
        //if(isLoading) return;
        int view = *(int*)a.addr;
        switch(view)
        {
        case 0:
            FreeView();
            break;
        case 1:
            TopView();
            isReceiveInput = true;
            break;
        case 2:
            LeftView();
            break;

        }

    }

    if(  (a.name == "num")  )
    {
        //if(num == 1)
        //	isReceiveInput = 1;
    }

    if(  (a.name == "isReceiveInput")  )
        isReceiveInput = 0;


}


void Camera::TopView()
{
    //cosd.SetTopText("top view", 50, num);
    view = 1;
    if(ATTR_EXIST("view"))
        atrs[atrsmap["view"]].data = view;
    //ei.coord.y=10;

    lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
    lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
    lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);


}


void Camera::LeftView()
{
    //cosd.SetTopText("left view", 50, num);
    view = 2;
    //ei.coord.y=0;
    ei.coord.z=5;
    //orient.B.y-=4;
    if(ATTR_EXIST("view"))
        atrs[atrsmap["view"]].data = view;
}

void Camera::FreeView()
{
    //cosd.SetTopText("free view", 50, num);
    view = 0;
    //ei.coord.y=5;
    if(ATTR_EXIST("view"))
        atrs[atrsmap["view"]].data = view;

    //ei.coord.y=5;
    /*orient.B.x=180.001;
    orient.B.y=0.001;
    orient.B.z=180.001;
    */
    lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
    lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
    lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);


}

void Camera::LookAt(char* name)
{
    Object* obj;
    obj = MREG_FindObj(name);
    if (!obj)
        return;
    looktarget = true;
    target = obj;
}
//void Camera::

void Camera::move2(float f1, float f2, float f3)
{
    mu_printf("move2");
    ei.coord = Pnt(f1,f2,f3);

}

//extern void projectp(Pnt op, CVector3& p1);


void Camera::mouse_move(int x, int y)
{

    if(!isReceiveInput)
    {
        //prev_x = x;
        //prev_y = y;
        return;
    }

    //GetResolution(&ResX, &ResY);
    // если прошло меньше секунды коррект шаг

    dt = Timer::gtimer.fsec - tm;
    tm = Timer::gtimer.fsec;

    CMainInterface& main_if = CMainInterface::getInstance();
    CInput* iinput = CInput::getInstance();
    CSelection& gselection = CSelection::getInstance();

//------------ ПЕРЕНЕСТИ ЭТОТ КОД КУДА ЛИБО (НЕ ОТНОСИТСЯ К КАМЕРЕ)	 --------------------------
if(main_if.isInRotateMode)
    {
            if(iinput->check_lmb() && gselection.objs.size()>0)
       {

            if(view == 1)
            {
                vector<Object*>::iterator obj;
                for(obj = gselection.objs.begin(); obj != gselection.objs.end(); obj++ )
                {


                if( main_if.axis == 0 ) //x
                {
                    //(*obj)->ei.coord.x+= (float)(y - prev_y)/50;
                    (*obj)->ei.rot.x+= (float)(y - prev_y)/5;
                }

                if( main_if.axis == 1 ) //y
                {
                    (*obj)->ei.rot.y+= (float)(y - prev_y)/5;
                }

                if( main_if.axis == 2 ) //z
                {
                    (*obj)->ei.rot.z+= (float)(y - prev_y)/5;
                }
                }
                //(*obj)->OnDrag(iinput->delta.x, 0, iinput->delta.z);
            }
        }
        prev_x = x;
        prev_y = y;

        return;
    }

if(main_if.isInScaleMode)
    {
        if(iinput->check_lmb() && gselection.objs.size()>0)
        {

            if(view == 1)
            {
                vector<Object*>::iterator obj;
                for(obj = gselection.objs.begin(); obj != gselection.objs.end(); obj++ )
                {



                if( main_if.axis == 0 ) //x
                {
                    //(*obj)->ei.coord.x+= (float)(y - prev_y)/50;
                    (*obj)->ei.scale.z = 1;
                    (*obj)->ei.scale.y = 1;
                    if((x - prev_x)<0)
                        (*obj)->ei.scale.x = 1.1f;
                    else
                        (*obj)->ei.scale.x = 0.9f;

                }

                if( main_if.axis == 1 ) //y
                {
                    //(*obj)->ei.scale.y = (float)(y - prev_y)/50;
                }

                if( main_if.axis == 2 ) //z
                {
                    (*obj)->ei.scale.x = 1;
                    (*obj)->ei.scale.y = 1;
                    if((y - prev_y)<0)
                        (*obj)->ei.scale.z = 1.1f;
                    else
                        (*obj)->ei.scale.z = 0.9f;
                }
                (*obj)->Scale((*obj)->ei.scale.x,(*obj)->ei.scale.y,(*obj)->ei.scale.z);
                }
                //(*obj)->OnDrag(iinput->delta.x, 0, iinput->delta.z);

            }
        }
        prev_x = x;
        prev_y = y;

        return;
    }


    if(main_if.isInMove)
    {
        if(iinput->check_lmb() && gselection.objs.size()>0)
        {
            //prn("move");
            if(view == 0)
            {
                vector<Object*>::iterator obj;
                for(obj = gselection.objs.begin(); obj != gselection.objs.end(); obj++ )
                {
                    (*obj)->ei.coord.z-= (float)(x - prev_x)/50;
                    (*obj)->ei.coord.x+= (float)(y - prev_y)/50;

                if( main_if.axis == 0 ) //x
                {
                    (*obj)->ei.coord.x+= (float)(y - prev_y)/50;
                }

                if( main_if.axis == 1 ) //y
                {
                    (*obj)->ei.coord.y-= (float)(y - prev_y)/50;
                }

                if( main_if.axis == 2 ) //z
                {
                    (*obj)->ei.coord.z+= (float)(y - prev_y)/50;
                }

                // обьект двигает группу
                /*	if((*obj)->groupname!="")
                    {
                        Object* obj2 = (Object*)GET_OBJECT((char*)(*obj)->groupname.c_str());
                        if(obj2!=NULL && obj2->isGroupMovable )
                        {
                            obj2->ei.coord.x+= (float)(y - prev_y)/50;
                            obj2->ei.coord.z-= (float)(x - prev_x)/50;
                        }
                    }
                */
                        // группа двигает обьект
                    if( (*obj)->isGroup )
                    {
                        Group* obj2 = (Group*)(*obj);
                        if(obj2!=NULL)
                        {
                            if( obj2->isGroupMovable )
                            {
                                for(unsigned int i=0 ; i<obj2->objs.size(); i++)
                                {
                                    //obj2->objs[i]->ei.coord = obj2->objs[i]->ei.coord0;
                                    //obj2->objs[i]->ei.coord.z-= (float)(x - prev_x)/50;
                                    //obj2->objs[i]->ei.coord.x-= (float)(y - prev_y)/50;
                                    //obj2->objs[i]->OnDrag((y - prev_y)/50, 0, (x - prev_x)/50);

                                    if( main_if.axis == 0 ) //x
                                    {
                                        obj2->objs[i]->ei.coord.x+= (float)(y - prev_y)/50;
                                    }

                                    if( main_if.axis == 1 ) //y
                                    {
                                        obj2->objs[i]->ei.coord.y-= (float)(y - prev_y)/50;
                                    }

                                    if( main_if.axis == 2 ) //z
                                    {
                                        obj2->objs[i]->ei.coord.z+= (float)(y - prev_y)/50;
                                    }
                                }
                            }
                        }
                    }

                }
            }


            if(view == 1)
            {
                vector<Object*>::iterator obj;
                for(obj = gselection.objs.begin(); obj != gselection.objs.end(); obj++ )
                {
                    //(*obj)->ei.coord.z-= iinput->delta.z;
                    //(*obj)->ei.coord.x-= iinput->delta.x;
                    //prn();
                    //mu_printf("delta %f %f %f", iinput->delta.x,iinput->delta.y,iinput->delta.z);
                    //(*obj)->OnDrag(iinput->delta.x, 0, iinput->delta.z);
                }
            }

        }
//------------ ПЕРЕНЕСТИ ЭТОТ КОД КУДА ЛИБО (НЕ ОТНОСИТСЯ К КАМЕРЕ)	 --------------------------


        prev_x = x;
        prev_y = y;

        return;


    }

    if(main_if.isInSelectionMode)
    {
        return;

    }

    /*

      if(x > (ResX - 50))
      {
      prev_x = 0;
      x = 0;
      SetCursorPos(x,y);
      }

        if(x < 10)
        {
        prev_x = ResX / 2;
        x = ResX / 2;
        SetCursorPos(x,y);
        }

          if(y > (ResY - 50))
          {
          prev_y = 0;
          y = 0;
          SetCursorPos(x,y);
          }

            if(y < 10)
            {
            prev_y = ResY / 2;
            y = ResY / 2;
            SetCursorPos(x,y);
            }
    */

    //if(cnt == 2)
    //{
    //if((x - prev_x)!=0)
    //	angle = 180/M_PI * atan(((y - prev_y))/((x - prev_x)));


//	cnt = 0;

    if(view == 0)
    {

        if(!isMouseEnabled)
            return;

        if(prev_x != 0)
        {
            orient.B.x+=(x - prev_x);
            orient.B.z-=(y - prev_y);
            OnMove();
        }


        if(!looktarget)
        {
            lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
            lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
            lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);


        }

        if(followtarget)
        {
                lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.x*sens);
                lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
                lookAt.z =  ei.coord.z + dist*cos(GR * orient.B.x*sens);
        }


            if(orient.B.x > 360) orient.B.x = orient.B.x - 360;
            if(orient.B.z > 360) orient.B.z = orient.B.z - 360;
            if(orient.B.x < -360) orient.B.x = orient.B.x + 360;
            if(orient.B.z < -360) orient.B.z = orient.B.z + 360;

    }

    if(view == 1)
    {
        if( iinput->check_lmb() )
        {
            //ei.coord.z-= (float)(x - prev_x)/50;
            if(prev_x != 0)
            {

                ei.coord.x+=  (float)drag_speed*(x - prev_x)/10;
                ei.coord.z+=  (float)drag_speed*(y - prev_y)/10;
                orient.B.x=180.001f;
                orient.B.y=0.001f;
                orient.B.z=180.001f;
            }
            lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z)*sin(GR * orient.B.x);
            lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z);
            lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z)*cos(GR * orient.B.x);
            OnMove();
        }
    }


    if(view == 2)
    {
        if( iinput->check_lmb() )
        {
            //ei.coord.z-= (float)(x - prev_x)/50;
            if(prev_x != 0)
            {

                ei.coord.x+=  (float)(x - prev_x)/10;
                ei.coord.z+=  (float)(y - prev_y)/10;
                orient.B.x=180.001f;
                orient.B.y=0.001f;
                orient.B.z=180.001f;
            }
            lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z)*sin(GR * orient.B.x);
            lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z);
            lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z)*cos(GR * orient.B.x);
            OnMove();
        }
    }
    //if(isReceiveInput)
    {
        prev_x = x;
        prev_y = y;
    }

//	cnt++;

    UpdateLookAt();
}

void Camera::Reset()
{
    ei.coord = Pnt(5,1,5);
    orient.A = Pnt(0.00,0.00,0.00);
    orient.B = Pnt(114.00f,0.52f,260.00f);
    //(0.00,0.00,0.00) - (114.00,0.52,260.00)
    //cosd.SetTopText("view reset", 50, num);

}

void Camera::key_pressed(int k)
{
    char    gstr[255];
    //sprintf(gstr, "key %d ch %c", k, (unsigned char)k);
    //prn(gstr);

    if( k == 32 )
        isReceiveInput = !isReceiveInput;

    if(!isReceiveInput) return;

    t_flags& flags = GetGFlags();
    CObjRegistry* MREG = CObjRegistry::getInstance();
    CSelection& gselection = CSelection::getInstance();

    ////cosd.SetTopText(gstr, 50, num);
    //COPY
    if(k==149)
    {
        if( gselection.objs.size()>0 )
            MREG->CopyObj(gselection.objs[0]);
    }

    //EXECUTE SCRIPT
    if(k == 4)
    {
#if 0
        bool dont=false;
        flags.bdisable_timer3 = 1;
        if(flags.bdisable_timer2==1)
            dont=true;
        flags.bdisable_timer2 = 1;
        flags.bdisable_timer1 = 1;
        //MU_Console_print("test");
        string c;
        c = myvm->GetReferences()+scripttext;
        myvm->execute(c);
        flags.bdisable_timer1 = 0;
        if(!dont)
            flags.bdisable_timer2 = 0;
        flags.bdisable_timer3 = 0;
        if(!flags.bdisable_timer2)
            sprintf_s(gstr,"executing script with update");
        else
            sprintf_s(gstr,"executing script no update ");
        //cosd.SetTopText(gstr, 50, num);
#endif
    }

    //DISABLE OBJ UPDATE
    if(k == 3)
    {
        flags.bdisable_timer2 = !flags.bdisable_timer2;
        sprintf_s(gstr,"update = %d", !flags.bdisable_timer2);
        //cosd.SetTopText(gstr, 50, num);

    }

    CInput* iinput = CInput::getInstance();
    // CLOSE
    if(iinput->keystates[233])
    {
        //SendMessage(pdlg->m_hWnd, WM_CLOSE, 0, 0);
    }


    // FOLLOW
    if(iinput->keystates['f']) //f
    {
        if(followtarget == 0)
        {
            followtarget = 1;
            FreeView();
            //cosd.SetTopText("followtarget", 50, num);
        }
        else
        {
            followtarget = 0;
            //cosd.SetTopText(strf(views, string(" "), view), 50, num);
        }
        looktarget = false;

    }

    //LOOK TARGET
    if(iinput->keystates['v']) //view
    {
        Object* obj;
        if( gselection.Count() > 0)
        {
            obj = gselection.objs[0];
            sprintf_s(gstr, "object %s selected", obj->name.c_str());
        }
        else
        {
            //default obj - veh01
            obj = MREG_FindObj("veh01");
/*
            sprintf(gstr, "no object selected");
            //cosd.SetTopText(gstr, 50, num);
*/
            //return;
        }


        //cosd.SetTopText(gstr, 50, num);
        mu_printf("object %s selected", obj->name.c_str());

        if (!obj) return;
        looktarget = true;
        target = obj;
        ei.coord = target->ei.coord;
        ei.coord.y = target->ei.coord.y + target->bounds.y ;

        return;
        isMouseEnabled=!isMouseEnabled;

        if(isMouseEnabled)
        {
            //ShowCursor(0);
        }
        else
        {
            //ShowCursor(1);
        }
        iinput->keystates[32]=false;
    }

    //OUT FOCUS
    if(iinput->keystates[27])//esc
    {
        isMouseEnabled=!isMouseEnabled;
        if(isMouseEnabled)
        {
            //ShowCursor(0);
        }
        else
        {
            prev_x = 0;
            prev_y = 0;
            //ShowCursor(1);
        }

    }

    //CAM SPEED
    if(iinput->keystates['+']||iinput->keystates[219])//+
    {
        speed+=1.0;
        sprintf_s(gstr, "camspeed = %f", speed);
        //cosd.SetTopText(gstr, 50, num);
    }

    //CAM SPEED
    if(iinput->keystates[221])//-
    {
        speed-=1.0;
        sprintf_s(gstr, "camspeed = %f", speed);
        //cosd.SetTopText(gstr, 100, num);
    }

    //TOP
    if(iinput->keystates['t'])//t
    {
//        sprintf(gstr, "top view", speed);
//        //cosd.SetTopText(gstr, 50, num);
//        ei.coord.y=20;
//        orient.B.y-=4;
//        OnMove();
        //PointsTest();
    }
    //BOTTOM
    if(iinput->keystates['b'])//t
    {
        sprintf_s(gstr, "top view", speed);
        //cosd.SetTopText(gstr, 50, num);
        ei.coord.y = -20;
        orient.B.y+=4;
        OnMove();
    }
    //RESET
    if(iinput->keystates['r'])//r
    {
        Reset();
        OnMove();
    }

    //VIEW
    if(iinput->keystates['c'])//c
    {
        //Reset();
        view++;
        if(view>1)
            view = 0;

        switch(view)
        {
        case 0:
            FreeView();
            break;
        case 1:
            TopView();
            break;
        case 2:
            LeftView();
            break;

        }

    }



}

void Camera::Slot1()
{

    char str[200];
    sprintf_s(str, "%dx%d(%f)", RES_X, RES_Y, aspect);
    //prn(str);

    //DoScan();

    ParamScreen& pscr = ParamScreen::getInstance();

    //следовать за обьектом
    if(followtarget)
    {
        if (target!=0)
        {
            ei.coord = target->ei.coord;
            ei.coord.y+=1;
        }
    }

    //смотреть заобьектом
    if(looktarget)
    {
            ei.coord = target->ei.coord;
            ei.coord.y = target->ei.coord.y + target->bounds.y ;
            lookAt = target->ei.coord;
            lookAt.y-=1;
            lookAt.x+=1;
            lookAt.z+=1;
    }


    // если прошло меньше секунды коррект шаг
    dt = (float)Timer::gtimer.fsec - tm;
    tm = Timer::gtimer.fsec;
    step = speed*dt;
    //step = 0.003 * dt;

    //переход от сферических к декартовым
    float phi, lambda;

    phi = GR * orient.B.z;
    lambda = GR * orient.B.x;

    //mu_printf("lambda=%1.1f phi=%1.1f",  lambda, phi );
    //iinput->mod = glutGetModifiers();
    //if(iinput->mod>0)
    //mu_printf("iinput->mod=%d",  iinput->mod);



    if(!isReceiveInput) return;

    // DOWN
    CInput* iinput = CInput::getInstance();
    if (iinput->keystates['z'])
    {
        //if(isMouseEnabled)
        {
            ei.coord.y = ei.coord.y - step;
            //return;
        }
        OnMove();
    }

    //UP
    if (iinput->keystates['q'])
    {
        //if(isMouseEnabled)
        {
            ei.coord.y = ei.coord.y + step;
            //return;
        }
        OnMove();
    }

    if(iinput->keystates['s']||iinput->keystates['S']) //s
    {
        //if(isMouseEnabled)
        if(view == 0)
        {
            ei.coord.x  = ei.coord.x + step*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
            ei.coord.y  = ei.coord.y - step*cos(GR * orient.B.z*sens);
            ei.coord.z =  ei.coord.z - step*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);
        }
        else if(view == 1)
        {
            ei.coord.z =  ei.coord.z - step;
        }

        lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
        lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
        lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);
        OnMove();

    }
    if(iinput->keystates['w']||iinput->keystates['W']) //w
    {
        //if(isMouseEnabled)
        if(view == 0)
        {
            this->ei.coord.x  = this->ei.coord.x - step*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
            this->ei.coord.y  = this->ei.coord.y + step*cos(GR * orient.B.z*sens);
            this->ei.coord.z =  this->ei.coord.z + step*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);
        }
        else if(view == 1)
        {
            ei.coord.z =  ei.coord.z + step;
        }

        lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
        lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
        lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);
        OnMove();
    }

    if(iinput->keystates['a']||iinput->keystates['A']) //a
    {
        //if(isMouseEnabled)
        {
            ei.coord.x  = ei.coord.x - step*sin(GR * (orient.B.x+90)*sens);
            ei.coord.z =  ei.coord.z + step*cos(GR * (orient.B.x+90)*sens);

            lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
            lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
            lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);
        }
        OnMove();
    }
    if(iinput->keystates['d']||iinput->keystates['D'])  //d
        //if(isMouseEnabled)
    {
        ei.coord.x  = ei.coord.x - step*sin(GR * (orient.B.x-90)*sens);
        ei.coord.z =  ei.coord.z + step*cos(GR * (orient.B.x-90)*sens);

        lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
        lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
        lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);
        OnMove();
    }

    //признак что было движение
    if(tlastmove>0)
    {
        if((Timer::gtimer.msec/1000 - tlastmove) < 1.0)
        {
            colx = RGB(0,0,200);
            //updi("cam_moving",1);
            //prn("CAMERA: moving");
        }
        else
        {
            colx = 255;
            //updi("cam_moving",0);
            isMoving = 1;
            //prn("CAMERA: notmoving");
        }
    }

    pscr.upd("camPos", ei.coord);

    //mu_printf("%x", main_if.selobj);

}

void Camera::UpdateLookAt()
{
    lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
    lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
    lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);
}

void Camera::OnMove()
{
    blink++;
    isMoving = 1;
    if(blink == 255)
        blink = 0;
    tlastmove = Timer::gtimer.msec / 1000;
    //prn("%f",tlastmove);

}

void Camera::click_lmb(int state)
{
    //mu_printf("Cam %s receive lmb", name.c_str());

    if(!isReceiveInput) return;

    CInput* iinput = CInput::getInstance();
    CMainInterface& main_if = CMainInterface::getInstance();
    CSelection& gselection = CSelection::getInstance();

    if( iinput->check_lmb() == true)	DoShot();

    if( main_if.isInPickPntMode )
    {
        if(state==1)
        {
            if(view == 1)
              main_if.MousePick();
        }

        return;
    }

    if(state==1)
        if( (main_if.isInSelectionMode) && (view == 1) ) // выбор только на виде top
        {

            //iinput->shift_on();
            if(main_if.selobj!=NULL)
            {
                //если зажат шифт
                if( iinput->shift_on() )
                {
                    gselection.ResetSelection();
                    gselection.AddObj(main_if.selobj);
                }
                else
                {

                    gselection.AddObj(main_if.selobj);
                }

                //::SendMessage(pdlg->m_hWnd, WM_USER+1112,0,0);
                mu_printf("object %s selected", main_if.selobj->name.c_str());
            }
            else
            {
                gselection.ResetSelection();
                mu_printf("nothing selected");
            }
            return;
        }

    if(main_if.isInMove)
    {
        isMouseEnabled = 0;

        //if(view == "top")
        {
            vector<Object*>::iterator obj;
            for(obj = gselection.objs.begin(); obj != gselection.objs.end(); obj++ )
            {
                (*obj)->ei.coord0 = (*obj)->ei.coord;
                    if((*obj)->isGroup)
                    {
                        Group* obj2 = (Group*)(*obj);
                        if(obj2!=NULL)
                        {
                            for(unsigned int i=0 ; i<obj2->objs.size(); i++)
                            {
                                //obj2->objs[i]->ei.coord0 = obj2->objs[i]->ei.coord;
/*
                                if(obj2->objs[i]->classname == "VPnt")
                                {
                                    obj2->objs[i]->ei.coord0 = *(VPnt*)obj2->objs[i];
                                    prn(obj2->objs[i]->ei.coord0.toStr().c_str());
                                }
*/
                            }
                        }
                    }

            }
        }
        return;
    }




        if(view == 0)
            if(state==1)
            {
                isMouseEnabled=!isMouseEnabled;
                if(isMouseEnabled)
                {
                    prev_x = 0;
                    prev_y = 0;
                    //ShowCursor(0);
                }
                else
                {
                    prev_x = 0;
                    prev_y = 0;
                    //ShowCursor(1);
                }
            }
}

void Camera::click_rmb(int state)
{
    if(!isReceiveInput) return;
    mu_printf("Cam %s receive rmb %d state", name.c_str(), state);
}

void Camera::click_mmb(int state)
{
    if(!isReceiveInput) return;
    mu_printf("Cam %s receive mmb %d", name.c_str(), state);
}

void Camera::mouse_wheel(int zDelta)
{
    if(!isReceiveInput) return;
    if(view == 1)
    {
        ei.coord.y-=(float)speed*zDelta/180;
        lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
        lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
        lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);

    }

    if(view == 2)
    {
        ei.coord.z-=(float)zDelta/180;
        lookAt.x  = ei.coord.x - dist*sin(GR * orient.B.z*sens)*sin(GR * orient.B.x*sens);
        lookAt.y  = ei.coord.y + dist*cos(GR * orient.B.z*sens);
        lookAt.z =  ei.coord.z + dist*sin(GR * orient.B.z*sens)*cos(GR * orient.B.x*sens);

    }

    if( ei.coord.y < 50 )
      drag_speed = 1;
    else
      drag_speed = 3;


}


void Camera::OnReceiveInput()
{

}

void Camera::OnSelect()
{
    string tmp = name + "-" + strf(views, string(" "), view);
    //cosd.SetTopText(tmp.c_str(), 40, num);
}

#define GET_A(i) ((i >> 24)&0xFF)
#define GET_R(i) ((i >> 16)&0xFF)
#define GET_G(i) ((i >> 8)&0xFF)
#define GET_B(i) ((i)&0xFF)

// выполняется когда камера смотрит на обьект
void Camera::DoScan()
{
#if 0
    if(view!=0) return;
    pscr.upd("teta",orient.B.z*sens);
    pscr.upd("phi",orient.B.x*sens);

    int  cp = get_centr_pixel(view);
    char str[30];
    sprintf_s(str,"cp=(%d %d %d %d)", GET_R(cp), GET_G(cp), GET_B(cp),GET_A(cp));
    pscr.upds("cp",str);
#endif

}
// выполняется когда нажата кнопка lmb
void Camera::DoShot()
{
#if 0
    Pnt p;
    t_scan_out out;
    float map_size = 0;
    float max_ray_len = 0;
    Pnt G0;

    // взять параметры пространства
    Object* space = MREG_FindObj("space");
    if(space!=0)
    {
        Pnt p = *(Pnt*)(space->GET_ATTR("map_size")->addr);
        map_size = p.x;
        max_ray_len = map_size * 2;
        G0 = *(Pnt*)(space->GET_ATTR("pos")->addr);
    }
    else
    {
        map_size = 1000;
        max_ray_len = map_size * 2;
    }

    //сделать скан
    Pnt p0 = ei.coord;
    int n = DO_SCAN(p0, orient.B.x*sens, orient.B.z*sens, map_size, max_ray_len, G0, out);

    //вывести результат в виде таблицы
    PrintScanOutput(out, n);
#endif
}

/*
#include <Wm5Vector3.h>
#include <Wm5Matrix3.h>
#include "drawings2d.h"
using namespace Wm5;
void Camera::create_pyr2(Vector3f v[5],float R, float H, float orientY, float orientZ, Pnt pos)
{   
    //for(uint i=0; i<5; i++) v[i] = Vector3f(p[i].x,p[i].y,p[i].z);
    v[0] = Vector3f(R,H,0);
    Matrix3f roty,rotx,rotz;
    roty.MakeRotation(Vector3f(0,1,0),GR*45);
    v[1] = v[0] *  roty;
    v[0] = v[1];
    roty.MakeRotation(Vector3f(0,1,0),GR*90);
    v[1] = v[0] *  roty;
    v[2] = v[1] *  roty;
    v[3] = v[2] *  roty;
    v[0] = v[3] *  roty;
    v[4] = Vector3f(0,0,0);
    roty.MakeRotation(Vector3f(0,1,0),GR*orientY);
    rotz.MakeRotation(Vector3f(0,0,1),GR*orientZ);
    for(uint i=0; i<5; i++)
    {
        v[i] = v[i] * rotz;
        v[i] = v[i] * roty;
        v[i] = v[i] + Vector3f(pos.x,pos.y,pos.z);
        draw_p(IntToStr(i),Pnt(v[i].X(),v[i].Y(),v[i].Z()),RGB(255,0,0),0);
        if(name=="cam02") CreatePoint(v[i],IntToStr(i));
    }
}
*/

void Camera::Render()
{
#if 0

    //------------- TEST DRAW CODE
    tvect vec;
    vec.A = ei.coord;
    vec.B = lookAt;
    //draw_v(vec,3,12,0.6,RGB(255,0,0));
    //------------- TEST DRAW CODE

    if(!isVisible)
        return;


/*	if(main_if.activeScreen != 1)
        return;
*/

    glPushMatrix();
//  glTranslatef ( x, y, z );
    glPointSize(3);


    glBegin(GL_POINTS);
    glVertex3f(ei.coord.x, ei.coord.y, ei.coord.z);
    glEnd();

    /*glBegin(GL_LINES);
    glVertex3f(ei.coord.x, ei.coord.y, ei.coord.z);
    glEnd();
    */
//	float alfa = vector_anglX(vec);

    glTranslatef( ei.coord.x, ei.coord.y, ei.coord.z);
    glRotatef(90-orient.B.x, 0, 1, 0);
    glRotatef(orient.B.z, 0, 0, 1);



    Pnt st[10];

    create_pyr(st, 0.2f, 0.5f);

    /*Vector3f p[10];
    for(int i=0; i < 10; i++)
        p[i] = Vector3f(st[i].x,st[i].y,st[i].z);
    create_pyr2(p, W, H, orient.B.x+90,orient.B.z,ei.coord);
    */
    connect_all_points(st, 0, 6);


//    if(isReceiveInput)
//    {
//        draw_p("l",st[1],RGB(255,0,0),0);
//        draw_p("r",st[2],RGB(255,0,0),0);
//        draw_p("r",st[3],RGB(0,123,0),0);
//        draw_p("l",st[0],RGB(0,123,0),0);
//    }
//    else
//    {
//        draw_p("l",st[1],RGB(0,0,0),0);
//        draw_p("r",st[2],RGB(0,0,0),0);
//        draw_p("r",st[3],RGB(0,0,0),0);
//        draw_p("l",st[0],RGB(0,0,0),0);
//    }

    //st[3].y+=2.5;

    draw_p(name.c_str(),st[4],RGB(0,0,colx),0);

    //updf("aX",alfa/GR);
    //updP("vec.B",vec.B);

    glPointSize(1);
    glPopMatrix();
#endif

}

