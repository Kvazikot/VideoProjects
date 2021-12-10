/**********************************************************************
 *  Project App6
 *
 *  move.cpp
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

// <OBJECT> MOVEMENT </OBJECT>
//====================================================================

#include "../include/geometry.h"
#include "../include/cobjregistry.h"
#include "../include/move.h"
#include "../include/group.h"
#include "../include/print.h"
#include "../include/glcode.h"
#include "../include/param_screen_bcb.h"

static ParamScreen& pscr = ParamScreen::getInstance();

int factorial(int b)
{
	int a,c;
	a = 1;
	c = 1;
	do
	{
		a=a*c;
		c++;
	}while (c<=b);
	return a;
}


CMovement::CMovement()
{
	//pscr.AddVar("mov.phi");
	//isNotSaving	= true;
	isCanRender = false;
    trenie = 0.05f;
	int j;
	//
	for(int i = 0; i < 1000; i++)
	{
		j = factorial(i);
		factorialmap[i] = j;
	}
	// запрос на апдейт
	Connect();
}

bool CMovement::CheckCollison(Object* obj1, Object* obj2)
{
	bool flag = false;

	if(obj1->classname == "Kirpich" && obj2->classname == "Kirpich")
	{
		flag = true;
	}

	if( obj1->mass == 0 || obj2->mass == 0)
		return false;

	
	if( obj1->bounds.isNull() || obj2->bounds.isNull())
		return false;

	if( obj1->isHidden >0 ||  obj2->isHidden>0)
		return false;


	if( intersect_boxes(obj1->bounds, obj1->ei.coord, obj2->bounds, obj2->ei.coord) )
		return true;
	return false;

}


void CMovement::CollisonDetected(Object* obj1, Object* obj2)
{
	colisstr+=obj1->name + "-" + obj2->name+", ";
	
	//проверить не зафиксирована ли эта коллизия
	if( colmap.find(obj1->name + "-" + obj2->name)!=colmap.end() )
	{
		colmap[ obj1->name + "-" + obj2->name ].cnt++;
        //prn("collision %d", colmap[ obj1->name + "-" + obj2->name ].cnt);

		//если это падение сверху
		Object* obj;
		if(obj1->ei.coord.y < obj2->ei.coord.y)
			obj = obj2;
		else
			obj = obj1;

		if(obj->v.y > 0) 
		{
			obj->onOpora = true;
			return;
		}

		
		if( obj->onOpora )
		{
			return;

		}

		//зафиксированный сквозное прохождение
		if(colmap[ obj1->name + "-" + obj2->name ].cnt>2)
		{
			
			Decolise(obj1, obj2);		
		}
		return;
	}

	//отработка коллизии по законам физики ( сохранение импульса )
	BallColision(obj1, obj2);
    prn("perform colision %s %s", obj1->name.c_str(), obj2->name.c_str());

	//занести коллизию в таблицу
	t_objpair p;
	p.obj1 = obj1;
	p.obj2 = obj2;
	colmap[obj1->name + "-" + obj2->name] = p;

	
}

void CMovement::Decolise(Object* obj1, Object* obj2)
{
	Pnt p[6];
	float r[6];

	for(int i=0; i<6; i++)
	  p[i] = obj1->ei.coord;
	
	p[0].x = obj1->ei.coord.x + obj1->bounds.x/2;
	p[1].x = obj1->ei.coord.x - obj1->bounds.x/2;
	p[2].y = obj1->ei.coord.y + obj1->bounds.y/2;
	p[3].y = obj1->ei.coord.y - obj1->bounds.y/2;
	p[4].z = obj1->ei.coord.z + obj1->bounds.z/2;
	p[5].z = obj1->ei.coord.z - obj1->bounds.z/2;
	
	for(int i=0; i<6; i++)
		r[i] = rasst(obj2->ei.coord, p[i]);

	float m = 1000000;
	for(int i=0; i<6; i++)
		if( r[i] < m )
			m = r[i];

	//m = 0.5;

	m = 0.05 * m;
	obj2->ei.coord.z  = obj2->ei.coord.z - m * sin(GR * obj2->kurs0.x);
	obj2->ei.coord.y  = obj2->ei.coord.y + m * sin(GR * obj2->kurs0.y);
	obj2->ei.coord.x  = obj2->ei.coord.x - m * cos(GR * obj2->kurs0.x);

}

void CMovement::CollisonNotDetected(Object* obj1, Object* obj2)
{
	//вынести коллизию из таблицы
	map<string, t_objpair>::iterator i1;

	i1 = colmap.find(obj1->name + "-" + obj2->name);
	if( i1 != colmap.end() )
	{
	  colmap.erase( i1 );
	  //выключить режим опоры
	  if( obj1->onOpora )
		obj1->onOpora = false;
	  if( obj2->onOpora )
		obj2->onOpora = false;
      prn("no more colision %s %s", obj1->name.c_str(), obj2->name.c_str());
	}
}


//расчет столкновения
void CMovement::BallColision( Object* obj1, Object* obj2 )
{
	
	float v1, v2, u1, u2; // новые скрости после столконовения
	float m1, m2;
	
	m1 = obj1->mass; 
	m2 = obj2->mass;
	v1 = obj1->v.x;
	v2 = obj2->v.x;

	u1 = fabs(m1-m2)*v1 / (m1+m2);
	u2 = 2*m1*v1 / (m1+m2);

	if(v1>v2)
	{
	  obj2->kurs0.x = obj1->kurs0.x;
	  obj1->kurs0.x = obj1->kurs0.x;
	}
	else
	{
	  obj1->kurs0.x = obj2->kurs0.x;
	  obj2->kurs0.x = obj2->kurs0.x;
	}
	
	obj1->v = Pnt(u1,0,0);
	obj2->v = Pnt(u2,0,0);



} 

//расчет влияния гравитация
void CMovement::CalcGravity(Object* obj)
{
	Pnt* coord = &obj->ei.coord;
	Pnt* v = &obj->v;
	Pnt* kurs0 = &obj->kurs0;
	float dist;

	if((coord->y > 0) && (!obj->onOpora))
	{
        v->y += 9.8f * 0.05f;
        dist = v->y * 0.05f;
		coord->y -= dist;
		if(coord->y < 0)
			coord->y = 0;
		
		dist = v->x * 0.05;
		coord->z  = coord->z - dist * sin(GR * kurs0->x);
		coord->x  =  coord->x - dist * cos(GR * kurs0->x); 
	}


}

// расчет трения
void CMovement::CalcTrenie(Object* obj)
{
	Pnt* coord = &obj->ei.coord;
	Pnt* v = &obj->v;

	if(coord->y == 0 || obj->onOpora)
	{
		if(v->x > 0)
		{
			v->x-=trenie * 9.8 * 0.05;
			if(v->x<0)
				v->x = 0;
		}
	}


}

// счисление пути
void CMovement::SchislenieCoord(Object* obj)
{
	Pnt* coord = &obj->ei.coord;
	Pnt* v = &obj->v;
	Pnt* kurs0 = &obj->kurs0;

	if(coord->y == 0 || obj->onOpora) // земля / опора	
	{
		
		//пока только в плоскости
		float dist = v->x * 0.05; //2 * M_PI * r * 0.05 *ffw;
		coord->z  = coord->z - dist*sin(GR * kurs0->x);
		coord->x  =  coord->x - dist*cos(GR * kurs0->x); 
		if(obj->isGroup)
			((Group*)obj)->AssignMyCoord();
	}


}
	

void CMovement::ProcessCircular(MotionData* md)
{
    float x;
    static float phi=0;
	
	x = md->pos.x - md->Co.x;
	//phi = acos(x / md->R);
	
	if ( phi > 2*M_PI ) 
		phi = phi - 2 * M_PI;
			
	phi = phi + 0.005 * md->f;  //50ms * частота
	
	pscr.upd("mov.phi", phi/GR);
    //prn("phi = %f", phi);

	md->pos.x = md->Co.x + md->R * cos(phi);
	md->pos.z = md->Co.z + md->R * sin(phi);
	md->pos.y = md->Co.y;
	md->rot.x   = phi/GR;
}

void CMovement::AssignCircular(Object* obj, Pnt Co, float f, float R)
{
	MotionData md;

	if(obj == NULL) return;

    prn("Assign circular to %s", obj->name.c_str());
	md.ttype = T_CIRCULAR;
	md.Co = Co;
	md.f = f;
	md.R = R;


	motmap[obj] = md;

}


int CMovement::Ckn(int k, int n)
{
	int c;

	c = factorialmap[n] / ( factorialmap[k] * factorialmap[n-k] );

	return c;
}

void CMovement::Slot1()
{
    //prn("Movement works");
    //prn("::%d", gtimer.msec);
	//return;
	MotionData md;
	t_atr a;
	map<Object*, MotionData>::iterator i;
	vector<Object*>::iterator obj1, obj2;

	// первый алгоритм детекции коллизий
	// проверяется все обьекты cо всеми по bounding box
	bool bFlag = 0;
	colisstr="";


    CObjRegistry& MREG = CObjRegistry::getInstance2();

	//составить objpairs
	int pair_cnt = Ckn(2, MREG.allObj.size());
	
	if(objpairs.size()!=pair_cnt)
	{
		objpairs.clear();
		for(obj1 = MREG.allObj.begin();obj1!= MREG.allObj.end(); obj1++)
		{
			for(obj2 = MREG.allObj.begin();obj2!= MREG.allObj.end(); obj2++)
			{
				if(obj2 == obj1)
					continue;
				
				if ( ( objpairs.find((*obj1)->name + "-" + (*obj2)->name)==objpairs.end() ) &&
					 ( objpairs.find((*obj2)->name + "-" + (*obj1)->name)==objpairs.end() ) )
				{
					t_objpair p;
					p.obj1 = (*obj1);
					p.obj2 = (*obj2);
					objpairs[(*obj1)->name + "-" + (*obj2)->name] = p;
				}
				

				
			}
		}

		
	}

	map<string, t_objpair>::iterator i1;
	for(i1 = objpairs.begin();i1!= objpairs.end(); i1++)
	{
		t_objpair p;
		p = i1->second;
		if( CheckCollison(p.obj1, p.obj2) )
		{
			CollisonDetected(p.obj1, p.obj2);
			bFlag = 1;
		}
		else
		{
			CollisonNotDetected(p.obj1, p.obj2);
		}
	
	}



	pscr.upd("info", colisstr);

	if(bFlag == false)
	 pscr.upd("info", "no colisions");


	for(i = motmap.begin(); i!=motmap.end(); i++)
	{
		md = i->second;
		
		md.pos = i->first->ei.coord;


		//md->pos = (*i->first).ei.coord ;
		if( md.ttype == T_CIRCULAR )
			ProcessCircular(&md);
		i->second = md;

		//UPDATE OBJECT DATA
		
	     i->first->ei.coord = md.pos;
		 //a = i->first->GET_ATTR("kurs");
		 //if(a.addr!=0)
		 //	*(Pnt*)a.addr = md.rot;


	}
}
