#ifndef LINUXOID_H
#define LINUXOID_H

#include <QImage>
#include "../include/object.h"
#include <vector>
#include "../include/gemobject.h"
#include "../include/mazegen.h"
#include "../include/rectangle.h"

struct TreeNode
{
    Cell* my_cell;
    TreeNode(Cell* cell)
    {
        my_cell = cell;
    }
    void AddChild(TreeNode* child)
    {
        children.push_back(child);
    }
    vector<TreeNode*> children;
};

class Windows : public GemObject
{
public:
    Windows(MazeGen*    mmaze);
    double   t0, trans_time;
    int      id;
    Cell*    my_cell;
    Cell*    my_new_cell;
    MazeGen*    maze;
    void SetID(int id) { this->id = id; }
    void DoTransition();
    void Slot1();
};

class Linuxoid : public Object
{
public:
    Linuxoid(string spritename, MazeGen* mazegen, QImage& maze_image);
    GemObject*  linuxsprite;
    vector<Windows*>  enemies;
    MazeGen*    maze;
    Cell*       my_cell;
    QImage      image;
    int         score;
    int         Nenemys;

    void Declare()
    {
        SetName("linuxoid01");
        SET_ATTR("pos", &ei.coord, "Pnt");
        SET_ATTR("score", &score, "int");
        SET_ATTR("Nenemys", &Nenemys, "int");
        ei.classname = string("Linuxoid");
        classname = string("Linuxoid");
    }
    void Slot1();
    void process_cell(Cell* cell);
    void make_tree(TreeNode* root);
    void reset_reachable();
    void delete_unreachable_coins();
    void create_linuxoid(string spritename);
    void create_enemys(int N);
    void highlight_cell(int i, int j);
    void click_lmb(int state);
    void key_pressed(int k);
    void CheckColision();
    int  CountCoins();
    void Kill();
    void print_actions_rewards(Cell* cell);    
    bool cellHasEnemy(Cell* cell);
    std::vector<Cell*> getNeibours(Cell* my_new_cell);
    int goUp2Wall(Cell* cell, int steps);
    int goDown2Wall(Cell* cell, int steps);
    int goLeft2Wall(Cell* cell, int steps);
    int goRight2Wall(Cell* cell, int steps);
};

#endif // LINUXOID_H
