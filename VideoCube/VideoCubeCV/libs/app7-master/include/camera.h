#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "object.h"
#include "renderobject.h"
#include <stdio.h>


class Camera : public RenderObject
{
//    Q_OBJECT
public:
    int      num;     // номер
//	Pnt      pos;
    Pnt		 lookAt;  // расчет точки наблюдения
    Vector	 orient;    // ориентация
    Object*  target;

    int RES_X;
    int RES_Y;
    unsigned char blink;
    bool   isMoving;
    float  tlastmove;
    float fovy;
    float aspect;
    float zNear;
    float zFar;
    float dist;
    float speed;
    float drag_speed;

    float sens;			// чувствительность мыши
    bool looktarget;
    bool followtarget;
    string views; // freecamera top left right bottom front back
    int    view;  //  0          1  2      3     4      5    6
    int prev_x;
    int	prev_y;
    bool isMouseEnabled;
    bool isReceiveInput;
    bool isVisible;
    bool bBreakLookAt;
    float step;
    float tm,dt,k;
    int colx; // цвет апертуры

    int ResX;
    int ResY;



    //	int ResX;
    //	int ResY;


    Camera();
    Camera(const Camera& cam)
    {
        ResX = cam.ResX;
    }
    void Reset();
    void LookAt(char* name);
    void TopView();
    void FreeView();
    void LeftView();
    void UpdateLookAt();
    //void PointsTest();
    //void create_pyr2(Wm5::Vector3f v[5], float R, float H, float orientY, float orientZ, Pnt pos);

    Camera(std::string Name, Pnt Pos, Vector Orient);

    //        virtual void Serialize(  );

    void Render();
    void Declare()
    {
        classname = "Camera";
        //SetName("cam01");

        //SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "move2", move2);
        //SET_METHOD("Object", "char*",(char*)classname.c_str(), "LookAt", LookAt);
        SET_OBJECT(this,(char*)name.c_str());
        SET_ATTR("view", &view, "int");
        SET_ATTR("orient", &orient, "Vector");
        SET_ATTR("lookAt", &lookAt, "Pnt");
        SET_ATTR("pos", &ei.coord, "Pnt");
        SET_ATTR("looktarget", &looktarget, "bool");
        SET_ATTR("fovy", &fovy, "float");
        SET_ATTR("aspect", &aspect, "float");
        SET_ATTR("zNear", &zNear, "float");
        SET_ATTR("zFar", &zFar, "float");
        SET_ATTR("dist", &dist, "float");
        SET_ATTR("speed", &speed, "float");

	
        // fill entety info
        //CRuntimeClass* pMyRTClass= GetRuntimeClass();
        ei.name = name;
        ei.classname = string("Camera");
        ei.groupname = "Cameras";
        char gstr[255];
        sprintf(gstr,"%d", this);
        ei.GUID = gstr;
        ei.material = "no material";
        ei.model = "no model";

        Object::Declare();

        //	CALL("cam1", (char*)classname.c_str(), "move2");
    }



    // [10]
    virtual void moveAt(float x, float y, float z)
    {
        ei.coord.x=x;
        ei.coord.y=y;
        ei.coord.z=z;
    }


    // [11]
    virtual void move(float dx, float dy, float dz)
    {
        ei.coord.x+=dx;
        ei.coord.y+=dy;
        ei.coord.z+=dz;
    }

    void DoScan();
    void DoShot();

    void move2(float f1, float f2, float f3);

    void OnAttrChange(t_atr& a);

    virtual void mouse_move(int x, int y);
    virtual void mouse_wheel(int zDelta);
    virtual void click_lmb(int state);
    virtual void click_rmb(int state);
    virtual void click_mmb(int state);
    virtual void key_pressed(int k);
    virtual void OnSelect();
    virtual void OnMove();
    virtual void OnReceiveInput();
    virtual void Slot1(); //timer slot

};
// Q_DECLARE_METATYPE(Camera)
//qRegisterMetaType<Camera>("Camera");
#endif
