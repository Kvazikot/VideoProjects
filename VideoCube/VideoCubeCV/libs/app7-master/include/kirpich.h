
#ifndef KIRPICH_H
#define KIRPICH_H

#include "renderobject.h"

class Kirpich :public RenderObject
{
//    Q_OBJECT
public:
        //DECLARE_SERIAL( Kirpich )
// Constructors
	Kirpich();
    Kirpich(string _name, float a, float b, float c);
    Kirpich(const Kirpich& B)
	{
        a=B.a;
        b=B.b;
        c=B.c;
        angle=B.angle;
        scale=B.scale;
    }
	
	 /*
	 operator=(Kirpich& A)
	 {
		 //this=*(Object*)&A;
		 a=A.a;
		 b=A.b;
		 c=A.c;
		 pos=A.pos;
		 angle=A.angle;
		 scale=A.scale;
		 //color = A.color;
	 }
	 */


	void Copy(Object* A)
	{
		 Kirpich* B = (Kirpich*)A;
		 Object::Copy(A);
		 a=B->a;
		 b=B->b;
		 c=B->c;
		 angle=B->angle;
		 scale=B->scale;
	}

	//~Kirpich();
	Pnt angle;
	Pnt scale;
	float a;
	float b;
	float c;
    float color[4];
    char    gstr[1024];

    virtual void Serialize(  );

	virtual void Render();
	void Scale(float ,float,float);
	void Rotate(float,float,float);

	void Declare()
	{
		classname = "Kirpich";
		if(name=="")
		  SetName("box01");
		
		SET_OBJECT(this,(char*)name.c_str());
  		SET_ATTR("a", &a, "float");
  		SET_ATTR("b", &b, "float");
  		SET_ATTR("c", &c, "float");
  		SET_ATTR("color", &color, "GLFloat*");
  		SET_ATTR("angle", &angle, "Pnt");
		SET_ATTR("scale", &scale, "Pnt");

		//SET_METHOD("Object", "bool",(char*)classname.c_str(), "Render", Render);
		//SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "Scale", Scale);
		//SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "Rotate", Rotate);

		// fill entety info	
                //CRuntimeClass* pMyRTClass= GetRuntimeClass();
		ei.name = name;
        ei.classname = string("Kirpich");
		ei.groupname = "Primitives";
		sprintf(gstr,"%d", this);
		ei.GUID = gstr;
		ei.material = "no material";
		ei.model = "no model";

		Object::Declare();
	}

    void Slot1(); //timer slot

protected:

};

#endif 

