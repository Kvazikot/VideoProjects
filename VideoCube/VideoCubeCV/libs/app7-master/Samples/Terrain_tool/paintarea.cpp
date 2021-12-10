#include "paintarea.h"
#include <QPainter>
#include <QMouseEvent>
#include <queue>
#include "cobjregistry.h"
#include "mterrain.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    theImage(500, 500, QImage::Format_ARGB32),
    color(Qt::blue),
    thickness(3),
    lastPos(-1, -1)
{
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_NoBackground);
    gtimer=0;
    startTimer(10);
    theImage.fill(qRgb(0, 0, 0));
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage image;
    if (!image.load(fileName))
        return false;

    setImage(image);
    return true;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    return theImage.save(fileName, fileFormat);
}

void PaintArea::setImage(const QImage &image)
{
    theImage = image.convertToFormat(QImage::Format_ARGB32);
    update();
    updateGeometry();
}

void PaintArea::insertShape(const QPainterPath &path)
{
    pendingPath = path;
#ifndef QT_NO_CURSOR
    setCursor(Qt::CrossCursor);
#endif
}

void PaintArea::setBrushColor(const QColor &color)
{
    this->color = color;
}

void PaintArea::setBrushWidth(int width)
{
    thickness = width;
}


QSize PaintArea::sizeHint() const
{
    return theImage.size();
}

void PaintArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), theImage);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPainter painter(&theImage);
        setupPainter(painter);
        points.clear();
    }

    lastPos = event->pos();


}

void PaintArea::timerEvent(QTimerEvent* event)
{
    event = event;
    gtimer+=10;
}


void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1, -1)) {
        //if (brushInterface) {
       {
            PointRec pr;
            pr.tsec = gtimer;
            pr.pos = event->pos();
            points.push_back(pr);
            //return;


            QPainter painter(&theImage);
            setupPainter(painter);
            QPoint oldPos = lastPos;
            QPoint newPos = event->pos();
            painter.save();

            int rad = painter.pen().width() / 2;
            QRect boundingRect = QRect(oldPos, newPos).normalized()
                                                      .adjusted(-rad, -rad, +rad, +rad);
            int thickness = painter.pen().width();

            QColor color = QColor::fromRgbF(0, 1, 0, 1);

            QBrush brush(Qt::SolidPattern);
            brush.setColor(color);
            QPainterPath path;
            path.addEllipse(QRect(newPos.x() - (thickness / 2), newPos.y() - (thickness / 2),
                                  thickness, thickness));
            painter.fillPath(path,brush   );

            painter.restore();

            update(boundingRect);
        }

        lastPos = event->pos();
    }
}


void PaintArea::processPoints()
{
    QPainter painter(&theImage);
    setupPainter(painter);
    painter.setBrush(Qt::SolidPattern);

    //points
    for(int i=1; i < (int)points.size(); i++)
    {
        QPoint newPos = points[i].pos;
        QPainterPath path;
        path.addEllipse(QRect(newPos.x() - (thickness / 2), newPos.y() - (thickness / 2),
                              thickness, thickness));
        painter.fillPath(path,Qt::SolidPattern );
    }
    update();

    QImage I(theImage), J(theImage);
    unsigned int Nq[8];
    QPoint Nqp[8];
    std::queue<QPoint> fifo;
    {
        for (int y = 1; y < I.height() - 1; ++y) {
        for (int x = 1; x < I.width() - 1; ++x) {
            unsigned int  p = I.pixel(x, y);
            if( qGreen(p)!=0 )
                I.setPixel(x,y,(unsigned int)1);
            else
                I.setPixel(x,y,0);
        }
        }

        for (int y = 1; y < I.height() - 1; ++y) {
        for (int x = 1; x < I.width() - 1; ++x) {
                unsigned int  p = I.pixel(x, y);
                Nq[0] = I.pixel(x-1, y - 1);
                Nq[1] = I.pixel(x, y - 1);
                Nq[2] = I.pixel(x + 1, y - 1);
                Nq[3] = I.pixel(x-1, y);
                Nq[4] = I.pixel(x+1, y);
                Nq[5] = I.pixel(x-1, y+1);
                Nq[6] = I.pixel(x, y+1);
                Nq[7] = I.pixel(x+1, y+1);
                if( p == 1  )
                {
                  bool add = false;
                  for(int i=0; i < 8; i++)
                      if( Nq[i] == 0 )
                          add = true;
                  if( add )
                  {
                      fifo.push(QPoint(x,y));
                      I.setPixel(x,y,2);
                  }

                }

            }
        }


        while( fifo.size()!=0 )
        {
            QPoint p = fifo.front();
            fifo.pop();
            Nq[0] = I.pixel(p.x()-1, p.y()-1);
            Nq[1] = I.pixel(p.x(), p.y() - 1);
            Nq[2] = I.pixel(p.x() + 1, p.y() - 1);
            Nq[3] = I.pixel(p.x()-1, p.y());
            Nq[4] = I.pixel(p.x()+1, p.y());
            Nq[5] = I.pixel(p.x()-1, p.y()+1);
            Nq[6] = I.pixel(p.x(), p.y()+1);
            Nq[7] = I.pixel(p.x()+1, p.y()+1);
            Nqp[0] = QPoint(p.x()-1, p.y()-1);
            Nqp[1] = QPoint(p.x(), p.y()-1);
            Nqp[2] = QPoint(p.x() + 1, p.y() - 1);
            Nqp[3] = QPoint(p.x()-1, p.y());
            Nqp[4] = QPoint(p.x()+1, p.y());
            Nqp[5] = QPoint(p.x()-1, p.y()+1);
            Nqp[6] = QPoint(p.x(), p.y()+1);
            Nqp[7] = QPoint(p.x()+1, p.y()+1);
            for(int i=0; i<8; i++)
            {
                 if( Nq[i] == 1 )
                 {
                     unsigned int  v = I.pixel(p.x(), p.y());
                     I.setPixel(Nqp[i].x(),Nqp[i].y(), v+1);
                     fifo.push(Nqp[i]);
                 }
            }

        }


        //I.save("I.bmp");
    }

    MTerrain* mterr = (MTerrain*)MREG.FindObj("defaultTerrain");
    if( mterr != 0 )
        mterr->SetBumpImage(I);


//    for (int y = 1; y < theImage.height() - 1; ++y) {
//    for (int x = 1; x < theImage.width() - 1; ++x) {
//        int alfa = I.pixel(x,y);
//        QRgb p = theImage.pixel(x, y);
//        theImage.setPixel(x, y, alfa*10);

//     }
//    }

    //theImage.fill(qRgb(0, 0, 0));
    //theImage = QImage(I);
    //painter.drawImage(QPoint(0, 0), I);
   // update();

}


void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && lastPos != QPoint(-1, -1)) {
//        if (brushInterface) {
    {
            QPainter painter(&theImage);
            setupPainter(painter);
            QRect rect = QRect(0, 0, 0, 0);
            update(rect);
            processPoints();
        }

        lastPos = QPoint(-1, -1);
    }
}

void PaintArea::setupPainter(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap,
                   Qt::RoundJoin));
}
