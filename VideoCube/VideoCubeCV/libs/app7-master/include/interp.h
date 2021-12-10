#pragma once
#include <QDebug>

double neville ( int, double*, double*, double );
double* divdiff ( int, double*, double* );
double nf_eval ( int, double*, double*, double );
void cubic_nak ( int, double*, double*, double*, double*, double* );
void cubic_clamped ( int, double*, double*, double*, double*,double*, double, double );
double spline_eval ( int, double*, double*, double*, double*,double*, double );


struct Spline
{
	 Spline(){
		  bDoSplit = false;
		  N = 100;
		  x = new double [N];
		  f = new double [N];
      	  b = new double [N];
		  c = new double [N];
		  d = new double [N];
	 }
	 Spline(int _N):N(_N)
	 {
		 bDoSplit = false;
		  x = new double [_N];
		  f = new double [_N];
      	  b = new double [_N];
		  c = new double [_N];
		  d = new double [_N];
	 }
	 void SetN(int _N)
	 {
		  delete x; delete f; delete b; delete c; delete d;	 
 		  N = _N;
	      x = new double [N];
		  f = new double [N];
      	  b = new double [N];
		  c = new double [N];
		  d = new double [N];
	 }
	 ~Spline()
	 {
		 delete x; delete f; delete b; delete c; delete d;	 
	 }
	 void Calc()
	 {
		if(N == 0 ) return;
		coeff = divdiff ( N, x, f );
		cubic_nak ( N, x, f, b, c, d );
	 }
	 
	 double GetF(double t)
	 {
		if(N == 0 ) 0;
		if( bDoSplit )
		{
			if(( t < xmin ) || ( t > xmax))
				return split_level;
		}
		
		return spline_eval ( N, x, f, b, c, d, t );
	 }
	 void SetXY(double* _x, double* _f)
	 {
		for(int i=0; i<N; i++)
		{
			x[i]=_x[i];
			f[i]=_f[i];
            //qDebug() << "x[i]=" << x[i];
            //qDebug() << "f[i]=" << f[i];
		}
	 }
	 void SetSignal(double* f, double time, int Res);

	 double *x;
     double *f;
     double *b;
     double *c;
     double *d;
     double *coeff;
	 int    N;
	 double xmin;			// ОДЗ
	 double xmax;			// ОДЗ
	 double split_level;	// значение функции вне ОДЗ
	 bool   bDoSplit;		// делать отчечение сплайна

};

//static
/*
struct Spline
{
	// 2 млн входных точек 
	#define MAX_SPLINE_SIZE 2000000 
	double x[MAX_SPLINE_SIZE];
	double f[MAX_SPLINE_SIZE];
	double b[MAX_SPLINE_SIZE];
	double c[MAX_SPLINE_SIZE];
	double d[MAX_SPLINE_SIZE];
	double* coeff;
	 Spline(){
		  N = 100;
	 }
	 Spline(int _N):N(_N)
	 {
	 }
	 void SetN(int _N)
	 {
 		  N = _N;
	 }
	 ~Spline()
	 {
		 //delete x; delete f; delete b; delete c; delete d;	 
	 }
	 void Calc()
	 {
		coeff = divdiff ( N, x, f );
		cubic_nak ( N, x, f, b, c, d );
	 }
	 
	 double GetF(double t)
	 {
		return spline_eval ( N, x, f, b, c, d, t );
	 }
	 void SetXY(double* _x, double* _f)
	 {
		for(int i=0; i<N; i++)
		{
			x[i]=_x[i];
			f[i]=_f[i];
		}
	 }


	 int    N;
};
*/
