#pragma once

#include "vbuf.h"
#include "vpnt.h"
#include "kirpich.h"
#include "timer.h"
#include "interp.h"
#include "var_parse.h"			// для разбора таблиц vt

struct FreqSample
{
		int   N;	// номер гармоники
		float f;	// частота гармоники
		float A;    // амплитуда
};

struct Spectrum
{
	vector<FreqSample> freqs;
	// кратные частоты  1 0.5 0.3 0.25
	// амплитуды		1 0.3 0.2 0.1	
	void Parse(string freq, string ampl)
	{
		vector<double> d1,d2;
		VarParser vp;
		vp.ParseDoubleArray(freq, d1);
		vp.ParseDoubleArray(ampl, d2);
		int size = min(d1.size(), d2.size());
		for(int i=0; i<size; i++)
		{
			FreqSample s;
			s.A = d2[i];
			s.f = d1[i];
			s.N = i;
			freqs.push_back(s);
		}
	}

};

struct Kolebanie
{
	Kolebanie()	{tK = -1; t = 0;}
	Kolebanie(float _Amax, float _tK, float _w, float _A):
	Amax(_Amax),tK(_tK),w(_w),A(_A)
	{tK = -1; t = 0;	}
	float Amax;    // амплитуда
	float tK;	   // время колебания
	float w;		// частота гармоники
	float A;	   // амплитуда затухания
	float At;	   // амплитуда конечная

	float t;	  //  текущее вермя смиуляции
	bool TimeIsUp() { return (t > tK);}
};


class Demper : public RenderObject
{
 //   Q_OBJECT
public:

	Demper();
	Demper(Demper& B)
	{
        Lt = B.Lt;
	}

	
	GLfloat color[4];
	Spectrum spectrum; 
	vector<Kolebanie> kolebs; // запланированные колебания
	Kirpich* kir1;
	Kirpich* kir2;
    string nullstring;
	float  dx1,dx2; // отклонения
	float  m1, m2; // массы приходящаяся на колесо
	Pnt    box1, box2;
	float  b; // коэффициент трения 
	float  k; // коэффициент жесткости амортизаторов 
	float  r,N; // параметры пружины	
	int    Nkolebani;
	float  A,At,Att,accel, Amax, tK, w;
	float  Ltt,Lttt,Lt_prev;
	float  Dreif;
	float  t_rascheta;
	float  t0, tsim, t_prev, tNachala;
	float  Lt;
	float  L0;
	float  L0_original;
	float  m1_or;
	double lastPushTime;
	float  Asimmetry;
	bool   bDoKolebanie;
	bool   bShowText;
	bool   bDrawGrafics;
	bool   bIsMasterObject;
    //VertexBuffer vbuf; // для отрисовки
	string args_push, freqs,ampls ;

	void LotusSettings();	// предустановки для лотуса			
	void Push(float dH, float tv);		  // Входные параметры - перепад высот, время воздействия
	double GetLt() { return Lt; }		  // Выходной параметр - длинна пружины
	void Push2(float dH, float tv);		  // Входные параметры - перепад высот, время воздействия
	void AddKoleb(float dH, float tv);    //  добавить воздействие (новую гармонику)
	void ClearKolebList();
	void DoDreif(float dt);
    double TimeSinceLastPush(){ return (Timer::gtimer.fsec - lastPushTime); }
	void Simulate();
	void StopSimulation();
	void StartSimulation();
	void ResetTime();
	void DrawSpiral();
	void CalcSpiral(float r, float N, float H);
	void literpP(float& param, float p0, float p1, float dt, float t0, float t1, float a);
	void literpPSpl(float& param, float& pt, float& ptt, float p0, float p1, float dt, float t, float t0, float t1);
    void Reset();
	void HandleMasterObject(bool Push);
	
	double		  Time_domain[200000];
	double		  arr[200000];
	Spline		  spline;

	void Declare()
	{
		classname = "Demper";
		
		SET_OBJECT(this,(char*)name.c_str());
		SET_ATTR("bIsMasterObject",  &bIsMasterObject, "bool"); 		
		SET_ATTR("pos",  &ei.coord, "Pnt"); 
		//SET_ATTR("freqs",  &freqs, "string"); 
		//SET_ATTR("ampls",  &ampls, "string"); 
		SET_ATTR("b",   &b, "float"); 
		SET_ATTR("Asimmetry",   &Asimmetry, "float"); 
		SET_ATTR("Nkolebani", &Nkolebani, "int"); 
		SET_ATTR("k",   &k, "float"); 
		SET_ATTR("m2",  &m2, "float"); 
		SET_ATTR("m1",  &m1, "float"); 
		SET_ATTR("L0",  &L0, "float"); 
		SET_ATTR("r",   &r, "float"); 
		SET_ATTR("N",   &N, "float"); 
		SET_ATTR("tK", &tK, "float"); 
		SET_ATTR("Dreif", &Dreif, "float");
		SET_ATTR("w", &w, "float"); 
		SET_ATTR("___",   &nullstring, "func"); NOT_SAVING;		
		SET_ATTR("A",   &A, "float"); NOT_SAVING;	
		SET_ATTR("Amax", &Amax, "float"); READ_ONLY;
		SET_ATTR("accel", &accel, "float"); 
		SET_ATTR("box1",  &box1, "Pnt"); 
		SET_ATTR("box2",  &box2, "Pnt"); 
		SET_ATTR("___",   &nullstring, "func"); NOT_SAVING;		
		SET_ATTR("StartSim",  &nullstring, "func"); NOT_SAVING;
		SET_ATTR("StopSim",   &nullstring, "func"); NOT_SAVING;
		SET_ATTR("Push",      &args_push, "func"); 

		FillEntetyInfo("Geometry","no material",name);	
		bHideStdAttrs = true;
		//Object::Declare();
	}
		


    void Render(QOpenGLShaderProgram& program);
	void OnAttrChange(t_atr& a);
    void Slot1(); //timer slot
    void Slot4(); //destroy slot
protected:

};
