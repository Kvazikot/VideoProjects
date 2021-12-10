#ifndef GROUP_H
#define GROUP_H

#include <QFile>
#include <QOpenGLShaderProgram>
#include "object.h"
#include "renderobject.h"
//====================================================================
//                  ÃÐÓÏÏÀ ÎÁÜÅÊÒÎÂ
//====================================================================


class Group: public RenderObject
{
 //   Q_OBJECT
public:
    Group():RenderObject()
    {
        isGroupMovable = true;
        isGroup = true;
    }
    Group(Group* B)
    {
       list = B->list;
    }
    Group(const Group& B)
    {
       list = B.list;
    }

    std::vector<Object*> objs;
    std::map<string,Object*> obj_map;
    string objnames;
    vector<string> lobjnames;
    int            list;
    int            n_obj;
    bool	 isGroupMovable;
    void	 ScaleGeometry(Pnt sf);
    void Load(string& s);
    void LoadFIO();
    void Save(string& s);
    virtual void Declare()
    {
        if( (classname=="Object") || (classname=="") )
            classname = "Group";
        //SET_OBJECT(this,(char*)name.c_str());
        bHideStdAttrs = true;
        SET_ATTR("owns objects",&objnames,"string"); atrs.back().NotSaving();
        SET_ATTR("n_obj",&n_obj,"int");
        SET_ATTR("coord",&ei.coord,"Pnt");
        SET_ATTR("rot",&ei.rot,"Pnt");
        SET_ATTR("scale",&ei.scale,"Pnt");
        Object::Declare();
    }
    virtual void Render(QOpenGLShaderProgram& program);

    void AddObj(Object* obj)
    {
        if(obj == NULL) return;
        lobjnames.push_back(obj->name);
        objnames = objnames + " " + obj->name;
        obj->groupname = name;
        objs.push_back(obj);
        n_obj = objs.size();
        obj_map[obj->name] = obj;
        //mu_printf("obj %s added to %s group", obj->name.c_str(), name.c_str());
    };

    Object* FindObj(string name)
    {
        vector<string>::iterator it;

//        for(int i=0; i<objs.size(); i++)
//        {
//            if( objs[i]->name == name)
//                return objs[i];
//        }

        if(obj_map.find(name)!=obj_map.end())
                return obj_map[name];
            else
                return 0;

    }

    void RemObj(Object* obj)
    {
        vector<string>::iterator it;
        it = find(lobjnames.begin(), lobjnames.end(), obj->name);
        if(it!=lobjnames.end())
        {
            lobjnames.erase(it);
            objnames="";
            for(it=lobjnames.begin(); it!=lobjnames.end();it++)
            {
                objnames = objnames + " " + *it;
            }
        }

        obj->groupname = "";
        //mu_printf("obj %s removed from %s group", obj->name.c_str(), name.c_str());

        //it = find_if (myvector.begin(), myvector.end(), IsOdd);
        objs.erase(find(objs.begin(), objs.end(), obj));
    };

    void Clear()
    {
        lobjnames.clear();
        objs.clear();
    }

    void AssignMyCoord()
    {
        vector<Object*>::iterator i;
        for(i = objs.begin(); i!=objs.end(); i++)
        {
            (*i)->ei.coord = ei.coord;
        }
    }

    void SelectAll()
    {
        vector<Object*>::iterator i;
        for(i = objs.begin(); i!=objs.end(); i++)
        {
            (*i)->isSelected = 2;
            (*i)->OnSelect();
        }
    }

    void UnSelectAll()
    {
        vector<Object*>::iterator i;
        for(i = objs.begin(); i!=objs.end(); i++)
        {
            (*i)->isSelected = 0;
            (*i)->OnUnselect();
        }
    }


};



#endif // GROUP_H
