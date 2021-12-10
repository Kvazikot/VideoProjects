#include <QDebug>
#include "../include/cinput.h"
#include "../include/print.h"
#include "linuxoid.h"
#include "animsprite.h"
#include "../include/timer.h"
#include "../include/cobjregistry.h"
#include "../include/rectangle.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();
static app6::Rectangle*  red_rectangle;

Linuxoid::Linuxoid(string spritename, MazeGen* mazegen, QImage& maze_image) :
    maze(mazegen), image(maze_image)
{
    score = 0;
    Nenemys = 4;
    create_linuxoid(spritename);
    create_enemys(Nenemys);

    //квадратик для подсветки хода
    Cell* cell = maze->getCell( 1,  1);
    float pixel_width = maze->getCellPixelWidth();
    red_rectangle = new app6::Rectangle(QRectF(cell->xworld-pixel_width/2,
                                               cell->zworld-pixel_width/2,
                                               pixel_width/2,pixel_width/2),
                                               QVector4D(1,0,0,1),
                                                10,
                                                3);
    red_rectangle->isCanRender = true;
    MREG.AddObj(red_rectangle);

    CInput* iinput = CInput::getInstance();
    iinput->add_delegat(this);

    // запрос на апдейт
    Connect();

}


void Linuxoid::create_linuxoid(string spritename)
{
    linuxsprite = new GemObject();
    linuxsprite->CopyMesh(spritename);
    linuxsprite->CenterModel();
    linuxsprite->Declare();
    linuxsprite->update();
    Cell* cell = maze->getCell( rand()%maze->getCellsX(),  rand()%maze->getCellsY());
    reset_reachable();
    TreeNode* root = new TreeNode(cell);
    make_tree(root);
    delete_unreachable_coins();
    my_cell = cell;
    linuxsprite->ei.coord.x = cell->xworld;
    linuxsprite->ei.coord.z = cell->zworld;
    linuxsprite->isCanRender = true;
    linuxsprite->model_matrix.setToIdentity();
    linuxsprite->model_matrix.translate(cell->xworld, 0, cell->zworld);
    MREG.AddObjNew(linuxsprite);
}

void Linuxoid::highlight_cell(int i, int j)
{
    Cell* cell = maze->getCell( i,  j);
    float pixel_width = maze->getCellPixelWidth();
    QRectF rect = QRectF(cell->xworld-pixel_width/2, cell->zworld-pixel_width/2, pixel_width/2,pixel_width/2);
    red_rectangle->setRect(rect);
}

Windows::Windows(MazeGen*    mmaze)
    :maze(mmaze)
{
    CopyMesh("Windows");
    CenterModel();
    Declare();
    trans_time = 1;
    t0 = Timer::gtimer.fsec;
    update();
    my_cell = maze->getCell( rand()%maze->getCellsX(),  rand()%maze->getCellsY());
    my_new_cell =  my_cell;
    ei.coord.x = my_cell->xworld;
    ei.coord.z = my_cell->zworld;
    isCanRender = true;
    model_matrix.setToIdentity();
    model_matrix.translate(my_cell->xworld, 0, my_cell->zworld);
    DoTransition();
}

void Windows::DoTransition()
{
    bool bSelected = false;
    while(bSelected == false )
    {
        int i = rand()%4;
        if( i==0 && !my_new_cell->topWall->bVisible  )
        {
            Cell* cell = maze->getCell( my_new_cell->i, my_new_cell->j - 1 );
            if(!cell->isNull())
            {
                my_cell = my_new_cell;
                my_new_cell = cell;
                bSelected = true;
            }
        }
        if( i==1 && !my_new_cell->bottomWall->bVisible  )
        {
            Cell* cell = maze->getCell( my_new_cell->i, my_new_cell->j + 1 );
            if(!cell->isNull())
            {
                my_cell = my_new_cell;
                my_new_cell = cell;
                bSelected = true;
            }
        }
        if( i==2 && !my_new_cell->leftWall->bVisible  )
        {
            Cell* cell = maze->getCell( my_new_cell->i - 1, my_new_cell->j );
            if(!cell->isNull())
            {
                my_cell = my_new_cell;
                my_new_cell = cell;
                bSelected = true;
            }
        }

        if( i==3 && !my_new_cell->rightWall->bVisible  )
        {
            Cell* cell = maze->getCell( my_new_cell->i + 1, my_new_cell->j);
            if(!cell->isNull())
            {
                my_cell = my_new_cell;
                my_new_cell = cell;
                bSelected = true;
            }
        }
    }

    t0 = Timer::gtimer.fsec;

}

void Windows::Slot1()
{
    //prn("Update from win %d", id);

    //calc pos
    double t = (Timer::gtimer.fsec - t0) / trans_time;
    Pnt A(my_cell->xworld, 0, my_cell->zworld);
    Pnt B(my_new_cell->xworld, 0, my_new_cell->zworld);

    Pnt C = line_eq(A, B, t);

    model_matrix.setToIdentity();
    model_matrix.translate(C.x, 0, C.z);
    ei.coord = C;

    if( (Timer::gtimer.fsec - t0) > trans_time )
        DoTransition();

}

void Linuxoid::create_enemys(int N)
{
    for(int i=0; i < N; i++)
    {
        Windows* win = new Windows(maze);
        win->SetID(i);
        MREG.AddObjNew(win);
        enemies.push_back(win);
    }

}

void Linuxoid::reset_reachable()
{
    for(int i=0; i < maze->getCellsX(); i++)
        for(int j=0; j < maze->getCellsY(); j++)
        {
            Cell* cell = maze->getCell(i,j);
            cell->bReachable = false;
        }

}

void Linuxoid::delete_unreachable_coins()
{
    for(int i=0; i < maze->getCellsX(); i++)
        for(int j=0; j < maze->getCellsY(); j++)
        {
            Cell* cell = maze->getCell(i,j);
            if( !cell->bReachable && cell->value!=0)
            {
                cell->object->isCanRender = false;
                cell->value = 0;
                prn("deleted unrechable %d %d", cell->i, cell->j);
            }
        }

}

void Linuxoid::make_tree(TreeNode* root)
{
    my_cell = root->my_cell;
    my_cell->bReachable = true;
    if( !my_cell->topWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_cell->i, my_cell->j - 1 );
        if( !cell->bReachable )
        {
            root->AddChild(new TreeNode(cell));
        }
    }
    if( !my_cell->bottomWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_cell->i, my_cell->j + 1 );
        if( !cell->bReachable )
        {
            root->AddChild(new TreeNode(cell));
        }
    }
    if( !my_cell->leftWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_cell->i - 1, my_cell->j );
        if( !cell->bReachable )
        {
            root->AddChild(new TreeNode(cell));
        }
    }

    if( !my_cell->rightWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_cell->i + 1, my_cell->j);
        if( !cell->bReachable )
        {
            root->AddChild(new TreeNode(cell));
        }
    }

    for(int i=0; i < root->children.size(); i++)
        make_tree(root->children[i]);

}

void Linuxoid::click_lmb(int state)
{

}

void Linuxoid::key_pressed(int k)
{
    if( k==0xFF52 ) //up
    {
        if( !my_cell->topWall->bVisible )
        {
            Cell* cell = maze->getCell( my_cell->i, my_cell->j - 1 );
            linuxsprite->model_matrix.setToIdentity();
            linuxsprite->model_matrix.translate(cell->xworld, 10, cell->zworld);
            //qDebug() << linuxsprite->model_matrix(0,3); //x
            //qDebug() << linuxsprite->model_matrix(2,3);//z
            my_cell = cell;
        }
    }
    else if(k==0xFF54) //down
    {
        if( !my_cell->bottomWall->bVisible )
        {
            Cell* cell = maze->getCell( my_cell->i, my_cell->j + 1 );
            linuxsprite->model_matrix.setToIdentity();
            linuxsprite->model_matrix.translate(cell->xworld, 10, cell->zworld);
            my_cell = cell;
        }
    }else if(k==0xFF51) // left
    {
        if( !my_cell->leftWall->bVisible )
        {
            Cell* cell = maze->getCell( my_cell->i - 1, my_cell->j );
            linuxsprite->model_matrix.setToIdentity();
            linuxsprite->model_matrix.translate(cell->xworld, 10, cell->zworld);
            my_cell = cell;
        }

    }else if(k==0xFF53)
    {
        if( !my_cell->rightWall->bVisible )
        {
            Cell* cell = maze->getCell( my_cell->i + 1, my_cell->j);
            linuxsprite->model_matrix.setToIdentity();
            linuxsprite->model_matrix.translate(cell->xworld, 10, cell->zworld);
            my_cell = cell;
        }
    }
    process_cell(my_cell);
    //highlight_cell(my_cell->i, my_cell->j);
    prn("my_cell[%d %d] = %f %f ", my_cell->i, my_cell->j, my_cell->xworld, my_cell->zworld);

}

void Linuxoid::process_cell(Cell* cell)
{
    if(cell->value > 0)
    {
        score+=cell->value;
        cell->object->isCanRender = false;
        cell->value = 0;
    }
    print_actions_rewards(cell);
}

bool sprites_intersects(GemObject* s1, GemObject* s2)
{
    float x0 = s1->model_matrix(0,3);
    float z0 = s1->model_matrix(2,3);
    float x1 = s2->model_matrix(0,3);
    float z1 = s2->model_matrix(2,3);
    QRect r1(x0 - s1->bounds.x/2, z0 - s1->bounds.z/2, s1->bounds.x, s1->bounds.z);
    QRect r2(x1 - s2->bounds.x/2, z1 - s2->bounds.z/2, s2->bounds.x, s2->bounds.z);
    return r1.intersects(r2);
}

void Linuxoid::Kill()
{
    //draw explosion
    AnimSprite* sprite = (AnimSprite*) MREG.FindObj("explosion_sprite01");
    if(sprite)
    {
        sprite->model_matrix.setToIdentity();
        sprite->model_matrix.translate(my_cell->xworld, 5, my_cell->zworld);
        sprite->PlayAndHide();
    }

    //respawn
    Cell* cell = maze->getCell( rand()%maze->getCellsX(),  rand()%maze->getCellsY());
    my_cell = cell;
    linuxsprite->ei.coord.x = cell->xworld;
    linuxsprite->ei.coord.z = cell->zworld;
    linuxsprite->model_matrix.setToIdentity();
    linuxsprite->model_matrix.translate(cell->xworld, 0, cell->zworld);
    score = 0;
}

int  Linuxoid::CountCoins()
{
    int n=0;
    for(int i=0; i < maze->getCellsX(); i++)
        for(int j=0; j < maze->getCellsY(); j++)
        {
            if( maze->getCell(i,j)->value > 0 )
                n++;
        }
    return n;
}

void Linuxoid::CheckColision()
{
    for(int i=0; i < enemies.size(); i++)
    {
        if( sprites_intersects(enemies[i], linuxsprite) )
            Kill();
    }

}

void Linuxoid::Slot1()
{
    CheckColision();
    if( CountCoins() == 0 )
        prn("GAME OVER!!");
}

// идет вверх до упора в стену
int Linuxoid::goUp2Wall(Cell* cell, int steps)
{
    if( !cell->topWall->bVisible  )
    {
        cell = maze->getCell( cell->i, cell->j - 1 );
        if( cellHasEnemy(cell) )
            return -steps-1;
        return goUp2Wall(cell, steps + 1);
    }
    return steps;
}

// идет вниз до упора в стену
int Linuxoid::goDown2Wall(Cell* cell, int steps)
{
    if( !cell->bottomWall->bVisible )
    {
        cell = maze->getCell( cell->i, cell->j + 1 );
        if( cellHasEnemy(cell) )
            return -steps-1;
        return goDown2Wall(cell, steps + 1);
    }
    return steps;
}

// идет влево до упора в стену
int Linuxoid::goLeft2Wall(Cell* cell, int steps)
{
    if( !cell->leftWall->bVisible )
    {
        cell = maze->getCell( cell->i - 1, cell->j );
        if( cellHasEnemy(cell) )
            return -steps-1;
        return goLeft2Wall(cell, steps + 1);
    }
    return steps;
}

// идет влево до упора в стену
int Linuxoid::goRight2Wall(Cell* cell, int steps)
{
    if( !cell->rightWall->bVisible  )
    {
        cell = maze->getCell( cell->i + 1, cell->j );
        if( cellHasEnemy(cell) )
            return -steps-1;
        return goRight2Wall(cell, steps + 1);
    }
    return steps;
}

int Direction(Cell* cell, Cell* cell2)
{
    if((cell2->j - cell->j) == 1) return 0;
    if((cell2->j - cell->j) == -1) return 1;
    if((cell2->i - cell->i) == 1) return 2;
    if((cell2->i - cell->i) == -1) return 3;
    return 0;
}

void Linuxoid::print_actions_rewards(Cell* cell)
{
    vector<int> Rt; // награды за 4 действия
    //
    // награда считается как функция [1]
    // R = (-расстояние до врага + количество свободных ячеек во всех направлениях )

    // 1. Взять все соседние ячейки в массив neibours
    vector<Cell*> neibours;
    neibours = getNeibours(cell);

    // 2. Расчитать функцию вознаграждения по формуле [1] для каждой из neibours
    int R=0;
    int cnt=0;
    for(auto it=neibours.begin(); it!=neibours.end(); it++)
    {
        int steps=0;
        R = 0;
        Cell* cell = (*it);
        steps=goUp2Wall(cell,0);
        R+=steps+cell->value;
        steps=goDown2Wall(cell,0);
        R+=steps+cell->value;
        steps=goLeft2Wall(cell,0);
        R+=steps+cell->value;
        steps=goRight2Wall(cell,0);
        R+=steps+cell->value;
        Rt.push_back(R);
    }

    // 3. Напечатать полученные в пункте 2 значения R
    int i = std::max_element(Rt.begin(),Rt.end()) - Rt.begin();
    //prn("R: %d %d %d %d",Rt[0],Rt[1],Rt[2],Rt[3]);
    highlight_cell(neibours[i]->i, neibours[i]->j);
    switch( Direction(cell, neibours[i]) )
    {
        case 0:
          prn("go bottom");
        break;
        case 1:
         prn("go top");
        break;
        case 2:
          prn("go right");
        break;
        case 3:
          prn("go left");
        break;

    }
    prn("best choice");

}

vector<Cell*> Linuxoid::getNeibours(Cell* my_new_cell)
{
    vector<Cell*> out;
    if( !my_new_cell->topWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_new_cell->i, my_new_cell->j - 1 );
        out.push_back(cell);
    }
    if( !my_new_cell->bottomWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_new_cell->i, my_new_cell->j + 1 );
        out.push_back(cell);
    }
    if( !my_new_cell->leftWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_new_cell->i - 1, my_new_cell->j );
        out.push_back(cell);
    }

    if( !my_new_cell->rightWall->bVisible  )
    {
        Cell* cell = maze->getCell( my_new_cell->i + 1, my_new_cell->j);
        out.push_back(cell);
    }
    return out;
}

bool Linuxoid::cellHasEnemy(Cell* cell)
{
    vector<Windows*>::iterator it;
    for(auto it = enemies.begin(); it!=enemies.end(); it++)
    {
        //prn("enemy at %d %d",(*it)->my_cell->i ,(*it)->my_cell->j );

        if(( (*it)->my_cell->i == cell->i ) && ( (*it)->my_cell->j == cell->j) )
            return true;
        //if( ( (*it)->my_new_cell->i == cell->i) && ((*it)->my_new_cell->j == cell->j) )
        //    return true;


    }
    return false;
}
