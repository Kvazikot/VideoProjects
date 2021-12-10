#ifndef MTERRAIN_H
#define MTERRAIN_H

#include "terrain.h"
#include "geometryobject.h"
#include <QImage>
#include <QPair>
#include <QMap>

struct MeshProps
{
    int w_segs;
    int h_segs;
    int l_segs;
    float sizex;
    float sizey;
    float sizez;
};

class MTerrain : public RenderObject
{
 //   Q_OBJECT
public:
    MTerrain();
    MTerrain(const MTerrain& B){ scale = B.scale; }
    int selvert;
    vector<PPnt*>   vertL;
    vector<PFace*>   faceL;
    QMap<QPair<int,int>,int> index_map;
    QMap<int,QPair<int,int> > reverse_index_map;
    GeometryObject* terrain_mesh;
    QImage          height_map;
    QImage          bump_texture;
    GLuint          texture;
    float           ylevel;
    int             var;
    Pnt scale;
    MeshProps mprops;

    PPnt* set_pnt(PPnt* p, int i);
    PPnt* get_pnt(int i);
    bool   IsIdxValid(int i);
    //void CreateFlatGround();
    void Create(int w_segs, int h_segs, int l_segs, float sizex, float sizey, float sizez, float ylevel);
    void CreateLinkedLists();
    void AssignTextureCoords();
    void AssignTextureCoords2();
    void AssignNormals();    
    void Render(QOpenGLShaderProgram& program);
    void Slot1();
    void SetBumpImage(QImage& I, float h);
    void SendToGPU();
    void Generate(int detail, float roughness, float hconst);
    bool isGround(PPnt* A, PPnt* B, PPnt* C, PPnt* D);
    void heightMapFromMesh();
    void Declare()
    {
        SET_ATTR("selvert",&selvert,"int");
        SET_ATTR("scale",&scale,"Pnt");
        SET_ATTR("ylevel",&ylevel,"float");
        SET_ATTR("var",&var,"int");
        SET_ATTR("isCanRender",&isCanRender,"bool");
        classname = "MTerrain";
    }

    // Interface to vehicle
    PPnt*  FindClosestTo(Pnt p, int from_idx,  int count, float delta);
    PPnt*  FindNearestP(Pnt& p);

};

#endif // MTERRAIN_H
