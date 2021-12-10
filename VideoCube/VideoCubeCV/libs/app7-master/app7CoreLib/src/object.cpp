/**********************************************************************
 *  Project App6
 *
 *  object.cpp
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

#include <QGLFunctions>
#include <time.h>
#include "../include/object.h"
#include "../include/print.h"
#include "../include/cinput.h"
#include "../include/str_utils.h"

map<string, vector<meta_entry > > Object::metatable;


Object::Object()
{
    name = "obj";
    m_Updated = 1;
    connected = 0;
    isCanRender = false;
    isEntety = false;
    isNotSaving = false;
    isHidden = 0;
    isGroup = false;
    isReference = false;
    onOpora = false;
    isSelected = 0;
    bounds = Pnt(0,0,0);
    C0 = Pnt(0,0,0);
    mass = 0;
    kurs0 = Pnt(0,0,0);
    v = Pnt(0,0,0);

}


void Object::SetPyFunc(string fname)
{
    SetPyAttr(fname, "func", "");
}


string Object::GetComboString(string atr_name)
{
    t_atr* a = GET_ATTR(atr_name);
    if( a->addr == 0 ) return "";
    if(*((string*)a->addr)=="") return "";
    vector<string> slst = StrSplitE(*((string*)a->addr)," ",0);
    if(a->data < (int)slst.size())
        return slst[a->data];
    else
        return "";
}


void Object::SetPyAttr(string aname, string type, string val)
{
    t_atr atr;

    if( atrsmap.find(aname)!=atrsmap.end() )
        return;

    atr.flags = 0;
    atr.data = 0;
    atr.name = aname;

    t_val* pval = new t_val;
    pyvals.push_back(pval);

    if(type == "Pnt") atr.addr = (void*)&pval->pntVal;
    if(type == "float") atr.addr = (void*)&pval->fVal;
    if(type == "int") atr.addr = (void*)&pval->iVal;
    if(type == "double") atr.addr = (void*)&pval->dVal;
    if(type == "string") atr.addr = (void*)&pval->str;
    if(type == "func") atr.addr = (void*)&pval->str;
    if(type == "selector") atr.addr = (void*)&pval->str;
    if(type == "bool") atr.addr = (void*)&pval->boolVal;

    atr.SetPy();
    atr.type = type;
    atr.pobj = this;
    atr.idx  = atrs.size();
    atr.fromString(val);
    atrs.push_back(atr);
    pval->atr = &atrs.back();
    atrsmap[aname] = atr.idx;
}


void Object::Connect()
{
    if(connected==0)
    {
        //CSignal sig = CSignal(this, &gtimer, 0);
        //sig.Post();
    }
}

bool isUnknowAttrType(string type)
{
    bool b;
    b = (type != "int") && (type != "float") && (type != "string") && (type != "selector") && (type != "Pnt") && (type != "GLfloat*") && (type != "bool");
    b = b && (type != "Vector");
    return b;
}

/*
void Object::WriteAttr(QDataStream& archive,  t_atr& atr )
{
    //if(isUnknowAttrType)
    WriteString2(archive, atr.type);
    archive << atr.data;
    archive << atr.min;
    archive << atr.max;
    archive << atr.step;
    WriteString2(archive, atr.name);

    //attr size
    if( atr.type == "int")
        archive << *(int*)atr.addr;

    else if( atr.type == "float")
        archive << *(float*)atr.addr;

    else if( atr.type == "string")
        WriteString2(archive, *(string*)atr.addr);

    else if( atr.type == "selector")
        WriteString2(archive, *(string*)atr.addr);

    else if( atr.type == "Pnt")
        archive.writeRawData((char*)atr.addr, sizeof(Pnt));
    else if( atr.type == "GLfloat*")
    {
        GLfloat* col = (GLfloat*)atr.addr;
        archive << col[0];
        archive << col[1];
        archive << col[2];
        archive << col[3];
    }
    else if( atr.type == "bool")
        archive << *(bool*)atr.addr;
    else if( atr.type == "Vector")
        archive.writeRawData((char*)atr.addr, sizeof(Vector));
    else
    {
        //archive << sizeof(int);
    }

}


void Object::ReadAttr(QDataStream& archive,  t_atr& atr )
{
    int pint; float pfloat; string pstring; Pnt ppnt; GLfloat pglfloat[4]; bool pbool; Vector pvector;


    ReadString2(archive, atr.type);
    archive >> (quint32&)atr.data;
    int rp = archive.device()->pos();
    archive >> (float&)atr.min;
    rp = archive.device()->pos();
    archive >> (float&)atr.max;
    archive >> (float&)atr.step;
    atr.pobj = this;
    ReadString2(archive, atr.name);
    atr.idx = -1;

    //	int float string selector Pnt GLfloat* bool

    if( atr.type == "int")
        archive >> pint;

    if( atr.type == "float")
        archive >> pfloat;

    if( atr.type == "string")
        ReadString2(archive, pstring);

    if( atr.type == "selector")
        ReadString2(archive, pstring);

    if( atr.type == "Pnt")
        archive.readRawData((char*)&ppnt, sizeof(Pnt));

    if( atr.type == "GLfloat*")
    {
        archive >> pglfloat[0];
        archive >> pglfloat[1];
        archive >> pglfloat[2];
        archive >> pglfloat[3];
    }

    if( atr.type == "bool")
        archive >> *(int*)&pbool;

    if( atr.type == "Vector")
        archive.readRawData((char*)&pvector, sizeof(Vector));

    t_atr* b = GET_ATTR(atr.name);
    if(b->addr!=0)
    {
        b->data = atr.data;
        b->min = atr.min;
        b->max = atr.max;
        b->step = atr.step;

        //DATA
        if( b->type == "int")
            *(int*)b->addr = pint;

        if( b->type == "float")
            *(float*)b->addr = pfloat;

        if(b->type == "string")
            *(string*)b->addr = pstring;

        if(b->type == "selector")
            *(string*)b->addr = pstring;

        if( b->type == "Pnt")
            *(Pnt*)b->addr = ppnt;

        if( b->type == "GLfloat*")
        {

            GLfloat* col = (GLfloat*)b->addr;
            col[0] = pglfloat[0];
            col[1] = pglfloat[1];
            col[2] = pglfloat[2];
            col[3] = pglfloat[3];


        }
        if( b->type == "bool")
            *(bool*)b->addr = pbool;
        if( b->type == "Vector")
            *(Vector*)b->addr = pvector;

    }

}

void Object::ReadString(string& str, QDataStream& archive)
{
    str="";
    while(!archive.atEnd())
    {
        char c[2];
        archive.readRawData(c, 1);
        //if(c[0] == '\n')
        //    break;
        if(c[0] == 0)
            break;
        str+=c[0];
    }
}

void Object::WriteString(string& str, QDataStream& archive)
{
    string wstr = str + '\n';
    archive.writeRawData(wstr.c_str(), wstr.size());
}

void Object::ReadString2(QDataStream& archive, string& str)
{
    char gstr[1024];
    int sz;
    str="";
    archive >> sz;
    if(sz>=0 && sz<1000000)
    {
        archive.readRawData(gstr, sz);
        gstr[sz]=0;
        str = string(gstr);
    }

}

void Object::WriteString2( QDataStream& archive, string& str)
{
    archive << (int)str.size();
    archive.writeRawData(str.c_str(), str.size());
}

*/

void Object::FillEntetyInfo(string groupname, string material, string model)
{
    //CRuntimeClass* pMyRTClass= GetRuntimeClass();
    ei.name = name;
    //ei.classname = string(pMyRTClass->m_lpszClassName);
    ei.groupname = groupname;
    char gstr[1024];
    sprintf(gstr,"%d", this);
    ei.GUID = gstr;
    ei.material = material;
    ei.model = model;

}


bool t_atr::isNULL()
{
    t_atr& a = *this;
    if(a.type == "string")
    {
        return (((string*)a.addr)->size() == 0);
    }
    else if(a.type == "selector")
    {
        return (((string*)a.addr)->size() == 0);

    }
    else if(a.type == "int")
    {
        return (*(int*)a.addr == 0);
    }
    else if(a.type == "float")
    {
        return (*(float*)a.addr == 0);
    }

    else if(a.type == "GLfloat*")
    {
        GLfloat* col = (GLfloat*)a.addr;
        return ( (col[0]==0) && (col[1]==0) && (col[2]==0) && (col[3]==0) );
    }

    else if(a.type == "Pnt")
    {
        Pnt p = *(Pnt*)a.addr;
        return ( (p.x == 0) && (p.y == 0) && (p.z == 0));

    }
    else if(a.type == "Vector")
    {
        Vector p = *(Vector*)a.addr;

        return ( p.A.isNull() && p.B.isNull() );
    }
    else if(a.type == "bool")
    {
        //return !*(bool*)a.addr;
        return false;
    }
    else if(a.type == "time_t")
    {
        //return !*(bool*)a.addr;
        return false;
    }

    return false;
}


string t_atr::toString()
{
    char gstr[1024];
    t_atr& a = *this;
    sprintf(gstr,"");
    if(a.type == "string")
    {
        if( ((string*)a.addr)->size() < 1024 )
        sprintf(gstr, "%s", ((string*)a.addr)->c_str());

    }
    else if(a.type == "selector")
    {
        sprintf(gstr, "%d", a.data);

    }
    else if(a.type == "int")
    {

        sprintf(gstr, "%d", *(int*)a.addr);

    }
    else if(a.type == "float")
    {
        char b[20];
        sprintf(b,"%f",0.1);
        sprintf(gstr, "%f", *(float*)a.addr);
        if( b[1]==',' )
            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == ',') gstr[i] = '.';

    }

    else if(a.type == "GLfloat*")
    {
        GLfloat* col = (GLfloat*)a.addr;
        char b[20];
        sprintf(b,"%f",0.1);
        if( b[1]==',' )
        {
            sprintf(gstr, "[%0.4f;%0.4f;%0.4f;%0.4f]", col[0], col[1], col[2], col[3]);
            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == ',') gstr[i] = '.';
            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == ';') gstr[i] = ',';

        }
        else
            sprintf(gstr, "[%0.4f,%0.4f,%0.4f,%0.4f]", col[0], col[1], col[2], col[3]);
    }

    else if(a.type == "Pnt")
    {
        Pnt p = *(Pnt*)a.addr;
        char b[20];
        sprintf(b,"%f",0.1);
        if( b[1]==',' )
        {
            sprintf(gstr, "%f;%f;%f", p.x, p.y, p.z);
            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == ',') gstr[i] = '.';
            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == ';') gstr[i] = ',';
        }
        else
            sprintf(gstr, "%f,%f,%f", p.x, p.y, p.z);

    }
    else if(a.type == "Vector")
    {
        Vector p = *(Vector*)a.addr;
        char b[20];
        sprintf(b,"%f",0.1);
        if( b[1]==',' )
        {
            sprintf(gstr, "(%f;%f;%f) - (%f;%f;%f)", p.A.x, p.A.y, p.A.z,
                p.B.x, p.B.y, p.B.z	);

            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == ',') gstr[i] = '.';
            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == ';') gstr[i] = ',';
        }
        else
            sprintf(gstr, "(%f,%f,%f) - (%f,%f,%f)", p.A.x, p.A.y, p.A.z,
                p.B.x, p.B.y, p.B.z	);

    }
    else if(a.type == "bool")
    {
        if(*(bool*)a.addr == true)
            sprintf(gstr, "True");
        else
            sprintf(gstr, "False");
    }
    else if(a.type == "time_t")
    {
        time_t t = *(time_t*)(a.addr);
        sprintf(gstr, "%s",asctime(localtime(&t)));
        gstr[strlen(gstr)-1]=0;
    }

    return string(gstr);
}

void t_atr::fromString(string strval)
{
    t_atr& a = *this;
    char gstr[1024];


    strcpy(gstr, strval.c_str());

    if(a.type == "func")
    {
        *(string*)a.addr = gstr;
    }

    if(a.type == "string")
    {
        *(string*)a.addr = gstr;
    }
    else if(a.type == "selector")
    {
        int val;
        if(sscanf(gstr,"%d",&val) == 1)
            a.data = val;
    }
    else if(a.type == "int")
    {
        int val;
        if(sscanf(gstr,"%d",&val) == 1)
        {
            *(int*)a.addr = val;
        }
    }
    else if(a.type == "float")
    {
        float val;
        char b[2];
        sprintf(b,"%f",0.1);
        if( b[1]==',' )
            for(int i=0; i<(int)strlen(gstr); i++)  if(gstr[i] == '.') gstr[i] = ',';
        if(sscanf(gstr,"%f",&val) == 1)
            *(float*)a.addr = val;
    }
    else if(a.type == "Pnt")
    {
        float x,y,z;

        char b[2];
        sprintf(b,"%f",0.1);
        if( b[1]==',' )
        {
            for(int i=0; i<(int)strlen(gstr); i++) { if(gstr[i] == ',') gstr[i] = ';'; }
            for(int i=0; i<(int)strlen(gstr); i++) { if(gstr[i] == '.') gstr[i] = ','; }
            if(sscanf(gstr,"%f;%f;%f",&x,&y,&z) == 3)
                *(Pnt*)a.addr = Pnt(x,y,z);
            return;

        }

        if(sscanf(gstr,"%f,%f,%f",&x,&y,&z) == 3)
        {
            *(Pnt*)a.addr = Pnt(x,y,z);
        }
    }
    else if(a.type == "Vector")
    {
        Vector p;
        char b[2];
        sprintf(b,"%f",0.1);
        if( b[1]==',' )
        {
            for(int i=0; i<(int)strlen(gstr); i++) { if(gstr[i] == ',') gstr[i] = ';'; }
            for(int i=0; i<(int)strlen(gstr); i++) { if(gstr[i] == '.') gstr[i] = ','; }
            if(sscanf(gstr, "(%f;%f;%f) - (%f;%f;%f)", &p.A.x, &p.A.y, &p.A.z,
                &p.B.x, &p.B.y, &p.B.z	) == 6 )
                *(Vector*)a.addr = p;
            return;
        }

        if(sscanf(gstr, "(%f,%f,%f) - (%f,%f,%f)", &p.A.x, &p.A.y, &p.A.z,
            &p.B.x, &p.B.y, &p.B.z	) == 6 )
        {
            *(Vector*)a.addr = p;
        }

    }

    else if(a.type == "GLfloat*")
    {
        float x,y,z,al;
        char b[2];
        sprintf(b,"%f",0.1);
        if( b[1]==',' )
        {
            for(int i=0; i<(int)strlen(gstr); i++) { if(gstr[i] == ',') gstr[i] = ';'; }
            for(int i=0; i<(int)strlen(gstr); i++) { if(gstr[i] == '.') gstr[i] = ','; }

            if(sscanf(gstr,"[%f;%f;%f;%f]",&x,&y,&z,&al) == 4)
            {
                GLfloat col[4] = {x, y, z, al};
                memcpy(a.addr,&col,sizeof(col));
            }
            return;


        }
        if(sscanf(gstr,"[%f,%f,%f,%f]",&x,&y,&z,&al) == 4)
        {
            GLfloat col[4] = {x, y, z, al};
            memcpy(a.addr,&col,sizeof(col));
            //*(Pnt*)a.addr = Pnt(x,y,z);
            //ed2[atr_idx].SetBkColor(RED);
        }
    }
    else if(a.type == "bool")
    {

            if(string(gstr)=="True")
            {
                *(bool*)a.addr = true;
            }

            if(string(gstr)=="False")
            {
                *(bool*)a.addr = false;
            }

    }
}


void Object::SetName(std::string str)
{
    //MU_Trace << str;
    name = str;
    ei.name = str;
}

void Object::Rename(std::string str)
{
    //MU_Trace << str;
    name = str;
    SET_OBJECT((void*)this, (char*)str.c_str());
    ei.name = name;
}


void Object::SetClassName(std::string str)
{
    classname = str;

}


void Object::SET_ATTR(string name, void* addr, string type, bool bHidden)
{
    t_atr atr;

    if( atrsmap.find(name)!=atrsmap.end() )
        return;


    atr.flags = 0;

    if( type == "func")
       atr.NotSaving();

    if(bHidden) atr.Hide();
    atr.data = 0;
    atr.name = name;
    //писать относительный адрес
    atr.addr = addr;
    atr.type = type;
    atr.pobj = this;
    atr.idx  = atrs.size();
    if((int)expressions.size() < atr.idx)
        expressions.push_back("");

    atrs.push_back(atr);
    atrsmap[name] = atr.idx;
}

void Object::SET_STATE(string name, string posible_states, void* addr)
{
    t_atr atr;

    if( atrsmap.find(name)!=atrsmap.end() )
        return;

    atr.flags = 0;
    atr.data = 0;
    atr.data2 = posible_states;
    atr.name = name;
    //писать относительный адрес
    atr.addr = addr;
    atr.type = "state";
    atr.pobj = this;
    atr.idx  = atrs.size();
    atrs.push_back(atr);
    atrsmap[name] = atr.idx;
}


void Object::SET_ATTR(string name, void* addr, string type)
{
    t_atr atr;
    atr.data = 0;
    atr.name = string( name );
    atr.addr = addr;
    atr.type = type;
    atr.pobj = this;
    atr.idx  = atrs.size();
    nullat.addr=0;
    nullat.data=0;
    nullat.name=string("");

    if((int)expressions.size() < atr.idx)
        expressions.push_back("");

    atrs.push_back(atr);
    atrsmap[name] = atr.idx;
}

t_atr* Object::GET_ATTR(string name)
{
    for(int i=0; i<(int)atrs.size(); i++)
     if(atrs[i].name==name)
         return &atrs[i];

    return &nullat;
}

void* Object::SET_METHOD(char* parentclassname, char* argstr, char* classname, char* methname, ...)
{

    meta_entry me;
    int n = 0;
    va_list ap;
    va_start(ap, methname);
    void* meth_addr = va_arg(ap, void*);
    va_end(ap);

    vector<meta_entry > meths = metatable[classname];
    for(vector<meta_entry>::iterator j=meths.begin(); j!=meths.end(); j++)
    {
        if(string((*j).methname) == string(methname))
            return (*j).meth_addr;

    }

    char gstr[1024];
    strcpy(gstr,"\n");
    sprintf(gstr,",%s,",argstr);

    char* p1 = gstr;
    char* p2 = gstr;
    while(*(p1)!='\0')
    {
    if(*(p1) == ',')
    {
            strncpy(gstr, p2, p1 - p2 + 1);
            if(strstr(p1,",int,") == p1)
            {
                me.arg[n].type = T_INT;
                me.arg[n].i = 1;
                //meth_args[n_meths][n] =  me.arg[n];
                n++;
            }
            if(strstr(p1,",float,") == p1)
            {
                me.arg[n].type = T_FLOAT;
                me.arg[n].flt = 1.0;
                //meth_args[n_meths][n] =  me.arg[n];
                n++;
            }
            if(strstr(p1,",char*,") == p1)
            {
                me.arg[n].type = T_CSTR;
                strcpy(me.arg[n].cstr, gstr);
                //meth_args[n_meths][n] =  me.arg[n];
                n++;
            }
            p2 = p1;


    }
        p1++;
    }
    me.nargs = n;


    strcpy(me.classname, classname);
    strcpy(me.methname, methname);
    strcpy(me.parentclass, parentclassname);
    me.meth_addr = meth_addr;
    metatable[classname].push_back(me);

    return meth_addr;


}

int Object::SET_OBJECT(void* this_ptr, char* objname)
{
    this_ptr=0;
    objname=0;
    return 0;
}

void* Object::GET_OBJECT(char* objname)
{
    objname=0;
    return 0;
}

void Object::DELETE_OBJECT(char* objname)
{
    objname=0;
}

void Object::OnDrag(float x, float y, float z)
{
    ei.coord = ei.coord0;
    CInput* iinput = CInput::getInstance();

    if(y == 0)
    {
        ei.coord = Pnt(iinput->vx,0,iinput->vz);
        return;
    }

    if(z == 0)
    {
        ei.coord.x-= iinput->delta.x;
        ei.coord.y-= iinput->delta.y;
        return;
    }

    if(x == 0)
    {
        ei.coord.z-= iinput->delta.z;
        ei.coord.y-= iinput->delta.y;
        return;
    }



}
