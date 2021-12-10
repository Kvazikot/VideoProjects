/**********************************************************************
 *  Project App6
 *
 *  geometry.cpp
 *
 *  Author: Vladimir Baranov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
**********************************************************************/

//---------------------------------------------------------------------------
//    М о д у л ь       г е о м е т р и и
//
//---------------------------------------------------------------------------
//#include "globals.h"
#include "../include/geometry.h"
#include "../include/matrix.h"
#define _USE_MATH_DEFINES
#include <math.h>
//#include "vpnt.h"
//#include "vector3d.h"

char gstr3[1000000];
//#define M_PI 3.1415926535897932384626433832795
Pnt DummyP;


void GetEiler(Pnt p, Pnt& ang)
{
	//CVector3 v(p);
/*	ang.x = atan(p.z/(p.y+0.00000001));//v.retXAng();
	ang.y = atan(p.x/(p.z+0.00000001)) - M_PI/2;//v.retYAng();
	ang.z = atan(p.x/(p.y+0.00000001));//v.retZAng();
	
	double d,m;
	d = sqrt( sq((double)p.x) + sq((double)p.y) );
	m = sqrt( sq((double)p.z) + sq((double)p.y) );
*/
    ang.x = (float)vect_angle(Pnt(1,0,0),p);
    ang.y = (float)vect_angle(Pnt(0,1,0),p);
    ang.z = (float)vect_angle(Pnt(0,0,1),p);

	
	

}    

void Eiler2Matrix(float angle_x, float angle_y, float angle_z, float res[3][3])
{
    float A       = cos(angle_x);
    float B       = sin(angle_x);
    float C       = cos(angle_y);
    float D       = sin(angle_y);
    float E       = cos(angle_z);
    float F       = sin(angle_z);
    float AD      =   A * D;
    float BD      =   B * D;
	
    unsigned char i,j; 
    for(i=0;i<4;i++)
    for(j=0;j<4;j++)
		res[i][j] = 0;
    res[0][0]  =   C * E;
    res[1][0]  =  -C * F;
    res[2][0]  =  -D;
    res[0][1]  = -BD * E + A * F;
    res[1][1]  =  BD * F + A * E;
    res[2][1]  =  -B * C;
    res[0][2]  =  AD * E + B * F;
    res[1][2]  = -AD * F + B * E;
    res[2][2] =   A * C;    
    res[3][3] =  1;
}

bool point_inside_box(Pnt bounds, Pnt C0, Pnt p )
{

	if(	(p.x >= (C0.x - bounds.x/2)) && (p.x <= (C0.x + bounds.x/2)) &&
		(p.y >= (C0.y - bounds.y/2)) && (p.y <= (C0.y + bounds.y/2)) &&
		(p.z >= (C0.z - bounds.z/2)) && (p.z <= (C0.z + bounds.z/2))  )
	{
		return true;
	}
	
	return false;


}


bool intersect_boxes(Pnt bounds1, Pnt C01,
					 Pnt bounds2, Pnt C02)
{

	float r = rasst(C01, C02);

	if(r>bounds1.x && r>bounds1.y && r>bounds1.z && r>bounds2.x && r>bounds2.y && r>bounds2.z)
		return false;

	Pnt box1[9];
	Pnt box2[9];
	box1[1].x = C01.x - bounds1.x/2; box1[1].y = C01.y + bounds1.y/2; box1[1].z = C01.z - bounds1.z/2;
	box1[2].x = C01.x - bounds1.x/2; box1[2].y = C01.y + bounds1.y/2; box1[2].z = C01.z + bounds1.z/2;
	box1[3].x = C01.x + bounds1.x/2; box1[3].y = C01.y + bounds1.y/2; box1[3].z = C01.z + bounds1.z/2;
	box1[4].x = C01.x + bounds1.x/2; box1[4].y = C01.y + bounds1.y/2; box1[4].z = C01.z - bounds1.z/2;
	box1[5].x = C01.x - bounds1.x/2; box1[5].y = C01.y - bounds1.y/2; box1[5].z = C01.z - bounds1.z/2;
	box1[6].x = C01.x - bounds1.x/2; box1[6].y = C01.y - bounds1.y/2; box1[6].z = C01.z + bounds1.z/2;
	box1[7].x = C01.x + bounds1.x/2; box1[7].y = C01.y - bounds1.y/2; box1[7].z = C01.z - bounds1.z/2;
	box1[8].x = C01.x + bounds1.x/2; box1[8].y = C01.y - bounds1.y/2; box1[8].z = C01.z + bounds1.z/2;

	box2[1].x = C02.x - bounds2.x/2; box2[1].y = C02.y + bounds2.y/2; box2[1].z = C02.z - bounds2.z/2;
	box2[2].x = C02.x - bounds2.x/2; box2[2].y = C02.y + bounds2.y/2; box2[2].z = C02.z + bounds2.z/2;
	box2[3].x = C02.x + bounds2.x/2; box2[3].y = C02.y + bounds2.y/2; box2[3].z = C02.z + bounds2.z/2;
	box2[4].x = C02.x + bounds2.x/2; box2[4].y = C02.y + bounds2.y/2; box2[4].z = C02.z - bounds2.z/2;
	box2[5].x = C02.x - bounds2.x/2; box2[5].y = C02.y - bounds2.y/2; box2[5].z = C02.z - bounds2.z/2;
	box2[6].x = C02.x - bounds2.x/2; box2[6].y = C02.y - bounds2.y/2; box2[6].z = C02.z + bounds2.z/2;
	box2[7].x = C02.x + bounds2.x/2; box2[7].y = C02.y - bounds2.y/2; box2[7].z = C02.z - bounds2.z/2;
	box2[8].x = C02.x + bounds2.x/2; box2[8].y = C02.y - bounds2.y/2; box2[8].z = C02.z + bounds2.z/2;

	for(int i=1; i<9; i++)
	{
		if( point_inside_box(bounds2, C02, box1[i]) )
			return true;
		if( point_inside_box(bounds1, C01, box2[i]) )
			return true;
	}
	
	return false;
	

}


// параметрическое уравнение линии
Pnt line_eq(Pnt A, Pnt B, float t)
{
    Pnt C;

    C.x = A.x + t * (B.x - A.x);
    C.y = A.y + t * (B.y - A.y);
    C.z = A.z + t * (B.z - A.z);

    return C;
}


//--параллельный перенос точки
Pnt translate_p(Pnt& p, float dx, float dy, float dz)
{
	p.x+=dx;
	p.y+=dy;
	p.z+=dz;
	return p;
}


// Скалярное произведение
double dot_product(Pnt A, Pnt B)

{
	return A.x*B.x + A.y*B.y + A.z*B.z;
}

// Длинна вектора
double len_vec(Pnt p1)
{
	return  sqrt( (double) ( sq(p1.x)  +  sq(p1.y  ) + sq(p1.z)))  ;
	
}

// Угол между векторами
double vect_angle(Pnt A, Pnt B)
{
	double r;
	r = acos( dot_product(A,B)/(len_vec(A)*len_vec(B)) ) ;
	return r;
}




// перенос вектора в начало координат
void translate_vector(tvect* v)
{
    //1. сместит нач. коорд в (0,0)
    v->B.x = v->B.x - v->A.x;
    v->B.y = v->B.y - v->A.y;
    v->B.z = v->B.z - v->A.z;
    v->A.x = 0;
    v->A.y = 0;
	v->A.z = 0;
}



void normalize_vector(tvect* v)
{
    //record the magnitude and, then change it into a unit vector
    float mag=rasst(v->A, v->B);
    if(mag!=0)
    {
		v->B.x = v->B.x/mag;
		v->B.y = v->B.y/mag;
		v->B.z = v->B.z/mag;
    }
   
    
    return ;
}



void normalize_vector(Pnt& v)
{
    //record the magnitude and, then change it into a unit vector
    float mag=rasst(Pnt(0,0,0), v);
    if(mag!=0)
    {
		v.x = v.x/mag;
		v.y = v.y/mag;
		v.z = v.z/mag;
    }
}


// угол между осью ox и вектором
double vector_anglX(tvect v)
{
   double a;
	translate_vector(&v);
   normalize_vector(&v);
   if(v.B.x == 0)
     v.B.x = 0.0000000000000001f;
   if(v.B.x > 0)
	a = atan((double)v.B.z / v.B.x) ;
   else
     a = atan((double)v.B.z / v.B.x) - M_PI ;
   a = -a;
   return a;
}

// угол между осью oy и вектором
double vector_anglY(tvect v)
{
   double a;
   translate_vector(&v);
   normalize_vector(&v);
   if(v.B.x == 0)
     v.B.x = 0.0000000000000001f;
   //if(v.B.x > 0)
	a = M_PI /2 - acos((double)v.B.y ) ;
   //else
   //  a = acos((double)v.B.y) - M_PI /2 ;
   //a = -a;
   return a;
}


void rotateAboutZ(Pnt& p, float theta)
{
    Pnt ret;
    ret.x= cos(theta)*p.x - sin(theta)*p.y;
    ret.y= -sin(theta)*p.x - cos(theta)*p.y;
    ret.z=p.z;
	p = ret;    
    return;
}


void rotateAboutY(Pnt& p, float theta)
{
    Pnt ret;
    ret.x= cos(theta)*p.x + sin(theta)*p.z;
    ret.y= p.y;
    ret.z= -sin(theta)*p.x + cos(theta)*p.z;
	p = ret;    
    return;
}



void rotateAboutX(Pnt& p, float theta)
{
    Pnt ret;
    
    ret.x=p.x;
    ret.y=cos(theta)*p.y + sin(theta)*p.z;
    ret.z=-sin(theta)*p.y + cos(theta)*p.z;

    
    return;
}

void decart2polar(Pnt& polar, Pnt decart)
{
    polar.x = (float)sqrt((double) (sq(decart.x )  +  sq(decart.z  ) + sq(decart.y )))+0.0000001f;
    polar.y =  (float)acos(decart.y / polar.x);
	if(decart.x > 0)
      polar.z = (float)atan((double) decart.z / decart.x);
	else
      polar.z = (float)atan((double) decart.z / decart.x) - (float)M_PI;
}


void polar2decart(Pnt& decart, Pnt polar)
{
	decart.x = polar.x * sin(polar.y) * cos(polar.z);
	decart.z = polar.x * sin(polar.y) * sin(polar.z);
	decart.y = polar.x * cos(polar.y);
}

// расстояние между точками
float rasst(Pnt p1, Pnt p2)
{
     float _t1 = (float)sqrt( (double) ( sq(p1.x - p2.x)  +  sq(p1.y - p2.y ) + sq(p1.z - p2.z)))  ;
     return _t1;
}

//вектор равен 0
int vect_isnull(tvect v)
{
   return !((v.A.x != 0) || (v.A.y != 0) || (v.B.x != 0) || (v.B.y != 0) || (v.B.z != 0) || (v.A.z != 0));
}


// изменить размер вектрора
tvect resize_v(tvect v, float factor)
{
   Pnt A, B, C;
   tvect out;
   A = v.A;
   B = v.B;
   C = line_eq(A, B, factor);
   out = v;
   out.A = C;
   return out;
}
















