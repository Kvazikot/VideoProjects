/***********************************
Copyright (c) 2006, Richard Cassan
All rights reserved.
***********************************/
#include <math.h>

#include "../include/vector3d.h"



CVector3 makeVector3(float x, float y, float z)
{
    CVector3 ret(x,y,z);
    return ret;
}


void CVector3::setVector(float _x, float _y, float _z)
{
    x=_x;
    y=_y;
    z=_z;
    //mag=findMagnitude();
}


float CVector3::findMagnitude(void)
{
    return sqrt(x*x + y*y + z*z);
}

CVector3 CVector3::normalize(void)
{
    //record the magnitude and, then change it into a unit vector
    mag=findMagnitude();
    if(mag!=0)
    {
    x = x/mag;
    y = y/mag;
    z = z/mag;
    }
    else
    { x=0;y=0;z=0;}

    return (*this);
}

float CVector3::retXAng(void)
{
    float ret=0;
    CVector3 xaxis(1,0,0),temp;

    temp=(*this);
    ~temp;

    ret = acos( ( (temp|xaxis)/(temp.mag*xaxis.mag) ) );

    //return ret;
    if( (temp.y)<0)
        return -ret;
    else
        return ret;
}

float CVector3::retYAng(void)
{
    float ret=0;
    CVector3 yaxis(0,1,0),temp;

    temp=(*this);
    ~temp;

    ret = acos( ( (temp|yaxis)/(temp.mag*yaxis.mag) ) );

    if( (temp.z)<0)
        return -ret;
    else
        return ret;
}


float CVector3::retZAng(void)
{
    float ret=0;
    CVector3 zaxis(0,0,1),temp;

    temp=(*this);
    ~temp;

    ret = acos( ( ( temp|zaxis) / (temp.mag*zaxis.mag) ) );

    if( (temp.x)<0)
        return -ret;
    else
        return ret;
}

CVector3 CVector3::operator+(CVector3 op2)
{
    CVector3 temp1,ret;

    temp1=(*this);

    temp1=~temp1;
    op2=~op2;

    ret.x = temp1.x*temp1.mag + op2.x*op2.mag;
    ret.y = temp1.y*temp1.mag + op2.y*op2.mag;
    ret.z = temp1.z*temp1.mag + op2.z*op2.mag;
    ret.mag = ret.findMagnitude();

    return ret;
}

CVector3 CVector3::operator-(CVector3 op2)
{
    CVector3 temp1,ret;

    temp1=(*this);

    temp1=~temp1;
    op2=~op2;

    ret.x = temp1.x*temp1.mag - op2.x*op2.mag;
    ret.y = temp1.y*temp1.mag - op2.y*op2.mag;
    ret.z = temp1.z*temp1.mag - op2.z*op2.mag;
    ret.mag = ret.findMagnitude();

    return ret;
}

CVector3 CVector3::operator+=(CVector3 op2)
{
    normalize();
    ~op2;

    x = x*mag + op2.x*op2.mag;
    y = y*mag + op2.y*op2.mag;
    z = z*mag + op2.z*op2.mag;
    mag = findMagnitude();

    return *this;
}

CVector3 CVector3::operator-=(CVector3 op2)
{
    ~(*this);
    ~op2;

    x = x*mag - op2.x*op2.mag;
    y = y*mag - op2.y*op2.mag;
    z = z*mag - op2.z*op2.mag;
    mag = findMagnitude();

    return *this;
}

CVector3 CVector3::rotateAboutZ(float theta)
{
    CVector3 temp,ret;

    temp=*this;

    //~temp;

    ret.x= cos(theta)*temp.x - sin(theta)*temp.y;
    ret.y= -sin(theta)*temp.x - cos(theta)*temp.y;
    ret.z=temp.z;

    //~ret;
    //ret.mag=1;

    return ret;
}


CVector3 CVector3::rotateAboutY(float theta)
{
    CVector3 temp,ret;

    temp=*this;

    //~temp;

    ret.x= cos(theta)*temp.x + sin(theta)*temp.z;
    ret.y=temp.y;
    ret.z= -sin(theta)*temp.x + cos(theta)*temp.z;


   // ~ret;
    //ret.mag=1;

    return ret;
}

CVector3 CVector3::rotateAboutX(float theta)
{
    CVector3 temp,ret;

    temp=*this;

   // ~temp;

    ret.x=temp.x;
    ret.y=cos(theta)*temp.y + sin(theta)*temp.z;
    ret.z=-sin(theta)*temp.y + cos(theta)*temp.z;


    //~ret;
   // ret.mag=1;

    return ret;
}

CVector3 CVector3::rotateAbout(CVector3 v, float theta)
{
    CVector3 temp1,ret;

    float value=1-cos(theta);

    ~v; //normalize v
    temp1=*this;
    ~temp1; //normalize this vector

    ret.x = (v.x*v.x)*(value)+cos(theta) +
            (v.x*v.y)*(value)+v.z*sin(theta)+
            (v.x*v.z)*(value)-v.y*sin(theta);

    ret.y = (v.x*v.y)*(value)-v.z*sin(theta) +
            (v.y*v.y)*(value)+cos(theta) +
            (v.y*v.z)*value+v.x*sin(theta);

    ret.z = (v.x*v.z)*value+v.y*sin(theta) +
            (v.y*v.z)*value-v.x*sin(theta) +
            (v.z*v.z)*value+cos(theta);
    ret.findMagnitude();
    ~ret;

    return ret;
}

//dot product
float CVector3::operator|(CVector3 op2)
{
    CVector3 temp1;
    float ret;

    temp1=(*this);

    temp1=~temp1;
    op2=~op2;

    ret= temp1.x*mag * op2.x*op2.mag  +  temp1.y*mag * op2.y*op2.mag + temp1.z*mag * op2.z*op2.mag;

    return ret;
}

CVector3 CVector3::rotateXYPlane(float angle)
{
    CVector3 temp;

    temp.x = x*cos(angle)  + y*sin(angle);
    temp.y = -x*sin(angle) + y*cos(angle);

    //(*this)=temp;

    return temp;

}

//normalize
CVector3 CVector3::operator~(void)
{
    normalize();

    return *this;
}


//cross-product
CVector3 CVector3::operator^(CVector3 op2)
{
    CVector3 op1=*this,ret;

    ret.x = op2.z*op1.y - op2.y*op1.z;
    ret.y = op2.x*op1.z - op2.z*op1.x;
    ret.z = op2.y*op1.x - op2.x*op1.y;

    return ret;

}

//a friend function
CVector3 operator*(CVector3 op1, float op2)
{
    CVector3 temp1,ret;

    ~op1;

    ret.x = op1.x*op1.mag * op2;
    ret.y = op1.y*op1.mag * op2;
    ret.z = op1.z*op1.mag * op2;
    ret.mag = ret.findMagnitude();

    return ret;
}


//a friend function
CVector3 operator*(float op1, CVector3 op2)
{
    CVector3 temp1,ret;

    ~op2;

    ret.x = op1 * op2.x*op2.mag;
    ret.y = op1 * op2.y*op2.mag;
    ret.z = op1 * op2.z*op2.mag;
    ret.mag = ret.findMagnitude();

    return ret;
}

CVector3 CVector3::operator/(float op2)
{
    CVector3 temp1,ret;

    temp1=(*this);

    ~temp1;

    ret.x = temp1.x*mag / op2;
    ret.y = temp1.y*mag / op2;
    ret.z = temp1.z*mag / op2;
    ret.mag = ret.findMagnitude();

    return ret;
}

//a friend function
bool operator==(CVector3 op1, float op2)
{
    return( op1.x==op2 && op1.y==op2 && op1.z==op2);

}

//a friend function
bool operator==(float op1, CVector3 op2)
{
    return( op1==op2.x && op1==op2.y && op1==op2.z);
}

//a friend func
bool operator>(CVector3 op1, float op2)
{
    return (op2>op1.x && op2>op1.y && op2>op1.z);
}

//a friend func
bool operator<(CVector3 op1, float op2)
{
    return (op2<op1.x && op2<op1.y && op2<op1.z);
}
