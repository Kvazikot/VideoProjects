#include <QThread>
#include <QDebug>
#include <QApplication>
#include "netbp.h"
#include <random>

//==============================================================


Network::Network()
{
    numInputs = 1;
    numOutputs = 1;
    vector<int> v_in;
    v_in.push_back(numInputs+1);
    v_in.push_back(4);
    v_in.push_back(4);
    v_in.push_back(numOutputs);
    Generate(v_in);
    alpha = 0.03;
    stopLearning = false;
    m_maxEpoch = 0;
    errorMeanThreshold = 0.00000001;
    m_current_epoch = 0;
}

void  Neuron::setFunction(Type_function type)
{
    typef = type;
    switch(type)
    {
        case PICEWISE:
            g = new Picewise();
        break;
        case THRESHOLD:
            g = new Threshold();
        break;
        case SIGMOID:
            g = new Sigmoid();
        break;
        case TANH:
            g = new HyperbolicTan();
        break;
    };
}

void Network::setInput(int id, double value)
{
    if( id < numInputs )
        getNeuron(id)->a = value;
}

void Network::setInput(vector<double> in)
{
    int id=0;
    for(auto it=in.begin(); it!=in.end(); it++)
    {
        if( id < numInputs )
            getNeuron(id++)->a = *(it);
    }
}

void Network::setOutput(vector<double> out)
{
    int id=getOutputsOffset();
    for(auto it=out.begin(); it!=out.end(); it++)
    {
        if( id < numOutputs )
            getNeuron(id++)->a = *(it);
    }
}

double Network::getOutput(int id)
{
    float a = getNeuron(getOutputsOffset() + id)->a;
    return a; //(1 + exp(-a));
}

void Network::ResetWeights()
{
    for( auto it1=w.begin(); it1!=w.end(); it1++ )
        (*it1).second = 1 - (float)2*rand()/RAND_MAX;

}

void Network::Clear()
{
    for(auto ni = neurons.begin(); ni!=neurons.end(); ni++  )
    {
        delete (*ni).second;
    }
    neurons.clear();
    connections.clear();
    m_layers.clear();
    w.clear();
    errors.clear();
    m_current_epoch = 0;
    m_maxEpoch = 0;
}



void Network::createConnections(vector<int> layers, double mean, double dispersion)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(mean,dispersion);
    w.clear();
    for(uint l=0; l < layers.size()-1; l++)
    {
        int n1 = getID(l) + layers[l];
        int n2 = getID(l+1) + layers[l+1];
        for(int i=getID(l); i < n1; i++)
            for(int j=getID(l+1); j < n2; j++)
            {
                if( w.find(make_pair(j,i)) == w.end() )
                if(i!=j)
                {
                    Neuron* in = getNeuron(i);
                    Neuron* out = getNeuron(j);
                    Connect* con = new Connect(i,j);
                    connections.push_back(con);
                    in->outcoming.push_back(con);
                    out->incoming.push_back(con);
                    w[make_pair(i,j)] =  d(gen);//1 - (float)2*rand()/RAND_MAX;

                }
            }
    }
}

void Network::Generate(vector<int> layers)
{
    m_layers = layers;
    //входные слои
    numInputs = layers[0];
    int id=0;

    for(int i=0; i < numInputs; i++)
        neurons[id++] = new Neuron(id);

    //скрытые слои
    for(uint l=1; l < layers.size()-1; l++)
    {
        for(int i=0; i < layers[l]; i++)
            neurons[id++] = new Neuron(id);
    }

    //выходной слой
    numOutputs = layers[layers.size()-1];
    for(int i=0; i < numOutputs; i++)
        neurons[id++] = new Neuron(id);


    //создать соединения
    // параметры; mean dispersion весов
    createConnections(layers,0,0.54);

}

int Network::getID(int layer_index)
{
    int os=0;
    for(int i=0; i < layer_index; i++)
        os+=m_layers[i];
    return os;
}

Neuron* Network::getNeuron(int id)
{
    if( neurons.find(id)!=neurons.end() )
        return neurons[id];
    else
       return &null_neuron;

}

int Network::getOutputsOffset()
{
    int os=0;
    for(uint i=0; i < m_layers.size()-1; i++)
    {
        os+=m_layers[i];
    }
    return os;
}


double Network::evaluteSum(int id, int b)
{
    Neuron* neuron = getNeuron(id);
    double sum = 0;
    for(auto cit = neuron->incoming.begin(); cit!=neuron->incoming.end(); cit++)
    {
        sum+=getNeuron((*cit)->in)->a * w[std::make_pair((*cit)->in, (*cit)->out)];
    }
    return sum + b;
}

double Network::evaluteDeltaSum(int id)
{
    Neuron* neuron = getNeuron(id);
    double sum = 0;
    for(auto cit = neuron->outcoming.begin(); cit!=neuron->outcoming.end(); cit++)
    {
        sum+=getNeuron((*cit)->out)->delta * w[std::make_pair((*cit)->in, (*cit)->out)];
    }
    return sum;
}

//выполянется в отдельном от гуи потоке
void Network::doLearn()
{
        qDebug() << "executing thread id - " << QThread::currentThreadId();
        BackPropLearning(m_data, m_maxEpoch);
        //PerceptronLearning(m_data, m_maxEpoch);
}

void Network::stopLearn()
{
    stopLearning = true;
}

// вычислить значение на выходе
void Network::evalute()
{
    // прямое вычисление сети
    for( uint l=1; l  < m_layers.size(); l++ )
    {
        // цикл по нейронам l слоя
        for(int id=getID(l); id < getID(l)+m_layers[l]; id++)
        {
            Neuron* neuron = getNeuron(id);
            double sum = evaluteSum(id);
            Transfer_fun* f = neuron->g;
            neuron->a = (*f)(sum);
            //qDebug() << "neuron->a=" << neuron->a;
        }
    }

//    for(int id=getOutputsOffset(); id < getOutputsOffset()+m_layers[m_layers.size()-1]; id++)
//    {
//        Neuron* neuron = getNeuron(id);
//        double sum = evaluteSum(id);
//        neuron->a = sum;
//        //qDebug() << "neuron->a=" << neuron->a;
//    }


}

std::tuple<bool,double>  Network::nextEpoch(map<std::pair<int,int>, double >& w1, map<std::pair<int,int>, double >& w2, double eps)
{
    double sum=0;
    for( auto it1=w1.begin(), it2=w2.begin(); it1!=w1.end(); it1++, it2++ )
        sum+=( (*it1).second - (*it2).second ) * ( (*it1).second - (*it2).second );

    sum/=w1.size();
    //qDebug() << sum;

    if(sum < eps)
        return std::make_tuple(true,sum);
    else
        return std::make_tuple(true,sum);


}

double computeMeanError(QVector<double>& errors);

// последовательный режим обучения
void Network::ForwardCalc()
{
    for( uint l=1; l  < m_layers.size(); l++ )
    {
        // цикл по нейронам l слоя
        for(int id=getID(l); id < getID(l)+m_layers[l]; id++)
        {
            Neuron* neuron = getNeuron(id);
            double sum = evaluteSum(id);
            Transfer_fun* f = neuron->g;
            neuron->a = (*f)(sum);
            //qDebug() << "neuron->a=" << neuron->a;
        }
    }
}

void Network::ComputeErrorInOutputs(ExamplesData& ed)
{
    int cnt=0;
    for(int id=getOutputsOffset(); id < getOutputsOffset()+m_layers[m_layers.size()-1]; id++)
    {
        Neuron* neuron = getNeuron(id);
        double sum = evaluteSum(id);
        Transfer_fun* f = neuron->g;
        neuron->delta = f->dF(sum) * ( ed.out[cnt++] - neuron->a );

        errors.append(neuron->delta*neuron->delta);

        //qDebug() << "neuron->delta=" << neuron->delta;
    }
}

void Network::shuffle_data(vector<ExamplesData>& data)
{
    std::random_device rd;
    std::mt19937 rf(rd());
    std::shuffle(data.begin(), data.end(), rf);
}

void Network::propagateError()
{
    for( int l =  m_layers.size()-2; l >= 0; l--)
    {
        // цикл по нейронам l слоя
        for(int j=getID(l); j < getID(l)+m_layers[l]; j++)
        {
            Neuron* neuron_j = getNeuron(j);
            double sum = evaluteSum(j);
            double dsum = evaluteDeltaSum(j);
            Transfer_fun* f = neuron_j->g;
            neuron_j->delta = f->dF(sum) * dsum;
            // цикл по нейронам l+1 слоя
            // обновление связей
            for(int i=getID(l+1); i < getID(l+1)+m_layers[l+1]; i++)
            {
                Neuron* neuron_i = getNeuron(i);
                w[make_pair(j,i)] = w[make_pair(j,i)] + alpha * neuron_j->a * neuron_i->delta;
                //print(QString("w[%1,%2]=%3").arg(j).arg(i).arg(w[make_pair(j,i)]));
                //print(QString("alpha * neuron_i->a * neuron_i->delta = %1").arg(alpha * neuron_i->a * neuron_i->delta));

            }

        }

    }
}

void Network::BackPropLearning(vector<ExamplesData>& data, int maxEpoch)
{
    //setInput(id++, x);  
    m_current_epoch = 0;
    errors.clear();
    stopLearning = false;
    map<std::pair<int,int>, double > w_prev;
    stat.clear();

    while( m_current_epoch++ < maxEpoch )
    {
            if( stopLearning ) break;
            w_prev = w;

            shuffle_data(data);

            for(auto it=data.begin(); it!=data.end(); it++)
            {
                qDebug() << "progress" << 1 - (float)(it - data.begin())/(float)data.size();

                if( stopLearning ) break;

                ExamplesData& ed = (*it);
                double prev_mean=10000000000000;
                errors.clear();

                while( 1 )
                {
                    if( stopLearning ) break;
                    // присвоение выходному слой входных данных
                    setOutput(ed.in);
                    setInput(ed.in);

                    // прямое вычисление сети
                    ForwardCalc();

                    // цикл по нейронам выходного слоя (вычисление ошибки)
                    ComputeErrorInOutputs(ed);

                    // распространение ошибки
                    propagateError();

                    //if( nextEpoch(w_prev,w) ) break;
                    double mean = computeMeanError(errors);
                    stat.mean_error.push_back(mean);
                    stat.epoches.push_back(m_current_epoch);
                    if( fabs( prev_mean - mean) < errorMeanThreshold ) break;
                    prev_mean = mean;
                    //qDebug() << fabs( prev_mean - mean);

                }
            }
            stat.weights_sko.push_back(std::get<1>(nextEpoch(w_prev,w)));
    }
    print("learning done!");
}

void Network::PerceptronLearning(vector<ExamplesData>& data, int maxEpoch)
{
    errors.clear();
    while( m_current_epoch++ < maxEpoch )
    {
        if( stopLearning ) break;


        for(auto it=data.begin(); it!=data.end(); it++)
        {
            ExamplesData& ed = (*it);
            setInput(ed.in);
            setOutput(ed.out);
            for(int i=getID(1); i < getID(1)+m_layers[1]; i++)
            {
                Neuron* neuron = getNeuron(i);
                double sum = evaluteSum(i);
                neuron->a = sum;
                Transfer_fun* f = neuron->g;
                neuron->delta = ed.out[0] - (*f)(neuron->a);
                //errors.push_back(neuron->delta);
                w[make_pair(0,i)] = w[make_pair(0,i)] + alpha * neuron->delta * f->dF(sum) * ed.in[0];
            }

        }
    }


}
