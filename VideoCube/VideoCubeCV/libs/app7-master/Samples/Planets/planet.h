#ifndef PLANET_H
#define PLANET_H

#include "renderobject.h"
#include "gemobject.h"
#include <Wm5HMatrix.h>

class Planet :public RenderObject
{
 //   Q_OBJECT
public:

// Constructors
    Planet();
    Planet(string _name, Pnt _C0, float _R, float r, float g, float b);
    Planet(Planet& B)
    {
        //mu_printf("");
        R=B.R;
    }

    //~Planet();
    Pnt scale;
    GemObject*  mesh;
    float R;	// радиус
    float phi1;
    float w;	// угловая скорость
    float f;  // частота
    Pnt	  rot;  // углы поворота модели
    int   Nseg;
    int   Npts;
    int   Nparal;
    GLfloat color[4];
    string algo;
    int    algon;
    //flags
    bool  bNeedGenerate;
    int	  gentime;
    string textureName;
    int texture_id;
    double myTilt;
    //virtual void Serialize( CArchive& archive );

    virtual void Render();
    void Scale(float ,float,float);
    void Rotate(float,float,float);
    void setOrbit(double orbit);
    void setTilt(double tilt);
    void SetTexture(string textureName);
    void Generate();

    void Declare()
    {
        classname = "Planet";
        if(name=="")
          SetName("Planet01");

        SET_OBJECT(this,(char*)name.c_str());
        SET_ATTR("R", &R, "float");
        SET_ATTR("Nseg", &Nseg, "int");
        SET_ATTR("Npts", &Npts, "int");
        SET_ATTR("Nparal", &Nparal, "int");
        SET_ATTR("color", &color, "GLfloat*");
        SET_ATTR("rot", &rot, "Pnt");
        SET_ATTR("scale", &scale, "Pnt");
        SET_ATTR("algo", (void*)&algo, "selector");

        //SET_METHOD("Object", "bool",(char*)classname.c_str(), "Render", Render);
        //SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "Scale", Scale);
        //SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "Rotate", Rotate);

        // fill entety info
        ei.name = name;
        ei.classname = string("Planet");
        ei.groupname = "Primitives";
        char gstr[255];
        sprintf(gstr,"%d", this);
        ei.GUID = gstr;
        ei.material = "no material";
        ei.model = "no model";

        Object::Declare();
    }

    void Slot1(); //timer slot
    void OnAttrChange(t_atr& a);

protected:

};


#endif // PLANET_H
