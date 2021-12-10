#ifndef MTERRAIN_H
#define MTERRAIN_H

#include "stdafx.h"
#include "terrain.h"
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
    GemObject* terrain_mesh;
    Pnt scale;
    MeshProps mprops;

    PPnt* set_pnt(PPnt* p, int i);
    PPnt* get_pnt(int i);
    bool   IsIdxValid(int i);
    //void CreateFlatGround();
    void Create(int w_segs, int h_segs, int l_segs, float sizex, float sizey, float sizez);
    void CreateLinkedLists();
    void Render();
    void Slot1();
    bool FindTerrainObject(string goname);
    void SetBumpImage(QImage& I);
    void Generate(int detail, float roughness, float hconst);
    void Declare()
    {
        SET_ATTR("selvert",&selvert,"int");
        SET_ATTR("scale",&scale,"Pnt");
    }

    // Interface to vehicle
    PPnt*  FindClosestTo(Pnt p, int from_idx,  int count, float delta);
    PPnt*  FindNearestP(Pnt& p);

};

#endif // MTERRAIN_H
