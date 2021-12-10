#ifndef MINGAMEWIDGET_H
#define MINGAMEWIDGET_H

#include <QWidget>
#include <map>
#include <tuple>
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


class MinGameWidget : public QWidget
{
    Q_OBJECT
public:
    int cellsX;
    int cellsY;
    int maxValue;
    int cell_size;
    QLearning qlearn;

    std::map<std::tuple<int, int>, Cell* >  cells;
    explicit MinGameWidget(QWidget* parent=0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void BuildStategraph(std::vector<Cell*>& allcells);
    QString PrintQOfSelected();
    void DrawCells(QPainter& painter);
    void SelectCell(QPoint pos);
    void CreateCells(int cellsX, int cellsY);
    void BuildStateGrapth(int cellsX, int cellsY);
    Cell* getCell(int i, int j);
    void ShowOptimalTrajectory(int length);
    void ShowAgentTrajectory(int length);
    Cell* MakeOptimalMove(Cell* cell);
    Cell* MakeAgentMove(Cell* cell);
    void DrawSelected(QPainter& painter);
    void DrawOptimalTrajectory(QPainter& painter, std::vector<Cell*>& trajectory, QColor color);
    int  getActionNumber(Cell* cell_from, Cell* cell_to);
    std::vector<Cell *> getNeibours(Cell* cell);
public slots:
    void saveValues();
    void loadValues();
    void genValues();
    void showIDS();
signals:
    void print(QString s);
private:
    Cell  null_cell;
    Cell* selected_cell;
    std::vector<Cell*> optimal_trajectory;
    std::vector<Cell*> agent_trajectory;
    bool bShowIDS;
};

#endif // MINGAMEWIDGET_H
