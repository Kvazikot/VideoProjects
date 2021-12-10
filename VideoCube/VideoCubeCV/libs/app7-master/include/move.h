#ifndef __MOVE_H__
#define __MOVE_H__

#include "geometry.h"
#include "object.h"
#include <stdio.h>
//====================================================================
//                  ��������                  
//====================================================================
enum TrajectoryType
{
	T_CIRCULAR=0, // �� �����
	T_LINEAR,     // �� ������
	T_SHAPE,      // �� ������
	T_COMPLEX
};

enum MovementLaw
{
	LAW_LINEAR,     // ��������
	LAW_QUADRATIC   // ���������������

};

struct MotionData
{
	TrajectoryType ttype;
	MovementLaw	   law;
	//Shape		   shape;

	float a;   // ���������
	float R;   // ������
	Pnt	  Co;  // �����
	Pnt   V;   // ��������
	Pnt   pos; // �������
	Pnt   rot; // ���� ��������
	float f;   // �������

};

struct t_objpair
{
	t_objpair()
	{ cnt = 0; }
	Object* obj1;
	Object* obj2;
	int     cnt;
};

class CMovement : public Object
{
public:

	CMovement();
    CMovement(const CMovement& B){ trenie = B.trenie;};
	void Reset();
	map<int, int>	 factorialmap;
	map<string, t_objpair>	 colmap;
	map<string, t_objpair>	 objpairs;
	map<Object*, MotionData> motmap; // ������� ����������� ����������
	//��������� �����
	float trenie;
	string colisstr;
    char   gstr[1024];

	int Ckn(int k, int n);
	void CollisonDetected(Object* obj1, Object* obj2); // ���������� ��� ����� ��������
	void CollisonNotDetected(Object* obj1, Object* obj2); // ���������� ��� ���������� ��������
    //bool PredictionCollision(Object* obj1, Object* obj2, float maxtime, Pnt* pc, float* time){};
	bool CheckCollison(Object* obj1, Object* obj2);
	void Decolise(Object* obj1, Object* obj2);
	void BallColision( Object* obj1, Object* obj2 ); 
	void ProcessCircular(MotionData* md);
	void SchislenieCoord(Object* obj); // ��������� ����������� �������� ���� ������ �� ���������� �������� 
	void AssignCircular(Object* obj, Pnt Co, float f, float R);
	void CalcGravity(Object* obj);
	void CalcTrenie(Object* obj);
	void Declare()
	{
		
		classname = "CMovement";
		//if(name=="")
		  SetName("mov01");

		

		//SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "move2", move2);
		SET_OBJECT(this,(char*)name.c_str());
		SET_ATTR("name", &name, "string");
		SET_ATTR("trenie", &trenie, "float");


		// fill entety info	
		ei.name = name;
        ei.classname = "CMovement";
		ei.groupname = "Movement";
        sprintf(gstr,"%d", this);
		ei.GUID = gstr;
		ei.material = "no material";
		ei.model = "no model";
		//ei.
	}



    void Slot1(); //timer slot

};

#endif
