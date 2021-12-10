#ifndef TRAJGEN_H
#define TRAJGEN_H

#include <string>
#include "law.h"
#include "interp.h"
#include "planesim.h"
void gprintf(const char* fmt,...);
bool isContains(string s, string substr);
Pnt line_eq(Pnt A, Pnt B, float t);

// ��������� ����������
class TrajGen
{
public:

		//TrajGen(){  }
		Law rolllaw;								// ����� ��� �����
		Law pitchlaw;								// ����� ��� �������
		Law yawlaw;									// ����� ��� �����
		Law vlaw[3];								// ������ ��� ���������
		Law alaw[3];								// ������ ��� ���������

		Spline spline_roll;							// ������ ������ ��� �����
		Spline spline_pitch;						// ������ ������ ��� �������
		Spline spline_yaw;							// ������ ������ ��� �����
		Spline spline_v[3];							// ������� ���������
		Spline spline_a[3];							// ������� ���������
		Spline spline_x, spline_y, spline_z;		// ������� ��������� (��� ��������)
		double t0_matsim;							//����� ������ �������������� ���������

		//  ---                  DATA					---------------------------------
		double XDATA[200000];
		double YDATA[200000];
		double ZDATA[200000];
		double RollLawArr[200000];					// �������� ����� ������ ��� ����� (������������)
		double YawLawArr[200000];					// �������� ����� ������ ��� ����� (������������)
		double PitchLawArr[200000];					// �������� ����� ������ ��� ������� (������������)
		double vxLawArr[200000];
		double vyLawArr[200000];
		double vzLawArr[200000];
		double axLawArr[200000];
		double ayLawArr[200000];
		double azLawArr[200000];		
		double  Time_domain[200000];


		float		  vertices[200000];
		unsigned int  indexes[200000];

		int			display_list;					// ���������� ������ ��� ��������� ����������
		bool		bIsCanRender;
		bool		disabled;
		int			vert_count;						// �����. ������ ��� ���������� ����������
		vector<Pnt> sec_pts;						// ����� ������
		int     ResolutionRoll;						// ���������� �� �����
		int     ResolutionYaw;						// ���������� �� �����
		int     ResolutionPitch;						// ���������� �� �������
		double  t_rascheta;							// ����� �� ������� ������������ ������
		int		Ninterpol;							// ���������� �������������� �����
		int		Ndef;								// ���������� �������� �����
		float	f_dat;									// ������� ������ ������ (200�� 1000��)
		Pnt		pos0;								// ��������� ������� ��������
		double	t_ost;								// �����-�� ���������� ����� �� ������� ������� ��������� 
		double  tsim;									// ���������� ������� ��� ��������� � 3d ��������� �� OSD)

		void Init();
		void GenerateLaws();
		void GenerateLawInterp(double* f, int Res, Spline& spl, double time, Law& law);
        void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out );
        //void InitPlaneSimulation( PlaneSim_Input& sim_in );
		void SetLawParams(Law& law, string table_key);
		void SetPolet(string table_key, int N);
		void CalcCoordSplines();
		void ReCalcSpline(string table_key);
		void ResetSpline(Spline& spl, double* arr);
//		void GetData(SensorData& data, double t, double f);  // ����� ��������� ������ �� ������ �������
		void UpdateLaw(string table_key);
		void PrintInfo();
		void Reset();
		void ApplyDefaults();
		void GenVertexes(double dt);
		void DrawLaw(string table_key);
		void RenderTrajectory();					// ��������� ��������������� ���������, ��������� � �������� ����� ������ � �.�
		void CreateAssociations();
		void Test(int Ntest);						// �����
		int  Ntest;									// ����� �������� �����	

		double GetYaw(double t) { return spline_yaw.GetF(t); }
		double GetRoll(double t) { return spline_roll.GetF(t); }
		double GetPitch(double t) { return spline_pitch.GetF(t); }
		double Getax(double t) { return spline_a[0].GetF(t); }
		double Getay(double t) { return spline_a[1].GetF(t); }
		double Getaz(double t) { return spline_a[2].GetF(t); }
		double Getvx(double t) { return spline_v[0].GetF(t); }
		double Getvy(double t) { return spline_v[1].GetF(t); }
		double Getvz(double t) { return spline_v[2].GetF(t); }
		double Getx(double t) { return spline_x.GetF(t); }
		double Gety(double t) { return spline_y.GetF(t); }
		double Getz(double t) { return spline_z.GetF(t); }
				
		double CalcTeilor(double dt, double r0, double v, double a, double t_ost)
		{
			double val;
            val = r0 + v*dt + a*dt*dt / 2. + t_ost;
			return val;
		}

		double CalcX(double dt, double t)
		{
			double val = 0;
			val = CalcTeilor(dt, pos0.x, Getvx(t), Getax(t), t_ost);
			pos0.x = val;
			return val;
		}

		double CalcY(double dt, double t)
		{
			double val;
			Pnt v,a;
			v = Pnt(Getvx(t),Getvy(t),Getvz(t));
			a = Pnt(Getax(t),Getay(t),Getaz(t));			
			val = CalcTeilor(dt, pos0.y, v.y, a.y, t_ost);
			pos0.y = val;
			return val;
		}

		double CalcZ(double dt, double t)
		{
			double val = 0;
			val = CalcTeilor(dt, pos0.z, Getvz(t), Getaz(t), t_ost);
			pos0.z = val;
			return val;
		}



};



#endif
