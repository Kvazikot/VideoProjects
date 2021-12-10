/**********************************************************************
 *  Project App6
 *
 *  param_screen_bcb.cpp
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

#include <algorithm>
#include <stdio.h>
#include <string.h>
#include "../include/print.h"
#include "../include/object.h"
#include "../include/str_utils.h"
#include "../include/param_screen_bcb.h"

//ParamScreen ps;
//ParamScreen vt;
static  char gstr[2048];
ParamScreen ParamScreen::instance;
ParamScreen ParamScreen::vt_instance;


ParamScreen::ParamScreen()
{
   MaxRows = 20;
   MaxCols = 100;
   mode = UPD_MODE;
   VARTIMEOUT = 8;
   // запрос на апдейт
   //Connect();
   for(int i=0; i<=MaxRows; i++)
   {
     paramStr.push_back("");
     //paramStr[i]="";
   }
}

ParamScreen& ParamScreen::getInstance()
{
    return instance;
}

ParamScreen& ParamScreen::getVtInstance()
{
    return vt_instance;
}

int ParamScreen::FindVar(string name, t_vsett& var_info, string& val)
{
    if( paramSetMap.find(name) != paramSetMap.end() )
    {
        var_info = paramSetMap[name];
        val = paramMap[name];
        return 1;
    }

    return 0;
}

// добавить имя
void ParamScreen::AddVar( string name )
{
   VarCol new_varcol;

   if( table_name!="" )   {		name = table_name + "." + name;   }

   if( paramMap.find(name) != paramMap.end() )
     return;
   varnames.push_back(name);
   new_varcol.i = varnames.size() - 1;
   new_varcol.active = false;
   new_varcol.num = 0;
   new_varcol.name = name;
   VarColTab[name] = new_varcol;
   paramMap[name] = "0.0";
   paramSetMap[name].always = false;
   paramSetMap[name].timeout=VARTIMEOUT;
   paramSetMap[name].disabled = 0;

   for(int i = 0; i < MaxRows + 1; i++)
   {
      tagPARAM p;
      VarRegTab[name].push_back(p);
   }


   ActivateVar(name);
   //DEBUG_BLD("Vars = %d", VarColTab.size()  );
}



void ParamScreen::AddVar( char* name, string color )
{
    AddVar(name);
    paramSetMap[name].timeout=VARTIMEOUT;
    paramSetMap[name].color = color;
}

// убрать имя
void ParamScreen::RemoveVar(char* name)
{
   if( VarColTab.find(name) == VarColTab.end() )
     return;
   //std::remove( );
   varnames.erase (find (varnames.begin(), varnames.end(), name));
   VarColTab.erase (VarColTab.find(name));
   paramMap.erase(paramMap.find(name));
   paramSetMap.erase(paramSetMap.find(name));
   alias2attrib.erase(alias2attrib.find(name));
}


void ParamScreen::PrintHeader(int nsheet)
{
    int j = 0;
    string name;
    map<string, VarCol>::iterator i;
    for (i = VarColTab.end(); i != VarColTab.begin(); i--)
    {
        name = (*i).first;
        j = (*i).second.i;
        WriteCell(nsheet, j, 0, name);
     }


}

void ParamScreen::AddAttr( char* alias, char* attrpath )
{
   alias=attrpath;
}



void ParamScreen::Enable( string name )
{
  if( paramMap.find(name) == paramMap.end() )
     return;

    paramSetMap[name].disabled = false;
}

void ParamScreen::Disable( string name )
{
  if( paramMap.find(name) == paramMap.end() )
     return;

    paramSetMap[name].disabled = true;
}


void ParamScreen::Reset()
{
    paramMap.clear();
    paramSetMap.clear();
    varnames.clear();
    tablenames.clear();
    VarColTab.clear();
    VarRegTab.clear();
    attrib2alias.clear();
    alias2attrib.clear();
    AddVar("info");

}

bool ParamScreen::upd(char* name, Pnt val)
{
    static char str[20]  = "";
    if(paramMap.find(name) == paramMap.end())
    {
       return false;
    }

    sprintf(str, "%.2f %.2f %.2f", val.x, val.y, val.z);
    string sval = paramMap[name];
    paramMap[name] = str;
    if(sval != str)
        paramSetMap[name].timeout=VARTIMEOUT;

    string cl = paramSetMap[name].color;


    return true;
}


bool ParamScreen::upd(char* name, string val)
{

    if(paramMap.find(name) == paramMap.end())
    {
       return false;
    }
    string sval = paramMap[name];
    paramMap[name] = val;
    if(sval != val)
        paramSetMap[name].timeout=VARTIMEOUT;
    return true;
}

//! обновить переменную типа float
bool ParamScreen::upd(char* name, float val)
{
    static char str[20]  = "";
    if(paramMap.find(name) == paramMap.end())
    {
       return false;
    }
    sprintf(str, "%.2f", val);
    string sval = paramMap[name];
    paramMap[name] = str;
    if(sval != str)
        paramSetMap[name].timeout=VARTIMEOUT;
    string cl = paramSetMap[name].color;

    return true;
}


//! обновить переменную типа int
bool ParamScreen::upd(char* name, int val)
{
    static char str[20]  = "";
    if(paramMap.find(name) == paramMap.end())
    {
       return false;
    }
    sprintf(str, "%d", val);
    paramMap[name] = str;
    //if(	paramSetMap[name].always)
    string sval = paramMap[name];
    if(sval != str)
        paramSetMap[name].timeout=VARTIMEOUT;
    string cl = paramSetMap[name].color;


   return true;
}


//! обновить переменную типа double
bool ParamScreen::upd(char* name, double val)
{
    static char str[20]  = "";
    if(paramMap.find(name) == paramMap.end())
    {
       return false;
    }
    sprintf(str, "%.2lf", val);
    paramMap[name] = str;
    string sval = paramMap[name];
    if(sval != str)
      paramSetMap[name].timeout=VARTIMEOUT;
    string cl = paramSetMap[name].color;

     return true;
}

float ParamScreen::getf(string name)
{
    if( table_name!="" )   {		name = table_name + "." + name;   }
    if( paramMap.find(name) == paramMap.end() ) return 0;
    string sval = paramMap[name];
    if(sval == "")
    {
       return 0;
    }
    float f = 0;
    if(sscanf(sval.c_str(),"%f",&f)==1)
    {
        return f;
    }
    return 0;
}

double ParamScreen::getd(string name )
{
    if( table_name!="" )   {		name = table_name + "." + name;   }
    if( paramMap.find(name) == paramMap.end() ) return 0;
    string sval = paramMap[name];
    if(sval == "")
    {
       return 0;
    }
    double f = 0;
    bool bAddPoint = true;
    for(int i=0; i<(int)sval.size(); i++)
        if( sval[i] == '.' )
            bAddPoint = false;
    if( bAddPoint )
        sval=sval+".0";
    if(sscanf(sval.c_str(),"%lf",&f)==1)
    {
        return f;
    }
    return 0;
}

int ParamScreen::geti(string name)
{
    if( table_name!="" )   {		name = table_name + "." + name;   }
    if( paramMap.find(name) == paramMap.end() ) return 0;
    string sval = paramMap[name];
    if(sval == "")
    {
       return 0;
    }
    int i = 0;
    if(sscanf(sval.c_str(),"%d",&i)==1)
    {
        return i;
    }
    return 0;
}

Pnt ParamScreen::getP(string name)
{
    if( table_name!="" )   {		name = table_name + "." + name;   }
    if( paramMap.find(name) == paramMap.end() ) return Pnt(0,0,0);
    string sval = paramMap[name];
    if(sval == "")
    {
       return Pnt(0,0,0);
    }
    Pnt val;
    if(sscanf(sval.c_str(),"%f,%f,%f",&val.x, &val.y, &val.z)==3)
    {
        return val;
    }
    return Pnt(0,0,0);
}

string ParamScreen::gets(string name)
{
    if( table_name!="" )   {		name = table_name + "." + name;   }
    if( paramMap.find(name) == paramMap.end() ) return "";
    string sval = paramMap[name];
    return sval;
}

bool ParamScreen::getarr(string name, vector<double>& arr)
{
    if( paramMap.find(name) == paramMap.end() ) return false;
    string sval = paramMap[name];
    vector<string> tok = StrSplitE(sval,",",false);
    for(int i=0; i<(int)tok.size(); i++)
        arr.push_back(StrToDouble(tok[i]));
    return true;
}

void ParamScreen::PrintMap()
{
    map<string, string >::iterator i;
    int j=0;
    string val=" ",str=" ";

    //return;
    //if(ncols!= MaxCols)
    {
        for(j=0; j<=MaxRows; j++)
            paramStr[j] = " ";

        //ncols = MaxCols;
    }

    j=0;


    val="";
    for(i = paramMap.begin(); i!=paramMap.end(); i++)
    {
        if(paramSetMap[i->first].timeout<=0)
            continue;
        if(paramSetMap[i->first].disabled)
            continue;
        if(i->second == "")
            continue;


        if(i->first != "info")
            val = i->first + "=" + i->second + " ";
        else
            val = i->second + " ";

        if( (int)(val.size() + str.size()) > MaxCols)
        {
            paramStr[j] = str;
            j++;
            str="";
        }

        str+=val;
    }
    paramStr[j] = str;
    return;

}

string ParamScreen::GetMap()
{
    map<string, string >::iterator i;
    string val=" ",str=" ";
    for(i = paramMap.begin(); i!=paramMap.end(); i++)
    {
        val = i->first + "=" + i->second + " ";
        str+=val;
    }
    return str;
}

void ParamScreen::GetVars(string tablename, vector<pair<string,string> >& vars)
{
    map<string, string >::iterator i;

    if(tablename == "Globals" )
    {
        for(i = paramMap.begin(); i!=paramMap.end(); i++)
        {
            if( i->first.find(".")==string::npos )
            {
                pair<string,string> var;
                var.first = i->first;
                var.second = i->second;
                vars.push_back(var);
            }
        }
        return;
    }
    for(i = paramMap.begin(); i!=paramMap.end(); i++)
    {
        int p = i->first.find(tablename + ".");
        int start = p + tablename.size() + 1;
        if( p!=string::npos )
        {
            pair<string,string> var;
            var.first = i->first.substr(start, i->first.size() - start);
            var.second = i->second;
            vars.push_back(var);
        }
    }

}


void ParamScreen::update_timeout()
{
    map<string, t_vsett >::iterator i;

    for(i = paramSetMap.begin(); i!=paramSetMap.end(); i++)
    {
        if((!i->second.always) && (i->first!="info"))
        {
            prn("%s-%d",i->first.c_str(), i->second.timeout);
            i->second.timeout--;
            //paramSetMap[i->first].timeout--;
        }
    }

}


























//------------------------СТАРАЯ ХРЕНЬ

// активировать колонку
void ParamScreen::ActivateVar( string name )
{
   VarCol varcol;
   if( VarColTab.find(name) == VarColTab.end() )
     return;

   VarColTab[name].active = true;

}

// деактивировать колонку
void ParamScreen::DeActivateVar( string name )
{
   VarCol varcol;
   if( VarColTab.find(name) == VarColTab.end() )
     return;
   VarColTab[name].active = false;
}

//---------------------------------------------------------------------------
bool ComparePCol(VarCol* col1, VarCol* col2)
{
   return (col1->i) < (col2->i);
}

void ParamScreen::CreatePCOLSFromVarColTab()
{
  VarCol* pcol;
  map<string, VarCol>::iterator i;

  pcols.resize(VarColTab.size());

  int j = 0;
  // составить массив указателей по таблице VarColTab
  for (i = VarColTab.end(); i != VarColTab.begin(); i--)
  {
        pcol = &((*i).second);
        pcols[j] = pcol;
        j++;
  }

  std::sort(pcols.begin(), pcols.end(), ComparePCol);

}

// поднять переменную (order + 1)
void ParamScreen::UpVar(string name)
{
  // --------- MoveUp --------------
  // сдвинуть переменную в VarColTab
    std::vector<VarCol*>::iterator i;
    VarCol  *a, *b;
  // 1. заполнение вектра cools по таблице VarRegTab
  CreatePCOLSFromVarColTab();

   // если первый - выход

 // 1. Нахождение в массиве cols
 for(i = pcols.begin(); i != pcols.end(); i++)
 {
    if( (*i)->name ==  name )
    {
       (*i)->i = (*i)->i - 1; a = (*i);
       i--;
       (*i)->i = (*i)->i + 1; b = (*i);
       i++;
        break;
       // обмен указателей
      // занесение указателей в pcols
      (*i) = b;
       i--;
       (*i) = a;
       i++;
       break;
    }

    if((*i)->i == -1)
      break;
 }

   for( i = pcols.begin(); i < pcols.end(); i++)
       if( (*i)->i == - 1 )
         break;

   std::sort(pcols.begin(), i, ComparePCol);
   cols_output();
}


void ParamScreen::cols_output()
{
    std::vector<VarCol*>::iterator i;
    string str;
    for(i = pcols.begin(); i < pcols.end(); i++)
    {
      if((*i)->i == -1)
        break;
      mu_printf("ParamScreen::cols_output col->i = %d", (*i)->i);
     }
}

// опустить переменную (order - 1)
void ParamScreen::DownVar(string name)
{
    std::vector<VarCol*>::iterator i;
    VarCol  *a, *b;

   // 1. заполнение вектра cools по таблице VarRegTab
   CreatePCOLSFromVarColTab();

   // 2. Нахождение переменной в массиве cols
   for(i = pcols.begin(); i != pcols.end(); i++)
   {
      if( (*i)->name ==  name )
      {
         (*i)->i = (*i)->i + 1; a = (*i);
         i++;
         (*i)->i = (*i)->i - 1; b = (*i);
         i--;
          break;
         // обмен указателей
         // занесение указателей в pcols
         (*i) = b;
         i++;
         (*i) = a;
         i--;

         break;
      }

      if((*i)->i == -1)
        break;
   }

   // сортировка массива
   for( i = pcols.begin(); i < pcols.end(); i++)
       if( (*i)->i == - 1 )
         break;
   std::sort(pcols.begin(), i, ComparePCol);
   cols_output();
}



// установить режим
void ParamScreen::SetMode(PSMode _mode)
{
   mode = _mode;
}

extern void WriteCellExt(int nsheet, int x, int y, string val);

void ParamScreen::WriteCell(int nsheet, int x, int y, string val)
{
   //WriteCellExt(nsheet, x, y, val);
    x=y;
    nsheet=x;
    val=x;
}


ParamScreen::~ParamScreen()
{

}



// коллбек таймера обновления параметров на экране
void ParamScreen::alarm()
{
//    update();
}


//--+------------------------++-----------------++---
//
//    Ф  У  Н  К  Ц  И  И    А  П  Д  Е  Й  Т  Е  Р А
//
//--+------------------------++-----------------++---
void MakeSnapshot()
{

}

//--+------------------------++-----------------++---
//
//       Ф У Н К Ц И И    Р Е Г И С Т Р А Т О Р А
//
//--+------------------------++-----------------++---

void ParamScreen::reg(char* name, double val)
{
   string sval;
   tagPARAM p;

    if( VarColTab.find(name) == VarColTab.end() )
     return;

   //add 2string map
   sprintf(gstr, "%f", val);
   paramMap[name] = gstr;

   //add to var table
   p.vartype = TYPE_FLOAT;
   p.fltVal = val;

   VarRegTab[string(name)].size();
   vector<tagPARAM> plist;
   switch(mode)
   {

      case GROW_MODE:
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
        plist = VarRegTab[name];
        mu_printf("ParamScreen::reg plist.size = %d", plist.size());
        //WriteCell(0, VarColTab[name].i, VarColTab[name].num, sval);
       //WriteCell(0, );
      break;

      case UPD_MODE:
        if( VarColTab[name].num > MaxRows )
        {
           VarColTab[name].num = 0;
        }
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
         // nothing only in mem
      break;

      case ACCUM_MODE:
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
         // nothing only in mem
      break;

   }

}

void ParamScreen::reg(char* name, char val)
{
   string sval;
   tagPARAM p;

   if( VarColTab.find(name) == VarColTab.end() )
     return;

   //add 2string map
   sprintf(gstr, "%c", val);
   paramMap[name] = gstr;

   //add to var table
   p.vartype = TYPE_CHAR;
   p.bVal = val;

   VarRegTab[string(name)].size();

   switch(mode)
   {

      case GROW_MODE:
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
        WriteCell(0, VarColTab[name].i, VarColTab[name].num, sval);
       //WriteCell(0, );
      break;

      case UPD_MODE:
        if( VarColTab[name].num > MaxRows )
        {
           VarColTab[name].num = 0;

        }

        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
         // nothing only in mem
      break;

      case ACCUM_MODE:
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
         // nothing only in mem
      break;

   }

}
void ParamScreen::reg(char* name, float val)
{
   string sval;
   tagPARAM p;
   int i;

     if( VarColTab.find(name) == VarColTab.end() )
     return;
   //add 2string map
   sprintf(gstr, "%f", val);
   paramMap[name] = gstr;

   //add to var table
   p.vartype = TYPE_FLOAT;
   p.fltVal = val;

   VarRegTab[string(name)].size();

   switch(mode)
   {

      case GROW_MODE:
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
        WriteCell(0, VarColTab[name].i, VarColTab[name].num, sval);
       //WriteCell(0, );
      break;

      case UPD_MODE:
        if( VarColTab[name].num > MaxRows )
        {
           VarColTab[name].num = 0;
        }

        i = VarColTab[name].num++;
        VarRegTab[name][i-1] = p ;
         // nothing only in mem
      break;

      case ACCUM_MODE:
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
         // nothing only in mem
      break;

   }

}

//extern void InsertClipboardAt(int nsheet, string row);

bool CompareCol(VarCol col1, VarCol col2)
{
   return col1.i < col1.i;
}


//! update lab els
void ParamScreen::update()
{
    map<string, VarCol>::iterator i;
    vector<VarCol> cols;
    VarCol col;
    vector<tagPARAM> plist;
    tagPARAM  param;
    string tmp, val;
    //TStringList* slist = new TStringList();
    //clrscr();

   int cnt = 0;
   cols.resize(VarColTab.size());
   for (i = VarColTab.begin(); i != VarColTab.end(); i++)
   {
       cols[cnt] = (*i).second;
       cnt++;
    }
    //sorting
    std::sort(cols.begin(), cols.end(), CompareCol);

    for(int j = 0; j < MaxRows; j++)
    {

      for (int k = 0; k < cnt; k++)
      {

          if( cols[k].active == false)
            continue;
          plist = VarRegTab[cols[k].name];

          //dbg.sprintf("plist.size = %d", plist.size());
          if( (int)plist.size() > j)
          {
            param = plist[j];
            switch( param.vartype )
            {
               case TYPE_DOUBLE :
                sprintf(gstr, "%f\t", param.fltVal);
                tmp = tmp + val;
               break;
               case TYPE_FLOAT :
                sprintf(gstr, "%f\t", param.fltVal);
                tmp = tmp + val;
               break;
               case TYPE_INT :
                sprintf(gstr, "%d\t", param.iVal);
                tmp = tmp + val;
               break;
               case TYPE_CHAR :
                sprintf(gstr, "%d\t", param.bVal);
                tmp = tmp + val;
               break;
            }
         }



          //std::cout << ;
      }
           //dbg = (*i).first + dbg;
          //dbg = tmp;
          //slist->Add(tmp);
          tmp = "";
           //add_line();
    }

/*
        VarColTab[name].num++;
        VarRegTab[name].push_back(p);
        WriteCell(0, VarColTab[name].i, VarColTab[name].num, sval);
*/
    //slist->Free();
//    Sleep(100);
}

void TableObj::SetTable(string t)
{
    ParamScreen& vt = ParamScreen::getVtInstance();
    vt.obj_map[t].push_back(this);
    my_table = t;
}

