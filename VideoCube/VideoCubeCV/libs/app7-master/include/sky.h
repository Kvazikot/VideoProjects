#ifndef SKY_H
#define SKY_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_0_Core>
#include "renderobject.h"

class Sky : public RenderObject
{
public:
    Sky(QOpenGLContext* context);
    void Render(QMatrix4x4 view_matrix, QMatrix4x4 proj_matrix, QVector4D lightWorld, float aspect, QVector3D eyePos);
    GLuint createNoiseTexture4f3D(int w, int h, int d, GLint internalFormat, bool mipmap);
    void initquad();

    //Sky params
    float sun_size;
    float skyHeight;
    float skyTop;
    float cloudDensity;

    QOpenGLShaderProgram sky_prog;

protected:
   QOpenGLFunctions_4_0_Core* m_funcs;
    // The sky "object"
   QOpenGLVertexArrayObject m_vao2;
   QOpenGLBuffer m_quadBuffer;
   GLuint mRandTex3D;
   float  m_time2;
   float  m_metersToUnits;
   GLuint vboIds[2];

};

#endif // SKY_H
