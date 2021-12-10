#ifndef VPnt_H
#define VPnt_H

#include "renderobject.h"
#include <QOpenGLShaderProgram>

#define ATTR SET_ATTR

class VPnt :public RenderObject, public Pnt
{
public:
// Constructors
	VPnt();
    VPnt(float r, float g, float b);
	VPnt(VPnt& B)
	{
        k = B.k;
	}

	void SyncDummy();

	void Copy(Object* A)
	{
		 VPnt* B = (VPnt*)A;
         Object::Copy(B);


	}

	Pnt   scale;
	string alias;
	float R;	// радиус
	float ti;   // врем€ путевой точки
	Pnt   kurs; // курс в точке (отображаетс€ стрелкой)
	Pnt   kp;   //коэффициент дл€ кубического сплайна касательна€ (по Ёрмиту)
	float k;    // длинна касательной
	float We;	// скорость в точке
    float cl;   // цвет подсветки
	
	
	bool  isWaipoint; // это точка €вл€етс€ вейпоинтом
	bool  isPointOfCurve; // точка принадлежит кривой
    bool  ShowName;
	bool  ShowKurs;   // показывать курс
    bool  ShowBounds;

    float color[4];
	string segment;
	int	   isegment;
	VPnt*   parent;

    virtual void Render(QOpenGLShaderProgram& program);
	void Scale(float ,float,float);
	void Rotate(float,float,float);
	void SetAngle(float angle);
	bool isDummy();
	void SetMode(bool isPointOfCurve, bool isWaipoint);

	void Declare()
	{
		classname = "VPnt";
		if(name=="")
		  SetName("VPnt01");
		
		SET_OBJECT(this,(char*)name.c_str());

		ATTR("isWaipoint", &isWaipoint, "bool");
		//ATTR("isPointOfCurve", &isPointOfCurve, "bool");
		ATTR("ti", &ti, "float");				
		ATTR("We", &We, "float");				
		ATTR("p", (Pnt*)this, "Pnt");				
		ATTR("kurs", &kurs, "Pnt");
	

		ATTR("R", &R, "float");
        ATTR("k", &k, "float"); HIDE
		ATTR("kp", &kp, "Pnt");		
		ATTR("segment", &segment, "selector");
		
        //ATTR("angle_type", &Angle_types, "selector");
        //ATTR("euler_order", &EulerAngle_order, "selector");

	
		// fill entety info	
		FillEntetyInfo("Helpers");
			
        //bHideStdAttrs = true;
        //Object::Declare();
	}

	//virtual void Serialize( CArchive& archive );
    void Slot1(); //timer slot
	void mouse_move(int x, int y);
	void OnAttrChange(t_atr& a);
	void OnSelect();
	void OnUnselect();
	void OnDrag(float x, float y, float z);

protected:

};

#endif 

