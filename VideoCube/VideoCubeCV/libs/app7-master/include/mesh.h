#ifndef MESH_H
#define MESH_H

#include <vector>
#include <set>
#include "renderobject.h"
#include "geometry.h"


//-----------------------------------------------------------------------------
//------MESH---------------------------------------
//-----------------------------------------------------------------------------

typedef std::vector<Pnt> NormalList;
typedef std::vector<unsigned int> AdjacencyList;
typedef std::set<unsigned int> AdjacencySet;

class MeshPoint;
// ����� ������������� �����
class MeshPoint : public Pnt
{
public:
     //�������� �������������� ���� ��� ������������� � vec3
    //operator vec3() const{return position;}
    //�������� �������������� ���� ��� ������������� � Pnt
    MeshPoint(){}
    MeshPoint(Pnt p){x=p.x;y=p.y;z=p.z;}
    MeshPoint(float X, float Y, float Z){x=X;y=Y;z=Z;}
//	MeshPoint(MeshPoint& pgh)
	/*MeshPoint(class MeshPoint *pgh,const class MeshPoint &pgh1)
		
	{
		position = pgh->position;
		adjacentFaces= pgh->adjacentFaces;
		adjacentVerts= pgh->adjacentVerts;
		faceCentroidsSum= pgh->faceCentroidsSum;
	}
	
	operator Pnt() const{return Pnt(position.x,position.y,position.z);}
	MeshPoint& operator=(MeshPoint& pgh){
		Pnt A, B;
		A = B;

		position = pgh.position;
		
		return pgh; 
	}
	*/

    //Pnt position;
    AdjacencyList adjacentFaces;
    AdjacencySet adjacentVerts;
    Pnt faceCentroidsSum;
};

//������� �����
struct Vertex
{
    Vertex(){index = 0;}
    unsigned int index;
    Pnt lightingNormal;
};

//����� �����
typedef struct TFace
{
    Vertex vertices[4]; // TODO: allow non-quads
    Pnt facetNormal;
	char matid;
}TFace;

// ������������� �����
struct Mesh
{
   //���� ����
    int TIMEVALUE;
    int PROP_MOTIONBLUR;
    int PROP_CASTSHADOW;
    int PROP_RECVSHADOW;
	int	MATERIAL_REF;
    std::vector<MeshPoint> points;
    std::vector<TFace> faces; // TODO: change to a LOD-index array
    unsigned int edgeCount;
    void clear()
    {
        points.clear();
        faces.clear();
        edgeCount = 0;
    }
    bool IndexIsOk(int i)
    {
        return ( ( i > 0) && (i<(int)points.size()) );
    }

};

enum RenderMode
{
    GRAPH, //connect all points discard faces
    FACES,
    RAND_TEXTURE,
    TEXTURE_MAP
};

//����� ������ ������� ������� ��������� �������
enum CreationModes
{
    REMAKE_EVERY_FRAME,
    REMAKE_ONCE
};

//====================================================================
//              ������������� ���� (��� ������)                  
//====================================================================
class MeshObject : public RenderObject
{
    public:
        float      time; // ������� ������� �� ��������� ����� �������
        RenderMode mode;
        Mesh mesh;
        MeshObject();
        ~MeshObject();
        virtual void Render();
        void MakeBounding();
};




#endif
