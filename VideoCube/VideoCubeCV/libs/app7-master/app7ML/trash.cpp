#include "trash.h"

#if 0

//State* getStateByID(std::vector<State*>& allstates, int id)
//{
//    return *(std::find(allstates.begin(), allstates.end(),
//              [&](State* s) -> bool
//              {
//                  return (s->id == id);
//              });)
//}

bool QLearning::hasTransition(int id_from, int id_to,int n_action=0)
{
    return state_table.find(std::make_tuple(id_from,id_to,n_action))!=state_table.end();
}

bool QLearning::isNeibours(int id1, int id2, int cellsY)
{
    int j1 = id1 / cellsY;
    int i1 = id1 % cellsY;
    int j2 = id2 / cellsY;
    int i2 = id2 % cellsY;
    int d1 = abs(i1 - i2);
    int d2 = abs(j1 - j2);
    if(d1 > 1) return false;
    if(d2 > 1) return false;
    return  d1 ^ d2 ;
}

float QLearning::getActionProbability(int id1, int id2, int n_action)
{
    if( hasTransition(id1, id2, n_action) )
    {
        Action* a = state_table[std::make_tuple(id1, id2, n_action)];
        return a->p;
    }
    else return 10000000;

}


//void QLearning::BuildTables(std::vector<State*>& allstates, int cellsX, int cellsY)
//{
//    int n_posible_trans = 0;

//    m_allstates = allstates;

//    //декартово произведение состояний
//    for(int i=0; i < allstates.size(); i++)
//    for(int j=0; j < allstates.size(); j++)
//    {
//        if( i==j ) continue;
//        State* s1 = allstates[i];
//        State* s2 = allstates[j];
//        //if( hasTransition(s2->id, s1->id) )
//        //    continue;
//        if( hasTransition(s1->id, s2->id) )
//            continue;

//        // вероятность перехода в соседнюю клетку
//        float probability = (float)rand()/RAND_MAX;
//        if( !isNeibours(s1->id, s2->id, cellsY) )
//        {
//            probability = 10; // невозможный переход
//            continue;
//        }
//        else
//        {
//            qDebug() << s1->id << " - " << s2->id << " ok!";
//            n_posible_trans++;
//        }

//        //вверх
//        Action* a1 = new Action(0, probability);
//        state_table[std::make_tuple(s1->id, s2->id, 0)] = a1;

//        //вниз
//        Action* a2 = new Action(1, probability);
//        state_table[std::make_tuple(s1->id, s2->id, 1)] = a2;

//        //вправо
//        Action* a3 = new Action(2, probability);
//        state_table[std::make_tuple(s1->id, s2->id, 2)] = a3;

//        //влево
//        Action* a4 = new Action(3, probability);
//        state_table[std::make_tuple(s1->id, s2->id, 3)] = a4;


//    }
//    qDebug() << "n_posible_trans=" << n_posible_trans;
//}

#endif
