/**********************************************************************
 *  Project App6
 *
 *  drawings2d.cpp
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

#include "../include/drawings2d.h"
#include "../include/cobjregistry.h"
#include "../include/vpnt.h"
#include "../include/curve.h"
#include "../include/glcode.h"
#include "../include/flags.h"
#include <QMutex>
#include <QWaitCondition>

static t_flags& flags = GetGFlags();
static CObjRegistry& MREG = CObjRegistry::getLayerInstance();
QMutex mutex;
QWaitCondition wait_cond;


void CreatePoint(Pnt p, string name)
{

    VPnt* vp1;
    vp1 = (VPnt*)MREG.FindObj(name);    
    if( vp1 == 0 )
    {
        vp1 = new VPnt();
        vp1->ShowBounds = false;
        vp1->name = name;
        MREG.AddObj(vp1);
    }
    *(Pnt*)vp1 = p;
    vp1->ei.coord = p;
}


void CreateLine(Pnt p1, Pnt p2, string name)
{
    CObjRegistry& MREG = CObjRegistry::getLayerInstance();
    Curve* curve;
    curve = (Curve*)MREG.FindObj(name);
    if(curve == 0)
    {
        curve  = new Curve();
        curve->name = name;
        MREG.AddObjNew(curve);
    }
    curve->algon = 2;
    curve->bShowLines = 1;
    curve->bShowPoints = 1;
    //curve->bShowName = 1;
    curve->spl.push_back(p1);
    curve->spl.push_back(p2);
    RGB2GL2(RGB(rand()%255,rand()%255,rand()%255),curve->color);

}

void CreateLineC(Pnt p1, Pnt p2, string name, int r, int g, int b)
{
    CObjRegistry& MREG = CObjRegistry::getLayerInstance();
    Curve* curve;
    curve = (Curve*)MREG.FindObj(name);
    if(curve == 0)
    {
       curve = new Curve();
       curve->name = name;
       curve->algon = 2;
       curve->bShowLines = 1;
       curve->bShowPoints = 1;
       RGB2GL2(RGB(r,g,b),curve->color);
       //curve->bShowName = 1;
       MREG.AddObjNew(curve);
    }
    curve->spl.clear();
    curve->spl.push_back(p1);
    curve->spl.push_back(p2);
}


//парам. ур-е линии
void CreateLineP(Pnt p0, Pnt V, string name)
{
    //wait_cond.wait(&mutex);
    //flags.bdisable_timer2 = true;
    //Sleep(10);
    //mutex.lock();
    CObjRegistry& MREG = CObjRegistry::getLayerInstance();
    Curve* curve;
    curve = (Curve*)MREG.FindObj(name);
    if(curve == 0)
    {
       curve = new Curve();
       curve->name = name;
       curve->algon = 2;
       curve->bShowLines = 1;
       curve->bShowPoints = 1;
       curve->n_attr_set = 2;
       //curve->bShowName = 1;
       RGB2GL2(RGB(rand()%255,rand()%255,rand()%255),curve->color);
       MREG.AddObjNew(curve);
    }
   curve->spl.clear();

   for(float t=0; t<100; t+=1)
   {
       Pnt p1 = Pnt( p0.x + V.x * t, p0.y + V.y * t, p0.z + V.z * t );
       curve->spl.push_back(p1);
   }
   //mutex.unlock();
   //flags.bdisable_timer2 = false;

}

void DeleteLines()
{
    CObjRegistry& MREG = CObjRegistry::getLayerInstance();
    for(int i=0; i < (int)MREG.allObj.size(); i++)
    {
        Object* o = MREG.allObj[i];
        if( o->classname == "Curve" )
            MREG.removeQuenue.push_back(o);
    }
}

void DeleteLine(string name)
{
    CObjRegistry& MREG = CObjRegistry::getLayerInstance();
    for(int i=0; i < (int)MREG.allObj.size(); i++)
    {
        Object* o = MREG.allObj[i];
        if( o->name == name )
            MREG.removeQuenue.push_back(o);
    }
}

void DeletePoints()
{
    CObjRegistry& MREG = CObjRegistry::getLayerInstance();
    for(int i=0; i < (int)MREG.allObj.size(); i++)
    {
        Object* o = MREG.allObj[i];
        if( o->classname == "VPnt" )
            MREG.removeQuenue.push_back(o);
    }
}

