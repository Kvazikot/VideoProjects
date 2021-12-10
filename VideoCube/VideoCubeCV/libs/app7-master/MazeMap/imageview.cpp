#include "imageview.h"
#include <QPainter>
#include <QDebug>
#include <math.h>
#include <iostream>
#include "../include/geometry.h"
#include "qlearning.h"
//#include "../include/cobjregistry.h"
//#include "../include/car_autopilot.h"

//static CObjRegistry& MREG = CObjRegistry::getInstance2();


int ImageView::getKey(Pnt p)
{
    int ix,iy;
    int block_sizeX = maze.width() / (maze_runner->cell_sizeX);
    int block_sizeY = maze.height() / (maze_runner->cell_sizeY);
    ix = int(p.x) / block_sizeX;
    iy = int(p.y) / block_sizeY;
    int n_block = ix + 8 * iy;
    bStopTimer = false;
    //qDebug() << x;
    //qDebug() << y;
    return int(n_block);
}

MazeRunner::MazeRunner(QImage& maze)
    : maze(maze)
{
    cell_sizeX = 8;
    cell_sizeY = 8;
    //create grapth
    for(int i=0; i < 1000; i++)
    {
        vector<int> v;
        v.resize(1000);
        g.push_back(v);
        for(int j=0; j < 1000; j++)
            g[i][j] = 1;

    }
}


ImageView::ImageView(QWidget *parent) : QLabel(parent)
{
    maze.load("D:\\src\\app7-mingw\\app7\\Data\\maze.png");
    maze_runner = new MazeRunner(maze);
    speed = 7;
    kurs = 0;
    bWorking = false;
    T0 = 0;
    cellsX = 8;
    cellsY = 8;
    CreateCells(cellsX, cellsY);
    BuildStateGrapth(cellsX, cellsY);
    qlearn.initQ(64);
    qlearn.initStates();


    qDebug() << getKey(Pnt(24,23,1));
    qDebug() << getKey(Pnt(26,24,1));

}

Pnt ImageView::movePoint()
{
    float dist2 = speed * 0.1;
    Pnt newP;
    newP.x = maze_runner->cur_pos.x + dist2 * cos(kurs * GR);
    newP.y = maze_runner->cur_pos.y + dist2 * sin(kurs * GR);
    maze_runner->SetCurrentPos(newP);

    return newP;
}

Cell* ImageView::getCell(int i, int j)
{
    if( cells.find(std::make_tuple(i,j))!= cells.end() )
        return cells[std::make_tuple(i,j)];
    else
        return &null_cell;
}

std::vector<Cell*> ImageView::getNeibours(Cell* cell)
{
    std::vector<Cell*> neibours;
    neibours.push_back(getCell(cell->i+1,cell->j));
    neibours.push_back(getCell(cell->i-1,cell->j));
    neibours.push_back(getCell(cell->i,cell->j+1));
    neibours.push_back(getCell(cell->i,cell->j-1));
    auto end_it = std::remove_if(neibours.begin(), neibours.end(),
    [&](Cell* a) -> bool
    {
        return a == &null_cell;
    });
    neibours.erase(end_it,neibours.end());
    return neibours;
}

void ImageView::BuildStateGrapth(int cellsX, int cellsY)
{
    std::vector<std::vector<int> > g;
    for(int i=0; i < cellsX*cellsY; i++)
        g.push_back(std::vector<int>());

    for(int j=0; j < cellsY; j++)
    for(int i=0; i < cellsX; i++)
    {
        Cell* cell = getCell(i,j);
        std::vector<Cell*> neibours = getNeibours(cell);
        for( auto it=neibours.begin(); it!=neibours.end(); it++)
        {
            g[cell->id].push_back((*it)->id)  ;
            //Q_ASSERT((*it)->id < -1);
        }
    }
    qlearn.SetStateGrapth(g);
}

void ImageView::CreateCells(int cellsX, int cellsY)
{
    std::vector<State* > allstates;
    for(int j=0; j < cellsY; j++)
    for(int i=0; i < cellsX; i++)
    {
        Cell* cell = new Cell(i, j);
        cell->value = 0;
        cell->bVisited = false;
        cell->id = j * cellsX + i;
        cells[std::make_tuple(i,j)] = cell;
        allstates.push_back((State*)cell);
    }
    qlearn.m_allstates = allstates;
}


int reverse_kurs(int kurs)
{
    if(kurs == 0) return 180;
    if(kurs == 180) return 0;
    if(kurs == 90) return 270;
    if(kurs == 270) return 90;
}

bool ImageView::checkExit(int id)
{
    vector<int> exits = {7,23,39,55,57,59,61};
    for(int i=0; i < exits.size(); i++)
        if( id == exits[i])
            return true;

    return false;
}



void ImageView::drawExitText(QImage& out_maze)
{
    QPainter painter(&out_maze);
    painter.setPen(Qt::red);
    QFont font;
    font.setPixelSize(25);
    painter.setFont(font);
    painter.drawText(100,100,"Exit!!!");
    setPixmap(QPixmap::fromImage(out_maze));
    speed = 0;
    killTimer(timerID);
}

void ImageView::getDistToLines(vector<int>& distances)
{
    if(kurs == 270 || kurs == 90)
    {
        distances = maze_runner->distanceToHorizLines(maze_runner->cur_pos);
        //qDebug() << distances[0] << " " << distances[1];
    }
    if(kurs == 0 || kurs == 180)
    {
        distances = maze_runner->distanceToVerticalLines(maze_runner->cur_pos);

    }
}

Pnt ImageView::movePointB()
{
    if(speed == 0) return Pnt();
    double d = 10; // расстояние до точки B
    double tb = d / speed; // время за которое точка проходит расстояние d
    double t_povorota = 2 * tb; // время поворота по дуге
    double ttek = (double)ttek_timer.elapsed()/1000.;

    double t0 = T0 - tb;
    double tt = (ttek - T0) / t_povorota;
    double kursb = kurs_from + tt * (kurs_to - kurs_from);


    qDebug() << "ttek=" << ttek << " kurs_from=" << kurs_from << " kurs_to=" << kurs_to << " kursb=" << kursb;
    if ( (kurs_from == 270 ) && ( kurs_to == 0 ) )
           kurs_to = 360;
    if ( (kurs_from == 0 ) && ( kurs_to == 270 ) )
           kurs_from = 360;

    if(T0 == ttek )
    {
        B0.x = maze_runner->cur_pos.x + d * cos((kurs_from-180) * GR);
        B0.y = maze_runner->cur_pos.y + d * sin((kurs_from-180) * GR);
    }

    Pnt B;
    float dist = 7 * 0.1;
    B0.x = B0.x + dist * cos(kursb*GR);
    B0.y = B0.y + dist * sin(kursb*GR);
    if(abs(kurs_to - kurs_from)==180)
    {
        B0.x = maze_runner->cur_pos.x + d * cos((kurs_to-180) * GR);
        B0.y = maze_runner->cur_pos.y + d * sin((kurs_to-180) * GR);
    }

    return B0;
}


void ImageView::timerEvent(QTimerEvent* )
{
    const int MAX_DIST_TO_LINE = 15;

    if(bWorking == true) return;

    bWorking = true;

    Pnt newP = movePoint();

    QImage out_maze(maze);

    maze_runner->GetScansFromDM(newP, maze, out_maze);
    vector<int> distances;
    getDistToLines(distances);

    //qDebug() << distances[0] << " " << distances[1];

    if((distances[0] < MAX_DIST_TO_LINE) && (distances[1] < MAX_DIST_TO_LINE) )
    {

        if( abs(distances[0] - distances[1]) < 2 )
        {
            speed = 0;
            int cur_id = getKey(maze_runner->cur_pos);
            if( checkExit(cur_id) )
            {
                drawExitText(out_maze);
                return;
            }

            if(maze_runner->out_angles.size())
            {
                int s0;
                for(int iters=0; iters < 100; iters++)
                for(int i=0; i < maze_runner->out_angles.size(); i++)
                {
                    //select next kurs

                    //int i = rand() % maze_runner->out_angles.size();
                    int id_out = maze_runner->GetPathsForNode(cur_id)[i];
                    int s = id_out;
                    s0 = cur_id; // состояние
                    int a0 = i; // действие

                    //reward
                    float r =  1. / maze_runner->g[cur_id][id_out] ;
                    //qDebug() << "REWARD " << r;
                    qlearn.Q[make_tuple(s0,a0)] = qlearn.Q[make_tuple(s0,a0)] +  qlearn.LF * (r + qlearn.DF * qlearn.MAXQ(s,maze_runner->out_angles.size()) - qlearn.Q[make_tuple(s0,a0)]);
                }
                    int a = 0; // новое действие
                    a = qlearn.ARGMAX(s0,maze_runner->out_angles.size()); //Decision (Выбор действия):
                    int id_out = maze_runner->GetPathsForNode(cur_id)[a];
                    float r =  1. / maze_runner->g[cur_id][id_out] ;
                    maze_runner->g[cur_id][id_out]++;
                    maze_runner->g[id_out][cur_id]++;

                    qDebug() << "1. from " << cur_id << " to " << id_out << " gvalue" << maze_runner->g[cur_id][id_out] << " reward" << r;
                    qDebug() << " Q_selected=" << qlearn.Q[make_tuple(s0,a)] ;
                    qDebug() << "2. n_actions=" << maze_runner->out_angles.size() << " ARGMAX a=" << a;
                    kurs_from = kurs;
                    kurs = maze_runner->out_angles[a];
                    kurs_to = kurs;

            }
            speed = 7;
            movePoint();
            movePoint();
            T0 = (double)ttek_timer.elapsed()/1000.;
        }
        
    }

    Pnt B = movePointB();
    //draw point B
    QPainter painter(&out_maze);
    painter.setPen(Qt::green);
    painter.drawEllipse(QPoint(B.x,B.y),1,1);

    setPixmap(QPixmap::fromImage(out_maze));
    bWorking = false;
}



float rasst(Pnt p1, Pnt p2)
{
     float _t1 = (float)sqrt( (double) ( sq(p1.x - p2.x)  +  sq(p1.y - p2.y )))  ;
     return _t1;
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
    //qDebug() << "rasst last to begin=" << rasst(hits[0],hits[hits.size()-1]);
    joined.push_back(*(hits.end()-1));


    //for(auto hit=joined.begin(); hit!=joined.end(); hit++)
    //    qDebug() << "hit joined" << hit->z;

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
         //qDebug() << " sum=" << sum;
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

   //qDebug() << "tupik=" << tupik_start.toStr().c_str();
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

void MazeRunner::findLines(std::vector<Pnt>& hits)
{
    const int MIN_POINTS_IN_LINE = 20;
    //find horizontal lines
    //qDebug() << "tan(90)" << tan(90*GR);
    hor_lines.clear();
    ver_lines.clear();
    for(int j=0; j < maze.height(); j++)
    {
        int cntY = 0;
        int cntX = 0;
        for(auto hit=hits.begin(); hit!=hits.end(); hit++)
        {
            if( int(hit->y) == j ) cntY++;
            if( int(hit->x) == j ) cntX++;
        }
        if( cntY > MIN_POINTS_IN_LINE )
            hor_lines.push_back(Pnt(0,j,0));
        if( cntX > MIN_POINTS_IN_LINE )
            ver_lines.push_back(Pnt(j,0,0));
    }
}

vector<int> MazeRunner::distanceToHorizLines(Pnt p)
{
    vector<int> distances;
    for(auto line=hor_lines.begin(); line!=hor_lines.end(); line++)
    {
        distances.push_back(rasst(p,Pnt(p.x,line->y,0)));
    }
    std::sort(distances.begin(), distances.end(), std::less<int>());
    return distances;
}

vector<int> MazeRunner::distanceToVerticalLines(Pnt p)
{
    vector<int> distances;
    for(auto line=ver_lines.begin(); line!=ver_lines.end(); line++)
    {
        distances.push_back(rasst(p,Pnt(line->x,p.y,0)));
    }
    std::sort(distances.begin(), distances.end(), std::less<int>());
    return distances;
}

void MazeRunner::drawLines(QPainter& painter)
{
    for(auto line=hor_lines.begin(); line!=hor_lines.end(); line++)
    {
        painter.setPen(Qt::white);
        painter.drawLine(QPoint(0,line->y), QPoint(maze.width(), line->y));
    }
    for(auto line=ver_lines.begin(); line!=ver_lines.end(); line++)
    {
        painter.setPen(Qt::white);
        painter.drawLine(QPoint(line->x,0), QPoint(line->x,maze.height()));
    }

}

std::vector<float>  MazeRunner::GetScansFromDM(Pnt p, QImage& maze, QImage& out_maze)
{
    //Pnt p(2,46,0);
    const float DELTA_HIT = 6;
    const float MIN_RASST_RAY = 30;

    hits.clear();
    lines.clear();
    out_angles.clear();

    QPainter painter(&out_maze);

    for(float alfa = 0; alfa < 360; alfa+=1)
    //float alfa = 45;ё
    {
        Pnt newP;
        Pnt hitP;
        for(double t=0; t < 1; t+=0.005)
        {
            newP.x = p.x + t * maze.width() * cos(alfa * GR);
            newP.y = p.y + t * maze.width() * sin(alfa * GR);
            if( newP.y > maze.height() ) continue;
            if( newP.x > maze.width() ) continue;
            if( newP.y < 0 ) continue;
            if( newP.x < 0 ) continue;
            if( qBlue(maze.pixel(newP.x, newP.y)) == 0xff )
            {
                hitP = newP;
                hitP.z = alfa;
                hits.push_back(hitP);
                if ( alfa==90 || alfa==180 || alfa==270 || alfa==0)
                {
                    float d = rasst(hitP, p);

                    if( d > MIN_RASST_RAY  )
                    {
                        lines.push_back(PointPair(p, hitP));
                    }
                }

                break;
            }

        }



        //qDebug() << " hitP" << hitP.toStr().c_str();
//        if(!out_maze.isNull())
//        {
//            painter.setPen(Qt::white);
//            painter.drawEllipse(QPoint(hitP.x, hitP.y),1,1);
//            painter.setPen(Qt::red);
//            painter.drawEllipse(QPoint(p.x,p.y),1,1);
//        }

        //create set of angles


    }

    for(auto line=lines.begin(); line!=lines.end(); line++)
    {
        out_angles.push_back(line->hit.z);
    }
   // qDebug() << "out_angles.size=" << out_angles.size();
    painter.setPen(Qt::red);
    painter.drawEllipse(QPoint(p.x,p.y),1,1);

    findLines(hits);
    drawLines(painter);

    painter.setPen(Qt::white);
    for(auto line=lines.begin(); line!=lines.end(); line++)
        painter.drawLine(QPoint(line->p.x,line->p.y), QPoint(line->hit.x, line->hit.y));

    // сджоинить точки с расстоянием < DELTA_HIT
    //findTupik(DELTA_HIT, out_maze, painter);

    if(hits.size()!=0)
    {
        //painter.setPen(Qt::green);
        //painter.drawEllipse(QPoint(hits[0].p.x,hits[0].p.y),1,1);
    }

    return out_angles;
}

void ImageView::updateImage(QImage& image)
{
    setPixmap(QPixmap::fromImage(image));
}

vector<int> MazeRunner::GetPathsForNode(int id)
{
    vector<int> paths;
    int block_sizeX = maze.width() / (cell_sizeX);
    int block_sizeY = maze.height() / (cell_sizeY);
    int ix = int(cur_pos.x) / block_sizeX;
    int iy = int(cur_pos.y) / block_sizeY;
    int n_block;

    for(int i=0; i < out_angles.size(); i++)
    {
        if(out_angles[i] == 270  )
            n_block = ix + 8 * (iy-1);
        if(out_angles[i] == 90  )
            n_block = ix + 8 * (iy+1);
        if(out_angles[i] == 0  )
            n_block = (ix + 1) + 8 * iy;
        if(out_angles[i] == 180  )
            n_block = (ix - 1) + 8 * iy;
        paths.push_back(n_block);
    }

    return paths;

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
    kurs_from = kurs;
    kurs_to = kurs;

//    Object* veh = (Object*)MREG.FindObj("veh01");
//    CarAutopilot* autopilot = (CarAutopilot*)MREG.FindObj("car_autopilot");
//    if( autopilot )
//    {
//        if(autopilot->turn_map == 0)
//            autopilot->makeTurnMap();

//        veh->ei.coord = autopilot->turn_map->FromPixelToWorld(p.x,p.y);
//        connect(autopilot, SIGNAL(sigoutMaze(QImage&)),this, SLOT(updateImage(QImage&)));
//    }

    qDebug() << "current cell " << getKey(p);
    //vector<int> paths = maze_runner->GetPathsForNode(getKey(p));
    //for(int i=0; i < paths.size(); i++)
    //    qDebug() << "path to " << paths[i];


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
    if( event->key() == Qt::Key_Space )
    {
        if(bStopTimer)
        {
           timerID = startTimer(100);
           ttek_timer.start();
        }
        else
        {
            killTimer(timerID);
        }
        bStopTimer = !bStopTimer;

    }
    QImage out_maze(maze);
    maze_runner->GetScansFromDM(maze_runner->cur_pos, maze, out_maze);
    setPixmap(QPixmap::fromImage(out_maze));

}
