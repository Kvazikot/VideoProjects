#ifndef GEOMETRYOBJECT_H
#define GEOMETRYOBJECT_H

#include <QtOpenGL>
#include <QFile>
#include "renderobject.h"
#include "vertexdata.h"


class GeometryObject : protected QGLFunctions, public RenderObject
{
public:
    GeometryObject();
    virtual ~GeometryObject();
    void init();
    void update();
    void Render(QOpenGLShaderProgram& program);
    void SetNumVert(int n_vert);
    void SetNumFaces(int n_faces);
    int  GetNumVert(){ return vertices.size(); }
    int  GetNumFaces(){ return indices.size()/3; }

    bool bUpdateMesh;
    std::vector<GLushort> indices;
    std::vector<VertexData> vertices;

private:
    GLuint vboIds[2];
};

#endif // GEMOBJECT_H
