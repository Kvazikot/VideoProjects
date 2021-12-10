/**********************************************************************
 *  Project App6
 *
 *  glcode.cpp
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

#include <QColor>
#include "../include/glcode.h"
#include "../include/matrix.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

//#include <gl/glew.h>
#include <math.h>

//-8------------------------------------------------------------------------------------
GLfloat light_specular[] = {1.0, 1.0, 1.0, 0.0};  /* Red diffuse light. */
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 0.0};  /* Red diffuse light. */
GLfloat light_position[] = {-70.0, 1.0, 1.0, 1.0};  /* Infinite light location. */
GLfloat light_position2[] = {70.0, 1.0, 1.0, 1.0};  /* Infinite light location. */
GLfloat light_position3[] = {0.0, 70.0, 1.0, 1.0};  /* Infinite light location. */
GLfloat light_position4[] = {0.0, 0.0, -70.0, 1.0};  /* Infinite light location. */
GLfloat light_position5[] = {0.0, 0.0, 70.0, 1.0};  /* Infinite light location. */
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
    {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
    };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
{0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
{4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */
GLfloat yellow [] = { 0.5, 0.5, 0.0, 1.0 };
GLfloat green [] = { 0.0, 0.5, 0.0, 1.0 };
GLfloat blue [] = { 0.0, 0.0, 1.0, 1.0 };
char gstr2[1024];


void init(void)
{
    static bool onetime = 1;

    /* Enable a single OpenGL light. */
    glLightModelfv(GL_AMBIENT_AND_DIFFUSE, light_diffuse);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position3);
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT3, GL_POSITION, light_position4);
    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT4, GL_POSITION, light_position5);
    glEnable(GL_LIGHT4);


    glEnable(GL_LIGHTING);

    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);

    //glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth (1.0);


}

void	drawGrid(int square,int step, int ylevel)
{
    //
    int i,j;
    GLfloat red[4];
    red[0]=1;red[1]=0;red[2]=0;red[3]=1;

    //ylevel=0;
    glPointSize(3);
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    glVertex3f(0,0,0);
    glEnd();

    for (int i=-square;i<square;i+=step)
    {

        glBegin(GL_LINES);
        for (int j=-square;j<square;j+=step)
        {
            glVertex3d(i,ylevel,j);
            glVertex3d(i+step,ylevel,j);
            glVertex3d(i+step,ylevel,j+step);
            glVertex3d(i,ylevel,j+step);
            glVertex3d(i,ylevel,j);

        }
        glEnd();
    }

        glLineWidth(5);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
        glBegin(GL_LINES);
        i = 0;
        for (int j=-square;j<square;j+=step)
        {
            glVertex3d(i,ylevel,j);
            //glVertex3d(i+step,ylevel,j);
            //glVertex3d(i+step,ylevel,j+step);
            glVertex3d(i,ylevel,j+step);
            //glVertex3d(i,ylevel,j);
        }

        glEnd();

        glBegin(GL_LINES);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
        j=0;
        for (i=-square;i<square;i+=step)
        {
            glVertex3d(i,ylevel,j);
            glVertex3d(i+step,ylevel,j);
            //glVertex3d(i+step,ylevel,j+step);
            // glVertex3d(i,ylevel,j+step);
            //glVertex3d(i,ylevel,j);
        }

        glEnd();
        glLineWidth(1);

}


void drawBox(void)
{
    int i;
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, front_color);
    for (i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}

#ifndef _WIN32
uint RGB(int r, int g, int b)
{
    QColor c(r,g,b);
    return c.value();
}
#endif

void RGB2GL2(uint CRGB, GLfloat* col)
{
    col[0] = (float)(CRGB & 0xFF) / 255;
    col[1] = (float)((CRGB & 0xFF00)>>8) / 255;
    col[2] = (float)((CRGB & 0xFF0000)>>16) / 255;
    col[3] = 1;
}


void set_col(uint rgb)
{
    GLfloat cl[4];
    glEnable(GL_COLOR_MATERIAL);
    RGB2GL2(rgb, cl);
    glColor3f(cl[0],cl[1],cl[2]);
}

void unset_col()
{
    glDisable(GL_COLOR_MATERIAL);
}


/*void drawStrelka( float x, float y, float len, float w, float percent, float alfa, uint rgb)
{
    int i;
    float L, sw;
    Pnt BB,S1,S2;
    static Pnt st[5];


    //alfa = -alfa;

    L=len; sw=5;

    st[0] = Pnt(0,0,0);
    st[1] = Pnt(L,0,0);

    BB = line_eq(Pnt(0,0,0), Pnt(L,0,0), percent);

    //st[2] = BB;
    S1=BB; S1.z=S1.z-w*0.5;
    S2=BB; S2.z=S2.z+w*0.5;
    st[2] = S1;
    st[3] = S2;

    //pts2con("st", st,  4);
    //arotateM(st, 5, alfa);
   // atranslateM(st, 5, x, y);
    glPushMatrix();
    glTranslatef( x, 0, y);
    glRotatef( alfa, 0, 1, 0);

    set_col(rgb);

    glBegin(GL_LINES);
    for(i=0; i<4; i++)
    {
        glVertex3f(st[i]->x, 0, st[i]->z);
    }
    glVertex3f(st[1]->x, 0, st[1]->z);
    glVertex3f(st[2]->x, 0, st[2]->z);
    glVertex3f(st[1]->x, 0, st[1]->z);
    glVertex3f(st[3]->x, 0, st[3]->z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(st[1]->x, 0, st[1]->z);
    glVertex3f(st[2]->x, 0, st[2]->z);
    glVertex3f(st[3]->x, 0, st[3]->z);
    glEnd();


    glPopMatrix();

    unset_col();

}
*/
//----------------------------------------------------------------------------------
// Отображает направление стрелкой
//  Координаты 1 конца
//  Длинна
// Ширина линии
// Процент деления отрезка
// Угол поворота
//----------------------------------------------------------------------------------
void drawStrelka( float x, float y, float len, float w, float percent, float alfa, uint rgb)
{
    int i;
    float L, sw;
    Pnt BB,S1,S2;
    static Pnt st[5];


    //alfa = -alfa;

    L=len; sw=5;

    st[0] = Pnt(0,0,0);
    st[1] = Pnt(L,0,0);

    BB = line_eq(Pnt(0,0,0), Pnt(L,0,0), percent);

    //st[2] = BB;
    S1=BB; S1.z=S1.z-w*0.5;
    S2=BB; S2.z=S2.z+w*0.5;
    st[2] = S1;
    st[3] = S2;

    //pts2con("st", st,  4);
    //arotateM(st, 5, alfa);
   // atranslateM(st, 5, x, y);
    glPushMatrix();
    glTranslatef( x, 0, y);
    glRotatef( alfa, 0, 1, 0);

    set_col(rgb);

    glBegin(GL_LINES);
    for(i=0; i<4; i++)
    {
        glVertex3f(st[i]->x, 0, st[i]->z);
    }
    glVertex3f(st[1]->x, 0, st[1]->z);
    glVertex3f(st[2]->x, 0, st[2]->z);
    glVertex3f(st[1]->x, 0, st[1]->z);
    glVertex3f(st[3]->x, 0, st[3]->z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(st[1]->x, 0, st[1]->z);
    glVertex3f(st[2]->x, 0, st[2]->z);
    glVertex3f(st[3]->x, 0, st[3]->z);
    glEnd();


    glPopMatrix();

    unset_col();

}


//template<typename T>

inline void draw_line(Pnt& p1, Pnt& p2)
{
    glBegin(GL_LINES);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glEnd();

}


void create_pyr(Pnt* p, float R, float H)
{
    static double mat[3][3];
    p[0] = Pnt(R,H,0);
    mroty(GR*45, mat);
    mvmult(3, mat,(float*)&p[0],(float*)&p[1]); // 120
    p[0] = p[1];
    mroty(GR*90, mat);
    mvmult(3, mat,(float*)&p[0],(float*)&p[1]); // 120
    mvmult(3, mat,(float*)&p[1],(float*)&p[2]); // 120
    mvmult(3, mat,(float*)&p[2],(float*)&p[3]); // 120
    mvmult(3, mat,(float*)&p[3],(float*)&p[0]); // 120
    p[4] = Pnt(0,0,0);

}



void draw_v( tvect v, float percent, uint rgb)
{
    Pnt BB;
    //EulerAngles ea,ea2;
    //HMatrix hmat;
    //Quat q,q2;
    tvect vn;


    BB = line_eq(v.A, v.B, percent);

    float len =  5;
    vn = v;
    translate_vector(&vn);
    double a = vector_anglX(vn);
    drawStrelka(v.A.x, v.A.z, len, 1.4f, 0.8f, a/GR, rgb);
    return;

}

void draw_p(QOpenGLShaderProgram& program, string name, Pnt p, uint rgb, bool bBounds)
{
    GLfloat cl[4];
    RGB2GL2(rgb, cl);
    draw_p_gl(program, name, p, cl, bBounds, 1);
}

void draw_pts(QOpenGLShaderProgram& program, vector<Pnt>& pts, string namepref, uint rgb, bool bBounds)
{
    for(int i=0; i<(int)pts.size(); i++)
    {
        sprintf(gstr2,"%s%d",namepref.c_str(), i);
        draw_p(program, gstr2,pts[i],rgb,bBounds);
    }

}


void draw_p_gl(QOpenGLShaderProgram& program, string name, Pnt p, GLfloat cl[4], bool bBounds, float R)
{
    Pnt p1[9];
    //glMatrixMode(GL_MODELVIEW);

    //glMaterialfv(GL_FRONT, GL_DIFFUSE, cl);
    program.setUniformValue("COLOR", QVector4D(cl[0],cl[1],cl[2],cl[3]));
    program.setUniformValue("RenderMode", 0);

    glPushMatrix();
    //glLoadIdentity();
    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();
    model_matrix.translate( p.x, p.y, p.z );
    program.setUniformValue("model_matrix", model_matrix);

    //glScalef(scale.x,scale.y,scale.z);
    p1[1] = Pnt(-(R / 2.), (R / 2.), (-R / 2.));
    p1[2] = Pnt(-(R / 2.), -(R / 2.), (-R / 2.));
    p1[3] = Pnt((R / 2.), -(R / 2.), (-R / 2.));
    p1[4] = Pnt((R / 2.), (R / 2.), (-R / 2.));
    p1[5] = Pnt((-R / 2.), (R / 2.), (R / 2.));
    p1[6] = Pnt(-(R / 2.), -(R / 2.), (R / 2.));
    p1[7] = Pnt((R / 2.), -(R / 2.), (R / 2.));
    p1[8] = Pnt((R / 2.), (R / 2.), (R / 2.));

//	static float cl=0;

    {

        glPointSize(5);

        glBegin(GL_POINTS);
        glVertex3f(0, 0, 0);
        glEnd();

       // renderBitmapString(0,0,0,cl[0],cl[1],cl[2],GLUT_BITMAP_9_BY_15,(char*)name.c_str());
        char *c;
        glRasterPos3f(0,0,0);
        for (c=(char*)name.c_str(); *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
        }

        if(bBounds)
        {
            glColor4f(cl[0],cl[1],cl[2],cl[3]);
        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[2].x,p1[2].y,p1[2].z);
        glVertex3f(p1[1].x,p1[1].y,p1[1].z);
        glVertex3f(p1[5].x,p1[5].y,p1[5].z);
        glVertex3f(p1[6].x,p1[6].y,p1[6].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[2].x,p1[2].y,p1[2].z);
        glVertex3f(p1[6].x,p1[6].y,p1[6].z);
        glVertex3f(p1[7].x,p1[7].y,p1[7].z);
        glVertex3f(p1[3].x,p1[3].y,p1[3].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[7].x,p1[7].y,p1[7].z);
        glVertex3f(p1[8].x,p1[8].y,p1[8].z);
        glVertex3f(p1[5].x,p1[5].y,p1[5].z);
        glVertex3f(p1[6].x,p1[6].y,p1[6].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[4].x,p1[4].y,p1[4].z);
        glVertex3f(p1[1].x,p1[1].y,p1[1].z);
        glVertex3f(p1[2].x,p1[2].y,p1[2].z);
        glVertex3f(p1[3].x,p1[3].y,p1[3].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[1].x,p1[1].y,p1[1].z);
        glVertex3f(p1[5].x,p1[5].y,p1[5].z);
        glVertex3f(p1[8].x,p1[8].y,p1[8].z);
        glVertex3f(p1[4].x,p1[4].y,p1[4].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[8].x,p1[8].y,p1[8].z);
        glVertex3f(p1[7].x,p1[7].y,p1[7].z);
        glVertex3f(p1[3].x,p1[3].y,p1[3].z);
        glVertex3f(p1[4].x,p1[4].y,p1[4].z);
        glEnd();
        }

        glPointSize(1);



    }



    glPopMatrix();

}

void renderBitmapString(float x,float y,float z,
                        float R, float G, float B,
                        void *font,	char *string)
//void renderBitmapString(float x, float y, void *font, char *string)
{
    glColor3f(R/255,G/255,B/255);
    char *c;
    glRasterPos3f(x,y,z);
    for (c=string; *c != '\0'; c++)
    {
    glutBitmapCharacter(font, *c);
    }
}

void draw_navp(string name, Pnt& p, uint rgb, bool bBounds, float R)
{
    Pnt p1[9];
    GLfloat cl[4];
    RGB2GL2(rgb, cl);
    glPushMatrix();
    glTranslatef ( p.x, p.y, p.z );
    p1[1] = Pnt(-(R / 2.), (R / 2.), (-R / 2.));
    p1[2] = Pnt(-(R / 2.), -(R / 2.), (-R / 2.));
    p1[3] = Pnt((R / 2.), -(R / 2.), (-R / 2.));
    p1[4] = Pnt((R / 2.), (R / 2.), (-R / 2.));
    p1[5] = Pnt((-R / 2.), (R / 2.), (R / 2.));
    p1[6] = Pnt(-(R / 2.), -(R / 2.), (R / 2.));
    p1[7] = Pnt((R / 2.), -(R / 2.), (R / 2.));
    p1[8] = Pnt((R / 2.), (R / 2.), (R / 2.));
    {

        set_col(rgb);
        glPointSize(5);

        glBegin(GL_POINTS);
        glVertex3f(0, 0, 0);
        glEnd();

        renderBitmapString(0,0.9f,0,cl[0],cl[1],cl[2],GLUT_BITMAP_9_BY_15,(char*)name.c_str());

        if(bBounds)
        {
        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[2].x,p1[2].y,p1[2].z);
        glVertex3f(p1[1].x,p1[1].y,p1[1].z);
        glVertex3f(p1[5].x,p1[5].y,p1[5].z);
        glVertex3f(p1[6].x,p1[6].y,p1[6].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[2].x,p1[2].y,p1[2].z);
        glVertex3f(p1[6].x,p1[6].y,p1[6].z);
        glVertex3f(p1[7].x,p1[7].y,p1[7].z);
        glVertex3f(p1[3].x,p1[3].y,p1[3].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[7].x,p1[7].y,p1[7].z);
        glVertex3f(p1[8].x,p1[8].y,p1[8].z);
        glVertex3f(p1[5].x,p1[5].y,p1[5].z);
        glVertex3f(p1[6].x,p1[6].y,p1[6].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[4].x,p1[4].y,p1[4].z);
        glVertex3f(p1[1].x,p1[1].y,p1[1].z);
        glVertex3f(p1[2].x,p1[2].y,p1[2].z);
        glVertex3f(p1[3].x,p1[3].y,p1[3].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[1].x,p1[1].y,p1[1].z);
        glVertex3f(p1[5].x,p1[5].y,p1[5].z);
        glVertex3f(p1[8].x,p1[8].y,p1[8].z);
        glVertex3f(p1[4].x,p1[4].y,p1[4].z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1[8].x,p1[8].y,p1[8].z);
        glVertex3f(p1[7].x,p1[7].y,p1[7].z);
        glVertex3f(p1[3].x,p1[3].y,p1[3].z);
        glVertex3f(p1[4].x,p1[4].y,p1[4].z);
        glEnd();
        }

        glPointSize(1);



    }

    glPopMatrix();
    unset_col();

}


void glutPrint2d(float x, float y, void* font, string text, float r, float g, float b, float a)
{

    if(!text.size()) return;

    bool blending = false;

    if(glIsEnabled(GL_BLEND)) blending = true;
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glColor4f(r,g,b,a);
    glRasterPos3f(x,y,0);

    for(int i=0; i<(int)text.size(); i++)
        glutBitmapCharacter(font, text[i]);

    if(!blending) glDisable(GL_BLEND);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

}

void set_sel_mode()
{
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    GLfloat def_col[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat def_col2[4] = {1.0, 1.0, 1.0, 1.0};
    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);
    glDisable (GL_LINE_SMOOTH);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, def_col);
    glLightfv(GL_LIGHT0, GL_SPECULAR, def_col2);
    glLightfv(GL_LIGHT0, GL_AMBIENT, def_col2);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, def_col2);
}

void unset_sel_mode()
{
}


//отрисовка стрелки курса (Эйлер)
void draw_kurs(char* label, Pnt p, Pnt Ea, char Eord, uint rgb, int len)
{
    glPushMatrix();


    glTranslatef(p.x, p.y, p.z);

    switch(Eord)
    {
        case EulOrdXYZ:
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.z, 0, 0, 1);
        break;

        case EulOrdXYX:
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.x, 1, 0, 0);
        break;

        case EulOrdXZY:
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.y, 0, 1, 0);

        break;

        case EulOrdXZX:
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.x, 1, 0, 0);
        break;

        case EulOrdYZX:
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.x, 1, 0, 0);
        break;

        case EulOrdYZY:
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.y, 0, 1, 0);
        break;

        case EulOrdYXZ:
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.z, 0, 0, 1);
        break;


        case EulOrdYXY:
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.y, 0, 1, 0);
        break;

        case EulOrdZXY:
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.y, 0, 1, 0);
        break;


        case EulOrdZXZ:
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.x, 1, 0, 0);
            glRotatef( Ea.z, 0, 0, 1);
        break;

        case EulOrdZYX:
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.x, 1, 0, 0);
        break;

        case EulOrdZYZ:
            glRotatef( Ea.z, 0, 0, 1);
            glRotatef( Ea.y, 0, 1, 0);
            glRotatef( Ea.z, 0, 0, 1);

        break;





    }


    renderBitmapString(0,0.1f,0,0,0,0,GLUT_BITMAP_9_BY_15,(char*)label);

    float axsis_width = 0.1966f;
    float strelka_percent = 0.7021f;

    len*=20;
    axsis_width*=20;
    drawStrelka(0, 0, len, axsis_width, strelka_percent, 0, rgb);
    glRotatef( 90, 1, 0, 0);
    drawStrelka(0, 0, len, axsis_width, strelka_percent, 0, rgb);
    //glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


