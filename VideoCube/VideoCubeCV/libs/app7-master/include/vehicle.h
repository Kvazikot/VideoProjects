#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "demper.H"
#include "group.h"
#include "mterrain.h"
#include "curve.h"
#include "gemobject.h"
//#include "Wm5IntrTriangle3Box3.h"
#include <tuple>
#include <QFile>

extern string kompass_dir; 

struct TerrainObject
{
	Pnt pos;
	Pnt rot;
	Pnt p_near;    // ��������� \ ���������	
	bool bFindNewPoint;
	double Hc;
	int near_idx; // ������ ��������� �����
	int prev_idx;  // ����� ���������� ��������� �����
	double ts,te,H0,H1,Ht,Htt,Httt;  // ��� ������������ ������
    GemObject* mesh;
    MTerrain*   ter;
    //vector<Wm5::Triangle3<double> > nfaces;
    bool bCollision;
    float colis_fix;


	TerrainObject()
	{
		Hc = 0;
        bCollision = false;
        colis_fix = 0;
	}
	char* toStr()
	{
        extern char gstr[10000];
		sprintf(gstr, "H0=%2.4lf H1=%2.4lf H=%2.4lf Htt=%2.4lf Httt=%2.4lf near_idx=%d",H0,H1,Ht,Htt,Httt,near_idx );
		return gstr;
	}
    void SetTerrain(MTerrain* t){ ter = t; }
    void SetOnTerrain(Pnt& p_near_f,float t_rascheta);
    void FindClosestFaces(PPnt* ps);
    void DetectColision();
	Pnt CalcDemphing(TerrainObject& tobj);	// ������� ��������� ��������������
	void AddConstantHeight(double Hconst){Hc = Hconst;}
    void Render();
};

class CVehicle : public Group
{
    Q_OBJECT
public:

	int      num;     // �����
	Pnt      pos;
	Pnt		 kurs;    // ����
	Pnt		 kurs_arrow;
	Pnt		 kren;    // ����
    bool     stopSignal;
	float    r;       // ������ ����� 
	float	 speed;   // ���������� �������� � \ �
	float	 ffw;     //������� �������� �������� ����� ��
	float	 fbw;     //������� �������� ������ �����   ��
	int      dir;     //������=-1 �����=1
	float	 S;      // ���������� ���� (�� ����� ttek - t0) � �.
	float    t0;     // ����� ������� �������� ��� ������ OnStart
	float    wp_delta; //���������� ����� �����������
	float    trenie;
	float    wang;
	float    max_wang;
	float    h_dempers;  // ������ ��������� ��������������
	float    delkoof;
    float    porog;
	string   curvesnames;
	int		 icurve;		//������ ����������
	//Pnt      campts[2][24];   
    //PivotPoint	pp1,pp2;	  // ����� ��������� ������ ������(16 ����� �� �������� �����)
	Pnt campts1[24];
    Pnt select_pointz[2];
    vector<std::tuple<Pnt,double> > lookpts;
	int      nCamPnt;		  // ����� ������� ������� ������
	char     wheels_pos_str[200];
	char     wheels_pos_str2[200];
	char     prijina_L_str[200];
	char     prijina_m_str[200];
	char     prijina_tK_str[200];

    //����������� ������������
    float a,b,c,d;

	
	double	    dt;					// ����� � ����������� ������� ��������
	double		_t1, _t2, _freq;    // temps
    QElapsedTimer timer;

	GemObject* w0,*w1,*w2,*w3; //������ ����� 
	GemObject* kuzov;         //�����m ������ 
	Curve*     activeTraj;   // ������� ����������
	VPnt*	   vp,*vp2;			// ��������� ����� 	
	vector<Curve*> trajs;
	vector<Pnt> waipoints;    // ������� �����
	Pnt			target_wp;    // ������� ������� �����
	bool		bReachTargetWp; //�������� ����� ���������� target_wp
	bool        bShowWaipoints;
	string		camPnt;			
	bool bGoToFinish;		// ���� ������ �� ����������
    bool bIsReceiveInput;
	

	//��� ����� �� ����������
	Pnt p_near;    // ��������� \ ���������	
	float t_rascheta; // ����� ��������� �����
	int near_idx; // ������ ��������� �����
	int prev_idx;  // ����� ���������� ��������� �����	
	double timeNewP;
    string nullstring;

	vector<TerrainObject> terrain_objs;


	Pnt P, Z, WW;
	float length_wheels, width_wheels;	//������ ���� �����

	//�������
	Demper*  dm[4]; 

	// ������ ������
	bool bDetectWalls;
	bool bGoByWaypoints;

    CVehicle();
	void SetCamPoints();
    void LoadDefModels();
	int LoadModels(string kuzname, string wn);
	void Reset();
	char* PrintfWheelPos(Pnt& w0p, Pnt& w1p, Pnt& w2p, Pnt& w3p);
	void AssignTrajectory();
	void Krivizna();
	void InputControl();
	void RenderOSD();
	void ListCurves();
	void Trenie();
	void LimitKurs();
	void KursArrow();
	void WheelReturn();
    void SetMatrix(GemObject* mesh);
	void OnStart();     // �� ����� ����������
	void FindNearPnt(Curve* traj, Pnt& p, int& idx); // ����� ���� ����� ����������
	int FindForward(Curve* traj, int pi_from, Pnt& p_to, float dist); // ����� ������ �� ���������� �� �������� ���������
	int FindBackward(Curve* traj, int pi_from, Pnt& p_to, float dist);// ����� ����� �� ���������� �� �������� ���������
	int FindWaipoints(Curve* traj, int pi_from, float d, float maxdist, vector<Pnt>& waipoints);
	void FindNearWayPoint(vector<Pnt>& waipoints, Pnt& p, int& idx);
    Pnt  takePointAtDist(float dist);
    bool CheckReachTargetWp();
    bool hasColision();
	
	//������� ��������
	void CalcPodveska();
	void SetOnTerrain(TerrainObject& tobj); // ����� �������� ����� ����� 1 ���. � ����������������� 
	void FitWheesInPlane(Pnt& w0p, Pnt& w1p, Pnt& w2p, Pnt& w3p );	// ������������ ����� �� ���������
	int FitWheel(Pnt w0p, Pnt w1p, Pnt w2p, Pnt w3p, int max_iters, int n, Pnt& Res, float dy, float prec ); // ��������� ���� ������ (����� ���-�� ��������)
	void CalcDemphing(TerrainObject& tobj);			// ������� ��������� ��������������
	void Svadba(float Hw);
	


    void func3(){};
	void func4(){};

	void Declare()
	{
		classname = "CVehicle";
		if(name =="")
		  SetName("veh01");

		//SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "move", move);
		//SET_METHOD("Object", "int",(char*)classname.c_str(), "LoadDefModels", LoadDefModels);
		SET_OBJECT(this,(char*)name.c_str());

		
		bHideStdAttrs = true;
		bool bHide = false;
		//SET_ATTR("num", &num, "int", bHide);		
		//SET_ATTR("campos", &kompass_dir, "selector", bHide);
		SET_ATTR("name", &name, "string", bHide);
		SET_ATTR("OnStart",&nullstring,"func", bHide);
		//SET_ATTR("Krivizna",&nullstring,"func", bHide);
		SET_ATTR("pos", &ei.coord, "Pnt", bHide);
		SET_ATTR("trenie", &trenie, "float", bHide);
        //SET_ATTR("speed", &v.x, "float", bHide);
		SET_ATTR("wp_delta", &wp_delta, "float", bHide); 
        //SET_ATTR("trajectory",&curvesnames, "selector", bHide);
		SET_ATTR("delkoof", &delkoof, "float", bHide);
        SET_ATTR("stopSignal", &stopSignal, "bool", bHide);
        SET_ATTR("a", &a, "float", bHide);
        SET_ATTR("b", &b, "float", bHide);
        SET_ATTR("c", &c, "float", bHide);
        SET_ATTR("d", &d, "float", bHide);

        //SET_ATTR("t_rascheta", &t_rascheta, "float", bHide);
        //SET_ATTR("porog", &porog, "float", bHide);
        SET_ATTR("kurs", &kurs, "Pnt", bHide);
        //SET_ATTR("kurs_arrow", &kurs_arrow, "Pnt", bHide);
        //SET_ATTR("kren", &kren, "Pnt", bHide);
        SET_ATTR("ffw", &ffw, "float", bHide);
        SET_ATTR("fbw", &fbw, "float", bHide);
        //SET_ATTR("dir", &dir, "int", bHide);
		SET_ATTR("wang", &wang, "float", bHide);
		SET_ATTR("max_wang", &max_wang, "float", bHide);
        //SET_ATTR("h_dempers", &h_dempers, "float", bHide);

        //SET_ATTR("ShowWaipoints", &bShowWaipoints, "bool", bHide);
        //SET_ATTR("bDetectWalls", &bDetectWalls, "bool", bHide);
        //SET_ATTR("bGoByWaypoints", &bGoByWaypoints, "bool", bHide);

		
		// fill entety info	
		ei.name = name;
        ei.classname = "CVehicle";
		ei.groupname = "Vehicles";
        //sprintf(gstr,"%d", this);
        //ei.GUID = gstr;
		ei.material = "no material";
		ei.model = "no model";

		Group::Declare();
		Object::Declare();

		//	CALL("cam1", (char*)classname.c_str(), "move2");
	}
	
	void NullReferences();

	// [11]
    void move(float dx, float dy, float dz)
    {
        ei.coord.x+=dx;
        ei.coord.y+=dy;
        ei.coord.z+=dz;
    }
    virtual void Slot1(); //timer slot
    void Render(QOpenGLShaderProgram& program);
	void OnSelect();
	void OnUnselect();
	void OnReceiveInput();
	void OnAttrChange(t_atr& a);

public slots:
    void resetTrenie();
};

CVehicle* asVeh(Object* o);

#endif
