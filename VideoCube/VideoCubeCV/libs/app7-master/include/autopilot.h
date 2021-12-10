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
	double t_start; // ����� ������ �������
	double t_end;   // ����� ����� �������
};

struct Intervl
{
	Intervl(){};
	Intervl(int i0,int i1):i_start(i0),i_end(i1){}
	int i_start; // ����� ������ �������
	int i_end;   // ����� ����� �������
};


//"�������������� �����" "����� ������"  "��������" "����� ��������" "������ ��������" "����� ���������"
enum autopilot_state {NullState, HorizFly, Climb, Descent, TurnLeft, TurnRight, Nesterov, MidState}; 
enum params {roll, yaw, pitch, ax, ay, az, vx, vy, vz, x, y, z};


//������� ���������
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
	string			state_name;		 // �������� ���������
	autopilot_state	apilot_state;	 // ����� ��������� ����������
	vector<FlyData*>	 DATA;		// ��� ������ ��������� (�����, ������, ��������)
	map<string,FlyData*> DATA_MAP;  //---||--- � ���� �������
	FlyData*  GetData(string param_key); // ����� ������ ��� ��������� ("roll", "ax")

	double  Time_domain[200000];	// ����� �������
	double  t_rascheta;				// ����� �� ������� ������������ ������
	int		Ndef;					// ���������� �������� ����� (��������� ���������� �������)
	double  tv;						// ��������� ����� ������� �������� (���������� � 0)
	
	PlaneSim_Input	sim_in;		// ��������� ��������� ���������� �� �������� ������� ������
	PlaneSim_Output	sim_out;	// ��������� ���������� �� ������ �� ����
	void    Calc(PlaneSim_Input in); // ������ ���������� �� ������ �� my_table
	void	PlaneSimulation(double t, double dt, PlaneSim_Output& sim_out); // ������ ��������� ���������� �� ������ t
	inline void SetValues(int Ni, Pnt& S,  Pnt& v, Pnt& a, Angles& ang);
	void	SetNi(int Ni);
	void	SmoothData(int Ni);
	void	NullData(int key, Intervl inv);
	void	ConstData(int key, double val, Intervl inv);
	void	GenData(int key, Intervl inv, Law law);
	void	SmartSmoothData(vector<StateTimes> times, vector<vector<int> > Nis); // ���������� ������ ������ ��������� �������� �����. �� �������
	void	Reset();
	
	//���������� ��� ��������(���������)
	Pnt v,S,a,v0,v1,a0,a1,A;
	Angles angles;
	double H,Hup,Hdown,aY,vY,R,N,M,dAlfa,alfa;


	
};


struct FlyProgram 
{
	vector<FlyState*>   fly_states;	// ����������� ���� ������
	vector<StateTimes> times;       // ����� ����� ��� ����������
	vector<vector<int> > Nis;			// ������ ���������� ���� ���������

	vector<FlyState*>   fly_states_complete;	// ����������� � ���������� ���� ������
	FlyState*			complete_state;			// ���� ����� � ����� ���������

	~FlyProgram();
	// �������� ���� 
	FlyState* AddFlyState( string tab, autopilot_state state, string state_name, double t_rascheta);			
	FlyState* CreateMiddleState(FlyState* cur, FlyState* fut);
	FlyState* JoinStates(vector<FlyState*> states_vector);
	void CalcMiddleStates();
	void CalcProgram();
	void CalcAndDraw();					// ���������� ��������� ������ �� �������
	void LoadFromTable();	
	void DrawStates(vector<FlyState*> states_vector);
	void LoadFromFile(string fn); 
	void Kill();

};

class Autopilot : public Object
{
public:
	Autopilot();
	vector<Transition>		transitions; // �������� ������� ������ �������
	FlyProgram	program;
	int			 cur_stateN;	// ����� �������� ���������
	bool		 disabled;
	autopilot_state	state;
	FlyState*    cur_state;	 // ������� ���� ������	�� ���������
	FlyState*    pre_state;  // ���������� ���� ������
	FlyState*    new_state;  // ��������� ���� ������ �� ���������
	double		 tsim;		// ���������� ������� ��� ��������� � 3d ��������� �� OSD)

	PlaneSim_Input sim_in;	// ��������� ����� ���������� �� ��� ������ �������
	PlaneSim_Output sim_out; // ��������� ������ ���������� �� ��� ������ �������

	void Init();			//������������� ��������� ����������
	void DoTransition( autopilot_state from, autopilot_state to );
	void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out );
	void CheckTransition();

	//
	
};
