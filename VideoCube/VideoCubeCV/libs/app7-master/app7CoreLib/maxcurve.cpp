#include "../include/var_parse.h"
#include "../include/maxcurve.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/vector3d.h"
#include <limits.h>

MaxCurve::MaxCurve()
{
    step = 0.5f;
}

double MaxCurve::Length()
{
    double sum = 0;
    for(int i=1; i<(int)spl.size(); i++)
      sum+=rasst(spl[i], spl[i-1]);
    return sum;
}

void MaxCurve::Load(string& s)
{
    VarParser p;


    // координаты контрольных\узловых точек
    // курс в узловых точках
    string ss = p.GetVarFromStr("pts", s);
    p.ParsePoints3d(ss, pts);
    ss = p.GetVarFromStr("vin", s);
    p.ParsePoints3d(ss, vin);
    ss = p.GetVarFromStr("vout", s);
    p.ParsePoints3d(ss, vout);

    //циклируем сплайн
    pts.push_back(pts[0]);
    vin.push_back(vin[0]);
    vout.push_back(vout[0]);

    CalcSpline();

    isCanRender = true;

}

void MaxCurve::CalcSpline()
{
    Pnt P1, P2, P0, P3;
    vector<double> Time_domain;
    vector<double> kurs_data;

    //генерируем сегменты сплайна Безье
    double tg=0;
    for(int i=0; i<(int)pts.size()-1; i++)
    {
        P0 = pts[i];
        P3 = pts[i+1];
        P1 = vout[i];
        P2 = vin[i+1];
        int N = rasst(P0, P3) * 5;
        double dt = 1. / (double)N;
        Pnt sp;
        spl.push_back(P0);
        for(double t=0; t <= 1; t+=dt)
        {
            sp = P0 * pow((1-t),3) + P1 * 3*t*pow((1-t),2) + P2 * 3*t*t*(1-t) + P3 * pow(t,3);
            spl.push_back(sp);
        }
    }
    spl.push_back(pts[pts.size()-1]);

    vector<Pnt> spl2;
    spl.push_back(spl[0]);
    for(int i=0; i<(int)spl.size()-1; i++)
    {
        P0 = spl[i];
        P3 = spl[i+1];
        int N = rasst(P0, P3) * 5;
        double dt = 1. / (double)N;
        spl2.push_back(P0);
        for(double t=0; t < 1; t+=dt)
        {
            P1 = line_eq(P0, P3, t);
            spl2.push_back(P1);
        }
        spl2.push_back(P3);
    }
    spl = spl2;

    //    // последний сегмент
    //    P0 = pts[pts.size()-1];
    //    P3 = pts[0];
    //    P1 = vout[vout.size()-1];
    //    P2 = vin[0];
    //    Pnt sp;
    //    for(double t=0; t <= 1; t+=step)
    //    {
    //        sp = P0 * pow((1-t),3) + P1 * 3*t*pow((1-t),2) + P2 * 3*t*t*(1-t) + P3 * pow(t,3);

    //        spl.push_back(sp);
    //    }
    //    spl.push_back(P3);

}




std::tuple<int, float, Pnt>  MaxCurve::FindNearestTo(int p_near, Pnt p, int n_steps)
{
    double minD=LONG_MAX;
    if(p_near == -1)
    {
        for(int i=0; i<(int)spl.size(); i++)
        {
           if( rasst(spl[i], p) < minD )
           {
               minD = rasst(spl[i], p);
               p_near = i;
           }
        }
    }
    else
    {
        int p_end, p_start;
        if((p_near+n_steps) > spl.size() )
            p_end = spl.size();
        else
            p_end = p_near+n_steps;
        for(int i=p_near; i<p_end; i++)
        {
            if( rasst(spl[i], p) < minD )
            {
                minD = rasst(spl[i], p);
                p_near = i;
            }
        }
        p_start = p_near - n_steps;
        if( p_start < 0 )
            p_start = 0;
        for(int i=p_start; i<p_near; i++)
        {
            if( rasst(spl[i], p) < minD )
            {
                minD = rasst(spl[i], p);
                p_near = i;
            }
        }

    }
    int p_next = p_near+1;
    if( p_next > spl.size() - 1 )
        p_next = 0;

    Pnt p1 = spl[p_near];
    Pnt p2 = spl[p_next];
    CVector3 onDest = p2 - p1;
    onDest.normalize();
    float kurs2dest  = atan2(onDest.z ,onDest.x) * 180/M_PI+180;


    tuple<int, float, Pnt> ret;
    get<0>(ret) = p_near;
    get<1>(ret) = kurs2dest;
    get<2>(ret) = spl[p_near];

    return ret;
}

Pnt MaxCurve::getP(int index)
{
    if( index > spl.size() || index < 0 )
        return Pnt(0,0,0);
    return spl[index];
}

// выдает позицию точки на кривой t [0..1]
Pnt MaxCurve::getP(float t)
{
    return Pnt(0,0,0);
}


void MaxCurve::Render(QOpenGLShaderProgram& program)
{
    program.setUniformValue("RenderMode", 0 );
    program.setUniformValue("COLOR", QVector4D(1,0,0,1));
    program.setUniformValue("model_matrix", QMatrix4x4() );

    glBegin(GL_LINES);
    for(int i=1; i<(int)spl.size(); i++)
    {
        glVertex3f(spl[i-1].x, spl[i-1].y, spl[i-1].z);
        glVertex3f(spl[i].x, spl[i].y, spl[i].z);
    }
    glEnd();

    program.setUniformValue("COLOR", QVector4D(1,1,1,1));
    glBegin(GL_POINTS);
    for(int i=1; i<(int)spl.size(); i++)
    {
        glVertex3f(spl[i-1].x, spl[i-1].y, spl[i-1].z);
        glVertex3f(spl[i].x, spl[i].y, spl[i].z);
    }
    glEnd();

}
