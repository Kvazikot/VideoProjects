#include <QtOpenGL>
#include <QFile>
#include "../include/material.h"
#include "../include/var_parse.h"
#include "../include/cobjregistry.h"
#include "../include/gemobject.h"
#include "../include/flags.h"
#include "../include/print.h"
#include "../include/glcode.h"

GemObject::GemObject()
{
    ei.scale = Pnt(1,1,1);
    renderMode = 1;
    classname = "GemObject";
    bIsBiped = false;
    initializeGLFunctions();
    rot = Pnt(0,0,0);
    rot0 = Pnt(0,0,0);
    loc = Pnt(0,0,0);
    ei.scale = Pnt(1,1,1);
    ei.coord0 = Pnt(0,0,0);
    ei.rot = Pnt(0,0,0);
    ei.texture_id = -1;
    ei.texture_name = "";
    dir = -1;
    firsttime = 1;
    indexes = NULL;
    vertices = NULL;
    colorArray = NULL;
    material = 0;
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
    material = 0;

    NODE_NAME = name;
    NODE_PARENT = B->NODE_NAME;
    nodetm = B->nodetm;
    ei.scale = B->ei.scale;
    rot = B->rot;
    rot0 = B->rot0;
    ei.rot = B->ei.rot;
    C0 = B->C0;
    renderMode = B->renderMode;
    bounds = B->bounds;
    material = B->material;

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
    material = B.material;
    renderMode = B.renderMode;
    bounds = B.bounds;

    vert_count = B.vert_count;
    face_count = B.face_count;

    ei.texture_id = B.ei.texture_id;
    ei.texture_name = B.ei.texture_name;
    for(char i=0; i<4; i++)
        ei.wire_color[i] = B.ei.wire_color[i];

    vertices = B.vertices;
    indexes = B.indexes;
    texcoords = B.texcoords;
    ranges = B.ranges;
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
        CObjRegistry* MREG = CObjRegistry::getInstance();
        GemObject* B = (GemObject*)MREG->DeepFindObj(goname);
        //B->isCanRender = false;
        if(B==NULL)
        {
            //mu_printf("GemObject::LoadMesh cant find gemobject of name %s", goname.c_str());
            return;
        }

        char gstr3[255];
        sprintf(gstr3,"%d",(int)rand() % 1000);
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
        bounds = B->bounds;
        ei.material = B->ei.material;
        vert_count = B->vert_count;
        face_count = B->face_count;
        vertices = B->vertices;
        material = B->material;
        indexes = B->indexes;
        normals = B->normals;
        texcoords = B->texcoords;
        ranges = B->ranges;        
        for(int i=0; i < 50; i++)
          ranges_array[i] = B->ranges_array[i];
        n_ranges = B->n_ranges;
        ei = B->ei;

        //for(int i=0; i < 3; i++)
        //  vboIds[i] = B->vboIds[i];
        InitVBO();

}

void GemObject::update()
{
    vertices2.clear();
    for(int i=0; i < vert_count; i++)
    {
        VertexData data;
        data.normal = QVector3D(normals[3*i],normals[3*i+1],normals[3*i+2]);
        data.texCoord = QVector2D(texcoords[2*i],texcoords[2*i+1]);
        data.position = QVector3D(vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
        vertices2.push_back(data);
    }

   inds.clear();
   for(int i=0; i<face_count*3; i++)
      inds.push_back(indexes[i]);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices2.size() * sizeof(VertexData), &vertices2[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, inds.size() * sizeof(GLushort), &inds[0]);
}


void GemObject::CopyMesh(string goname)
{
    CObjRegistry* MREG = CObjRegistry::getInstance();
    GemObject* B = (GemObject*)MREG->DeepFindObj(goname);
    if(B==NULL)
    {
        //mu_printf("GemObject::LoadMesh cant find gemobject of name %s", goname.c_str());
        return;
    }
    Copy(B);
}

void GemObject::Copy(Object* A)
{
    GemObject* B = (GemObject*)A;
    name = B->name + "_copy";
    m_Updated = 1;
    isCanRender = true;
    isEntety = true;
    isNotSaving = false;
    classname = "GemObject";


    NODE_NAME = B->NODE_NAME;
    NODE_PARENT = B->NODE_PARENT;
    nodetm = B->nodetm;
    rot = B->rot;
    rot0 = B->rot0;
    C0 = B->C0;
    bounds = B->bounds;
    ei.material = B->ei.material;
    material = B->material;
    vert_count = B->vert_count;
    face_count = B->face_count;
    ei = B->ei;

    vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
    indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));
    normals = (float*)malloc((vert_count+1)*3*sizeof(float));
    texcoords = (float*)malloc((vert_count+1)*2*sizeof(float));

    memcpy(vertices, B->vertices, B->vert_count * sizeof(float) * 3);
    memcpy(normals, B->normals, B->vert_count * sizeof(float) * 3);
    memcpy(indexes, B->indexes, B->face_count * sizeof(int) * 3);
    memcpy(texcoords, B->texcoords, B->vert_count * sizeof(float) * 2);
    for(int i=0; i < 50; i++)
      ranges_array[i] = B->ranges_array[i];
    n_ranges = B->n_ranges;

    InitVBO();
    ranges = B->ranges;
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

    ei.coord = C0;

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
            zmax = vertices[i*3+2];

        if(vertices[i*3] < xmin)
            xmin = vertices[i*3];
        if(vertices[i*3+1] < ymin)
            ymin = vertices[i*3+1];
        if(vertices[i*3+2] < zmin)
            zmin = vertices[i*3+2];

    }

    //prn("C0=%f,%f,%f for %s", C0.x, C0.y, C0.z, nodetm.NODE_NAME);

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


void GemObject::Render(QOpenGLShaderProgram& program)
{
    t_flags& flags = GetGFlags();
    if(isCanRender == false) return;

//-----------------------------------------------------------------------------

    program.setUniformValue("model_matrix", model_matrix);


//------------------------------------------------------------------------------
    if(isSelected==2)
    {
        Pnt vbox[9];
        //gizmo

        vbox[1] = Pnt(C0.x - bounds.x/2, C0.y + bounds.y/2, C0.z - bounds.z/2);
        vbox[2] = Pnt(C0.x - bounds.x/2, C0.y + bounds.y/2, C0.z + bounds.z/2);
        vbox[3] = Pnt(C0.x + bounds.x/2, C0.y + bounds.y/2, C0.z + bounds.z/2);
        vbox[4] = Pnt(C0.x + bounds.x/2, C0.y + bounds.y/2, C0.z - bounds.z/2);
        vbox[5] = Pnt(C0.x - bounds.x/2, C0.y - bounds.y/2, C0.z - bounds.z/2);
        vbox[6] = Pnt(C0.x - bounds.x/2, C0.y - bounds.y/2, C0.z + bounds.z/2);
        vbox[7] = Pnt(C0.x + bounds.x/2, C0.y - bounds.y/2, C0.z - bounds.z/2);
        vbox[8] = Pnt(C0.x + bounds.x/2, C0.y - bounds.y/2, C0.z + bounds.z/2);
        //C0.y-=bounds.y/2;

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

    //pass ranges array
    program.setUniformValue("n_ranges", n_ranges);
    program.setUniformValueArray("ranges", ranges_array, 50);

    if(isSelected == 2)
        program.setUniformValue("COLOR", QVector4D(1,0,0,1));
    else
        program.setUniformValue("COLOR", QVector4D(ei.wire_color[0],ei.wire_color[1],ei.wire_color[2],ei.wire_color[3]));


   /* if( renderMode == 0 )
    {
        for(int i=0; i<(int)ranges.size(); i++)
        {

            if(flags.bColorRendering)
            {
                offset = ranges[i].start;
                glColor4f(ar[i].col[0],ar[i].col[1],ar[i].col[2],1);
                program.setUniformValue("COLOR", QVector4D(ar[i].col[0],ar[i].col[1],ar[i].col[2],1));
                glDrawElements( GL_TRIANGLES, (ranges[i].end - ranges[i].start), GL_UNSIGNED_INT, (const void *)offset);
                //glDrawElements( GL_TRIANGLES, (ranges[i].end - ranges[i].start)*3, GL_UNSIGNED_INT, indexes+ranges[i].start*3);

            }
        }

    }
   else
   */
   {
        if( material )
            material->Apply(program, *this);

        if(flags.bColorRendering)
            glDrawElements( GL_TRIANGLES, inds.size(), GL_UNSIGNED_SHORT, 0);




        if(flags.bWire)
        {
            //program.setUniformValue("COLOR", QVector4D(1,1,1,1));
            //glDrawElements( GL_LINES, face_count, GL_UNSIGNED_INT, 0);
        }


    }






    return;
}

void GemObject::InitVBO()
{
    // Generate 2 VBOs
    glGenBuffers(2, vboIds);

    vertices2.clear();
    for(int i=0; i < vert_count; i++)
    {
        VertexData data;
        data.normal = QVector3D(normals[3*i],normals[3*i+1],normals[3*i+2]);
        data.texCoord = QVector2D(texcoords[2*i],texcoords[2*i+1]);
        data.position = QVector3D(vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
        vertices2.push_back(data);
    }

    if( vertices2.size() > 0 )
    {
        // Transfer vertex data to VBO 0
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(VertexData), &vertices2[0], GL_STATIC_DRAW );

        inds.clear();
        for(int i=0; i<face_count*3; i++)
        {
            inds.push_back(indexes[i]);
        }
        // Transfer index data to VBO 1
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(GLushort), &inds[0], GL_STATIC_DRAW  );

    }
}


void GemObject::LoadFIO(FILE* f,Object* group)
{
    int size=0;
    char gstr[2048];
    myGroup = group;
    fread(&size, sizeof(int), 1, f);
    fread(nodetm.NODE_NAME, 1, size, f);
    nodetm.NODE_NAME[size]=0;
    NODE_NAME = nodetm.NODE_NAME;
    fread(&rot,  sizeof(rot), 1, f);
    fread(&rot0, sizeof(rot), 1, f);
    fread(&C0, sizeof(rot), 1, f);
    fread(&bounds, sizeof(bounds), 1, f);
    fread(&vert_count, sizeof(int), 1, f);
    fread(&face_count, sizeof(int), 1, f);
    vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
    normals = (float*)malloc((vert_count+1)*3*sizeof(float));
    indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));
    texcoords = (float*)malloc((face_count+1)*9*sizeof(float));
    mat_id_array = (int*)malloc((face_count+1)*sizeof(int));
    fread(vertices, sizeof(float)*vert_count*3, 1, f);
    fread(indexes, sizeof(unsigned int)*face_count*3, 1, f);
    fread(texcoords, sizeof(float)*vert_count*2, 1, f);
    fread(normals, sizeof(float)*vert_count*3, 1, f);

    fread(&size, sizeof(int), 1, f);
    t_range r;
    for(int i=0; i<size;i++)
    {
        fread(&r, sizeof(t_range), 1, f);
        ranges.push_back(r);
    }

    fread(&size, sizeof(int), 1, f);   
    std::vector<char> buff(size+1);
    fread(&buff[0], 1, size, f); ei.material = string(buff.begin(), buff.end());
    //ei.texture_name = StrRep(ei.texture_name,"E:","C:");
    //prn(ei.texture_name.c_str());

    //чтение face id map
    map<int,pair<int,int> > ranges_map;
    fread(&size, sizeof(int), 1, f);
    fread(mat_id_array, sizeof(int)*face_count, 1, f);

    //qDebug() << "mat_id_array";
    for(int i=0; i < face_count; i++)
    {
        //qDebug() << mat_id_array[i];
        if( ranges_map.find(mat_id_array[i])!= ranges_map.end())
        {
            ranges_map[mat_id_array[i]] = make_pair(ranges_map[mat_id_array[i]].first,i);
        }else
        {
            ranges_map[mat_id_array[i]] = make_pair(i,0);
        }
    }

    map<int,pair<int,int> >::iterator i;
    int j=0;
    for(i=ranges_map.begin(); i!=ranges_map.end(); i++)
    {
        ranges_array[j]=QVector2D((float)((*i).second.first), (float)((*i).second.second));
        j++;
    }
    n_ranges = j;


    CMaterial* mater = (CMaterial*)MREG_FindObj("matman");
    material = mater->parse(ei.material);

    //FillMesh();
    //ComputeC0();
    //CenterModel();

    if( vert_count )
      InitVBO();

        //Get4VertFaces();
    NODE_NAME = StrRep(NODE_NAME,"\"","");
    if(name!=NODE_NAME)
    {
        Rename(NODE_NAME);
        SetName(NODE_NAME);
    }
}


//вычисление центра тяжести и коррекция под мир app5
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
