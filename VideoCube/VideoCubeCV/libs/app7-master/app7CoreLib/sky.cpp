#include <QFile>
#define _USE_MATH_DEFINES
#include <math.h>
#include "sky.h"


static inline float frand()
{
    return rand() / (float) RAND_MAX;
}


GLuint Sky::createNoiseTexture4f3D(int w, int h, int d, GLint internalFormat, bool mipmap)
{
    srand(0);
    float *data = new float [w*h*d*4];
    float *ptr = data;
    for(int z=0; z<d; z++) {
        for(int y=0; y<h; y++) {
            for(int x=0; x<w; x++) {
              *ptr++ = frand()*2.0f-1.0f;
              *ptr++ = frand()*2.0f-1.0f;
              *ptr++ = frand()*2.0f-1.0f;
              *ptr++ = frand()*2.0f-1.0f;
                //qDebug() << frand();
            }
        }
    }

    GLuint tex;
    m_funcs->glActiveTexture( GL_TEXTURE0 );
     m_funcs->glGenTextures(1, &tex);
     m_funcs->glBindTexture(GL_TEXTURE_3D, tex);

    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     m_funcs->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
     m_funcs->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     m_funcs->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     m_funcs->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     m_funcs->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

     m_funcs->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     m_funcs->glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, w, h, d, 0, GL_RGBA, GL_FLOAT, data);
    if (mipmap) {
        m_funcs->glGenerateMipmap(GL_TEXTURE_3D);
    }

    delete [] data;
    return tex;
}

QString createStringFromAsset(QString fn)
{
    QFile f(fn);

    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "cannot open file !" + fn;
        return "";
    }
    QTextStream ts(&f);
    ts.setCodec("UTF-8");
    return ts.readAll();
}

Sky::Sky(QOpenGLContext* context)
{
    sun_size = 10;
    skyHeight = 100;
    skyTop = 10;
    cloudDensity = 2;




    m_funcs = context->versionFunctions<QOpenGLFunctions_4_0_Core>();
    if ( !m_funcs )
    {
        qDebug("Requires OpenGL >= 4.0");
        return;
    }
    m_funcs->initializeOpenGLFunctions();

    int noiseSize3D = 64;
    mRandTex3D = createNoiseTexture4f3D(noiseSize3D, noiseSize3D, noiseSize3D, GL_RGBA16F, false);




      QString uniformsHeader = createStringFromAsset("shaders/uniforms.h");
      QString noiseHeader = createStringFromAsset("shaders/noise.glsl");
      QString noise3DHeader = createStringFromAsset("shaders/noise3D.glsl");
      QString currentHeader = uniformsHeader + "\n" + noise3DHeader;
      QString skyHeaderFS = createStringFromAsset("shaders/sky_fs.glsl");
      QString skyHeaderVS = createStringFromAsset("shaders/sky_vs.glsl");
      QString sky_fs = currentHeader + "\n" + skyHeaderFS;
      QString sky_vs = uniformsHeader + "\n" + skyHeaderVS;


      // Create a shader program
      if ( !sky_prog.addShaderFromSourceCode( QOpenGLShader::Vertex, sky_vs.toStdString().c_str() ) )
          qCritical() << QObject::tr( "Could not compile vertex shader. Log:" ) << sky_prog.log();

      //":/shaders/sky_fs.glsl"
      if ( !sky_prog.addShaderFromSourceCode(QOpenGLShader::Fragment, sky_fs.toStdString().c_str() ) )
          qCritical() << QObject::tr( "Could not compile fragment shader. Log:" ) << sky_prog.log();

      if ( !sky_prog.link() )
          qCritical() << QObject::tr( "Could not link shader program. Log:" ) << sky_prog.log();

      if ( !sky_prog.create() )
          qCritical() << QObject::tr( "Could not create shader program. Log:" ) << sky_prog.log();





}

void Sky::initquad()
{
    GLfloat v[] = {
           -1.0f, -1.0f, 0.99999f,
            1.0f, -1.0f, 0.99999f,
           -1.0f,  1.0f, 0.99999f,
            1.0f,  1.0f, 0.99999f
       };
    // Generate 2 VBOs
    m_funcs->glGenBuffers(2, vboIds);
    // Transfer vertex data to VBO 0
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    m_funcs->glBufferData(GL_ARRAY_BUFFER,  12 * sizeof( float ), v, GL_STATIC_DRAW );
    //glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexData), &vertices[0]);
}

void Sky::Render(QMatrix4x4 view_matrix, QMatrix4x4 proj_matrix, QVector4D lightWorld, float aspect, QVector3D eyePos)
{
    //return;
    //glDis(GL_DEPTH_TEST);

    m_funcs->glEnable(GL_TEXTURE_3D);
    m_funcs->glActiveTexture(GL_TEXTURE0);
    m_funcs->glBindTexture(GL_TEXTURE_3D, mRandTex3D);

    sky_prog.bind();
    //shader->setUniformValue( "line.width", 1.0f );
    //shader->setUniformValue( "line.color", QVector4D( 0.0f, 1.0f, 0.0f, 1.0f ) );
    QMatrix4x4 m;
    //m.rotate(90,0,1,0);


    QMatrix4x4 viewMatrixInv = view_matrix* m;
    viewMatrixInv = viewMatrixInv.inverted();
    //qDebug() << viewMatrixInv;
    sky_prog.setUniformValue( "InvView", viewMatrixInv );
    QMatrix4x4 projectionMatrix;
    //nv::perspective( projectionMatrix, , (float)m_width/(float)m_height, 0.01f, 100.0f);
    projectionMatrix.perspective(25.0f * 2.0f*M_PI / 360.0f, aspect, 0.01f, 100.0f);

    QMatrix4x4 projInv = proj_matrix.inverted();
    sky_prog.setUniformValue( "InvProjection", projInv );
    sky_prog.setUniformValue("invNoise3DSize", 1.0f / 64.f);

    sky_prog.setUniformValue("eyePosWorld", QVector4D(eyePos.x(), eyePos.y(), eyePos.z(), 1.0f));
    sky_prog.setUniformValue("translate",QVector2D(0,0));
    sky_prog.setUniformValue("randTex3D",(int)0);
    sky_prog.setUniformValue("time",m_time2);
    sky_prog.setUniformValue("m_metersToUnits",m_metersToUnits);
    //QVector3D l(10.0f, 10.0f, -0.25f);// =  worldLightDirection.toVector3D();
    //QVector3D l((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, -(float)rand()/RAND_MAX);
    QVector3D l(lightWorld.x(),lightWorld.y(),lightWorld.z());
    l.normalize();
    sky_prog.setUniformValue("lightDirWorld",l);
    sky_prog.setUniformValue("sun_size", sun_size );
    sky_prog.setUniformValue("skyHeight", skyHeight );
    sky_prog.setUniformValue("skyTop", skyTop );
    sky_prog.setUniformValue("cloudDensity", cloudDensity );


    QVector3D lightDir = QVector3D( view_matrix *QVector4D(l, 0.0));   // transform to eye space
    sky_prog.setUniformValue("lightDir",lightDir);

    m_time2+=0.1;
    if( m_time2 > 50 )
        m_time2 = 0;


    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    sky_prog.enableAttributeArray( "vertex" );
    sky_prog.setAttributeBuffer( "vertex", GL_FLOAT, 0, 3 );
    m_funcs->glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    sky_prog.release();



}
