/**********************************************************************
 *  Project App6
 *
 *  vehicle.cpp
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

#include "../include/stdafx.h"
#include "../include/cinput.h"
#include "../include/cobjregistry.h"
#include "../include/print.h"
#include "../include/timer.h"
#include "../include/matrix.h"
#include "../include/mterrain.h"
#include "../include/vehicle.h"
#include "../include/terrain.h"
#include "../include/glcode.h"
#include "../include/matrix.h"
#include "../include/param_screen_bcb.h"
#include "../include/move.h"
#include "../include/cmaininterface.h"
#include "../include/demper.H"
#include "../include/vector3d.h"
#include <GL/glut.h>

string kompass_dir = "n nu nd w wu wd e eu ed s su sd nw nwu nwd ne neu ned sw swu swd se seu sed";

static CMainInterface& main_if = CMainInterface::getInstance();
static ParamScreen& pscr = ParamScreen::getInstance();
//extern Timer gtimer;
CObjRegistry& MREG = CObjRegistry::getInstance2();
static CInput& iinput = CInput::getInstance2();

CVehicle* asVeh(Object* o)
{
    return (CVehicle*)o;
}

CVehicle::CVehicle()
{
    Object();
    num = 0;
    ei.coord = Pnt(0,0,0);
    kuzov = NULL;
    w0 = NULL; w1 = NULL; w2 = NULL; w3 = NULL;
    fbw = 1;
    ffw = 1;
    dir = 1;
    timer.start();
    t_rascheta = 0.01f;
    wp_delta = 0;
    porog = 10;
    speed = 0.4f; // m/c
    kurs = Pnt(0,0,0);
    trenie =0.05f;
    h_dempers = 0.1f;
    r   = 0.6f;
    t0 = Timer::gtimer.fsec;
    S = 0;
    delkoof = 3;
    max_wang = 45;
    bIsReceiveInput = true;
    wang = 0; // угол поворота колес
    //подгоночные коэффициенты для расчетов геометрии (временно)
    a=0.78;
    b=1.12;
    c=10;
    d=10;

    isCanRender = true;
    isEntety = true;
    isHidden = 0;
    isGroup = true;
    isGroupMovable = false;
    stopSignal = false;
    icurve = -1;
    w0 = new GemObject();
    w1 = new GemObject();
    w2 = new GemObject();
    w3 = new GemObject();
    kuzov = new GemObject();
    trajs.push_back((Curve*)0);
    activeTraj = 0;
	for(int i=0; i<4; i++)
	{
		dm[i] = new Demper();
		dm[i]->SetName("dm" +IntToStr(i));
		dm[i]->bShowText = false;
		dm[i]->LotusSettings();
        //dm[i]->Declare();
		dm[i]->isLoading = false;
        dm[i]->isCanRender = false;
        //MREG.AddObj(dm[i]);
		dm[i]->StartSimulation();

	}
    
    // запрос на апдейт
    Connect();
    
    //присвоить вращение
    
    
}

// на старт траектории
void CVehicle::OnStart()
{
    if ( activeTraj == 0 )
        return;

    if ( activeTraj->pts.size() == 0)
        return;
    
    t0 = Timer::gtimer.fsec ;
    S = 0;
    ei.coord = *(Pnt*)activeTraj->pts[0];

}

void CVehicle::RenderOSD()
{
	if(main_if.activeScreen != 1) return;
    /*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
    GLsizei RES_X, RES_Y;
    glViewport(0, 0, RES_X, RES_Y);
	gluOrtho2D(0.0, RES_X, 0.0, RES_Y);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


    char gstr[1024];
    //sprintf(gstr, "gtimer.t=%f сек.", Timer::gtimer.fsec);
    glutPrint2(1, RES_Y-20, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);
	//sprintf(gstr, "pitch %3.3f(%3.3f) roll %3.3f(%3.3f) yaw %3.3f", angles.pitch, rulePitch, angles.roll, ruleRoll, angles.yaw);
    glutPrint2(1, RES_Y-40, GLUT_BITMAP_9_BY_15, string(terrain_objs[0].toStr()),1,0,0,1);
	sprintf(gstr, "kuzov x=%3.3f y=%3.3f z=%3.3f", kuzov->ei.coord.x, kuzov->ei.coord.y, kuzov->ei.coord.z);
    glutPrint2(1, RES_Y-60, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);
    glutPrint2(1, RES_Y-80, GLUT_BITMAP_9_BY_15, string(prijina_m_str),1,0,0,1);
    glutPrint2(1, RES_Y-100, GLUT_BITMAP_9_BY_15, string(prijina_L_str),1,0,0,1);
    glutPrint2(1, RES_Y-120, GLUT_BITMAP_9_BY_15, string(prijina_tK_str),1,0,0,1);
*/

	
	//glutPrint2(1, RES_Y-80, GLUT_BITMAP_9_BY_15, string(wheels_pos_str),1,0,0,1);
	//glutPrint2(1, RES_Y-100, GLUT_BITMAP_9_BY_15, string(wheels_pos_str2),1,0,0,1);

	
	

	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
}


// расчет кривизны траектории
//----------------------------------------------------------
void CVehicle::Krivizna()
{
    vector<double> Skr;
    unsigned int i;
    
    //return;
    if(activeTraj==0) return;
    
    //точки траектории
    vector<Pnt>& pts = activeTraj->spl;

    //индексы поворотов 
     // 0 - начало поворота
     // 1 - конец поворота
    vector<int> pnt_i;
    pnt_i.resize(pts.size());

    Skr.resize(pts.size());


    for(i=0; i < pts.size() - 1; i++ )
    {
        Skr[i] = (pts[i]->y - pts[i+1]->y) / (pts[i]->x - pts[i+1]->x);
        prn(pts[i].toStr().c_str());
        
    }
        
    prn("-----");
    // порог кривизны
    //float porog = 12;
    for(i=0; i < pts.size(); i++ )
    {
        if( fabs(Skr[i]) < porog )
            Skr[i] = 0;
        else
          prn("%f",Skr[i]);
    }
    
    

    int  max = 0;
    float Smax = 0;

    //поиски начала поворота и конца
/*    for(i=0; i < pts.size(); i++ )
    {
        if(( Skr[i] > 0 ) && ( start == 0 ) )
            start = i;


        if( Skr[i] == 0)
        {
            end = i;
            start = 0;
        }

        pnt_i[i] = 0;

        if( start == 0 )
            pnt_i[i] = 1;
        
        if( end == 0 )
            pnt_i[i] = 2;
    }
*/
    //максимум
    for(i=0; i < pts.size(); i++ )
    {

        //if( Skr[i] > Smax )
        Skr[i] = fabs(Skr[i]);
        if( Skr[i] > Smax )
        {
            Smax = Skr[i];
            max = 1;
        }
        else
            max = 0;
        
        // максимальная кривизна траектории
        if( max == 1 )
        {
            pnt_i[i] = 3;
            //prn("Skr_max=%f", Skr[i]);
        }
    }

   

        
    prn("porog = %f", porog);
    prn("-----");


}


// 
//sector(Pnt p1, Pnt p2)

        

// расчет пути
double put_se(vector<VPnt*>& pts, int s, int e)
{
    int i=0;
    double summ=0;
    
    for( i = s; i <= e; i++ )
    {
        summ+= sqrt (sq(pts[i]->x - pts[i+1]->x) + sq(pts[i]->y - pts[i+1]->y));
    }

    return summ;
}




//----------------------------------------------------------
/*Скорость на повороте = const
1. Расчет Skr по pts
2. Заполнить массив индексов поворотов pvt_i


3. Когда прошли точку pvt0_start
   Рачет угла поворота r_alfa расчет dSalfa = put_se(pvt0_i, pvte_i)
   Расчет приращения dwang = dSalfa / V_start
*/


//выполняется присвоение траектории 
//поиск вейпоинтов
//сброс переменных
void CVehicle::AssignTrajectory()
{
    if( icurve>0 && icurve<(int)trajs.size() )
    {
        activeTraj = trajs[icurve];        
    }
    if(activeTraj!=0)
      FindWaipoints(activeTraj, 0, wp_delta, 100, waipoints);

    near_idx = -1;
    target_wp = Pnt(ei.coord.x+5,ei.coord.y,ei.coord.z);
    bReachTargetWp = 1;
}

void CVehicle::ListCurves()
{
    vector<Object*>::iterator i;
    curvesnames = "Not_Assigned ";
    for(i = MREG.allObj.begin(); i!=MREG.allObj.end(); i++)
    {
        if((*i)->classname == "Curve")
        {
            curvesnames+=(*i)->name + " ";
            trajs.push_back((Curve*)(*i));
        }
    }
    //icurve = 1;

}

void CVehicle::LoadDefModels()
{
    LoadModels("kuzov", "w0");
}

int CVehicle::LoadModels(string kuzname, string wn)
{
    int ok = 0;

    kuzov->LoadMesh(kuzname);
    if(kuzov == NULL)
        ei.model = kuzname; 
    kuzov->groupname = name;
    kuzov->renderMode = 0;
    kuzov->isCanRender = true;

    //load mesh
    w0->CopyMesh("w2");
	w0->CenterModel();
    w0->update();
    w0->renderMode = 0;
    w0->groupname = name;
    w1->CopyMesh("w2");
	w1->CenterModel();
    w1->update();
    w1->renderMode = 0;
    w1->groupname = name;
    w2->CopyMesh("w2");
	w2->CenterModel();
    w2->update();
    w2->renderMode = 0;
    w2->groupname = name;
    w3->CopyMesh("w2");
    w3->renderMode = 0;
	w3->CenterModel();
    w3->update();
    w3->groupname = name;


    //add to group
    AddObj(kuzov); AddObj(w0); AddObj(w1); AddObj(w2); AddObj(w3);
    ListCurves();

	//add as terrain objects
	TerrainObject o;	
	o.mesh = w0; o.near_idx = -1; o.H1 = 0; terrain_objs.push_back(o);
	o.mesh = w1; o.near_idx = -1; o.H1 = 0; terrain_objs.push_back(o);
	o.mesh = w2; o.near_idx = -1; o.H1 = 0; terrain_objs.push_back(o);
	o.mesh = w3; o.near_idx = -1; o.H1 = 0; terrain_objs.push_back(o);
	o.mesh = kuzov; o.near_idx = -1; o.H1 = 0; terrain_objs.push_back(o);


    //добавить визуальную точку (для прикидки)
    vp = new VPnt();
    vp->x = rand()%10; vp->y = rand()%10; vp->z = rand()%10;
    vp->SetName("vp1");
    vp->alias = vp->name;
    //vp->R = 20;
    //vp->Declare();
    MREG.AddObjNew(vp);

    vp2 = new VPnt();
    vp2->x = rand()%10; vp->y = rand()%10; vp->z = rand()%10;
    vp2->SetName("vp2");
    vp2->alias = vp2->name;
    //vp2->Declare();
    MREG.AddObjNew(vp2);


    //icurve = 1;
    //AssignTrajectory();

    return ok;
}

float mc2kmh(float mc){ return (float)mc * 3600 / 1000; }

float kmh2mc(float kmh) { return (float)kmh * 1000 /3600; }

void CVehicle::FindNearPnt(Curve* traj, Pnt& p, int& idx)
{
    if ( traj == 0 )
        return;

    if ( traj->pts.size() == 0)
        return;
    
    //ei.coord = *(Pnt*)activeTraj->pts[0];
    float min = (float)INT_MAX;
    float rr;
    map<int,int> mp;
    for(unsigned int i=0; i < traj->spl.size(); i++)
    {
        rr = rasst(ei.coord, traj->spl[i]);
        if( rr < min)
        {
            min = rr;
            mp[min] = i;
        }
    }
    //updf("rasst",min);
    idx = mp[min];
    p = traj->spl[mp[min]];
    //prn("min %f %d", r, mp[min] );

}

void CVehicle::FindNearWayPoint(vector<Pnt>& waipoints, Pnt& p, int& idx)
{
    float min = (float)INT_MAX;
    float rs;
    map<int,int> mp;
    idx = -1;
    if(waipoints.size()==0) return;
    for(unsigned int i=0; i < waipoints.size(); i++)
    {
        rs = rasst(p, waipoints[i]);
        if( rs < min)
        {
            min = rs;
            mp[min] = i;
        }
    }
    //updf("rasst",min);
    idx = mp[min];
    //p =waipoints[mp[min]];
    //prn("min %f %d", r, mp[min] );

}

// точка вперед по траектории на заданную дистанцию
int CVehicle::FindForward(Curve* traj, int pi_from, Pnt& p_to, float dist)
{
    Pnt p_from = traj->spl[pi_from];
    for(unsigned int i=pi_from; i < traj->spl.size(); i++)
    {
        p_to = traj->spl[i];
        float rr = rasst(p_from,p_to);        
        if(rr > dist)
        {
            return i;
        }
            //break;        
            
    }
    return 0;
}

//разбить траекторию на вейпоинты
// указывается расстояние между вейпоинтами на траектории и максимальное расстояние просмотра
// сохраняется в waipoints
int CVehicle::FindWaipoints(Curve* traj, int pi_from, float d, float maxdist, vector<Pnt>& waipoints)
{
    float rr,r2;
    Pnt p_to;
    Pnt p_from = traj->spl[pi_from];
    Pnt p_from0 = p_from;
    
    waipoints.clear();
    for(unsigned int i=pi_from; i < traj->spl.size(); i++)
    {
        p_to = traj->spl[i];
        rr = rasst(p_from, p_to);        
        if(rr > d)
        {
            waipoints.push_back(p_to);
            p_from = p_to;
        }
        r2 = rasst(p_from0, p_to);
        if(r2 > maxdist)
            break;
            
    }

    return 0;
}

// точка назад по траектории на заданную дистанцию
int CVehicle::FindBackward(Curve* traj, int pi_from, Pnt& p_to, float dist)
{
    Pnt p_from = traj->spl[pi_from];
    for(int i=pi_from; i >= 0; i--)
    {
        p_to = traj->spl[i];
        float rr = rasst(p_from,p_to);        
        if(rr > dist)
        {
            return i;
        }
    }
    return 0;
}

//выставляет флаг bReachTargetWp если достигли точки
bool CVehicle::CheckReachTargetWp()
{
    float rr = rasst(ei.coord, target_wp);
    if(rr < 2 )
      bReachTargetWp = true;
    else
         bReachTargetWp = false;
    return bReachTargetWp;
    
}

void CVehicle::OnReceiveInput()
{
	prn("input!");
}

//применяем управление
void CVehicle::InputControl()
{
    float ac = 0.33f;
    
	//если нет управления - выход
	if(iinput.bKeyPressed == false) return;

    kurs = kurs0;
    speed = v.x;
    //iinput.keystates[]; //влево
    if( iinput.keystates[0x74] ) // газ
    {
        //mu_printf("CVehicle::Slot1 gaz");
        if(speed<0)
        {
            speed+=2*ac;
        }
        else speed+=ac;
        
    }
    if( iinput.keystates[0x66] ) // влево
    {
        //mu_printf("left");        
        //kurs.x-=5;
        if(wang<max_wang)        
            wang+=2;
    }
    if( iinput.keystates[0x68] ) // вправо
    {
        //mu_printf("right");
        //kurs.x+=5;
        if(wang>-max_wang)
            wang-=2;
    }
    if( iinput.keystates[0x67] ) // тормоз
    {
        //mu_printf("CVehicle::Slot1 brake");
        stopSignal = true;
        if(speed == 0)
        {
            speed-=ac;                
        }
        else
        {
            if(speed >0)
            {
                speed-=ac*2;
                if(speed < 0)
                    speed = 0;
            }
            else
            {
                speed-=ac;
            }
        }
        //sprintf(gstr, "speed = %f kmh", mc2kmh(speed));
        //cosd.SetTopText(gstr, 100);
    }
    else
        stopSignal = false;
    if( iinput.keystates[32] ) // space
        speed = 0;
    

    
    
    
}

//влияние трения
void CVehicle::Trenie()
{
    float trenie1 = trenie;
    if((rand()/RAND_MAX)>0.5)
    {
        trenie1 = trenie * rand()/RAND_MAX*10;
        //trenie1 = trenie1 / (0.1*speed);
    }
    
    if(speed > 0)
    {
        speed-=trenie1;
        if(speed<0)
            speed = 0;
    }
    
    if(speed < 0)
    {
        speed+=trenie1;
        if(speed>0)
            speed = 0;
    }
}

//автоматический возврат руля после вопорота
void CVehicle::WheelReturn()
{

    if((wang<2) && (wang>0))
        wang=0;
    if((wang>2) && (wang>0))
        wang-=2*(float)wang/max_wang;
    if((wang<-2) && (wang<0))
        wang-=2*(float)wang/max_wang;
    if((wang>-2) && (wang<0))
        wang=0;
}

//ограничение угла курса
void CVehicle::LimitKurs()
{
//	prn("%f", kurs.x);
	if ( kurs.x > 160 ) 
        kurs.x = kurs.x - 360;
    
    if ( kurs.x < 0 ) 
        kurs.x = kurs.x + 360;

    KursArrow();

}

//ограничитель стелки курса
void CVehicle::KursArrow()
{           
	kurs_arrow = kurs;
	kurs_arrow.x = 90 - kurs_arrow.x;
	return;

    if( kurs.x < 90 ) 
        kurs_arrow.x = 90 - kurs_arrow.x;
    else if ( (kurs.x) > 90 && (kurs.x < 180)) 
       kurs_arrow.x =  90 - kurs_arrow.x;    
    else if ( (kurs.x > 180) && (kurs.x < 270)) 
	{
       kurs_arrow.x =  (90 - (kurs_arrow.x+180));     
	   
	   //kurs_arrow.x-=kurs_arrow.x;
	   //int a=1;
	   //kurs_arrow.x+=180;
	}
    else if (  (kurs.x > 270) && (kurs.x < 360) ) 
	{
       kurs_arrow.x =   (90 - (kurs_arrow.x+180));   
	   //int a=1;
	   //kurs_arrow.x+=180;
	}


//    if ( kurs_arrow.x < -90 ) 
  //      kurs_arrow.x = kurs_arrow.x + 360;

	return;
    
    //prn("kurs.x = %f",kurs.x);

    //prn("kurs_arrow.x = %f",kurs_arrow.x);
}

void interp(double TTek, double t1, double t0, double h1, double h0, double* hh, double* hht, double* hhtt)
{
     double DT, A, AT, ATT, B, BT, BTT, a;
     DT = t1 - t0;
     A = (t1 - TTek) * (t1 - TTek) * (2.0 * (TTek - t0) + DT);
     AT = 6.00 * TTek * TTek + 2.0 * (DT - 2.0 * t0 - 4.0 * t1) * TTek +  4.0 * t1 * t0 - 2.0 * t1 * DT + 2.0 * t1 * t1;
     ATT = 12.0 * TTek + 2.0 * (DT - 2.0 * t0 - 4.0 * t1);
     B = (TTek - t0) * (TTek - t0) * (2.0 * (t1 - TTek) + DT);
     BT = -6.0 * TTek * TTek + 2.0 * (2.0 * t1 + DT + 4.0 * t0) * TTek - (4.0 * t0 * t1 + 2.0 * t0 * DT + 2.0 * t0 * t0);
     BTT = -12.0 * TTek + 2.0 * (2.0 * t1 + DT + 4.0 * t0);
     a = DT * DT * DT;
     *hh = (h0 * A + h1 * B) / a;
     *hht = (h0 * AT + h1 * BT) / a;
     *hhtt = (h0 * ATT + h1 * BTT) / a;
}


double rasstToPlane(Pnt p, double& A, double& B, double& C, double& D)
{
	return fabs(A*p.x+B*p.y+C*p.z+D)/sqrt(sq(A)+sq(B)+sq(C));
}


bool equalTo(double lv, double rv, double prec)
{
	return ( fabs(lv - rv) < prec );
}



int  CVehicle::FitWheel(Pnt w0p, Pnt w1p, Pnt w2p, Pnt w3p, int max_iters, int n, Pnt& Res, float dy, float prec  ) // подогнать одно колесо
{
	int iters = max_iters;
	
	if(1)
	{
		if( n == 0 ) 
		{ 

			while( --iters > 0) 
			{
				if( equalTo(rasst(w0p, w1p), width_wheels, prec )  &&
					equalTo(rasst(w0p, w2p), length_wheels, prec ))
				{
					Res = w0p;
					return iters;
				}
				w0p.y+=dy;
			}
		}
		if( n == 1 ) 
		{ 
			
			while( --iters > 0) 
			{
				if( equalTo(rasst(w0p, w1p), width_wheels, prec )  &&
					equalTo(rasst(w1p, w3p), length_wheels, prec ))
				{
					Res = w1p;
					return iters;
				}
				w1p.y+=dy;
			}
		}
		if( n == 2 ) 
		{ 
			
			while( --iters > 0) 
			{
				if( equalTo(rasst(w2p, w0p), length_wheels, prec )  &&
					equalTo(rasst(w2p, w3p), width_wheels, prec ))
				{
					Res = w2p;
					return iters;
				}
				w2p.y+=dy;
			}
		}
		if( n == 3 ) 
		{ 
			
			while( --iters > 0) 
			{
				if( equalTo(rasst(w3p, w1p), length_wheels, prec )  &&
					equalTo(rasst(w2p, w3p), width_wheels, prec ))
				{
					Res = w3p;
					return iters;
				}
				w3p.y+=dy;
			}
		}
	}	
    return 0;
}

//int pair_cnt = Ckn(2, MREG.allObj.size());
//ближайшее колесо на оси
int near_wheel(int i)
{
	if(i==0) return 1;
	if(i==1) return 0;
	if(i==2) return 3;
	if(i==3) return 2;
    return 0;
}

char* CVehicle::PrintfWheelPos(Pnt& w0p, Pnt& w1p, Pnt& w2p, Pnt& w3p)
{
	sprintf(wheels_pos_str, "w0 %s w1 %s", w0p.toStr("%1.2f,%1.2f,%1.2f").c_str(), 
													   w1p.toStr("%1.2f,%1.2f,%1.2f").c_str());
    prn(wheels_pos_str);
	sprintf(wheels_pos_str2, "w2 %s w3 %s", w2p.toStr("%1.2f,%1.2f,%1.2f").c_str(), 
													   w3p.toStr("%1.2f,%1.2f,%1.2f").c_str());
    prn(wheels_pos_str2);

	return wheels_pos_str;
}

// выравнивание колес по плоскости
void CVehicle::FitWheesInPlane(Pnt& w0p, Pnt& w1p, Pnt& w2p, Pnt& w3p )
{
	Pnt*  wp[4];
	double y[4];
	double Dis[4];
    float  dy = 0.001f;   // шаг по высоте
    //Pnt R0,R1,R2,R3;

	y[0] = w0p.y; y[1] = w1p.y; y[2] = w2p.y; y[3] = w3p.y;
	wp[0] = &w0p; wp[1] = &w1p; wp[2] = &w2p; wp[3] = &w3p;

	// фильтр слишком высоких выбросов колес
	// разница высот не дб больше длинны кузова
	// декартово пересечение
	double d[6];
	//int n_pairs = Ckn(2, 3);
	d[0] = fabs( w0p.y - w1p.y );
	d[1] = fabs( w0p.y - w3p.y );
	d[2] = fabs( w0p.y - w2p.y );
	d[3] = fabs( w1p.y - w3p.y );
	d[4] = fabs( w1p.y - w2p.y );
	d[5] = fabs( w3p.y - w2p.y );

	if( dy > length_wheels )
	{
		//dy = fabs( w2p.y - w0p.y );
	
	}
	// мат ожидание
	double M=0;
	for(int i=0; i<4; i++) M+=y[i];
	M=M/4;

	// дисперсия
	for(int i=0; i<4; i++) Dis[i] = ( y[i] - M ) * ( y[i] - M );

	//если дисперсия колеса > 1, коррекция высоты колеса и остановка
	double porog = 0.8;
	for(int i=0; i<4; i++) 
	{
		
		if( Dis[i] > porog )
		{
			wp[i]->y = wp[near_wheel(i)]->y;
			prn("correct %d", i);
			//speed = 0;
		}

	}
	


	//условие опрокидывания kren > 40 
	dy = w2p.y - w3p.y;
    //double kren = 90-atan(width_wheels / dy)/GR;
	//prn("kren %lf", kren );
	//prn("d %f %f %f %f %f %f", kren, d[0], d[1], d[2], d[3], d[4], d[5] );
	//prn("disperia %f %f %f %f", Dis[0], Dis[1], Dis[2], Dis[3]);
	


	
}

// выравнивание кузова по позиции и углу
void CVehicle::Svadba(float Hw)
{
    Pnt B,BB,C,CC;
    double dy,kren,tangaj,pribavka;
    Pnt w0p = w0->ei.coord;
    Pnt w1p = w1->ei.coord;
    Pnt w2p = w2->ei.coord;
    Pnt w3p = w3->ei.coord;
    B = line_eq(w1p, w3p, 0.5);
    BB = line_eq(w0p, w2p, 0.5);
    CC = line_eq(B, BB, 0.5);
    CC.y-= w1->bounds.y/2;

    kuzov->ei.coord = CC;
    kuzov->ei.coord.y+=0.2;

	dy = min(w1p.y, w0p.y) - min(w3p.y, w1p.y);
	if(dy < 0) pribavka = -90; else pribavka = 90;
	kren = 180 - atan(width_wheels / dy)/GR  + pribavka ;

	dy = (w3p.y - w1p.y);
	if(dy < 0) pribavka = 270; else pribavka = -270;

	tangaj = atan(length_wheels / dy)/GR + pribavka;
	//kuzov->rot.y = kurs.x ;

    kuzov->rot0 = Pnt(0, -kurs.x , kurs.y);
    kuzov->rot.x = kren;
    kuzov->rot.z = tangaj;

	//prn("kurs %f tangaj %f kren %f", tangaj, kren, kurs.x + 180);
	
	//length_wheels

}

void TerrainObject::FindClosestFaces(PPnt* p)
{


}

void TerrainObject::Render()
{
    /*
    for(unsigned int i=0; i < nfaces.size(); i++)
    {
        glEnable(GL_COLOR_MATERIAL);
        if( colision_faces[i] )
           glColor3f(1,0,0);
        else
           glColor3f(0,1,0);
        glBegin(GL_TRIANGLES);
            glVertex3f(nfaces[i].V[0].X(), nfaces[i].V[0].Y(), nfaces[i].V[0].Z());
            glVertex3f(nfaces[i].V[1].X(), nfaces[i].V[1].Y(), nfaces[i].V[1].Z());
            glVertex3f(nfaces[i].V[2].X(), nfaces[i].V[2].Y(), nfaces[i].V[2].Z());
        glEnd();
    }
    */
    //nfaces.clear();
}

bool CVehicle::hasColision()
{
    for(int i=0; i < terrain_objs.size(); i++)
        if(terrain_objs[i].bCollision)
            return true;
    return false;
}

Pnt CVehicle::takePointAtDist(float dist)
{
    Pnt p_near_f;
    p_near_f.z  = ei.coord.z - (dist/2)*sin(GR * kurs.x);
    p_near_f.y  = ei.coord.y + (dist/2)*sin(GR * kurs.y);
    p_near_f.x  = ei.coord.x - (dist/2)*cos(GR * kurs.x);
    return p_near_f;
}


void TerrainObject::DetectColision()
{
}

void TerrainObject::SetOnTerrain(Pnt& p_near_f, float t_rascheta)
{
	PPnt* ps; // стартовая точка на рельефе
	PPnt* pe; // конечная точка на рельефе через t_rascheta
	PPnt* p;
	
	if( ter==0 ) { return; }
	
	if( near_idx == -1 )
	{
		p = ter->FindNearestP(p_near_f);  // по всему ландшафту
		pe = p;
		near_idx = p->i;
        ts = Timer::gtimer.fsec;
        te = Timer::gtimer.fsec + t_rascheta;
	}
	else
	{
        p = ter->FindClosestTo(p_near_f, near_idx, 500, 0); // 100 точек в окрестности near_idx точки
        FindClosestFaces(p);
		
        if(1) // Отладкаs
        if(strstr(mesh->name.c_str(),"w0")!=NULL)
		{
			prn(p_near_f.toStr().c_str());
            //prn("minR = %f", rasst(ter->get_p(p->i), p_near_f));
		}

		if(p==0) { near_idx = -1; return; }
		
		double dy=0;
		
		if( ter->IsIdxValid( prev_idx ) )		
		{
			dy = *(ter->get_pnt(p->i)->y) - *(ter->get_pnt(prev_idx)->y);			
			dy = fabs(dy);
		}
		
		
		//if(p->i != prev_idx )
		//if( dy > bounds.y/4 )
        if((te  - Timer::gtimer.fsec ) < 0.5)
		//if(dy > 0.05)
		{
			bFindNewPoint = true;
			near_idx = p->i;		
            //prn("near_idx=%d", near_idx);
            ps = ter->FindClosestTo(pos, near_idx, 500, 0);
            FindClosestFaces(ps);
			pe = p;
			near_idx = p->i;
            ts = Timer::gtimer.fsec;
            te = Timer::gtimer.fsec + t_rascheta;
			H0 = *ps->y;
			H1  = *pe->y;
            //prn("ps = %d pe = %d \n heights %lf to %lf dy=%f", ps->i, pe->i, H0, H1, dy);
			p_near = Pnt(*(pe->x), *(pe->y), *(pe->z));			
		}
		else
			bFindNewPoint = false;

		
		prev_idx = near_idx;

	}
	
	
	//if(strstr(mesh->name.c_str(),"w0")!=NULL)		prn("near_idx =%d", near_idx);

	// найдена расчетная точка на ландшафте
	//ИНТЕРПОЛИРОВАТЬ
	if(H1!=0)
	{
        interp(Timer::gtimer.fsec, te, ts, H1, H0, &Ht, &Htt, &Httt);
		mesh->ei.coord = pos;
        mesh->ei.coord.y = Hc + Ht;
        //prn("mesh->ei.coord.y=%f",mesh->ei.coord.y);

        DetectColision();


		//prn("H tek = %f", Ht);
		//prn("raschet dlya %s", mesh->name.c_str());
	}

	// ДЕМПФИНГ
	//m * Htt + b * Ht + dH;

}

void CVehicle::resetTrenie()
{
    trenie = 0;
}

// расчет динамики подвески
void CVehicle::CalcPodveska()
{
	Pnt C0 = ei.coord;
	
//	kuzov->isCanRender = false;
	// расчет положения колес от-но рамы
    Pnt w0p;
    Pnt w1p;
    Pnt w2p;
    Pnt w3p;

	
    float w = c;
    float l = d;

    //1. точка переднего вала - P
    float al = 90 - kurs.x ;
    P = Pnt(a*l*sin(GR*al), 0, a*l*cos(GR*al));
    Z = Pnt(b*l*sin(GR*(al+180)), 0, b*l*cos(GR*(al+180)));


    w2p = Pnt(0, 0, w);
    rotateAboutY(w2p, GR*(al + 90)); w0p = w2p;


    w3p = Pnt(0, 0, -w);
    rotateAboutY(w3p, GR*(al + 90)); w1p = w3p;
    translate_p(w2p, P.x, P.y, P.z);
	translate_p(w3p, P.x, P.y, P.z);
    translate_p(w1p, Z.x, Z.y, Z.z);
    translate_p(w0p, Z.x, Z.y, Z.z);
	width_wheels = rasst(w0p, w1p);
	length_wheels = rasst(w1p, w3p);
	translate_p(P, C0.x, C0.y, C0.z);
	translate_p(Z, C0.x, C0.y, C0.z);
	translate_p(w3p, C0.x, C0.y-0.3, C0.z);
	translate_p(w2p, C0.x, C0.y-0.3, C0.z);
	translate_p(w1p, C0.x, C0.y-0.3, C0.z);
	translate_p(w0p, C0.x, C0.y-0.3, C0.z);
	WW = w2p;
	
    terrain_objs[0].pos = w0p;
    terrain_objs[1].pos = w1p;
    terrain_objs[2].pos = w2p;
    terrain_objs[3].pos = w3p;
    terrain_objs[4].pos = kuzov->ei.coord;
    p_near = terrain_objs[4].p_near;

	// расчет положения колес на ландшафте	
	// Ищем проекции точек колес на ландшафте через 1 секунду 
    MTerrain* ter = (MTerrain*)MREG.FindObj("maze01");
    if(0) // временно отключено
    for(int i=0; i<4; i++)
	{
		//расчет на секунду вперед
		float dist2 = speed * t_rascheta; 
		Pnt p_near_f;
	    p_near_f.z  = terrain_objs[i].pos.z - dist2*sin(GR * kurs.x);
	    p_near_f.y  = terrain_objs[i].pos.y + dist2*sin(GR * kurs.y); 
	    p_near_f.x  = terrain_objs[i].pos.x - dist2*cos(GR * kurs.x); 
		terrain_objs[i].SetTerrain(ter);
		terrain_objs[i].AddConstantHeight(0.2);
        //prn("before SetOnTerrain %s", terrain_objs[i].pos.toStr().c_str());
		terrain_objs[i].SetOnTerrain( p_near_f, t_rascheta);
        //prn("after SetOnTerrain %s", terrain_objs[i].pos.toStr().c_str());

		//гравитация
/*		Pnt f[4];
		ter->FindClosestFace(f, terrain_objs[i].pos, terrain_objs[i].near_idx,  500, 0);
		float Ysr = (f[0].y + f[1].y + f[2].y) / 3;
		float dy = (terrain_objs[i].pos.y - Ysr);
		//prn("dy %f", dy);
		terrain_objs[i].pos.y-=1.2*dy;
*/

		// дрейф
		//if(speed == 0)
		//dm[i]->DoDreif(dt);
		

		//если нашли новую точку даем воздействие на аммортизаторы
		//if( terrain_objs[i].bFindNewPoint )
		if( dm[i]->TimeSinceLastPush() > 3  )
		{
			float dH = fabs(terrain_objs[i].Htt*3);			
			
			dH = (terrain_objs[i].H0 - terrain_objs[i].H1)*0.1;
			//dt = 2.5 * rand()/RAND_MAX ;
			//dt = 0.8;
			//prn("Push %d %f %f Htt %f", i, dH, dt, terrain_objs[i].Htt);
			//dm[i]->Push(dH, dt);			
			if(dH > 5) dH = 0.5;
			if(dH < -5) dH = 0.5;
			//if( dm[i]->tK < 0.1 )  dm[i]->Push2(2, 2.5 * rand()/RAND_MAX);
//			if(Timer::gtimer.fsec < 10)
			//dm[i]->AddKoleb(2, 2.5 * rand()/RAND_MAX);
			dm[i]->AddKoleb(dH, (float)dt);
			dm[i]->Push2(dH, (float)dt);
            //if(i==0) GRAF(0,0,Timer::gtimer.fsec,dH);
			
		}
	}

	//растяжение и сжатие пружин демперов от гравитации	
	float k = 4;
	if(w3p.y < w0p.y)
	{
		float dy = fabs(w3p.y - w0p.y) * k;
		dm[3]->m1 = dm[3]->m1_or - dy;
		dm[0]->m1 = dm[0]->m1_or + dy;
	}
	if(w3p.y > w0p.y)
	{
		float dy = fabs(w3p.y - w0p.y)* k;
		dm[3]->m1 = dm[3]->m1_or - dy;
		dm[0]->m1 = dm[0]->m1_or + dy;
	}
	if(w1p.y < w2p.y)
	{
		float dy = fabs(w1p.y - w2p.y)* k;
		dm[1]->m1 = dm[1]->m1_or  - dy;
		dm[2]->m1 = dm[2]->m1_or + dy;
	}
	if(w1p.y > w2p.y)
	{
		float dy = fabs(w1p.y - w2p.y)* k;
		dm[1]->m1 = dm[1]->m1_or  - dy;
		dm[2]->m1 = dm[2]->m1_or  + dy;
	}
	sprintf(prijina_m_str,"m prujin: %f %f %f %f", dm[0]->m1,dm[1]->m1,dm[2]->m1,dm[3]->m1 );
	sprintf(prijina_tK_str,"tK prujin: %f %f %f %f", dm[0]->tK,dm[1]->tK,dm[2]->tK,dm[3]->tK );



	// выравнивание в плоскости
    //FitWheesInPlane(w0p, w1p, w2p, w3p);
	

	// положения демперов
    dm[0]->ei.coord = terrain_objs[0].p_near; dm[0]->ei.coord.y+=h_dempers;
    dm[1]->ei.coord = terrain_objs[1].p_near; dm[1]->ei.coord.y+=h_dempers;
    dm[2]->ei.coord = terrain_objs[2].p_near; dm[2]->ei.coord.y+=h_dempers;
    dm[3]->ei.coord = terrain_objs[3].p_near; dm[3]->ei.coord.y+=h_dempers;

	// шаг симуляции демперов
	for(int i=0; i<4; i++)
	{
		//dm[i]->Lt = 10 /sqrt(10+terrain_objs[i].Ht); // L = k / sqrt(H)
        dm[i]->Slot1();
	}

    /*
	w0p.y+= dm[0]->Lt;
	w1p.y+= dm[1]->Lt;
	w2p.y+= dm[2]->Lt;
	w3p.y+= dm[3]->Lt;
    */

    // присвоить колесам только высоты обьектов TerrainObject
    w0p.y = terrain_objs[0].p_near.y;
    w0->ei.coord = w0p;
    w1p.y = terrain_objs[1].p_near.y;
    w1->ei.coord = w1p;
    w2p.y = terrain_objs[2].p_near.y;
    w2->ei.coord = w2p;
    w3p.y = terrain_objs[3].p_near.y;
    w3->ei.coord = w3p;

    //PrintfWheelPos(w0p, w1p, w2p, w3p);

/*	
	// демпфинг
	Pnt wpD[4];
	for(int i=0; i<4; i++)	
		wpD[i] = terrain_objs[i].CalcDemphing(terrain_objs[i]);
	
	// выравнивание в для Свадьбы колес и кузова	

*/

	//Свадьба
    Svadba(0.15f);

	//воврат колес
/*	w0p.y =  terrain_objs[0].Ht + terrain_objs[0].Hc - dm[0]->Lt*dm[0]->Asimmetry;
	w1p.y =  terrain_objs[1].Ht + terrain_objs[1].Hc - dm[1]->Lt*dm[1]->Asimmetry;
	w2p.y =  terrain_objs[2].Ht + terrain_objs[2].Hc - dm[2]->Lt*dm[2]->Asimmetry;
	w3p.y =  terrain_objs[3].Ht + terrain_objs[3].Hc - dm[3]->Lt*dm[3]->Asimmetry;
*/

	sprintf(prijina_L_str,"Lt prujin: %f %f %f %f", dm[0]->Lt,dm[1]->Lt,dm[2]->Lt,dm[3]->Lt );
}

void CVehicle::Slot1() //timer slot
{
    static float cnt=0;
    static float phi1=0, phi2=0;
    int idx;

    if((kuzov->vertices == NULL) || (w0->vertices==NULL)|| (w1->vertices==NULL)|| (w2->vertices==NULL)|| (w3->vertices==NULL))
    {
        LoadModels("kuzov", "w0");

        return;
    }

    //применяем управление
    if( bIsReceiveInput )
        InputControl();
    
    //Krivizna();
    
    //возврат руля после поворота
    WheelReturn();

    //влияние трения
    Trenie();
    
    //скорости вращения колес
    ffw = speed / 2*M_PI;
    fbw = speed / 2*M_PI;

    //расчитываем курс от угла поворота колес
    kurs.x-= wang * speed / delkoof;

 // Делаем цикл симуляции
     dt = (double)(timer.elapsed())/1000.;
     timer.restart();



    float dist = speed * dt; //2 * M_PI * r * 0.05 *ffw;
    
    S+=dist;

    // поиск ближайшей путевой точки
    if( activeTraj != 0)
    {
        int i0 = near_idx;
          FindNearPnt(activeTraj, p_near, near_idx);
        int i1 = near_idx; 

        
        if( near_idx != i0)
        {
            if( i1 > i0)
            {
                bGoToFinish = true;
                RGB2GL2(RGB(0,255,0),vp->color);
                RGB2GL2(RGB(0,0,255),vp2->color);

            }
            else
            {
                bGoToFinish = false;
                RGB2GL2(RGB(0,255,0),vp2->color);
                RGB2GL2(RGB(0,0,255),vp->color);
            }
        }

        //if(bGoToFinish)
        //   upds("WhereTo","Finish");
        //else
        //      upds("WhereTo","Start");


        CheckReachTargetWp();

        if( bReachTargetWp )
        {
            prn("Достигли target_wp");
            FindNearWayPoint(waipoints, ei.coord, idx);
            idx-=1;
            if(idx<0)
                idx=waipoints.size()-1;

            if( ((int)waipoints.size() > idx) && idx >= 0 )
            {
                target_wp = waipoints[idx];
                pscr.upds("target_wp",target_wp.toStr());
            }
            else
                target_wp = Pnt(ei.coord.x+5,ei.coord.y,ei.coord.z);
        }


        //расчитать вперед или назад по траектории на расстояние до 4 едениц        
        //if( bForward )        
        float max_dal = 4; // максимальная дистанция от машины до расчетной точки
        Pnt p_to;
        int fi = FindForward(activeTraj, near_idx, p_to, max_dal);
        *(Pnt*)vp = p_to;
        //else
        int bi = FindBackward(activeTraj, near_idx, p_to, max_dal);        
        *(Pnt*)vp2 = p_to; //p_near;

        pscr.updi("fi", fi);        pscr.updi("bi", bi);

        ei.coord.z  = ei.coord.z - dist*sin(GR * kurs.x);
        ei.coord.y  = ei.coord.y + dist*sin(GR * kurs.y); 
        ei.coord.x =  ei.coord.x - dist*cos(GR * kurs.x); 

        
    }
    else
    {
        ei.coord.z  = ei.coord.z - dist*sin(GR * kurs.x);
        ei.coord.y  = ei.coord.y + dist*sin(GR * kurs.y); 
        ei.coord.x  = ei.coord.x - dist*cos(GR * kurs.x); 
		
    }

    //AssignMyCoord(); 	
    //kuzov->ei.coord = ei.coord;
    //kuzov->rot0 = Pnt(kurs.z, kurs.x, kurs.y);

    //kuzov->isCanRender = false;
    //расчеты поведения подвески на ландшафте
    CalcPodveska();

    //kuzov->isCanRender = true;

    //ограничение курса
    //LimitKurs();
    
    // вращение колес
    if ( phi1 > 360 ) 
        phi1 = phi1 - 360;
    if ( phi2 > 360 ) 
        phi2 = phi2 - 360;

    phi1 = phi1 + 360*0.05*ffw;  //50ms * частота передних

    w0->rot = Pnt(0, wang, -phi1); w0->dir = dir;
    w1->rot = Pnt(0, wang, -phi1); w1->dir = dir;
    w0->rot0 = Pnt(kurs.z, -kurs.x, 0); w0->dir = dir;
    w1->rot0 = Pnt(kurs.z, -kurs.x, 0); w1->dir = dir;
    phi2 = phi2 + 360*0.05*fbw;  //50ms * частота задних
    w2->rot = Pnt(0, 0, -phi2); w2->dir = dir;
    w3->rot = Pnt(0, 0, -phi2); w3->dir = dir;
    w2->rot0 = Pnt(kurs.z, -kurs.x, 0); w2->dir = dir;
    w3->rot0 = Pnt(kurs.z, -kurs.x, 0); w3->dir = dir;



    SetMatrix(w0);
    SetMatrix(w1);
    SetMatrix(w2);
    SetMatrix(w3);
    SetMatrix(kuzov);


    
    pscr.upd("carPos", ei.coord);
    pscr.upd("phi1", phi1);
    pscr.upd("wang", wang);
    pscr.upd("kurs", kurs.x);
    pscr.upd("S", S);
    //mu_printf("Pos=%s",ei.coord.toStr().c_str());
    
    //TEST ON VERTEX FLOATING
    /*

    for(int i=0; i<kuzov->vert_count; i++)
    {
    flags.bChangedRenderMode = true;
    kuzov->vertices[i*3] = 10 * rand() / RAND_MAX;
    }

    */
    bounds = kuzov->bounds;
    v = Pnt(speed,0,0);
    kurs0 = kurs;


    

}

void CVehicle::SetMatrix(GemObject* mesh)
{
    QMatrix4x4 model_matrix;

    model_matrix.translate(mesh->ei.coord.x,mesh->ei.coord.y,mesh->ei.coord.z);
    model_matrix.rotate( mesh->rot0.y, 0,dir, 0);
    model_matrix.rotate( mesh->rot0.x, dir, 0, 0);
    //model_matrix.rotate( mesh->rot0.z, 0, 0, -1);
    model_matrix.translate( mesh->C0.x, mesh->C0.y, mesh->C0.z );
    model_matrix.rotate( mesh->rot.y, 0, dir, 0);
    model_matrix.rotate( mesh->rot.x, dir, 0, 0);
    model_matrix.rotate( mesh->rot.z, 0, 0, -1);
    //model_matrix.rotate( ei.rot.x, rot.x, rot.y, rot.z);
    model_matrix.translate( -mesh->C0.x, -mesh->C0.y, -mesh->C0.z );
    model_matrix.rotate( -90, 0, 1, 0);
    mesh->model_matrix = model_matrix;

}

void CVehicle::Render(QOpenGLShaderProgram& program)
{
    static double mat[3][3],mat3[3][3],mat4[3][3],mat5[3][3],mat6[3][3];
    static double mat2[3][3];
    static double Mat[4][4],Mat2[4][4],Mat3[4][4],Mat4[4][4],Mat5[4][4],Mat6[4][4],Mat7[4][4];


	  if((kuzov->vertices == NULL) || (w0->vertices==NULL)|| (w1->vertices==NULL)|| (w2->vertices==NULL)|| (w3->vertices==NULL))
        return;
    {
        //kuzov->bUpdateMesh = true;

        //if(kuzov->isCanRender)
        {
            kuzov->Render(program);

            w0->Render(program);
            w1->Render(program);
            w2->Render(program);
            w3->Render(program);
        }

    }

   program.setUniformValue("stopSignal", stopSignal);

   //draw_p(program,"P",P,RGB(255,0,0),0);
   //draw_p(program,"Z",Z,RGB(0,0,255),0);
   //Pnt future_p = takePointAtDist(0);
   //draw_p(program,"future_p",future_p,RGB(0,255,0),0);


    for(int i=1; i<lookpts.size(); i++)
        draw_p(program,"",std::get<0>(lookpts[i]),RGB(0,0,std::get<1>(lookpts[i])*50),0);

    draw_p(program,"",select_pointz[0],RGB(255,0,0),1);
    //draw_p(program,"",select_pointz[1],RGB(255,0,0),0);


    RenderOSD();

    if( bShowWaipoints )
      draw_pts(program, waipoints,"",RGB(130,11,11),0);
     
    //линия до target point
/*    set_col(RGB(255,0,0));
    glBegin(GL_LINES);
        glVertex3f(ei.coord.x,ei.coord.y,ei.coord.z);
        glVertex3f(target_wp.x,target_wp.y, target_wp.z);
    glEnd();
    unset_col();

*/
    //вектор курса
     Pnt kp1(0,0,0), kp2(0,0,-4);

    tvect v;
    mroty(kurs_arrow.x*GR, mat2);   // mat3344(mat2,Mat2);
    mvmult(3,mat2,(float*)&kp1,(float*)&v.A);   
    mvmult(3,mat2,(float*)&kp2,(float*)&v.B);
	//draw_p("kp2", v.B, RGB(255,0,0),1);
	

    for(unsigned int i=0; i<terrain_objs.size(); i++)
    {
        //
        draw_p(program, "", terrain_objs[i].p_near, RGB(255,0,255),0);
        if( terrain_objs[i].bCollision )
        {
            terrain_objs[i].mesh->isSelected = 2;
        }
        else
            terrain_objs[i].mesh->isSelected = 0;
        terrain_objs[i].Render();
    }

    //draw_p("P", P, RGB(0,220,255),1);
    //draw_p("Z", Z, RGB(0,230,255),1);
    //draw_p("WW", WW, RGB(0,230,255),1);
	

    //prn("T(%s) ->%f %f", ei.coord.toStr().c_str(), f1, f2);   
    v.A.x+=ei.coord.x;
    v.A.y+=ei.coord.y;
    v.A.z+=ei.coord.z;
    v.B.x+=ei.coord.x;
    v.B.y+=ei.coord.y;
    v.B.z+=ei.coord.z;

	//вектор курса только если окно сверху
	if(main_if.activeScreen != 1) 
        draw_v(v, 0.3f, RGB(0,0,255));
    //prn("v = %s %s ", v.A.toStr().c_str(), v.B.toStr().c_str());

    //draw_p("v", v.B, RGB(255,0,0),1);

    //upds("A",v.B.toStr());
    //upds("B",v.A.toStr());

    //draw_v(tvect(ei.coord, target_wp), 0.3, RGB(255,0,0));
    
    //точка назначения
    draw_p(program, "target_wp", target_wp, RGB(255,0,0),1);

    if( bReachTargetWp )
        draw_p(program, "target_wp", target_wp, RGB(0,0,255),1);


	SetCamPoints();
	
//	draw_p("dp",dp,RGB(0,0,0),1);


}

void CVehicle::NullReferences()
{
    kuzov->vertices = NULL;
    w0->vertices = NULL;
    w1->vertices = NULL;
    w2->vertices = NULL;
    w3->vertices = NULL;
};


void CVehicle::OnSelect()
{
    mu_printf("CVehicle::OnSelect()");
    SelectAll();
    ListCurves();
}

void CVehicle::OnUnselect()
{
    mu_printf("CVehicle::OnUnselect()");
    UnSelectAll();
}


void CVehicle::SetCamPoints()
{
  	// точки направления 
	Pnt rt;
	rt = Pnt(0, (90 - kurs.x) * GR, 0);

    //pp1.GetProjections( ei.coord, rt, 8 , campts1);
    //pp2.GetProjections( ei.coord, rt, 10,  lookpts2);


}



//object inspector event
void CVehicle::OnAttrChange(t_atr& a)
{
	static int ft= 1;

	if(isLoading)
	{
		
		ft = 0;
		//return;
	}
    
    mu_printf("CVehicle::OnAttrChange %s", a.name.c_str());
    if(  (a.name == "trajectory")  )
    {
        icurve = a.data;        
        AssignTrajectory();
        //prn("%d", 1);
        return;
    }
	
//"n nu nd w wu wd e eu ed s su sd nw nwu nwd ne neu ned sw swu swd se seu sed";
	if(  (a.name == "campos")  )
    {
        nCamPnt = a.data;        
        prn("campos[0..24] = %d", nCamPnt);
		
        return;
    }

	
    if(  (a.name == "wp_delta")  )
    {
        AssignTrajectory();
        return;
    }

       if(  (a.name == "Krivizna")  )
    {
        Krivizna();
        return;
    }
      if(  (a.name == "OnStart")  )
    {
        OnStart();
        return;
    }

    
    Object::OnAttrChange(a);

}
