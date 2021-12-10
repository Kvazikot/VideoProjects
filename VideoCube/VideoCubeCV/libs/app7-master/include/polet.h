#pragma once

#include "autopilot.h"    // для FlyData*
#include "planesim.h" 

#define	g 9.8L

// Точка фазового центра
struct Fca
{
	Pnt     coord;	// счисляемая точка ФЦА
	double  r0;     // расстояние от центра самолета
	double  phi;	// угол вектора ФЦА

};
// Узловая точка полета
struct PoletPoint
{
	int    i;		// номер точки
	double t;		// время определения точки
	double H;		// высота точки
	double We;		// скорость
	double psi0;    // истинный курс
	
	Pnt     coord; // расчитанная координата (выходные данные)
};

// Точка 
class LAPoint : public Pnt
{
	LAPoint();
	string key;
};


// Данные интерполяционного сплайна и парметров
// для одного шага
struct CalcData
{
	CalcData(){};
	double H;	  // высота
	double We;	  // скорость
	double WWe;  // ускорение
	double WWWe;
	double a[3];  // углы 
	double aa[3]; // производные углов

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

// Полет по путевым точкам
struct WPPolet
{
    enum param_keys {roll, yaw, pitch, ax, ay, az, vx, vy, vz, x, y, z, we, wh, wn};
	enum point_keys {C0, p2};
	vector<string> pnt_key;		// массив строк названий точек
	vector<string> param_key;	// массив строк названий параметров
	
	double tsim;
	double f_dat;
	double t_rascheta;
	bool   bLock;
	CalcData  data;
	bool   dataGenerated;
	bool   disabled;
	WPPolet();
	void Init();				//  инициализация путевых точек, микроплана, особых точек ЛА
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
	void PrintInfo();			// вывод путевых точек, вывод точек ЛА в виде таблицы
	void CalcStep( double t0, double t1,  double Ttek, double dt,  int i,  CalcData& out, PoletPoint p0, PoletPoint p1 );
	void AddWaypoint(int i, string tab);
	void AddVertex(Pnt S);
	void RenderTrajectory();
	void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out );
	Fca						 fca;
	vector<PoletPoint>		 waypoints;
	vector<LAPoint>			 la_points;
	

	#define MAX_LA_POINTS		 100 // max 100 точек
    #define MAX_FLY_PARAMS		 15
	FlyData* 				 DATA[MAX_LA_POINTS][MAX_FLY_PARAMS];			
	float		  vertices[200000];
	unsigned int  indexes[200000];
	double		  Time_domain[200000];
	int			  vert_count;
	bool		 bIsCanRender;
	// ключ в таблице состоит из НАЗВАНИЕ_ТОЧКИ.ПАРАМЕТР
	//			      например   Ptpa.vx Ptpa.ay
	map<string, FlyData* >   DATA_MAP;

};

