#ifndef WEAPON_H
#define WEAPON_H

#include "weapon.h"
#include "gemobject.h"
#include "vector3d.h"
#include "vertexdata.h"

struct Ray
{
    CVector3 pos;
    CVector3 p0;
    CVector3 dir;
    double   t;
    double   life;
    Ray()
    {
        t=0; life=-1;
    }
};

class Weapon : protected QGLFunctions, public RenderObject
{
public:
    GemObject* gun;
    double     t,speed;
    int        ray_texture_id;
    Ray        rays[11];
    int        Nrays;
    static const int  MAX_RAYS=10;

    std::vector<GLushort> indices;
    std::vector<VertexData> vertices;
    GLuint vboIds[2];

    enum {RAILGUN, SHOTGUN, KOLT } type;
    Weapon();
    void Init();
    void update();
    void Shot(CVector3 p, CVector3 dir);
    void HitObjects(CVector3 p, CVector3 dir);
    void Render(QOpenGLShaderProgram& program);
    void Slot1();
};

#endif // WEAPON_H
