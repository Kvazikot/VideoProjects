#include <QDebug>
#include <QPainter>
#include "mazegen.h"
#include <time.h>

Wall::Wall(WALL_TYPE t, Cell* parent)
{
    type = t;
    parent_cell = parent;
    parent_cell2 = parent;
    bVisible = true;
}

void Wall::SetCells(Cell* c1, Cell* c2)
{
    parent_cell = c1;
    parent_cell2 = c2;
}

Cell::Cell()
{
    bVisited = false;
}

Cell::Cell(int ii, int jj): i(ii), j(jj)
{
    leftWall = new Wall(LEFT, this);
    rightWall = new Wall(RIGHT, this);
    topWall = new Wall(TOP, this);
    bottomWall = new Wall(BOTTOM, this);
    bVisited = false;
}

void Cell::addWalls(std::vector<Wall*>& list)
{
    list.push_back(leftWall);
    list.push_back(rightWall);
    list.push_back(topWall);
    list.push_back(bottomWall);
}

void Cell::addNeiborWalls(WALL_TYPE t, std::vector<Wall*>& list)
{
    if( t == LEFT || t == RIGHT )
    {
        list.push_back( topWall );
        list.push_back( bottomWall );
    }
    if( t == BOTTOM || t == TOP )
    {
        list.push_back( leftWall );
        list.push_back( rightWall );
    }

}

MazeGen::MazeGen()
{
}

void dilateImage(QImage& I)
{

    QImage O = I;
    unsigned int Nq[8];
    unsigned int MAX;

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
                MAX = Nq[0] & 255;
                for(int i=0; i < 8; i++)
                   if( (Nq[i] & 255) > MAX )
                       MAX = Nq[i] & 255;
                O.setPixel(x,y,qRgba(0,0,MAX,255));


        }
    }
    I = O;
}

MazeGen::MazeGen(QImage& image, int cellsX, int cellsY, int wall_width)
{
    this->cellsX = cellsX;
    this->cellsY = cellsY;
    this->wall_width = wall_width;
    CreateCells(cellsX, cellsY);
    CreateMaze();
    DrawCells(image);
    /*dilateImage(image);
    dilateImage(image);
    dilateImage(image);
    dilateImage(image);
    */
}

void MazeGen::CreateMaze()
{
    Cell* rand_cell = cells[std::make_tuple(rand()%cellsX, rand()%cellsY)];
    if(  rand_cell == 0) return;
    rand_cell->bVisited = true;
    rand_cell->addWalls(wall_list);

    srand (time(NULL));

    while( !wall_list.empty() )
    {
        int n = rand() % wall_list.size();
        Wall* wall = wall_list[n];
        if( wall->parent_cell && wall->parent_cell2)
        {
            if( (wall->parent_cell->bVisited && !wall->parent_cell2->bVisited) )
            {
                wall->bVisible = false;
                wall->parent_cell2->addNeiborWalls(wall->type, wall_list);
                //copy(wall->neibours.begin(), wall->neibours.end(), std::back_inserter(wall_list));
                wall->parent_cell2->bVisited = true;
            }

            if( (!wall->parent_cell->bVisited && wall->parent_cell2->bVisited) )
            {
                wall->bVisible = false;
                wall->parent_cell->addNeiborWalls(wall->type, wall_list);
                //copy(wall->neibours.begin(), wall->neibours.end(), std::back_inserter(wall_list));
                wall->parent_cell->bVisited = true;
            }

            if( !(wall->parent_cell->bVisited ^ wall->parent_cell2->bVisited) )
            {
                wall_list.erase( find(wall_list.begin(),wall_list.end(), wall) );
            }

        }
    }


}



void MazeGen::DrawCells(QImage& image)
{
    int cell_size = image.width() / cellsX;

    QPainter painter(&image);

    painter.scale(0.99,0.99);
    painter.translate(cell_size/2, cell_size/2);

    for(int i=0; i < cellsX; i++)
    for(int j=0; j < cellsY; j++)
    {
       if( cells.find(std::make_tuple(i,j))!= cells.end() )
       {
           Cell* cell = cells[std::make_tuple(i,j)];
           QPoint C( i * cell_size, j * cell_size );
           if( cell->leftWall->bVisible )
           {
               QRect wall_rc( C.x() - cell_size/2 - wall_width/2, C.y() - cell_size/2 - wall_width/2 , wall_width, cell_size + wall_width );
               //painter.drawRect(wall_rc);
               painter.fillRect(wall_rc, Qt::blue);
           }
           if( cell->rightWall->bVisible )
           {
               QRect wall_rc( C.x() + cell_size/2 - wall_width/2, C.y() - cell_size/2  - wall_width/2 , wall_width, cell_size + wall_width );
               //painter.drawRect(wall_rc);
               painter.fillRect(wall_rc, Qt::blue);
           }
           if( cell->topWall->bVisible )
           {
               QRect wall_rc( C.x() - cell_size/2 - wall_width/2 , C.y() - cell_size/2 - wall_width/2, cell_size + wall_width, wall_width );
               //painter.drawRect(wall_rc);
               painter.fillRect(wall_rc, Qt::blue);
           }
           if( cell->bottomWall->bVisible )
           {
               QRect wall_rc( C.x() - cell_size/2 - wall_width/2, C.y() + cell_size/2 - wall_width/2, cell_size + wall_width, wall_width );
               //painter.drawRect(wall_rc);
               painter.fillRect(wall_rc, Qt::blue);
           }

       }
       else
           qDebug("missed %d %d", i, j);

    }


}

void MazeGen::CreateCells(int cellsX, int cellsY)
{
    int cellsXAdd = cellsX+5;
    int cellsYAdd = cellsY+5;

    for(int i=0; i < cellsXAdd; i+=2)
    for(int j=0; j < cellsYAdd; j+=2)
    {
        cells[std::make_tuple(i,j)] = new Cell(i, j);
    }

    //проставить смежность
    for(int i=1; i < cellsXAdd-1; i+=2)
    for(int j=0; j < cellsYAdd; j+=2)
    {
        Cell* c = new Cell();
        cells[std::make_tuple(i,j)] = c;
        c->setIJ(i,j);
        c->leftWall = cells[std::make_tuple(i-1,j)]->rightWall;
        c->rightWall = cells[std::make_tuple(i+1,j)]->leftWall;
        c->topWall = new Wall(TOP, c);
        c->bottomWall = new Wall(BOTTOM, c);
    }

    for(int i=0; i < cellsXAdd; i+=2)
    for(int j=1; j < cellsYAdd-1; j+=2)
    {
        Cell* c = new Cell();
        cells[std::make_tuple(i,j)] = c;
        c->setIJ(i,j);
        c->leftWall = new Wall(LEFT, c);
        c->rightWall = new Wall(RIGHT, c);
        c->topWall = cells[std::make_tuple(i,j-1)]->bottomWall;
        c->bottomWall = cells[std::make_tuple(i,j+1)]->topWall;
    }


    for(int i=1; i < cellsXAdd-1; i+=2)
    for(int j=1; j < cellsYAdd-1; j+=2)
    {
        Cell* c = new Cell();
        cells[std::make_tuple(i,j)] = c;
        c->setIJ(i,j);
        c->leftWall = cells[std::make_tuple(i-1,j)]->rightWall;
        c->rightWall = cells[std::make_tuple(i+1,j)]->leftWall;
        c->topWall = cells[std::make_tuple(i,j-1)]->bottomWall;
        c->bottomWall = cells[std::make_tuple(i,j+1)]->topWall;
    }


    //return;
    // дл€ каждой стены смежные €чейки
    for(int i=0; i < cellsXAdd-1; i++)
    for(int j=0; j < cellsYAdd-1; j++)
    {
        Cell* c = cells[std::make_tuple(i,j)];
        if( i > 0 )
          c->leftWall->SetCells(c, cells[std::make_tuple(i-1,j)]);
        if( i < (cellsXAdd-2) )
          c->rightWall->SetCells(c, cells[std::make_tuple(i+1,j)]);
        if( j > 0 )
          c->topWall->SetCells(c, cells[std::make_tuple(i,j-1)]);
        if( j < (cellsYAdd-2) )
          c->bottomWall->SetCells(c, cells[std::make_tuple(i,j+1)]);
    }




}
