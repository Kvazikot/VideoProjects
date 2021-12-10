#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QElapsedTimer>
#include "../include/geometry.h"
#include <set>
#include "qlearning.h"

class Cell : public State
{
public:
    int   i,j;
    int   xpix, ypix;
    Cell():State(){ i=-1; j=-1; id=-1; value=10e3; }
    Cell(int ii, int jj){ setIJ(ii,jj); }
    void setIJ(int ii, int jj){ i=ii; j=jj;}
    bool isNull();
    QString toStr(){ return QString("i=%1 j=%2 val=%3").arg(i).arg(j).arg(value); }
};


struct PointPair
{
   PointPair(Pnt p, Pnt hit):
       p(p), hit(hit){}
   Pnt p;
   Pnt hit;
};



class MazeRunner
{
public:
    MazeRunner(QImage& maze);
    QImage maze;
    int    cell_sizeX;
    int    cell_sizeY;

    Pnt   cur_pos;
    float kurs;
    std::vector<float> out_angles;
    std::vector<Pnt> hits;
    std::vector<PointPair> lines;
    vector<Pnt> joined;
    vector<Pnt> hor_lines;
    vector<Pnt> ver_lines;
    // grath of maze
    std::vector<std::vector<int> > g;
    int s; //state
    int a; //action


    std::vector<float>  GetScansFromDM(Pnt p, QImage& maze, QImage& out_maze);
    void SetCurrentPos(Pnt p){ cur_pos = p;}
    void GoUp(){ cur_pos.y-=1; }
    void GoDown(){ cur_pos.y+=1; }
    void GoLeft(){ cur_pos.x-=1; }
    void GoRight(){ cur_pos.x+=1; }
    void findTupik(const float DELTA_HIT, QImage& out_maze, QPainter& painter);
    void findLines(std::vector<Pnt>& hits);
    void drawLines(QPainter& painter);
    vector<int> distanceToVerticalLines(Pnt p);
    vector<int> distanceToHorizLines(Pnt p);
    vector<int> GetPathsForNode(int id);


};

class ImageView : public QLabel
{
    Q_OBJECT
public:
    QElapsedTimer ttek_timer;
    QImage maze;
    MazeRunner* maze_runner;
    float speed;
    float kurs, kurs_from, kurs_to;
    double T0;
    Pnt B0;
    bool  bWorking;
    bool  bStopTimer;
    int timerID;
    bool bExitFound;
    //point  last selected kurs
    int cellsX;
    int cellsY;
    Cell  null_cell;
    QLearning qlearn;
    std::map<std::tuple<int, int>, Cell* >  cells;

    explicit ImageView(QWidget *parent = nullptr);

    bool checkExit(int id);
    void mousePressEvent(QMouseEvent* );
    void keyPressEvent(QKeyEvent* );
    void timerEvent(QTimerEvent* );
    int getKey(Pnt p);
    Pnt movePoint();
    Pnt movePointB();

    Cell* getCell(int i, int j);
    std::vector<Cell*> getNeibours(Cell* cell);
    void BuildStateGrapth(int cellsX, int cellsY);
    void CreateCells(int cellsX, int cellsY);

    void drawExitText(QImage& out_maze);

    void getDistToLines(vector<int>& distances);

signals:

public slots:
    void updateImage(QImage& image);
};

#endif // IMAGEVIEW_H
