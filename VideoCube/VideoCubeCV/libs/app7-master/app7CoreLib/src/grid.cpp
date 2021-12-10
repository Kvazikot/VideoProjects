/**********************************************************************
 *  Project App6
 *
 *  grid.cpp
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

//====================================================================
// <OBJECT> Координатная сетка  </OBJECT>
//====================================================================

//REALISATION CPP
#include "../include/grid.h"
#include "../include/cinput.h"
#include "../include/glcode.h"

Grid::Grid()
{
    ylevel = 0;
    square = 100;
    step = 1;
    curstep = 1;
    for(int i=0; i<100; i++)
        steps[i] = step / 2;
    plane = 0;
    RGB2GL2(RGB(0,255,0),color);
    //MREG.AddObj(this);
}

void Grid::SetStepForView(int view, int step)
{
    view2step[view] = step;
}

int Grid::GetStepForView(int view )
{
    if ((view < 10) && (view >= 0) )
        return view2step[view];
    else
        return view2step[0];
}

void drawStrelka( float x, float y, float len, float w, float percent, float alfa, DWORD rgb);

void Grid::DrawGridSeg()
{
    float i,j;
    //
    GLfloat red[4];
    GLfloat blue[4];
    GLfloat green[4];
    red[0]=1;red[1]=0;red[2]=0;red[3]=1;
    blue[0]=0;blue[1]=0;blue[2]=1;blue[3]=1;
    green[0]=0;green[1]=0.5;green[2]=0;green[3]=1;
    for ( i=0;i<(square);i+=step)
    {

        glBegin(GL_LINES);
        for ( j=0;j<(square);j+=step)
        {
            glVertex3d(i,ylevel,j);
            glVertex3d(i+step,ylevel,j);
            glVertex3d(i+step,ylevel,j+step);
            glVertex3d(i,ylevel,j+step);
            glVertex3d(i,ylevel,j);

        }
        glEnd();

    }
        float square2 = (float)square ;
        glBegin(GL_LINES);
        glVertex3f(0,ylevel,0);
        glVertex3f(0,ylevel,0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(square2,ylevel,square2);
        glVertex3f(square2,ylevel,0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(square2,ylevel,square2);
        glVertex3f(0,ylevel,square2);
        glEnd();


/*	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    for ( i=-square;i<square;i+=step)
    {

        glBegin(GL_LINES);
        for ( j=-square;j<square;j+=step)
        {
            glVertex3d(i,j,0);
            glVertex3d(i+step,j,0);
            glVertex3d(i+step,j+step,0);
            glVertex3d(i,j+step,0);
            glVertex3d(i,j,0);

        }
        glEnd();
    }
*/



}

void Grid::DrawAxes(QOpenGLShaderProgram& program)
{
    float i,j;
    GLfloat red[4];
    GLfloat blue[4];
    GLfloat green[4];
    red[0]=1;red[1]=0;red[2]=0;red[3]=1;
    blue[0]=0;blue[1]=0;blue[2]=1;blue[3]=1;
    green[0]=0;green[1]=0.5;green[2]=0;green[3]=1;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
        program.setUniformValue("COLOR", QVector4D(1,0,0,1));
        glBegin(GL_LINES);
        i = 0;
        for (  j=-square;j<square;j+=step)
        {
            glVertex3d(i,j,0);
            glVertex3d(i,j+step,0);
        }

        glEnd();

        program.setUniformValue("COLOR", QVector4D(0,0,1,1));
        glBegin(GL_LINES);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
        j=0;
        for ( i=-square;i<square;i+=step)
        {
            glVertex3d(i,ylevel,j);
            glVertex3d(i+step,ylevel,j);
        }

        glEnd();


}


void Grid::Render(QOpenGLShaderProgram& program)
{

    GLfloat red[4];
    GLfloat blue[4];
    GLfloat green[4];
    red[0]=1;red[1]=0;red[2]=0;red[3]=1;
    blue[0]=0;blue[1]=0;blue[2]=1;blue[3]=1;
    green[0]=0;green[1]=0.5;green[2]=0;green[3]=1;

    glPushMatrix();

    glPointSize(3);
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    glVertex3f(0,0,0);
    glEnd();

    Pnt p = Pnt(5,0,0);
    draw_p(program, "X", p, RGB(0,0,255), 0);
    p = Pnt(0,0,5);
    draw_p(program, "Z", p, RGB(255,0,0), 0);
    p = Pnt(0,5,0);
    draw_p(program,"Y", p, RGB(0,0,0), 0);

    glLineWidth(2);
    program.setUniformValue("COLOR", QVector4D(0,0,1,1));
    drawStrelka(0.f, 0., 5., 0.5f, 0.6f, 0., (uint)RGB(0,0,255));
    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();
    model_matrix.rotate(90, 0, 0, 1);
    program.setUniformValue("model_matrix", model_matrix);
    program.setUniformValue("COLOR", QVector4D(1,0,0,1));
    //drawStrelka(0, 0, 5, 0.5f, 0.6f, -90, RGB(255,0,0));

    glPushMatrix();

    model_matrix.setToIdentity();
    model_matrix.rotate(-90, 0, 1, 0);
    program.setUniformValue("model_matrix", model_matrix);
    program.setUniformValue("COLOR", QVector4D(0,1,0,1));
    drawStrelka(0, 0, 5, 0.5f, 0.6f, -90, (uint)RGB(0,255,0));
    glPopMatrix();
    glLineWidth(1);

    model_matrix.setToIdentity();
    program.setUniformValue("model_matrix", model_matrix);
    //рисуем оси
    DrawAxes(program);

    glMaterialfv ( GL_FRONT, GL_DIFFUSE, green );
    program.setUniformValue("COLOR", QVector4D(0,1,0,1));

    glLineWidth(1);

    model_matrix.setToIdentity();
    model_matrix.translate(-square/2, ylevel, -square/2);
    program.setUniformValue("model_matrix", model_matrix);
    DrawGridSeg();

    glPopMatrix();

    //Pnt M = Pnt(iinput.vx,iinput.vy,iinput.vz);
    //N = FindNearest(M);
    //draw_p("N", N, RGB(255,0,0), 0);
    //AutoStep();

}

void Grid::AutoStep()
{

}

Pnt Grid::FindNearest(Pnt M)
{
    Pnt Res;
    int n = square / step;

    float min = 2147483647.0f;

    Pnt M2 = M;
    M2 = Pnt(fabs( M.x ), fabs( M.y ), fabs( M.z));

    for(int i = 0; i < n; i++ )
    {
        float Rm = fabs(( i*step - M2.x ));
        if( Rm < min ) { min = Rm;  Res.x = i*step;}
    }


    min = 2147483647.0f;
    for(int i = 0; i < n; i++ )
    {
        float Rm = fabs(( i*step - M2.z ));
        if( Rm < min ) { min = Rm; Res.z =  i*step;}
    }

    Res.y = ylevel;

    if(M.x<0) Res.x = -Res.x;
    if(M.z<0) Res.z = -Res.z;
    //Res = Pnt(0,0,0);
    //Res = M;
    return Res;

}

