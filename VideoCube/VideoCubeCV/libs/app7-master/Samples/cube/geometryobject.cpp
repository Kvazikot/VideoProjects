#include <QDebug>
#include "geometryobject.h"

GeometryObject::GeometryObject()
{
}

GeometryObject::~GeometryObject()
{
    glDeleteBuffers(2, vboIds);
}

void GeometryObject::SetNumVert(int n_vert)
{
    vertices.resize(n_vert);
}

void GeometryObject::SetNumFaces(int n_faces)
{
    indices.resize( n_faces*3 );
}

void GeometryObject::init()
{
    initializeGLFunctions();

//    qDebug() << "vertices>------------";
//    for(int i=0; i < vertices.size(); i++)
//    {
//        qDebug() << vertices[i].position;
//        qDebug() << vertices[i].texCoord;
//    }

//    qDebug() << "indices>------------";
//    for(int i=0; i < indices.size(); i++)
//    {
//        qDebug() << indices[i];
//    }

    // Generate 2 VBOs
    glGenBuffers(2, vboIds);

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_DYNAMIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexData), &vertices[0]);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_DYNAMIC_DRAW );
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLushort), &indices[0]);

}

void GeometryObject::update()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexData), &vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLushort), &indices[0]);
}

void GeometryObject::Render(QOpenGLShaderProgram& program)
{
    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program.attributeLocation("a_texcoord");
    program.enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    //Offset to normal
    offset += sizeof(QVector2D);
    int normalLocation = program.attributeLocation("a_normal");
    program.enableAttributeArray(normalLocation);
    glVertexAttribPointer(normalLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);


    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
    //glDrawElements(GL_LINES, GetNumFaces(), GL_UNSIGNED_SHORT, 0);
}
