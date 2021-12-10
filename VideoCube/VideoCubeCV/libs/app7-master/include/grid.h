#ifndef Grid_H
#define Grid_H

#include <QOpenGLShaderProgram>
#include <QtOpenGL>
#include "geometry.h"

#define DWORD unsigned long

class Grid
{
public:
    Grid();
    Grid(const Grid& grid)
    {plane = grid.plane;}
    Pnt scale;
    Pnt   N;        // nearest point
    int   plane;    // 0 - xz plane  1 -  xy plane 2 - zy plane
    int   curstep;
    float ylevel;
    float square;
    float step;
    float steps[150];
    int   view2step[10];
    GLfloat color[4];

    virtual void Render(QOpenGLShaderProgram& program);
    void map2view(int view);
    void SetStepForView(int view, int step);
    int  GetStepForView(int view);
    Pnt  FindNearest(Pnt M);
    void DrawGridSeg();
    void DrawAxes(QOpenGLShaderProgram& program);
    void AutoStep();

protected:

};

#endif 

