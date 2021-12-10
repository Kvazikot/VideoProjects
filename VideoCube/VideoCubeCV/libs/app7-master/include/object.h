#ifndef OBJECT_H
#define OBJECT_H

#include "geometry.h"
#include "str_utils.h"
#include <string>
#include <map>
#include <QObject>
//#include <QtOpenGL>
//#include <Gl/GL.h>

#define NOT_SAVING atrs.back().NotSaving()
#define SET_STD    atrs.back().SetStd()
#define READ_ONLY  atrs.back().ReadOnly();
#define HIDE	   atrs.back().Hide();
#define ATTR	   SET_ATTR
#define ATTR_EXIST(a) atrsmap.find("a")!=atrsmap.end()
#define ATTR_HIDE(a)  atrs[atrsmap["a"]].Hide()
#define ATTR_UNHIDE(a)  atrs[atrsmap["a"]].Unhide()


enum TYPES_ENUM
{
    T_BOOL,
    T_INT,
    T_FLOAT,
    T_CSTR,
    T_SELECT
};


// TODO: не учитывается наследование классов
// нужно сделать GetParentClass
typedef struct EntetyInfo
{
    string model;
    string classname;
    string GUID;
    string groupname;
    string name; // same as Object->name
    string material;
    string texture_name;
    float  wire_color[4];

    int    texture_id;
    Pnt    coord;
    Pnt    coord0;
    Pnt    rot;
    Pnt    scale;
}EntetyInfo;


typedef struct tagTYPE
{
    int type;
    union
    {
        float flt;
        char  cstr[200];
        bool  flag;
        int	  i;

    };
}tagTYPE;

//описание метода класса
struct meta_entry
{
    void* meth_addr;
    char  parentclass[200];
    char  classname[200];
    char  methname[200];
    tagTYPE arg[100];
    int		   nargs;
};



class Object;

// аттрибут обьекта
struct t_atr
{
    string name;
    void* addr;
    int    data;
    string data2;
    float   min;
    float   max;
    float   step;
    Object* pobj;
    string type;
    long   flags;
    //string expression;
    int    idx;

    t_atr()
    {
        flags = 0;
    }
    bool isNULL();
    string toString();
    void   fromString(string strval);


    void Watch()
    {
        SET_BIT(flags, 0);
    }

    bool isWatched()
    {
        return CHECK_BIT(flags, 0);
    }

    void Unwatch()
    {
        CLEAR_BIT(flags, 0);
    }


    void Hide()
    {
        SET_BIT(flags, 1);
    }

    bool isHidden()
    {
        if(CHECK_BIT(flags, 1))
            return true;
        else
            return false;
    }

    void Unhide()
    {
        CLEAR_BIT(flags, 1);
    }

    void ReadOnly()
    {
        SET_BIT(flags, 2);
    }

    bool isReadOnly()
    {
        if( CHECK_BIT(flags, 2) )
            return true;
        else
            return false;
    }

    void NotSaving()
    {
        SET_BIT(flags, 3);
    }

    bool isNotSaving()
    {
        if( CHECK_BIT(flags, 3)!=0 )
            return true;
        else
            return false;
    }



    void Select()
    {
        SET_BIT(flags, 4);
    }

    void UnSelect()
    {
        CLEAR_BIT(flags, 4);
    }


    bool isSelected()
    {
        if(CHECK_BIT(flags, 4))
            return true;
        else
            return false;
    }

    void SetStd()
    {
        SET_BIT(flags, 5);
    }

    bool isStd()
    {
        if( CHECK_BIT(flags, 5))
            return true;
        else
            return false;
    }

    void SetPy()
    {
        SET_BIT(flags, 6);
    }

    bool isPy()
    {
       CHECK_BIT(flags, 6)?true:false;
    }

};

struct SimplePnt
{
    float x,y,z;
};

//значение аттрибута
struct t_val
{
    t_atr* atr;
    union
    {
        char str[1024];
        int    iVal;
        float  fVal;
        double dVal;
        bool   boolVal;
        SimplePnt    pntVal;
    };

};

// базовый абстрактный класс
class Object : public QObject
{
 //Q_OBJECT
public:
      Object();

      Object(Object& B)
      {

          classname = B.classname;
          livetime = B.livetime;

          connected = B.connected;
          ei = B.ei;	// информация о ентети

          isEntety = B.isEntety;
          isNotSaving = B.isNotSaving;
          isCanRender = B.isCanRender;
          isSelected = B.isSelected;
          isHidden = B.isHidden;
          m_Updated = B.m_Updated;
          bounds = B.bounds; //размеры
          C0 = B.C0;     //центр
          groupname = B.groupname;

          isGroup = B.isGroup;
          updatescript = B.updatescript;	//скрипт для обновления параметров
          initscript = B.initscript;    //скрипт для инициализации
          bounds = B.bounds; //размеры
          C0 = B.C0;     //центр
          mass = B.mass;	//масса
          kurs0 = B.kurs0; //курс радиус вектор из C0
          v = B.v;    //мгновенная скорость

          SetName(B.name);
      }

          virtual Object& operator=(Object& A)
      {

          classname = A.classname;
          livetime = A.livetime;

          connected = A.connected;
          ei = A.ei;	// информация о ентети
          isEntety = A.isEntety;
          isNotSaving = A.isNotSaving;
          isCanRender = A.isCanRender;
          isSelected = A.isSelected;
          isHidden = A.isHidden;
          m_Updated = A.m_Updated;
          bounds = A.bounds; //размеры
          C0 = A.C0;     //центр
          groupname = A.groupname;

          isGroup = A.isGroup;
          updatescript = A.updatescript;	//скрипт для обновления параметров
          initscript = A.initscript;    //скрипт для инициализации
          bounds = A.bounds; //размеры
          C0 = A.C0;     //центр
          mass = A.mass;	//масса
          kurs0 = A.kurs0; //курс радиус вектор из C0
          v = A.v;    //мгновенная скорость

          SetName(A.name);
                  return *this;
      }


     void Connect();

     void SetPyAttr(string aname, string type, string val);
     void SetPyFunc(string fname);
     vector<t_val*>  pyvals;

     void Disconnect();

     string GetFlags()
     {
         string s;
         if(connected!=0)
             s="#";
         if(isHidden>0)
             s+="H";
         if(isCanRender == true)
             s+="R";
         if(isNotSaving == false)
             s+="S";
         if(isGroup == true)
             s+="G";
         return s;
     }
     void HideAllAtrs()
     {
         t_atr a;
         for( unsigned int i=0; i < atrs.size(); i++ )
             atrs[i].Hide();
     }

     void ShowAtrs(string s)
     {
         for( unsigned int i=0; i < atrs.size(); i++ )
             if( StrHasToken(s, atrs[i].name, " ") )
                 atrs[i].Unhide();
     }
     void ShowAllAtrs()
         {
             t_atr a;
             for( unsigned int i=0; i < atrs.size(); i++ )
             {
                 atrs[i].Unhide();
                 if( atrs[i].isStd() && bHideStdAttrs )
                     atrs[i].Hide();
             }
         }

     void SetName(std::string str);
     void Rename(std::string str);
     void SetClassName(std::string);
     string GetComboString(string atr_name);
     virtual void Rotate(float x, float y, float z){ x=0; y=0; z=0;}
     virtual void Scale(float x, float y, float z){ x=0; y=0; z=0; }

     //слоты для загрузки состояния обьект из своего файла (например данные вершин сетки)
     virtual void Load(string& s){s=s;}
     virtual void Save(string& s){s=s;}

     //CString namex;

     std::string name;  //имя
     std::string classname;  //имя
     std::string groupname;  //имя группового обьекта
     int		 livetime; //время жизни
     bool		 connected;
     bool		 bHideStdAttrs;
     EntetyInfo	 ei;	// информация о ентети
     vector<t_atr> atrs; // информация об аттрибутах
     map<string,int> atrsmap;
     t_atr		   nullat;
     vector<string> expressions;//выражения
     //tagTYPE	 meth_args[1][1]; // параметры функции
     bool		 isEntety;
     bool        isReference;
     bool		 isNotSaving;
     bool		 isCanRender;
     int		 isSelected;
     bool        isGroup;
     bool		 onOpora;	 // свободно либо на опоре
     int		 isHidden; //0-visible 1-filtered by flags 2-hided by user
     bool        isLoading;
     bool		 m_Updated;

     string		 updatescript;	//скрипт для обновления параметров
     string		 initscript;    //скрипт для инициализации
     Pnt		 bounds; //размеры
     Pnt		 C0;     //центр
     float		 mass;	//масса
     Pnt		 kurs0; //курс радиус вектор из C0
     Pnt         v;    //мгновенная скорость

     time_t      osCreationTime;
     long		 osCreationTimeTicks;
     static map<string, vector<meta_entry > > metatable;


     virtual void Declare()
     {
        //if( atrs.size() > 0 ) return;
        SET_ATTR("ei.coord", &ei.coord, "Pnt"); SET_STD;
        SET_ATTR("mass", &mass, "float");SET_STD;
        SET_ATTR("bounds", &bounds, "Pnt");SET_STD;
        SET_ATTR("C0", &C0, "Pnt");SET_STD;
        SET_ATTR("kurs0", &kurs0, "Pnt");SET_STD;
        SET_ATTR("v", &v, "Pnt");SET_STD;

        SET_ATTR("onOpora", &onOpora, "bool");SET_STD;
        SET_ATTR("isGroup", &isGroup, "bool");SET_STD;
        if(isGroup==false)
        SET_ATTR("group", &groupname, "string");SET_STD;
        SET_ATTR("isSelected", &isSelected, "int");SET_STD;
        SET_ATTR("isHidden", &isHidden, "int");SET_STD;
        SET_ATTR("connected", &connected, "bool");SET_STD;
        SET_ATTR("isNotSaving", &isNotSaving, "bool");SET_STD;
        SET_ATTR("isCanRender", &isCanRender, "bool");SET_STD;

                //CRuntimeClass* pMyRTClass= GetRuntimeClass();
                ei.classname = string("Object");

     }

     //bool  isRenderable;

         //virtual void Serialize(QDataStream& archive, bool IsStoring );
/*         void WriteAttr(QDataStream& archive, t_atr& atr );
         void ReadAttr( QDataStream& archive, t_atr& atr );
         void WriteString(string& str, QDataStream& archive);
         void ReadString(string& str, QDataStream& archive);
         void ReadString2(QDataStream& archive, string& str);
         void WriteString2(QDataStream& archive,  string& str);
*/

     virtual int Size()
     {
         int esize=0;
         for(unsigned int i=0; i<expressions.size(); i++)
             esize+=expressions[i].size();

         return sizeof(this) + name.size()+classname.size()+groupname.size()+atrs.size()*sizeof(t_atr)+esize+initscript.size()+updatescript.size();
     }
         void FillEntetyInfo(string groupname, string material, string model);
             void FillEntetyInfo()	{		FillEntetyInfo("Ungrouped","no material","no model");	}
             void FillEntetyInfo(string groupname)	{		FillEntetyInfo(groupname,"no material","no model");	}

     virtual void Copy(Object* A)
     {
         livetime = A->livetime;
         connected = A->connected;
         isEntety = A->isEntety;
         isNotSaving = A->isNotSaving;
         isCanRender = A->isCanRender;
         isSelected = A->isSelected;
         isHidden = A->isHidden;
         m_Updated = A->m_Updated;
         bounds = A->bounds; //размеры
         C0 = A->C0;     //центр
         groupname = A->groupname;
         isGroup = A->isGroup;
         updatescript = A->updatescript;	//скрипт для обновления параметров
         initscript = A->initscript;    //скрипт для инициализации
         bounds = A->bounds; //размеры
         C0 = A->C0;     //центр
         mass = A->mass;	//масса
         kurs0 = A->kurs0; //курс радиус вектор из C0
         v = A->v;    //мгновенная скорость
         ei.coord = A->ei.coord; //координаты

     }

     // INTERFACE
     void  SET_ATTR(string name, void* addr, string type);
     void  SET_ATTR(string name, void* addr, string type, bool bHidden);
     void  SET_STATE(string name, string posible_states, void* addr);
     t_atr* GET_ATTR(string name);
     void* SET_METHOD(char* parentclassname, char* argstr, char* classname, char* methname, ...);
     int   SET_OBJECT(void* this_ptr, char* objname);
     static void* GET_OBJECT(char* objname);
     static void  DELETE_OBJECT(char* objname);
     int   CALL(char* obname, char* classname, char* methname);
     int   RemoveSignals();


    // often used slots
    virtual void mouse_move(int x, int y){x=y;}
    virtual void mouse_wheel(int zDelta){zDelta=0;}
    virtual void click_lmb(int state){state=0;}
    virtual void click_rmb(int state){state=0;}
    virtual void click_mmb(int state){state=0;}
    virtual void key_pressed(int k){k=0;}
    virtual void key_upressed(int k){k=0;}

    // public slots
    virtual void Slot1(){}
    virtual void Slot2(){}
    virtual void Slot3(){}
    virtual void Slot4(){}

    //events
    virtual void OnSelect(){}
    virtual void OnUnselect(){}
    virtual void OnAttrChange(t_atr& a){ a.isHidden();}
    virtual void OnAttrSelect(t_atr& a){ a.isHidden();}
    virtual void OnDrag( float x, float y, float z );

};

#endif // OBJECT_H
