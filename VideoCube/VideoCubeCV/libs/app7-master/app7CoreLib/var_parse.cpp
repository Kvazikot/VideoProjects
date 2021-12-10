/**********************************************************************
 *  Project App6
 *
 *  var_parse.cpp
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

#include "../include/str_utils.h"
#include "../include/var_parse.h"
#include "../include/param_screen_bcb.h"
#include <stack>

VarParser varparser;

VarParser::VarParser()
{

}

// собрать описание
//обратное присвоение из таблицы в строку описания
void VarParser::UpdateTable(string table_key)
{
    vector<string> slist;
    vector<string> slist_out;

    if(table_key == "") table_key = "Globals";
    outputS = "";

    if(inputS.size() < 3) return;

    slist = StrSplit(inputS);
    slist_out = StrSplit(inputS);

    for(int i=0; i<(int)slist.size(); i++)
    {
        string& s = slist[i];
        string s_out = slist[i];

        //обработка комментов
        /*
        if(s.size()>0)
        if ( (s[0] == '/' ) || (s[0] == '-') )
        {
            //очистка строки от знаков комментариев
            s = StrRep(s, "/","");
            string str_minus;
            for(int i=0; i<s.size(); i++)
                if( s[i]!='-') break;
                else str_minus+='-';
            s = StrRep(s, str_minus, "");
            if( CountSeps(s, "{") > 0  )
            {
                string tablename = strf(s, "{", 0);
                tablename=StrRep(tablename, " ", "");
                int p = s.find_first_of("{");
                s = s.substr( p, s.size() - p );
                s=StrRep(s, "{", "");
                s=StrRep(s, "}", "");
            }

        }
        */

        bool bIsThatTable=false;

        // разбор переменной
        if( CountSeps(s, "=") > 0  )
        {
            if( CountSeps(s, "{") > 0  )
            {
                string tablename = strf(s, "{", 0);
                tablename=StrRep(tablename, " ", "");
                table_key=StrRep(table_key, " ", "");
                int p = s.find_first_of("{");
                s = s.substr( p, s.size() - p );
                s=StrRep(s, "{", "");
                s=StrRep(s, "}", "");
                //vt.NewTable(tablename);
                if(table_key == tablename)
                    bIsThatTable = true;

            }
            else
                if(table_key=="Globals" )
                    bIsThatTable = true;

            if( bIsThatTable )
            {
                //цикл по переменным

                //string s2 = StrRep(s, " ", "\n");
                string s2;
                vector<string> tokens = VarsSplit(s);
                //обратное присвоение из таблицы в строку описания
                if( table_key != "Globals" )
                    s2 = table_key + "{";
                for(int j=1; j<(int)tokens.size(); j+=2)
                {
                    string var = tokens[j-1];
                    string val;// = tokens[j];
                    string k;
                    if( table_key != "Globals" )
                        k = table_key + "." + var;
                    else
                        k = var;
                    if(vt.hasKey(k))
                    {
                        val = vt.paramMap[k];
                        s2+= var + "=" + val + " ";
                    }
                    else
                    {
                        s2 = slist[i];
                        //table_key="";
                    }
                }
                s_out = s2;
                if( table_key != "Globals" )
                    s_out+="}";

            }


        }
        CleanEndingSpaces(s_out);
        outputS+= s_out + "\r\n";

        //outputS+=s_out;
    }

    outputS = outputS.substr(0, outputS.size()-2);
    //
    //outputS = StrJoin(slist_out, "\n");
}


void VarParser::ParsePoints2d(string& pts_s, vector<Pnt>& parr)
{
    if( pts_s != "")
    {
        pts_s = StrRep(pts_s,"[","");
        pts_s = StrRep(pts_s,"]","");
        vector<string> elm = StrSplitE(pts_s,",",false);
        parr.clear();
        for(int i=0; i<(int)elm.size()-1; i+=2)
        {
            PntEx p;
            if((elm[i]=="") || (elm[i+1]=="") )
                continue;
            sscanf(elm[i].c_str(),"%f",&p.x);
            sscanf(elm[i+1].c_str(),"%f",&p.y);
            parr.push_back(p);
        }
    }
}

void VarParser::ParsePoints3d(string& pts_s, vector<Pnt>& parr)
{
    if( pts_s != "")
    {
        pts_s = StrRep(pts_s,"[","");
        pts_s = StrRep(pts_s,"]","");
        vector<string> elm = StrSplitE(pts_s,", ",false);
        if(elm.size() == 0) return;
        parr.clear();
        for(int i=0; i<(int)elm.size()-2; i+=3)
        {
            Pnt p = Pnt(0,0,0);
            if((elm[i]=="") || (elm[i+1]=="") || (elm[i+2]=="") )
                continue;
            sscanf(elm[i].c_str(),"%f",&p.x);
            sscanf(elm[i+1].c_str(),"%f",&p.y);
            sscanf(elm[i+2].c_str(),"%f",&p.z);
            parr.push_back(p);
        }
    }
}

void VarParser::ParseDoubleArray(string& pts_s, vector<double>& parr)
{
    if( pts_s != "")
    {
        pts_s = StrRep(pts_s,"[","");
        pts_s = StrRep(pts_s,"]","");
        vector<string> elm = StrSplitE(pts_s," ",false);
        parr.clear();
        for(int i=0; i<(int)elm.size(); i++)
        {
            double d;
            sscanf(elm[i].c_str(),"%lf",&d);
            parr.push_back(d);
        }
    }
}

void VarParser::ParseDoubleArray(string& pts_s, vector<double>& parr, string sep)
{
    if( pts_s != "")
    {
        pts_s = StrRep(pts_s,"[","");
        pts_s = StrRep(pts_s,"]","");
        vector<string> elm = StrSplitE(pts_s,sep,false);
        parr.clear();
        for(int i=0; i<(int)elm.size(); i++)
        {
            double d;
            sscanf(elm[i].c_str(),"%lf",&d);
            parr.push_back(d);
        }
    }
}


void  VarParser::ParseIntArray(string& pts_s, vector<int>& parr, string sep)
{
    if( pts_s != "")
    {
        pts_s = StrRep(pts_s,"[","");
        pts_s = StrRep(pts_s,"]","");
        vector<string> elm = StrSplitE(pts_s,sep,false);
        parr.clear();
        for(int i=0; i<(int)elm.size(); i++)
        {
            int d;
            sscanf(elm[i].c_str(),"%d",&d);
            parr.push_back(d);
        }
    }
}

void VarParser::UpdatePointsStr(string& val, vector<PntEx>& pts)
{
    char str[1024];
    val="[";
    for(int i=0; i<(int)pts.size(); i++)
    {
        if( i>0 ) val+=",";
        sprintf(str, "%3.2f,%3.2f",pts[i].x, pts[i].y);
        val+=str;
    }
    val+="]";
}


string VarParser::GetVarFromStr(string varname, string s)
{
    vector<MyVar> vars;
    ParseString(s, vars);
    for(int i=0; i<(int)vars.size(); i++)
        if( vars[i].name == varname)
        {
            return vars[i].val;
        }
    return "";
}

MyTable EmptyTable;

void MyTable::set_names(MyTable* table)
{
    for(int i=0; i < table->vars.size(); i++)
    {
        if( table->vars[i].name == "name" )
            table->tablename = table->vars[i].val;
    }
    for(int i=0; i < table->children.size(); i++)
        set_names( table->children[i] );
}

MyTable* MyTable::find(MyTable* table,string name)
{
    if( name == table->tablename )
      return table;

   for(int i=0; i < table->children.size(); i++)
   {
        return find(table->children[i], name);
   }
}

MyTable* MyTable::first_child()
{
    if( children.size() == 0 )
        return &EmptyTable;
    return children[0];
}

string MyTable::get_prop(string name)
{
    for(int i=0; i < vars.size(); i++)
    {
        if( vars[i].name == name )
            return vars[i].val;
    }
    return "";
}

// "{type=multi name=w0_mso {id=1 type=bitmap name=inner_rim file=G:\3d_scenes\scenes\models2\CARS\LOTUS\wheel0.tga U_Tiling=1.0 V_Tiling=1.0 specular=0.0,0.0,0.0 diffuse=204.0,204.0,204.0 specular=127.5,127.5,127.5} {id=2 type=bitmap name=rim file=G:\3d_scenes\scenes\models2\CARS\LOTUS\wheel.tga U_Tiling=1.0 V_Tiling=1.0 specular=127.5,127.5,127.5 diffuse=127.5,127.5,127.5 specular=229.5,229.5,229.5} {id=3 type=bitmap name=tire file=G:\3d_scenes\scenes\models2\CARS\LOTUS\tirea0.tga U_Tiling=1.0 V_Tiling=1.0 specular=0.0,0.0,0.0 diffuse=204.0,204.0,204.0 specular=127.5,127.5,127.5} }";
//парсинг таблицы
Parse_Err VarParser::ParseTable(string s, MyTable* out_table )
{
    //return PARSER_NO_ERROR;
    if( CountSeps(s, "{") == 0 )
        return PARSER_PARETNESS_ERROR;
    if( CountSeps(s, "}") == 0 )
        return PARSER_PARETNESS_ERROR;
    if( CountSeps(s, "{") != CountSeps(s, "}") )
        return PARSER_PARETNESS_ERROR;

    int Nvars = CountSeps(s, "=");
    if( Nvars == 0 ) return PARSER_EMPTY_TABLE;

    vector<string> tokens = StrSplitE(s," ={}", true);
    stack<MyTable*> table_stack;
    map<int, MyTable*> table_map;

    MyTable* parent_table = out_table;

    //занести индексы операторов "равно"
    vector<int> eq_idx;
    for(int j=0; j<(int)tokens.size(); j++)
    {
        if( tokens[j] == "{" )
        {
            MyTable* table = new MyTable();
            parent_table->children.push_back(table);
            parent_table = table;
            table_stack.push(table);
        }
        if( tokens[j] == "}" )
        {
            if(!table_stack.empty())
            {
                MyTable* t = table_stack.top();
                table_stack.pop();
                if(!table_stack.empty())
                  parent_table = table_stack.top();
            }
        }

        if( tokens[j] == "=" )
        {
            eq_idx.push_back(j);
            table_map[j] = parent_table;
        }
    }


    if( eq_idx.size() == 0 ) return PARSER_EMPTY_TABLE;

    //проход по операторам "равно"
    string left_val, right_val;
    for(int i=0; i<(int)eq_idx.size(); i++)
    {
        MyVar var;

        left_val = "";
        right_val = "";

        //находим первый не пробельный слева
        int j = eq_idx[i];
        while(j>0)
        {
            j--;
            if( tokens[j]!=" ")
            {
                left_val = tokens[j];
                break;
            }

        }

        //загоняем все до следующего оп. равно в правый
        int start = eq_idx[i];
        int end = tokens.size()-1;
        if( i < (int)(eq_idx.size()-1))
            end = eq_idx[i+1];
        while(end>0)
        {
            end--;
            if( tokens[end]!=" ")
            {
                break;
            }
        }

        if(Nvars==1)
            right_val+=tokens[end];
        else
        {
            if(start < end)
            {

                for(j=start; j<end; j++)
                    right_val+=tokens[j];

                //отдельно случай последней перемнной
                if( i == (eq_idx.size() - 1))
                {
                    right_val = "";
                    for(j=start; j<=end; j++)
                        right_val+=tokens[j];
                }

            }
        }


        //создаем переменную
        clean_string_seps(left_val,"=");
        clean_string_seps(right_val,"=");
        clean_string_seps(left_val,"{}");
        clean_string_seps(right_val,"{}");
//		CleanEndingSpaces(left_val);
        CleanEndingSpaces(right_val);
        var.name = left_val;
        var.val = right_val;
        if( table_map.find(eq_idx[i]) != table_map.end() )
        {
          MyTable* table = table_map[eq_idx[i]];//find_table_by_token(out_table, eq_idx[i]);
          table->vars.push_back(var);         
        }

    }
    out_table->set_names(out_table);
    return PARSER_NO_ERROR;
}

//парсинг строки
void VarParser::ParseString(string s, vector<MyVar>& vars)
{
    MyVar var;

    s+=" ";

    // обработка значений массивов

    // обработка таблиц
    if( CountSeps(s, "{") > 0  )
    {
        string tablename = strf(s, "{", 0);
        tablename=StrRep(tablename, " ", "");
        int p = s.find_first_of("{");
        s = s.substr( p, s.size() - p );
        s=StrRep(s, "\n", "");
        s=StrRep(s, "{", "");
        s=StrRep(s, "}", "");
        var.isInTable = true;
        var.tablename = tablename;
    }

    // обработка значений таблиц
    int Nvars = CountSeps(s, "=");
    if( Nvars == 0 ) { vars.push_back(var); return; }

    vector<string> tokens = StrSplitE(s," =", true);

    //занести индексы операторов "равно"
    vector<int> eq_idx;
    for(int j=0; j<(int)tokens.size(); j++)
    {
        if( tokens[j] == "=" )
            eq_idx.push_back(j);
    }

    if( eq_idx.size() == 0 ) return;

    //проход по операторам "равно"
    string left_val, right_val;
    for(int i=0; i<(int)eq_idx.size(); i++)
    {
        left_val = "";
        right_val = "";

        //находим первый не пробельный слева
        int j = eq_idx[i];
        while(j>0)
        {
            j--;
            if( tokens[j]!=" ")
            {
                left_val = tokens[j];
                break;
            }

        }

        //загоняем все до следующего оп. равно в правый
        int start = eq_idx[i];
        int end = tokens.size()-1;
        if( i < (int)(eq_idx.size()-1))
            end = eq_idx[i+1];
        while(end>0)
        {
            end--;
            if( tokens[end]!=" ")
            {
                break;
            }
        }

        if(Nvars==1)
            right_val+=tokens[end];
        else
        {
            if(start < end)
            {

                for(j=start; j<end; j++)
                    right_val+=tokens[j];

                //отдельно случай последней перемнной
                if( i == (eq_idx.size() - 1))
                {
                    right_val = "";
                    for(j=start; j<=end; j++)
                        right_val+=tokens[j];
                }

            }
        }


        //создаем переменную
        clean_string_seps(left_val,"=");
        clean_string_seps(right_val,"=");
//		CleanEndingSpaces(left_val);
        CleanEndingSpaces(right_val);
        var.name = left_val;
        var.val = right_val;

        vars.push_back(var);

    }


}

//разобрать описание
void VarParser::ParseInput()
{
    MyMatrix Mat;
    vector<string> slist;
    vector<string> mlist;
    slist = StrSplit(inputS);
    enum {outGroupComment, inGroupComment} comment_state = outGroupComment;

    if(inputS.size() < 3) return;

    vt.Reset();

    for(int i=0; i<(int)slist.size(); i++)
    {
        string& s = slist[i];

        if( s.find("/*")!=-1 )
            comment_state = inGroupComment;

        if( s.find("*/")!=-1 )
            comment_state = outGroupComment;

        if(comment_state == inGroupComment)
            continue;

        if( s == "return")
            continue;

        //обработка комментов
        if(s.size()>0) if ( (s[0] == '/' ) || (s[0] == '-') )
        {
            //очистка строки от знаков комментариев
            s = StrRep(s, "/","");
            string str_minus;
            for(int i=0; i<(int)s.size(); i++)
                if( s[i]!='-') break;
                else str_minus+='-';
            s = StrRep(s, str_minus, "");
            continue;
        }

        bool bIsTable=false;
        vector<MyVar> vars;

        ParseString(s, vars);

        if(vars.size()>0)
        {
            bIsTable=false;
            if(vars[0].tablename!="")
            {
                bIsTable = true;
                vt.NewTable(vars[0].tablename);
            }
            for(int j=0; j<(int)vars.size(); j++ )
            {
                if( bIsTable )
                {
                    vt.AddVar(vars[j].name);
                    vt.Set(vars[j].name, vars[j].val);
                }
                else
                    vt.Set(vars[j].name, vars[j].val);
            }
            if(bIsTable)
                vt.EndTable();
        }

        // разбор матрицы
        string NameS = strf(s, " ", 0);
        string SizeS = strf(s, " ", 1);
        string Ms = strf(s, " ", 2);
        int m,n;
        Mat.name = NameS;

        if( sscanf(SizeS.c_str(), "%dx%d", &m, &n) == 2)
            Mat.m = m; Mat.n = n;

        for(int j=0; j<m; j++)
            for(int k=0; k<n; k++)
            {
                float val;
                int idx = j*n + k;
                string el;
                el = strf(Ms, ",", idx);
                if(el!="")
                {
                    sscanf(el.c_str(), "%f", &val);
                    Mat.elem[j][k] = val;
                }
            }
            //gprintf(Mat.Print().c_str());

        matrixes.push_back(Mat);



    }


    string s = vt.GetMap();
    //gprintf(s.c_str());
    //gprintf("%d", slist.size());
}

void VarParser::ParseColor(string col_s, GLfloat color[4])
{
    vector<double> parr;
    ParseDoubleArray(col_s, parr, ",");
    if(parr.size()>2)
    {
        color[0] = parr[0];
        color[1] = parr[1];
        color[2] = parr[2];
        color[3] = 1;
    }
    if(parr.size()>3)	color[3] = parr[3];
}


