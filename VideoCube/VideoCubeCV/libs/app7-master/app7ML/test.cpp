
#include "qwt.h"
#include <qwt_plot_curve.h>
#include <iostream>
#include "test.h"
#include "gradientdescent.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <QVector>
#include <QThread>
#include <QDebug>


Test::Test(QwtPlotCurve* curves[16], Network* network)
{
    for(int i=0; i < 16; i++)
        this->curves[i] = curves[i];

    //--------------------- PERFORM TESTS ---------------------------------------
    //test_freq_func();
    test_sigmoid_func();
    //test_weight_distribution();
    //test_nn_input(network);
    //test_transfer_fun();
    //test_gradient_descent();
}

void Test::reset_curves()
{
    for(int i=0; i < 16; i++)
        curves[i]->setSamples(QVector<double>(), QVector<double>());
}

Test::~Test()
{
    //stop thread
    //nn_thread->stopLearning();
    //usleep(100);
    //delete nn_thread;
}

void Test::test_weight_distribution()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0,0.5);
    reset_curves();
    test_sigmoid_func();
    QVector<double> xData;
    QVector<double> yData;
    for(float x=0; x < 1; x+=0.0001) {
        xData.push_back(d(gen));
        yData.push_back(x);
    }
    curves[1]->setStyle(QwtPlotCurve::Dots);
    curves[1]->setSamples(xData, yData);
    curves[1]->setPen(QPen(Qt::green));
}

void Test::test_sigmoid_func()
{
    QVector<double> xData;
    QVector<double> yData;
    QVector<double> zData;

    for(float x=-10; x < 10; x+=0.01)
    {
        //float y = 2/(1+exp(-4.9*x))-1;
        float y =  1. / (1 + exp(-x));
        float z =  sin(x);
        y = tanh(x);//1.7159 * tanh(2./3. * x);
        float dy = 1 - tanh(x) * tanh(x);
        //y = derivative_sigmoid(x);
        //y = cos(x) / x;
        xData.push_back(x);
        yData.push_back(y);
        zData.push_back(dy);
    }


    curves[0]->setStyle(QwtPlotCurve::Lines);
    curves[0]->setSamples(xData, yData);
    curves[1]->setStyle(QwtPlotCurve::Lines);
    curves[1]->setSamples(xData, zData);

}

void Test::test_freq_func()
{
    QVector<double> xData;
    QVector<double> yData;
    for(int f=1; f < 10000; f++)
    {
        xData.push_back(f);
        if(f==2000)
            f++;
        float y = exp(8000./(f+2000))/55.;
        yData.push_back(y);
    }
    curves[0]->setStyle(QwtPlotCurve::Lines);
    curves[0]->setSamples(xData, yData);
}

template<typename T>
void normilize_data(T& data)
{
  double Sr=0,mmin=111111111,mmax=-1111111111111;
  for(T::iterator it = data.begin(); it!=data.end(); it++)
  {
      Sr+=(*it);
      mmin = qMin(mmin, (*it));
      mmax = qMax(mmax, (*it));
      qDebug() << (*it);
  }
  Sr=Sr/data.size();

  qDebug() << "Sr = " << Sr;
  for(T::iterator it = data.begin(); it!=data.end(); it++)
  {
      (*it) = (*it)/mmax;
      //(*it) = mmax  + (*it)*(mmax)/ 2;
  }

}

void Test::test_learn_func(Network* network, int maxEpoch)
{
    // подготовка данных
    vector<double> input_data;
    vector<double> output_data;
    vector<ExamplesData> examples_data;
    for(float x=-M_PI; x <=  M_PI; x+=M_PI/180.)
    {
        input_data.push_back(x);
        input_data.push_back(1);
        output_data.push_back(sin(2*x)); //!!! нормировка среднее значение по всей выборке = 0
        //output_data.push_back(sin(x));
        examples_data.push_back(ExamplesData(input_data, output_data));
        input_data.clear();
        output_data.clear();
    }
    network->setData(examples_data, maxEpoch);    
    // обучение сети
    //network->BackPropLearning(examples_data, 10000);
    qDebug() << "main thread id - " << QThread::currentThreadId();
    nn_thread = new QThread;
    network->moveToThread(nn_thread);
    nn_thread->start();
    QMetaObject::invokeMethod(network, "doLearn", Qt::QueuedConnection);
}

void Test::test_approximate_fun(Network* network)
{
    QVector<double> xData;
    QVector<double> yData;
    QVector<double> zData;

    for(float x=-M_PI; x <= M_PI; x+=M_PI/180.)
    {
        //float y = 2/(1+exp(-4.9*x))-1;
        float y =  sin(2*   x);
        //float z = (float)rand()/RAND_MAX;
        network->setInput(0,x);
        network->setInput(1,1);
        network->evalute();
        float z = network->getOutput(0);
        xData.push_back(x);
        yData.push_back(y);
        zData.push_back(z);
    }

    curves[0]->setStyle(QwtPlotCurve::Lines);
    curves[0]->setSamples(xData, yData);
    //curves[0]->setSamples(QVector<double>(), QVector<double>());

    curves[1]->setPen(QPen(Qt::green));
    curves[1]->setStyle(QwtPlotCurve::Lines);
    curves[1]->setSamples(xData, zData);

    curves[2]->setSamples(QVector<double>(), QVector<double>());

}

double computeMeanError(QVector<double>& errors)
{
    double mean=0;
    for(int i=0; i < errors.size(); i++)
    {
        mean+=errors[i];
    }
    mean = mean / errors.size();
    return mean;
}

double Test::drawErrors(LearningStatistics& stat)
{
    for(int i=0; i < 16; i++)
        curves[i]->setSamples(QVector<double>(), QVector<double>());
    QVector<double> xData;
    for(int i=0; i < stat.epoches.size(); i++)
        xData.push_back(stat.epoches[i]);

    curves[2]->setPen(QPen(Qt::green));
    curves[2]->setStyle(QwtPlotCurve::Lines);
    curves[2]->setSamples(xData, stat.mean_error);
    return 0;
}

double Test::drawStatistic(LearningStatistics& stat)
{
    for(int i=0; i < 16; i++)
        curves[i]->setSamples(QVector<double>(), QVector<double>());

    QVector<double> xData;
    for(int i=0; i < stat.epoches.size(); i++)
        xData.push_back(stat.epoches[i]);

    curves[2]->setPen(QPen(Qt::green));
    curves[2]->setStyle(QwtPlotCurve::Lines);
    curves[2]->setSamples(xData, stat.weights_sko);
    return 0;
}

void Test::test_transfer_fun()
{
    Transfer_fun* g = new HyperbolicTan();
    qDebug() <<  "Test::test_transfer_fun()=======";
    qDebug() << "a=" <<  g->a;
    qDebug() << "b=" <<  g->b;
    qDebug() << "HyperbolicTan";
    for(float i=-5; i < 5; i++ )
    {
        qDebug() << (*g)(i);
    }
    delete g;
    g = new Sigmoid();
    qDebug() << "a=" <<  g->a;
    qDebug() << "b=" <<  g->b;
    qDebug() << "Sigmoid";
    for(float i=-5; i < 5; i++ )
    {
        qDebug() << (*g)(i);
    }

}


void Test::test_gradient_descent()
{
    qDebug() <<  "Test::test_gradient_descent()=======";
    for(int i=0; i < 16; i++)
        curves[i]->setSamples(QVector<double>(), QVector<double>());

    QVector<double> xData;
    QVector<double> yData;
    QVector<double> hData;
    Hipotetic_fun hf;
    for(double x=-100; x < 100; x+=1)
    {
        xData.push_back(x);
        hData.push_back( hf(x, -0.1, 900) );
        yData.push_back( hf(x, 2.1, 0) + ((float)rand()/RAND_MAX)*50 );
    }

    //применение алгоритма градиентного спуска для линейной регрессии
    GradientDescent algorithm(0.000001, 1, 1);
    qDebug() <<  "alpha = " << algorithm.alpha;
    qDebug() <<  "theta0 = " << algorithm.theta0;
    qDebug() <<  "theta1 = " << algorithm.theta1;
    algorithm.Fit(xData, yData);

    //данные
    curves[1]->setPen(QPen(Qt::black));
    curves[1]->setStyle(QwtPlotCurve::Dots);
    curves[1]->setSamples(xData, yData);

    // линия регрессии
    curves[2]->setPen(QPen(Qt::green));
    curves[2]->setStyle(QwtPlotCurve::Lines);
    curves[2]->setSamples(xData, hData);



}
