#ifndef GLWidget_H
#define GLWidget_H

#include <QtOpenGL>
#include <QImage>
#include <QGLContext>
#include "camera.h"
#include "grid.h"
#include "cinput.h"
#include "cobjregistry.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QGLContext* ctx, QWidget *parent);
    void SetMinimapMode();
    void SetCamName(QString name);
    QImage   frame;
    QPixmap  pixmap;


    bool bMinimap;
protected:

    Camera* cam;
    QString cam_name;
    CObjRegistry*  MREG; // реестр 3д обьектов
    Grid    grid;
    CInput* iinput;

    bool bMakeSelect;
    QPointF lastPos;
    void CopyImage(QImage& img);
    bool isMouseInPolygon(Pnt m, Pnt p1, Pnt p2, Pnt p3, Pnt p4);
    void select();
    void display(void);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *);
    //void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);

private:
    GLuint list;

 signals:
   void ObjectsIsLoaded();
};

void GetBmpPtrs(char** top, char** pers);
void GetBmpProps(int& stribe, int& maxX, int& maxY);

#endif // GLWidget_H
