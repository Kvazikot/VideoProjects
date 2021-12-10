#ifndef GLWIDGET2_H
#define GLWIDGET2_H

#include "camera.h"
#include "sky.h"
#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QGLShaderProgram>
#include <QOpenGLShaderProgram>

class GlWidget2 : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:
    explicit GlWidget2(QWidget *parent = 0);
    void setCamName(QString cname){ cam_name = cname; }
    void ReloadShaders(){ bNeedReloadShaders = true; }

    ~GlWidget2();

protected:
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);
    void reloadShaders();
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void initShaders();
    void initTextures();
    void CopyImage(QImage& img);

private:
    bool        bNeedReloadShaders;
    Sky*        sky;
    float       m_sunTheta;
    QImage      white_texture;
    QImage      cube_texture;
    QImage      frame;
    QString     cam_name;
    Camera*     cam;
    QPoint      lastPos;
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    //QOpenGLShaderProgram program0;
    GLuint texture;
    QMatrix4x4 projection;
    QMatrix4x4 m_viewportMatrix;


signals:
  void ObjectsIsLoaded();
};

#endif // MAINWIDGET_H
