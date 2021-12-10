/**********************************************************************
 *  Project App6
 *
 *  cobjregistry.cpp
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

#include <QMetaType>
#include <time.h>
#include <stdio.h>
#include "../include/cobjregistry.h"
#include "../include/print.h"
#include "../include/cinput.h"
#include "../include/var_parse.h"
#include "../include/group.h"
#include "../include/flags.h"

CObjRegistry CObjRegistry::instance;
CObjRegistry CObjRegistry::layer_instance;

static CInput& iinput = CInput::getInstance2();

CObjRegistry::CObjRegistry()
{

}

CObjRegistry*   CObjRegistry::getInstance()
{
    return &instance;
}

CObjRegistry&   CObjRegistry::getLayerInstance()
{
    return layer_instance;
}

CObjRegistry&   CObjRegistry::getInstance2()
{
    return instance;
}


Object* MREG_FindObj(string name)
{    
    CObjRegistry* MREG = CObjRegistry::getInstance();
    return MREG->FindObj(name);
}

void  CObjRegistry::SetSceneFile(string sf)
{
    scenefile = sf;
}

Object* CObjRegistry::CreateObj(char *objname, char *classname)
{
    Object* obj = 0;
    t_flags& flags = GetGFlags();
    int id = QMetaType::type(classname);
    if(id!=0)
    {
        obj = (Object*)QMetaType::create(id);
        if( obj == NULL )
            return NULL;
        obj->SetName(objname);
        obj->Declare();
        AddObj(obj);
        prn("MREG Created object name=%s class=%s",objname, classname);
    }
    flags.bNeedUpdObjectList = true;

    return obj;
}

void  CObjRegistry::AddObj(Object* obj)
{
    t_flags& flags = GetGFlags();
    allObj.push_back(obj);
    //newObjs.push_back(obj);
    if(obj->name!="")
        objMap[obj->name] = obj;
    else
        objMap["unknow#$%^&"] = obj;

    if(obj->name.size()>0)
        prn("REGISTRY: Added object %s", obj->name.c_str());
    flags.bNeedUpdObjectList = true;
}

void  CObjRegistry::AddObjNew(Object* obj)
{
    t_flags& flags = GetGFlags();
    newObjs.push_back(obj);
    if(obj->name!="")
        objMap[obj->name] = obj;
    else
        objMap["unknow#$%^&"] = obj;

    if(obj->name.size()>0)
        prn("REGISTRY: Added object %s", obj->name.c_str());
    flags.bNeedUpdObjectList = true;
}


void  CObjRegistry::UpdateNew()
{
    if(newObjs.size() > 0)
    {
        vector<Object*>::iterator i;
        for(i=newObjs.begin(); i!=newObjs.end(); i++)
        {
            allObj.push_back(*i);
        }
        newObjs.clear();
    }
}

vector<Object*> CObjRegistry::FindObjByClass(string classname)
{
    vector<Object*> out;
    vector<Object*>::iterator i;
    for(i = allObj.begin(); i != allObj.end(); i++ )
    {
        if( (*i)->classname == classname || (*i)->ei.classname == classname )
            out.push_back(*i);
    }
    return out;
}

Object* CObjRegistry::FindObj(string name)
{
    vector<Object*>::iterator obj;

    if( objMap.find(name)!= objMap.end() )
    {
        return objMap[name];
    }
/*
    for(obj = allObj.begin(); obj != allObj.end(); obj++ )
    {
        if((*obj)->name == name)
            return (*obj);
    }
*/
    return 0;
}

Object* CObjRegistry::DeepFindObj(string name)
{
  map<string,Object*>::iterator obj;
    obj = objMap.find(name);
    if( obj == objMap.end())
    {
        for(unsigned int i=0; i<allObj.size(); i++)
            //if(allObj[i]->isGroup )
            if(allObj[i]->classname == "Group")
            {
                Group* g = (Group*)allObj[i];
                Object* o = g->FindObj(name);
                if(o!=NULL)
                    return o;
            }
        return 0;
    }
    else
        return obj->second;


}

void CObjRegistry::ListObjs()
{
    vector<Object*>::iterator i;
    for(i = allObj.begin(); i != allObj.end(); i++ )
    {
        prn((*i)->name.c_str());
    }

}

void CObjRegistry::CopyObj(Object* obj)
{
    if(obj==NULL) return;
/*
    Object* objc;// = (Object*) obj->GetRuntimeClass()->CreateObject();
    *objc = *obj; // Object::
    objc->Copy(obj);

    AddObj(objc);
    objc->Declare();
*/

    prn("REGISTRY: Copy object %s to %s", obj->name.c_str(), obj->name.c_str());


    return;

}


void CObjRegistry::RemObj(Object* obj)
{
    vector<Object*>::iterator i,i2;
    t_flags& flags = GetGFlags();
    flags.bdisable_timer1 = true;
    flags.bdisable_timer2 = true;
    flags.bdisable_timer3 = true;

    obj->isCanRender = false;
    //from map
    if( FindObj(obj->name)!=0 )
        objMap.erase(objMap.find(obj->name));

    //from delegates
    for(i = iinput.delegates.begin(); i != iinput.delegates.end(); i++ )
    {
        if((*i) == obj)
        {
            iinput.delegates.erase(i);
            break;
        }

        if(i==iinput.delegates.end())
            break;
    }

    //first delete references in groups

    for(i = allObj.begin(); i != allObj.end(); i++ )
    {
        if((*i)->isGroup)
        {


        }

        if((*i)->name == obj->name)
        {
            Object::DELETE_OBJECT((char*)obj->name.c_str());
            allObj.erase(i);
            break;
        }

    }

    mu_printf("REGISTRY: Removed object %s", obj->name.c_str());
    delete obj;
    //MREG.ListObjs();
    flags.bdisable_timer1 = false;
    flags.bdisable_timer2 = false;
    flags.bdisable_timer3 = false;
    return;

}

//    Object    GemObject  .mesh
void CObjRegistry::SaveScene(bool bWriteNullAttrs)
{
    vector<string> slist;
    vector<string> slist2;
    int   totalVertexes=0;
    int   totalMeshSize=0;
    int   NMeshes=0;
    string objlist;
    string s;
    string ss;
    Object* o;
    time_t  t;
    t_atr   dma;
    t_atr&  a = dma;
    char  gstr[1024];


    for(int i=0; i<3; i++)	slist.push_back(" ");
    time(&t);
    sprintf(gstr,"--  app5 %d   %s", allObj.size(), asctime(localtime(&t)));
    gstr[strlen(gstr)-1] = 0;
    slist[0]=gstr;
    for(unsigned int i=0; i<allObj.size(); i++)
    {
        o = allObj[i];
        if(o->isNotSaving) continue;
        s = o->classname + ":" + o->name + "{";
        ss="";
        objlist+= o->classname + ":" + o->name + " ";
/*        if( o->classname == "GemObject" )
        {
            GemObject* go = (GemObject*)o;
            totalMeshSize+=go->vert_count*3*4 + go->face_count*3*4;
            totalVertexes+=go->vert_count;
            NMeshes++;
            if( bSaveMeshes )
                go->Save(ss); //        .mesh
        }
        else
            o->Save(ss); //
*/

        for(unsigned int j=0; j<o->atrs.size(); j++)
        {
            a = o->atrs[j];
            if( a.isNotSaving() ) continue;
            if( !bWriteNullAttrs )
            {
                if(!a.isNULL())
                    s+=a.name + "=" + a.toString() + " ";
            }
            else
                s+=a.name + "=" + a.toString() + " ";
        }
        s+="}";
        s+="\n";
        o->Save(ss);
        if(ss!="") //    .  _DATA
        {
            ss=o->name+"_DATA{" + ss + "}";
            slist.push_back(ss);
        }
        slist.push_back(s); //

    }

    sprintf(gstr,"--totalVertexes %d totalMeshSize %d NMeshes %d", totalVertexes, totalMeshSize, NMeshes);
    slist[1]=gstr;

    // save commented lines
    FILE* f = fopen("objects.ini","r");
    int cnt=0;
    if( f!=NULL )
    {
        while(!feof(f))
        {
            fgets(gstr,10000,f);
             if (strstr(gstr,"--")!=NULL )
             {
                if(cnt > 1) slist2.push_back(gstr);
                cnt++;
             }
        }
        fclose(f);
    }



    f = fopen("objects.ini","w+");
    if( f!=NULL )
    {
        for(unsigned int i=0; i<slist.size(); i++)
            fprintf(f,"%s\n",slist[i].c_str());
        for(unsigned int i=0; i<slist2.size(); i++)
            fprintf(f,"%s\n",slist2[i].c_str());
        fclose(f);
        //ShellExecute(0, _T("open"), _T("notepad.exe"), _T("objects.ini"), 0, SW_NORMAL);
    }


}

void CObjRegistry::SaveObjsStates(vector<string>& slist)
{
    string s;
    string ss;
    Object* o;
    t_atr   dma;
    t_atr&  a = dma;

    for(unsigned int i=0; i<allObj.size(); i++)
    {
        o = allObj[i];
        s = o->classname + ":" + o->name + "{";
        ss="";
        for(unsigned int j=0; j<o->atrs.size(); j++)
        {
            a = o->atrs[j];
            if( a.isNotSaving() ) continue;
            s+=a.name + "=" + a.toString() + " ";
        }
        s+="}";
        slist.push_back(s);
    }
}

void CObjRegistry::LoadObjsStates(vector<string> slist)
{
    VarParser var_parser;
    string s;
    vector<MyVar> vars;
    string objname, classname;
    t_flags& flags = GetGFlags();

    flags.bIsLoading = true;

    for(unsigned int i=0; i<slist.size(); i++)
    {
        s=slist[i];
        vars.clear();
        var_parser.ParseString(s, vars);

        if(vars.size() > 0)
        {
            vector<string> l2 = StrSplitE(vars[0].tablename,":",false);
            if( l2.size() == 2 )
            {
                objname		= l2[1];
                classname	= l2[0];
            }
            else
                continue;

            Object* o = FindObj(objname);
            if (o == 0) continue;
            o->isLoading = true;
            for(unsigned int j=0; j<vars.size(); j++)
            {
                if( o->atrs.size() != 0 )
                {
                    t_atr* a = o->GET_ATTR(vars[j].name);
                    if( a->addr != 0 )
                        a->fromString(vars[j].val);
                }
            }
        }

    }

    flags.bIsLoading = false;
}


Object* CObjRegistry::CreateObj(string descr_str, bool madeLoad)
{
    Object* o=0;
    VarParser var_parser;
    vector<MyVar> vars;
    string objname, classname, s;

    var_parser.ParseString(descr_str, vars);

    for(uint j=0; j<vars.size(); j++)
        if(vars[j].name == "loadArgs")
            s = vars[j].val;

    if(vars.size() > 0)
    {
        vector<string> l2 = StrSplitE(vars[0].tablename,":",false);
        if( l2.size() == 2 )
        {
            objname		= l2[1];
            classname	= l2[0];
        }
        else
            return 0;

       Object* eo = FindObj(objname);
       if(eo!=NULL && eo->classname == classname)
           o = eo;
       else
         o = CreateObj((char*)objname.c_str(), (char*)classname.c_str());

        if (o == 0) return 0;

        o->isLoading = true;
        for(unsigned int j=0; j<vars.size(); j++)
        {
            if( o->atrs.size() != 0 )
            {
                t_atr* a = o->GET_ATTR(vars[j].name);
                if( a->addr != 0 )
                    a->fromString(vars[j].val);
            }
        }
        if( madeLoad )
            o->Load(s);
        o->isLoading = false;
        return o;

    }
     return 0;
}

//loading by objects.ini
void CObjRegistry::Load()
{
    VarParser var_parser;
    string s,ss,sss;
    vector<string> slist;
    vector<MyVar> vars;
    string objname, classname;
    Object* o;
    char    gstr[1024];

    //Load_OLD(); return;
    t_flags& flags = GetGFlags();

    flags.bIsLoading = true;

    prn("CObjRegistry loading...");

    FILE* f;
    if( scenefile.size()>0 )
       f = fopen(scenefile.c_str(),"r");
    else
       f = fopen("objects.ini","r");

    if( f!=NULL )
    {
        while(!feof(f))
        {
            //fscanf(f,"%[^\n]*",gstr);
            fgets(gstr,10000,f);
            slist.push_back(gstr);
        }
        fclose(f);
    }
    else
    {prn("cant find scenefile..."); return;}

    slist.pop_back();

    for(unsigned int i=0; i<slist.size(); i++)
    {
        s=slist[i];
        if(s[0]=='-') continue;
        vars.clear();

        { //section _DATA
            if( s.find("_DATA")!=string::npos )
            {
                string tablename = strf(s, "{", 0);
                tablename=StrRep(tablename, " ", "");
                int p = s.find_first_of("{");
                ss = s.substr( p, s.size() - p );
                ss=StrRep(ss, "{", "");
                ss=StrRep(ss, "}", "");
                vector<string> l = StrSplitE(tablename,"_",false);
                continue;
            }
        }

        var_parser.ParseString(s, vars);

        for(uint j=0; j<vars.size(); j++)
           if(vars[j].name == "loadArgs")
                sss = vars[j].val;

        if(vars.size() > 0)
        {
            vector<string> l2 = StrSplitE(vars[0].tablename,":",false);
            if( l2.size() == 2 )
            {
                objname		= l2[1];
                classname	= l2[0];
            }
            else
                continue;

           Object* eo = FindObj(objname);
           if(eo!=NULL && eo->classname == classname)
               o = eo;
           else
             o = CreateObj((char*)objname.c_str(), (char*)classname.c_str());


            if (o == 0) continue;
            o->isLoading = true;
            //AddObj(o);
            //AutoGroupAssign(o, (char*)classname.c_str());
            for(unsigned int j=0; j<vars.size(); j++)
            {
                if( o->atrs.size() != 0 )
                {
                    t_atr* a = o->GET_ATTR(vars[j].name);
                    if( a->addr != 0 )
                        a->fromString(vars[j].val);
                }
            }
            o->Load(ss);

            for(unsigned int j=0; j<o->atrs.size(); j++)
                //if(!o->atrs[i].isNotSaving())
                    o->OnAttrChange(o->atrs[j]);

            o->isLoading = false;

        }

    }

    flags.bIsLoading = false;
}

void CObjRegistry::Clear()
{
    vector<Object*> ptrCopys;
    vector<Object*>::iterator i;
    //delete all shit
    ptrCopys = allObj;
    for(i = ptrCopys.begin(); i != ptrCopys.end(); i++ )
    {
        if(!(*i)->isNotSaving)
            RemObj(*i);
    }


}
