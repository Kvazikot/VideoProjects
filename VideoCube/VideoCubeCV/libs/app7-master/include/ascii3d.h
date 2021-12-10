#ifndef CASCIILOADER_H
#define CASCIILOADER_H

#include <time.h>
#include <string>
#include <sstream>
#include "object.h"
#include "renderobject.h"
#include <QtOpenGL>
#include <QFile>
#include "animation.h"
#include "mesh.h"

//extern char gstr[512];


using namespace std;
struct t_col{	GLfloat col[4]; };

#pragma pack(push,1)
struct Node_TM
{

public:
    char NODE_NAME[1055];
    //char NODE_PARENT[1055];
    Pnt INHERIT_POS;
    Pnt INHERIT_ROT;
    Pnt INHERIT_SCL;
    Pnt TM_ROW0;
    Pnt TM_ROW1;
    Pnt TM_ROW2;
    Pnt TM_ROW3;
    Pnt TM_POS;
    Pnt TM_ROTAXIS;
    float TM_ROTANGLE;
    Pnt TM_SCALE;
    Pnt TM_SCALEAXIS;
    float TM_SCALEAXISANG;
};
#pragma pack(pop)



struct CMatMap
    {
    string  TAG;
    string	MAP_NAME;
    string	MAP_CLASS;
    int	MAP_SUBNO;
    float	MAP_AMOUNT;
    string BITMAP;
    string MAP_TYPE;
    float	UVW_U_OFFSET;
    float	UVW_V_OFFSET;
    float	UVW_U_TILING;
    float	UVW_V_TILING;
    float	UVW_ANGLE;
    float	UVW_BLUR;
    float	UVW_BLUR_OFFSET;
    float	UVW_NOUSE_AMT;
    float	UVW_NOISE_SIZE;
    float	UVW_NOISE_LEVEL;
    float	UVW_NOISE_PHASE ;
    string	BITMAP_FILTER;

};

struct CMaxMaterial
{
    vector<CMaxMaterial> submaterials;

    int     num;
    int	    NUMSUBMTLS;
    string	MATERIAL_NAME;
    string	MATERIAL_CLASS;
    GLfloat	MATERIAL_AMBIENT[4];
    GLfloat	MATERIAL_DIFFUSE[4];
    GLfloat	MATERIAL_SPECULAR[4];

    float	MATERIAL_SHINE;
    float	MATERIAL_SHINESTRENGTH;
    float	MATERIAL_TRANSPARENCY;
    float	MATERIAL_WIRESIZE;
    string	MATERIAL_SHADING;
    float	MATERIAL_XP_FALLOFF;
    float	MATERIAL_SELFILLUM;
    string	MATERIAL_FALLOFF;
    string	MATERIAL_XP_TYPE;
    CMatMap	mapdiffuse;


};


struct Vertex_knot
{
    int idx;
    Pnt c;
    bool knot; // узловая точка или интерпол
};

struct ShapeLine
{
    int SHAPE_VERTEXCOUNT;
    vector<Vertex_knot> vertex_knots;
};

//-----------------------------------------------------------


class ShapeObject : public Object
{
public:
    string NODE_NAME;
    string NODE_PARENT;
    Node_TM modetm;
    int SHAPE_LINECOUNT;
    vector<ShapeLine> shapelines;

    ShapeObject();
    void Declare()
    {
        classname = "ShapeObject";
        SetName("shape01");

        //SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "move2", move2);
        SET_OBJECT(this,(char*)name.c_str());
        SET_ATTR("SHAPE_LINECOUNT", &SHAPE_LINECOUNT, "int");
        SET_ATTR("name", &NODE_NAME, "string");

        SET_ATTR("parent", &NODE_PARENT, "string");

        // fill entety info
        //CRuntimeClass* pMyRTClass= GetRuntimeClass();
        ei.name = name;
        //ei.classname = string(pMyRTClass->m_lpszClassName);
        ei.groupname = "Shapes";
        char gstr[255];
        sprintf(gstr,"%d", this);
        ei.GUID = gstr;
        ei.material = "no material";
        ei.model = "no model";
    }
    virtual void Serialize( ){};

};

//-----------------------------------------------------------
string trim_quotes(string str);


//-----------------------------------------------------------
class HelperObject  : public Object
{
public:
    string NODE_NAME;
    string HELPER_CLASS;
    Node_TM modetm;
    Pnt BOUNDINGBOX_MIN;
    Pnt BOUNDINGBOX_MAX;
    HelperObject();
    void Declare()
    {
        //SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "move2", move2);
        //SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "scale", scale);
        //SET_METHOD("Object", "float,float,float",(char*)classname.c_str(), "rotate", rotate);

        name = NODE_NAME;
        //if(name=="")
        //	name="helper";
        SET_OBJECT(this,(char*)NODE_NAME.c_str());
        //SET_ATTR("NODE_NAME", &modetm.NODE_NAME, "string");
        SET_ATTR("HELPER_CLASS", &HELPER_CLASS, "string");
        SET_ATTR("TM_POS", &modetm.TM_POS, "Pnt");

        Object::Declare();

        // fill entety info

        ei.name = name;
        //CRuntimeClass* pMyRTClass= GetRuntimeClass();
        //ei.classname = string(pMyRTClass->m_lpszClassName);
        ei.groupname = NODE_NAME;
        char gstr[255];
        sprintf(gstr,"%d", this);
        ei.GUID = gstr;
        ei.material = "no material";
        ei.model = "no model";
    }
};

//-----------------------------------------------------------

class GroupObject: public Object
{
public:
    string NODE_NAME;
    string HELPER_CLASS;
    Pnt BOUNDINGBOX_MIN;
    Pnt BOUNDINGBOX_MAX;
    Node_TM nodetm;
};

const int	LINE_SIZE=100000;




class CAsciiLoader
{
public:
// Constructors
    CAsciiLoader();

    //ОПЦИИ ЗАГ� УЗКИ ASE файла
    bool bLoadAnimation;
    bool bLoadMaterials;
    bool bLoadInSingleGroup;

    vector<string>	info;
    std::string  filename;
    tm      time;
    int     version;
    int		NumVertex;
    int		NumFaces;
    float	**Vert; //указатель на вершины
    int     **Face; //указатель на вершины


    //ПЕ� ЕМЕННЫЕ ФАЙЛА
    std::string SCENE_FILENAME;
    int			SCENE_FIRSTFRAME;
    int			SCENE_LASTFRAME;
    int			MATERIAL_COUNT;
    int			NUMSUBMTLS;
    vector<CMaxMaterial> materials;

    //СОСТОЯНИЯ
    int numMaterial; // текущий
    int numSubMaterial;


    char*	buff;
    stringstream st;
    string       line;




    //ФЛАГИ
    bool fgMaterialList;
    bool fgMaterial;
    bool fgSubMaterial;
    bool fgScene;
    bool fgGeomObject;
    bool fgMesh;
    bool fgNodeTm;
    bool fgMeshFaceList;
    bool fgMap;
    bool fgHelperObj;
    bool fgAnimation;
    bool fgAnimControlPos;
    bool fgAnimControlRot;
    //номер строки
    int nstr;

    // Временные
    vector<Object*> objs;
    GemObject    gemobj;
    ShapeObject  shapeobj;
    HelperObject  helpobj;
    Node_TM       nodetm;
    Animation	  anim;
    Animations	  animations;
    CMatMap	   mtmap;
    CMaxMaterial  mt;
    CMaxMaterial  submt;
    CMaxMaterial* pmt;
    FILE *f,*fout;

    int cnt;
    int size;

    void ProcessMesh();
    char* NextLine();
    void Reset();
    void AssignMt()
    {
        if(fgMaterial)
            pmt = &mt;
        if(fgSubMaterial)
            pmt = &submt;

    }
    void ParseMaterials(char* buff);
    void ResetFlags()
    {
        if(fgMaterialList)
        fgMaterialList = 0;
        if(fgScene)
        fgScene = 0;
        if(fgMaterialList)
        fgMaterialList = 0;
        if(fgMaterial)
        fgMaterial = 0;
        if(fgSubMaterial)
        fgSubMaterial = 0;
        if(fgScene)
        fgScene = 0;
        if(fgGeomObject)
        fgGeomObject = 0;
        if(fgMesh)
        fgMesh = 0;
        if(fgNodeTm)
        fgNodeTm = 0;
        if(fgMeshFaceList)
        fgMeshFaceList = 0;
        if(fgMap)
        fgMap = 0;


    }


    //~CAsciiLoader();
    int LoadASCIIIFile(char* ASC3d_filename, char* objname);

    void GetInfo();

    //void Slot1(CSignal* sig); //timer slot

protected:

};

extern CAsciiLoader asciloader;

#endif
