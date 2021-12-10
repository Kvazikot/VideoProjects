/**********************************************************************
 *  Project App6
 *
 *  player.cpp
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

#include "../include/stdafx.h"
#include "../include/player.h"
#include "../include/cobjregistry.h"
#include "../include/camera.h"
#include "../include/vector3d.h"
#include "../include/cinput.h"
#include "../include/print.h"


Player::Player()
{
    timer.start();
    pos = Pnt(0,0,0);
    rot = Pnt(1,0,0);
    speed = 2;
    cam_dist = 0.5;
}

void Player::SetFPCamera()
{
    static Camera* cam = 0;
    static Camera* cam3rd = 0;
    CObjRegistry& MREG = CObjRegistry::getInstance2();

    if(cam == 0)
    {
        cam = (Camera*)MREG.FindObj("cam02");
        cam3rd = (Camera*)MREG.FindObj("cam_3person");
    }
    else
    {
        cam->ei.coord = pos;
        cam->ei.coord.y+=0.3f;
        cam->isReceiveInput = true;
        cam->UpdateLookAt();
        rot = (cam->lookAt - cam->ei.coord);
        float t = -cam_dist;
        CVector3 P3 = CVector3(cam->ei.coord)+t*CVector3(rot);
        if(cam3rd!=0)
        {
            cam3rd->ei.coord = P3.toPnt();
            cam3rd->lookAt = cam->lookAt;
            cam3rd->isReceiveInput = false;
        }
        return;
    }

}

void Player::SetControlShema(unsigned char f, unsigned char b,
                             unsigned char r, unsigned char l)
{
    control.ForwardKey = f;
    control.BackwardKey = b;
    control.LeftKey = l;
    control.RightKey = r;
}

void Player::ProcessInput()
{
    CInput* iinput = CInput::getInstance();

    if(iinput->keystates[control.ForwardKey])
    {
        CVector3 pos_new = CVector3(pos)+step*CVector3(rot);
        pos.x = pos_new.x;
        pos.z = pos_new.z;
    }
    if(iinput->keystates[control.BackwardKey])
    {
        CVector3 pos_new = CVector3(pos)-step*CVector3(rot);
        pos.x = pos_new.x;
        pos.z = pos_new.z;
    }

    if(iinput->keystates[control.LeftKey])
    {
        CVector3 pos_new = CVector3(pos)-step*CVector3(rot).rotateAboutY((float)M_PI_2);
        pos.x = pos_new.x;
        pos.z = pos_new.z;

        //pos.x  = pos.x - step*sin((kurs+1.5707));
        //pos.z  =  pos.z + step*cos((kurs+1.5707));
    }
    if(iinput->keystates[control.RightKey])
     {
        CVector3 pos_new = CVector3(pos)+step*CVector3(rot).rotateAboutY((float)M_PI_2);
        pos.x = pos_new.x;
        pos.z = pos_new.z;
    }

}

void Player::Slot1()
{
    // Делаем цикл симуляции
    dt=(double)(timer.elapsed())/(double)(1000.);
    step = speed * dt;
    ProcessInput();
    SetFPCamera();
    //prn("cur_frame=%d",cur_frame);
    timer.restart();
}
