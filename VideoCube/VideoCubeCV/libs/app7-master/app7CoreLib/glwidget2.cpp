#include "../include/glwidget2.h"
#include <GL/glu.h>
#include "../include/cobjregistry.h"
#include "../include/renderobject.h"
#include "../include/print.h"
#include "../include/material.h"
#include "../include/mterrain.h"
#include "../include/camera.h"
#include "../include/timer.h"
#include "../include/grid.h"
#include "../include/shared_data.h"
#include "../include/cinput.h"
#include "../include/flags.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QMouseEvent>
#include <math.h>

static CObjRegistry& MREG = CObjRegistry::getInstance2();
static CInput* iinput = CInput::getInstance();
static Grid    grid;
static t_flags& flags = GetGFlags();
static char persp_bmp[2048*1024*4];

GlWidget2::GlWidget2(QWidget *parent) :
    QGLWidget(parent)
{
    m_sunTheta = 30;
    cam = 0;
    bNeedReloadShaders = false;
    white_texture = QImage(300,300,QImage::Format_ARGB32);
    frame.load("frame.bmp");
    white_texture.fill(Qt::white);
    setMouseTracking(true);
    setFocusPolicy( Qt::StrongFocus );
}

GlWidget2::~GlWidget2()
{
    deleteTexture(texture);
}

void GlWidget2::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    focusWidget();

    if ( event->button() == Qt::LeftButton)
    {
      iinput->click_lmb(1);
    }

    if ( event->button() == Qt::RightButton)
    {
        iinput->click_rmb(1);
    }
}

void GlWidget2::mouseReleaseEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    if ( event->button() == Qt::LeftButton)
    {
      iinput->click_lmb(0);
    }
    if ( event->button() == Qt::RightButton)
    {
      iinput->click_rmb(0);
    }

}

void GlWidget2::keyPressEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat() )
    {
        iinput->key_pressed(event->nativeVirtualKey());
        event->accept();
    }
}

void GlWidget2::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat() )
    {
        iinput->key_upressed(event->nativeVirtualKey());
        event->accept();
    }
}


void GlWidget2::mouseMoveEvent(QMouseEvent *event)
{
    iinput->mouse_move(event->x(), event->y());
    lastPos = event->pos();
}

void GlWidget2::wheelEvent(QWheelEvent *e)
{
    iinput->mouse_wheel(e->delta());
}


void GlWidget2::timerEvent(QTimerEvent *)
{ 
    updateGL();
}

void GlWidget2::initializeGL()
{
    qDebug() << "OpenGL version:" << format().majorVersion() << "." << format().minorVersion();
    prn("OpenGL version: %d.%d",format().majorVersion(), format().minorVersion());
    if( format().majorVersion() < 4 )
    {
        prn("check your graphics card or driver!");
        prn("");
        return;
    }

    initializeGLFunctions();
    //sky = new Sky(context()->contextHandle());

    qglClearColor(Qt::black);
    initShaders();
    initTextures();


    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    glEnable (GL_BLEND);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);


    //sky->initquad();

    // загрузка обьектов
    MREG.Load();
    MREG.ListObjs();
    emit ObjectsIsLoaded();

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void GlWidget2::reloadShaders()
{
    program.release();
    program.removeAllShaders();

    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vshader.glsl"))
    {
        print(program.log());
        return;
    }

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fshader.glsl"))
    {
        print(program.log());
        return;
    }

    // Compile geometry shader
    //if (!program.addShaderFromSourceFile(QGLShader::Geometry, "shaders/gshader.glsl"))
    //    ExitProcess(0);

    // Link shader pipeline
    if (!program.link())
    {
        print(program.log());
        return;
    }

    // Bind shader pipeline for use
    if (!program.bind())
    {
        print(program.log());
        return;
    }

}

void GlWidget2::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vshader.glsl"))
        //ExitProcess(0);
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fshader.glsl"))
        close();

    // Compile geometry shader
    //if (!program.addShaderFromSourceFile(QGLShader::Geometry, "shaders/gshader.glsl"))
    //    ExitProcess(0);

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();


}

void GlWidget2::initTextures()
{
    // Load cube.png image
    //load texture
    cube_texture.load("textures/cube.png");


}


void GlWidget2::resizeGL(int w, int h)
{

    if( format().majorVersion() < 4 )
        return;

    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    m_viewportMatrix.setToIdentity();
    m_viewportMatrix.setColumn( 0, QVector4D( w2, 0.0f, 0.0f, 0.0f ) );
    m_viewportMatrix.setColumn( 1, QVector4D( 0.0f, h2, 0.0f, 0.0f ) );
    m_viewportMatrix.setColumn( 2, QVector4D( 0.0f, 0.0f, 1.0f, 0.0f ) );
    m_viewportMatrix.setColumn( 3, QVector4D( w2, h2, 0.0f, 1.0f ) );

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1, zFar = 100, fov = 45;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void GlWidget2::CopyImage(QImage& img)
{
    char* pix = (char*)img.bits();
    //persp_bmp
    /*
    for(int i=0; i < (int)img.height() * img.width() * 4; i++)
    {
        persp_bmp[i] = *pix;
        pix++;
    }*/
    pix = persp_bmp;
    for(int j=0; j < (int)img.height(); j++)
    for(int i=0; i < (int)img.width(); i++)
    {
        *pix = qRed(img.pixel(i,j));
        pix++;
        *pix = 0;
        pix++;
        *pix = 0;
        pix++;
        *pix = 0;
        pix++;
    }

}



void GlWidget2::paintGL()
{

    if( format().majorVersion() < 4 )
        return;

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(bNeedReloadShaders)
    {
       reloadShaders();
       bNeedReloadShaders = false;
    }

    // Calculate model view transformation
    QMatrix4x4 matrix;
    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();

    cam = (Camera*)MREG.FindObj(cam_name.toStdString());
    if(cam!=0)
    {
        projection.setToIdentity();
        matrix.lookAt(QVector3D(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z),
                      QVector3D(cam->lookAt.x, cam->lookAt.y, cam->lookAt.z)
                      ,QVector3D(0,1,0));

        if(cam->view==1) //top
        {
            projection.ortho(-cam->ei.coord.y*cam->aspect/2,cam->ei.coord.y*cam->aspect/2,-cam->ei.coord.y/2,cam->ei.coord.y/2,cam->zNear,cam->zFar);
            GLint    viewport[4];
            GLdouble projectionA[16];
            GLfloat projectionF[16];
            GLdouble modelviewA[16];
            GLfloat modelviewF[16];
            projection.transposed().copyDataTo(projectionF);
            matrix.transposed().copyDataTo(modelviewF);
            for(int i=0; i<16; i++)
            {
                projectionA[i] = projectionF[i];
                modelviewA[i] = modelviewF[i];
            }
            viewport[0]=0; viewport[1]=0; viewport[2]=width(); viewport[3]=height();
            gluUnProject(iinput->mouse_x, height() - iinput->mouse_y, 0, modelviewA, projectionA, viewport, &iinput->vx,&iinput->vy,&iinput->vz);

        }
        else
             projection.perspective(cam->fovy, cam->aspect, cam->zNear, cam->zFar);
    }
    const float degToRad = float( M_PI / 180.0 );
    int sun_dist = 1000;
    QVector4D worldLightPosition( sun_dist*sinf( m_sunTheta * degToRad ), sun_dist*cosf( m_sunTheta * degToRad ), 0.0f, 0.0f );
    //отрисовка рейтрейсингом небо
    //if(cam!=0)
    //  sky->Render(matrix, projection, worldLightPosition, cam->aspect, QVector3D(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z));



    //viewport matrix
    program.bind();
    program.setUniformValue("viewportMatrix", m_viewportMatrix);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
    program.setUniformValue("model_matrix", model_matrix);

    // Set the lighting parameters
    //QMatrix4x4 worldToEyeNormal( normalMatrix );
    //QVector4D lightDirection = worldToEyeNormal * worldLightDirection;    
    program.setUniformValue( "light.position", worldLightPosition );
    program.setUniformValue( "light.intensity", QVector3D( 1.0f, 1.0f, 1.0f ) );
    if(cam!=0)
      program.setUniformValue( "eyePosWorld", QVector3D(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z) );


    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    bindTexture(white_texture);

    MREG.UpdateNew();

    //цикл отрисовки
    vector<Object*>::iterator obj;
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {
        if((*obj)->isCanRender)
        {
            if((*obj)->classname == "MTerrain")
            //if(0)
            {
                MTerrain* mterrain = (MTerrain*)*obj;
                glEnable(GL_TEXTURE_2D);
                glActiveTexture(GL_TEXTURE0);
                bindTexture(mterrain->bump_texture);
                //bindTexture(cube_texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

                mterrain->Render(program);

            }
            else
            {
                        RenderObject* ro =  dynamic_cast<RenderObject*>(*obj);
                        ro->Render(program);
            }
        }

    }

    CObjRegistry& MREG2 = CObjRegistry::getLayerInstance();
    MREG2.UpdateNew();
    for(obj = MREG2.allObj.begin(); obj != MREG2.allObj.end(); obj++ )
    {
        RenderObject* ro =  dynamic_cast<RenderObject*>(*obj);
        ro->Render(program);
    }

    if(flags.bShowGrid)
    {
        grid.Render(program);
        program.setUniformValue("RenderMode", 0 );
        glEnable(GL_TEXTURE_2D);
        bindTexture(white_texture);
        renderText(20,20,"NEW RENDER");
    }


    frame = grabFrameBuffer();
    //CopyImage(frame);



    //QString cam_pos_str;
    //cam_pos_str.sprintf("cam(%f,%f,%f)", cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z);
    //renderText(-0.5, -0.4, -0.5, cam_pos_str);

}
