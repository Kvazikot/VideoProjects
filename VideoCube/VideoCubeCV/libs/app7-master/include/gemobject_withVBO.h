#ifndef GEMOBJECT_H
#define GEMOBJECT_H

#include <QtOpenGL>
#include <QFile>
#include "renderobject.h"
#include "mesh.h"
#include "tranform.h"

struct t_col{	GLfloat col[4]; };

struct t_clipping_params
{
    bool crop;
    double clipu;
    double clipv;
    double clipw;
    double cliph;
};

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

class GemObject : public RenderObject
{
 //   Q_OBJECT

 public:
    enum RotateMode {EulerAngles, AxisAngle};
    RotateMode rotMode;
    t_clipping_params  clipping;
    string NODE_NAME;
    string NODE_PARENT;
    Transform  transform;
    Transform  transform_new;
    Node_TM nodetm;
    Mesh	mesh;
    Pnt     rot;
    Pnt     rot0;
    Pnt     loc;
    int     dir;
    bool    bScaleToOnes;
    bool    bNotSerializeBase;
    bool    bTransformReady;
    float   c;
    t_col  ar[11];
    bool firsttime;
    string fn;

    int   renderMode;

    int    idx;
    bool   bUpdateMesh;
    float* vertices;
    float* normals;
    float* texcoords;
    unsigned int*  indexes;
    unsigned int*  faces;
    float*   colorArray;
    string texture_file;
    int    vert_count;
    int    face_count;
    vector<t_range> ranges;
    int list;
    Object*  myGroup;
    QGLBuffer vertexDataVBO, normalDataVBO, textureDataVBO, indexDataVBO;

    GemObject();
    ~GemObject()
    {
        //mu_printf("destroying %s", name.c_str());
        vert_count = 0; face_count= 0;

        delete vertices;
        delete indexes;
    }
    GemObject(GemObject* B);

    GemObject(const GemObject& B);

    void Load(string& s);
    void Save(string& s);
    void LoadFIO(FILE* f,Object* group);
    void CopyMesh(string goname);
    void Copy(Object* A)
    {
        GemObject* B = (GemObject*)A;
        name = "aa";
        m_Updated = 1;
        isCanRender = true;
        isEntety = true;
        isNotSaving = false;
        classname = "GemObject";


        NODE_NAME = B->NODE_NAME;
        NODE_PARENT = B->NODE_PARENT;
        nodetm = B->nodetm;
        rot = B->rot;
        rot0 = B->rot0;
        C0 = B->C0;
        memcpy(ar, B->ar, sizeof(t_col)*11);

        bounds = B->bounds;

        vert_count = B->vert_count;
        face_count = B->face_count;

        vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
        indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));

        memcpy(vertices, B->vertices, B->vert_count * sizeof(float) * 3);
        memcpy(indexes, B->indexes, B->face_count * sizeof(int) * 3);

        ranges = B->ranges;
    }

    void FreeMem();

    void OnAttrChange(t_atr& a);

    void SetNumFaces(int n)
    {
        face_count = n;
        if(indexes == NULL)
            indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));
        else
            indexes = (unsigned int*)realloc(indexes, (face_count+1)*3*sizeof(int));
    }

    void SetNumVert(int n)
    {
        vert_count = n;
        if(vertices == NULL)
            vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
        else
            vertices = (float*)realloc(vertices,(vert_count+1)*3*sizeof(float));
        if(texcoords == NULL)
            texcoords = (float*)malloc((vert_count+1)*2*sizeof(float));
        else
            texcoords = (float*)realloc(texcoords,(vert_count+1)*2*sizeof(float));
    }


    void LoadMesh(string goname);
    void NullReferences()
    {
        vertices = NULL;
        vert_count = 0;
        face_count = 0;
        indexes = NULL;

    }

    void Serialize(QFile& archive, bool IsStoring );
    void ComputeC0();
    void FillArrays();
    void FillMesh();
    void CenterModel();
    void OnSelect();
    void OnUnselect();
    void drawpoly3d(Pnt P1,Pnt P2,Pnt P3,Pnt P4);
    void ScaleGeometry(Pnt sf);
    void ComputeBounds();


    void Declare()
    {
        if(name.size()==0)
            SetName("mesh01");
        SET_OBJECT(this,(char*)name.c_str());
        SET_ATTR("NAME", &NODE_NAME, "string");
        //SET_ATTR("PARENT", &NODE_PARENT, "string");
        SET_ATTR("filename", &fn, "string");
        SET_ATTR("TM_POS", &nodetm.TM_POS, "Pnt");
        SET_ATTR("ROT0", &rot0, "Pnt");
        SET_ATTR("ei.coord", &ei.coord, "Pnt");
        SET_ATTR("rot", &rot, "Pnt");
        SET_ATTR("scale", &ei.scale, "Pnt");
        //SET_ATTR("bScaleToOnes", &bScaleToOnes, "bool");
        SET_ATTR("renderMode", &renderMode, "int"); atrs.back().ReadOnly();
        SET_ATTR("VERT", &vert_count, "int");
        SET_ATTR("FACES", &face_count, "int");
        SET_ATTR("bounds", &bounds, "Pnt");

//		SetName(NODE_NAME);

        // fill entety info
        //CRuntimeClass* pMyRTClass= GetRuntimeClass();
        ei.name = name;
        ei.classname = string("GemObject");
        classname = ei.classname;
        ei.groupname = "Geometry";
        char gstr[255];
        sprintf(gstr,"%d", this);
        ei.GUID = gstr;
        ei.material = "no material";
        ei.model = "no model";
        //ei.coord = nodetm.TM_POS;

        Object::Declare();

    }
    void Slot1()
    {
    }; //timer slot

    void Test1()
    {
        //mu_printf("ok");
    }

    void Render();
    void RenderOLD();
    void RenderVer0();


};


#endif // GEMOBJECT_H
