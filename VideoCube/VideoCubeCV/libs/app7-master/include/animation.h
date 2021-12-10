#pragma once

#include <QElapsedTimer>
#include "interp.h"
#include "object.h"
#include "quaterion.h"
// јЌ»ћј÷»я 

//анимаци€ поворота
struct ControlRotSample{	int n_frame;	Quat quat; };
//анимаци€ позиции
struct ControlPosSample{	int n_frame;	Pnt pos; };

enum TangentType
{aauto=0, 
smooth,
linear,
step,
slow,
custom};

//ключ анимации (может быть присвоен любому аттрибуту)
struct AnimKey
{
	float time;
	float value;
	float inTangent;
	float outTangent;
    TangentType	  inTangentType;
	unsigned int	  outTangentType;	
	string	attr;
	Object* o;
};

// структура дл€ хранени€ ключей какого либо параметра
struct AttrKeys
{
	string node_name;
	string attr_name;
	vector<AnimKey> keys;
	t_atr*	a;
};





// ласс определ€ющий обход ключей
class Keys  : public Object
{
	public:
	Keys(){cur_idx = 0; cur_key = &null_key; };
	map<pair<string,string>, vector<AnimKey> > keys;
	AnimKey	null_key;
	AnimKey*	cur_key;
	int			cur_idx;
	string  nullstring;
	string  attr_names;
	string  node_names;
	int	    n_keys;
	void    AddKey(string node, string attr, AnimKey& key);
	void    SelectKey(string node, string attr, int idx, AnimKey& key);
	void	Next();
	void	Prev();
	int		GetNKeys();

	void SetAtrs()
	{
		atrs.clear();
		atrsmap.clear(); 
		SET_ATTR("node_name", &node_names, "selector");
		SET_ATTR("attr_name", &attr_names, "selector");
		SET_ATTR("Next", &nullstring, "func"); atrs.back().NotSaving(); 
		SET_ATTR("Prev", &nullstring, "func");atrs.back().NotSaving(); 
		SET_ATTR("cur_idx", &cur_idx, "int");atrs.back().NotSaving(); 
		SET_ATTR("n_keys", &n_keys, "int");
		SET_ATTR("time", &cur_key->time, "float");
		SET_ATTR("value", &cur_key->value, "float");
		SET_ATTR("inTangent", &cur_key->inTangent, "float");
		SET_ATTR("outTangent", &cur_key->outTangent, "float");
        n_keys = GetNKeys();

	}

	void Declare()
	{
	    classname = "Keys";
		cur_key = &null_key;
		cur_idx		= 0;
		isNotSaving = true;	
		SetAtrs();		
	}
	void OnAttrChange(t_atr& a);

};


// ќЅў»≈ 
typedef float (*interpfunc)(float t,  AnimKey* from_key, AnimKey* to_key);
float LinInterp(float t,  AnimKey* from_key, AnimKey* to_key);
float BezieInterp(float t,  AnimKey* from_key, AnimKey* to_key);



// состо€ние геометрического обьекта 
struct StateData
{
	Object*	 go;
	string		 state_name;
	Pnt			 coord;
	Pnt		     rot;
};

struct Clip
{
	string name;
	int	   from;
	int		to;
	bool   bReturnC0;
};

class Animations;

//структура хранени€ ключей анимации
class Animation  : public Object
{
//    Q_OBJECT
	public:
	Animations* animations;

	string node;
	Pnt   scalef;
	bool  isPlaying;
	int   cur_from, cur_to,cur_keyframe;
	Pnt	  pos,rot;
	float t_sim;
	float slow_mo;
	float rot_time, pos_time, PosTotalTime, RotTotalTime;
	double t_rascheta;  // полное врем€ анимации	
	vector<Clip>	clips;
	string			clips_str;
	vector<AnimKey>  rot_keys_x, rot_keys_y, rot_keys_z;
	vector<AnimKey>	 pos_keys_x, pos_keys_y, pos_keys_z;	
	
	
	AnimKey	null_key;
	string interp_str;
	string nullstring;	
	string play_range_args;
	string newclip_args;
	string val;

    double*	TEMP_ARR;
    double* Time_domain;

    Animation(){}
    Animation(const Animation& a)
    { val = a.val; }
	void Init(Animations* parent);
	void MakeClipsString();
	void Clear();
	void Print();
	void InterpolateDrawKeys();
	void Play();
	void Stop();
	void Reset();
	void Draw();
	void Update(double dt);
	float GetNowVal(double t, vector<AnimKey>& keys); // с учетом интерпол€ции
	float GetTotalTime(vector<AnimKey>& keys);
	AnimKey* FindPrevKey(double t, vector<AnimKey>& keys);
	AnimKey* FindNextKey(double t, vector<AnimKey>& keys);	
	void PlayClip(int from, int to);
	void NewClip(string name, int from, int to);
	void OnAttrChange(t_atr& a);
    void DrawKeys(vector<AnimKey>& keys, float dt, TangentType interpolant, int nChart, int nSer);
    void InterpolateKeys(vector<AnimKey>& keys, interpfunc fun, double* ARR, float dt, int& N);
	void Declare()
	{
	    classname = "Animation";
		name = node+"_animation";
		SetName(name);		
		SET_ATTR("CurrentClip", &clips_str, "selector");
		SET_ATTR("Interpolation", &interp_str, "selector");
		SET_ATTR("Play", &play_range_args, "func"); atrs.back().NotSaving(); 
		SET_ATTR("Rewind", &play_range_args, "func");atrs.back().NotSaving(); 
		SET_ATTR("Stop", &play_range_args, "func");atrs.back().NotSaving(); 
		SET_ATTR("NewClip", &newclip_args, "func");atrs.back().NotSaving(); 
		SET_ATTR("from", &cur_from, "int");
		SET_ATTR("to", &cur_to, "int");
		SET_ATTR("cur_keyframe", &cur_keyframe, "int");
		SET_ATTR("t_sim", &t_sim, "float");atrs.back().ReadOnly();
		SET_ATTR("slow_mo", &slow_mo, "float");
		SET_ATTR("_____", &nullstring, "string"); atrs.back().NotSaving(); 
		SET_ATTR("pos", &pos, "Pnt");
		SET_ATTR("rot", &rot, "Pnt");
		SET_ATTR("RotTotalTime", &RotTotalTime, "float");
		SET_ATTR("PosTotalTime", &PosTotalTime, "float");
		
		
		SET_ATTR("isPlaying", &isPlaying, "bool"); atrs.back().ReadOnly(); atrs.back().NotSaving(); 
		
	}

	Object*	o;
	vector<ControlRotSample>			 rot_samples; // дл€ совместимости 
	vector<ControlPosSample>			 pos_samples; // дл€ совместимости
	Spline*					 rot_spline[4];
	Spline*					 pos_spline[3];

};


//анимации обьектов
class Animations : public Object
{
 //   Q_OBJECT
public:
	vector<Animation*> animations;
	Keys*			   keys;

	string			  strs[20000];
	int n;


	
	Animations();
    Animations(const Animations& a)
    { n = a.n; }
	
	Animation* FindAnimFor(string nodename);

	double t_rascheta;  // полное врем€ анимации
	double t_sim;
	double _t2, _t1, dt, _freq;
    QElapsedTimer t;
	
	void AddAnimation(Animation* a);
	void OnAttrChange(t_atr& a);
	void OnAttrSelect(t_atr& a);
	void Save(string& s);
	void Load(string& s);
    void Slot1();
	void Calc();
    void Declare();
  
};
