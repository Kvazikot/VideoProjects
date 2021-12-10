/**********************************************************************
 *  Project App6
 *
 *  random_staff.cpp
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

#include "../include/geometry.h"
#include "../include/str_utils.h"
#include "../include/planesim.h"
#include "../include/random_staff.h"
#include <math.h>
#include <stdlib.h>
//#include <random>

double Xdata[10000];
double Ydata[10000];
long   P[1000000];
extern void prn(const char* fmt,...);
extern void SetGrafEvent();
#define Sqr(a) fabs( a*a )
#define Random11() (double)rand()/RAND_MAX 


int rand_sign(float P)
{
	if( ((float)rand()/RAND_MAX) > P )
		return -1;
	else 
		return 1;
}


double randf(float w)
{
	return (double)w*rand()/RAND_MAX;
}


double randLawByTable(std::map<double, double> probOfLvl)
{
	return 0;
}


double triangular(double a, double b, double c) {
    double U = rand() / (double) RAND_MAX;
    double F = (c - a) / (b - a);
    if (U <= F)
        return a + sqrt(U * (b - a) * (c - a));
    else
        return b - sqrt((1 - U) * (b - a) * (b - c));
}

double exponential(double lambda)
{
	double x = -logf(Random11())/lambda;
	return x;
}



double normal3(double stddev, double mean)
{
	double u1,v1,s2;
	s2 = 3;
	while(s2 > 1)
	{
		u1 = (double)2*(Random11()) - 1;
		v1 = (double)2*(Random11()) - 1;
		s2 = Sqr(u1) + Sqr(v1);
	}
	v1 = log((double)s2);
	v1 = (double)-2*log(s2)/s2;
	double res = (double)u1*sqrt(v1)*stddev + mean ;
	return res; 
}




void normal_distr(double y[], double stddev, double mean, int Ni)
{
	double labmda;

	labmda = 0;
	for(int i=0; i < Ni; i++)
	{
		labmda = normal3(stddev, mean);
		y[i] = labmda;
	}

}

void get_normal()
{
	  const int nrolls=10000;  // number of experiments
	  const int nstars=100;    // maximum number of stars to distribute

	  int p[10]={};

	  for (int i=0; i<nrolls; ++i) {
		double number = fabs( normal3(2.0, 6.0) );
		if ((number>=0.0)&&(number<=10.0)) ++p[int(number)];
	  }

      prn("normal_distribution (5.0,2.0):");

	  string s;
	  for (int i=0; i<10; ++i) {
	    s=IntToStr(i) + "-" + IntToStr(i+1) + ": ";
		s+=std::string(p[i]*nstars/nrolls,'*');
        prn(s.c_str());
	  }
}

void DrawProbGrafic()
{	

	return;

}

