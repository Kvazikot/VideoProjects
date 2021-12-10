#ifndef GENECURVEFITTER_H
#define GENECURVEFITTER_H

#include <QPainter>
#include <vector>

#define MAX_POPULATION 25


class GeneCurveFitter
{
public:
    double a,b,c,m,n;    // коэффициенты
    double s1,s2,s3,s4;
    double tSum;
    int  Ngenes;
    std::vector<QPointF>  data_points;
    std::vector<double> w;        // текущий вектор
    std::vector<double> tw;       //целевой вектор
    double Population[MAX_POPULATION+1][5];
    double NextPopulation[MAX_POPULATION+1][5];
    double likehood[MAX_POPULATION+1];
    double fitnesss[MAX_POPULATION+1];
    bool   bStop;

    GeneCurveFitter();
    void Solve();
    double get_fitness(double Population[][5]);
    int   get_index(double val);
    void  crossover(double p1[5], double p2[5], double c[5]);
    void  crossoverN(int n, double p1[], double p2[], double c[]);
    double Average(double Population[][5]);
    void  DrawGraphic();

};

#endif // GENECURVEFITTER_H
