#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include "mingamewidget.h"

MinGameWidget::MinGameWidget(QWidget* parent)
    :QWidget(parent)
{
    cellsX = 8;
    cellsY = 8;
    maxValue = 10;
    cell_size = 40;
    selected_cell = &null_cell;
    bShowIDS = false;
    CreateCells(cellsX, cellsY);
    BuildStateGrapth(cellsX, cellsY);
    qlearn.Learning(120000, maxValue, cellsX * cellsY);
}

std::vector<Cell*> MinGameWidget::getNeibours(Cell* cell)
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

void MinGameWidget::BuildStateGrapth(int cellsX, int cellsY)
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

void MinGameWidget::CreateCells(int cellsX, int cellsY)
{
    std::vector<State* > allstates;
    for(int j=0; j < cellsY; j++)
    for(int i=0; i < cellsX; i++)
    {
        Cell* cell = new Cell(i, j);
        cell->value = -maxValue + rand()%(2*maxValue);
        cell->bVisited = false;
        cell->id = j * cellsX + i;
        cells[std::make_tuple(i,j)] = cell;
        allstates.push_back((State*)cell);
    }
    loadValues();
    qlearn.m_allstates = allstates;
}

void MinGameWidget::showIDS()
{
    bShowIDS = !bShowIDS;
    repaint();
}

void MinGameWidget::genValues()
{
    for(int j=0; j < cellsY; j++)
    for(int i=0; i < cellsX; i++)
    {
        getCell(i,j)->value = -maxValue + rand()%(2*maxValue);
    }
    repaint();
}

void MinGameWidget::saveValues()
{
    QFile f("values.txt");
    if( f.open(QFile::WriteOnly | QFile::Text | QFile::Truncate) )
    {
        QTextStream stream(&f);
        for(int j=0; j < cellsY; j++)
        for(int i=0; i < cellsX; i++)
        {
            stream <<  QString::number(getCell(i,j)->value) << " ";
        }
    }
    f.close();
}

void MinGameWidget::loadValues()
{
    QFile f("values.txt");
    if( f.open(QFile::ReadOnly | QFile::Text ) )
    {
        QTextStream stream(&f);
        for(int j=0; j < cellsY; j++)
        for(int i=0; i < cellsX; i++)
        {
            //stream <<  QString::number() << " ";
            stream >> getCell(i,j)->value;
        }
    }
    f.close();
}


void MinGameWidget::DrawSelected(QPainter& painter)
{
    if(selected_cell != &null_cell)
    {
        QRect rc(selected_cell->i * cell_size, selected_cell->j * cell_size, cell_size, cell_size);
        painter.fillRect(rc, Qt::black);
        painter.setPen(Qt::white);
        painter.drawText(rc, Qt::AlignCenter, QString::number(selected_cell->value));
        painter.setPen(Qt::black);
    }
}

void MinGameWidget::DrawOptimalTrajectory(QPainter& painter, std::vector<Cell*>& trajectory , QColor color)
{
    painter.setPen(color);
    for(int i=1; i < trajectory.size(); i++)
    {
        painter.drawLine(trajectory[i-1]->xpix+2, trajectory[i-1]->ypix+2,
                         trajectory[i]->xpix+2, trajectory[i]->ypix+2);

    }
    painter.setPen(Qt::black);
}

void MinGameWidget::DrawCells(QPainter& painter)
{
    for(int j=0; j < cellsY; j++)
    for(int i=0; i < cellsX; i++)
    {
       if( cells.find(std::make_tuple(i,j))!= cells.end() )
       {
           Cell* cell = cells[std::make_tuple(i,j)];
           cell->xpix = i * cell_size + cell_size/2;
           cell->ypix = j * cell_size + cell_size/2;
           //QPoint C( i * cell_size + cell_size/2, j * cell_size + cell_size/2 );
           QRect rc(i * cell_size, j * cell_size, cell_size, cell_size);
           if( cell->bVisited )
               painter.setPen(Qt::red);
           else
               painter.setPen(Qt::black);
           painter.drawRect(rc);
           if(!bShowIDS)
                painter.drawText(rc, Qt::AlignCenter, QString::number(cell->value));
           else
               painter.drawText(rc, Qt::AlignCenter, QString::number(cell->id));
       }
       else
           qDebug("missed %d %d", i, j);
    }

}

Cell* MinGameWidget::getCell(int i, int j)
{
    if( cells.find(std::make_tuple(i,j))!= cells.end() )
        return cells[std::make_tuple(i,j)];
    else
        return &null_cell;
}

void MinGameWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(0.98,0.98);
    DrawCells(painter);
    DrawSelected(painter);
    DrawOptimalTrajectory(painter, optimal_trajectory, Qt::blue);
    DrawOptimalTrajectory(painter, agent_trajectory, Qt::green);
}

void MinGameWidget::SelectCell(QPoint pos)
{
    int i = pos.x() / cell_size;
    int j = pos.y() / cell_size;
    if( cells.find(std::make_tuple(i,j))!= cells.end() )
    {
        selected_cell = cells[std::make_tuple(i,j)];
        //qDebug() << selected_cell->toStr();
    }
}

void MinGameWidget::mousePressEvent(QMouseEvent *ev)
{
    if( ev->button() == Qt::LeftButton )
    {
        SelectCell(ev->pos());
        emit print(QString::number(qlearn.getReward(selected_cell->id,maxValue)));
    }
    repaint();
}

int  MinGameWidget::getActionNumber(Cell* cell_from, Cell* cell_to)
{
    if( (cell_from->j - cell_to->j) == -1 )
         return 0;
     if( (cell_from->j - cell_to->j) == 1 )
         return 1;
     if( (cell_from->i - cell_to->i) == -1 )
         return 2;
      if( (cell_from->i - cell_to->i) == 1 )
         return 3;
     return -1;
}

Cell* MinGameWidget::MakeOptimalMove(Cell* cell)
{
    std::vector<Cell*> neibours = getNeibours(cell);
    std::sort(neibours.begin(), neibours.end(),
        [&](Cell* a, Cell*  b) -> bool
    {
      return a->value < b->value;
    });
    Cell* new_cell = neibours[0];
    return neibours[0];
}

void MinGameWidget::ShowAgentTrajectory(int length)
{
    agent_trajectory.clear();
    agent_trajectory.push_back(selected_cell);
    Cell* cell = selected_cell;
    float reward=0;
    for(int i=0; i < length; i++)
    {
        cell = MakeAgentMove(cell);
        agent_trajectory.push_back(cell);
        reward+=qlearn.getReward(cell->id,maxValue);
    }
    print(QString("total reward is %1").arg(reward));
    repaint();
}

void MinGameWidget::ShowOptimalTrajectory(int length)
{
    for(int i=0; i < optimal_trajectory.size(); i++)
        optimal_trajectory[i]->bVisited = false;
    optimal_trajectory.clear();
    optimal_trajectory.push_back(selected_cell);
    Cell* cell = selected_cell;
    cell->bVisited = true;
    for(int i=0; i < length; i++)
    {
        cell = MakeOptimalMove(cell);
        cell->bVisited = true;
        optimal_trajectory.push_back(cell);
    }
    repaint();
}


QString getArrow(int a)
{
    switch(a)
    {
        case 0: return "right";
        case 1: return "left ";
        case 2: return "down ";
        case 3: return "up   ";
        default: return "";
    }

}

QString MinGameWidget::PrintQOfSelected()
{
    QString out;
    std::vector<Cell*> neibours = getNeibours(selected_cell);
    int a = 0;
    for( auto it=neibours.begin(); it!=neibours.end(); it++)
    {
        int s0 = selected_cell->id;
        int f = qlearn.getFreq(s0,a);
        int s = qlearn.getState(s0,a);
        float Q = qlearn.Q[std::make_tuple(s0,a)];
        float Q0 = qlearn.Q_original[std::make_tuple(s0,a)];
        out+=QString("s0=%1 s=%2 Q=%3 Q0=%4 f=%5 a=%6").arg(s0).arg(s).arg(Q).arg(Q0).arg(f).arg(getArrow(a)) + "\n";
        a++;
    }
    return out;
}

Cell* MinGameWidget::MakeAgentMove(Cell* cell)
{
    std::vector<Cell*> neibours = getNeibours(cell);
    int a = 0;
    int amax = 0;
    float maxQ=0;
    for( auto it=neibours.begin(); it!=neibours.end(); it++)
    {
        int s0 = cell->id;
        float Q = qlearn.Q[std::make_tuple(s0,a)];
        if(maxQ < Q)
        {
            maxQ = Q;
            amax = a;
        }
        a++;
    }
    return neibours[amax];
}
