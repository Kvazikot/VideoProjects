/********************************

  Demper это класс который симулирует работу аммортизатора

  Свойства:

  Методы:
    Методы для отработки функций
  

*********************************/

//REALISATION CPP
#include "../include/stdafx.h"
#include "../include/print.h"
#include "../include/curve.h"
#include "../include/vpnt.h"
#include "../include/geometry.h"
#include "../include/glcode.h"
#include "../include/random_staff.h"
#include "../include/law.h"
#include "../include/matrix.h"
#include "../include/vector3d.h"
#include "../include/kirpich.h"
#include "../include/interp.h"
#include "../include/demper.H"
#include "../include/vehicle.h"
#include "../include/timer.h"
#include "../include/cinput.h"
#include "../include/cobjregistry.h"

extern CInput iinput;
static CObjRegistry& MREG = CObjRegistry::getInstance2();
char gstr[10000];
//====================================================================
// <OBJECT>   </OBJECT>
//====================================================================

Demper::Demper()
{
	classname = "Demper";
	isCanRender = true;
	isEntety = true;

    lastPushTime = Timer::gtimer.fsec;
	m1 = 14; 
	tsim = 0;
	m2 = 2000; // массы приходящаяся на колесо
    b = 0.003f; // коэффициент трения
    k = 0.3f; // коэффициент жесткости амортизаторов
	L0   =  18; // длинна пружины в покое
	Amax =  L0/8; // амплитуда колебаний 
	tK =    -1;	// время колебаний
	w =     1;    // Гц частота
	accel = 2;
	tNachala = 0;
	Asimmetry = 0;
    Dreif = 0.13f; // дрейф нуля
    t0 = Timer::gtimer.fsec;
	t_prev = 0;
	Nkolebani = 0;
	bIsMasterObject = false;
	bDrawGrafics = false;

	kir1 = new Kirpich("kir1",box1.x,box1.y,box1.z); 
	kir1->Declare();
	MREG.AddObj(kir1);
	kir1->isNotSaving = true;
	kir1->isCanRender = true;
	kir1->ei.coord = Pnt(0, 0, 0);
	kir2 = new Kirpich("kir2",box2.x,box2.y,box2.z); 
	kir2->Declare(); 
	MREG.AddObj(kir2);
	kir2->isNotSaving = true;
	kir2->ei.coord = Pnt(0, L0, 0);


	// красный цвет по умолчанию
	RGB2GL2(RGB(255,0,0),color);

	//iinput.add_delegat(this);
	
	//Connect();

	StartSimulation();
}

void Demper::LotusSettings()
{
	A = 0;
    box1=Pnt(0.200000f,0.200000f,0.200000f);
    box2=Pnt(0.200000f,0.200000f,0.200000f);
	kir1->a = box1.x; kir1->b = box1.y; kir1->c = box1.z;
	kir2->a = box2.x; kir2->b = box2.y; kir2->c = box2.z;
	m1=22.000000 ;
	m1_or=22.000000;
	L0=0.500000 ;
	L0_original=L0;
    b=0.700000f;
    k=4541.299805f;
    r=0.100000f;
	N=12.000000 ;
    Amax=0.674794f;
	tK=-1;
    w=14.367418f;
    Dreif=0.003000f;

}

void Demper::StopSimulation() 
{ 
    //CTime _tst = CTime::GetCurrentTime().GetTime();
    //sprintf(gstr,"%d", GetTickCount());
    //CString s = _tst.Format("Simulation stopped at %H:%M:%S.") + gstr;
    //prn(s.GetBuffer());
	bDoKolebanie = false; 
}


void Demper::literpP(float& param, float p0, float p1, float dt, float t0, float t1, float a)
{
	float dp = (p1 - p0) * dt / t1 + a*dt*dt / 2.;	
	param+=dp;    
    t0=0;

}



// инициализация
void Demper::StartSimulation()
{

	if( bDoKolebanie ) 
	{
		Reset();
		bDoKolebanie = false;  
		return;
	}
	bDoKolebanie = true;  
	Lt = 0;
	Amax =  L0/8; // амплитуда колебаний 
	A = Amax;
	dx1 = 2; 
	dx2 = 2; // отклонения
	Amax =  L0/8; // амплитуда колебаний 
	accel = 2;
    t0 = Timer::gtimer.fsec;
    //GRAF_CLEAR();	// настройки графиков

}; 

//Сброс
void Demper::Reset()
{
	Lt = Amax / 2;
	A = Amax;
}


// сброс времени
void Demper::ResetTime()
{
	tsim = 0;
	t_prev = tsim;
}
void Demper::DoDreif(float dt)
{
    dt=0;
}

void Demper::ClearKolebList()
{
	vector<Kolebanie>::iterator i;
	vector<int> to_del;
    for(int i=0; i<(int)kolebs.size(); i++)
	{
		if( kolebs[i].TimeIsUp() )
			to_del.push_back(i);
	}

    for(int i=0; i<(int)to_del.size(); i++)
	{
		if(to_del[i]!=0)
			kolebs.erase(kolebs.begin()+to_del[i]);
	}

}

//  добавить воздействие (новую гармонику)
void Demper::AddKoleb(float dH, float tv)
{
	Kolebanie klb;
    float	g = 9.8f;
	if(tv == 0) return;
	if(dH == 0) return;
	float v0 = (dH + g * tv * tv / 2) / tv; // мгновенная скорость падения груза
	klb.Amax = sqrt( fabs(m1 * v0 * v0 / k) ); // растяжение пружины
	klb.tK = b * dH;			// время колебаний пропорционально перепаду высот 									
	klb.w = sqrt(k / m1);       // времени падения груза
	klb.A    =  Amax;
	klb.t = 0;
	kolebs.push_back(klb);
	Nkolebani = kolebs.size();
}


// воздействие без колебания (просто пересчет сжатия и растяжения)
void Demper::Push2(float dH, float tv)
{
    float	g = 9.8f;
	float	v0;
	
	if( (tsim - tNachala) < tK )
		return;
	
	bDoKolebanie = false;
    lastPushTime = Timer::gtimer.fsec;
	v0 = (dH + g * tv * tv / 2) / tv; // мгновенная скорость падения груза
	Amax = sqrt( m1 * v0 * v0 / k ); // растяжение пружины
	tK = fabs(b * dH) +0.1;			// время колебаний пропорционально перепаду высот 									
	w = sqrt(k / m1);       // времени падения груза
	A    =  Amax;
	tNachala = tsim;
	Lt = L0 + Amax;
	//bDoKolebanie = true;
    //prn("Push2 at tsim %f tK=%f Amax=%f ", tsim, tK, Amax);
}

// воздействие задающее затухающее колебание пружины
void Demper::Push(float dH, float tv)
{
    double  dt,t;
	int     i=0,Ni=0;
    float	g = 9.8f;
	float	v0;
	
	if(!bDoKolebanie) return;

    lastPushTime = Timer::gtimer.fsec;
	bDoKolebanie = false;
	//GRAF_CLEAR();	// настройки графиков
	//StartSimulation();
	v0 = (dH + g * tv * tv / 2) / tv; // мгновенная скорость падения груза
	Amax = sqrt( m1 * v0 * v0 / k ); // растяжение пружины
	A    =  Amax;
	tK = b * dH;			// время колебаний пропорционально перепаду высот 									
	w = sqrt(k / m1);       // времени падения груза
	t = 0;
	
	if(0)
	if(bIsMasterObject) // SPLINE
	{
		float Fd = 100;
		dt = 1.0/Fd;
		Ni = (int)(tK * Fd);
		set_timedomain(Time_domain, dt, Ni);
		while( t < tK )
		{
			literpP(A, Amax, 0, dt, 0, tK, accel);
			float f = L0  + A * cos( 2 * M_PI * w * t );	
			arr[i]=f;
			t+=dt;
			i++;
		}

		spline.SetN(Ni);
		spline.SetXY(Time_domain, arr);
		spline.Calc();

		t = 0;
		while( t < tK )
		{
			
			//mpage0->AddXY(0, 0, t, f);
             //GRAF(0, 1, t, spline.GetF(t));
			 t+=dt;
		}
		A    =  Amax;
	}
	bDoKolebanie = true;
}

//

// шаг симуляции 
void Demper::Simulate()
{

    float dt = Timer::gtimer.fsec	- t_prev;
	tsim+=dt;

	// одиночное колебание
	if( bDoKolebanie ) 
	{
		literpP(A, Amax, 0, dt, 0, tK, accel);
		if( fabs(A) < L0 )
			Lt = L0  + A * cos( 2 * M_PI * w * tsim );	
		
			
	}

	//суперпозиция нескольких колебаний 
/*	vector<Kolebanie>::iterator i;	
	Lt = L0;
	for(i=kolebs.begin(); i<kolebs.end(); i++)
	{
		if(i == kolebs.begin()) continue;
		Kolebanie& p = (*i);
		literpP(p.A, p.Amax, 0, dt, 0, p.tK, accel);
		//if( fabs(p.At) < L0 )
		p.At = p.A * cos( 2 * M_PI * p.w * p.t );			
		p.t+=dt;		
		Lt=p.At ;
	}
*/
	//очистить просимулированные колебания
	ClearKolebList();

	// Начинать сложение колебаний после 5 секунд симулЯции

    //if(name == "dm0")
    //	GRAF(0, 0, tsim, Lt);

    t_prev = Timer::gtimer.fsec;

}



void Demper::CalcSpiral(float r, float N, float H)
{
	float a,da,L,ddy;
	Pnt   p;

	L = 0;
	float dy = H / N; 
	while( L < H )
	{
		a = 0;
		da = 1;
		ddy = dy / 360;
		while( a < 360 )
		{

			p.x = ei.coord.x + r * cos( a * GR );	
			p.y = ei.coord.y + L;					
			p.z = ei.coord.z + r * sin( a * GR );
            //vbuf.AddVertex(p);
			a+=da; L+=ddy;
		}
		
		
	}

		
}


void Demper::Render(QOpenGLShaderProgram& program)
{
    Pnt p1[9];
    if(!isCanRender)
        return;
	glPushMatrix();
	

	// пружина
	glTranslatef(0, 2*kir1->bounds.y,0);
	glEnable (GL_LINE_SMOOTH);      
    //vbuf.Draw(1,0,0,1);
	glDisable (GL_LINE_SMOOTH);     

	// подпись к блокам
	if(bShowText)
	{
		sprintf(gstr, "			m1 = %2.2f kg ", m1);
        draw_p(program, gstr, Pnt(ei.coord.x, Lt, ei.coord.z), RGB(0,0,1), 0);
	}
	
	
	glPopMatrix();
	


}


//object inspector event
void Demper::OnAttrChange(t_atr& a)
{
	bDrawGrafics = bIsMasterObject;
	prn("Demper::OnAttrChange %s", a.name.c_str());
	if(a.name == "StartSim") StartSimulation();
	if(a.name == "StopSim") StopSimulation();

	if(a.name == "box1") 
	{
		kir1->a = box1.x; kir1->b = box1.y; kir1->c = box1.z;
	}

	if(a.name == "box2") 
	{
		kir2->a = box2.x; kir2->b = box2.y; kir2->c = box2.z;
	}

	if(a.name == "Push") 
	{
		if (isLoading) return;
		float dH, dt;
		dH = rand()%4  + (float)rand()/RAND_MAX;
		dt = 2.5 * rand()/RAND_MAX ;
		sprintf(gstr,"%f %f", dH, dt);
		args_push = gstr;
		sscanf(args_push.c_str(), "%f %f", &dH, &dt);
		Push(dH, dt);
	}

	if(a.name == "freqs") 
		spectrum.Parse(freqs, ampls);
	
	if(a.name == "ampls") 
		spectrum.Parse(freqs, ampls);	

	if(bIsMasterObject)
		HandleMasterObject(a.name == "Push");
}

// настройки подвески лотуса
void Demper::HandleMasterObject(bool Push)
{
	CVehicle* veh01 = (CVehicle*) MREG.FindObj("veh01");
	if(veh01==0) return;
	for(int i=0; i<4; i++)
	{
		veh01->dm[i]->m2 = m2;
		veh01->dm[i]->m1 = m1;
		veh01->dm[i]->b = b;
		veh01->dm[i]->k = k;
		veh01->dm[i]->Dreif = Dreif;
		veh01->dm[i]->Asimmetry = Asimmetry;
		if(Push)
		{
			float dH, dt;
			dH = rand()%4 + rand()/RAND_MAX;
			dt = 2.5 * rand()/RAND_MAX;			
			veh01->dm[i]->Push(dH, dt);
		}
	}

}


//obj update slot
void Demper::Slot1()
{
	//отно позиции
	kir2->ei.coord = ei.coord;
	kir2->ei.coord = ei.coord;

	// симулировать
	Simulate();

	// просчитать изображение демпфера
    //vbuf.Clear();
	if( (Lt > 0) && (Lt < 1000) )
	{
		CalcSpiral(r, N, Lt);	
		//kir2->ei.coord.y = Lt;		
		kir2->ei.coord.y+=Lt + kir1->bounds.y;
	}

}

//obj destroy
void Demper::Slot4()
{

}
