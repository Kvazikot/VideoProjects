#include "imageview.h"
#include <QPainter>
#include <QDebug>
#include "../include/geometry.h"
//#include "../include/cobjregistry.h"
//#include "../include/car_autopilot.h"

//static CObjRegistry& MREG = CObjRegistry::getInstance2();

float GetScansFromDM(Pnt p, QImage& distance_map);

ImageView::ImageView(QWidget *parent) : QLabel(parent)
{
    maze.load("D:\\src\\app7-mingw\\app7\\Data\\maze.png");
    maze_runner = new MazeRunner();
    startTimer(100);
}

void ImageView::timerEvent(QTimerEvent* )
{
    //newP.x = p.x + 2 * cos(alfa * GR);
    //newP.y = p.y + 2 * sin(alfa * GR);

}



float rasst(Pnt p1, Pnt p2)
{
     float _t1 = (float)sqrt( (double) ( sq(p1.x - p2.x)  +  sq(p1.y - p2.y )))  ;
     return _t1;
}

MazeRunner::MazeRunner()
{

}


std::set<float>  MazeRunner::GetScansFromDM(Pnt p, QImage& maze, QImage& out_maze)
{
    //Pnt p(2,46,0);
    const float DELTA_HIT = 6;
    const float MIN_RASST = 30;
    const float BOX_SIZE = 10;
    const float MAX_RAY_LENGTH = 50;
    std::vector<std::vector<Pnt>> hits_all;
    std::vector<std::vector<PointPair>> lines_all;


    hits.clear();
    lines.clear();
    out_angles.clear();

    QPainter painter(&out_maze);

    vector<Pnt> scan_pts;

    // build a box of points for the scans
    for(int j=-BOX_SIZE/2; j < BOX_SIZE/2; j+=4)
        for(int i=-BOX_SIZE/2; i < BOX_SIZE/2; i+=4)
        {
            scan_pts.push_back( Pnt(p.x+i, p.y+j, 0) );
            hits_all.push_back(vector<Pnt>());
            lines_all.push_back(std::vector<PointPair>());
        }

    // make a scan from each point
    int n_scan = 0;
    vector<float> scan_distances;

    for(auto pp=scan_pts.begin(); pp!=scan_pts.end(); pp++)
    {
        float sum_of_dist = 0;
        for(float alfa = 0; alfa < 360; alfa+=1)
        {
            Pnt newP;
            Pnt hitP;
            for(double t=0; t < 1; t+=0.005)
            {
                newP.x = pp->x + t * maze.width() * cos(alfa * GR);
                newP.y = pp->y + t * maze.width() * sin(alfa * GR);
                if( newP.y > maze.height() ) continue;
                if( newP.x > maze.width() ) continue;
                if( newP.y < 0 ) continue;
                if( newP.x < 0 ) continue;
                if( qBlue(maze.pixel(newP.x, newP.y)) == 0xff )                {

                    hitP = newP;
                    hitP.z = alfa;
                    float d = rasst(*pp,newP);
                    if( d < MAX_RAY_LENGTH )
                    {
                        hits_all[n_scan].push_back(hitP);
                        sum_of_dist+=d;
                    }

                    if ( ( alfa==90 || alfa==180 || alfa==270 || alfa==0) &&
                        ( rasst(hitP, p) > MIN_RASST ) )

                    {
                        lines_all[n_scan].push_back(PointPair(*pp, hitP));
                    }

                    break;
                }
             }


        }
        scan_distances.push_back(sum_of_dist);
        n_scan++;
    }

    //select the best scan
    std::vector<float>::iterator result;
    result = std::max_element(scan_distances.begin(), scan_distances.end());
    int index_of_best_scan = std::distance(scan_distances.begin(), result);

    hits = hits_all[index_of_best_scan];
    lines = lines_all[index_of_best_scan];

    //qDebug() << " hitP" << hitP.toStr().c_str();

    // draw starting points
    painter.setPen(Qt::green);
    painter.drawEllipse(QPoint(p.x,p.y),1,1);
    painter.setPen(Qt::red);
    painter.drawEllipse(QPoint(scan_pts[index_of_best_scan].x,scan_pts[index_of_best_scan].y),1,1);

    // draw hits
    for(auto hit=hits.begin(); hit!=hits.end(); hit++)
    {
        painter.setPen(Qt::white);
        painter.drawEllipse(QPoint(hit->x, hit->y),1,1);
    }

    // draw lines
    for(auto line=lines.begin(); line!=lines.end(); line++)
        painter.drawLine(QPoint(line->p.x,line->p.y), QPoint(line->hit.x, line->hit.y));

    //create set of angles

    for(auto line=lines.begin(); line!=lines.end(); line++)
    {
        out_angles.insert(line->hit.z);
    }

    // сджоинить точки с расстоянием < DELTA_HIT
    findTupik(DELTA_HIT, out_maze, painter);
//    //последний интервал
//    if( ((hits.end()-1) ->z - hits.begin()->z) > 250 )
//    {
//        tupik_start = *hit;
//        tupik_end = *hitNext;
//    }



//    std::sort(hits.begin(), hits.end(),
//           [&](const PointPair  a, const PointPair  b) -> bool
//       {
//                 return rasst(p,a.hit) > rasst(p,b.hit);
//       });
    if(hits.size()!=0)
    {
        //painter.setPen(Qt::green);
        //painter.drawEllipse(QPoint(hits[0].p.x,hits[0].p.y),1,1);
    }

    return out_angles;
}


void MazeRunner::findTupik(const float DELTA_HIT, QImage& out_maze, QPainter& painter)
{
    joined.clear();
    joined.push_back(*hits.begin());
    for(auto hit=hits.begin(); hit!=hits.end()-1; hit++)
    {
        auto hitNext = hit + 1;
        if( rasst(*hit, *hitNext) > DELTA_HIT )
            joined.push_back(*hit);
    }
    bool joinLastFirst=false;
    if(rasst(*(hits.end()-1), *(hits.begin())) < DELTA_HIT )
        joinLastFirst = true;
    qDebug() << "rasst last to begin=" << rasst(hits[0],hits[hits.size()-1]);
    joined.push_back(*(hits.end()-1));


    for(auto hit=joined.begin(); hit!=joined.end(); hit++)
        qDebug() << "hit joined" << hit->z;

    // найти тупик
    Pnt tupik_start;
    Pnt tupik_end;
    for(auto hit=joined.begin(); hit!=joined.end()-1; hit++)
    {
        auto hitNext = hit + 1;
        if( (hitNext->z - hit->z) > 270 )
        {
            tupik_start = *hit;
            tupik_end = *hitNext;
        }
    }
    if( joinLastFirst )
    {
         float sum = joined[1]->z - joined[0]->z;
         sum+=(joined.end()-1)->z - (joined.end()-2)->z;
         qDebug() << " sum=" << sum;
         if(sum > 270)
         {
             if(!out_maze.isNull())
             for(auto hit=hits.begin(); hit!=hits.end()-1; hit++)
             {
                 if( (hit->z > joined[0]->z) && (hit->z < joined[1]->z)  )
                 {
                     painter.setPen(Qt::red);
                     painter.drawEllipse(QPoint(hit->x,hit->y),1,1);
                 }
                 if( (hit->z < (joined.end()-1)->z) && (hit->z > (joined.end()-2)->z)  )
                 {
                     painter.setPen(Qt::red);
                     painter.drawEllipse(QPoint(hit->x,hit->y),1,1);
                 }

             }
         }

    }

    qDebug() << "tupik=" << tupik_start.toStr().c_str();
    // закрасить тупик красным
    if(!out_maze.isNull())
    for(auto hit=hits.begin(); hit!=hits.end()-1; hit++)
    {
        if( (hit->z > tupik_start.z) && (hit->z < tupik_end.z)  )
        {
            painter.setPen(Qt::red);
            painter.drawEllipse(QPoint(hit->x,hit->y),1,1);
        }
    }
}

void ImageView::updateImage(QImage& image)
{
    setPixmap(QPixmap::fromImage(image));
}

void ImageView::mousePressEvent(QMouseEvent* ev)
{

    QImage out_maze(maze);
    float sx = (float)maze.width() / geometry().width();
    float sy = (float)maze.height() / geometry().height();
    Pnt p((float)ev->pos().x() * sx,
          (float)ev->pos().y() * sy,0);

    maze_runner->GetScansFromDM(p, maze, out_maze);
    maze_runner->SetCurrentPos(p);

//    Object* veh = (Object*)MREG.FindObj("veh01");
//    CarAutopilot* autopilot = (CarAutopilot*)MREG.FindObj("car_autopilot");
//    if( autopilot )
//    {
//        if(autopilot->turn_map == 0)
//            autopilot->makeTurnMap();

//        veh->ei.coord = autopilot->turn_map->FromPixelToWorld(p.x,p.y);
//        connect(autopilot, SIGNAL(sigoutMaze(QImage&)),this, SLOT(updateImage(QImage&)));
//    }

    int b = qBlue(maze.pixel(p.x, p.y));
    int r = qRed(maze.pixel(p.x, p.y));
    int g = qGreen(maze.pixel(p.x, p.y));
    int a = qAlpha(maze.pixel(p.x, p.y));
    qDebug() << QString("r=%1 g=%2 b=%3 a=%4").arg(QString::number(r),QString::number(g),QString::number(b),QString::number(a));

    setPixmap(QPixmap::fromImage(out_maze));
}

void ImageView::keyPressEvent(QKeyEvent* event)
{
    if( event->key() == Qt::Key_Up )
        maze_runner->GoUp();
    if( event->key() == Qt::Key_Down )
        maze_runner->GoDown();
    if( event->key() == Qt::Key_Left )
        maze_runner->GoLeft();
    if( event->key() == Qt::Key_Right )
        maze_runner->GoRight();
    QImage out_maze(maze);
    maze_runner->GetScansFromDM(maze_runner->cur_pos, maze, out_maze);
    setPixmap(QPixmap::fromImage(out_maze));

}
