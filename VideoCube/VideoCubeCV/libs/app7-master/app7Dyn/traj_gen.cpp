/**********************************************************************
 *  Project App6
 *
 *  traj_gen.cpp
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
#include "../include/geometry.h"
#include "../include/print.h"
#include "../include/interp.h"
#include "../include/str_utils.h"
#include <algorithm>
#include <string> 
#include "../include/traj_gen.h"
#include "../include/var_parse.h"
#include "../include/planesim.h"
#include "../include/law.h"
#include "../include/random_staff.h"
#include "../include/param_screen_bcb.h"
#include "../include/assoc_map.h"

#define PLANEGEN_DEBUG_INFO_LEVEL 5
#define DBG(level, fmt, args) do{ if ((level) <= PLANEGEN_DEBUG_INFO_LEVEL) gprintf(fmt,##args); } while(0)


// генератор траекторий ГЛОБАЛЬНЫЙ
AssotiationMap ass_map;
TrajGen traj_gen;
extern VarParser varparser;
static ParamScreen& vt = ParamScreen::getVtInstance();
extern char gstr3[1000000];



// инициализация генератора траеторий
void TrajGen::Init()
{
        vt = varparser.vt;

        vt.PrintMap();
        string s = vt.GetMap();
        prn(s.c_str());

		static bool ft = 1;
		ResolutionRoll = vt.geti("ResolutionRoll");
		ResolutionYaw = vt.geti("ResolutionYaw");
		ResolutionPitch = vt.geti("ResolutionPitch");
		t_rascheta = vt.getd("t_rascheta");
		Ninterpol = 30000;
		Ndef = vt.geti("N");;
		f_dat = vt.geti("f_dat");
		if(f_dat == 0) f_dat = 200;
		
		Ntest = vt.geti("Ntest");
		//gprintf(vt.GetMap().c_str());
		pos0 = Pnt(0,0,0);
		bIsCanRender = false;

		//генерация законов для 4 тестов чтобы проверить иниц. законов
		for(int i=0; i<5; i++)
			Test(i);

		// Выбор закона для конкретного теста
		Test(Ntest);
		//PrintInfo();

/*		spline_roll.SetN(ResolutionRoll);
	ResolutionRoll = pscr.geti("N");		// разрешение сигнала для крена (в точках)
	gentime = pscr.getd("gentime");
	gprintf("gentime %f",gentime);	
	pscr.BeginTable("Rolllaw");
		rolllaw.Amin = pscr.getd("Amin");
		rolllaw.Amax = pscr.getd("Amax");
		rolllaw.T = pscr.getd("T");
		rolllaw.type = pscr.gets("type");
	pscr.EndTable("Rolllaw");
*/		
}

void TrajGen::CreateAssociations()
{
/*
        //ass_map.DeleteAll();
        a = ass_map.NewAssotiation("ax");
        a->AddN(5, axLawArr, &alaw[0], &gdlg->AccelSeries[0], &spline_a[0], &gdlg->m_ChartAccel );
        a = ass_map.NewAssotiation("ay");
        a->AddN(5, ayLawArr, &alaw[1], &gdlg->AccelSeries[1], &spline_a[1], &gdlg->m_ChartAccel );
        a = ass_map.NewAssotiation("az");
        a->AddN(5, azLawArr, &alaw[2], &gdlg->AccelSeries[2], &spline_a[2], &gdlg->m_ChartAccel );
        a = ass_map.NewAssotiation("vx");
        a->AddN(5, vxLawArr, &vlaw[0], &gdlg->SpeedSeries[0], &spline_v[0], &gdlg->m_ChartSpeed );
        a = ass_map.NewAssotiation("vy");
        a->AddN(5, vyLawArr, &vlaw[1], &gdlg->SpeedSeries[1], &spline_v[1], &gdlg->m_ChartSpeed );
        a = ass_map.NewAssotiation("vz");
        a->AddN(5, vzLawArr, &vlaw[2], &gdlg->SpeedSeries[2], &spline_v[2], &gdlg->m_ChartSpeed );
        a = ass_map.NewAssotiation("roll");
        a->AddN(5, RollLawArr, &rolllaw, &gdlg->AnglesSeries[2], &spline_roll, &gdlg->m_ChartAngles );
        a = ass_map.NewAssotiation("yaw");
        a->AddN(5, YawLawArr, &yawlaw, &gdlg->AnglesSeries[0], &spline_yaw, &gdlg->m_ChartAngles );
        a = ass_map.NewAssotiation("pitch");
        a->AddN(5, PitchLawArr, &pitchlaw, &gdlg->AnglesSeries[1], &spline_pitch, &gdlg->m_ChartAngles );
*/

        //gprintf( ass_map.PrintInfo().c_str() );

        //Law* law = (Law*)ass_map.FindAssociation("azlaw").ptrs[0];
        //Law* law = ass_map.Assign<Law>("azlaw",0);
        //Spline* spline = ass_map.Assign<Spline>("azlaw",2);


}



//сбросить законы
void TrajGen::Reset()
{
	rolllaw.Reset();
	pitchlaw.Reset();
	yawlaw.Reset();
	vlaw[0].Reset();
	vlaw[1].Reset();
	vlaw[2].Reset();
	alaw[0].Reset();
	alaw[1].Reset();
	alaw[2].Reset();
	pos0 = Pnt(0,0,0);
	pos0 = vt.getP("pos0");
    tsim = 0;

	//шкала времени
    set_timedomain(Time_domain, t_rascheta/Ndef, Ndef);		

	memset(RollLawArr,0,200000);
	memset(YawLawArr,0,200000);
	memset(PitchLawArr,0,200000);
	memset(vxLawArr,0,200000);
	memset(vyLawArr,0,200000);
	memset(vzLawArr,0,200000);
	memset(axLawArr,0,200000);
	memset(ayLawArr,0,200000);
	memset(ayLawArr,0,200000);
	memset(azLawArr,0,200000);

	
	ResetSpline(spline_roll,  RollLawArr);
	ResetSpline(spline_yaw,   RollLawArr);
	ResetSpline(spline_pitch, RollLawArr);
	ResetSpline(spline_a[0],  RollLawArr);
	ResetSpline(spline_a[1],  RollLawArr);
	ResetSpline(spline_a[2],  RollLawArr);
	ResetSpline(spline_v[0],  RollLawArr);
	ResetSpline(spline_v[1],  RollLawArr);
	ResetSpline(spline_v[2],  RollLawArr);

}

void TrajGen::ResetSpline(Spline& spl, double* arr)
{
		//генерируем массивы коэффициентов сплайна
		spl.SetN(Ndef);
		spl.SetXY(Time_domain, arr);
		spl.Calc();
}



//парсировать законы из окна GraficsDialog
void TrajGen::Test(int Ntest)						
{
	char table_key[255];	
	{
		Reset();
		rolllaw.Res = ResolutionRoll+1;  // могут перекрываться переменной "Res" !
		pitchlaw.Res = ResolutionPitch+1;
		yawlaw.Res = ResolutionYaw+1;
		sprintf(table_key,"Rolllaw%d",Ntest);	SetLawParams(rolllaw, table_key); 
		sprintf(table_key,"Pitchlaw%d",Ntest);	SetLawParams(pitchlaw, table_key);
		sprintf(table_key,"Yawlaw%d",Ntest);	SetLawParams(yawlaw, table_key);
		sprintf(table_key,"VXlaw%d",Ntest);		SetLawParams(vlaw[0], table_key);
		sprintf(table_key,"VYlaw%d",Ntest);		SetLawParams(vlaw[1], table_key);
		sprintf(table_key,"VZlaw%d",Ntest);		SetLawParams(vlaw[2], table_key);
		sprintf(table_key,"AXlaw%d",Ntest);		SetLawParams(alaw[0], table_key);
		sprintf(table_key,"AYlaw%d",Ntest);		SetLawParams(alaw[1], table_key);
		sprintf(table_key,"AZlaw%d",Ntest);		SetLawParams(alaw[2], table_key);

	}
}


// Обновить параметры закона из таблицы
void TrajGen::UpdateLaw(string table_key)
{
		string key;
		int i=table_key.find("law");
		key = table_key.substr(0,i);

        //find law object by table_key
        double* d = ass_map.Assign<double>(key,0);
        Law* plaw = ass_map.Assign<Law>(key,1);
        Spline* spline = ass_map.Assign<Spline>(key,3);

		if( plaw==0 ) return;
		
		SetLawParams(*plaw, table_key); 
		ApplyDefaults();

		GenerateLawInterp(d, plaw->Res, *spline, t_rascheta, *plaw);	

}

// Обновить параметры закона из таблицы
void TrajGen::ReCalcSpline(string table_key)
{
#ifndef DISABLE_TRAJGEN
		string key;
		int i=table_key.find("law");
		key = table_key.substr(0,i);
		//find law object by table_key
		double* d = ass_map.Assign<double>(key,0);
		Law* plaw = ass_map.Assign<Law>(key,1);
		Spline* spline = ass_map.Assign<Spline>(key,3);
		if( plaw==0 ) return;
		GenerateLawInterp(d, plaw->Res, *spline, t_rascheta, *plaw);	
#endif
}

//нарисовать на соотв графике закон по ключу в таблице
void TrajGen::DrawLaw(string table_key)
{
/*
#ifndef DISABLE_TRAJGEN	
	CSeries* series=0;
	Spline*	 spline=0;
	double X,Y,t;
	double dt;
	string clean_key;

	int i=table_key.find("law");
	clean_key = table_key.substr(0,i);
	series = ass_map.Assign<CSeries>(clean_key,2);
	spline = ass_map.Assign<Spline>(clean_key,3);

	if((series==0) || (spline==0)) return;
	
	f_dat+=0.0000000001;
	dt = (double) 1. / f_dat;	
	series->Clear();
	t = 0;		
	while( t < t_rascheta)
	{
		series->AddXY(t,spline->GetF(t),"",1);
		t+=dt;
		
	}
#endif
*/
}

//парсинг строки закона, парсинг правил. Правила заносятся в таблицу vt
void TrajGen::SetLawParams(Law& law, string table_key)
{
	//defaults
	vt.EndTable();
	law.xmin = 0;
	law.xmax = t_rascheta;
	if( vt.hasKey("xmin") )
	{
		law.xmin = vt.getd("xmin");
		law.xmax = vt.getd("xmax");
		law.bDoSplit = true;
		law.split_level = vt.getd("split_level");
	}
	if( !vt.hasTable(table_key) ) return;
	vt.BeginTable( table_key );
	law.name = table_key;
	law.type = vt.gets("type");
	law.Amax = vt.getd("Amax");
		law.Amin = vt.getd("Amin");
	law.T = vt.getd("T");
	law.phi0 = vt.getd("phi0");
	law.tmax = vt.getd("tmax");
	law.scale = vt.getd("scale");
	law.t0 = vt.getd("t0");	
	if( vt.hasKey("xmin") )
	{
		law.xmin = vt.getd("xmin");	
		law.split_level = vt.getd("split_level");	
		law.bDoSplit = true;
	}
	if( vt.hasKey("xmax") )
	{
		law.xmax = vt.getd("xmax");	
		law.split_level = vt.getd("split_level");	
		law.bDoSplit = true;
	}

	if( vt.hasKey("Res") )
		law.Res = vt.getd("Res");
	else
		law.Res = -1;

	string pts_s = vt.gets("PTS_XY");
/*    if( pts_s != "")
    {
        VarParser vp;
        vp.ParsePoints2d(pts_s,law.parr);
    }
*/

    if( pts_s != "")
	{
		pts_s = StrRep(pts_s,"[","");
		pts_s = StrRep(pts_s,"]","");
		vector<string> elm = StrSplitE(pts_s,",",false);
		law.parr.clear();
        for(int i=0; i<(int)elm.size()-1; i+=2)
		{
			PntEx p = Pnt(0,0,0);
			if((elm[i]=="") || (elm[i+1]=="") )
				continue;
			sscanf(elm[i].c_str(),"%f",&p.x);
			sscanf(elm[i+1].c_str(),"%f",&p.y);					
			law.parr.push_back(p);
		}
	}

	vt.EndTable();
}


void TrajGen::ApplyDefaults()
{

	// присваиваем дефолтное разрешение функций, если не определено
	if( rolllaw.Res == -1 )		rolllaw.Res = ResolutionRoll;
	if( pitchlaw.Res == -1 )	pitchlaw.Res = ResolutionPitch;
	if( yawlaw.Res == -1 )		yawlaw.Res = ResolutionYaw;

	if( alaw[0].Res == -1 )		alaw[0].Res = Ndef;
	if( alaw[1].Res == -1 )		alaw[1].Res = Ndef;
	if( alaw[2].Res == -1 )		alaw[2].Res = Ndef;

	if( vlaw[0].Res == -1 )		vlaw[0].Res = Ndef;
	if( vlaw[1].Res == -1 )		vlaw[1].Res = Ndef;
	if( vlaw[2].Res == -1 )		vlaw[2].Res = Ndef;
}

// генерация траеторий
void TrajGen::GenerateLaws()
{
#ifndef DISABLE_TRAJGEN
	bIsCanRender = false;
	ApplyDefaults();

	GenerateLawInterp(RollLawArr, rolllaw.Res, spline_roll, t_rascheta, rolllaw);	
	GenerateLawInterp(PitchLawArr, pitchlaw.Res, spline_pitch, t_rascheta, pitchlaw);	
	GenerateLawInterp(YawLawArr, yawlaw.Res, spline_yaw, t_rascheta, yawlaw);	
	GenerateLawInterp(axLawArr, alaw[0].Res, spline_a[0], t_rascheta, alaw[0]);	
	GenerateLawInterp(ayLawArr, alaw[1].Res, spline_a[1], t_rascheta, alaw[1]);	
	GenerateLawInterp(azLawArr, alaw[2].Res, spline_a[2], t_rascheta, alaw[2]);	
	GenerateLawInterp(vxLawArr, vlaw[0].Res, spline_v[0], t_rascheta, vlaw[0]);	
	GenerateLawInterp(vyLawArr, vlaw[1].Res, spline_v[1], t_rascheta, vlaw[1]);	
	GenerateLawInterp(vzLawArr, vlaw[2].Res, spline_v[2], t_rascheta, vlaw[2]);
    //t0_matsim = gtimer.sec; // время начала инициализации

    //GenVertexes(1./f_dat);		// построить opengl траекторию
    CalcCoordSplines();			// сгенерировать сплайны для координат (нужно для датчиков)

	bIsCanRender = true;
#endif
}

//Дать идеальные данные на заданное время
/*void TrajGen::GetData(SensorData& data, double t, double f)
{
	double dt = 1./f;
	data.a = Pnt( Getax(t), Getay(t), Getaz(t));
	data.v = Pnt( Getvx(t), Getvy(t), Getvz(t));
	data.angles = Pnt( GetYaw(t),GetRoll(t), GetPitch(t) );
	//data.S = Pnt( CalcX(dt, t), CalcY(dt, t), CalcZ(dt, t));
	data.S = Pnt( Getx(t), Gety(t), Getz(t));
	data.rule.rulePitch = iinput.LY*0.4; 
	data.rule.ruleRoll =  iinput.RX*0.6;
}
*/

//генерация сплайнов координат
void TrajGen::CalcCoordSplines()
{
	int i=0;
	double t=0;
	double dt = 1./f_dat;

	// расчет исходных точек траектории
	pos0 = vt.getP("pos0");
	while( t < t_rascheta)
	{
		XDATA[i] = CalcX(dt, t);		
		YDATA[i] = CalcY(dt, t);		
		ZDATA[i] = CalcZ(dt, t);
		t+=dt; i++;
	}

	
	//генерируем массивы коэффициентов сплайна координат
	spline_x.bDoSplit = 0; spline_y.bDoSplit = 0; spline_z.bDoSplit = 0;
	spline_x.SetN(i);
	set_timedomain(Time_domain, t_rascheta/i, i);	
	spline_x.SetXY(Time_domain, XDATA);
	spline_x.Calc();

	spline_y.SetN(i);
	set_timedomain(Time_domain, t_rascheta/i, i);	
	spline_y.SetXY(Time_domain, YDATA);
	spline_y.Calc();

	spline_z.SetN(i);
	set_timedomain(Time_domain, t_rascheta/i, i);	
	spline_z.SetXY(Time_domain, ZDATA);
	spline_z.Calc();

}



//генерация vertex_array траектории
void TrajGen::GenVertexes(double dt)
{
	double X,Y,Z,t,t_sec;
	t = 0;
	t_sec = 0;
	
	vert_count = 0;
	sec_pts.clear();
	bIsCanRender = false;
	pos0 = vt.getP("pos0");
	while( t < t_rascheta)
	{
		X = CalcX(dt, t);
		Y = CalcY(dt, t);
		Z = CalcZ(dt, t);
		vertices[vert_count*3]=X;
		vertices[vert_count*3+1]=Y;
		vertices[vert_count*3+2]=Z;
		indexes[vert_count*3] = vert_count;
		indexes[vert_count*3+1] = vert_count+1;
		indexes[vert_count*3+2] = vert_count+2;
		//indexes[vert_count]=vert_count;
		vert_count++;
		// каждую секунду отмечать точку
		if(t_sec > 1.0)
		{
			sec_pts.push_back(Pnt(X,Y,Z));
			t_sec = 0;
		}
		t+=dt; t_sec+=dt;
	}
	bIsCanRender = true;

	return;


}

// рендеринг сгенерированной траетории, стартовой и конечной точек полета и т.д
void TrajGen::RenderTrajectory()
{
#if 0
    int i;
	double X,Y,Z,t2,dt,t_sec;	
	char s[255];

#ifndef DISABLE_TRAJGEN		
	if( !bIsCanRender ) return;
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, vertices );

	glPushMatrix();
    //glTranslatef ( 0, 0, 0 );
	//glRotatef(angle.x, angle.y, angle.z,1);
	

	//линии траектории
	glVertexPointer( 3, GL_FLOAT, 0, vertices );
	glEnable (GL_LINE_SMOOTH); 
	glDrawElements(GL_LINES, vert_count*3, GL_UNSIGNED_INT, indexes);
	glDisable (GL_LINE_SMOOTH); 
	
	//отрисовать "секундные точки"
	for(int i=0; i<sec_pts.size(); i++)
	{
		sprintf(s,"sec %d",i);		
		draw_p(s,sec_pts[i], RGB(0,0,0),0);
	}

	//путевые точки
/*	for(int i=0; i<polet.waypoints.size(); i++)
	{
		sprintf(s,"%d",i);		
		draw_p(s,polet.waypoints[i], RGB(0,0,0),0);
	}
*/
	glPopMatrix();
#endif
#endif
}

// выдать состояние симулятора по сгенерированным законам для углов, скоростей и ускорений
// на момент времени 

void TrajGen::PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out )
{
#ifndef DISABLE_TRAJGEN	
	
	sim_out.angles.yaw   =  GetYaw(tsim);

	sim_out.a = Pnt( Getax(tsim), Getay(tsim), Getaz(tsim) );
	sim_out.v = Pnt( Getvx(tsim), Getvy(tsim), Getvz(tsim) );
	if( rolllaw.type == "PAD" )
	{
		sim_out.angles.roll = sim_out.angles.roll  + sim_in.rule.ruleRoll;
		//gprintf("%f %f", sim_in.rule.rulePitch,sim_in.rule.ruleRoll);
	}
	else
		sim_out.angles.roll  =  GetRoll(tsim);
	
	if( pitchlaw.type == "PAD" )
	{
		
		sim_out.angles.pitch = GetPitch(tsim) + sim_out.angles.pitch  + sim_in.rule.rulePitch;
		//gprintf("%f %f", sim_in.rule.rulePitch,sim_in.rule.ruleRoll);
	}
	else
		sim_out.angles.pitch =  GetPitch(tsim);

    sim_out.ang.x = sim_out.angles.x;
    sim_out.ang.y = sim_out.angles.y;
    sim_out.ang.z = sim_out.angles.z;

	//gprintf("SIM: t = %f x,y,z %f %f %f", t, CalcX(t), CalcY(t), CalcZ(t));

	//sim_out.S = Pnt( CalcX(dt, tsim), CalcY(dt, tsim), CalcZ(dt, tsim) );
	sim_out.S = Pnt( Getx(tsim), Gety(tsim), Getz(tsim) );

	tsim+=dt;

	//циклируем время
	if( tsim > t_rascheta )
	{		
        InitPlaneSimulation(sim_in,sim_out);
		tsim = 0;
	}
#endif
}



// Сгенерировать закон 
// Res - разрешением исходных точек
// Spline - обьект сплайна для которого делается расчет
// time - время генерации
// law - закон или функция изменения параметра
void TrajGen::GenerateLawInterp(double* f, int Res, Spline& spl, double time, Law& law)
{
	if( law.type == "" ) return;
	GenerateLaw(f,Res,time,law);
	//шкала времени
	set_timedomain(Time_domain, time/Res, Res);	
	//генерируем массивы коэффициентов сплайна
	spl.bDoSplit = law.bDoSplit;
	spl.split_level = law.split_level;
	spl.xmin = law.xmin;
	spl.xmax = law.xmax;
	spl.SetN(Res);
	spl.SetXY(Time_domain, f);
	spl.Calc();
}

// распечатать состояние генератора
void TrajGen::PrintInfo()
{
	
        prn("---------TrajGen----------");

        prn("Ntest = %d               \tnumber of test \r\n"
                "t0_matsim = %f       \tgeneration time \r\n"
                "ResolutionRoll = %d  \tresolution roll \r\n"
                "ResolutionYaw = %d   \tresolution yaw \r\n"
                "ResolutionPitch =%d  \tresolution Pitch\r\n"
                "t_rascheta = %4.2f	  \ttime \r\n"
                "N = %d                 \t\tresolutions for laws \r\n"
                "f = %4.2f;            \tdata frequency (200 Hz 1000 Hz ) \r\n"
                "pos0 = %s;            \tstarting point \r\n"
                "t_ost = %4.2f;        \tsome thing from Teilor Macloren Formula\r\n",
                Ntest,t0_matsim, ResolutionRoll, ResolutionYaw, ResolutionPitch, t_rascheta, Ndef, f_dat, pos0.toStr("%.1f,%.1f,%.1f").c_str(), t_ost);

}


