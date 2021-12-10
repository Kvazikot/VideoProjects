#ifndef Curve_H
#define Curve_H

#include "group.h"
#include "vpnt.h"
#include "str_utils.h"

extern string curve_algos;
extern string atr_sets;
extern string nullstring;

class Curve : public Group
{
  //      Q_OBJECT
public:
// Constructors


	Curve();
    Curve(const Curve& B){ R = B.R;	}
	void Copy(Object* A);

    string nullstring;
    char gstr[1024];
	float tlastselect;  // врем€ выбора обьекта 
	float R;		   // радиус
	float a,b,c,d;    // прочие параметры    
	Pnt	  rot;  // углы поворота 
	Pnt   scale;
    Pnt   SeverP;
    float HT;  // высота траектории
    float Vsr; // средн€€ скорость
    float color[4];
	string args;	// аргументы NewPoint
	float step;		//шаг при сплайновой интерпол€ции (колво точек сплайна)
	bool  bClosed;  //замкнута€
	int   algon;   // алгоритм линии \ сплайн \ формула
	int	  Npts;   // количество опорных точек кривой
	int	  Nspl;   // количество точек в сплайне
	bool  bShowPoints;
	bool  bShowLines;
    bool  bShowControlPoints;
    int   n_attr_set;	// номер множества аттрибутов
	bool  bMark;    // если 1 отобразить выделение обьекта
	int	  gentime;
	

    vector<VPnt*> pts;   // визуальные точки траектории
	vector<Pnt> pts_or; //оригинальные значени€ без scale
	vector<Pnt> spl;
	vector<Pnt> pts2;  //дл€ параметрических кривых
	//vector<Pnt> ptsvals;
    //virtual void Serialize( CArchive& archive );
    virtual void Render(QOpenGLShaderProgram& program);
    VPnt* NewPoint(int param);
	void DelLastPoint();
    void DeleteWaypoints();
	void MakeSpline();
    void MakeCantenaryLine( double a, double b, double c, double R);
	void SelectAtrSet(int idx);
	void ShowAllAtrsFunc(bool b);
	void Load(string& s);
	void Save(string& s);
	void MakeSpiral();
	void Scale();
	void Rotate();
    void click_rmb(int state);
    void InitWaypoints();


#define HIDE atrs.back().Hide();
	
	void Declare()
	{
		if(name=="")
		  SetName("curve01");
		
		SET_OBJECT(this,(char*)name.c_str());
		//SET_METHOD("Object","int","Curve","NewPoint2",(void*)&NewPoint);
		//SET_ATTR("ShowAllAtrs", &bShowAllAtrs, "bool");		 
        SET_ATTR("Filter", (void*)&atr_sets, "selector");
		SET_ATTR("NewPoint", &args, "func"); atrs.back().NotSaving();
        SET_ATTR("NewRandomPoint", &nullstring, "func"); atrs.back().ReadOnly(); atrs.back().NotSaving();
		SET_ATTR("DeleteLastPoint", &nullstring, "func");atrs.back().ReadOnly(); atrs.back().NotSaving();
        //SET_ATTR("MakeSpiral", &nullstring, "func");atrs.back().ReadOnly(); atrs.back().NotSaving();
        //SET_ATTR("MakeDuga", &nullstring, "func");atrs.back().ReadOnly(); atrs.back().NotSaving();
        SET_ATTR("InitWaypoints", &nullstring, "func");atrs.back().ReadOnly(); atrs.back().NotSaving();
        SET_ATTR("DeleteWaypoints", &nullstring, "func");atrs.back().ReadOnly(); atrs.back().NotSaving();

        SET_ATTR("R", &R, "float");
        SET_ATTR("HT", &HT, "float");
        SET_ATTR("Vsr", &Vsr, "float");
        SET_ATTR("SeverP", &SeverP, "Pnt");

        SET_ATTR("ShowLines", &bShowLines, "bool");        
        SET_ATTR("step", &step, "float");      HIDE;
        SET_ATTR("color", &color, "GLfloat*"); HIDE;
        SET_ATTR("scale", &scale, "Pnt");      HIDE;
        SET_ATTR("rotate", &rot, "Pnt");       HIDE;
        SET_ATTR("Npts", &Npts, "int"); atrs.back().ReadOnly(); atrs.back().NotSaving();
        SET_ATTR("Nspl", &Nspl, "int"); atrs.back().ReadOnly(); atrs.back().NotSaving();
		SET_ATTR("algon", &algon, "int"); //atrs.back().ReadOnly(); atrs.back().Hide(); HIDE
		//SET_ATTR("ei.coord", &algon, "int"); //atrs.back().ReadOnly(); atrs.back().Hide();
        SET_ATTR("algo", (void*)&curve_algos, "selector");atrs.back().NotSaving();
        SET_ATTR("Closed", &bClosed, "bool");
        SET_ATTR("ShowPoints", &bShowPoints, "bool");
		//SET_METHOD("Object", "void",(char*)classname.c_str(), "NewPoint", NewPoint);		
		//SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "Scale", Scale);
		//SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "Rotate", Rotate);

		// fill entety info	
        //CRuntimeClass* pMyRTClass= GetRuntimeClass();
		ei.name = name;
        ei.classname = "Curve";
		ei.groupname = "Curves";
        //sprintf(gstr,"%d", this);
        //ei.GUID = gstr;
		ei.material = "no material";
		ei.model = "no model";
		
		bHideStdAttrs = true;
		Group::Declare();
		Object::Declare();
		classname = "Curve";
	}

    void Slot1(); //timer slot
	void OnAttrChange(t_atr& a);
	void OnAttrSelect(t_atr& a);
	void OnSelect();
	void OnUnselect();
protected:

};

#endif 

