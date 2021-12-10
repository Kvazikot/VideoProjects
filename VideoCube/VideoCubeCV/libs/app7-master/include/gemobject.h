#ifndef GEMOBJECT_H
#define GEMOBJECT_H

#include <QtOpenGL>
#include <QFile>
#include "renderobject.h"
#include "material.h"
#include "vertexdata.h"
#include "mesh.h"

struct t_col{	GLfloat col[4]; };

#pragma pack(push,1)
struct Node_TM
{

public:
    char NODE_NAME[1055];
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


class GemObject :  protected QGLFunctions, public RenderObject
{
 //   Q_OBJECT

 public:
    QMatrix4x4 model_matrix;
    string NODE_NAME;
    string NODE_PARENT;
    Material* material;
    Node_TM nodetm;
    Mesh	mesh;
    Pnt     rot;
    Pnt     rot0;
    Pnt     loc;
    int     dir;
    bool    bIsBiped;
    bool firsttime;
    int   renderMode;
    bool   bUpdateMesh;
    float* vertices;
    float* texcoords;
    float* normals;
    std::vector<VertexData> vertices2;
    QVector2D ranges_array[50];
    vector<GLushort> inds;
    unsigned int*  indexes;
    float*   colorArray;
    int*   mat_id_array;

    int    vert_count;
    int    face_count;
    vector<t_range> ranges;
    int             n_ranges;

    Object*  myGroup;
    GLuint  vboIds[3];


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


    void LoadFIO(FILE* f,Object* group);
    void InitVBO();
    void CopyMesh(string goname);
    void Copy(Object* A);

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
    }


    void LoadMesh(string goname);
    void NullReferences()
    {
        vertices = NULL;
        vert_count = 0;
        face_count = 0;
        indexes = NULL;

    }
    void ComputeC0();
    void FillArrays();
    void FillMesh();
    void update();
    void CenterModel();
    void OnSelect();
    void OnUnselect();
    void drawpoly3d(Pnt P1,Pnt P2,Pnt P3,Pnt P4);
    void ScaleGeometry(Pnt sf);
    void RenderVer0(QOpenGLShaderProgram& program);
    void ComputeBounds();


    void Declare()
    {
        if(name.size()==0)
            SetName("mesh01");
        SET_OBJECT(this,(char*)name.c_str());
        SET_ATTR("NAME", &NODE_NAME, "string");
        //SET_ATTR("PARENT", &NODE_PARENT, "string");
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

        ei.name = name;
        ei.classname = string("GemObject");
        classname = ei.classname;
        ei.groupname = "Geometry";
        char gstr[255];
        sprintf(gstr,"%d", this);
        ei.GUID = gstr;
        ei.material = "no material";
        ei.model = "no model";
        Object::Declare();

    }
     void Render(QOpenGLShaderProgram& program);



};


#endif // GEMOBJECT_H
