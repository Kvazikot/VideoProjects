#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <stdarg.h>
using namespace std;
/*
Сделать класс AssotiationMap
и Assotiation
в нем вектор векторов ассоциированных обьектов (void ссылки)
Cписок названий классов в ассоциации в порядке добавления
функция найти ассоциацию FindAssociation(void*)
функция добавить ассоциацию AddToAssociation()
функция удалить обьект из всех ассоциаций RemoveFromAll()
Например для триады CFastLineSeries TLaw Spline Curve
*/
class Assotiation
{
public:
	vector<void*> ptrs;
	vector<string> classnames;
	vector<string> objnames;
	string key;
	int	   id;
	void AddN(int n,...) 
	{ 
		void* val;
		va_list ap;	
		va_start(ap,n);
		for (int i=0;i<n;i++)
		{
			val=va_arg(ap,void*);
			Add(val);
			//printf (" [%.2f]",val);
		}
	    va_end(ap);

	}


	void Add(void* p) 
	{ 
		if( isExist(p) == -1)
		{
			ptrs.push_back(p);
			classnames.push_back("unknow");
			objnames.push_back("unknow");
		}
	}

	void Add(void* p, string clas, string objname) 
	{ 
		if( isExist(p) == -1)
		{
			ptrs.push_back(p);
			classnames.push_back(clas);
			objnames.push_back(objname);
		}
	}
	bool Remove(void* p)
	{
        for(int i=0; i<(int)ptrs.size(); i++)
		{
			if( p == ptrs[i])
			{
				ptrs.erase(ptrs.begin()+i);
				classnames.erase(classnames.begin()+i);
				objnames.erase(objnames.begin()+i);
				return 1;
			}		
		}
		return 0;
	}
	bool RemoveClass(string clas)
	{
        for(int i=0; i<(int)classnames.size(); i++)
		{
			if( clas == classnames[i])
			{
				ptrs.erase(ptrs.begin()+i);
				classnames.erase(classnames.begin()+i);
				objnames.erase(objnames.begin()+i);
				return 1;
				
			}		
		}
		return 0;
	}

	bool RemoveObj(string name)
	{
        for(int i=0; i<(int)objnames.size(); i++)
		{
			if( name == objnames[i])
			{
				ptrs.erase(ptrs.begin()+i);
				classnames.erase(classnames.begin()+i);
				objnames.erase(objnames.begin()+i);
				return 1;
				
			}		
		}
		return 0;
	}


	void Clear()
	{
		ptrs.clear();
		classnames.clear();
		objnames.clear();
	}
	//-1 если нет, индекс в векторе если да
	int isExist(void* p)
	{
        for(int i=0; i<(int)ptrs.size(); i++)
			if( p == ptrs[i])
				return i;
		return -1;	
	}
	int FindPtr(void* p) {return isExist(p);}
	int FindClass(string classname) 
	{
        for(int i=0; i<(int)classnames.size(); i++)
			if( classname == classnames[i])
				return i;
		return -1;	
	}
	bool isNull(){ return  (ptrs.size()==0);}
	string toStr()
	{
		string res;
		char gstr[20];
        for(int i=0; i<(int)classnames.size(); i++)
		{
			sprintf(gstr,"%x ",ptrs[i]);
			res+=gstr + objnames[i] + "(" + classnames[i]+"),";
		}
		return res;
	}
};



class AssotiationMap
{
public:
	Assotiation NullAssotiation;
	vector<Assotiation> ass;
	Assotiation* NewAssotiation(string k)
	{
		Assotiation a;
		std::transform(k.begin(), k.end(), k.begin(), ::tolower);
		a = NullAssotiation;
		a.key = k;
		ass.push_back(a);
		return &ass.back();
	}
	void RemoveFromAll(void* obj)
	{	
        for(int i=0; i < (int)ass.size(); i++)
			ass[i].Remove(obj);		
	}
	void DeleteAll() { ass.clear();}
	Assotiation* FindAssociation(string key)
	{
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        for(int i=0; i < (int)ass.size(); i++)
		    if( ass[i].key == key )
				return &ass[i];
		return &NullAssotiation;
	}
	vector<Assotiation> FindAssociations(void* p)
	{
		vector<Assotiation> res;
        for(int i=0; i < (int)ass.size(); i++)
		    if( ass[i].isExist( p ) )
				res.push_back( ass[i] );	
		return res;
	}
	template<class T>
	T* Assign(string key, int n) 
	{
		T* p;
		Assotiation* a = FindAssociation(key);
        if( (int)a->ptrs.size() > n )
			p = (T*)a->ptrs[n];
		else
			p = 0;
		return p;
	}
	string PrintInfo()
	{
		string res;
		
        for(int i=0; i < (int)ass.size(); i++)
		{
			res+="\r\nAssociation " + ass[i].key + ":";
			res+=ass[i].toStr() + "\r\n";
		}
		return res;		
	}
	//vector<Assotiation> FindAssociations(void*, vector<string> class_order);
	//порядковый номер обьекта в ассоциации
	//vector<Assotiation> FindAssociations(void*, int n);

};
