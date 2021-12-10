/**********************************************************************
 *  Project App6
 *
 *  law.cpp
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
//#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/geometry.h"
#include "../include/law.h"
#include "../include/random_staff.h"
#include "../include/str_utils.h"
#include "../include/param_screen_bcb.h"

double  Time_domain22[200000];

double hypf(double x) { return (double)1./x; }
double x2(double x) { return x*x; }
static ParamScreen& vt = ParamScreen::getVtInstance();


//парсинг строки закона, парсинг правил. Правила заносятся в таблицу vt
void SetLawParams(Law& law, string table_key, double t_rascheta)
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
	if( pts_s != "")
	{
		pts_s = StrRep(pts_s,"[","");
		pts_s = StrRep(pts_s,"]","");
		vector<string> elm = StrSplitE(pts_s,",",false);
		law.parr.clear();
        for(int i=0; i<(int)elm.size()-1; i+=2)
		{
            PntEx p;
			if((elm[i]=="") || (elm[i+1]=="") )
				continue;
			sscanf(elm[i].c_str(),"%f",&p.x);
			sscanf(elm[i+1].c_str(),"%f",&p.y);					
			law.parr.push_back(p);
		}
	}
	vt.EndTable();
}

// Сгенерировать закон 
// Res - разрешением исходных точек
// time - время генерации
// law - закон или функция изменения параметра
void GenerateLaw(double* f, int N, double time, Law& law)
{
	double x,k,Fmax,dt;
	int i_max, No, steps;

	if( law.type == "" ) return;

	//gprintf("GenerateLaw %s Res=%d par %s %f %f %f", law.name.c_str(), Res, law.type.c_str(), law.Amax, law.Amin, law.T);

	//шкала времени
    set_timedomain(Time_domain22, time/N, N);


	// Несглаженная функция ШУМ
	if( law.type == "TREUG_NOISE" )
	for(int i=0; i<N; i++)
		f[i] = randf(law.Amax)*rand_sign(0.5);

	// СИНУС
	if( law.type == "SIN" )
	{
		if(law.T == 0) law.T = 1;
		double freq = 2 * M_PI * law.T;
		for(int i=0; i<N; i++)
		{
            f[i] = law.Amax * sin( (double) Time_domain22[i] * freq  + law.phi0 * GR);
          //gprintf("%f %f", Time_domain22[i], f[i]);
		}

	}

	// ЭКСПОНЕЦИАЛЬНЫЙ СКАЧОК
	if( law.type == "EXP" )
	{
		double Fmax = exp(time);
		for(int i=0; i<N; i++)
		{			
            f[i] = law.Amax * exp( (double) Time_domain22[i] ) / Fmax - law.Amin;
          //gprintf("%f %f", Time_domain22[i], f[i]);
		}
	}

	// КВАДРАТИЧНЫЙ СКАЧОК
	if( law.type == "UP_QUAD" )
	{
			for(int i=0; i<N; i++)
			{
                x =  Time_domain22[i];
				Fmax = x2(law.tmax);
				k = law.Amax / Fmax;
				f[i] = k * x2(x);
			}
	}

	// КВАДРАТИЧНЫЙ СПАД
	if( law.type == "DOWN_QUAD" )
	{
			for(int i=0; i<N; i++)
			{
                x =  time - Time_domain22[i];
				Fmax = x2(law.tmax);
				k = law.Amax / Fmax;
				f[i] = k * x2(x);
			}
	}


	// ЛИНЕЙНЫЙ СКАЧОК
	if( law.type == "UP_LIN" )
	{
		k = law.Amax / time;
		for(int i=0; i<N; i++)
		{		
            x =  Time_domain22[i];
			f[i] = k * x + law.Amin;
		}
	}

	//ЛИНЕЙНЫЙ СПАД
	if( law.type == "DOWN_LIN" )
	{
		k = law.Amax / time;
		for(int i=0; i<N; i++)
		{		
            x =  time - Time_domain22[i];
			f[i] = k * x + law.Amin;
		}
	}

	//КУСОЧНАЯ ФУНКЦИЯ СО СГЛАЖИВАНИЕМ
	if( law.type == "KUS_SPL" )
	{
		for(int i=0; i<N; i++)
			f[i] = 0;
		No = law.parr.size() -1;
		steps = N / No;			// количество точек на отрезок
        for(int i=0; i<(int)law.parr.size()-1; i++)
		{
			Pnt A = law.parr[i];
			Pnt B = law.parr[i+1];
			Pnt C;
			for(int j=0; j<steps; j++)
			{			
				C = line_eq(A,B,(float)j/steps);
				x = C.x;				
				f[i*steps+j] = C.y;
			}
		}

	}	

	//ЭКСПОНЕЦИАЛЬНЫЙ ХОЛМ
	if( law.type == "HOLM" )
	{
		Fmax = x2(law.tmax);
		k =  law.Amax / Fmax;
		dt = time / N;
		i_max = int((law.tmax *N) / time );
		x = 0;
		for(int i=0; i<i_max; i++)
		{
			x += dt;		
			f[i] = k * x2(x);
		}
		x = -law.tmax;
		for(int i=i_max; i<N; i++)
		{
			x += dt;		
			f[i] = k * x2(x) ;
			//f[i] = law.Amax;
		}


	}


}


// растянуть сигнал всталять нулевые отчеты
void prorej_signal(double in[], double out[], int Nl, int Nh)
{
	int j = 0;
	int skip = Nh / Nl;
	for(int i=0; i<Nh; i++) out[i] = 0;
	for(int i=0; i<Nh; i+=skip, j++) 	out[i] = in[j];
}

//скопировать сигнал
void copy_signal(double in[], double out[], int N)
{
	for(int i=0; i<N; i++)
		out[i] = in[i];
}

// сжать сигнал
int compress_signal(double in[], double out[], int N, double f)
{
	int skip = int(1. / f);
	int k = 0;
	for(int i=0; i<N; i+=skip)
	{
		out[k] = in[i];
		k++;
	}
	return k;
}

// взять первую производную функции
// на выходе массив тогоже размера  
void get_derev(double in[], double out[], int N, int dx)
{
	int cnt=0;
	double dy=0, d=0;
	for(int i=0; i<dx; i++) out[i] = 0;
	for(int i=dx; i<N; i++)
	{
		out[i] = d;
		if( cnt == dx )
		{
			dy = in[i-dx] - in[i];
			d = fabs( dy / dx );
			out[i] = d; 
			cnt = 0;
		}
		cnt++;
	}
	
}

//взять среднее
double get_mean(double in[], int N)
{
	double S=0;
	for(int i=0; i<N; i++)
		S+=in[i];
	return S/N;
}

double get_max(double in[], int N){	return *max_element(in, in+N);}
double get_min(double in[], int N){	return *min_element(in, in+N);}
void shift_left_signal(double a[], int dx, int N){	for(int i=dx; i<N; i++)	a[i-dx] = a[i]; }
void shift_right_signal(double a[], int dx, int N){	for(int i=N-dx; i>0; i--)	a[i+dx] = a[i]; }
void mul_signal(double a[], double konst, int N){	for(int i=0; i<N; i++)	a[i] = konst*a[i]; }

//выбрать точки по маске (например по производной)
void get_marker_sig(double* in, double* marker, int N, double porog)
{
	for(int i=0; i<N; i++)
		if( marker[i] < porog  )
			in[i] = 0;
}

//добавить шума
void add_noise1(double* in, int N, int A, float p)
{
	for(int i=0; i<N; i++)
		if(rand_sign(p) > 0)
			in[i] = A / 2 + randf(A)*rand_sign(0.5);
}

// параметрическое уравнение прямой
double lin_ip(double from, double to, double t)
{
    return from + t * (to - from);
}


// загрубить сигнал
void blur_signal_lin(double in[], double out[], int N, double factor)
{
	int skip = int(1. / factor);
	double from,to;
	for(int i=0; i<N-skip; i+=skip)
	{
		from = in[i];
		to = in[i+skip];
		for(int k=0; k < skip; k++)			
		 out[i+k] = lin_ip(from, to, (double)k/skip);
		
	}
}

// установить массив времени 
void set_timedomain(double* data,double step,int N)
{
  	 double t=0;
	 for(int i=0; i < N; i++)	 {	data[i] = t;		t+=step;	 }
}
