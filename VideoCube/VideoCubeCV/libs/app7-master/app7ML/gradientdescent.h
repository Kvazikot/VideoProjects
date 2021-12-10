#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include <QVector>

// Класс реализует алгоритм градиентного спуска
class Hipotetic_fun
{
public:
    double operator()(double x, double k, double b)
    {
        return b + k*x;
    }

};
class GradientDescent
{
public:
    GradientDescent(double alpha, double theta0, double theta1);
    double alpha;
    double theta0;
    double theta1;
    void Fit(QVector<double>& xData, QVector<double>& yData);
};

#endif // GRADIENTDESCENT_H
