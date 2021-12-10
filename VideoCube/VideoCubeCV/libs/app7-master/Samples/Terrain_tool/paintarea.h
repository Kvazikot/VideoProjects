#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QColor>
#include <QImage>
#include <QPainterPath>
#include <QWidget>
#include <vector>


struct PointRec
{
    double tsec;
    QPoint pos;
};

class PaintArea : public QWidget
{
    Q_OBJECT

public:
    PaintArea(QWidget *parent = 0);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setImage(const QImage &image);
    void insertShape(const QPainterPath &path);
    void setBrushColor(const QColor &color);
    void setBrushWidth(int width);
    //void setBrush(BrushInterface *brushInterface, const QString &brush);
    void processPoints();

    QImage image() const { return theImage; }
    QColor brushColor() const { return color; }
    int brushWidth() const { return thickness; }
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent* event);

private:
    void setupPainter(QPainter &painter);

    QImage theImage;
    QColor color;
    int thickness;
    long gtimer;
    std::vector<PointRec> points;

    QString brush;
    QPoint lastPos;

    QPainterPath pendingPath;
};

#endif
