#include "mainwidget.h"
#include "cobjregistry.h"
#include "renderobject.h"
#include "camera.h"
#include "timer.h"
#include "grid.h"
#include "cinput.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QMouseEvent>
#include <math.h>

static CObjRegistry& MREG = CObjRegistry::getInstance2();
static CInput* iinput = CInput::getInstance();
static Grid    grid;

MainWidget::MainWidget(QWidget *parent) :
    QGLWidget(parent)
{

    cam = new Camera();
    cam->SetName("cam01");
    cam->isCanRender = false;
    cam->isReceiveInput = true;
    cam->Declare();
    cam->speed = 100;
    cam->zFar = 3000;
    cam->isNotSaving = true;
    //cam->isThirdPerson = true;
    cam->orient.B.x = (float)M_PI;
    m_sunTheta = 30;
    MREG.AddObj(cam);
}

MainWidget::~MainWidget()
{
    deleteTexture(texture);
}

void MainWidget::mousePressEvent(QMouseEvent *event)
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

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
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

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    iinput->key_pressed(event->nativeVirtualKey());
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    iinput->key_upressed(event->nativeVirtualKey());
}


void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    iinput->mouse_move(event->x(), event->y());
    lastPos = event->pos();
}

void MainWidget::wheelEvent(QWheelEvent *e)
{
    iinput->mouse_wheel(e->delta());
}


void MainWidget::timerEvent(QTimerEvent *)
{
    // ƒелаем цикл симул€ции
    Timer::gtimer.msec+=Timer::gtimer._timer.elapsed();
    Timer::gtimer.fsec = Timer::gtimer.msec / 1000.;
    CObjRegistry* MREG = CObjRegistry::getInstance();
    vector<Object*>::iterator obj;
    for(obj = MREG->allObj.begin(); obj != MREG->allObj.end(); obj++ )
    {
            (*obj)->Slot1();
    }
    Timer::gtimer._timer.start();
    updateGL();

}

void MainWidget::initializeGL()
{
    initializeGLFunctions();
    qglClearColor(Qt::black);
    initShaders();
    initTextures();

    qDebug() << "OpenGL version:" << format().majorVersion() << "." << format().minorVersion();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}


void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vshader.glsl"))
        ExitProcess(0);

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fshader.glsl"))
        ExitProcess(0);

    // Compile geometry shader
    //if (!program.addShaderFromSourceFile(QGLShader::Geometry, "shaders/gshader.glsl"))
    //    ExitProcess(0);


    // Link shader pipeline
    if (!program.link())
        ExitProcess(0);

    // Bind shader pipeline for use
    if (!program.bind())
        ExitProcess(0);
}

void MainWidget::initTextures()
{
    // Load cube.png image
    glEnable(GL_TEXTURE_2D);
    texture = bindTexture(QImage(":/cube.png"));

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void MainWidget::resizeGL(int w, int h)
{
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
    const qreal zNear = cam->zNear, zFar = cam->zFar, fov = cam->fovy;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;

    //matrix.translate(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z);
    //matrix.rotate(rotation);
    matrix.lookAt(QVector3D(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z),
                  QVector3D(cam->lookAt.x, cam->lookAt.y, cam->lookAt.z)
                  ,QVector3D(0,1,0));

    //viewport matrix
   // program.setUniformValue("viewportMatrix", m_viewportMatrix);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Set the lighting parameters
    const float degToRad = float( M_PI / 180.0 );
    QVector4D worldLightDirection( sinf( m_sunTheta * degToRad ), cosf( m_sunTheta * degToRad ), 0.0f, 0.0f );
    //QMatrix4x4 worldToEyeNormal( normalMatrix );
    //QVector4D lightDirection = worldToEyeNormal * worldLightDirection;
    program.setUniformValue( "light.position", worldLightDirection );
    program.setUniformValue( "light.intensity", QVector3D( 1.0f, 1.0f, 1.0f ) );

    // Set the material properties
    program.setUniformValue( "material.Ka", QVector3D( 0.1f, 0.1f, 0.1f ) );
    program.setUniformValue( "material.Kd", QVector3D( 1.0f, 1.0f, 1.0f ) );
    program.setUniformValue( "material.Ks", QVector3D( 0.3f, 0.3f, 0.3f ) );
    program.setUniformValue( "material.shininess", 50.0f );

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);


    //цикл отрисовки
    vector<Object*>::iterator obj;
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {
        if((*obj)->isCanRender)
        {
                    RenderObject* ro =  (RenderObject*)*obj;
                    ro->Render(program);
        }

    }
    grid.Render(program);

    //QString cam_pos_str;
    //cam_pos_str.sprintf("cam(%f,%f,%f)", cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z);
    //renderText(-0.5, -0.4, -0.5, cam_pos_str);

}
