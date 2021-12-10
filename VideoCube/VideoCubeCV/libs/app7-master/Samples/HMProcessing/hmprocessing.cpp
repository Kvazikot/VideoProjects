#include <QLabel>
#include <QApplication>
#include <QImage>
#include <queue>
#include "hmprocessing.h"
#include "ploter.h"


HMProcessing::HMProcessing()
{
    maxD = 0;
}

QRgb color_pix(unsigned int p, int maxD);
QRgb color_mix(QRgb c1, QRgb c2, float t);


void HMProcessing::process(QString fn, int porog)
{
    QImage theImage, outImage;

    theImage.load(fn);
    theImage.convertToFormat(QImage::Format_RGB32);

    processHM(theImage, porog);
    outImage.save("distance.png");
}

#define qNoAlpha(p) qRgba(qRed(p),qGreen(p),qBlue(p),0)

void HMProcessing::processHM(QImage& I, int porog)
{
    unsigned int Nq[8];
    QPoint Nqp[8];
    std::queue<QPoint> fifo;
    {

        for (int y = 1; y < I.height() - 1; ++y) {
        for (int x = 1; x < I.width() - 1; ++x) {
            unsigned int  p = qNoAlpha(I.pixel(x, y));
            if( qBlue(p) > porog )
                I.setPixel(x,y,qRgba(0,0,0,0));
            else
                I.setPixel(x,y,qRgba(0,0,1,0));
                p = I.pixel(x, y);
        }
        }

       // I.save("after_porog.png");
       // return;

        for (int y = 1; y < I.height() - 1; ++y) {
        for (int x = 1; x < I.width() - 1; ++x) {
                unsigned int  p = qNoAlpha(I.pixel(x, y));
                Nq[0] = qNoAlpha(I.pixel(x-1, y - 1));
                Nq[1] = qNoAlpha(I.pixel(x, y - 1));
                Nq[2] = qNoAlpha(I.pixel(x + 1, y - 1));
                Nq[3] = qNoAlpha(I.pixel(x-1, y));
                Nq[4] = qNoAlpha(I.pixel(x+1, y));
                Nq[5] = qNoAlpha(I.pixel(x-1, y+1));
                Nq[6] = qNoAlpha(I.pixel(x, y+1));
                Nq[7] = qNoAlpha(I.pixel(x+1, y+1));
                if( p == 0  )
                {
                  bool add = false;
                  for(int i=0; i < 8; i++)
                      if( Nq[i] == 1 )
                          add = true;
                  if( add )
                  {
                      fifo.push(QPoint(x,y));
                      I.setPixel(x,y,2);
                  }

                }

            }
        }
        //I.save("after_boarders.png");
        //return;

        while( fifo.size()!=0 )
        {
            QPoint p = fifo.front();
            fifo.pop();
            Nq[0] = qNoAlpha(I.pixel(p.x()-1, p.y()-1));
            Nq[1] = qNoAlpha(I.pixel(p.x(), p.y() - 1));
            Nq[2] = qNoAlpha(I.pixel(p.x() + 1, p.y() - 1));
            Nq[3] = qNoAlpha(I.pixel(p.x()-1, p.y()));
            Nq[4] = qNoAlpha(I.pixel(p.x()+1, p.y()));
            Nq[5] = qNoAlpha(I.pixel(p.x()-1, p.y()+1));
            Nq[6] = qNoAlpha(I.pixel(p.x(), p.y()+1));
            Nq[7] = qNoAlpha(I.pixel(p.x()+1, p.y()+1));
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
                     unsigned int  v = qNoAlpha(I.pixel(p.x(), p.y()));
                     I.setPixel(Nqp[i].x(),Nqp[i].y(), v+1);
                     maxD = qMax(v+1, maxD);
                     //qDebug("%d",v+1);
                     fifo.push(Nqp[i]);
                 }
            }

        }

    }

  /*
   Ploter* ploter = Ploter::getInstance();
   unsigned int hist[5000];
   unsigned int maxD = 0;
   unsigned int meanD = 0;
   for(int i=0; i < 5000; i++) hist[i] = 0;
   for (int y = 1; y < I.height() - 1; ++y) {
   for (int x = 1; x < I.width() - 1; ++x) {
       unsigned int  p = I.pixel(x, y);
       maxD = qMax(maxD,p);
       meanD+=p;
       if(p < 5000)
         hist[p]++;
   }
   }
   qDebug("maxD=%d meanD=%d",maxD, meanD/(I.height()*I.width()));


   *  for (int y = 1; y < I.height() - 1; ++y) {
   for (int x = 1; x < I.width() - 1; ++x) {
     unsigned int  p = I.pixel(x, y);
     if( p < 255)
       I.setPixel(x,y,qRgb(p,0,0));
     else
       I.setPixel(x,y,qRgb(255,0,0));
   }
   }


   for(int i=0; i < 5000; i++)
       if(hist[i]>0 && i>0)
          ploter->setXY(0, i, hist[i]);

   //qDebug()''


     //I.save("distance.png");
     //I.load("distance.png");

     for (int y = 1; y < I.height() - 1; ++y) {
     for (int x = 1; x < I.width() - 1; ++x) {
       unsigned int  p = I.pixel(x, y);
       I.setPixel(x,y,color_pix(p, maxD));
     }
     }
     QLabel* l = new QLabel();
     l->setWindowTitle("distancemap");
     l->resize(640,480);
     l->setPixmap(QPixmap::fromImage(I));
     l->setScaledContents(true);
     l->show();

     QLabel* l2 = new QLabel();
     l2->setWindowTitle("heightmap");
     l2->resize(640,480);
     l2->setPixmap(QPixmap::fromImage(theImage));
     l2->setScaledContents(true);
     l2->show();
     //I.save("distance_viewed.png");
     //theImage.save("heightmap.png");

*/
}


QRgb color_pix(unsigned int p, int maxD)
{
    int r,g,b;
    double k = (double)p/(maxD);

    QRgb rgb = color_mix(qRgb(0,51,0),qRgb(250,51,0), k );
    r = qRed(rgb);
    b = qBlue(rgb);
    g = qGreen(rgb);
    if(p==0)
      return qRgb(255,0,0);


    //qDebug("r=%d g=%d b=%d",r,g,b);
    return qRgb(r,g,b);
}


QRgb color_mix(QRgb c1, QRgb c2, float t)
{
    QRgb res;
    int r = qRed(c1) + ((float)(qRed(c1) - qRed(c2))) * t;
    int b = qBlue(c1) + ((float)(qBlue(c1) - qBlue(c2))) * t;
    int g = qGreen(c1) + ((float)(qGreen(c1) - qGreen(c2))) * t;
    //qDebug("r=%d g=%d b=%d",r,g,b);
    return qRgb(r,g,b);
}

void HMProcessing::amplify(QImage& I)
{
    for (int y = 1; y < I.height() - 1; ++y) {
    for (int x = 1; x < I.width() - 1; ++x) {
        unsigned int  p = qNoAlpha(I.pixel(x, y));
        I.setPixel(x,y,color_pix(p, maxD));

    }
    }
}

int main_hm(int argc, char** argv)
{
    //QApplication app(argc,argv);
    //Ploter* ploter = new Ploter(0);
    //ploter->plot->show();
    //process("height_map.png", 100);
    //color_pix(140,829);
    //QRgb rgb = color_mix(qRgb(178,102,255),qRgb(255,51,255), 0.6 );
    HMProcessing h;
    QImage I;
    I.load("maze_rect.png");
    I = I.convertToFormat(QImage::Format_RGB32);
    h.processHM(I, 100);
    I.save("distance.png");

    //return app.exec();
    return 0;
}
