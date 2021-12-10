#pragma once

#include "param_screen_bcb.h"
//#include "traj_gen.h"
#include "planesim.h"
#include "interp.h"
#include <string>
#include "law.h"

using namespace std;

struct FlyData
{
	FlyData(){ };
	FlyData(string k)
	{
		key = k;
		law.name = k + "law";
		Ni = 0;
	}
	string  key;
	Law		law;
	Spline  spline;
	int     Ni;
	double	arr[200000];

};

struct StateTimes
{
	StateTimes(){};
	StateTimes(double t0,double t1):t_start(t0),t_end(t1){}
	double t_start; // время начала маневра
	double t_end;   // время конца маневра
};

struct Intervl
{
	Intervl(){};
	Intervl(int i0,int i1):i_start(i0),i_end(i1){}
	int i_start; // отчет начала маневра
	int i_end;   // отчет конца маневра
};


//"Горизонтальный полет" "Набор высоты"  "Снижение" "Левый разворот" "Правый разворот" "Петля Нестерова"
enum autopilot_state {NullState, HorizFly, Climb, Descent, TurnLeft, TurnRight, Nesterov, MidState}; 
enum params {roll, yaw, pitch, ax, ay, az, vx, vy, vz, x, y, z};


//переход состояния
struct Transition
{
	Transition()
	{
		from = -1;		err = 0;		to = -1;
	}
	Transition(int f, int t, int* s): from(f), to(t), state(s) 
	{	
		Transition();
		if( f == t) err = 1;
	}
	void Do()	{		*state = to;	}
	void Print();
	int from;  
	int to;
	int err;
	int* state;
};

class FlyState : public TableObj
{
public:
	FlyState();
	
	FlyState(FlyState& B);

	~FlyState();
	string			state_name;		 // название состояния
	autopilot_state	apilot_state;	 // номер состояния автопилота
	vector<FlyData*>	 DATA;		// все данные состояния (закон, сплайн, значения)
	map<string,FlyData*> DATA_MAP;  //---||--- в виде таблицы
	FlyData*  GetData(string param_key); // взять данные для параметра ("roll", "ax")

	double  Time_domain[200000];	// базис времени
	double  t_rascheta;				// время на которое генерируется сигнал
	int		Ndef;					// количество исходных точек (дефолтное разрешение сплайна)
	double  tv;						// внутренне время расчета состояни (НАЧИНАЕТСЯ с 0)
	
	PlaneSim_Input	sim_in;		// начальное состояние симулятора от которого ведется расчет
	PlaneSim_Output	sim_out;	// состояние симулятора на выходе из фазы
	void    Calc(PlaneSim_Input in); // расчет траектории по данным из my_table
	void	PlaneSimulation(double t, double dt, PlaneSim_Output& sim_out); // выдать состояние симулятора на момент t
	inline void SetValues(int Ni, Pnt& S,  Pnt& v, Pnt& a, Angles& ang);
	void	SetNi(int Ni);
	void	SmoothData(int Ni);
	void	NullData(int key, Intervl inv);
	void	ConstData(int key, double val, Intervl inv);
	void	GenData(int key, Intervl inv, Law law);
	void	SmartSmoothData(vector<StateTimes> times, vector<vector<int> > Nis); // исправляет резкие скачки совершает пересчет коорд. по Тейлору
	void	Reset();
	
	//переменные для расчетов(локальные)
	Pnt v,S,a,v0,v1,a0,a1,A;
	Angles angles;
	double H,Hup,Hdown,aY,vY,R,N,M,dAlfa,alfa;


	
};


struct FlyProgram 
{
	vector<FlyState*>   fly_states;	// расчитанные фазы полета
	vector<StateTimes> times;       // время когда они включаются
	vector<vector<int> > Nis;			// отчеты параметров всех состояний

	vector<FlyState*>   fly_states_complete;	// расчитанные и переходные фазы полета
	FlyState*			complete_state;			// весь полет в одном состоянии

	~FlyProgram();
	// добавить фазу 
	FlyState* AddFlyState( string tab, autopilot_state state, string state_name, double t_rascheta);			
	FlyState* CreateMiddleState(FlyState* cur, FlyState* fut);
	FlyState* JoinStates(vector<FlyState*> states_vector);
	void CalcMiddleStates();
	void CalcProgram();
	void CalcAndDraw();					// нарисовать программу полета на графике
	void LoadFromTable();	
	void DrawStates(vector<FlyState*> states_vector);
	void LoadFromFile(string fn); 
	void Kill();

};

class Autopilot : public Object
{
public:
	Autopilot();
	vector<Transition>		transitions; // переходы которые сделал автомат
	FlyProgram	program;
	int			 cur_stateN;	// номер текущего состояния
	bool		 disabled;
	autopilot_state	state;
	FlyState*    cur_state;	 // текущая фаза полета	из программы
	FlyState*    pre_state;  // переходная фаза полета
	FlyState*    new_state;  // следующая фаза полета из программы
	double		 tsim;		// переменная времени при симуляции в 3d выводится на OSD)

	PlaneSim_Input sim_in;	// состояние входа симулятора на тек момент времени
	PlaneSim_Output sim_out; // состояние выхода симулятора на тек момент времени

	void Init();			//инициализация программы автопилота
	void DoTransition( autopilot_state from, autopilot_state to );
	void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out );
	void CheckTransition();

	//
	
};
