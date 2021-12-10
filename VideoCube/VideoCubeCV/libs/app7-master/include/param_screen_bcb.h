/***************************************************************************
 *   Copyright (C) 2006 by V. Baranoff   *
 *   vdbar@rambler.ru   *
 *   Интерфейс обновляемого экрана с параметрами
 *   Работает совместно с програмным компонентом Tracer
 ***************************************************************************/

#ifndef PARAMSCREEN_H
#define PARAMSCREEN_H
#include "geometry.h"
#include <map>
#include <string.h>
#include <algorithm>

using namespace std;

//обьект загружаемый из таблицы
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

// колонка переменных
typedef struct VarCol
{
   int i;    // номер колонки
   int num; // количество накоплений
   bool active; // активна (переменная выводится на экран)
   int  order;
   //--- дополнительные поля

   // диапазон вывода
   double max;
   double min;

   double Wsrednee;  // среднее значение
   double Filtered;  // отфильтрованное
   // и. др


   string  name; // имя переменной
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

// параметр
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
   GROW_MODE,   // режим приращения (строки накапливаются)
   UPD_MODE,    // строки обноволяются
   ACCUM_MODE  // нет отображения, переменные кладутся только в память

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


     // режим работы
     PSMode mode;
     void SetMode(PSMode _mode);
     void update();
     void WriteCell(int nsheet, int x, int y, string val);
     void CreatePCOLSFromVarColTab();
     void NewTable(string nm) { table_name = nm; tablenames.push_back(nm);}
     void DeleteTable(string nm) {
         //удалить имя таблицы
         vector<string>::iterator i = find(tablenames.begin(), tablenames.end(), nm);
         if(i!=tablenames.end())
            tablenames.erase(i);
         map<string, string >::iterator j;
         vector<map<string, string >::iterator> to_del;
         //удалить все ключи(переменные) таблицы
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


     // регистрация параметра (к примеру на флеш)
     void reg(char* name, float val);
     void reg(char* name, double val );
     //void reg(char* name, int val);
     //void reg(char* name, complex_float val);
     void reg(char* name, char val);
     //bool reg(const char* name, float** matrix, int ncol, int nrow);
    int FindVar(string name, t_vsett& var_info, string& val);
    // Снепшот всей таблицы переменных
    void MakeSnapshot();
    void alarm();

     // время обновления параметров
     int upd_iterval;
     void Reset();

    // обновление его значения (режим экрана)
      bool upd(char* name, float val);//{return false;};
      bool upd(char* name, double val );//{return false;};
      bool upd(char* name, int val);//{return false;};
      bool upd(char* name, Pnt val);//{return false;};
      bool upd(char* name, string val);//{return false;};
    //void upd(char* name, complex_float val);
    //bool upd(const char* name, float** matrix, int ncol, int nrow);
   //bool upd(const char* name, char* val);

    //те же функции для луа
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


    // печатать заголовок
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

    // список имен переменных
    std::vector<string> varnames;
    std::vector<string> tablenames;

    // таблица информации о колонке
    map<string, VarCol> VarColTab;

    // массив указателей на VarCol
    std::vector<VarCol*> pcols;

    // таблица ПЕРЕМЕННЫХ В РЕЖИМЕ  UPD_MODE
    map<string, vector<tagPARAM> > VarRegTabFixed;


    // таблица ПЕРЕМЕННЫХ В РЕЖИМЕ  ACCUM_MODE
    map<string, vector<tagPARAM> > VarRegTab;

    // старая таблица __оставлено для совместимоти с пред версией__
    map<string, string > paramMap;
    map<string, t_vsett > paramSetMap;
    map<string, string > attrib2alias; // таблица перевода например carPos в lotus.ei.coord
    map<string, string > alias2attrib; // таблица перевода например carPos в lotus.ei.coord


    //массив строк для вывода построчно
    vector<string> paramStr;

    //int nParams; // -- количество регистрируемых переменных
    int MaxRows; // -- количество отображаемых строк в режиме апдейта
    int MaxCols;
    int VARTIMEOUT;

    //void Slot1(CSignal& sig);


};


#endif
