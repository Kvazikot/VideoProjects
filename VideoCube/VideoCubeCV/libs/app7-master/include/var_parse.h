#ifndef VARPARSE_H
#define VARPARSE_H

#include <string>
#include <vector>
#include <qopengl.h>
#include "geometry.h"
#include "param_screen_bcb.h"

using namespace std;
void gprintf(const char* fmt,...);

vector<string> VarsSplit(string str);

struct MyVar
{
	MyVar()
	{
		name = "var1";
		type = "string";
		val  = "val1";
		isInTable = false;
	}
	string name;
	string tablename;
	string val;
	string type;
	bool   isInTable;
};

struct MyTable
{
    MyTable()
    {
        char test[10];
        sprintf(test,"tab_%d", rand());
        tablename=test;
    }
    MyTable*         find(MyTable* table,string name);
    void             set_names(MyTable* table);
    string           get_prop(string name);
    MyTable*         first_child();
    string           tablename;    
    vector<MyVar>    vars;
    vector<MyTable* > children;
};

struct MyMatrix
{
	string name;
	int    m;
	int    n;
	double elem[100][100];
	MyMatrix()
	{
		for(int i=0; i<100; i++)
		for(int j=0; j<100; j++)
			elem[i][j]=0;	
	}
	string Print()
	{
		char v[255];
		string s;
		for(int i=0; i<m; i++)
		{
			s+="\r\n";
		for(int j=0; j<n; j++)
		{
			sprintf(v,"%3.3f, ",elem[i][j]);
			s+=v;
		}
		}
		//gprintf("%s = %s", name.c_str(), s.c_str()); 
		s = name + "=" + s;
		return s;

	}
};

enum Parse_Err
{
    PARSER_NO_ERROR, PARSER_EMPTY_TABLE, PARSER_PARETNESS_ERROR
};

struct VarParser
{
	vector<MyMatrix> matrixes;
	VarParser();
	string inputS;
	string outputS;
    ParamScreen vt;

	string GetVarFromStr(string varname, string s);
	void ParseString(string s, vector<MyVar>& vars);
	void ParseInput();
    void UpdatePointsStr(string& val, vector<PntEx>& pts);
	void ParsePoints3d(string& pts_s, vector<Pnt>& parr);
	void ParsePoints2d(string& pts_s, vector<Pnt>& parr);
	void ParseDoubleArray(string& pts_s, vector<double>& parr);
    void ParseDoubleArray(string& pts_s, vector<double>& parr, string sep);
    void ParseIntArray(string& pts_s, vector<int>& parr, string sep);
    Parse_Err ParseTable(string s, MyTable* out_table);
    void ParseColor(string col_s, GLfloat color[4]);
	void UpdateTable(string table_key);
	template<class M>
	int  GetMat(string name, M m, int maxN, int maxM)
	{
		MyMatrix Mat;

		for(int i=0; i<matrixes.size(); i++)
		{
			Mat = matrixes[i];
			if ( Mat.name == name ) break;
		}

		if(maxN > Mat.n ) maxN = Mat.n;
		if(maxM > Mat.m ) maxM = Mat.m;

		for(int i=0; i<maxN; i++)
			for(int j=0; j<maxN; j++)
				m[i][j] = Mat.elem[i][j];
		return 0;
	}

	template<class M>
	int  ZeroMat(M m, int maxN, int maxM)
	{
		for(int i=0; i<maxN; i++)
			for(int j=0; j<maxN; j++)
				m[i][j] = 0;
		return 0;
	}

	template<class M>
	void mprn(string name, M m)
	{
        //prn("{ %s } = ",name.c_str());
		//prn("| %0.4f  %0.4f  %0.4f  %0.4f |", m[0][0],m[1][0],m[2][0],m[3][0]);
	}
	
};


#endif
