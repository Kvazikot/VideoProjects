
#ifndef MAXCURVE_H
#define MAXCURVE_H

#include <QOpenGLShaderProgram>
#include <tuple>
#include <vector>
#include "../include/renderobject.h"
#include "../include/geometry.h"
#include "../include/interp.h"


class MaxCurve : public RenderObject
{
public:
    MaxCurve();
    MaxCurve(const MaxCurve& B){}
    vector<Pnt> pts,vin,vout,spl;
    float step;		//шаг при сплайновой интерполяции (колво точек сплайна)
    Spline    kurs_spline;

    void CalcSpline();
    Pnt getP(float t);
    Pnt getP(int index);
    double Length();
    void Load(string& s);
    virtual void Render(QOpenGLShaderProgram& program);
    std::tuple<int, float, Pnt>  FindNearestTo(int p_near, Pnt p, int n_steps);
};

#endif // MAXCURVE_H
