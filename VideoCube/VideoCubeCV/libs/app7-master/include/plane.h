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

// Особые точки ЛА важные для модели навигационной системы
struct LANavigationPoints 
{
	Pnt Pc0;    // точка - центра масс
	Pnt Psns;   // точка установки СНС
	Pnt Ptpa;   // точка крепления антены

};

// геометрические характеристики ЛА
struct LAGeometry
{
	double      L;					// длинна ЛА
	double      xr;					// длинна от центра до носа
	double      xh;                 // длинна от центра до хвоста
};

// обьект внутри app5 для задания точек
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
	
	LAGeometry			geom;				 // геометрия ЛА
	LAPoints*			la_pts;
//	LANavigationPoints  nav_pts;             // точки ЛА важные для модели навигационной системы
	Pnt&				pos;				 
	Pnt&				v;					 //  состовляющие вектора ск-ти
	Pnt&				a;					 //  состовляющие вектора ускорения
	Angles&				angles;				 //  углы от-но OGL  x - крен   roll
									 //					 y - курс   yaw 
									 //				 	 z - тангаж pitch 
	Pnt		    rot0;			    //   начальные углы поворота модели
	Pnt			camPos;				 //  позиция камеры в самолете от-но самолета
	float		Psi;				 //  курс
	float		Hr;					 //  высота 
	float		fuel;				 //  топливо
    int         mode;                // 0-законы 1-траектория 2-джойстик


	// Переменные управления
	float&	    trac;			  	 // тяга (устаревшее)
	float&      ruleRoll;            // рули крена
	float&      rulePitch;           // рули тангажа
	float&      ruleRollW;			 // вес
	float&      rulePitchW;			 // вес
	float&		ruleTrottleUp;		 // тяга вверх
	float&		ruleTrottleDown;	 // тяга вниз	
	GLfloat		color[4];			 //  цвет

    double	    t;					// время
	double	    dt;					// время с предыдущего запуска расчетов
	double		_t1, _t2, _freq;    // temps
	bool		bSimulate;
	bool		bRenderOSD;
    long        time1, time2;
	// Константы 
	int			algon;			// 0 - полет с постоянной скоростью
	float       RuleReturnTime; // время возврата рулей

	//Текцщее состояние симулятора
	PlaneSim_Input  sim_in;

	//Текущее состояние генератора
	//PlaneGen_Input  gen_in;

	// Выход симулятора или генератора
	PlaneSim_Output sim_out;
    char            gstr[1024];

	


	// Агрегируемые обьекты
	Camera*			cam;
	GemObject*		model;				// полигональная сетка

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

