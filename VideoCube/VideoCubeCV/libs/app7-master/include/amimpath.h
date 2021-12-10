#ifndef AMIMPATH_H
#define AMIMPATH_H

#include "maxcurve.h"
#include "object.h"
#include "curve.h"
#include "vpnt.h"

// Узловая точка полета
struct Waypoint
{
    Waypoint()
    {
        t = 0;
        We = 10;
        psi0 = 0;
    }
    Waypoint(double _t, double _We, double _psi0) { t = _t; We=_We; psi0=_psi0; }

    double t;		// время определения точки
    double We;		// скорость
    double psi0;    // истинный курс
};

struct CalcData
{
    CalcData(){};
    double We;	  // скорость
    double WWe;  // ускорение
    double WWWe;
    double a[3];  // углы
    double aa[3]; // производные углов

    double y, yy, yyy;
    double HHH,HH;
    double we,wn,wh;
    double WE,  WN,  WH;
    double WE1, WN1, WH1;
    double WE2, WN2, WH2;
    double VE,VN,VH;
    double gamma;
    Pnt      start_point;
};

class AmimPath : public MaxCurve
{
public:
    VPnt* test_pnt;
    vector<Waypoint*>  waypoints;
    AmimPath();
    AmimPath(MaxCurve* curve, float fulltime);
    AmimPath(Curve* curve, float fulltime);
    void CalcStep( double t0, double t1,  double Ttek,  double dt,  CalcData& out, Waypoint p0, Waypoint p1 );
    double fulltime, t0, phase;
    Pnt pos;
    float kurs2dest;
    void createWaypoints(double speed);
    void createWaypoints(Curve* curve);
    void createTrajectory();
    double GetAverageOrientation(int N1, int n);
    Pnt GetPosition() { return pos; }
    float GetOrientation() { return kurs2dest; }
    void SetFulltime(double ft) { fulltime = ft; if(ft <=0 ) fulltime = 1; }
    void CreateKursSpline();
    void Render(QOpenGLShaderProgram &program);
    void Reset();
    void Slot1();
};

#endif // AMIMPATH_H
