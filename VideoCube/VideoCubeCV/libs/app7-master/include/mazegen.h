#ifndef MAZEGEN_H
#define MAZEGEN_H

#include <QImage>
#include <tuple>
#include <map>
#include "gemobject.h"

struct Cell;

enum WALL_TYPE
{
   LEFT, RIGHT, TOP, BOTTOM
};


// стена клетки
struct Wall
{
    WALL_TYPE type;
    Cell*     parent_cell;
    Cell*     parent_cell2;
    std::vector<Wall*>  neibours;
    bool      bVisible;
    Wall();
    Wall(WALL_TYPE t, Cell* parent);
    void SetCells(Cell* c1, Cell* c2);
    bool isBorder(){ return  parent_cell2 == 0;}
};

// клетка лабиринта
struct Cell
{
    int   i,j;
    int   xpix, ypix;
    float xworld, zworld;
    bool  bVisited;
    bool  bReachable;
    int   value;
    GemObject* object;
    Cell();
    Cell(int ii, int jj);
    void setIJ(int ii, int jj){ i=ii; j=jj;}
    bool isNull();
    void addWalls(std::vector<Wall*>& list);
    void addNeiborWalls(WALL_TYPE t, std::vector<Wall*>& list);
    Wall* leftWall;
    Wall* rightWall;
    Wall* topWall;
    Wall* bottomWall;

};

class MazeGen
{
public:
    std::map<std::tuple<int, int>, Cell* >  cells;
    std::vector<Wall*> wall_list;
    MazeGen();
    MazeGen(QImage& image, int cellsX, int cellsY, int wall_width);
    void CreateCells(int cellsX, int cellsY);
    void DrawCells(QImage& image);
    void CreateMaze();
    int getCellsX() { return cellsX; }
    int getCellsY() { return cellsY; }
    float getCellPixelWidth();
    Cell* getCell(int i, int j);
protected:
    int cellsX;
    int cellsY;
    int wall_width;

};

#endif // MAZEGEN_H
