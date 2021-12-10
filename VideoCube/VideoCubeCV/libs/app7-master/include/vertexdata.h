#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <QVector3D>
#include <QVector2D>

//#pragma pack(push, 1)
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};
//#pragma pack(pop)

#endif 
