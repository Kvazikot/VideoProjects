/***************************************************************************
 *   Copyright (C) 2006 by V. Baranoff   *
 *   vdbar@rambler.ru   *
 *   ��������� ������������ ������ � �����������
 *   �������� ��������� � ���������� ����������� Tracer
 ***************************************************************************/

#ifndef PARAMSCREEN_H
#define PARAMSCREEN_H
#include "geometry.h"
#include <map>
#include <string.h>
#include <algorithm>

using namespace std;

//������ ����������� �� �������
class TableObj
{
public:
    TableObj(){	}
    void SetTable(string t);

    string my_table;
    virtual void OnTableUpdate(){}
    virtual void AutoLoad(){}
    virtual void AutoSave(){}
};

// ������� ����������
typedef struct VarCol
{
   int i;    // ����� �������
   int num; // ���������� ����������
   bool active; // ������� (���������� ��������� �� �����)
   int  order;
   //--- �������������� ����

   // �������� ������
   double max;
   double min;

   double Wsrednee;  // ������� ��������
   double Filtered;  // ���������������
   // �. ��


   string  name; // ��� ����������
} VarCol;

enum vartypes
{
   TYPE_LONG_INT,
   TYPE_INT,
   TYPE_LONG,
   TYPE_CHAR,
   TYPE_SHORT,
   TYPE_FLOAT,
   TYPE_DOUBLE
};

// ��������
typedef struct tagPARAM
{
    unsigned short vartype;
    union
    {
       long int llVal;
       long lVal;
       char bVal;
       short iVal;
       float fltVal;
       double dblVal;
    };
} tagPARAM;

enum PSMode
{
   GROW_MODE,   // ����� ���������� (������ �������������)
   UPD_MODE,    // ������ ������������
   ACCUM_MODE  // ��� �����������, ���������� �������� ������ � ������

};

struct t_vsett
{
    int   timeout;
    bool  disabled;
    bool  always;
    string color;
    string attrpath;  // lotus.ei.coord
    string alias; //carPos -> paramMap
};


class ParamScreen
{
public:
     ParamScreen();
     //ParamScreen(int iterval = 1000, int ncols = 3, int nrows = 3);
    ~ParamScreen();

     static ParamScreen instance;
     static ParamScreen vt_instance;
     static ParamScreen& getInstance();
     static ParamScreen& getVtInstance();


     // ����� ������
     PSMode mode;
     void SetMode(PSMode _mode);
     void update();
     void WriteCell(int nsheet, int x, int y, string val);
     void CreatePCOLSFromVarColTab();
     void NewTable(string nm) { table_name = nm; tablenames.push_back(nm);}
     void DeleteTable(string nm) {
         //������� ��� �������
         vector<string>::iterator i = find(tablenames.begin(), tablenames.end(), nm);
         if(i!=tablenames.end())
            tablenames.erase(i);
         map<string, string >::iterator j;
         vector<map<string, string >::iterator> to_del;
         //������� ��� �����(����������) �������
         nm = nm + ".";
         for(j= paramMap.begin(); j!=paramMap.end(); j++)
         {
            string key = j->first;
            if(strstr(key.c_str(),nm.c_str())!=NULL)
                to_del.push_back(j);
                //paramMap.erase(key);
         }
         for(unsigned int i=0; i<to_del.size(); i++)
            paramMap.erase(to_del[i]);



     }
     void BeginTable(string nm) { table_name = nm; }
     void EndTable() { table_name = "";}
     string GetMap();
     bool hasTable(string nm)
     {
         for(unsigned int i=0; i<tablenames.size(); i++)
             if(tablenames[i] == nm) return true;
         return false;
     }



     bool hasKey(string key)
     {
         string nm ;
         if(table_name!="")
            nm = table_name + "." + key;
         else
             nm = key;
         if( paramMap.find(nm) != paramMap.end() ) return true;
         return false;
     }

     bool Set(string t, string key, string val)
     {
         if(!hasTable(t))
             return SafeSet(key, val);
         key = t + "." + key;
         paramMap[key] = val;
         OnTableUpdate(t);
     }

     map<string, vector<TableObj*> > obj_map;
     void OnTableUpdate(string t)
     {

         if( obj_map.find(t)!=obj_map.end())
         {
            vector<TableObj*>& objs = obj_map[t];
            for(unsigned int i=0; i < objs.size(); i++)
                objs[i]->OnTableUpdate();
         }


     }
     bool SafeSet(string key, string val)
     {
         string nm ;
         if(table_name!="")
            nm = table_name + "." + key;
         else
             nm = key;

         if( hasKey(nm) )
         {
             paramMap[nm] = val;
             return true;
         }
         else
         {
             AddVar(key);
             paramMap[nm] = val;
             return true;
         }
     }
     void seti(string key, int val)
     {
        char s[255];
        sprintf(s,"%d",val);
        SafeSet(key,s);
     }
     void setd(string key, double val)
     {
        char s[255];
        sprintf(s,"%f",val);
        SafeSet(key,s);
     }

     string Get(string key){return SafeGet(key);}
     bool Set(string key, string val){
         return SafeSet(key, val);

     }
     string SafeGet(string key)
     {
         if( hasKey(key) )
         {
             return paramMap[key];
         }
         else
             return "";
     }

    void GetVars(string tablename, vector<pair<string,string> >& vars);


     vector<string> matchKeys(string key)
     {
         vector<string> keys;
         for(unsigned int i=0; i<tablenames.size(); i++)
         {
             string nm = tablenames[i] + "." + key;
             if( paramMap.find(nm) != paramMap.end() )
                 keys.push_back(nm);
         }
         if( paramMap.find(key) != paramMap.end() )
                 keys.push_back(key);

         return keys;
     }


     // ����������� ��������� (� ������� �� ����)
     void reg(char* name, float val);
     void reg(char* name, double val );
     //void reg(char* name, int val);
     //void reg(char* name, complex_float val);
     void reg(char* name, char val);
     //bool reg(const char* name, float** matrix, int ncol, int nrow);
    int FindVar(string name, t_vsett& var_info, string& val);
    // ������� ���� ������� ����������
    void MakeSnapshot();
    void alarm();

     // ����� ���������� ����������
     int upd_iterval;
     void Reset();

    // ���������� ��� �������� (����� ������)
      bool upd(char* name, float val);//{return false;};
      bool upd(char* name, double val );//{return false;};
      bool upd(char* name, int val);//{return false;};
      bool upd(char* name, Pnt val);//{return false;};
      bool upd(char* name, string val);//{return false;};
    //void upd(char* name, complex_float val);
    //bool upd(const char* name, float** matrix, int ncol, int nrow);
   //bool upd(const char* name, char* val);

    //�� �� ������� ��� ���
      bool updf(char* name, float val){ return upd(name,val);}
      bool updd(char* name, double val ){ return upd(name,val);}
      bool updi(char* name, int val){ return upd(name,val);}
      bool updP(char* name, Pnt val){ return upd(name,val);}
      bool upds(char* name, string val){ return upd(name,val);}

    float getf(string name);
    double getd(string name);
    int geti(string name);

    float f(string name){return getf(name);}
    double d(string name){return getd(name);}
    int i(string name){return geti(name);}
    Pnt P(string name){return getP(name);}

    Pnt getP(string name);
    string gets(string name);
    bool getarr(string name, vector<double>& arr);
    string table_name;


    // �������� ���������
    void PrintHeader(int nsheet);

    void AddVar(string name);
    //void AddVar( char* name, bool always );
    void AddVar( char* name, string color );
    void AddAttr( char* alias, char* attrpath );


    void RemoveVar(char* name);
    void UpVar(string name);
    void DownVar(string name);
    void ActivateVar( string name );
    void DeActivateVar( string name );
    void Enable( string name );
    void Disable( string name );
    void cols_output();
    void update_timeout();
    void PrintMap();

    // ������ ���� ����������
    std::vector<string> varnames;
    std::vector<string> tablenames;

    // ������� ���������� � �������
    map<string, VarCol> VarColTab;

    // ������ ���������� �� VarCol
    std::vector<VarCol*> pcols;

    // ������� ���������� � ������  UPD_MODE
    map<string, vector<tagPARAM> > VarRegTabFixed;


    // ������� ���������� � ������  ACCUM_MODE
    map<string, vector<tagPARAM> > VarRegTab;

    // ������ ������� __��������� ��� ������������ � ���� �������__
    map<string, string > paramMap;
    map<string, t_vsett > paramSetMap;
    map<string, string > attrib2alias; // ������� �������� �������� carPos � lotus.ei.coord
    map<string, string > alias2attrib; // ������� �������� �������� carPos � lotus.ei.coord


    //������ ����� ��� ������ ���������
    vector<string> paramStr;

    //int nParams; // -- ���������� �������������� ����������
    int MaxRows; // -- ���������� ������������ ����� � ������ �������
    int MaxCols;
    int VARTIMEOUT;

    //void Slot1(CSignal& sig);


};


#endif
