#ifndef NETBP_H
#define NETBP_H

#include <vector>
#include <map>
#include <tuple>
#include <QPainter>
#include "transfer_fun.h"

using namespace std;

class Connect
{
public:
    int in;
    int out;
    Connect()
    {
        in = 0;
        out = 0;
    }
    Connect(int in, int out)
        :in(in),out(out)
    {}
};


class Neuron
{
public:
    Neuron()
    {
        a = -1;
        a = rand();
        delta = 0;
        setFunction(SIGMOID);
    }
    Neuron(int id)
        :id(id)
    {
        a = -1;
        a = rand();
        delta = 0;
        setFunction(SIGMOID);
    }
    Neuron(int id, double value, Type_function typef=SIGMOID)
        :id(id),a(value)
    {
        value = -1;
        value = rand();
        delta = 0;
        setFunction(typef);
    }
    void  setFunction(Type_function type);
    double a;
    double delta;
    int id;
    vector<Connect*> incoming;
    vector<Connect*> outcoming;
    Type_function     typef;
    Transfer_fun*     g;
};

struct LearningStatistics
{
    QVector<double> weights_sko;
    QVector<double> mean_error;
    QVector<double> epoches;
    void clear()
    {
        weights_sko.clear();
        mean_error.clear();
        epoches.clear();
    }

};

class ExamplesData
{
public:
    ExamplesData(vector<double>& in, vector<double>& out)
        :in(in), out(out)
    {
    }
    vector<double> in;
    vector<double> out;
};

class Network : public QObject
{
    Q_OBJECT
public:
    LearningStatistics  stat;
    map<int, Neuron*> neurons;
    vector<Connect*> connections;

    double alpha; // скорость обучения
    double errorMeanThreshold; // кпитерий остановки обучения
    int numInputs;
    int numOutputs;
    vector<int> m_layers;
    map<std::pair<int,int>, double > w; // матрица связей

    QVector<double> errors;

    Network();
    void Generate(vector<int> layers);
    void Draw(QPainter& painter);
    void ResetWeights();
    int   getOutputsOffset();
    Neuron* getNeuron(int id);
    int getID(int layer_index);
    void setInput(int id, double value);
    void setInput(vector<double> in);
    void setOutput(vector<double> out);
    double getOutput(int id);
    void evalute();
    double evaluteSum(int id, int b=0); //b - порог
    double evaluteDeltaSum(int id);
    int    getCurrentEpoch(){ return m_current_epoch; }
    int    getMaxEpoch(){ return m_maxEpoch; }
    void    setCurrentEpoch(int e) {  m_current_epoch = e; }
    void    setMaxEpoch(int e) {  m_maxEpoch = e; }
    void    setFunction(Type_function type);
    std::tuple<bool,double>  nextEpoch(map<std::pair<int,int>, double >& w1, map<std::pair<int,int>, double >& w2, double eps=10e-6);

    void createConnections(vector<int> layers, double mean, double dispersion);
    void ForwardCalc();
    void ComputeErrorInOutputs(ExamplesData& ed);
    void shuffle_data(vector<ExamplesData>& data);
    void propagateError();
public slots:
    void Clear();
    //void Evalute();
    void doLearn();
    void stopLearn();
    void BackPropLearning(vector<ExamplesData>& data, int maxEpoch);
    void PerceptronLearning(vector<ExamplesData>& data, int maxEpoch);
    void setData(vector<ExamplesData>& data, int maxEpoch=10000)
    {
        m_data = data;
        m_maxEpoch = maxEpoch;
    }

signals:
    void print(QString s);
    void clearConsole();

private:
   Neuron null_neuron;
   vector<ExamplesData> m_data;
   int m_maxEpoch;
   int m_current_epoch;
   bool stopLearning;
};




#endif // NETBP_H
