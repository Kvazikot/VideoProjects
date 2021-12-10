/**********************************************************************
 *  Project App6
 *
 *  vpnt.cpp
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

//REALISATION CPP
#include "../include/print.h"
#include "../include/vpnt.h"
#include "../include/glcode.h"
#include "../include/cmaininterface.h"
#include "../include/cinput.h"
#include "../include/cobjregistry.h"

string waypoint_attrs="isWaipoint ti We kurs p";

//====================================================================
// <OBJECT> ¬»«”¿À»«»–”≈Ã¿ﬂ “Œ◊ ¿  </OBJECT>
//====================================================================


VPnt::VPnt()
{
	//RenderObject();
	classname = "VPnt";
	isCanRender = true;
	isEntety = true;
	isNotSaving = true;
	parent = this;
	//is
    RGB2GL2(RGB(0,0,255),color);
	
	kurs = Pnt(0,0,0);
	isWaipoint = false;
	isPointOfCurve = false;
	ShowKurs = false;
    ShowName = true;
	segment = "0 45 90 135 180 225 270 315 360";
	isegment = 0;
	mass = 0;
	C0 = Pnt(0,0,0);
    R = 0.5;
	k = 15;
    cl = 0;
	bounds = Pnt(R*2,R*2,R*2);
	scale = Pnt(1,1,1);
    CInput* iinput = CInput::getInstance();
    iinput->add_delegat(this);
}

VPnt::VPnt(float r, float g, float b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = 1;
}

bool VPnt::isDummy()
{
	if(name=="dpnt1")
		return true;
	if(name=="dpnt2")
		return true;
	return false;
}

void VPnt::OnDrag(float x, float y, float z)
{
    Object::OnDrag(x,y,z);
    //*(Pnt*)this = ei.coord;
	//prn(groupname.c_str());
    Object* o = MREG_FindObj(groupname);
	if(o!=NULL)
		o->m_Updated = true;

}



void VPnt::SetMode(bool isPointOfCurve, bool isWaipoint)
{
    isPointOfCurve=0;
	if( isWaipoint )
	{
		HideAllAtrs();
		ShowAtrs(waypoint_attrs);
	}
	else
		ShowAllAtrs();
/*	if(!isPointOfCurve)
	{
		ATTR_HIDE(kp);
		ATTR_HIDE(k);
		ATTR_HIDE(segment);
	}
	else
	{
		ATTR_UNHIDE(kp);
		ATTR_UNHIDE(k);
		ATTR_UNHIDE(segment);	
	}

	if(!isWaipoint)
	{
		ATTR_HIDE(kurs);
		ATTR_HIDE(angle_type);
		ATTR_HIDE(euler_order);
	}
	else
	{
		ATTR_UNHIDE(kurs);
		ATTR_UNHIDE(angle_type);
		ATTR_UNHIDE(euler_order);	
	}
	flags.bNeedReinitObjInspector = true;
*/
}

void VPnt::SyncDummy()
{
	if(isDummy()) return;
	if(isDummy()) return;
    VPnt* dp1 = (VPnt*)MREG_FindObj("dpnt1");
	if((dp1==NULL) || (dp1==NULL))
		return;
	*(Pnt*)dp1 = kp;
	dp1->ei.coord = kp;
	dp1->isHidden = 0;
	dp1->groupname = groupname;
	dp1->parent = this;

}

void VPnt::OnSelect()
{
	isHidden = 0;
	if(!isDummy())
	  SyncDummy();

	//flags.bNeedUpdObjectList = true;
}

void VPnt::OnUnselect()
{
//	isHidden = 2;
		

}

void VPnt::Scale(float,float,float)
{
	
}

void VPnt::Rotate(float,float,float)
{
	
}


void VPnt::Render(QOpenGLShaderProgram& program)
{
	if(!isCanRender)
		return;

	if( isDummy() ) return;

    if(isSelected>0)
	{
        //GLfloat color[] = {1-cl,0,cl,1};
        color[0] = 1-cl;
        color[1] = 0;
        color[2] = cl;
        color[3] = 1;
		cl=cl+0.1;
		if(cl>1.0) cl=0;
        if( ShowName )
          draw_p_gl(program, name, *(Pnt*)this, color, ShowBounds, 0.5);
        else
          draw_p_gl(program, "", *(Pnt*)this, color, ShowBounds, 0.5);
		
	}
	else
	{
        if( ShowName )
          draw_p_gl(program, name, *(Pnt*)this, color, ShowBounds, 0.5);
        else
           draw_p_gl(program, "", *(Pnt*)this, color, ShowBounds, 0.5);
	}
	
	return;
}

void VPnt::SetAngle(float angle)
{
	Pnt P1 = Pnt(x,y,z);
	kp.x =  P1.x + k * cos(GR*angle);
	kp.z =  P1.z + k * sin(GR*angle);
}

//object inspector event
void VPnt::OnAttrChange(t_atr& a)
{
    prn("VPnt::OnAttrChange %s", a.name.c_str());
	if(  (a.name == "isWaipoint") || (a.name == "isPointOfCurve")  )
	{
		SetMode(isPointOfCurve, isWaipoint);
		return;
	}

	if(  (a.name == "p")  )
	{
		ei.coord = *(Pnt*)this;

        Object* o = (Object*)GET_OBJECT((char*)groupname.c_str());
        if(o!=0)
            o->m_Updated = true;
		return;
	}
	if( (a.name == "kp") )
	{
		SyncDummy();
		Object* o = (Object*)GET_OBJECT((char*)groupname.c_str());
		if(o!=0)
			o->m_Updated = true;
		return;	
	}
	if( (a.name == "k")  )
	{
		Pnt P1 = Pnt(x,y,z);
		float angle = isegment*45;
		kp.x =  P1.x + k * cos(GR*angle);
		kp.z =  P1.z + k * sin(GR*angle);

		Object* o = (Object*)GET_OBJECT((char*)groupname.c_str());
		if(o!=0)
			o->m_Updated = true;
		return;
	}
	if(  (a.name == "segment")  )
	{
		isegment = a.data;

		//‚˚·Ó ÔÂ‰ÛÒÚ‡ÌÓ‚ÎÂÌÌ˚ı ‚‡Ë‡ÌÚÓ‚
		Pnt P1 = Pnt(x,y,z);
		float angle = isegment*45;
		kp.x =  P1.x + k * cos(GR*angle);
		kp.z =  P1.z + k * sin(GR*angle);


		Object* o = (Object*)GET_OBJECT((char*)groupname.c_str());
		if(o!=0)
			o->m_Updated = true;
		return;
	}
	
	
}

//obj update slot
void VPnt::Slot1()
{
	
	if(m_Updated)
	{
		//bounds = Pnt(0,0,0);
		m_Updated = false;
        //prn("%s accept new params %s", name.c_str(), bounds.toStr().c_str());
	}
	
	
	
	
}


void VPnt::mouse_move(int x, int y)
{
    x=y;
    CMainInterface& main_if = CMainInterface::getInstance();
    CInput* iinput = CInput::getInstance();
    if(main_if.isInMove)
    if(iinput->check_lmb())
    {
        if(isDummy())
        {
            parent->kp = ei.coord;
            //prn("dummy pos %d ", ei.coord.x);
            /*Object* o = MREG.FindObj(groupname);
            if(o!=NULL)
                o->m_Updated = true;
                */


        }

        if(isSelected)
        {
            ei.coord.z = iinput->vz;
            ei.coord.x = iinput->vx;
            *(Pnt*)this = ei.coord;
        }

    }



}
