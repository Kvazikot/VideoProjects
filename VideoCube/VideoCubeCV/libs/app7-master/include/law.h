#pragma once

#include <string>
#include "planesim.h"
#include "interp.h"

extern void prn(const char* fmt,...);

// ��������� ������� ��������� ���������
//(�������� ��������, ���������, ����)
struct Law
{
	Law()
	{
		Amin = 0;
		Amax = 1;
		type = "";
		Res  = 500;
		T = 0.0;
	
	}
	// ������ � �������� Amax Amin
	Law(double Amax, double Amin, string type)
		:Amax(Amax),type(type),Amin(Amin)
	{
		T = 0.0;
		Res  = 500;
	}
	//������ �������������
	Law(double Amax, double T, double phi0, string type)
		:Amax(Amax),T(T),phi0(phi0),type(type)
	{
	
	}
	void Print()
	{
            prn("%s\t%s %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f", name.c_str(),type.c_str(), Amax, Amin, Res, T, phi0, tmax);
	}
	void Reset()
	{
		type = "";
		Amax = 0; Amin = 0; Res  = 500; T = 0; phi0 = 0; tmax = 0;
	}

	string type;	// ��� ������
	string name;	// ��� ������
	double Amax;	// ������������ ��������� \ �������� �������
	double Amin;	// ����������� ��������� ( ������� ������� )
	double Res;		// ���������� ��� ������������ ������� \ ���-�� ���. ����� ��� �������	
	double T;		// ������ ���������
	double phi0;    // ��������� ���� ���������
	double tmax;    // ����� ��������� ������� (�������)
	double scale;   // �������������� \ �� �������������� ������� �� t E [0..t_rascheta]
	double t0;		// ������ ���������
	bool  bDoSplit;  // ������������ ��������
	double xmin;	// ���
	double xmax;	// ���
	double split_level; // ������� ��������� ������� ( ������ 0 )
	double sigma;	// ��� ����������� ������ ������
	char   PTS_XY;	// ������ ����� (�������-�������� �������)
	vector<PntEx>	   parr;
	vector<Law>		   sublaws;
	Spline			   spl;
	
};

void set_timedomain(double* data,double step,int N);
void prorej_signal(double in[], double out[], int Nl, int Nh);
int  compress_signal(double in[], double out[], int N, double f);
void copy_signal(double in[], double out[], int N);
void get_derev(double in[], double out[], int N, int dx);
double get_mean(double in[], int N);
double get_max(double in[], int N);
double get_min(double in[], int N);
void shift_left_signal(double a[], int dx, int N);
void shift_right_signal(double a[], int dx, int N);
void get_marker_sig(double* in, double* marker, int N, double porog);
void blur_signal_lin(double in[], double out[], int N, double factor);
void GenerateLaw(double* f, int N, double time, Law& law);
void mul_signal(double a[], double konst, int N);
void add_noise1(double* in, int N, int A, float P);
