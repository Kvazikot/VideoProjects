/**********************************************************************
 *  Project App6
 *
 *  animation.cpp
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

#include "../include/interp.h"
#include "../include/quaterion.h"
#include "../include/animation.h"
#include "../include/str_utils.h"
#include "../include/cobjregistry.h"
#include "../include/param_screen_bcb.h"
#include "../include/flags.h"
#include "../include/gemobject.h"
#include "../include/objectinspector.h"
#include "../include/print.h"

extern t_flags flags;
static CObjRegistry& MREG = CObjRegistry::getInstance2();
static ParamScreen& pscr = ParamScreen::getInstance();
//static ObjectInspector& inspector2 = ObjectInspector::getInstance2();

//void CalcSpline(Spline* spl, int Ni, double* td, double* arr)
//{
//	spl->SetN(Ni);
//    spl->SetXY(td, arr);
//	spl->Calc();
//}

Animations::Animations()
{
	isNotSaving = true;
	n = 0;

    t.start();
	t_sim = 0;
	Connect();
	
	
	
}

void Animation::NewClip(string name, int from, int to)
{
	Clip clip;
	clip.name = name;
	clip.from = from;
	clip.to = to;
	clips.push_back(clip);
	MakeClipsString();
}

void Animation::MakeClipsString()
{
	string s;
    for(int i=0; i<(int)clips.size(); i++)
		s = s + clips[i].name + " ";		
	clips_str = s;
}

void Animation::Play()
{
	isPlaying = true;
	PosTotalTime = GetTotalTime(pos_keys_x);
	RotTotalTime = GetTotalTime(rot_keys_x);
}


void Animation::Stop()
{
	isPlaying = false;
}


void Animation::Reset()
{
	t_sim = 0;
	rot_time = 0;
	pos_time = 0;
	t_rascheta = 0;
}

//инициализация анимации
void Animation::Init(Animations* parent)
{
	animations = parent;
    TEMP_ARR = (double*)malloc(200000*sizeof(double));
    Time_domain  = (double*)malloc(200000*sizeof(double));
	o = 0;
	null_key.time = 0;  
	null_key.value = 0;
	pos = Pnt(0,0,0); 
	rot = Pnt(0,0,0);
    slow_mo = 0.001f;
	cur_keyframe = 0;
	cur_from = 0;
	t_sim = 0;
	rot_time = 0;
	pos_time = 0;
	t_rascheta = 0;
	isPlaying = false;
	cur_to = max(pos_keys_x.size(),rot_keys_x.size());
	pscr.AddVar("t_sim");
	interp_str = "Linear Cubic_Spline Auto";
	for(int i=0; i<4; i++)
		rot_spline[i] = new Spline();
	for(int i=0; i<3; i++)
		pos_spline[i] = new Spline();
	clips.clear();
	NewClip("All_sequence",0,cur_to);
}

void Animation::OnAttrChange(t_atr& a)
{
	//if(isLoading ) return;
	if (a.name == "Play" ) Play();
	if (a.name == "Stop" ) Stop();
	if (a.name == "Rewind" ) Reset();	
	if (a.name == "NewClip" ) NewClip(newclip_args, cur_from, cur_to);

}

//object inspector event
void Animations::OnAttrChange(t_atr& a)
{
    prn("Animation::OnAttrChange %s", a.name.c_str());
	
	if(isLoading ) return;
	if (a.name == "Фильтр" )
	{	
	}

	Animation* anim = FindAnimFor(a.name);
	if(anim!=NULL)
	{
		//prn("rots = %d pos = %d t_rascheta = %f", anim->rot_samples.size(), anim->pos_samples.size(), anim->t_rascheta);
        //page4->Clear();
		anim->InterpolateDrawKeys();
		anim->Draw();
		if( anim->isPlaying )
		{
			anim->Reset();
			anim->Stop();
		}
		else
			anim->Play();
		//for(int i=0; i<anim->pos_keys_x.size(); i++)
		//	prn("t = %f pos = %f %f %f",anim->pos_keys_x[i].time, anim->pos_keys_x[i].value,anim->pos_keys_y[i].value,anim->pos_keys_z[i].value);

	}	
}

AnimKey* Animation::FindPrevKey(double t, vector<AnimKey>& keys)
{
    for(int i=0; i < (int)keys.size()-1; i++)
	{
		if( ( keys[i].time < t ) && ( keys[i+1].time > t ))
			return &keys[i];
	}
	return &null_key;
}

AnimKey* Animation::FindNextKey(double t, vector<AnimKey>& keys)
{
    for(int i=0; i < (int)keys.size()-1; i++)
	{
		if( ( keys[i].time < t ) && ( keys[i+1].time > t ))
			return &keys[i+1];
	}
	return &null_key;
}


float Animation::GetTotalTime(vector<AnimKey>& keys)
{
	if(keys.size()>0)
		return keys.back().time;
	else return 0;
}

float Animation::GetNowVal(double t, vector<AnimKey>& keys)
{
	if(keys.size()==0) return 0;
	AnimKey* k0 = FindPrevKey(t, keys);
	AnimKey* k1 = FindNextKey(t, keys);	
	if(k0->time > k1->time) return 0;
	if(k0->time == k1->time) return 0;
	//if(k0->time == 0)  return 0;
	//if(k1->time == 0)  return 0;
	//if( k0->inTangentType == TangentType::linear )  //линейный
	return LinInterp(t, k0, k1); 
    if( k0->inTangentType == custom ) // безье интерполянт
		return BezieInterp(t, k0, k1);

}
void Animation::InterpolateKeys(vector<AnimKey>& keys, interpfunc fun, double* ARR, float dt, int& N)
{
	float t;		
	t = keys[0].time;
	int j = 0;
    for(int i=0; i < (int)keys.size()-1; i++)
	{
		AnimKey k0, k1;
		k0 = keys[i];
		k1 = keys[i+1];			
		t = k0.time;
		while(t < k1.time)
		{
			Time_domain[j] = t;
			ARR[j] =  fun(t, &k0, &k1);
			t+=dt; j++;
		}				
	}
	N = j; // количество отчетов получившихся в результате
	
}



void Animation::DrawKeys(vector<AnimKey>& keys, float dt, TangentType interpolant, int nChart, int nSer)
{
	if(keys.size()==0) return;

    nChart = 0;
    nSer = 0;

	if(interpolant == aauto)
	{
		//рисовать по полю структуры inTangent
	}
	
	int N;

	if(interpolant == custom) //сплайн
        InterpolateKeys(keys, BezieInterp, TEMP_ARR, dt, N);

	if(interpolant == linear) //линейный
        InterpolateKeys(keys, LinInterp, TEMP_ARR, dt, N);
	
    //for(int i = 0; i < N; i++)			page4->AddXY(nChart, nSer, i*dt, ARR[i]);
}

// расчет интерполяционных сплайнов и времени анимации
void Animation::InterpolateDrawKeys()
{

	string s1 = node+" " + IntToStr(rot_samples.size()) + " ROT samples" ;
	string s2  =node+" " + IntToStr(pos_samples.size()) + " POS samples";
//	page4->SetTitles(s1,s2);
//	page4->SetSerName(0, 0, "rot_x");
//	page4->SetSerName(0, 1, "rot_y");
//	page4->SetSerName(0, 2, "rot_z");
//	page4->SetSerName(1, 0, "pos_x");
//	page4->SetSerName(1, 1, "pos_y");
//	page4->SetSerName(1, 2, "pos_z");
	//	Cubic Spline
	TangentType typ;
	if( GET_ATTR("Interpolation")->data == 0)  typ = linear;
	if( GET_ATTR("Interpolation")->data == 1)  typ = custom;
    float dt = 0.01f;
	DrawKeys(rot_keys_x, dt, typ, 0, 0);
	DrawKeys(rot_keys_y, dt, typ, 0, 1);
	DrawKeys(rot_keys_z, dt, typ, 0, 2);
	DrawKeys(pos_keys_x, dt, typ, 1, 1);
	DrawKeys(pos_keys_y, dt, typ, 1, 2);
	DrawKeys(pos_keys_z, dt, typ, 1, 3);


}

// лин. интерполяция 
float LinInterp(float t, AnimKey* from_key, AnimKey* to_key)
{
	float t0 = from_key->time;
	float t1 = to_key->time;
	float tm = t1 - t0;
	float tt = (t - t0) / tm;
	float v = from_key->value + tt * ( to_key->value - from_key->value);
	return v;
}

// кубические сплайны по методу Эрмитта
float BezieInterp(float t, AnimKey* from_key, AnimKey* to_key)
{
	//http://masters.donntu.edu.ua/2003/ggeo/burik/library/spline1.htm
    float P1,P2,R1,R2;
	float t0 = from_key->time;
	float t1 = to_key->time;
	float tm = t1 - t0;
	float tt = (t - t0) / tm;
	t = tt;
	P1 = from_key->value;
	P2 = to_key->value;
	R1 = from_key->inTangent;
	R2 = from_key->outTangent; 
	return P1 * (2*t*t*t-3*t*t+1) + P2 * (-2*t*t*t+3*t*t) + R1 * (t*t*t-2*t*t+t) + R2 * (t*t*t-t*t); 
}


void Animation::Draw()
{

	
}


void Animation::Clear()
{
	rot_samples.clear();
	pos_samples.clear();
}

void Animation::Print()
{
    RotTotalTime = GetTotalTime(rot_keys_x);
	val="N_rot=" + IntToStr(rot_keys_x.size()) ;
	val+=" N_pos=" + IntToStr(pos_keys_x.size()) ;
    val+=" TotalTime=" + DoubleToStr(RotTotalTime) + " sec";
}

Animation* Animations::FindAnimFor(string nodename)
{
    for(int i=0;  i<(int)animations.size(); i++)
	{
		if( animations[i]->node  == nodename )
			return animations[i];
	}
	return 0;
}


void Animations::AddAnimation(Animation* a)
{
	a->Init(this);
	animations.push_back(a);
	a->Print();	
	//a->InterpolateDrawKeys();
	a->Declare();

	strs[animations.size()] = a->val;
	SET_ATTR(a->node, &strs[animations.size()], "string"); atrs.back().ReadOnly();  atrs.back().NotSaving();
}

void Animations::Declare()
{
	isNotSaving = false;
    classname = "Animations";
	name = "animations";
	SetName(name);
//		SET_OBJECT(this,(char*)name.c_str());
    SET_ATTR("n", &n, "int");
}

void Animations::Save(string& s)
{	
    for(int i=0; i<(int)animations.size();i++)
	{
		Animation* a = animations[i];
		if(a->clips.size()>0)
		{
			s = a->node + "_clips=";
            for(int j=0; j < (int)a->clips.size();j++)
				s=s+a->clips[j].name+" ";
		}
		if(a->clips.size()>0)
		{
			s = a->node + "_ranges=";
            for(int j=0; j < (int)a->clips.size();j++)
				s=s+IntToStr(a->clips[j].from)+"-"+IntToStr(a->clips[j].to)+" ";
		}

	}
}


void  SeekNext2(FILE* f, unsigned char sig, int Ntimes)
{
	int count = 0;
	while( !feof(f) )
	{
		unsigned char c;
		fread(&c, 1, 1, f);
		if( c == sig )
		  count++;
		else
	  	  count=0;
		if(count == Ntimes) 
			break;
	}
	
}


void ReadKey(FILE* f, AnimKey& key)
{	
	fread(&key.time, sizeof(float), 1, f);
	fread(&key.value, sizeof(float), 1, f);
	fread(&key.inTangent, sizeof(float), 1, f);
	fread(&key.outTangent, sizeof(float), 1, f);
	fread(&key.inTangentType, sizeof(unsigned int), 1, f);
	fread(&key.outTangentType, sizeof(unsigned int), 1, f);
	key.time = key.time / 30.;
}


void   Keys::SelectKey(string node, string attr, int idx, AnimKey& key)
{
	key = null_key;
	//keys[].size();
	if(keys.find(make_pair(node,attr))==keys.end())
		return;
    if(idx > (int)keys[make_pair(node,attr)].size() )
		return;
	key = keys[make_pair(node,attr)][idx];
}


int		Keys::GetNKeys()
{
	string node = GetComboString("node_name");
	string attr = GetComboString("attr_name");
	if(keys.find(make_pair(node,attr))==keys.end())
		return 0;
	vector<AnimKey> vec = keys[make_pair(node,attr)];
	return keys[make_pair(node,attr)].size();
}

void Keys::Prev()
{
    if(--cur_idx<0)cur_idx=0;
    string node_name = GetComboString("node_name");
    string attr_name = GetComboString("attr_name");
    SelectKey(node_name, attr_name, cur_idx, *cur_key);
    prn("%f", cur_key->value);
    //SetAtrs();
    n_keys = GetNKeys();
    flags.bNeedReinitObjInspector = true;
}

void Keys::Next()
{
	++cur_idx;
    prn("cur_idx %d", cur_idx);
	n_keys = GetNKeys();
    if(cur_idx>=n_keys) cur_idx=n_keys-1;
	string node_name = GetComboString("node_name");
	string attr_name = GetComboString("attr_name");
	SelectKey(node_name, attr_name, cur_idx, *cur_key);
	prn("%f", cur_key->value);
//	SetAtrs();	
    flags.bNeedReinitObjInspector = true;
}

void Keys::AddKey(string node, string attr, AnimKey& key)
{
	Object* o = MREG.DeepFindObj(node);
	key.attr = attr;
	if(o!=0)
		key.o = o;
	keys[make_pair(node,attr)].push_back(key);
	if (node_names.find(node)== string::npos ) 
		node_names+=node + " ";
	if (attr_names.find(attr)== string::npos ) 
		attr_names+=attr + " ";
}

void Keys::OnAttrChange(t_atr& a)
{
	if (a.name == "Next" ) Next();	if (a.name == "Prev" ) Prev();
	if (a.name == "node_name" ) { cur_idx=0; Prev(); }
	if (a.name == "attr_name" ) { cur_idx=0; Prev(); }
}

void Animations::Load(string& s)
{
	Animation a;

    s = s;
	string fn =  "animations.anim";
	FILE* f = fopen(fn.c_str(), "rb");
	if(f == NULL) return;
	
	keys = new Keys();	
	keys->SetName("keys");
	keys->Declare();
	MREG.AddObj(keys);
	pscr.AddVar("anim_t_sim");

    char gstr[512];
	for(int j=0;  j<n; j++)
	{
		Animation* a = new Animation();
		
		fread(gstr, 1, 256, f); 
		a->node = gstr;

		int size=0; 
		
		//rot keys
		fread(&size, sizeof(int), 1, f);
		for(int i=0; i<size;i++)
		{
			AnimKey k; ReadKey(f,k); keys->AddKey(a->node, "rotx", k);
		

			a->rot_keys_x.push_back(k);
		}
		for(int i=0; i<size;i++)
		{
			AnimKey k; ReadKey(f,k); keys->AddKey(a->node, "roty", k);
			a->rot_keys_y.push_back(k);
		}
		for(int i=0; i<size;i++)
		{
			AnimKey k; ReadKey(f,k); keys->AddKey(a->node, "rotz", k);
			a->rot_keys_z.push_back(k);
		}

		//pos keys
		size=0; 
		fread(&size, sizeof(int), 1, f);
		for(int i=0; i<size;i++)
		{
			AnimKey k; ReadKey(f,k); keys->AddKey(a->node, "posx", k);
			a->pos_keys_x.push_back(k);
		}
		for(int i=0; i<size;i++)
		{
			AnimKey k; ReadKey(f,k); keys->AddKey(a->node, "posy", k);
			a->pos_keys_y.push_back(k);
		}
		for(int i=0; i<size;i++)
		{
			AnimKey k; ReadKey(f,k); keys->AddKey(a->node, "posz", k);
			a->pos_keys_z.push_back(k);
		}

		AddAnimation(a);

		SeekNext2(f, 0xFA, 8);

	}
	fclose(f);


	
}

//расчеты по всему набору анимаций
void Animations::Calc()
{
	
}


// РЕАЛЬНО КОД АНИМАЦИИ
void Animations::Slot1()
{
	//return;
	if(animations.size()==0) return;
	//prn("%f", gtimer.fsec);
	 // Делаем цикл симуляции   
     dt=(double)(t.elapsed())/(double)(1000);
	 t_sim+=dt;
	 pscr.updd("anim_t_sim", t_sim);

	 //page4->DrawTimeSlider(t_sim);
    for(int i=0;  i<(int)animations.size(); i++)
	{
		Animation* a = animations[i];
		a->Update(dt);
	}
/*		
		Pnt pos;
		Pnt rot;
		pos =	a->pos_spline[0]->GetF(t_sim);
		pos.y =	a->pos_spline[1]->GetF(t_sim);		
		pos.z =	a->pos_spline[2]->GetF(t_sim);
		rot =	a->rot_spline[0]->GetF(t_sim);
		rot.y =	a->rot_spline[1]->GetF(t_sim);		
		rot.z =	a->rot_spline[2]->GetF(t_sim);

		*/
	

	if( t_sim > t_rascheta)
		t_sim = 0;
	
    t.restart();
}

float CorrAngleAt90(float& a)
{
	if ( ( a > 89 ) && ( a < 91 ) )
        a+=0.1f;

/*	if ( ( a > -1 ) && ( a < 1 ) )
		a+=pp;

	if ( ( a > 179 ) && ( a < 181 ) )
		a+=pp;
*/
	return a;
}

void Animation::Update(double dt)
{
		if(isPlaying)
		{
			t_sim+=(dt*slow_mo);
			rot_time+=(dt*slow_mo);
			pos_time+=(dt*slow_mo);
			if( rot_time > GetTotalTime(rot_keys_x) ) rot_time = 0; // loop rot animation
			if( pos_time > GetTotalTime(pos_keys_x) ) pos_time = 0; // loop pos animation
			
			if(o==NULL)
			{
				o = MREG.DeepFindObj(node);
                if(o == 0) return;
				Object* group = MREG.FindObj(o->groupname);
				scalef = group->ei.scale;
				
			}
			else
			{
				GemObject* go = (GemObject*)o;		

				// ИНТЕРПОЛЯЦИЯ ПОВОРОТА И ПОЗИЦИИ
				float tm = GetTotalTime(rot_keys_x);
				rot.x = GetNowVal(rot_time, rot_keys_x);
				//rot.x = 0;
				rot.y = GetNowVal(rot_time, rot_keys_y);
				rot.z = GetNowVal(rot_time, rot_keys_z);

				//CorrAngleAt90(rot.x);

				//float r = 180*rand()/RAND_MAX;
				//rot   = Pnt(180*sin(20 * GR * rot_time),0,0);
				tm = GetTotalTime(pos_keys_x);
				pos.x = GetNowVal(pos_time, pos_keys_x);
				pos.y = GetNowVal(pos_time, pos_keys_y);
				pos.z = GetNowVal(pos_time, pos_keys_z);
				
				// ПРИСВОЕНИЕ ЗНАЧЕНИЯ

				go->rot = Pnt(rot.x, rot.y, rot.z);//rot;				
				go->ei.coord = Pnt( pos.x,  pos.z, pos.y);
				//prn();
				//go->C0 = Pnt(pos.x, pos.z, pos.y);
				//go->rot = rot;

				//if( name == "Pyramid01")
			/*	prn("rot_time=%f pos_time=%f pos=%s rot=%s name=%s",rot_time,pos_time,
													pos.toStr().c_str(),
													rot.toStr().c_str(),
													name.c_str());
													*/
			   //  prn("%f rot=%s", rot_time, rot.toStr().c_str() );
				//prn("%f pos=%s", pos_time, go->ei.coord.toStr().c_str() );

			
			}
		}

}

//выбран обьект анимации 
void Animations::OnAttrSelect(t_atr& a)
{
    for(int i=0;  i<(int)animations.size(); i++)
	{
		
		Animation* aa = animations[i];
		if( a.name == aa->node )
		{
			if(aa->o!=NULL)
				aa->o->isSelected = 2;
			else
			{
				aa->o = MREG.DeepFindObj(aa->node);
				if(aa->o!=NULL)
					aa->o->isSelected = 2;
			}

            //inspector2.GetObjectDetails(aa);

		}
	}

	
}
