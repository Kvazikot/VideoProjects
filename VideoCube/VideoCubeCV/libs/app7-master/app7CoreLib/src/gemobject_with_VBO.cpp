/**********************************************************************
 *  Project App6
 *
 *  gemobject.cpp
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

#include <GL/glew.h>
#include <QtOpenGL>
#include <QFile>
#include "material.h"
#include "var_parse.h"
#include "cobjregistry.h"
#include "gemobject.h"
#include "flags.h"
#include "print.h"

GemObject::GemObject() : vertexDataVBO(QGLBuffer::VertexBuffer)
      , normalDataVBO(QGLBuffer::VertexBuffer)
      , textureDataVBO(QGLBuffer::VertexBuffer)
      , indexDataVBO(QGLBuffer::IndexBuffer)

{
    bNotSerializeBase=0;
    ei.scale = Pnt(1,1,1);
    renderMode = 1;
    classname = "GemObject";
    bTransformReady = false;
    //PALLETE
    ar[0].col[0] = 0.0; ar[0].col[1] = 0.0; ar[0].col[2] = 1.0; ar[0].col[3] = 1.0;
    ar[1].col[0] = 0.0; ar[1].col[1] = 1.0; ar[1].col[2] = 1.0; ar[1].col[3] = 1.0;
    ar[2].col[0] = 1.0; ar[2].col[1] = 0.0; ar[2].col[2] = 1.0; ar[2].col[3] = 1.0;
    ar[3].col[0] = 0.0; ar[3].col[1] = 0.0; ar[3].col[2] = 0.0; ar[3].col[3] = 1.0;
    ar[4].col[0] = 0.5; ar[4].col[1] = 0.3f; ar[4].col[2] = 0.3f; ar[4].col[3] = 1.0;
    ar[5].col[0] = 0.0; ar[5].col[1] = 0.0; ar[5].col[2] = 1.0; ar[5].col[3] = 1.0;
    ar[6].col[0] = 1.0; ar[6].col[1] = 0.0; ar[6].col[2] = 0.0; ar[6].col[3] = 1.0;
    ar[7].col[0] = 1.0; ar[7].col[1] = 0.0; ar[7].col[2] = 0.0; ar[7].col[3] = 1.0;
    ar[8].col[0] = 1.0; ar[8].col[1] = 0.0; ar[8].col[2] = 0.0; ar[8].col[3] = 1.0;
    ar[9].col[0] = 1.0; ar[9].col[1] = 0.0; ar[9].col[2] = 0.0; ar[9].col[3] = 1.0;


    //anim = NULL;
    c=0;
    rot = Pnt(0,0,0);
    rot0 = Pnt(0,0,0);
    loc = Pnt(0,0,0);
    ei.scale = Pnt(1,1,1);
    ei.coord0 = Pnt(0,0,0);
    ei.rot = Pnt(0,0,0);
    ei.texture_id = -1;
    ei.texture_name = "";
    dir = -1;
    list = 0;
    firsttime = 1;
    indexes = NULL;
    vertices = NULL;
    colorArray = NULL;
    texcoords = NULL;
    vert_count = 0;
    face_count = 0;
    bUpdateMesh = false;
    myGroup = 0;
    isCanRender = true;
    isEntety = true;
    isNotSaving = false;
}

GemObject::GemObject(GemObject* B)
:RenderObject()
{
    GemObject();
    name = "aa";
    m_Updated = 1;
    isCanRender = true;
    isEntety = true;
    isNotSaving = false;
    classname = "GemObject";
    myGroup = 0;
    isReference = true;

    NODE_NAME = name;
    NODE_PARENT = B->NODE_NAME;
    nodetm = B->nodetm;
    ei.scale = B->ei.scale;
    rot = B->rot;
    rot0 = B->rot0;
    ei.rot = B->ei.rot;
    C0 = B->C0;
    renderMode = B->renderMode;
    memcpy(ar, B->ar, sizeof(t_col)*11);

    bounds = B->bounds;

    vert_count = B->vert_count;
    face_count = B->face_count;

    ei.texture_id = B->ei.texture_id;
    ei.texture_name = B->ei.texture_name;
    for(char i=0; i<4; i++)
        ei.wire_color[i] = B->ei.wire_color[i];

    vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
    indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));
    texcoords = (float*)malloc((vert_count+1)*2*sizeof(float));

   //vertices = B->vertices;
    memcpy(vertices, B->vertices,sizeof(float)*3*vert_count);
    //indexes = B->indexes;
    memcpy(indexes, B->indexes,sizeof(unsigned int)*3*face_count);
    //faces = B->faces;
    //memcpy(faces, B->faces,sizeof(unsigned int)*vert_count * 4);
    //texcoords = B->texcoords;
    memcpy(texcoords, B->texcoords,sizeof(float)*vert_count * 2);


    ranges = B->ranges;
    list = 0;
}

GemObject::GemObject(const GemObject& B)
:RenderObject()
{
    GemObject();
    name = "aa";
    m_Updated = 1;
    isCanRender = true;
    isEntety = true;
    isNotSaving = false;
    classname = "GemObject";
    myGroup = 0;
    isReference = true;

    NODE_NAME = name;
    NODE_PARENT = B.NODE_NAME;
    nodetm = B.nodetm;
    ei.scale = B.ei.scale;
    rot = B.rot;
    rot0 = B.rot0;
    ei.rot = B.ei.rot;
    C0 = B.C0;
    renderMode = B.renderMode;
    memcpy(ar, B.ar, sizeof(t_col)*11);

    bounds = B.bounds;

    vert_count = B.vert_count;
    face_count = B.face_count;

    ei.texture_id = B.ei.texture_id;
    ei.texture_name = B.ei.texture_name;
    for(char i=0; i<4; i++)
        ei.wire_color[i] = B.ei.wire_color[i];

    vertices = B.vertices;
    indexes = B.indexes;
    faces = B.faces;
    texcoords = B.texcoords;
    ranges = B.ranges;
    list = 0;
}

void GemObject::FreeMem()
{
    free(indexes);
    free(vertices);
    mesh.clear();
    ranges.clear();
}


void GemObject::LoadMesh(string goname)
{
        GemObject* B = (GemObject*)MREG_FindObj(goname);
        if(B==NULL)
        {
            //mu_printf("GemObject::LoadMesh cant find gemobject of name %s", goname.c_str());
            return;
        }

        char gstr3[255];
        sprintf(gstr3,"%d",(int)this % 1000);
        name = goname + gstr3;
        m_Updated = 1;
        isCanRender = true;
        isEntety = true;
        isNotSaving = true;
        classname = "GemObject";

        NODE_NAME = goname;
        NODE_PARENT = B->NODE_PARENT;
        nodetm = B->nodetm;
        rot = B->rot;
        rot0 = B->rot0;
        C0 = B->C0;
        memcpy(ar, B->ar, sizeof(t_col)*11);
        bounds = B->bounds;
        vert_count = B->vert_count;
        face_count = B->face_count;
        vertices = B->vertices;
        indexes = B->indexes;
        ranges = B->ranges;
}


void GemObject::CopyMesh(string goname)
{
    GemObject* B = (GemObject*)MREG_FindObj(goname);
    if(B==NULL)
    {
        //mu_printf("GemObject::LoadMesh cant find gemobject of name %s", goname.c_str());
        return;
    }
    Copy(B);
}


void ScaleMesh(Mesh& mesh, Pnt sf, Pnt C0)
{
    Pnt  B,A;
    for(int i=0; i < (int)mesh.points.size(); i++)
    {
            A = Pnt(mesh.points[i].x, mesh.points[i].y, mesh.points[i].z);
            mesh.points[i].x = (A.x - C0.x) * sf.x + C0.x;
            mesh.points[i].y = (A.y - C0.y) * sf.y + C0.y;
            mesh.points[i].z = (A.z - C0.z) * sf.z + C0.z;
    }
}


void GemObject::ComputeC0()
{
    float xmax=-10000000,ymax=-10000000,zmax=-10000000;
    float xmin=10000000,ymin=10000000,zmin=10000000;

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


    C0.x = xmin + (xmax - xmin) / 2;
    C0.y = ymin + (ymax - ymin) / 2;
    C0.z = zmin + (zmax - zmin) / 2;

    bounds.x = fabs(xmax - xmin);
    bounds.y = fabs(ymax - ymin);
    bounds.z = fabs(zmax - zmin);

    //?????????????????? ?????? app5
    //Pnt sf;
    //C0 = C0 * scale;
    ei.coord = C0;
    //bScaleToOnes = 1;
    //ScaleMesh(mesh, scale, C0);
    //nodetm.TM_POS = nodetm.TM_POS * sf;

}


void GemObject::ScaleGeometry(Pnt sf)
{
    //find bounds
    float xmax=-10000000,ymax=-10000000,zmax=-10000000;
    float xmin=10000000,ymin=10000000,zmin=10000000;
    sf = Pnt(0,0,0);
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

    bounds.x = fabs(xmax - xmin);
    bounds.y = fabs(ymax - ymin);
    bounds.z = fabs(zmax - zmin);
    float sr = (bounds.x + bounds.y + bounds.z) / 3;
    float s = 3. /sr;
    bounds=bounds*s;

    for(int i=0; i<(int)mesh.points.size(); i++)
    {
        mesh.points[i].x = mesh.points[i].x * s;
        mesh.points[i].y = mesh.points[i].y * s;
        mesh.points[i].z = mesh.points[i].z * s;
        vertices[i*3]=mesh.points[i].x;
        vertices[i*3+1]=mesh.points[i].y;
        vertices[i*3+2]=mesh.points[i].z;
    }



    C0.x = xmin*s + (xmax - xmin)*s / 2;
    C0.y = ymin*s + (ymax - ymin)*s / 2;
    C0.z = zmin*s + (zmax - zmin)*s / 2;


    //ei.scale = Pnt(s,s,s);
}


void GemObject::CenterModel()
{
    float xmax=-10000000,ymax=-10000000,zmax=-10000000;
    float xmin=10000000,ymin=10000000,zmin=10000000;

    for(int i=0; i<vert_count; i++)
    {

        if(vertices[i*3] > xmax)
            xmax = vertices[i*3] ;
        if(vertices[i*3+1] > ymax)
            ymax = vertices[i*3+1];
        if(vertices[i*3+2] > zmax)
            zmax = vertices[i*+2];

        if(vertices[i*3] < xmin)
            xmin = vertices[i*3];
        if(vertices[i*3+1] < ymin)
            ymin = vertices[i*3+1];
        if(vertices[i*3+2] < zmin)
            zmin = vertices[i*3+2];

    }

    for(int i=0; i<vert_count; i++)
    {
        vertices[i*3]   -=xmin + (xmax - xmin)/2 ;
        vertices[i*3+1] -=ymin + (ymax - ymin) / 2;
        vertices[i*3+2]	-=zmin + (zmax-zmin)/2;
    }


    C0.x = 0;
    C0.y = 0;
    C0.z = 0;

    bounds.x = fabs(xmax - xmin);
    bounds.y = fabs(ymax - ymin);
    bounds.z = fabs(zmax - zmin);

}

void GemObject::FillArrays()
{
    t_range r;
    vert_count = mesh.points.size();
    vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
    colorArray = (float*)malloc((vert_count+1)*3*sizeof(float));
    for(int i=0; i<(int)mesh.points.size(); i++)
    {
        vertices[i*3]=mesh.points[i].x;
        vertices[i*3+1]=mesh.points[i].y;
        vertices[i*3+2]=mesh.points[i].z;
        colorArray[i*3] = (float)rand()/RAND_MAX;
        colorArray[i*3+1] = 0.;
        colorArray[i*3+2] = 0.;
    }

    indexes = (unsigned int*)malloc((mesh.faces.size()+1)*3*sizeof(int));

    int id=-1,st=0;
    face_count = mesh.faces.size();
    for(int i=0; i<(int)mesh.faces.size(); i++)
    {
        int A = mesh.faces[i].vertices[0].index;
        int B = mesh.faces[i].vertices[1].index;
        int C = mesh.faces[i].vertices[2].index;
        //int D = mesh.faces[i].vertices[3].index;
        indexes[i*3] = A;
        indexes[i*3+1] = B;
        indexes[i*3+2] = C;
        if(id!=mesh.faces[i].matid)
        {
            if(id!=-1)
            {
                id = mesh.faces[i].matid;
                r.start = st;
                r.end = i;
                ranges.push_back(r);
                //mu_printf("ran=%d-%d", r.start, r.end);
            }
            else
                id = mesh.faces[i].matid;
            st = i;
        }

    }
    r.start = r.end;
    r.end = mesh.faces.size();
    ranges.push_back(r);


    mu_printf("");


}


void GemObject::FillMesh()
{

    colorArray = (float*)malloc((vert_count+1)*3*sizeof(float));
    mesh.points.resize(vert_count);
    for(int i=0; i<vert_count; i++)
    {
        mesh.points[i].x = vertices[i*3];
        mesh.points[i].y = vertices[i*3+1];
        mesh.points[i].z = vertices[i*3+2];
        colorArray[i*3] = (float)rand()/RAND_MAX;
        colorArray[i*3+1] = 0.;
        colorArray[i*3+2] = 0.;
    }

    mesh.faces.resize(face_count);
    for(int i=0; i<(int)mesh.faces.size(); i++)
    {
        //int D = mesh.faces[i].vertices[3].index;
        int A = indexes[i*3];
        int B = indexes[i*3+1];
        int C = indexes[i*3+2];
        mesh.faces[i].vertices[0].index = A;
        mesh.faces[i].vertices[1].index = B;
        mesh.faces[i].vertices[2].index = C;


    }

    mu_printf("%s MESHES FILLED", name.c_str());


}

void GemObject::Load(string& s)
{
    s=s;
    QFile objFile;
    string fn = name + ".mesh";
    objFile.setFileName(fn.c_str());
    if( objFile.open(QFile::ReadWrite ))
    {
        Serialize(objFile, 0);
    }
}

void GemObject::Save(string& s)
{
    QFile objFile;
    s=s;
    string fn = name + ".mesh";
    objFile.setFileName(fn.c_str());
    if( objFile.open(QFile::ReadWrite | QFile::Truncate))
    {
        Serialize(objFile, 1);
    }
}


void GemObject::Serialize( QFile& archive, bool IsStoring )
{

    QDataStream a(&archive);

    //Object::Serialize( a , IsStoring);

    if ( IsStoring )
    {
        //str =NODE_NAME.c_str();	str+="\n";
        //a.WriteString(str);
        //WriteString(NODE_NAME, a);
        a.writeRawData(nodetm.NODE_NAME, 1055);
        //a.writeRawData(NODE_NAME.c_str(), NODE_NAME.size());
        //a.writeRawData((char*)&nodetm, sizeof(nodetm));
        //a.Write(&nodetm, sizeof(nodetm));
        a.writeRawData((char*)&rot, sizeof(rot));
        a.writeRawData((char*)&rot0, sizeof(rot));
        a.writeRawData((char*)&C0, sizeof(rot));
        a.writeRawData((char*)&bounds, sizeof(bounds));
        a.writeRawData((char*)&vert_count, sizeof(int));
        a.writeRawData((char*)&face_count, sizeof(int));
        a.writeRawData((char*)vertices, sizeof(float)*vert_count*3);
        a.writeRawData((char*)indexes, sizeof(int)*face_count*3);
        int i = ranges.size();
        a.writeRawData((char*)&i, sizeof(int));
        for(i=0; i<(int)ranges.size();i++)
            a.writeRawData((char*)&ranges[i], sizeof(t_range));
    }
    else
    {


        //a.readRawData((char*)NODE_NAME.c_str(), NODE_NAME.size());
          //ReadString(NODE_NAME, a);

          //Node_TM* nodetmold= new Node_TM();
          //a.readRawData((char*)&nodetm, sizeof(Node_TM));
          a.readRawData(nodetm.NODE_NAME, 1055);
          a.readRawData((char*)&rot, sizeof(rot));
          a.readRawData((char*)&rot0, sizeof(rot));
          a.readRawData((char*)&C0, sizeof(rot));
          a.readRawData((char*)&bounds, sizeof(bounds));
          a.readRawData((char*)&vert_count, sizeof(int));
          a.readRawData((char*)&face_count, sizeof(int));
          vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
          indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));
          texcoords = (float*)malloc((vert_count+1)*2*sizeof(float));
          normals = (float*)malloc((face_count+1)*3*sizeof(float));

          a.readRawData((char*)vertices, sizeof(float)*vert_count*3);
          a.readRawData((char*)indexes, sizeof(int)*face_count*3);
          int size;
          t_range r;
          a.readRawData((char*)&size, sizeof(int));
          //qDebug() << vert_count;
          //qDebug() << size;
          for(int i=0; i<size;i++)
          {
              a.readRawData((char*)&r, sizeof(t_range));
              ranges.push_back(r);
              //qDebug() << r.start << "\n";
              //qDebug() << r.end << "\n";
          }

          for(int i=0; i<vert_count; i+=4)
          {
              texcoords[i*2]=0.0;
              texcoords[i*2+1]=1.0;
              texcoords[i*2+2]=1.0;
              texcoords[i*2+3]=0.0;


          }

          FillMesh();
          ComputeC0();
          ei.scale = Pnt(1,1,1);

          // Create the buffer objects
          vertexDataVBO.create();
          normalDataVBO.create();
          textureDataVBO.create();
          indexDataVBO.create();

          // Copy data to video memory
          // Vertex data
          vertexDataVBO.bind();
          vertexDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
          vertexDataVBO.allocate(vertices, sizeof(GLfloat)*vert_count*3);
          vertexDataVBO.release();

          // Normal data
          normalDataVBO.bind();
          normalDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
          normalDataVBO.allocate(normals, sizeof(GLfloat)*face_count*3);
          normalDataVBO.release();

          // Texture coordinates
          textureDataVBO.bind();
          textureDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
          textureDataVBO.allocate(texcoords, sizeof(GLfloat)*vert_count*2);
          textureDataVBO.release();

          // Indexes
          indexDataVBO.bind();
          indexDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
          indexDataVBO.allocate(indexes, sizeof(int)*face_count*3);
          indexDataVBO.release();


          NODE_NAME = string(nodetm.NODE_NAME);
          NODE_NAME = StrRep(NODE_NAME,"\"","");
          if(name!=NODE_NAME)
          {
              Rename(NODE_NAME);
              SetName(NODE_NAME);
          }


      }

}

void GemObject::drawpoly3d(Pnt P1,Pnt P2,Pnt P3,Pnt P4)
{

    glEnable(GL_COLOR_MATERIAL);
    glColor4f(1,0,0,1);
    glBegin(GL_LINE_LOOP);
    //for(int i=1; i<9; i++)
    {
        glVertex3f(P1.x,P1.y,P1.z);
        glVertex3f(P2.x,P2.y,P2.z);
        glVertex3f(P3.x,P3.y,P3.z);
        glVertex3f(P4.x,P4.y,P4.z);
        //glVertex3f(P1.x,P1.y,P1.z);
    }
    glEnd();
   glDisable(GL_COLOR_MATERIAL);
   //if(gtimer.msec%10 == 0)
   //  c=c+0.01;
  // if(c>1.0) c=0;

}

void GemObject::OnSelect()
{
    GetGFlags().bChangedRenderMode = true;
}

void GemObject::OnUnselect()
{
    GetGFlags().bChangedRenderMode = true;
}

//object inspector event
void GemObject::OnAttrChange(t_atr& a)
{
    //mu_printf("GemObject::OnAttrChange %s data=%d", a.name.c_str(), a.data);
    if(  (a.name == "filename")  )
    {
        //asciloader.LoadASCIIIFile((char*)a.toString().c_str(), (char*)name.c_str());
        //mu_printf("LoadASCIIIFile %s %s", a.toString().c_str(), name.c_str());
    }
}


void GemObject::RenderVer0()
{
    if(isCanRender == false)
        return;

    t_flags& flags = GetGFlags();


    if(flags.bChangedRenderMode)
    {
        glDeleteLists(list, 1);
        list = 0;

    }

    if( texture_file!="" && ei.texture_id == -1 )
    {
        CMaterial* mater = (CMaterial*)MREG_FindObj("matman");
        ei.texture_id = mater->GenTexture(ei.wire_color);
    }

    glPushMatrix();
//-----------------------------------------------------------------------------
    glTranslatef (ei.coord.x, ei.coord.y, ei.coord.z ); //???????????????????? ??????????????
    glRotatef( rot0.x, dir, 0, 0);
    glRotatef( rot0.y, 0, dir, 0);
    glRotatef( rot0.z, 0, 0, dir);
    glTranslatef( C0.x, C0.y, C0.z );
    glRotatef( rot.x, dir, 0, 0);
    glRotatef( rot.y, 0, dir, 0);
    glRotatef( rot.z, 0, 0, dir);
    glTranslatef( -C0.x, -C0.y, -C0.z );

    glEnableClientState( GL_VERTEX_ARRAY );
    if(isSelected==2)
    {
        Pnt vbox[9];

        vbox[1] = Pnt(C0.x - bounds.x/2, C0.y + bounds.y/2, C0.z - bounds.z/2);
        vbox[2] = Pnt(C0.x - bounds.x/2, C0.y + bounds.y/2, C0.z + bounds.z/2);
        vbox[3] = Pnt(C0.x + bounds.x/2, C0.y + bounds.y/2, C0.z + bounds.z/2);
        vbox[4] = Pnt(C0.x + bounds.x/2, C0.y + bounds.y/2, C0.z - bounds.z/2);
        vbox[5] = Pnt(C0.x - bounds.x/2, C0.y - bounds.y/2, C0.z - bounds.z/2);
        vbox[6] = Pnt(C0.x - bounds.x/2, C0.y - bounds.y/2, C0.z + bounds.z/2);
        vbox[7] = Pnt(C0.x + bounds.x/2, C0.y - bounds.y/2, C0.z - bounds.z/2);
        vbox[8] = Pnt(C0.x + bounds.x/2, C0.y - bounds.y/2, C0.z + bounds.z/2);


        drawpoly3d(vbox[1],vbox[4],vbox[7],vbox[5]);
        drawpoly3d(vbox[4],vbox[7],vbox[8],vbox[3]);
        drawpoly3d(vbox[8],vbox[3],vbox[2],vbox[6]);
        drawpoly3d(vbox[5],vbox[6],vbox[1],vbox[2]);
        drawpoly3d(vbox[1],vbox[4],vbox[3],vbox[2]);
        drawpoly3d(vbox[5],vbox[7],vbox[8],vbox[6]);
    }

    glVertexPointer( 3, GL_FLOAT, 0, vertices );


    if(list>0)
    {
      GLfloat light_position[4] = {ei.coord.x, ei.coord.y+1, ei.coord.z, 1.0};
      glLightfv(GL_LIGHT1, GL_POSITION, light_position);
      glEnable(GL_LIGHT1);

        glCallList(list);
        glPopMatrix();
        glDisable(GL_LIGHT1);
        return;
    }

          list = glGenLists( 1 );
          glNewList( list, GL_COMPILE_AND_EXECUTE );

          //glDisable(GL_TEXTURE);

          if(!flags.bColorRendering && !flags.bWire)
          {
              if(1)
              for(int i=0; i<face_count; i++)
              {
                  //mater->Bind(ei.material);
                  //mater->BindTexture(ei.texture_name);
                  glBindTexture(GL_TEXTURE_2D, ei.texture_id);
                  glBegin(GL_TRIANGLES);
                      int j = indexes[i*3];
                      glTexCoord2f(0.0f, 0.0f);
                      //glTexCoord2f(texcoords[i*6], texcoords[i*6+1]);
                      glVertex3f(vertices[j*3], vertices[j*3+1], vertices[j*3+2]);
                      j = indexes[i*3+1];

                      glTexCoord2f(1.0f, 0.0f);
                      //glTexCoord2f(texcoords[i*6+2], texcoords[i*6+3]);
                      glVertex3f(vertices[j*3], vertices[j*3+1], vertices[j*3+2]);
                      j = indexes[i*3+2];

                      glTexCoord2f(1.0f, 1.0f);
                      //glTexCoord2f(texcoords[i*6+4], texcoords[i*6+5]);
                      glVertex3f(vertices[j*3], vertices[j*3+1], vertices[j*3+2]);
                  glEnd();
              }
          }
          for(int i=0; i<(int)ranges.size(); i++)
          {

                if(isSelected!=2)
                {
                    glEnable(GL_COLOR_MATERIAL);
                    glColor4f(0.5,0.5,0.5,1);
                }
                else
                    glDisable(GL_COLOR_MATERIAL);

              if(flags.bColorRendering)
              {
                  glEnable(GL_COLOR_MATERIAL);
                  glColor4f(ar[i].col[0],ar[i].col[1],ar[i].col[2],1);
                  glDrawElements( GL_TRIANGLES, (ranges[i].end - ranges[i].start)*3, GL_UNSIGNED_INT, indexes+ranges[i].start*3);
                  glDisable(GL_COLOR_MATERIAL);



              }

              if(flags.bWire)
              {
                  glEnable(GL_COLOR_MATERIAL);
                  glColor4f(1,1,1,1);
                  glDrawElements( GL_LINES, (ranges[i].end - ranges[i].start)*3, GL_UNSIGNED_INT, indexes+ranges[i].start*3);
                  glDisable(GL_COLOR_MATERIAL);
              }




                if(isSelected!=2)
                {
                    glDisable(GL_COLOR_MATERIAL);
                }



          }
          //glEnable(GL_TEXTURE);

          glEndList();

         glPopMatrix();


    return;
}


void GemObject::Render()
{
    t_flags& flags = GetGFlags();

    RenderOLD();
    return;
    if(isCanRender == false) return;

    glEnableClientState(GL_INDEX_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    if( texture_file!="" && ei.texture_id == -1 )
    {
        CMaterial* mater = (CMaterial*)MREG_FindObj("matman");
        if(texture_file=="no_file")
            ei.texture_id = mater->GenTexture(ei.wire_color);
        else
            ei.texture_id = mater->LoadTexture(texture_file);

        // Create the buffer objects
        vertexDataVBO.create();
        normalDataVBO.create();
        textureDataVBO.create();
        indexDataVBO.create();

        // Copy data to video memory
        // Vertex data
        vertexDataVBO.bind();
        vertexDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
        vertexDataVBO.allocate(vertices, sizeof(GLfloat)*vert_count*3);
        //vertexDataVBO.release();

        // Normal data
        //normalDataVBO.bind();
        //normalDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
        //normalDataVBO.allocate(normals, sizeof(GLfloat)*face_count*3);
        //normalDataVBO.release();

        // Texture coordinates
        textureDataVBO.bind();
        textureDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
        textureDataVBO.allocate(texcoords, sizeof(GLfloat)*vert_count*2);
        //textureDataVBO.release();

        // Indexes
        indexDataVBO.bind();
        indexDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
        indexDataVBO.allocate(indexes, sizeof(int)*face_count*3);
        //indexDataVBO.release();
    }

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    //glRotatef(90,0,0,1);
//    if( clipping.crop )
//    {
//        glTranslatef(clipping.clipu, clipping.clipv, 0);
//        glScalef(clipping.clipw, clipping.cliph, 1);
//    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    glTranslatef (ei.coord.x, ei.coord.y, ei.coord.z );
    glRotatef( rot0.x, 1, 0, 0);
    glRotatef( rot0.y, 0, 1, 0);
    glRotatef( rot0.z, 0, 0, 1);
    glTranslatef( C0.x, C0.y, C0.z );
    if( rotMode == EulerAngles)
    {
        glRotatef( rot.x, 1, 0, 0);
        glRotatef( rot.y, 0, 1, 0);
        glRotatef( rot.z, 0, 0, 1);
    }
    else // AxisAngle
        glRotatef(ei.rot.x,rot.x,rot.y,rot.z);
    glTranslatef( -C0.x, -C0.y, -C0.z );
    //glScalef(ei.scale.x,ei.scale.y,ei.scale.z);

    //-----------------------------------------------------------------------------


    // Vertex data
    vertexDataVBO.bind();
    glVertexPointer(3, GL_FLOAT,0,0);

    // Normal data
    //normalDataVBO.bind();
    //glNormalPointer(GL_FLOAT, 0, 0);

    // Texture coordinates
    textureDataVBO.bind();
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    // Indexes
    indexDataVBO.bind();
    //glIndexPointer(3, GL_UNSIGNED_INT, 0);

    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, ei.texture_id);
    glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, 0);

    //indexDataVBO.release();
    //textureDataVBO.release();
    //normalDataVBO.release();
    //vertexDataVBO.release();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}


void RGBA2GL(BYTE R, BYTE G, BYTE B, BYTE A, GLfloat* col)
{
    col[0] = (float)(R) / 255;
    col[1] = (float)(G) / 255;
    col[2] = (float)(B) / 255;
    col[3] = (float)(A) / 255;
}


void GemObject::RenderOLD()
{

    t_flags& flags = GetGFlags();

    if( renderMode == 0 )
    {
        RenderVer0(); return;
    }

    if(isCanRender == false) return;

    if(flags.bChangedRenderMode || bUpdateMesh)
    {
        glDeleteLists(list, 1);
        list = 0;
        bUpdateMesh = false;
    }

    if( texture_file!="" && ei.texture_id == -1 )
    {
        CMaterial* mater = (CMaterial*)MREG_FindObj("matman");
        if(texture_file=="no_file")
            ei.texture_id = mater->GenTexture(ei.wire_color);
        else
            ei.texture_id = mater->LoadTexture(texture_file);
    }

    glPushMatrix();
//-----------------------------------------------------------------------------
    if( bTransformReady )
       transform = transform_new;
    transform.Apply();

/*
    if( bIsBiped )
    {
        glTranslatef( ei.coord0.x, ei.coord0.y, ei.coord0.z );
        glRotatef(ei.rot.y,0,1,0);
        Pnt tv = Pnt(ei.coord.x - C0.x,  ei.coord.y - C0.y, ei.coord.z - C0.z);
        glTranslatef (tv.x, tv.y,tv.z );
    }
    else
        glTranslatef( 0, 0, 0 );

    glTranslatef( C0.x, C0.y, C0.z );
    if( !bIsBiped )
    {
        glRotatef( rot.x, 1, 0, 0);
        glRotatef( rot.y, 0, 0, 1);
        glRotatef( rot.z, 0, 1, 0);
    }
    else
        glRotatef( ei.rot.x, rot.x, rot.y, rot.z);

    glTranslatef( -C0.x, -C0.y, -C0.z );
*/

//-----------------------------------------------------------------------------

    if(isSelected==2)
    {
        Pnt vbox[9];
        vbox[1] = Pnt(C0.x - bounds.x/2, C0.y + bounds.y/2, C0.z - bounds.z/2);
        vbox[2] = Pnt(C0.x - bounds.x/2, C0.y + bounds.y/2, C0.z + bounds.z/2);
        vbox[3] = Pnt(C0.x + bounds.x/2, C0.y + bounds.y/2, C0.z + bounds.z/2);
        vbox[4] = Pnt(C0.x + bounds.x/2, C0.y + bounds.y/2, C0.z - bounds.z/2);
        vbox[5] = Pnt(C0.x - bounds.x/2, C0.y - bounds.y/2, C0.z - bounds.z/2);
        vbox[6] = Pnt(C0.x - bounds.x/2, C0.y - bounds.y/2, C0.z + bounds.z/2);
        vbox[7] = Pnt(C0.x + bounds.x/2, C0.y - bounds.y/2, C0.z - bounds.z/2);
        vbox[8] = Pnt(C0.x + bounds.x/2, C0.y - bounds.y/2, C0.z + bounds.z/2);
        drawpoly3d(vbox[1],vbox[4],vbox[7],vbox[5]);
        drawpoly3d(vbox[4],vbox[7],vbox[8],vbox[3]);
        drawpoly3d(vbox[8],vbox[3],vbox[2],vbox[6]);
        drawpoly3d(vbox[5],vbox[6],vbox[1],vbox[2]);
        drawpoly3d(vbox[1],vbox[4],vbox[3],vbox[2]);
        drawpoly3d(vbox[5],vbox[7],vbox[8],vbox[6]);

        GLfloat r,g,b;
        r = 0.3*rand()/RAND_MAX;
        g = 0.5*rand()/RAND_MAX;
        b = 0.2*rand()/RAND_MAX;
        //renderBitmapString(C0.x,C0.y+bounds.y,C0.z,0,0,b,GLUT_BITMAP_TIMES_ROMAN_24,(char*)name.c_str());
    }

    glVertexPointer( 3, GL_FLOAT, 0, vertices );

    if(list>0)
    {
        glCallList(list);
        glPopMatrix();
        return;
    }


          list = glGenLists( 1 );
          glNewList( list, GL_COMPILE_AND_EXECUTE );

         int j=0;

          if (flags.bScanRendering)
          {
                glEnable(GL_COLOR_MATERIAL);
                GLfloat color[4];
                RGB2GL2(idx,color);
                glColor3f(color[0],color[1],color[2]);
                glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT, color );
                glVertexPointer(3, GL_FLOAT, 0, vertices);
                glDrawElements(GL_TRIANGLES, face_count, GL_UNSIGNED_INT, indexes);
                glDisable(GL_COLOR_MATERIAL);
                glEndList();
                glPopMatrix();
                return;
          }

          if(( ei.texture_id == -1))
              for(int i=0; i<(int)ranges.size(); i++)
              {


                  if( ei.texture_name=="")
                  {
                      ei.wire_color[0] = ar[i].col[0];
                      ei.wire_color[1] = ar[i].col[1];
                      ei.wire_color[2] = ar[i].col[2];
                      ei.wire_color[3] = ar[i].col[3];
                  }


                  if(flags.bColorRendering)
                      glDrawElements( GL_TRIANGLES, (ranges[i].end - ranges[i].start)*3, GL_UNSIGNED_INT, indexes+ranges[i].start*3);

                  if(flags.bWire)
                  {
                      glEnable(GL_COLOR_MATERIAL);
                      glColor4f(1,1,1,1);
                      glDrawElements( GL_LINES, (ranges[i].end - ranges[i].start)*3, GL_UNSIGNED_INT, indexes+ranges[i].start*3);
                      glDisable(GL_COLOR_MATERIAL);
                  }
                  if(!flags.bColorRendering && !flags.bWire)
                  {
                      glEnableClientState(GL_VERTEX_ARRAY);
                      glDisable(GL_TEXTURE);
                      glEnable(GL_COLOR_MATERIAL);
                      glColor4f(ei.wire_color[0],ei.wire_color[2],ei.wire_color[3],ei.wire_color[4]);
                      glDrawElements( GL_TRIANGLES, (ranges[i].end - ranges[i].start)*3, GL_UNSIGNED_INT, indexes+ranges[i].start*3);
                      glDisable(GL_COLOR_MATERIAL);
                      glEnable(GL_TEXTURE);
                  }



              }



            if( (flags.bShading) &&  ( ei.texture_id != -1) )
            {
                glDisableClientState(GL_COLOR_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);
                glDisableClientState(GL_INDEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
                glEnable(GL_COLOR_MATERIAL);
                glColor4f(1,1,1,1);
                glVertexPointer( 3, GL_FLOAT, 0, vertices);
                glNormalPointer(GL_FLOAT, 0, normals );
                glBindTexture(GL_TEXTURE_2D, ei.texture_id);
                //glDrawElements(GL_LINES, face_count*3, GL_UNSIGNED_INT, indexes);
                glDrawElements(GL_TRIANGLES, face_count*3, GL_UNSIGNED_INT, indexes);

                glDisableClientState(GL_VERTEX_ARRAY);

            }


          glEndList();
          glPopMatrix();


}



void GemObject::LoadFIO(FILE* f,Object* group)
{
    char gstr[2048];

    fread(nodetm.NODE_NAME, 1, 1024, f);
    NODE_NAME = nodetm.NODE_NAME;
    fread(&rot,  sizeof(rot), 1, f);
    fread(&rot0, sizeof(rot), 1, f);
    fread(&C0, sizeof(rot), 1, f);
    fread(&bounds, sizeof(bounds), 1, f);
    fread(&vert_count, sizeof(int), 1, f);
    fread(&face_count, sizeof(int), 1, f);

    vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
    normals = (float*)malloc((face_count+1)*3*sizeof(float));
    indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));
    texcoords = (float*)malloc((vert_count+1)*2*sizeof(float));
    faces =  (unsigned int*)malloc( sizeof(unsigned int) * vert_count * 4 );
    fread(vertices, sizeof(float)*vert_count*3, 1, f);
    fread(indexes, sizeof(int)*face_count*3, 1, f);
    fread(texcoords, sizeof(float)*vert_count*2, 1, f);
    fread(normals, sizeof(float)*face_count*3, 1, f);





    for(int i=0; i<vert_count*3; i++)
    {
        /*if(vertices[i] > 10000)
          vertices[i] = 0;
        if(vertices[i] < -10000)
          vertices[i] = 0;
          */
       // qDebug("vertices %f",vertices[i]);
    }

    for(int i=0; i<face_count*3; i++)
    {
       // qDebug("normals %f",normals[i]);
        /*if(indexes[i] > vert_count)
          indexes[i] = 0;
        if(indexes[i] < 0)
          indexes[i] = 0;
          */
    }

    for(int i=0; i<vert_count*2; i++)
    {
      //  qDebug("texcoords %f",texcoords[i]);
        /*if(texcoords[i] > 100)
        {

          texcoords[i] = 100;

        }
        if(texcoords[i] < 0)
          texcoords[i] = 0;
          */
    }

    int size=0;
    fread(&size, sizeof(int), 1, f);
    t_range r;
    for(int i=0; i<size;i++)
    {
        fread(&r, sizeof(t_range), 1, f);
        ranges.push_back(r);
    }
    fread(gstr, 1, 1024, f); ei.texture_name = string(gstr);
    ei.texture_name = StrRep(ei.texture_name,"E:","C:");
    //qDebug(ei.texture_name.c_str());


    if(ei.texture_name!="no texture")
    {
        VarParser vp;
        string col_s = vp.GetVarFromStr("wire_color", gstr);
        string typ = vp.GetVarFromStr("type", gstr);
        texture_file = vp.GetVarFromStr("file", gstr);
        //texture_file = StrRep(texture_file,"E:","C:");
        string U_TilingS = vp.GetVarFromStr("U_Tiling", gstr);
        string V_TilingS = vp.GetVarFromStr("V_Tiling", gstr);

        string crop = vp.GetVarFromStr("crop", gstr);
        if( crop == "true" )
        //do cropping
        {
            clipping.clipu = StrToDouble(vp.GetVarFromStr("clipu", gstr));
            clipping.clipv = StrToDouble(vp.GetVarFromStr("clipv", gstr));
            clipping.clipw = StrToDouble(vp.GetVarFromStr("clipw", gstr));
            clipping.cliph = StrToDouble(vp.GetVarFromStr("cliph", gstr));
            clipping.crop = true;
        }
        else
            clipping.crop = false;
        vp.ParseColor(col_s, ei.wire_color);
        if(typ == "color")
            ei.texture_id = -1;
        //if(typ == "bitmap")
        //    ei.texture_id = mater->LoadTexture(texture_file);
        double U_Tiling = StrToDouble(U_TilingS);
        double V_Tiling = StrToDouble(V_TilingS);

        if(typ == "color")
        for(int i=0; i<vert_count*2; i++)
        {
           texcoords[i] = rand()/RAND_MAX;
        }

    }

        //Get4VertFaces();
    NODE_NAME = StrRep(NODE_NAME,"\"","");
    if(name!=NODE_NAME)
    {
        Rename(NODE_NAME);
        SetName(NODE_NAME);
    }
}


//?????????? ?????? ??????? ? ????????? ??? ??? app5
void GemObject::ComputeBounds()
{
    float xmax=-100000000,ymax=-100000000,zmax=-100000000;
    float xmin=100000000,ymin=100000000,zmin=100000000;


    for(int i=0; i<vert_count; i++)
    {
        if(vertices[i*3] > xmax)
            xmax = vertices[i*3];
        if(vertices[i*3+1] > ymax)
            ymax = vertices[i*3+1];
        if(vertices[i*3+2] > zmax)
            zmax = vertices[i*3+2];

        if(vertices[i*3] < xmin)
            xmin = vertices[i*3];
        if(vertices[i*3+1] < ymin)
            ymin = vertices[i*3+1];
        if(vertices[i*3+2] < zmin)
            zmin = vertices[i*3+2];

    }

    bounds.x = fabs(xmax - xmin);
    bounds.y = fabs(ymax - ymin);
    bounds.z = fabs(zmax - zmin);

}
