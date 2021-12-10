/**********************************************************************
 *  Project App6
 *
 *  group.cpp
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
#include <QGLFunctions>
#include "../include/gemobject.h"
#include "../include/mesh.h"
#include "../include/print.h"
#include "../include/glcode.h"
#include "../include/group.h"
#include "../include/flags.h"


void Group::Render(QOpenGLShaderProgram& program)
{
    t_flags& flags = GetGFlags();
    std::vector<Object*>::iterator j;
    for(j=objs.begin(); j!=objs.end(); j++	)
    {
        RenderObject* ro = (RenderObject*)(*j);
        ro->ei.scale = ei.scale;
        if( ro->isCanRender)
            ro->Render(program);
    }

}


void Group::ScaleGeometry(Pnt sf)
{
    //find bounds
    float xmax=-10000000,ymax=-10000000,zmax=-10000000;
    float xmin=10000000,ymin=10000000,zmin=10000000;
    sf=sf;


    std::vector<Object*>::iterator j;
    for(j=objs.begin(); j!=objs.end(); j++	)
    {
        if((*j)->classname != "GemObject") continue;
        Mesh& mesh =((GemObject*)(*j))->mesh;

        if(mesh.points.size()==0) return;
        for(int i=0; i<(int)mesh.points.size(); i++)
        {
            if(mesh.points[i].x > xmax)
                xmax = mesh.points[i].x;
            if(mesh.points[i].y > ymax)
                ymax = mesh.points[i].y;
            if(mesh.points[i].z > zmax)
                zmax = mesh.points[i].z;

            if(mesh.points[i].x < xmin)
                xmin = mesh.points[i].x;
            if(mesh.points[i].y < ymin)
                ymin = mesh.points[i].y;
            if(mesh.points[i].z < zmin)
                zmin = mesh.points[i].z;

        }
    }
    C0.x = xmin + (xmax - xmin) / 2;
    C0.y = ymin + (ymax - ymin) / 2;
    C0.z = zmin + (zmax - zmin) / 2;
    bounds.x = fabs(xmax - xmin);
    bounds.y = fabs(ymax - ymin);
    bounds.z = fabs(zmax - zmin);
    float sr = (bounds.x + bounds.y + bounds.z) / 3;
    float s = 3. /sr;
    bounds=bounds*s;
    ei.scale = Pnt(s,s,s);
}

void Group::Load(string& s)
{
    s=s;
    LoadFIO();
}

void Group::Save(string& s)
{
    s=s;
}

void  SeekNext(FILE* f, unsigned char sig, int Ntimes)
{
    int count = 0;
    while( !feof(f) )
    {
        unsigned char c;
        fread(&c, 1, 1, f);
        if( c == sig )
          count++;
        else
          count=0;
        if(count == Ntimes)
            break;
    }
}


void Group::LoadFIO()
{
    string fn = name + ".group";

    FILE* f = fopen(fn.c_str(), "rb");
    if( f == NULL ) return;
    isCanRender = false;
    isEntety = true;
    for(int i=0; i<n_obj; i++)
    {
        GemObject* go = new GemObject();
        go->LoadFIO(f,go);
        if(go->isCanRender == true)
        {
            go->SetName(go->nodetm.NODE_NAME);
            if(go->name!="")
            //prn("Loading from group %s", go->name.c_str());
            go->groupname = name;
            //go->ComputeC0();
            go->ei.coord = go->C0 * ei.scale;
            go->ComputeBounds();
            if(go==NULL) continue;
            obj_map[go->nodetm.NODE_NAME] = go;
            objs.push_back( go );
            lobjnames.push_back(go->name);
            objnames = objnames + " " + go->name;
        }
        SeekNext(f, 0xFA, 8);
        if( feof(f) )
            break;

    }
    fclose(f);
    isCanRender = true;
}


