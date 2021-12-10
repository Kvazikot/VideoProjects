#ifndef COBJREGISTRY_H
#define COBJREGISTRY_H

#include <QObject>
#include <QFile>
#include "object.h"

class CObjRegistry
{
 //   Q_OBJECT
public:
    bool ok;

    CObjRegistry();

    void Init();

    //группы
    std::map<string,Object*> objMap;
    std::vector<Object*> allObj;
    std::vector<Object*> newObjs;
    std::vector<Object*> removeQuenue;

    //пользовательские группы
    //std::vector<Group> groups;

    Object* FindObj(string name);
    vector<Object*> FindObjsByClass(string classname);
    Object* CreateObj(char* objname, char* classname);
    Object* CreateObj(string descr_str, bool madeLoad);
    Object* DeepFindObj(string name);
    void    SaveObjsStates(vector<string>& slist);
    void    LoadObjsStates(vector<string> slist);
    void  Clear(); // очистка сцены
    void  Load(); // загружает сцену ( возможно будет создаваться по классу CSpace )

    void  Load_OLD();
    void  Save(); // сохраняет сцену
    void  SaveScene(bool bWriteNullAttrs);
    void  AddObj(Object* obj);
    void  AddObjNew(Object* obj);
    int	  AddEntety(EntetyInfo en);
    int	  RemoveEntety(EntetyInfo en);
    void  RemObj(Object* obj);
    void  CopyObj(Object* obj);
    void  ListObjs();
    void  ReLoadObj(string name);
    void  UpdateNew();
    void  SetSceneFile(string sf);
    vector<Object*> FindObjByClass(string classname);

    void skip2signature(QDataStream& oar); //поиск слудующей записи
    map<int,EntetyInfo> entetytable;
    QFile				classesFile;
    QFile				objFile;
    QFile               offsets_file;
    string              scenefile;

    static CObjRegistry        instance;
    static CObjRegistry        layer_instance;
    static CObjRegistry*   getInstance();
    static CObjRegistry&   getInstance2();
    static CObjRegistry&   getLayerInstance();


signals:

public slots:

};

Object* MREG_FindObj(string name);

#endif // COBJREGISTRY_H
