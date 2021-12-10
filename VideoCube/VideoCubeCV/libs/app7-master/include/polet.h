#pragma once

#include "autopilot.h"    // ��� FlyData*
#include "planesim.h" 

#define	g 9.8L

// ����� �������� ������
struct Fca
{
	Pnt     coord;	// ���������� ����� ���
	double  r0;     // ���������� �� ������ ��������
	double  phi;	// ���� ������� ���

};
// ������� ����� ������
struct PoletPoint
{
	int    i;		// ����� �����
	double t;		// ����� ����������� �����
	double H;		// ������ �����
	double We;		// ��������
	double psi0;    // �������� ����
	
	Pnt     coord; // ����������� ���������� (�������� ������)
};

// ����� 
class LAPoint : public Pnt
{
	LAPoint();
	string key;
};


// ������ ����������������� ������� � ���������
// ��� ������ ����
struct CalcData
{
	CalcData(){};
	double H;	  // ������
	double We;	  // ��������
	double WWe;  // ���������
	double WWWe;
	double a[3];  // ���� 
	double aa[3]; // ����������� �����

	double y, yy, yyy;
	double HHH,HH;
	double we,wn,wh;
	double WE,  WN,  WH;
	double WE1, WN1, WH1;
	double WE2, WN2, WH2;
	double VE,VN,VH;
	double gamma;  
	PlaneSim_Output	sim_out;
	Pnt      start_point;
};

void DrawStep(double t, double dt, CalcData& data);

// ����� �� ������� ������
struct WPPolet
{
    enum param_keys {roll, yaw, pitch, ax, ay, az, vx, vy, vz, x, y, z, we, wh, wn};
	enum point_keys {C0, p2};
	vector<string> pnt_key;		// ������ ����� �������� �����
	vector<string> param_key;	// ������ ����� �������� ����������
	
	double tsim;
	double f_dat;
	double t_rascheta;
	bool   bLock;
	CalcData  data;
	bool   dataGenerated;
	bool   disabled;
	WPPolet();
	void Init();				//  ������������� ������� �����, ����������, ������ ����� ��
    void GenData(Pnt start_point);
	void DrawData(int pointkey);
    Pnt  getTrajectoryPoint(double t);
    Pnt  getTrajectoryAngles(double t);
	void SetData(int Ni, point_keys point, PlaneSim_Output& sim_out);
	void SmoothData(int Ni, double dt, point_keys point);
	void RenderTrajectory(int pointkey);
	void RenderWaypoints();
	void AssignWaypointsCoords();
	void RenderLAPoints();
	void PrintInfo();			// ����� ������� �����, ����� ����� �� � ���� �������
	void CalcStep( double t0, double t1,  double Ttek, double dt,  int i,  CalcData& out, PoletPoint p0, PoletPoint p1 );
	void AddWaypoint(int i, string tab);
	void AddVertex(Pnt S);
	void RenderTrajectory();
	void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out );
	Fca						 fca;
	vector<PoletPoint>		 waypoints;
	vector<LAPoint>			 la_points;
	

	#define MAX_LA_POINTS		 100 // max 100 �����
    #define MAX_FLY_PARAMS		 15
	FlyData* 				 DATA[MAX_LA_POINTS][MAX_FLY_PARAMS];			
	float		  vertices[200000];
	unsigned int  indexes[200000];
	double		  Time_domain[200000];
	int			  vert_count;
	bool		 bIsCanRender;
	// ���� � ������� ������� �� ��������_�����.��������
	//			      ��������   Ptpa.vx Ptpa.ay
	map<string, FlyData* >   DATA_MAP;

};

