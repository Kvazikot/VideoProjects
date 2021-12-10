#include <QDebug>
#include <math.h>
#include "gradientdescent.h"

GradientDescent::GradientDescent(double alpha, double theta0, double theta1)
    :alpha(alpha),theta0(theta0),theta1(theta1)
{

}

void GradientDescent::Fit(QVector<double>& xData, QVector<double>& yData)
{
    Hipotetic_fun hf;
    double temp0=0,temp1=0,diff;
    int maxIterations=100000000;

    while(1)
    {
        double sum=0;
        for(int i=0; i < xData.count(); i++)
        {
            sum+=hf(xData[i], theta0, theta1) - yData[i];
        }
        temp0 = theta0 - alpha * (1. / (float)xData.count()) * sum;
        sum=0;
        for(int i=0; i < xData.count(); i++)
        {
            sum+=(hf(xData[i], theta0, theta1) - yData[i])*xData[i];
        }
        temp1 = theta1 - alpha * (1. / (float)xData.count()) * sum;
        diff = fabs(theta0-temp0) + fabs(theta1-temp1);
        theta0 = temp0;
        theta1 = temp1;
        if(diff < 0.0001)
            break;
        maxIterations--;
        if( maxIterations == 0 )
            break;
        qDebug() << "theta0=" << theta0;
        qDebug() << "theta1=" << theta1;
    }

}
