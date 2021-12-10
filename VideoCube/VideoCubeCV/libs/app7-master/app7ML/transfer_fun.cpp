#include "transfer_fun.h"

//================  передаточные функции ========================

//double sigmoid(double x, double a=2, double b=3, double c=4)
//{
//    //return 2/(1+exp(-4.9*x))-1;
//    //return 1. / (1 + exp(-0.5*x));
//    return 1. / (1 + exp(-x));
//}

//double hyperbolic_tan(double x, double a=1.7159, double b=2./3., double c=4)
//{
//    return a * tanh(b * x);
//}

//double picewise(double x, double a=2, double b=3, double c=4)
//{
//    if(x >= 0.5) return 1;
//    else if( x < 0.5 && x > -0.5) return fabs(x);
//    else if( x <= -0.5 ) return 0;
//}

//double threshold(double x, double a=2, double b=3, double c=4)
//{
//    return (x >=0 )?1:0;
//}

//double derivative_sigmoid(double x, double a=2, double b=3, double c=4)
//{
//    float gf = sigmoid(x);
//    return gf*(1-gf);
//    //float f = exp(-0.2*x);
//    //return  0.2*f/ ((1 + f) * (1 + f));
//}

//double derivative_tanh(double x, double a=1.7159, double b=2./3., double c=4)
//{
//    return (1 - tanh(x) * tanh(x)) * a * b;
//}

