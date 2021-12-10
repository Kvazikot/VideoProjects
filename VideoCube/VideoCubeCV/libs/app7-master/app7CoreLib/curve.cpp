/**********************************************************************
 *  Project App6
 *
 *  curve.cpp
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

//REALISATION CPP
#include <limits.h>
#include "../include/cinput.h"
#include "../include/curve.h"
#include "../include/cinput.h"
#include "../include/flags.h"
#include "../include/print.h"
#include "../include/random_staff.h"
#include "../include/var_parse.h"
#include "../include/geometry.h"
#include "../include/glcode.h"
#include "../include/matrix.h"
#include "../include/cobjregistry.h"
#include "../include/vector3d.h"
#include "../include/glcode.h"

static ParamScreen& vt = ParamScreen::getVtInstance();
static CObjRegistry* MREG;
static CInput& iinput = CInput::getInstance2();
extern t_flags flags;
string curve_algos = "lines ermit kochanek-bartels spiral";
string atr_sets = "”зловые_точки ‘ункции јтрибуты_—плайна";
static char* atr_sets_arr[] = {"‘ильтр NewPoint DeleteLastPoint",
						"‘ильтр NewPoint NewRandomPoint DeleteLastPoint",
						"‘ильтр R step Npts Nspl algo Closed ShowPoints ShowLines"}; 
//====================================================================
// <OBJECT>  –»¬јя  </OBJECT>
//====================================================================

Curve::Curve()
{
    name = "curve01";
    n_attr_set = 0;
    RGB2GL2(RGB(255,0,0),color);
    MREG = CObjRegistry::getInstance();
    // запрос на апдейт
    args="1,1,1";
    //gentime = gtimer->msec;    
    HT = 20;
    R = 5;
    step = 0.05f;
    algon = 1;
    Nspl = 0;
    bMark = 0;
    bClosed = 0;
    rot = Pnt(0,0,0);
    scale = Pnt(1,1,1);
    Npts = 0;
    spl.reserve(sizeof(Pnt)*1000);
    isCanRender = true;
    isHidden = 0;
    iinput.add_delegat(this);
    isGroup = true;

}

void Curve::Copy(Object* A)
{
     string nm;

     //SetName(nm);
     name = nm;
     //Object::Copy(A);
     Curve* AA = (Curve*)A;
     step = AA->step;
     bClosed = AA->bClosed;
     algon = AA->algon;
     bShowPoints= AA->bShowPoints;
     gentime = AA->gentime;
     //визуальные точки пересоздаютс€ ?
     for(unsigned int i=0; i<AA->pts.size(); i++)
     {
        Pnt p = *(Pnt*)AA->pts[i];
        args = p.toStr();
        NewPoint(0);
     }

     MakeSpline();

    //Object::Copy(A);
    //Rename(nm);
}

// 0 - coords from p
// 1-  random point
VPnt* Curve::NewPoint(int param )
{
    Pnt p;

        if(param == 0)
        if( sscanf(args.c_str(),"%f,%f,%f",&p.x,&p.y,&p.z) != 3 )
            return 0;

        Npts++;
        VPnt* p1 = new VPnt();
        if(param == 1)
        {
            p1->x = rand()%10; p1->y = rand()%10; p1->z = rand()%10;
        }
        if(param == 0)
        {
            p1->x = p.x; p1->y = p.y; p1->z = p.z;
        }

        //p1->SetAngle(rand()%180); // ”Ѕ–ј“№!!

        p1->ei.coord = *(Pnt*)p1;
        sprintf(gstr,"p%d_%s",Npts,name.c_str());
        p1->SetName(gstr);
        sprintf(gstr,"p%d",Npts);
        p1->alias = gstr;
        p1->isPointOfCurve = true;
        p1->Connect();
        prn("size of vpnt %d", sizeof(VPnt));
        p1->Declare();
        p1->isNotSaving = true;
        p1->groupname = this->name;
        MREG->AddObj(p1);        
        pts.push_back(p1);
        pts_or.push_back(Pnt(p1->x,p1->y,p1->z));
        Npts = pts.size();
        AddObj(p1);
        sprintf(gstr,"p%d",Npts);

        SET_ATTR(gstr, (Pnt*)p1, "Pnt");atrs.back().NotSaving();

        if(!isLoading)
        {
            t_flags& flags = GetGFlags();
            flags.bNeedReinitObjInspector = true;
            flags.bNeedUpdObjectList = true;
        }

        ei.coord = findcenter(pts, pts.size());
        //prn("C = %s", C.toStr().c_str());
        if(algon == 0)
            spl.push_back(*(Pnt*)p1);
        p = *(Pnt*)p1;

        if(pts.size() > 2)
        {
            VPnt* pprev = pts[pts.size()-2];
            p1->kurs = pprev->kurs;
            p1->We = pprev->We;
            p1->ti = pprev->ti;
        }

        prn("Point %s added",p.toStr().c_str());
        return p1;


}

void Curve::DelLastPoint()
{
    if(pts.size()==0)
        return;

    //pdlg->m_ObjInspector2->Clear();
    Npts--;
    VPnt* dp = pts.back();    
    string p_name = dp->alias;
    string p_full_name =dp->alias + "_" + this->name ;
    MREG->RemObj(dp);
    spl.clear();

    pts.erase(pts.end()-1);
    pts_or.erase(pts_or.end()-1);

    atrs.erase(atrs.end()-1);
    atrsmap.erase(p_name);

    //flags.bNeedReinitObjInspector = true;
    flags.bNeedUpdObjectList = true;


}

// —пираль
void Curve::MakeSpiral()
{
    // построим в декартовых координатах
    float t,x,y,z,z2;

    t = 0;
    z2 = b;

    return;
    pts2.clear();
    //spl.clear();

    while( t > z2 )
    {
        x = R * cos(t);
        y = R * sin(t);
        z = a * t;
        t-=step;
        Pnt p = Pnt(x,z,y);
        pts2.push_back(p);
        //pts.push_back(p);
        //args = p.toStr();
        //NewPoint(0);
        prn(p.toStr().c_str());
    }
;


}



// кубические сплайны по методу Ёрмитта
void Curve::MakeSpline()
{
    //http://masters.donntu.edu.ua/2003/ggeo/burik/library/spline1.htm
    Pnt P1,P2,R1,R2,P3,R3,P4;
    float t;
    int i;

    if(Npts < 2) return;




    // Kochanek_Bartelts
    if(algon == 2)
    {
        //prn("Curve::MakeSpline() Kochanek_Bartelts %d", gtimer->msec);
        spl.clear();
        //prn("tension %f bias %f continuity %f", t, b, c);
        //add dummy to end
    /*	pts.push_back(pts.back());
        pts.push_back(pts.back());
        pts.push_back(pts.back());
        pts.push_back(pts.back());
*/

        for(i=0; i<(int)pts.size()-3; i++)
        {
            P1 = *(Pnt*)pts[i];
            P2 = *(Pnt*)pts[i+1];
            P3 = *(Pnt*)pts[i+2];
            P4 = *(Pnt*)pts[i+3];
            float tt = pts[i]->kp.x; // tension pts[i]->kp.x
            float b = pts[i]->kp.y; // bias  pts[i]->kp.y
            float c = pts[i]->kp.z; // continuity	pts[i]->kp.z

            R1.x = ((1-tt)*(1+b)*(1+c)/2) * (P2.x - P1.x) + ((1-tt)*(1-b)*(1-c)/2) * (P3.x - P2.x);
            R1.z = ((1-tt)*(1+b)*(1+c)/2) * (P2.z - P1.z) + ((1-tt)*(1-b)*(1-c)/2) * (P3.z - P2.z);

            R2.x = ((1-tt)*(1+b)*(1-c)/2) * (P3.x - P2.x) + ((1-tt)*(1-b)*(1+c)/2) * (P4.x - P3.x);
            R2.z = ((1-tt)*(1+b)*(1-c)/2) * (P3.z - P2.z) + ((1-tt)*(1-b)*(1+c)/2) * (P4.z - P3.z);

            //k1 = pts[i]->k;
            //k4 = pts[i+1]->k;
            Pnt sp;
            for(t=0; t < 1; t+=step)
            {
                sp.x = P1.x * (2*t*t*t-3*t*t+1) + P2.x * (-2*t*t*t+3*t*t) + R1.x * (t*t*t-2*t*t+t) + R2.x * (t*t*t-t*t);
                sp.y = P1.y * (2*t*t*t-3*t*t+1) + P2.y * (-2*t*t*t+3*t*t) + R1.y * (t*t*t-2*t*t+t) + R2.y * (t*t*t-t*t);
                sp.z = P1.z * (2*t*t*t-3*t*t+1) + P2.z * (-2*t*t*t+3*t*t) + R1.z * (t*t*t-2*t*t+t) + R2.z * (t*t*t-t*t);
                //prn(sp.toStr().c_str());
                spl.push_back(sp);
            }
        }
    }

    // Hermit
    if ( algon == 1 )
    {
        //prn("Curve::MakeSpline() hermit %d", gtimer->msec);
        spl.clear();
        //prn("P1(%0.3f,%0.3f) P2(%0.3f,%0.3f) R1(%0.3f,%0.3f) R2(%0.3f,%0.3f)", P1.x, P1.z, P2.x, P2.z, R1.x, R1.z, R2.x, R2.z);
        //генерируем сегменты сплайна Ёрмитта
        for(i=0; i<(int)pts.size()-1; i++)
        {
            P1 = *(Pnt*)pts[i];
            P2 = *(Pnt*)pts[i+1];
        //	P3 = *(Pnt*)pts[i+2];
            R1 = pts[i]->kp;
            R2 = pts[i+1]->kp;

            //k1 = pts[i]->k;
            //k4 = pts[i+1]->k;
            Pnt sp;
            for(t=0; t < 1; t+=step)
            {
                sp.x = P1.x * (2*t*t*t-3*t*t+1) + P2.x * (-2*t*t*t+3*t*t) + R1.x * (t*t*t-2*t*t+t) + R2.x * (t*t*t-t*t);
                sp.y = P1.y * (2*t*t*t-3*t*t+1) + P2.y * (-2*t*t*t+3*t*t) + R1.y * (t*t*t-2*t*t+t) + R2.y * (t*t*t-t*t);
                sp.z = P1.z * (2*t*t*t-3*t*t+1) + P2.z * (-2*t*t*t+3*t*t) + R1.z * (t*t*t-2*t*t+t) + R2.z * (t*t*t-t*t);
                //prn(sp.toStr().c_str());
                spl.push_back(sp);
            }
        }
        if( bClosed )
        {
            P1 = *(Pnt*)pts.front();
            P2 = *(Pnt*)pts.back();
            R1 = pts.front()->kp;
            R2 = pts.back()->kp;
            //k1 = pts.front()->k;
            //k4 = pts.back()->k;
            Pnt sp;
            for(float t=0; t < 1; t+=step)
            {
                sp.x = P1.x * (2*t*t*t-3*t*t+1) + P2.x * (-2*t*t*t+3*t*t) + R1.x * (t*t*t-2*t*t+t) + R2.x * (t*t*t-t*t);
                sp.y = P1.y * (2*t*t*t-3*t*t+1) + P2.y * (-2*t*t*t+3*t*t) + R1.y * (t*t*t-2*t*t+t) + R2.y * (t*t*t-t*t);
                sp.z = P1.z * (2*t*t*t-3*t*t+1) + P2.z * (-2*t*t*t+3*t*t) + R1.z * (t*t*t-2*t*t+t) + R2.z * (t*t*t-t*t);
                //prn(sp.toStr().c_str());
                spl.push_back(sp);
            }

        }
    }
    Nspl = spl.size();
}

double FixAngle(double grad)
{
    return grad;
}

void Curve::DeleteWaypoints()
{
    while(pts.size()!=0)
        DelLastPoint();
    return;
}

// метод работает с узловыми точками
void Curve::InitWaypoints()
{
    double t_rascheta = vt.getd("t_rascheta");
    //длинна пути ~
    double S=0;

    for(uint i=1; i<pts.size(); i++ )
    {
        CVector3 v0(pts[i-1]->x,pts[i-1]->y,pts[i-1]->z);
        CVector3 v1(pts[i]->x,pts[i]->y,pts[i]->z);
        CVector3 v = v1 - v0;
        S+=v.findMagnitude();
    }

    t_rascheta = vt.getd("t_rascheta");
    double dt = t_rascheta / pts.size();

    Vsr = S / t_rascheta;
    prn("S=%f dt=%f Npts=%d Vsr=%f",S,dt,pts.size(),Vsr);



    double t=0;
    for(uint i=1; i<pts.size(); i++)
    {
        VPnt* p0 = pts[i-1];
        VPnt* p1 = pts[i];
        p0->y+= normal3(2,50);
        p1->y+= normal3(2,50);
        p0->ti = t;
        p0->We = Vsr;
        CVector3 v0(pts[i-1]->x,pts[i-1]->y,pts[i-1]->z);
        CVector3 v1(pts[i]->x,pts[i]->y,pts[i]->z);
        CVector3 v = v1 - v0;
        CVector3 Sever(SeverP);
        double angle = acos( (v | Sever) /( v.findMagnitude() * Sever.findMagnitude()));
        p0->kurs.x = angle/GR;
        prn("kurs %f",p0->kurs.x);
        t+=dt;
    }

    VPnt* p_last = pts.back();
    p_last->ti = t_rascheta;
    p_last->We = Vsr;
    int i=pts.size()-1;
    CVector3 v0(pts[i-1]->x,pts[i-1]->y,pts[i-1]->z);
    CVector3 v1(pts[i]->x,pts[i]->y,pts[i]->z);
    CVector3 v = v1 - v0;
    CVector3 Sever(SeverP);
    double angle = acos( (v | Sever) /( v.findMagnitude() * Sever.findMagnitude()));
    p_last->kurs.x =  angle/GR;


}

void Curve::Scale()
{
    prn("Curve::Scale()");
    ascale(pts, pts_or, pts.size(), scale.x, scale.y, scale.y);
}

void Curve::Rotate()
{
    prn("Curve::Rotate()");

    //Affine::arotateM(pts, pts.size(), rot);
    arotateCM(pts, pts.size(), rot);

}


void Curve::OnSelect()
{
    for(unsigned int i=0; i<pts.size(); i++)
    {
      pts[i]->isHidden = 0;
      //pts[i]->isSelected = 2;
    }
    MakeSpline();

}


void Curve::ShowAllAtrsFunc(bool b)
{
        if( !b )
        {
            bHideStdAttrs = true;
            HideAllAtrs();
            ShowAtrs("ShowAllAtrs NewPoint DeleteLastPoint");
            t_atr a;
            for( int i=0; i < (int)atrs.size(); i++ )
                if( atrs[i].name.find("p") == 0 )
                    atrs[i].Unhide();
        }
        else
        {
            bHideStdAttrs = false;
            ShowAllAtrs();

        }

}

void Curve::Save(string& s)
{


    s+="pts=[";
    for(int i=0; i<(int)pts.size();i++)
    {
        Pnt p = *(Pnt*)pts[i];
        s+=p.toStr()+" ";
    }
    s+="] ";
    s+="kp=[";
    //control points
    if(algon == 1) //ermit
        for(int i=0; i<(int)pts.size();i++)
            s+=pts[i]->kp.toStr() + " ";
    s+="] ";
    s+="kurs=[";
    for(int i=0; i<(int)pts.size();i++)
        s+=pts[i]->kurs.toStr()+" ";
    s+="] ";
    s+="ti=[";
    for(int i=0; i<(int)pts.size();i++)
        s+=DoubleToStr(pts[i]->ti) +" ";
    s+="] ";
    s+="We=[";
    for(int i=0; i<(int)pts.size();i++)
        s+=DoubleToStr(pts[i]->We) +" ";
    s+="] ";

}

void SortPoints(vector<Pnt>& in_pts, vector<Pnt>& out_pts);

void Curve::Load(string& s)
{
    VarParser p;
    vector<Pnt> pts1,kurs,kp;
    vector<double> ti,We;
    string val;

    // координаты контрольных\узловых точек
    // курс в узловых точках
    string ss = p.GetVarFromStr("pts", s);
    p.ParsePoints3d(ss, pts1);
    ss = p.GetVarFromStr("kurs", s);
    p.ParsePoints3d(ss, kurs);
    ss = p.GetVarFromStr("ti", s);
    p.ParseDoubleArray(ss, ti);
    ss = p.GetVarFromStr("We", s);
    p.ParseDoubleArray(ss, We);
    ss = p.GetVarFromStr("kp", s);
    p.ParsePoints3d(ss, kp);



    for(int i=0; i<(int)pts1.size();i++)
    {
        args = pts1[i].toStr();
        NewPoint(0);
        if( (int)kurs.size()> i)
        {
            pts.back()->kurs = kurs[i];
            pts.back()->SetMode(0,1);
            if( (int)ti.size()> i)
                pts.back()->ti = ti[i];
            if( (int)We.size()> i)
                pts.back()->We = We[i];
            if( (int)kp.size()> i)
                pts.back()->kp = kp[i];

        }
    }




    return;
}



void Curve::OnUnselect()
{
    prn("Curve::OnUnselect()");
    //for(int i=0; i<(int)pts.size(); i++)
    //  pts[i]->isHidden = 2;
    //flags.bNeedUpdObjectList = true;
}


// уравнение цепной линии R - рассто€ние между опорами,
// t - параметр [0..1] a,b,c - коэффициенты
void Curve::MakeCantenaryLine( double a, double b, double c, double R)
{
    double r;
    double t=-10,dt=0.1;
    spl.clear();
    r=0; b=0; c=0; R=0;
    while( t < 10 )
    {
        t+=dt;
        double m = a * cosh( t / a);
        //prn("%f",m);
        Pnt p;
        p.x = t;
        p.y = m;
        p.z = 0;
        spl.push_back(p);
    }
}


void Curve::Render(QOpenGLShaderProgram& program)
{
    Pnt p1[9];
    int i;
    glPushMatrix();

    if(!isCanRender)
        return;

    program.setUniformValue("RenderMode", 0 );
    program.setUniformValue("COLOR", QVector4D(color[0],color[1],color[2],color[3]));

    static float cl=0;
    if(isSelected==2)
    {
        glColor4f(0,0,1,cl);
        glPointSize(1);

    }
    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();
    program.setUniformValue("model_matrix", model_matrix);

    if( bShowPoints == 1)
    {
        glPointSize(3);
        glBegin(GL_POINTS);
        for(i=0; i<(int)spl.size(); i++)
        {
            glVertex3f(spl[i].x, spl[i].y, spl[i].z);
        }
        glEnd();
        glPointSize(1);

    }


    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);    // Use The Good Calculations
    glEnable (GL_LINE_SMOOTH);          // Enable Anti-Aliasing


    //спираль
    if( algon == 3 )
        if( pts2.size()>0 )
        {

            RGB2GL2(RGB(255,0,0),color);
            program.setUniformValue("COLOR", QVector4D(color[0],color[1],color[2],color[3]));
            //glVertex3f(spl[0].x, spl[0].y, spl[0].z);
            glBegin(GL_LINES);
            for(i=1; i<(int)pts2.size(); i++)
            {
                glVertex3f(pts2[i-1].x, pts2[i-1].y, pts2[i-1].z);
                glVertex3f(pts2[i].x, pts2[i].y, pts2[i].z);
            }
            glEnd();
        }

    //линии
    if( bShowLines == 1)
    if(pts.size()>1)
    {

        if(bClosed)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f(pts[0]->x, pts[0]->y, pts[0]->z);
            for(i=1; i<(int)pts.size(); i++)
            {
                glVertex3f(pts[i-1]->x, pts[i-1]->y, pts[i-1]->z);
                glVertex3f(pts[i]->x, pts[i]->y, pts[i]->z);
            }
            glEnd();
        }
        else
        {
            glVertex3f(pts[0]->x, pts[0]->y, pts[0]->z);
            glBegin(GL_LINES);
            for(i=1; i<(int)pts.size(); i++)
            {
                glVertex3f(pts[i-1]->x, pts[i-1]->y, pts[i-1]->z);
                glVertex3f(pts[i]->x, pts[i]->y, pts[i]->z);
            }
            glEnd();
        }



        // ќтрисовка узловых точек
        if( n_attr_set == 0 )
        {

            for(i=0; i<(int)pts.size(); i++)
            {

                char s[100];
                sprintf(s, " %d",  i);
                pts[i]->ShowName = false;
                draw_p_gl(program, s,*(Pnt*)pts[i],pts[i]->color, 1, 1);
            }

        }

        // –ежим сплайна
        if( n_attr_set == 2 )
        {
            if( algon == 1 )
            {
                //
                RGB2GL2(RGB(0,0,0),color);                
                if ( bMark ) RGB2GL2(RGB(rand()%255,rand()%255,rand()%255),color);
                program.setUniformValue("COLOR", QVector4D(color[0],color[1],color[2],color[3]));
                glBegin(GL_LINES);
                for(i=0; i<(int)pts.size(); i++)
                {
                    if(pts[i]->isSelected > 0)
                    {
                        glVertex3f(pts[i]->x, pts[i]->y, pts[i]->z);
                        glVertex3f(pts[i]->kp.x, pts[i]->kp.y, pts[i]->kp.z);
                    }
                }
                glEnd();
            }


            //сплайн
            if( algon > 0 )
            if( spl.size()>0 )
            {


                RGB2GL2(RGB(255,0,0),color);
                if ( bMark ) RGB2GL2(RGB(rand()%255,0,0),color);
                else
                {
                    if(isSelected == 2)
                    RGB2GL2(RGB(0,0,255),color);

                }

                program.setUniformValue("COLOR", QVector4D(color[0],color[1],color[2],color[3]));
                glVertex3f(spl[0].x, spl[0].y, spl[0].z);
                glBegin(GL_LINES);
                for(i=0; i<(int)spl.size(); i++)
                {
                    //glVertex3f(spl[i-1].x, spl[i-1].y, spl[i-1].z);
                    glVertex3f(spl[i].x, spl[i].y, spl[i].z);
                }
                glEnd();
            }
        }



        //glVertex3f(pts.back()->x, pts.back()->y, pts.back()->z);
        //glEnd();
    }




    glPopMatrix();

    glDisable (GL_LINE_SMOOTH);          // Enable Anti-Aliasing

}

void Curve::SelectAtrSet(int idx)
{
    bHideStdAttrs = true;
    HideAllAtrs();
    n_attr_set = idx;
    ShowAtrs(atr_sets_arr[idx]);
    //t_flags& flags = GetGFlags();
    //flags.bNeedReinitObjInspector = true;
    //flags.bNeedUpdObjectList = true;

}

extern void wppolet_init();
//object inspector event
void Curve::OnAttrChange(t_atr& a)
{
    if(isLoading) return;

//    wppolet_init();

    prn("Curve::OnAttrChange %s", a.name.c_str());


    if (a.name == "‘ильтр" )
    {
        SelectAtrSet(a.data);
        if(a.data == 0)
        {
            for( int i=0; i < (int)atrs.size(); i++ )
                if( atrs[i].name.find("p") == 0 )
                    atrs[i].Unhide();
        }

    }

    if (a.name == "algo" )
    {
        algon = a.data;
        MakeSpline();
        if( algon == 3 )
            MakeSpiral();
        prn("algo is %d", algon);
        return;
    }



    if( (a.name == "Nseg") || (a.name == "R")  || (a.name == "Npts"))
    {
        MakeSpline();
        if( algon == 3 )
            MakeSpiral();
        return;
    }


    if (a.name == "step" )
    {
        MakeSpline();
        if( algon == 3 )
            MakeSpiral();
        prn("n points in spline %d", spl.size());
        return;
    }



    if (a.name == "scale" )
    {
        Scale();
        return;
    }

    if (a.name == "rotate" )
    {
        Rotate();
        return;
    }

    if (a.name == "ei.coord" )
    {
        CVector3 O1, O2, O3;

        if(isLoading!=0) return;
        //return;

        O1 = ei.coord0;
        O2 = ei.coord;
        O3 = O2 - O1;

        prn("O3 = %f %f %f", O3.x, O3.y, O3.z);

        atranslateM(pts, pts.size(), O3.x, O3.y, O3.z);
        ei.coord0 = ei.coord;


        MakeSpline();
        return;
    }

    //==============/ функциии

    if (a.name == "NewPoint" )
    {
        NewPoint(0);
        if(pts.size()>1)
        {
            int i = pts.size()-1;
            *(pts[i]) = *(pts[i-1]);
        }

        MakeSpline();
        return;
    }

    if (a.name == "NewRandomPoint" )
    {
        NewPoint(1);
        MakeSpline();
        return;
    }

    if (a.name == "DeleteWaypoints" )
    {
        DeleteWaypoints();
        return;
    }



    if (a.name == "DeleteLastPoint" )
    {
        DelLastPoint();
        return;
    }

    if (a.name == "MakeSpline" )
    {
        MakeSpline();
        return;
    }

    if (a.name == "MakeSpiral" )
    {
        MakeSpiral();
        return;
    }


    if (a.name == "MakeDuga" )
    {
        //MakeDuga();
        return;
    }

    if (a.name == "InitWaypoints" )
    {
        InitWaypoints();
        return;
    }

    if (a.name == "OffsetX" || a.name == "OffsetZ")
    {
        prn("Move Curve");
        //MoveBy(OffsetX, 0, OffsetZ);
        //bOffsetChanged = true;
    }
}



//object inspector event
void Curve::OnAttrSelect(t_atr& a)
{
    prn("%s",a.name.c_str());
    for(int i=0; i < (int)pts.size(); i++)
        pts[i]->isSelected = 0;
    for(int i=0; i < (int)pts.size(); i++)
    {
        if(pts[i]->name.find(a.name)!=string::npos)
            pts[i]->isSelected = 1;
    }
}

//obj update slot
void Curve::Slot1()
{
    if(isLoading) return;

    if(m_Updated)
    {
        MakeSpline();
        m_Updated = false;
        //prn("%s accept new params %s", name.c_str(), bounds.toStr().c_str());
    }





}

void Curve::click_rmb(int state)
{
        if( isSelected == 0 ) return;
        if(state == 1)
        {
            isCanRender = false;
            args = Pnt(iinput.vx,HT,iinput.vz).toStr();
            NewPoint(0);
            isCanRender = true;
            iinput.bNeedBreak = true;
            //NewPoint(1);
        }

}
