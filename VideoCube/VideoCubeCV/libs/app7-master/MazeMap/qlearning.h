#ifndef QLEARNING_H
#define QLEARNING_H

#include <map>
#include <vector>

struct State
{
    int id;     // номер состояния
    int value;  // данные в этом состоянии
    bool  bVisited;
    State()
    {
        id = -1;
        value = 10e4;
    }
};

struct Action
{
    Action(int id, float p)
        : p(p), id(id)
    {
    }

    int   id; // номер действия
    float p; // вероятность, что действие a в состоянии s во
             // время t перейдёт в состояние s' ко времени t + 1

};


class QLearning
{
public:
    std::map<std::tuple<int, int, int>, Action* > state_table;
                      // s1  a    s2    a
    std::map<std::tuple<int,int>, float> Q;
    std::map<std::tuple<int,int>, float> Q_original;
    std::map<std::tuple<int,int>,int>       freqTable;
                       //s  a      Q
    std::vector<State*> m_allstates;
    std::vector<std::vector<int> > m_state_graph;

    const int max_actions=4;

    float LF; //это фактор обучения. Чем он выше, тем сильнее агент доверяет новой информации.
    float DF; // это фактор дисконтирования. Чем он меньше, тем меньше агент задумывается о выгоде от будущих своих действий.
    float RK; // масштабный коэф. награды


    QLearning();    
    void BuildTables(std::vector<State*>& allstates, int cellsX, int cellsY );
    void SetStateGrapth(std::vector<std::vector<int> >&  state_graph);
    void Learning(int maxIterations, int maxValue, int n_states);
    int ARGMAX(int s, int Max_actions);
    float MAXQ(int s, int Max_actions);
    State* getStateByID(int id);
    bool hasTransition(int id_from, int id_to,int n_action);
    bool isNeibours(int id1, int id2, int cellsY);
    float getActionProbability(int id1, int id2, int n_action);
    int getState(int s, int a);
    float getReward(int s, int maxValue);
    int getFreq(int s, int a);
    float alpha(float f);
    void initQ(int n_states);
    void initStates();
};

#endif // QLEARNING_H
