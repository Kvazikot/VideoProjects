/***********************************
Copyright (c) 2006, Richard Cassan
All rights reserved.
***********************************/

#ifndef cvector3_def
#define cvector3_def

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "geometry.h"

using namespace std;


class CVector3
{
private:

public:

    float x,y,z;          //position of vector
    float mag;          //magnitude of vector



    void setVector(float _x, float _y, float _z);
    Pnt toPnt(){return Pnt(x,y,z); };

    float findMagnitude(void);
    float retMagnitude(void) {return mag;}

    CVector3 normalize(void);

    void zero(void) {x=0;y=0;z=0;mag=0;}

    void draw(void);

    //rotates vector counter-clockwise by angle radians
    CVector3 rotateXYPlane(float angle);
    CVector3 rotateAbout(CVector3 v, float theta);

    CVector3 rotateAboutZ(float theta);
    CVector3 rotateAboutY(float theta);
    CVector3 rotateAboutX(float theta);

    float retXAng(void);
    float retYAng(void);
    float retZAng(void);

    CVector3(void) {x=0;y=0;z=0; mag=0;}
    CVector3(float _x, float _y, float _z) {x=_x; y=_y; z=_z; mag=findMagnitude();}
    CVector3(Pnt p){ x=p.x; y=p.y; z=p.z;mag=findMagnitude();}

//Overloaded operators
    //vector addition and subtraction
    CVector3 operator+(CVector3 op2);
    CVector3 operator-(CVector3 op2);
    CVector3 operator+=(CVector3 op2);
    CVector3 operator-=(CVector3 op2);

    //dot product
    float operator|(CVector3 op2);

    //normalize
    CVector3 operator~(void);

    //cross-product
    CVector3 operator^(CVector3 op2);

    //scalar division
    CVector3 operator/(float op2);
    //scalar multiplication
    friend CVector3 operator*(float op1, CVector3 op2);
    friend CVector3 operator*(CVector3 op1, float op2);
    friend bool operator==(CVector3 op1, float op2);
    friend bool operator==(float op1, CVector3 op2);

    friend bool operator>(CVector3 op1, float op2);
    friend bool operator<(CVector3 op1, float op2);
};

CVector3 makeVector3(float x, float y, float z);


#endif
