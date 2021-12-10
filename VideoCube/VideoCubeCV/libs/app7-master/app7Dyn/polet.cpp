/**********************************************************************
 *  Project App6
 *
 *  polet.cpp
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
#include <algorithm>
#include <string> 
#include "../include/geometry.h"
#include "../include/str_utils.h"
#include "../include/glcode.h"
#include "../include/cobjregistry.h"
#include "../include/curve.h"
#include "../include/law.h"
//#include "traj_gen.h"			// взять RenderTrajectory
#include "../include/var_parse.h"			// для разбора таблиц vt
#include "../include/random_staff.h"
#include "../include/autopilot.h"			// для FlyData*
#include "../include/planesim.h"			// для CalcTeilor()
#include "../include/polet.h"
#include "../include/matrix.h"
#include "../include/param_screen_bcb.h"
#include "../include/vector3d.h"
#include "../include/ploter.h"

//#define  DISABLE_WPPOLET

static ParamScreen& vt = ParamScreen::getVtInstance();
WPPolet		wppolet;

#define StrSplitW(s) StrSplitE(s," ",0)
#define for_each(e,s) e; vector<string> vecc = StrSplitE(s," ",0); vecc.push_back(" "); e=vecc[0];  for(int i=1; i<vecc.size(); e=vecc[i], i++  )
inline int which_tok(string tok, string s) { vector<string> v = StrSplitE(s," ",0); return (find(v.begin(),v.end(),tok)-v.begin()); }
#define sqr(a) fabs((double)a*(double)a)


WPPolet::WPPolet()
{


}

void wppolet_init(){ wppolet.Init(); }

void Zmeika(Curve* curve)
{
    //static Ploter* ploter = Ploter::getInstance();
    curve->pts.clear();
    double TotalTime = 20;
    int Npts = 10;
    double dt = TotalTime / Npts;
    const double We = 5;
    const double H = 5;
    const double T = 3;
    double t = 0;
    double w;
//    for( int i=0; i < Npts; i++ )
//    {
//        VPnt* p = new VPnt();
//        p->ti = t;
//        p->We = We;
//        p->y = H;//+2*(double)rand()/RAND_MAX;
//        w = 2*M_PI/T;
//        p->kurs.x = 45*cos(w*t);
//        curve->pts.push_back(p);
//        ploter->setXY(0,t,p->kurs.x);
//        t+=dt;
//    }

    Npts = 10;
    double a=0;
    double da=360/Npts;
    TotalTime = 20;
    dt = TotalTime / Npts;
    for( int i=0; i < Npts; i++ )
    {
        VPnt* p = new VPnt();
        p->ti = t;
        p->We = We;
        p->y = H;//+2*(double)rand()/RAND_MAX;
        p->kurs.x=a;
        a+=da;
        curve->pts.push_back(p);
        //ploter->setXY(0,t,p->kurs.x);
        t+=dt;
    }

}

void WPPolet::Init()				//  инициализация путевых точек, микроплана, особых точек ЛА
{
#ifndef DISABLE_WPPOLET

    vert_count = 0;
    bIsCanRender = false;
    dataGenerated = false;

    string param_skeys = "roll yaw pitch ax ay az vx vy vz x y z we wh wn" ;
    string point_skeys = "C0 p2";
    pnt_key = StrSplitW(point_skeys);
    param_key = StrSplitW(param_skeys);
    for(int i=0; i < (int)pnt_key.size(); i++)
        for(int j=0; j < (int)param_key.size(); j++)
        {
            string key = pnt_key[i] + "." + param_key[j];
            DATA_MAP[key] = new FlyData(key);
            DATA[i][j] = DATA_MAP[key];
        }

    //НОВАЯ ЛОГИКА - ЧТЕНИЕ ИЗ ОБЬЕКТА класса Curve
	tsim = 0;
	Curve* wp_curve;
    CObjRegistry& MREG = CObjRegistry::getInstance2();
	wp_curve = (Curve*)MREG.FindObj("curve01");
    //Zmeika(wp_curve);
	if( wp_curve!=0 )
	{
		f_dat = 200;//vt.geti("f_dat");
		if( wp_curve->pts.size() == 0 ) return;
		waypoints.clear();
        for(int i=0; i < (int)wp_curve->pts.size(); i++)
		{
			PoletPoint p;
			p.t = wp_curve->pts[i]->ti;
			p.i = i;
			p.H = wp_curve->pts[i]->y;
			p.We  = wp_curve->pts[i]->We;
			p.psi0   = wp_curve->pts[i]->kurs.x*GR;
			waypoints.push_back(p);
		}
        //сгенерировать траекторию
		GenData(  *(Pnt*)wp_curve->pts[0] );
	}

	return;


#endif
}


void WPPolet::AssignWaypointsCoords()
{
//НОВАЯ ЛОГИКА - ЧТЕНИЕ ИЗ ОБЬЕКТА класса Curve
/*	Curve* wp_curve;
    CObjRegistry& MREG = CObjRegistry::getInstance2();
	wp_curve = (Curve*)MREG.FindObj("curve01");
	if( wp_curve==0 ) return;
	if( wp_curve->pts.size() == 0 ) return;
	for(int i=0; i < wp_curve->pts.size(); i++)
	{
		//*(Pnt*)wp_curve->pts[i] = waypoints[i].coord;
		wp_curve->pts[i]->x = waypoints[i].coord.x;
		wp_curve->pts[i]->y = waypoints[i].coord.y;
		wp_curve->pts[i]->z = waypoints[i].coord.z;

	}
*/

}


void WPPolet::AddWaypoint(int i, string tab)
{
	PoletPoint p;
	vt.BeginTable(tab);	
	p.t = vt.getd("ti");
	p.i = i;
	p.H = vt.getd("H");
	p.We  = vt.getd("We");
	p.psi0   = vt.getd("psi0")*GR;
	waypoints.push_back(p);
	vt.EndTable();
}

// Цикл симуляции
// выдать состояние симулятора по сгенерированным законам для углов, скоростей и ускорений
// на момент времени
#define SP(point,param,tt) DATA[point][param]->spline.GetF(tt)
void WPPolet::PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out )
{
	if(waypoints.size()==0) return;
	if(dataGenerated==0) return;
    sim_in=sim_in;
    // циклируем время
	if( tsim > t_rascheta )
	{		
		tsim = 0;
	}
	sim_out.angles.yaw   =  SP(C0,yaw,tsim);
	sim_out.angles.roll  =  SP(C0,roll,tsim);
	sim_out.angles.pitch =  SP(C0,pitch,tsim);
    sim_out.ang.x   =  SP(C0,pitch,tsim);
    sim_out.ang.y  =  SP(C0,yaw,tsim);
    sim_out.ang.z =  SP(C0,roll,tsim);
	sim_out.a = Pnt( SP(C0,ax,tsim), SP(C0,ay,tsim), SP(C0,az,tsim) );
	sim_out.v = Pnt( SP(C0,vx,tsim), SP(C0,vy,tsim), SP(C0,vz,tsim) );	
	sim_out.S = Pnt( SP(C0,x,tsim),  SP(C0,y,tsim),  SP(C0,z,tsim) );
    sim_out.w = Pnt( SP(C0,we,tsim),  SP(C0,wh,tsim),  SP(C0,wn,tsim) );

	tsim+=dt;

}

void WPPolet::SmoothData(int Ni, double dt, point_keys point)
{
	set_timedomain(Time_domain, dt, Ni);		
	for(int i=0; i<MAX_FLY_PARAMS; i++)
	{
		FlyData* d = DATA[point][i];
		d->spline.SetN(Ni);
		d->spline.SetXY(Time_domain, d->arr);
		d->spline.Calc();
	}
}

//взять расчитанную точку траектории для времени t
Pnt WPPolet::getTrajectoryPoint(double t)
{
    if( t > t_rascheta ) return Pnt(0,0,0);
    Pnt p;
    p.x = DATA[C0][x]->spline.GetF(t);
    p.y = DATA[C0][y]->spline.GetF(t);
    p.z = DATA[C0][z]->spline.GetF(t);
    return p;
}

//взять углы траектории
Pnt WPPolet::getTrajectoryAngles(double t)
{
    if( t > t_rascheta ) return Pnt(0,0,0);
    Pnt p;
    p.x = DATA[C0][roll]->spline.GetF(t);
    p.y = DATA[C0][pitch]->spline.GetF(t);
    p.z = DATA[C0][yaw]->spline.GetF(t);
    return p;
}


// вложить данные
void WPPolet::SetData(int Ni, point_keys point, PlaneSim_Output& sim_out)
{
	for(int i=0; i<MAX_FLY_PARAMS; i++)
		DATA[point][i]->Ni = Ni;
	DATA[point][x]->arr[Ni] = sim_out.S.x;
	DATA[point][y]->arr[Ni] = sim_out.S.y;
	DATA[point][z]->arr[Ni] = sim_out.S.z;
	DATA[point][vx]->arr[Ni] = sim_out.v.x;
	DATA[point][vy]->arr[Ni] = sim_out.v.y;
	DATA[point][vz]->arr[Ni] = sim_out.v.z;
	DATA[point][ax]->arr[Ni] = sim_out.a.x;
	DATA[point][ay]->arr[Ni] = sim_out.a.y;
	DATA[point][az]->arr[Ni] = sim_out.a.z;
	DATA[point][roll]->arr[Ni] = sim_out.angles.roll;
	DATA[point][pitch]->arr[Ni] = sim_out.angles.pitch;
	DATA[point][yaw]->arr[Ni] = sim_out.angles.yaw;
    DATA[point][we]->arr[Ni] = sim_out.w.x;
    DATA[point][wh]->arr[Ni] = sim_out.w.y;
    DATA[point][wn]->arr[Ni] = sim_out.w.z;
    DATA[point][pitch]->arr[Ni] = sim_out.ang.x;
    DATA[point][yaw]->arr[Ni] = sim_out.ang.y;
    DATA[point][roll]->arr[Ni] = sim_out.ang.z;

	
}

// Сгенерировать траекторию
void WPPolet::GenData(Pnt start_point)
{
	double    dt,t;
	int		  Ni;

	dt = 1. / f_dat;
	//dt = 0.1;
	t  = 0;
	Ni = 0;
	
	dataGenerated = false;
	bIsCanRender = false;
	t_rascheta = waypoints.back().t;
	vert_count = 0;
	data.start_point = start_point;
	data.sim_out.S = start_point;
    for(int i=0; i < (int)waypoints.size()-1; i++)
	{
		t = waypoints[i].t;
		waypoints[i].coord = data.sim_out.S;
		while( t < waypoints[i+1].t  )
		{
	   	    CalcStep(waypoints[i].t, waypoints[i+1].t, t, dt, i, data, waypoints[i], waypoints[i+1]);
            //добавить вершину для рендеринга траектории
			AddVertex(data.sim_out.S);
            //добавить данные
			SetData(Ni, C0, data.sim_out); 
			t+=dt; Ni++;
		}

	}
	SmoothData(Ni, dt, C0);
	//waypoints.back().coord = data.sim_out.S;
	AssignWaypointsCoords();
	bIsCanRender = true;
	dataGenerated = true;

	/*	for(int i=0; i < pnt_key.size(); i++)
	for(int j=0; j < param_key.size(); j++)
	FlyData* fd = DATA[i][j] ;
	*/
}

void WPPolet::AddVertex(Pnt S)
{
	vertices[vert_count*3]=S.x;
	vertices[vert_count*3+1]=S.y;
	vertices[vert_count*3+2]=S.z;
	indexes[vert_count*3] = vert_count;
	indexes[vert_count*3+1] = vert_count+1;
	indexes[vert_count*3+2] = vert_count+2;
	vert_count++;
}


// Сплайновая аппроксимация параметров движения
void spl(double TTek, double t1, double t0, double h1, double h0, double* hh, double* hht, double* hhtt) 
{
	 double DT, A, AT, ATT, B, BT, BTT, a;
	 DT = t1 - t0;
	 A = (t1 - TTek) * (t1 - TTek) * (2.0 * (TTek - t0) + DT);
	 AT = 6.0 * TTek * TTek + 2.0 * (DT - 2.0 * t0 - 4.0 * t1) * TTek +  4.0 * t1 * t0 - 2.0 * t1 * DT + 2.0 * t1 * t1;
	 ATT = 12.0 * TTek + 2.0 * (DT - 2.0 * t0 - 4.0 * t1);
	 B = (TTek - t0) * (TTek - t0) * (2.0 * (t1 - TTek) + DT);
	 BT = -6.0 * TTek * TTek + 2.0 * (2.0 * t1 + DT + 4.0 * t0) * TTek - (4.0 * t0 * t1 + 2.0 * t0 * DT + 2.0 * t0 * t0);
	 BTT = -12.0 * TTek + 2.0 * (2.0 * t1 + DT + 4.0 * t0);
	 a = DT * DT * DT;
	 *hh = (h0 * A + h1 * B) / a;
	 *hht = (h0 * AT + h1 * BT) / a;
	 *hhtt = (h0 * ATT + h1 * BTT) / a;
}


void vec_rotation(Pnt& V, double kurs, double kren, double tangaj)
{
	static double mat[3][3],mat2[3][3],mat3[3][3],mat4[3][3],mat5[3][3],mat6[3][3];
	mrotx(kren,mat);
	mroty(kurs,mat2);
	mrotz(tangaj,mat3);
	mmult(3,3,mat3,mat2,mat4);
	mmult(3,3,mat4,mat,mat5);
	Pnt tmp;
	mvmult(3, mat5, (float*)&V, (float*)&tmp);
	V = tmp;	
}

// расчет сегмента траектории между узловыми точками для момента Ttek
// время текущее, время точки 2, время точки 1, высота точки 2, высота точки 1, парам-р, скор. изм. пар-ра, скор. скорости изм. пар-ра
void WPPolet::CalcStep( double t0, double t1,  double Ttek,  double dt,  int i,  CalcData& out, PoletPoint p0, PoletPoint p1 )
{
    i=0;
	// -------------------- Р’РҐРћР” -----------------------------------
	double h1  = p1.H;
	double h0 = p0.H;
	double v1  = p1.We;
	double v0 = p0.We;
	double psi1  = p1.psi0;
	double psi0 = p0.psi0;
	double& WE1=out.WE1,&WN1=out.WN1,&WH1=out.WH1;
	double& we=out.we,&wn=out.wn,&wh=out.wh;
	double& WE=out.WE,&WN=out.WN,&WH=out.WH;
	double& WE2=out.WE2,&WN2=out.WN2,&WH2=out.WH2;
	double& VE=out.VE,&VN=out.VN,&VH=out.VH;
    double& HH=out.HH,&HHH=out.HHH;
	double& We=out.We,&WWe=out.WWe;
	double& y=out.y,&yy=out.yy, &yyy=out.yyy;
	bool ModelFail;
	// -------------------- Р’РҐРћР” -----------------------------------
	
	//prn("CalcStep %lf %lf %lf %d", t0, t1, Ttek, i);

    spl(Ttek, t1, t0, h1, h0, &out.H, &out.HH, &out.HHH );	 // высота 1 произв 2 произв
    spl(Ttek, t1, t0, v1, v0, &out.We, &out.WWe, &out.WWWe); // скорость 1 произв 2 произв
    spl(Ttek, t1, t0, psi1, psi0, &out.y, &out.yy, &out.yyy); // курс 1 произв 2 произв


	//*hhm = hm;
    double j,b,c,k;
	j = out.HH/ out.We;
	b = (out.HHH * out.We - out.HH* out.WWe) / (out.We * out.We);
    if (1.0L - j * j < 0.L) {
		ModelFail = true;
		return;
	}
    c = sqrt(1.0L - j * j);
	k = b / c; 
	out.a[1] = atan((double)j / c); 

	j = out.We * out.yy / (g*g);
    b = (1.0L + j * j) * (g*g);
	out.a[0] = -(out.WWe* out.yy + out.We * out.yy) / b;
	out.a[1] = atan((double)-out.We * out.yyy / (g*g)); 

    //углы
    double d1 = out.HH / out.We;
    if( d1 <=1 && d1 >=-1  )
        out.a[0] = asin(d1);// тангаж
	double d2 = (double)-(out.We / g) * out.yy;
    out.a[1] = atan( d2	);// крен

    //производные углов
	k = 1. / ( sqrt( 1 - (HH / We) * (HH / We) ) );
	out.aa[0] = k *( (HHH * We - HH * WWe )/(We*We) ); // С‚Р°РЅРіР°Р¶
	out.aa[1] = - (1 / (1 + (We * yy)*(We * yy) / g)) * (1./g) * (WWe * yy + We * yyy); // РєСЂРµРЅ


    //линейные состовляющие  ускорения
	WE1 = WWe*sin(y)*cos(out.a[1]);
	WN1 = WWe*cos(y)*cos(out.a[1]);
	WH1 = WWe*sin(out.a[0]);

    //вращательные состовляющие скорости
	WE = We*sin(y)*cos(out.a[1]);
	WN = We*cos(y)*cos(out.a[1]);
	WH = We*sin(out.a[0]);
	//vec_rotation(out);
	

    //угловые скорости
	we = out.aa[0]*cos(y) - yy * sin(y) * cos(out.a[0]);
	wn = out.aa[0]*sin(y) + yy * cos(y) * cos(out.a[0]);
	wh = yy + out.a[1] * sin(out.a[0]);
    //prn("we=%3.3f wh=3.3f wn=3.3f",we,wh,wn);


	WE2 = wn*WH - wh*WN;
	WN2 = wh*WE - we*WH;
	WH2 = we*WN - wn*WE;

    // сумарные состовляющие скорости
	VE = WE1 + WE2; 
	VN = WN1 + WN2;
	VH = WH1 + WH2;


    //этап полмешивания шумов  - WE - WN


    // -------------------- ВЫХОД -----------------------------------

        //расчет координат по ТЕйлору и
        //dt = 1. / f_dat;
        Pnt& S = out.sim_out.S;
        Pnt& ang =  out.sim_out.ang;
        //ang = Angles(-out.a[0]/GR, out.a[1]/GR, 90 - y/GR);
        ang.x = -out.a[0]/GR ;     // pitch
        ang.z = out.a[1]/GR;          // roll
        ang.y = 90 - y/GR   ;          // yaw
        S.x = S.x + WE*dt + WE1*dt*dt / 2.;
        S.y = S.y + WH*dt + WH1*dt*dt / 2.;
        S.y = out.H;
        S.z = S.z + WN*dt + WN1*dt*dt / 2.;
        out.sim_out.v = Pnt(WE, WH, WN);
        // зпишем угловые скорости
        out.sim_out.w = Pnt(we,wh,wn);

}

// рендеринг сгенерированной траетории, стартовой и конечной точек полета и т.д
void WPPolet::RenderTrajectory()
{
	if( !bIsCanRender ) return;
	if( vert_count == 0 ) return;
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);	
	glColor4f(1,0,0,1);

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, vertices );


    //линии траектории
	glVertex3f(vertices[0],vertices[1],vertices[2]);
	glVertexPointer( 3, GL_FLOAT, 0, vertices );
	glEnable (GL_LINE_SMOOTH); 
	glDrawElements(GL_LINES, (vert_count)*3, GL_UNSIGNED_INT, indexes);
	glDisable (GL_LINE_SMOOTH); 

    //путевые точки
    for(int i=0; i<(int)waypoints.size(); i++)
	{
		PoletPoint& p = waypoints[i];
		//string s = "("+ IntToStr(i) + ") " +  DoubleToStr(pts[i]->ti) + " " + DoubleToStr(pts[i]->kurs.x);
		char s[100];				
		sprintf(s, "   %1.1f %1.1f %1.1f",  p.t,  p.H, p.psi0);		
        //draw_kurs(s, p.coord, Pnt(0,(p.psi0/GR)-90,0), EulOrdXYZ, RGB(255,0,0), 2);
        //renderBitmapString(p.coord.x,p.coord.y,p.coord.z,0,0.3,0,GLUT_BITMAP_HELVETICA_18,(char*)IntToStr(i+1).c_str());

	}



	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

}

