#ifndef TEST_H
#define TEST_H

#include <qwt_plot_curve.h>
#include "netbp.h"

class Test
{
public:
    Test(QwtPlotCurve* curves[16], Network* network);
    ~Test();
    void reset_curves();
    void test_freq_func();
    void test_sigmoid_func();
    void test_learn_func(Network* network, int maxEpoch);
    void test_approximate_fun(Network* network);
    void test_weight_distribution();
    void test_transfer_fun();
    void test_gradient_descent();
    double drawErrors(LearningStatistics& stat);
    double drawStatistic(LearningStatistics& stat);

    QwtPlotCurve* curves[16];
    QThread* nn_thread;
    double computeMEanError(QVector<double>& errors, QVector<double> xData);
};

#endif // TEST_H
