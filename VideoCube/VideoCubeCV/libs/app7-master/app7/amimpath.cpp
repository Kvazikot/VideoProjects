#include "../include/timer.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QtGlobal>
#include "../include/cobjregistry.h"
#include "../include/amimpath.h"
#include "../include/vector3d.h"
#include "../include/vpnt.h"
#include "../include/print.h"
#include "../include/plot.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();

AmimPath::AmimPath()
{
   fulltime = 20;
   phase = (float) rand()/RAND_MAX;
   t0 = Timer::gtimer.fsec;
   test_pnt = new VPnt(1,0,0);
   test_pnt->isCanRender = true;
   MREG.AddObjNew(test_pnt);
   isCanRender = true;
}

AmimPath::AmimPath(MaxCurve* curve, float fulltime)
{
    this->fulltime = fulltime;
    phase = (float) rand()/RAND_MAX;
    t0 = Timer::gtimer.fsec;
    test_pnt = new VPnt((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
    test_pnt->SetName("name"+rand()%100);
    test_pnt->isCanRender = true;
    MREG.AddObjNew(test_pnt);
    pts = curve->pts;
    step = curve->step;
    vin = curve->vin;
    vout = curve->vout;
    spl = curve->spl;
    createWaypoints(10);
    createTrajectory();
    curve->spl = spl;
}

AmimPath::AmimPath(Curve* curve, float fulltime)
{
    this->fulltime = fulltime;
    phase = (float) rand()/RAND_MAX;
    t0 = Timer::gtimer.fsec;
    test_pnt = new VPnt((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
    test_pnt->SetName("name"+rand()%100);
    test_pnt->isCanRender = true;
    MREG.AddObjNew(test_pnt);
    createWaypoints(curve);
    createTrajectory();
    curve->spl = spl;
    isCanRender = true;

}

void AmimPath::createTrajectory()
{
    double dt = 1./10.;
    CalcData data;
    spl.clear();
    pos = pts[0];
    for(int i=0; i < waypoints.size()-1; i++)
    {
        Waypoint* w0 = waypoints[i];
        Waypoint* w1 = waypoints[i+1];
        double t = waypoints[i]->t;
        while( t < waypoints[i+1]->t  )
        {
            CalcStep(w0->t, w1->t, t, dt, data, *w0, *w1);
            spl.push_back(pos);
            //prn(pos.toStr().c_str());
            plot(0,0,t,pos.x);
            plot(0,1,t,pos.z);
            t+=dt;
        }
    }
}

tuple<double, double> solve_quadratic(double a, double b, double c)
{
    double d = (b * b - 4 * a * c);

    if( d < 0 )
        return make_tuple(qSNaN(),qSNaN());
    if( d == 0 )
        return make_tuple(-b*b/2*a,qSNaN());
    if( d > 0 )
        return make_tuple((-b*b+sqrt(d))/2*a,(-b*b-sqrt(d))/2*a);
}

double get_arc_len(double m, double alfa)
{
    double r;
    r = m / (2 * sin( alfa /2 ));
    return alfa * r;

}

void AmimPath::createWaypoints(Curve* curve)
{
    double s, v0, a ,dt;
    tuple<double, double> roots;

    waypoints.clear();
    waypoints.push_back(new Waypoint(curve->pts[0]->ti, curve->pts[0]->We, curve->pts[0]->kurs.x));
    for(int i=0; i < curve->pts.size()-1; i++)
    {
        VPnt* p0 = curve->pts[i];
        VPnt* p1 = curve->pts[i+1];
        s = rasst(*(Pnt*)p0, *(Pnt*)p1);
        v0 = p0->We;
        if( fabs(p1->kurs.x*GR - p0->kurs.x*GR) > 0)
        {
            double arc_len = get_arc_len(s, fabs(p1->kurs.x*GR - p0->kurs.x*GR));
            double k = arc_len / s;
            s = arc_len;
        }
        a = (p1->We*p1->We - p0->We*p0->We) / 2 *s;
        if( a > 0 )
        {
            roots = solve_quadratic(a/2, v0, -s);
            if( get<0>(roots) > 0 && !qIsNaN(get<0>(roots)) )
                dt = get<0>(roots);
            else if( get<1>(roots) > 0 && !qIsNaN(get<1>(roots)) )
                dt = get<1>(roots);
        }
        else
            dt = s / v0;
        p1->ti = p0->ti + dt ;

        pts.push_back(*(Pnt*)p0);
        waypoints.push_back(new Waypoint(p1->ti, p1->We, p1->kurs.x * GR));
    }
}

void AmimPath::createWaypoints(double speed)
{
    Pnt P0,P3,P1,P2;
    waypoints.clear();
    waypoints.push_back(new Waypoint());
    double time=0;
    for(int i=0; i<(int)pts.size()-1; i++)
    {
        P0 = pts[i];
        P3 = pts[i+1];
        P1 = vout[i];
        P2 = vin[i+1];
        int N = rasst(P0, P3) * 5;
        double dt = 1. / (double)N;
        Pnt sp, sp0;
        sp0 = P0;
        double len =0;
        for(double t=0; t <= 1; t+=dt)
        {
            sp = P0 * pow((1-t),3) + P1 * 3*t*pow((1-t),2) + P2 * 3*t*t*(1-t) + P3 * pow(t,3);
            len+=rasst(sp,sp0);
            sp0 = sp;
        }
        time+=len /speed;

        CVector3 onDest = vin[i+1] - pts[i+1];
        onDest.normalize();
        double psi = atan2(onDest.z ,onDest.x) + M_PI;
        waypoints.push_back(new Waypoint(time, speed, psi ));
        prn("Waypoint: time=%f speed=%f psi=%f", time, speed, psi);

    }

}

void AmimPath::Reset()
{
    t0 = Timer::gtimer.fsec;
}

float clamp_kursAA(float a)
{
    if(a > 360)
    {
        return a - 360;
    }
    if(a < 0)
    {
        return (a + 360);
    }

    return a;
}

double AmimPath::GetAverageOrientation(int N1, int n)
{
    if( (N1 - n/2) > 0 )
    if( spl.size() > N1 + n/2 )
    {
        double avg=0;
        for(int i=N1 - n/2; i < N1 + n/2; i++)
        {
            Pnt p1 = spl[N1 - n/2];
            Pnt p2 = spl[i+1];
            p1.y = 0;  p2.y = 0;
            CVector3 onDest = p2 - p1;
            onDest.normalize();
            avg+=( atan2(onDest.z ,onDest.x) * 180/M_PI+180);
        }
        avg = avg / n;
        return avg;
    }
    return kurs2dest;
}


// —плайнова€ аппроксимаци€ параметров движени€
void cspl(double TTek, double t1, double t0, double h1, double h0, double* hh, double* hht, double* hhtt)
{
     double DT, A, AT, ATT, B, BT, BTT, a;
     DT = t1 - t0;
     A = (t1 - TTek) * (t1 - TTek) * (2.0 * (TTek - t0) + DT);
     AT = 6.0 * TTek * TTek + 2.0 * (DT - 2.0 * t0 - 4.0 * t1) * TTek +  4.0 * t1 * t0 - 2.0 * t1 * DT + 2.0 * t1 * t1;
     ATT = 12.0 * TTek + 2.0 * (DT - 2.0 * t0 - 4.0 * t1);
     B = (TTek - t0) * (TTek - t0) * (2.0 * (t1 - TTek) + DT);
     BT = -6.0 * TTek * TTek + 2.0 * (2.0 * t1 + DT + 4.0 * t0) * TTek - (4.0 * t0 * t1 + 2.0 * t0 * DT + 2.0 * t0 * t0);
     BTT = -12.0 * TTek + 2.0 * (2.0 * t1 + DT + 4.0 * t0);
     a = DT * DT * DT;
     *hh = (h0 * A + h1 * B) / a;
     *hht = (h0 * AT + h1 * BT) / a;
     *hhtt = (h0 * ATT + h1 * BTT) / a;
}

void AmimPath::CalcStep( double t0, double t1,  double Ttek,  double dt,  CalcData& out, Waypoint p0, Waypoint p1 )
{
    double& WE1=out.WE1,&WN1=out.WN1,&WH1=out.WH1;
    double& WE=out.WE,&WN=out.WN,&WH=out.WH;
    double& We=out.We,&WWe=out.WWe;
    double& y=out.y;

    //prn("CalcStep %lf %lf %lf %d", t0, t1, Ttek, 0);
    cspl(Ttek, t1, t0, p1.We, p0.We, &out.We, &out.WWe, &out.WWWe); // скорость 1 произв 2 произв
    cspl(Ttek, t1, t0, p1.psi0, p0.psi0, &out.y, &out.yy, &out.yyy); // курс 1 произв 2 произв
    //prn("We=%f y=%f", out.We, out.y );

    out.a[0] = GR * 0; // тангаж
    out.a[1] = GR * 0; // крен
    //out.a[2]  // курс
    //plot(0, 0, Ttek, WN1);

    //линейные состовл€ющие  ускорени€
    WE1 = WWe*sin(y)*cos(out.a[1]);
    WN1 = WWe*cos(y)*cos(out.a[1]);
    WH1 = WWe*sin(out.a[0]);
    //plot(2, 0, Ttek, WE1);
    //plot(2, 1, Ttek, WN1);

    //вращательные состовл€ющие скорости
    WE = We*sin(y)*cos(out.a[1]);
    WN = We*cos(y)*cos(out.a[1]);
    WH = We*sin(out.a[0]);
    //plot(1, 0, Ttek, WE);
    //plot(1, 1, Ttek, WN);


    // -------------------- ¬џ’ќƒ -----------------------------------
    //расчет координат по “≈йлору и
    //dt = 1. / f_dat;
    kurs2dest = 90 - y/GR   ;          // yaw
    pos.x = pos.x + WE*dt + WE1*dt*dt / 2.;
    pos.y = pos.y + WH*dt + WH1*dt*dt / 2.;
    pos.z = pos.z + WN*dt + WN1*dt*dt / 2.;
    //plot(3, 0, Ttek, pos.x);
    //plot(3, 1, Ttek, pos.z);

}

void AmimPath::Slot1()
{
    double delta = Timer::gtimer.fsec - t0;

    if( delta > fulltime )
    {
        t0 = Timer::gtimer.fsec;
        return;
    }

    double t = phase + delta / fulltime;
    if( t > 1 ) t-=1;
    int N1 = int( (spl.size()-1) * t - 0.5 );
    int N2 = int( (spl.size()-1) * t + 0.5 );
    if(N2 == spl.size())
        N2 = 0;

    double t2 = (t - (double)N1/fulltime) / spl.size();
    pos = line_eq(spl[N1], spl[N2], t2);
    //for test
    *(Pnt*)test_pnt = pos;
    kurs2dest  = kurs_spline.GetF(t);//GetAverageOrientation(N1, 20);
    qDebug("delta=%f kurs2dest=%f N1=%d N2=%d spl.size=%d",delta, kurs2dest,  N1, N2, spl.size() );

}

void AmimPath::Render(QOpenGLShaderProgram &program)
{
    MaxCurve::Render(program);
}
