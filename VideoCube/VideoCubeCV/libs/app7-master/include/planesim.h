#ifndef PlaneSIM_H
#define PlaneSIM_H

#include "geometry.h"
#include <map>


//������� ��������� �����
struct Rule_Input
{
    Rule_Input()
    {
        ruleRoll = 0;
        rulePitch = 0;
        RollW = 0.4f;
        PitchW = 0.6f;
        TrottleW = 0.5f;
    }
    Rule_Input(float _ruleRoll, float _rulePitch)
        : ruleRoll(_ruleRoll), rulePitch(_rulePitch)
    {}
    float      ruleRoll;            // ���� �����
    float      rulePitch;            // ���� �������
    float      ruleTrottleUp;			// ���� ����
    float      ruleTrottleDown;			// ���� ����
    float      RollW;			 // ���
    float      PitchW;			// ���
    float      TrottleW;		//���
};



//��������� ���������
struct Atmosphere
{
    Atmosphere();
    // ������� ������������
    //	double Level;  // ����������� �������
    //	double prob; // �������
    std::map<double, double> probOfLvl;
    float      wRoll[10];     // ��������� ���� ���� �� �����
    float      wPitch[10];    // ��������� ���� ���� �� �������
    float	   laminar;       // ������� �����������  ����. ������ (����������)
    float	   turbulent;     // ������� ������������ ����. ������ (�� ��� �������)
    Pnt		   veterDir;      // �����������	�����
    float	   vVetra;        // �������� �����

    bool       applyRandom;
    bool	   applyTurbulence;
    bool       applyVeter;



};

struct PlaneSim_Output;
struct PlaneSim_Input;

//������������� ��������� ����������
#pragma pack(push, 1)
struct PlaneSim_Output
{
    PlaneSim_Output()
    {
        S = Pnt(0,0,0); // ������� ����������
        a = S;			// ��������
        v = S;			// ���������
        angles = S;
    }
    PlaneSim_Output(Pnt &_S)
        :S(_S)
    {  }
    //�������������� �� ������
    void AssignFromInput(PlaneSim_Input& in);


    Pnt S;				// ������� ����������
    Pnt a;				// ������� ���������
    Pnt v;				// ������� ��������
    Pnt w;				// ������������ ��������    
    Angles	angles;    // ������� ����( ���� ���� ������)
    Pnt     ang;
};
#pragma pack(pop)

//������� ��������� ����������
struct PlaneSim_Input
{
    PlaneSim_Input()
    {
        v = Pnt(0,0,0); a = Pnt(0,0,0); S = Pnt(0,0,0);
    }
    PlaneSim_Input(Pnt &_v, Pnt &_a, Pnt &_S)
        :v(_v), a(_a),S(_S)
    {  }
    //�������������� �� ������
    void AssignFromOutput(PlaneSim_Output& out);
    float   mass; // ����� ��������
    float	trac; // ����
    Pnt		v;    // ������� ��������
    Pnt		a;    // ������� ���������
    Pnt		S;    // ������� ����������
    Angles	angles;    // ������� ����( ���� ���� ������)
    Pnt     ang;
    Rule_Input rule;  // ��������� �����

    // ��������� ��������
    double	Y;	  // ��������� ����
    double	G;    // ���� �������


};

void InitPlaneSimulation( PlaneSim_Input& sim_in,  PlaneSim_Output& sim_out  );
void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out );
void ApplyAtmosphere(Atmosphere& atm, Angles& agl, Pnt& S);

#endif
