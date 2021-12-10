#include "qlearning.h"
#include <math.h>
#include <algorithm>
#include <tuple>
#include <QDebug>

using namespace std;

QLearning::QLearning()
{
    LF = 0.01;
    DF = 0.001;
    RK = 0.1;

}


void QLearning::SetStateGrapth(vector<vector<int> >&  state_graph)
{
    m_state_graph = state_graph;
}

void QLearning::initQ(int n_states)
{
    for(int s=0; s < n_states; s++)
    {
//        int n1 = rand()%25;
//        int n2 = rand()%25;
//        int n3 = rand()%25;
//        int n4 = rand()%25;
//        int n5 = (100 - (n1+n2+n3+n4))/4;
//        n1+=n5; n2+=n5; n3+=n5; n4+=n5;
        Q[make_tuple(s,0)] = 1 - 0.1*(float)rand()/RAND_MAX;
        Q[make_tuple(s,1)] = 1 - 0.1*(float)rand()/RAND_MAX;
        Q[make_tuple(s,2)] = 1 - 0.1*(float)rand()/RAND_MAX;
        Q[make_tuple(s,3)] = 1 - 0.1*(float)rand()/RAND_MAX;
    }
    Q_original = Q;
}



// выбирает действие с макс. полезностью
int QLearning::ARGMAX(int s,int maxIterations)
{
    int amax = 0;

    for(int a=0; a < max_actions; a++)
    {
        if( freqTable[make_tuple(s,a)] > maxIterations/(m_allstates.size()*4) )
        {
            return rand()%max_actions;
        }
        if( Q[make_tuple(s,a)] > Q[make_tuple(s,amax)] )
            amax = a;
    }
    return amax;
}

float QLearning::MAXQ(int s)
{
    float max = 0;
    for(int a=0; a < max_actions; a++)
        if( Q[make_tuple(s,a)] > max )
            max = Q[make_tuple(s,a)];
    return max;
}

void QLearning::Learning(int maxIterations, int maxValue, int n_states)
{
    int s0, a0, s, a;
    float r;

    initQ(n_states);
    for_each(m_allstates.begin(), m_allstates.end(),
                  [&](State* s) {(s->bVisited = false);
                                for(int a=0; a < max_actions; a++)
                                    freqTable[make_tuple(s->id,a)]=1;
                                  });

    // случ. начальное состо€ние
    s0 = rand()%(n_states);
    qDebug() << "start at " << s0;
    s = s0;
    a = 0;
    bool bBadAction=false;
    int iter = maxIterations;
    while( iter > 0)
    {
        a0 = a; s0 = s;
        //ѕолучить текущие состо€ние с сенсора
        s = getState(s0, a);
        freqTable[make_tuple(s0,a)]++;
        // ѕолучить вознаграждение за предыдущее действие
        if( bBadAction )
            r = 0;
        else
        {
            r = (float)(maxValue - m_allstates[s]->value) * RK;
            //r = m_allstates[s]->value * RK;
            if(r < 0) r = 0;
        }

        if(s0 == s) // если был переход в невозможное состо€ние
            bBadAction = true;
        else
            bBadAction = false;
        m_allstates[s]->bVisited = true;

        //Update (ќбновление ценности):
        //LF = alpha(freqTable[make_tuple(s0,a0)])
        Q[make_tuple(s0,a0)] = Q[make_tuple(s0,a0)] +  LF * (r + DF * MAXQ(s) - Q[make_tuple(s0,a0)]);

        a = ARGMAX(s,maxIterations); //Decision (¬ыбор действи€):

        iter--;
    }
    qDebug() << "learning done!";
}

int QLearning::getState(int s, int a)
{
    if( a < m_state_graph[s].size() )
    {
        if(m_state_graph[s][a] == -1)
            return s;
        else
            return m_state_graph[s][a];
    }
    else
       return s;
}

float QLearning::getReward(int s, int maxValue)
{
    return (float)(maxValue - m_allstates[s]->value) * RK;
}

float QLearning::alpha(float f)
{
    return exp(8000./(f+2000))/55. * LF;
}

int QLearning::getFreq(int s, int a)
{
    return freqTable[make_tuple(s,a)];
}
