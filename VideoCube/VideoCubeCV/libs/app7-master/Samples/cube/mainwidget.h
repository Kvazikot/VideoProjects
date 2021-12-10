#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"
#include "camera.h"
#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QGLShaderProgram>
#include <QOpenGLShaderProgram>

class GeometryEngine;

class MainWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initTextures();

private:
    float       m_sunTheta;
    Camera*     cam;
    QPoint      lastPos;
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    GLuint texture;
    QMatrix4x4 projection;
    QMatrix4x4 m_viewportMatrix;
    QVector2D mousePressPosition;
};

#endif // MAINWIDGET_H
