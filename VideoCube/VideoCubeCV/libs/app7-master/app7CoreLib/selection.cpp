/**********************************************************************
 *  Project App6
 *
 *  selection.cpp
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

#include <QtOpenGL>
#include <QOpenGLFunctions_4_3_Core>
#include "../include/selection.h"
#include "../include/print.h"
#include "../include/vector3d.h"
#include "../include/cinput.h"
#include "../include/gemobject.h"
#include "../include/group.h"
#include "../include/cobjregistry.h"
#include <GL/glu.h>


CSelection CSelection::instance;

CSelection& CSelection::getInstance()
{
    return instance;
}

CSelection::CSelection()
{
    //iinput->add_delegat(this);
}

void CSelection::AddObj(Object* obj)
{
    if(obj == NULL) return;
    lobjnames.push_back(obj->name);
    objnames = objnames + " " + obj->name;
    obj->isSelected = 2;
    obj->OnSelect();
    objs.push_back(obj);
    //mu_printf("obj %s added to selection", obj->name.c_str());
};

void CSelection::RemObj(Object* obj)
{
    vector<string>::iterator it;
    if( find(objs.begin(), objs.end(), obj) == objs.end() )
        return;

    it = find(lobjnames.begin(), lobjnames.end(), obj->name);
    if(it!=lobjnames.end())
    {
        lobjnames.erase(it);
        objnames="";
        for(it=lobjnames.begin(); it!=lobjnames.end();it++)
        {
            objnames = objnames + " " + *it;
        }
    }

    obj->isSelected = 0;
    obj->OnUnselect();
    //mu_printf("obj %s removed from selection", obj->name.c_str());

    //it = find_if (myvector.begin(), myvector.end(), IsOdd);
    objs.erase(find(objs.begin(), objs.end(), obj));
};



void projectp(Pnt op, CVector3& p1)
{
#if 1
  GLint    viewport[4];
  GLdouble projection[16];
  GLdouble modelview[16];
  GLdouble vx,vy,vz;
  glGetIntegerv(GL_VIEWPORT,viewport);
  glGetDoublev(GL_PROJECTION_MATRIX,projection);
  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);

  gluProject(op.x, op.y, op.z, modelview, projection, viewport, &vx, &vy, &vz);
  p1 = CVector3(vx,vy,vz);
#endif
}

extern int getResX();
extern int getResY();

bool isMouseInPolygon(Pnt m, Pnt p1, Pnt p2, Pnt p3, Pnt p4)
{
  #if 1
  Pnt p[5];
  float minx=100000,maxx=-100000,miny=100000,maxy=-100000;
  GLint    viewport[4];
  GLdouble projection[16];
  GLdouble modelview[16];

  glGetIntegerv(GL_VIEWPORT,viewport);
  glGetDoublev(GL_PROJECTION_MATRIX,projection);
  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);

  //mu_printf("viewport %d %d %d %d", viewport[0],viewport[1],viewport[2],viewport[3]);
  //mu_printf("modelview %f %f %f %f %f %f %f %f", modelview[0],modelview[1],modelview[2],modelview[3],
//												  modelview[4],modelview[5],modelview[6],modelview[7]);

  GLdouble d[4];
  gluProject(p1.x, p1.y, p1.z, modelview, projection, viewport, &d[1], &d[2], &d[3]);
  p[1].x = d[1]; p[1].y=d[2];
  gluProject(p2.x, p2.y, p2.z, modelview, projection, viewport, &d[1], &d[2], &d[3]);
  p[2].x = d[1]; p[2].y=d[2];
  gluProject(p3.x, p3.y, p3.z, modelview, projection, viewport, &d[1], &d[2], &d[3]);
  p[3].x = d[1]; p[3].y=d[2];
  gluProject(p4.x, p4.y, p4.z, modelview, projection, viewport, &d[1], &d[2], &d[3]);
  p[4].x = d[1]; p[4].y=d[2];

  for(int i=1; i<5; i++)
  {
    if(p[i].x < minx)
        minx = p[i].x;
    if(p[i].y < miny)
        miny = p[i].y;
    if(p[i].x > maxx)
        maxx = p[i].x;
    if((p[i].y) > maxy)
        maxy = p[i].y;
  }

  //prn("x {%f,%f} y{%f,%f}",minx,maxy,maxx,maxy);

  int RES_X = viewport[2];
  int RES_Y = viewport[3];
  //prn("m.x=%f", m.x);
 // prn("m.y=%f", m.y);
  //prn("RES_X=%d", RES_X);
  //prn("RES_Y=%d", RES_Y);

  if( (m.x >= minx) && (m.x <= maxx) && (RES_Y - m.y >= miny) && (RES_Y - m.y <= maxy) )
      return true;
  else
      return false;

#endif
  return 1;

}

Object* CSelection::SelectByMouseTop(double vx, double vy, double vz)
{
    vx=0;vy=0;vz=0;
    vector<Object*>::iterator obj;
    vector<Object*>::iterator obj2;
    Object* go;
    Pnt vbox[9];
    //mu_printf(string(p1.toStr()+" "+p2.toStr()).c_str() );
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    CInput* iinput = CInput::getInstance();
    //find obj
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {

        if((*obj)->isHidden>0)
            continue;

        if((*obj)->isGroup)
        {
            Group* gr = (Group*)*obj;
            for(obj2 = gr->objs.begin(); obj2 != gr->objs.end(); obj2++ )
            {
                go = (*obj2);

                if( (iinput->vx > go->ei.coord.x - go->bounds.x/2) && (iinput->vx < go->ei.coord.x + go->bounds.x/2) )
                if( (iinput->vz > go->ei.coord.z - go->bounds.z/2) && (iinput->vz < go->ei.coord.z + go->bounds.z/2) )
                {
                    //gr->isSelected = 1;
                    return gr;
                }




            }
        }

        if((*obj)->isCanRender)
        {
            go = (*obj);

            if( (iinput->vx >= go->ei.coord.x - go->bounds.x) && (iinput->vx <= go->ei.coord.x + go->bounds.x) )
            if( (iinput->vz >= go->ei.coord.z - go->bounds.z) && (iinput->vz <= go->ei.coord.z + go->bounds.z) )
            {
                //go->isSelected = 1;
                return go;
            }


        }


    }
    return NULL;

}

void CSelection::SelectByName(char* name)
{
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    Object* obj = MREG.DeepFindObj(name);
    if( obj!=NULL)
        AddObj(obj);

}

void CSelection::UnSelectByName(char* name)
{
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    Object* obj = MREG.DeepFindObj(name);
    if( obj!=NULL)
        RemObj(obj);

}



Object* CSelection::SelectByMouse(float x, float y)
{
    vector<Object*>::iterator obj;
    vector<Object*>::iterator obj2;
    Object* go;
    Pnt vbox[9];
    //mu_printf(string(p1.toStr()+" "+p2.toStr()).c_str() );

    //prn("%f %f",x,y);
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    //find obj
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {


        if((*obj)->isHidden>0)
            continue;

        if((*obj)->classname == "Curve")
        {
            Group* gr = (Group*)*obj;
            for(obj2 = gr->objs.begin(); obj2 != gr->objs.end(); obj2++ )
            {
                go = (*obj2);
                Pnt bnd = go->bounds * 2;

                vbox[1] = Pnt(go->ei.coord.x - bnd.x/2, go->ei.coord.y + bnd.y/2, go->ei.coord.z - bnd.z/2);
                vbox[2] = Pnt(go->ei.coord.x - bnd.x/2, go->ei.coord.y + bnd.y/2, go->ei.coord.z + bnd.z/2);
                vbox[3] = Pnt(go->ei.coord.x + bnd.x/2, go->ei.coord.y + bnd.y/2, go->ei.coord.z + bnd.z/2);
                vbox[4] = Pnt(go->ei.coord.x + bnd.x/2, go->ei.coord.y + bnd.y/2, go->ei.coord.z - bnd.z/2);
                vbox[5] = Pnt(go->ei.coord.x - bnd.x/2, go->ei.coord.y - bnd.y/2, go->ei.coord.z - bnd.z/2);
                vbox[6] = Pnt(go->ei.coord.x - bnd.x/2, go->ei.coord.y - bnd.y/2, go->ei.coord.z + bnd.z/2);
                vbox[7] = Pnt(go->ei.coord.x + bnd.x/2, go->ei.coord.y - bnd.y/2, go->ei.coord.z - bnd.z/2);
                vbox[8] = Pnt(go->ei.coord.x + bnd.x/2, go->ei.coord.y - bnd.y/2, go->ei.coord.z + bnd.z/2);

                bool match = 0;
                if(isMouseInPolygon(Pnt(x,y,0),vbox[1],vbox[4],vbox[7],vbox[5]))
                    match = 1;
                if(isMouseInPolygon(Pnt(x,y,0),vbox[4],vbox[7],vbox[8],vbox[3]))
                    match = 1;
                if(isMouseInPolygon(Pnt(x,y,0),vbox[8],vbox[3],vbox[2],vbox[6]))
                    match = 1;
                if(isMouseInPolygon(Pnt(x,y,0),vbox[5],vbox[6],vbox[1],vbox[2]))
                    match = 1;
                if(isMouseInPolygon(Pnt(x,y,0),vbox[1],vbox[4],vbox[3],vbox[2]))
                    match = 1;
                if(isMouseInPolygon(Pnt(x,y,0),vbox[5],vbox[7],vbox[8],vbox[6]))
                    match = 1;

                if(match == 1)
                {
                    go->isSelected = 1;
                    //prn("%s ",go->name.c_str());
                    return go;
                }



            }
        }

        if((*obj)->isCanRender)
        {
            go = (*obj);

            vbox[1] = Pnt(go->ei.coord.x - go->bounds.x/2, go->ei.coord.y + go->bounds.y/2, go->ei.coord.z - go->bounds.z/2);
            vbox[2] = Pnt(go->ei.coord.x - go->bounds.x/2, go->ei.coord.y + go->bounds.y/2, go->ei.coord.z + go->bounds.z/2);
            vbox[3] = Pnt(go->ei.coord.x + go->bounds.x/2, go->ei.coord.y + go->bounds.y/2, go->ei.coord.z + go->bounds.z/2);
            vbox[4] = Pnt(go->ei.coord.x + go->bounds.x/2, go->ei.coord.y + go->bounds.y/2, go->ei.coord.z - go->bounds.z/2);
            vbox[5] = Pnt(go->ei.coord.x - go->bounds.x/2, go->ei.coord.y - go->bounds.y/2, go->ei.coord.z - go->bounds.z/2);
            vbox[6] = Pnt(go->ei.coord.x - go->bounds.x/2, go->ei.coord.y - go->bounds.y/2, go->ei.coord.z + go->bounds.z/2);
            vbox[7] = Pnt(go->ei.coord.x + go->bounds.x/2, go->ei.coord.y - go->bounds.y/2, go->ei.coord.z - go->bounds.z/2);
            vbox[8] = Pnt(go->ei.coord.x + go->bounds.x/2, go->ei.coord.y - go->bounds.y/2, go->ei.coord.z + go->bounds.z/2);


            if(isMouseInPolygon(Pnt(x,y,0),vbox[1],vbox[4],vbox[7],vbox[5]))
                return *obj;
            if(isMouseInPolygon(Pnt(x,y,0),vbox[4],vbox[7],vbox[8],vbox[3]))
                return *obj;
            if(isMouseInPolygon(Pnt(x,y,0),vbox[8],vbox[3],vbox[2],vbox[6]))
                return *obj;
            if(isMouseInPolygon(Pnt(x,y,0),vbox[5],vbox[6],vbox[1],vbox[2]))
                return *obj;
            if(isMouseInPolygon(Pnt(x,y,0),vbox[1],vbox[4],vbox[3],vbox[2]))
                return *obj;
            if(isMouseInPolygon(Pnt(x,y,0),vbox[5],vbox[7],vbox[8],vbox[6]))
                return *obj;


        }


    }


    return NULL;
}


void CSelection::ResetSelection()
{
    vector<Object*>::iterator obj;
    vector<Object*>::iterator obj2;
    //mu_printf(string(p1.toStr()+" "+p2.toStr()).c_str() );

    objs.clear();
    objnames="";

    CObjRegistry* MREG = CObjRegistry::getInstance();
    //find obj
    for(obj = MREG->allObj.begin(); obj != MREG->allObj.end(); obj++ )
    {
        if((*obj)->classname == "Group")
        {
            Group* g = (Group*)(*obj);
            for(obj2 = g->objs.begin(); obj2 != g->objs.end(); obj2++ )
            {

                if((*obj2)->isSelected>0)
                {
                    (*obj2)->isSelected = 0;
                    (*obj2)->OnUnselect();

                }
            }
        }

        if((*obj)->isSelected>0)
        {
            (*obj)->isSelected = 0;
            (*obj)->OnUnselect();

        }


    }

    //main_if.selobj = NULL;

    return;
}

CVector3 getNorm(CVector3 P1, CVector3 P2, CVector3 P3)
{
    CVector3 a = P2 - P1;
    CVector3 b = P3 - P1;
    CVector3 ret;
/*	float mult=0;
    float a[3];
    float b[3];
    float c[3];
    float n[3];

    a[0]=P1.x; a[1]=P1.y; a[2]=P1.z;
    b[0]=P2.x; b[1]=P2.y; b[2]=P2.z;
    c[0]=P3.x; c[1]=P3.y; c[2]=P3.z;
    n[0]=(b[1]-a[1])*(c[2]-a[2])-(b[2]-a[2])*(c[1]-a[1]);
    n[1]=(c[0]-a[0])*(b[2]-a[2])-(b[0]-a[0])*(c[2]-a[2]);
    n[2]=(b[0]-a[0])*(c[1]-a[1])-(c[0]-a[0])*(b[1]-a[1]);
    for (int i=0;i<3;i++) mult+=a[i]*n[i];
    if (mult<0) for (int j=0;j<3;j++) n[j]=-n[j];
*/
    //ret.x = n[0];ret.y = n[1];ret.z = n[2];
    ret = a ^ b;
    ret.mag = ret.findMagnitude();
    ret = ret / ret.mag;

    return ret;
}

int f1_sgn(const float& k)
{
  if( k > 0 ) return 1;
  if( k < 0 ) return -1;
  return 0;
}

bool intersect_triangle_line(CVector3  v1,
                             CVector3  v2,
                             CVector3  v3,
                             CVector3  n,
                             CVector3  p1,
                             CVector3  p2,
                             CVector3& pc)
{
  float r1 = n | (p1 - v1);
  float r2 = n | (p2 - v1);
  if( f1_sgn(r1) == f1_sgn(r2) ) return false;
  CVector3 ip = (p1 + ((p2 - p1) * (-r1 / (r2 - r1))));
  if( (((v2 - v1) ^ (ip - v1)) | n) >= 0)
  {
      if( (((v3 - v2) ^ (ip - v2)) | n) <= 0) return false;
      if( (((v1 - v3) ^ (ip - v3)) | n) <= 0) return false;
  }
  else
  {
      if( (((v3 - v2) ^ (ip - v2)) | n) >= 0) return false;
      if( (((v1 - v3) ^ (ip - v3)) | n) >= 0) return false;
  }  pc = ip; return true;
}


//x y mouse
Object* CSelection::SelectByVector(Pnt p1, Pnt p2)
{
    vector<Object*>::iterator obj;
    //mu_printf(string(p1.toStr()+" "+p2.toStr()).c_str() );
    p1=p2;
    CObjRegistry* MREG = CObjRegistry::getInstance();
    //find obj
    for(obj = MREG->allObj.begin(); obj != MREG->allObj.end(); obj++ )
    {


        if((*obj)->classname == "GemObject")
        if((*obj)->name == "kuzov")
        {
            GemObject* go = (GemObject*)(*obj);
            CVector3 vbox[9];

            //mu_printf("%s %f %f %f", go->NODE_NAME.c_str(), go->bounds.x, go->bounds.y, go->bounds.z);

            vbox[1] = CVector3(go->C0.x - go->bounds.x/2, go->C0.y + go->bounds.y/2, go->C0.z - go->bounds.z/2);
            vbox[2] = CVector3(go->C0.x - go->bounds.x/2, go->C0.y + go->bounds.y/2, go->C0.z + go->bounds.z/2);
            vbox[3] = CVector3(go->C0.x + go->bounds.x/2, go->C0.y + go->bounds.y/2, go->C0.z + go->bounds.z/2);
            vbox[4] = CVector3(go->C0.x + go->bounds.x/2, go->C0.y + go->bounds.y/2, go->C0.z - go->bounds.z/2);
            vbox[5] = CVector3(go->C0.x - go->bounds.x/2, go->C0.y - go->bounds.y/2, go->C0.z - go->bounds.z/2);
            vbox[6] = CVector3(go->C0.x - go->bounds.x/2, go->C0.y - go->bounds.y/2, go->C0.z + go->bounds.z/2);
            vbox[7] = CVector3(go->C0.x + go->bounds.x/2, go->C0.y - go->bounds.y/2, go->C0.z - go->bounds.z/2);
            vbox[8] = CVector3(go->C0.x + go->bounds.x/2, go->C0.y - go->bounds.y/2, go->C0.z + go->bounds.z/2);
            CVector3 pc;
            /*
            if( intersect_triangle_line(vbox[1],vbox[4],vbox[5],
                                getNorm(vbox[1],vbox[4],vbox[5]),
                                p1,p2,pc) )
                return *obj;
            if( intersect_triangle_line(vbox[5],vbox[4],vbox[7],
                                getNorm(vbox[5],vbox[4],vbox[7]),
                                p1,p2,pc) )
                return *obj;
            if( intersect_triangle_line(vbox[4],vbox[7],vbox[8],
                                getNorm(vbox[4],vbox[7],vbox[8]),
                                p1,p2,pc) )
                return *obj;
            if( intersect_triangle_line(vbox[4],vbox[8],vbox[3],
                                getNorm(vbox[4],vbox[8],vbox[3]),
                                p1,p2,pc) )
                return *obj;
            if( intersect_triangle_line(vbox[6],vbox[3],vbox[8],
                                getNorm(vbox[6],vbox[3],vbox[8]),
                                p1,p2,pc) )
                return *obj;
            if( intersect_triangle_line(vbox[6],vbox[3],vbox[2],
                                getNorm(vbox[6],vbox[3],vbox[2]),
                                p1,p2,pc) )
                return *obj;
            if( intersect_triangle_line(vbox[5],vbox[6],vbox[2],
                                getNorm(vbox[5],vbox[6],vbox[2]),
                                p1,p2,pc) )
                return *obj;
            if( intersect_triangle_line(vbox[1],vbox[5],vbox[2],
                                getNorm(vbox[1],vbox[5],vbox[2]),
                                p1,p2,pc) )
                return *obj;


*/



        }
        else
        {
        }


    }


    return NULL;
}


void CSelection::SelectByGUID(int guid)
{
    vector<Object*>::iterator obj;

    char gstr3[255];
    sprintf(gstr3, "%d", guid);
    string sguid = gstr3;

    CObjRegistry* MREG = CObjRegistry::getInstance();
    for(obj = MREG->allObj.begin(); obj != MREG->allObj.end(); obj++ )
    {
         if((*obj)->ei.GUID == sguid)
         {
             objs.push_back(*obj);
         }
    }

}

void CSelection::key_pressed(int k)
{
    k=0;
}
