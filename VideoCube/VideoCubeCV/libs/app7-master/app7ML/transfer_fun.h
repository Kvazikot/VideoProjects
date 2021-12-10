#ifndef TRANSFER_FUN_H
#define TRANSFER_FUN_H

#include <math.h>

enum Type_function {THRESHOLD, PICEWISE, SIGMOID, TANH};


class Transfer_fun
{
public:
    double a,b,c;
    Transfer_fun()
    {
        a = 1; b = 1; c = 1;
    }
    virtual double dF(double x)=0;
    virtual double operator()(double x)=0;
};

class Sigmoid : public Transfer_fun
{
public:
    double dF(double x)
    {
        float gf =  1. / (1 + exp(-x));
        return gf*(1-gf);
    }
    double operator()(double x)
    {
        return 1. / (1 + exp(-x));
    }

};

class HyperbolicTan : public Transfer_fun
{
    public:
    HyperbolicTan()
    {
        a=1.7159;
        b=2./3;
    }
    double dF(double x)
    {
        return (1 - tanh(x) * tanh(x)) * a * b;
    }
    double operator()(double x)
    {
        return a * tanh(b * x);
    }

};

class Picewise : public Transfer_fun
{
    public:
    double dF(double x)
    {
        if(x == 0.5) return 1;
        else return 0;
    }
    double operator()(double x)
    {
        if(x >= 0.5) return 1;
        else if( x < 0.5 && x > -0.5) return fabs(x);
        else if( x <= -0.5 ) return 0;
    }
};



class Threshold : public Transfer_fun
{  
    double operator()(double x)
    {
        return (x >=0 )?1:0;
    }
    double dF(double x)
    {
        //производной нет
        return 0;
    }

};





#endif // TRANSFER_FUN_H
