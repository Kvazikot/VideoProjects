#ifndef CPlane_H
#define CPlane_H

#include "gemobject.h"
#include "group.h"
#include "vpnt.h"
#include "camera.h"
#include "planesim.h"
#include <time.h>
#include <QFile>

extern string CPlane_polet;

// ������ ����� �� ������ ��� ������ ������������� �������
struct LANavigationPoints 
{
	Pnt Pc0;    // ����� - ������ ����
	Pnt Psns;   // ����� ��������� ���
	Pnt Ptpa;   // ����� ��������� ������

};

// �������������� �������������� ��
struct LAGeometry
{
	double      L;					// ������ ��
	double      xr;					// ������ �� ������ �� ����
	double      xh;                 // ������ �� ������ �� ������
};

// ������ ������ app5 ��� ������� �����
class LAPoints : public RenderObject
{
public:
//	DECLARE_SERIAL( LAPoints )
	
	LANavigationPoints nav_pts;
	LAPoints();
    LAPoints(LAPoints& B)	{ nav_pts = B.nav_pts; }
	void Declare()
	{
		classname = "LAPoints";
	
		if(name=="")
		  SetName("la_pts");
		
		SET_OBJECT(this,(char*)name.c_str());
		SET_ATTR("Pc0", (void*)&nav_pts.Pc0, "Pnt");
		SET_ATTR("Psns", (void*)&nav_pts.Psns, "Pnt");
		SET_ATTR("Ptpa", (void*)&nav_pts.Ptpa, "Pnt");
		bHideStdAttrs = true;

		FillEntetyInfo("Geometry","no material",name);
		RenderObject::Declare();
		//Object::Declare();
	}
	void Render();

};


class CPlane :public Group
{
  //  Q_OBJECT
public:

	CPlane();
    CPlane(const CPlane& B)
        : v(sim_in.v),RuleReturnTime(1),trac(sim_in.trac),bSimulate(1),pos(ei.coord),a(sim_in.a),angles(sim_in.angles),
        ruleRoll(sim_in.rule.ruleRoll), rulePitch(sim_in.rule.rulePitch), ruleRollW(sim_in.rule.RollW), rulePitchW(sim_in.rule.PitchW),
        ruleTrottleUp(sim_in.rule.ruleTrottleUp), ruleTrottleDown(sim_in.rule.ruleTrottleDown)
    {
        pos = B.pos;
    }
	
	LAGeometry			geom;				 // ��������� ��
	LAPoints*			la_pts;
//	LANavigationPoints  nav_pts;             // ����� �� ������ ��� ������ ������������� �������
	Pnt&				pos;				 
	Pnt&				v;					 //  ������������ ������� ��-��
	Pnt&				a;					 //  ������������ ������� ���������
	Angles&				angles;				 //  ���� ��-�� OGL  x - ����   roll
									 //					 y - ����   yaw 
									 //				 	 z - ������ pitch 
	Pnt		    rot0;			    //   ��������� ���� �������� ������
	Pnt			camPos;				 //  ������� ������ � �������� ��-�� ��������
	float		Psi;				 //  ����
	float		Hr;					 //  ������ 
	float		fuel;				 //  �������
    int         mode;                // 0-������ 1-���������� 2-��������


	// ���������� ����������
	float&	    trac;			  	 // ���� (����������)
	float&      ruleRoll;            // ���� �����
	float&      rulePitch;           // ���� �������
	float&      ruleRollW;			 // ���
	float&      rulePitchW;			 // ���
	float&		ruleTrottleUp;		 // ���� �����
	float&		ruleTrottleDown;	 // ���� ����	
	GLfloat		color[4];			 //  ����

    double	    t;					// �����
	double	    dt;					// ����� � ����������� ������� ��������
	double		_t1, _t2, _freq;    // temps
	bool		bSimulate;
	bool		bRenderOSD;
    long        time1, time2;
	// ��������� 
	int			algon;			// 0 - ����� � ���������� ���������
	float       RuleReturnTime; // ����� �������� �����

	//������� ��������� ����������
	PlaneSim_Input  sim_in;

	//������� ��������� ����������
	//PlaneGen_Input  gen_in;

	// ����� ���������� ��� ����������
	PlaneSim_Output sim_out;
    char            gstr[1024];

	


	// ������������ �������
	Camera*			cam;
	GemObject*		model;				// ������������� �����

	void Declare()
	{
		classname = "CPlane";
	
		if(name=="")
		  SetName("plane01");
		
		SET_OBJECT(this,(char*)name.c_str());
		//SET_ATTR("polet", (void*)&CPlane_polet, "selector");
		SET_ATTR("algon", &algon, "int"); 
		SET_ATTR("RuleReturnTimeout", &RuleReturnTime,	 "float"); 
//		SET_ATTR("v",		 &v,		 "Pnt"); 
//		SET_ATTR("a",		 &a,		 "Pnt"); 
//		SET_ATTR("angles",	 &angles,		 "Pnt"); 
//		SET_ATTR("camPos",   &camPos,	 "Pnt"); 
		SET_ATTR("Hr",	     &Hr,		 "float"); 
		SET_ATTR("trac",     &trac,		 "float"); 
		SET_ATTR("rot0",     &rot0,		 "Pnt"); 
//		SET_ATTR("fuel",     &fuel,		 "float"); 	

        SET_ATTR("bSimulate", &bSimulate,	 "bool"); atrs.back().NotSaving();
		

		
		
		
		FillEntetyInfo("Planes","no material",name);
		
		//bHideStdAttrs = true;
		Group::Declare();
		Object::Declare();
	}

	void DrawGrafics();
	void DefineGeometryPoints();
	void InitSimulation();
	void StopSimulation();
	void StartSimulation();
	void SetModelPos();
	void Simulate(double _dt );
	int  LoadModel(string name);
	void InputControl();
	void RuleReturn();
    void Write2SharedMemory(PlaneSim_Output& sim_out);
    void Render(QOpenGLShaderProgram& program);
	void RenderOSD();
	void OnSelect();
	void OnUnselect();
	void OnAttrChange(t_atr& a);
    void Slot1(); //timer slot
	

protected:

};

#endif 

