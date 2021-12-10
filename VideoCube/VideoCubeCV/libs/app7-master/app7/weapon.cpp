/**********************************************************************
 *  Project App6
 *
 *  weapon.cpp
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

#include "../include/weapon.h"
#include "../include/vector3d.h"
#include "../include/glcode.h"
#include "../include/material.h"
#include "../include/cobjregistry.h"
#include "../include/group.h"
#include "../include/print.h"
#include "Wm5IntrRay3Box3.h"
#include "Wm5Matrix3.h"
#include "drawings2d.h"

static CObjRegistry& MREG =  CObjRegistry::getInstance2();
using namespace Wm5;

Weapon::Weapon()
{
    speed = 1;
    ray_texture_id = -1;
    isCanRender = true;
    Nrays = 0;    
}

void Weapon::Init()
{
    if( ray_texture_id  == -1)
    {
        CMaterial* mater = (CMaterial*)MREG_FindObj("matman");
        switch (type) {
        case Weapon::RAILGUN:
              ray_texture_id = mater->LoadTexture("textures/laser/laser_beam2.jpg", imageSampler());
            break;
        default:
            break;
        }

        initializeGLFunctions();

        // Generate 2 VBOs
        glGenBuffers(2, vboIds);

    }
}

void Weapon::update()
{
    if( vertices.size() > 0 )
    {
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_DYNAMIC_DRAW );
        //glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexData), &vertices[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_DYNAMIC_DRAW );
        //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLushort), &indices[0]);
    }
}


void Weapon::Shot(CVector3 p, CVector3 dir)
{
    t = 0;
    Ray ray;
    ray.pos = p;
    ray.p0 =  p;
    ray.dir = dir;
    ray.life = 6000;
    rays[Nrays] = ray;
    Nrays++;
    if( Nrays > MAX_RAYS )
        Nrays = 0;
    //HitObjects(p, dir);
}

CVector3 get_newP(CVector3 p, CVector3 dir, double t)
{
    CVector3 p_new;
    p_new.x  = p.x - t*sin(GR * dir.y)*sin(GR * dir.x);
    p_new.y  = p.y + t*cos(GR * dir.y);
    p_new.z =  p.z + t*sin(GR * dir.y)*cos(GR * dir.x);
    return p_new;
}

void get_bounding_box(GemObject* go, Box3<double>& box, bool isStaticObject)
{
    Vector3<double> center;
    if( !isStaticObject )
       center = Vector3<double>(go->ei.coord.x, go->ei.coord.y, go->ei.coord.z);
    else
        center = Vector3<double>(go->C0.x, go->C0.y, go->C0.z) * go->ei.scale.x;
    double extent[3];
    Vector3<double> axis[3];
    Matrix3<double> mat;
    mat.MakeEulerXYZ(go->rot.x,go->rot.y,go->rot.z);
    axis[0] = Vector3<double>(1,0,0) * mat;
    axis[1] = Vector3<double>(0,1,0) * mat;
    axis[2] = Vector3<double>(0,0,1) * mat;
    extent[0] = go->bounds.x * go->ei.scale.x;
    extent[1] = go->bounds.y * go->ei.scale.x;
    extent[2] = go->bounds.z * go->ei.scale.x;
    // prn("%s %f",go->name.c_str(),go->ei.scale.x);
    box = Box3<double>(center,axis,extent);
}


void Weapon::HitObjects(CVector3 p, CVector3 dir)
{
    Vector3<double> p0(p.x,p.y,p.z);
    CVector3 p1 = get_newP(p,dir,5.5);
    CVector3 p2 = get_newP(p,dir,0);
    CVector3 d = p1 - p2;
    d.normalize();
    vector<Object*>::iterator i,j;
    map<double, string> hitMap;

    Ray3<double> ray(Vector3<double>(p.x,p.y,p.z),Vector3<double>(d.x,d.y,d.z));
    //CreateLineP(p.toPnt(),d.toPnt(),"HitLine");
   // CreateLine(Pnt(0,0,0),Pnt(10,0,0),"x line");


    for(i = MREG.allObj.begin(); i != MREG.allObj.end(); i++)
    {
        if( (*i)->classname == "Group" )
        {
            Group* g = (Group*)(*i);
            for(j = g->objs.begin(); j != g->objs.end(); j++)
            {
                if( (*j)->classname == "GemObject" )
                {
                    GemObject* go = (GemObject*)(*j);
                    Box3<double> box;
                    get_bounding_box(go, box, true);
                    IntrRay3Box3<double> intr(ray, box);
                    if( intr.Find() )
                    {
                        Vector3<double> hit = intr.GetPoint(0);
                        p0 = p0 - hit;
                        hitMap[p0.Length()] = go->name;
                        //prn("Hit the object %s in %f %f %f",go->name.c_str(), hit.X(),hit.Y(),hit.Z());
                    }
                }
            }

        }
        if( (*i)->classname == "GemObject" )
        {
            GemObject* go = (GemObject*)(*i);
            Box3<double> box;
            get_bounding_box(go, box, false);
            IntrRay3Box3<double> intr(ray, box);
            if( intr.Find() )
            {
                Vector3<double> hit = intr.GetPoint(0);
                p0 = p0 - hit;
                hitMap[p0.Length()] = go->name;
                //prn("Hit the object %s in %f %f %f",go->name.c_str(), hit.X(),hit.Y(),hit.Z());
            }

        }
    }
    map<double, string>::iterator k;
    for(k=hitMap.begin(); k!=hitMap.end(); k++)
    {
        prn("Hit the object %s at %f meters",k->second.c_str(), k->first);

    }
}

void Weapon::Render(QOpenGLShaderProgram& program)
{
    program.setUniformValue("RenderMode", 2);
    program.setUniformValue("texture", 0);

    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program.attributeLocation("a_texcoord");
    program.enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    //Offset to normal
    offset += sizeof(QVector2D);
    int normalLocation = program.attributeLocation("a_normal");
    program.enableAttributeArray(normalLocation);
    glVertexAttribPointer(normalLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    program.setUniformValue("model_matrix", QMatrix4x4());

    vertices.clear();
    indices.clear();

    for(int i=0; i<MAX_RAYS; i++)
    {
        if( rays[i].life > 0 )
        {
            CVector3 p = rays[i].p0;
            CVector3 dir = rays[i].dir;


            //draw_p("p",p.toPnt(),RGB(255,0,0),true);

            CVector3 p1 = get_newP(p,dir,rays[i].t+5.5);
            p1.y-=0.5;
            CVector3 p2 = get_newP(p,dir,rays[i].t);
            p2.y-=0.5;
            CVector3 p3 = p1;
            p3.y+=0.5;
            CVector3 p4 = p2;
            p4.y+=0.5;
            //prn("p=%s",p1.toPnt().toStr().c_str());

            //glDisable(GL_TEXTURE_2D);
            VertexData v;
            v.position = QVector3D(p1.x,p1.y,p1.z);
            v.texCoord = QVector2D(0.0f,0.0f);
            vertices.push_back(v);
            v.position = QVector3D(p2.x,p2.y,p2.z);
            v.texCoord = QVector2D(1.0f,0.0f);
            vertices.push_back(v);
            v.position = QVector3D(p3.x,p3.y,p3.z);
            v.texCoord = QVector2D(0.0f,1.0f);
            vertices.push_back(v);
            v.position = QVector3D(p4.x,p4.y,p4.z);
            v.texCoord = QVector2D(1.0f,1.0f);
            vertices.push_back(v);
            indices.push_back(vertices.size()-4);
            indices.push_back(vertices.size()-3);
            indices.push_back(vertices.size()-2);
            indices.push_back(vertices.size()-1);

        }
    }

    update();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ray_texture_id);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
    //glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_SHORT, 0);

}

void Weapon::Slot1()
{
    for(int i=0; i<MAX_RAYS; i++)
    {
        if( rays[i].life > 0 )
        {
            rays[i].t+=speed;
            rays[i].pos = rays[i].p0 + rays[i].t*rays[i].dir;
            rays[i].life--;
        }
    }
}
