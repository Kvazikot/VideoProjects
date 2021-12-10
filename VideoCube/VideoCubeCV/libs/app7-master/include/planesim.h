#ifndef PlaneSIM_H
#define PlaneSIM_H

#include "geometry.h"
#include <map>


//Текущее состояние рулей
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
    float      ruleRoll;            // рули крена
    float      rulePitch;            // рули тангажа
    float      ruleTrottleUp;			// руль тяга
    float      ruleTrottleDown;			// руль тяга
    float      RollW;			 // вес
    float      PitchW;			// вес
    float      TrottleW;		//вес
};



//Параметры атмосферы
struct Atmosphere
{
    Atmosphere();
    // Таблица вероятностей
    //	double Level;  // вероятность влияния
    //	double prob; // уровень
    std::map<double, double> probOfLvl;
    float      wRoll[10];     // случайный уход угла по крену
    float      wPitch[10];    // случайный уход угла по тангажу
    float	   laminar;       // уровень ламинарного  возд. потока (продольный)
    float	   turbulent;     // уровень турбулентого возд. потока (во все стороны)
    Pnt		   veterDir;      // направление	ветра
    float	   vVetra;        // скорость ветра

    bool       applyRandom;
    bool	   applyTurbulence;
    bool       applyVeter;



};

struct PlaneSim_Output;
struct PlaneSim_Input;

//Расчитываемое состояние симулятора
#pragma pack(push, 1)
struct PlaneSim_Output
{
    PlaneSim_Output()
    {
        S = Pnt(0,0,0); // текущая координата
        a = S;			// скорости
        v = S;			// ускорения
        angles = S;
    }
    PlaneSim_Output(Pnt &_S)
        :S(_S)
    {  }
    //переприсвоение из выхода
    void AssignFromInput(PlaneSim_Input& in);


    Pnt S;				// текущее координаты
    Pnt a;				// текущее ускорение
    Pnt v;				// текущая скорость
    Pnt w;				// вращательная скорость    
    Angles	angles;    // текущие углы( курс крен тангаж)
    Pnt     ang;
};
#pragma pack(pop)

//Текцщее состояние симулятора
struct PlaneSim_Input
{
    PlaneSim_Input()
    {
        v = Pnt(0,0,0); a = Pnt(0,0,0); S = Pnt(0,0,0);
    }
    PlaneSim_Input(Pnt &_v, Pnt &_a, Pnt &_S)
        :v(_v), a(_a),S(_S)
    {  }
    //переприсвоение из выхода
    void AssignFromOutput(PlaneSim_Output& out);
    float   mass; // масса самолета
    float	trac; // тяга
    Pnt		v;    // текущая скорость
    Pnt		a;    // текущее ускорение
    Pnt		S;    // текущая координата
    Angles	angles;    // текущие углы( курс крен тангаж)
    Pnt     ang;
    Rule_Input rule;  // состояние рулей

    // Расчетные величины
    double	Y;	  // подьемная сила
    double	G;    // сила тяжести


};

void InitPlaneSimulation( PlaneSim_Input& sim_in,  PlaneSim_Output& sim_out  );
void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out );
void ApplyAtmosphere(Atmosphere& atm, Angles& agl, Pnt& S);

#endif
