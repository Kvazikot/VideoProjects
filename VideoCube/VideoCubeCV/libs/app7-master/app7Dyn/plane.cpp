/**********************************************************************
 *  Project App6
 *
 *  plane.cpp
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
#include "../include/stdafx.h"
#include "../include/curve.h"
#include "../include/flags.h"
#include "../include/cinput.h"
#include "../include/timer.h"
#include "../include/vpnt.h"
#include "../include/cobjregistry.h"
#include "../include/geometry.h"
#include "../include/glcode.h"
#include "../include/gemobject.h"
#include "../include/matrix.h"
#include "../include/vector3d.h"
#include "../include/plane.h"
#include "../include/polet.h"
#include "../include/planesim.h"
#include "../include/camera.h"
#include "../include/planesim.h"
#include "../include/traj_gen.h"
#include <stdio.h>
#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glut.h"

string CPlane_polet = "vconst aconst anotconst";
extern TrajGen      traj_gen;
extern WPPolet		wppolet;
static ParamScreen& pscr = ParamScreen::getInstance();
static CObjRegistry& MREG = CObjRegistry::getInstance2();
//====================================================================
// <OBJECT>   </OBJECT>
//====================================================================

CPlane::CPlane()
: v(sim_in.v),RuleReturnTime(1),a(sim_in.a),trac(sim_in.trac),bSimulate(1),pos(ei.coord),angles(sim_in.angles),ruleRoll(sim_in.rule.ruleRoll), rulePitch(sim_in.rule.rulePitch), ruleRollW(sim_in.rule.RollW), rulePitchW(sim_in.rule.PitchW),
ruleTrottleUp(sim_in.rule.ruleTrottleUp), ruleTrottleDown(sim_in.rule.ruleTrottleDown)
{
	name = "plane01";
	classname = "CPlane";
	isCanRender = true;
	isEntety = true;
	isGroup = true;
	algon = 0;
	model = 0;
    bRenderOSD = 1;
    mode = 1;
    //QueryPerformanceCounter((LARGE_INTEGER *)&_t1);
    //QueryPerformanceCounter((LARGE_INTEGER *)&_t2);

	a		  =	 Pnt(0,0,0);
	v		  =	 Pnt(0,0,0);
    //angles	  =	 Angles(0.,0.,0.);
	trac	  =	 -1;
    rulePitch = -2.3f;
	rulePitchW = 1;
    ruleRoll  =  2.3f;
	ruleRollW =  1;
	fuel      =  200000;
	Hr		  = 0.0;


    cam = new Camera();
    sprintf(gstr, "planecam");
	cam->SetName(gstr);
    //cam->FreeView();
	cam->isCanRender = false;
    //cam->isVisible = true;
	cam->Declare();
    cam->zFar = 3000;
	cam->isNotSaving = true;
    MREG.AddObj(cam);

	la_pts = new LAPoints();
	la_pts->SetName("la_pts");
	la_pts->isNotSaving = true;
	la_pts->isCanRender = true;
	la_pts->Declare();
    //MREG.AddObj(la_pts);
	
	DefineGeometryPoints();

    bSimulate = false;

	ei.model = "AirbFuse";

	RGB2GL2(RGB(255,0,0),color);

    pscr.AddVar("fsec");
    pscr.AddVar("angles");
    pscr.AddVar("coord");
    pscr.AddVar("v");
    pscr.AddVar("w");

    CInput* iinput = CInput::getInstance();
    iinput->add_delegat(this);

	Connect();
}


LAPoints::LAPoints()
{
	isNotSaving = true;
	isCanRender = true;
	classname = "LAPoints";
	isEntety = true;
	isGroup = true;
	Connect();
}

void CPlane::DefineGeometryPoints()
{
	if(model == 0 ) return;

}

void LAPoints::Render()
{
	
	
}


extern void wppolet_init();
void CPlane::InitSimulation()
{
	InitPlaneSimulation(sim_in, sim_out);

    //wppolet_init();
    if(mode == 1)
        wppolet.Init();

    t = 0;
	sim_out.S = sim_in.S;
	trac = sim_in.trac;

    if(model!=0) model->rot = sim_in.angles;
	ei.coord = sim_in.S;
    if(model!=0) model->ei.coord = sim_in.S;
    Hr = sim_in.S.y;

	cam->ei.coord = ei.coord;
    cam->lookAt.x  = ei.coord.x - cam->dist*sin(GR * 90)*sin(GR * 10);
    cam->lookAt.y  = ei.coord.y + cam->dist*cos(GR * 90);
    cam->lookAt.z =  ei.coord.z + cam->dist*sin(GR * 90)*cos(GR * 10);
}
void CPlane::StopSimulation() 
{ 
	bSimulate = false; 

}

void CPlane::StartSimulation()
{
	bSimulate = true;  
}


void CPlane::InputControl()
{
    CInput* iinput = CInput::getInstance();

//----------------  управление тангажом
    if( iinput->keystates[70] )
    {
        prn("CPlane::InputControl тангаж+ %f",rulePitch);
		rulePitch+=rulePitchW;
    }
   if( iinput->keystates[72] )
    {
        prn("CPlane::InputControl тангаж- %f",rulePitch);
		rulePitch-=rulePitchW;
        //sprintf_s(gstr, "speed = %f kmh", mc2kmh(speed));
        //cosd.SetTopText(gstr, 100);
    }
    if( iinput->keystates[69] )
    {
        prn("CPlane::InputControl крен- %f", ruleRoll);
		ruleRoll-=ruleRollW;
    }
    if( iinput->keystates[71] )
    {
        prn("CPlane::InputControl крен+ %f", ruleRoll);
		ruleRoll+=ruleRollW;
    }
    if( iinput->keystates[49] )
    {
        prn("CPlane::InputControl тяга+ %f", trac);
        trac+=0.3f;
	}
    if( iinput->keystates[48] )
    {
        prn("CPlane::InputControl тяга- %f", trac);
        trac-=0.3f;
	}

			
	RuleReturn();
	

    
    
}
//автоматический возврат руля
void CPlane::RuleReturn()
{
	static double t0 = 0;
	static double t1 = 1;
	
    //ограничение углов рулей
	if(ruleRoll > 360) ruleRoll = ruleRoll - 360;
	if(rulePitch > 360) rulePitch = rulePitch - 360;
	if(ruleRoll < -360) ruleRoll = ruleRoll + 360;
	if(rulePitch < -360) rulePitch = rulePitch + 360;

	
    //время возврата
    t1 = Timer::gtimer.fsec;
	if( (t1 - t0) > RuleReturnTime )
	{
        t0 = Timer::gtimer.fsec;
		//prn("%f %f", t0, t1);
		ruleRoll = 0.0;
		rulePitch = 0.0;
	}

	
	
	if( ruleRoll > 0) ruleRoll-=ruleRoll*0.01; else ruleRoll+=ruleRoll*0.01;
	if( rulePitch > 0) rulePitch-=rulePitch*0.01; else rulePitch+=rulePitch*0.01;

	


  
}

void CPlane::DrawGrafics()
{

}

//obj update slot
void CPlane::Slot1()
{

	 if(model == 0 )
	 {
         if( LoadModel("AirbFuse") == 1 )
		 {
             ei.model = "AirbFuse";
			 model->isCanRender = true;
			 DefineGeometryPoints();	
			 InitSimulation();             
             model->ei.coord = sim_out.S;
			 prn("CPlane loaded model %s", ei.model.c_str());
		 }
		 return;
	 }
	 //model->rot =  rot0;
	 model->rot0 = rot0; 
	 //return;

	 
     //применяем управление
    InputControl();

    //возврат рулей
   	RuleReturn();

    //Вывод на графики
	DrawGrafics();

     // Делаем цикл симуляции
    _t2 = Timer::gtimer.fsec;
     double _time=(double)(_t2-_t1);
     dt = _time;
     if(dt > 1) dt = 0.000000000001;
     if(dt < 0) dt = 0.000000000001;


	 
	 if( bSimulate )
	 {
         // ------------------------ ВХОД ---------------------

		sim_in.mass = mass;
		sim_in.angles = sim_out.angles;
        sim_in.ang = sim_out.ang;

        if(mode == 0)
            traj_gen.PlaneSimulation(dt,sim_in, sim_out);


        if( mode == 1 )
		{
            //t_flags& flags = GetGFlags();
            // симуляция по узловым точкам
            //if( flags.bFixedStepSim )                dt = vt.getd("dt");
            wppolet.PlaneSimulation(dt, sim_in, sim_out);
		}

        if( mode == 3 ) // симуляция полета с применением симулятора planesim и рулевым управлением
			PlaneSimulation(dt, sim_in, sim_out);	 


        // Присваиваем положение и углы модели
        // ------------------------ ВЫХОД

         //angles = sim_out.angles;
         //angles.roll = 0;
         //angles.yaw = 0;
         //angles.pitch = 0;
         model->rot = sim_out.ang;
         model->model_matrix.setToIdentity();
         model->model_matrix.translate(sim_out.S.x, sim_out.S.y, sim_out.S.z);
         model->model_matrix.rotate(180-sim_out.angles.yaw,0,1,0);
         model->model_matrix.rotate(-sim_out.angles.roll,1,0,0);
         model->model_matrix.rotate(sim_out.angles.pitch,0,0,1);
         //model->model_matrix.rotate( 90, 0, 1, 0);
         model->model_matrix.scale( model->ei.scale.x,  model->ei.scale.y,  model->ei.scale.z);

         ei.coord = sim_out.S;
		 model->ei.coord = sim_out.S;
		 Hr = sim_out.S.y;
         //DefineGeometryPoints(); // пересчитать точки привязки
         Write2SharedMemory(sim_out);


	 }

     _t1 = Timer::gtimer.fsec;

    //апдейт положения камеры
	cam->ei.coord = ei.coord;
    //cam->ei.coord.y+=30;
    //cam->lookAt = 64.232529,233.131149,-51.090717
    cam->orient.B = Pnt(angles.y-90,90.000000,133.000000);
    cam->FreeView();

}

void CPlane::SetModelPos()
{
	if( model != 0 )
	{
		model->ei.coord = pos;
		model->rot = angles;
	}
}

int CPlane::LoadModel(string name)
{
    static int cnt=0;

    if(cnt > 10) return -1;

   CObjRegistry& MREG = CObjRegistry::getInstance2();
    model = (GemObject*)MREG.DeepFindObj(name);
    if(model == NULL)
	{
        //model->renderMode = 0;
        prn("CPlane::unable load model %s", name.c_str());
        ei.model = name;
        cnt++;

		return -1;
	}
    else
    {
        model->ei.coord = Pnt(100,0,0);
    }
/*
     CObjRegistry& MREG = CObjRegistry::getInstance2();
     Group* group = (Group*)MREG.FindObj("TTT");
     if(group!=0)
     {
         //group->FindObj("AirbFuse");
         model = (GemObject*)group->FindObj("AirbFuse");
         if(model!=0)
             return 1;
     }
*/

    return 0;
}
	

void CPlane::Write2SharedMemory(PlaneSim_Output& sim_out)
{
     CObjRegistry& MREG = CObjRegistry::getInstance2();
}


void CPlane::OnSelect()
{

	//flags.bNeedUpdObjectList = true;
}

void CPlane::OnUnselect()
{
	prn("CPlane::OnUnselect()");
	//for(int i=0; i<pts.size(); i++)	
	//  pts[i]->isHidden = 2;
	//flags.bNeedUpdObjectList = true;
}

void CPlane::RenderOSD()
{
//	if(main_if.activeScreen != 1) return;
	if(!bRenderOSD) return;
    if(!bSimulate) return;
/*	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, RES_X, 0.0, RES_Y);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
*/
	
    sprintf(gstr, "gtimer->t=%f СЃРµРє. dt = %f ", Timer::gtimer.fsec, dt);
    pscr.paramMap["fsec"] = gstr;
    //glutPrint2(1, RES_Y-20, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);
    //cosd.strings[0] = gstr;
    sprintf(gstr, "pitch %3.3f(%3.3f) roll %3.3f(%3.3f) yaw %3.3f", angles.pitch, rulePitch, angles.roll, ruleRoll, angles.yaw);
    pscr.paramMap["angles"] = gstr;
    //cosd.strings[1] = gstr;
    //glutPrint2(1, RES_Y-40, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);
    sprintf(gstr, "x=%3.3f y=%3.3f z=%3.3f", ei.coord.x, ei.coord.y, ei.coord.z);
    pscr.paramMap["coord"] = gstr;
    //cosd.strings[2] = gstr;
    //glutPrint2(1, RES_Y-60, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);
    sprintf(gstr, "vx=%3.3f vy=%3.3f vz=%3.3f  Hr %3.3f", sim_out.v.x, sim_out.v.y, sim_out.v.z,Hr);
    pscr.paramMap["v"] = gstr;
    sprintf(gstr, "we=%3.3f wn=%3.3f wh=%3.3f", sim_out.w.x, sim_out.w.y, sim_out.w.z,Hr);
    pscr.paramMap["w"] = gstr;

    //cosd.strings[3] = gstr;
    //glutPrint2(1, RES_Y-80, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);

/*
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
*/
}


void CPlane::Render(QOpenGLShaderProgram& program)
{
    Pnt p1[9];
	glPushMatrix();
	
	if(!isCanRender)
		return;

    if(model != 0)		model->Render(program);

    glTranslatef ( 0, 0, 0 );
	RenderOSD();	
    wppolet.RenderTrajectory();
	//glRotatef(angle.x, angle.y, angle.z,1);
	static float cl=0;		
	if(isSelected==2)
	{
		glEnable(GL_COLOR_MATERIAL);	
		glColor4f(0,0,1,cl);
		glPointSize(3);
		

		glPointSize(1);
		glDisable(GL_COLOR_MATERIAL);
	}

	
	glPopMatrix();
	glDisable (GL_LINE_SMOOTH);          // Enable Anti-Aliasing
}




//object inspector event
void CPlane::OnAttrChange(t_atr& a)
{
	
	prn("CPlane::OnAttrChange %s", a.name.c_str());

	if( a.name == "polet" )
		algon = a.data;


}


