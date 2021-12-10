#ifndef LEARNING_FUN_H
#define LEARNING_FUN_H

#include <QString>
#include "transfer_fun.h"

class LearningF
{
public:
    double dF(double x)
    {
    }
    double operator()(double x)
    {
        //float y = 2/(1+exp(-4.9*x))-1;
        float y = sin(x);
        return y;
    }
    QString toString()
    {
        return QString(" y = sin(x)");
    }

};

class learning_fun
{
public:
    learning_fun();
};

#endif // LEARNING_FUN_H
