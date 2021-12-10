#include <QApplication>
#include <time.h>
#include <algorithm>
#include <QDebug>
#include <QPainter>
#include <cmath>
#include "../include/genecurvefitter.h"
#include "../include/ploter.h"

GeneCurveFitter::GeneCurveFitter()
{
    a = 3;
    b = 4;
    c = 3;
    bStop = false;
    //double U = (double)rand()/RAND_MAX;
    //qDebug() << 10*U;
    tSum = 200;
    Ngenes = 3;
    data_points.push_back(QPointF(0,5));
    data_points.push_back(QPointF(-5,8));
    data_points.push_back(QPointF(5,8));
    //data_points.push_back(QPointF(5,8));
    //data_points.push_back(QPointF(-10,8));
    //data_points.push_back(QPointF(8,12));
    //data_points.push_back(QPointF(5,31));

    w.resize(data_points.size());
    tw.resize(data_points.size());

}

double parabola(double c[5], double x)
{
    return c[0] * x * x + c[1] * x + c[2];
}

double cantenary_line(double c[5], double x)
{
    if( c[0] == 0 ) return 0.000001;
    double res = c[0] * cosh((x+c[2])/c[0]) + c[1];
    if( res > 10000000 ) return 10000000;
    return res;
}

double equation(double c[5], double x)
{
    //Res = ax^2+bx+c;
    //return c[0] * x * x + c[1] * x + c[2];
    return parabola(c, x);
    //return cantenary_line(c, x);
}

double GetSumKoeff(double c[5])
{
    return 1./c[0] + 1./c[1] + 1./c[2] + 1./c[3] + 1./c[4];
}

void setin_population(int i,double a, double b, double c, double m, double n, double Population[][5])
{
    Population[i][0] = a;
    Population[i][1] = b;
    Population[i][2] = c;
    Population[i][3] = m;
    Population[i][4] = n;
}

void setin_population2(int i,double c[5], double Population[][5])
{
    Population[i][0] = c[0];
    Population[i][1] = c[1];
    Population[i][2] = c[2];
    Population[i][3] = c[3];
    Population[i][4] = c[4];
}


void get_from_population(int i, double Population[][5], double c[5])
{
    c[0] = Population[i][0];
    c[1] = Population[i][1];
    c[2] = Population[i][2];
    c[3] = Population[i][3];
    c[4] = Population[i][4];
}

void GeneCurveFitter::crossoverN(int n, double p1[], double p2[], double c[])
{
    int board = rand()%(n-1);
    bool orr = rand()%101 < 50;

    if( orr )
    {
        for(int i=0; i < board; i++) c[i] = p1[i];
        for(int i=board; i < n; i++) c[i] = p2[i];
    }
    else
    {
        for(int i=0; i < board; i++) c[i] = p2[i];
        for(int i=board; i < n; i++) c[i] = p1[i];
    }
    //мутации
    //int max = tSum;
    for(int i=0; i<n; i++)
      if (rand() % 101 < 5) c[i] = tSum * ((float)rand()/RAND_MAX);

}

void GeneCurveFitter::crossover(double p1[5], double p2[5], double c[5])
{
   int board = rand()%4;
   bool orr = rand()%101 < 50;

   switch (board) {
   case 0:
       if( orr )
         { c[0] = p1[0]; c[1] = p2[1]; c[2] = p2[2]; c[3] = p2[3]; c[4] = p2[4]; }
       else
         { c[0] = p2[0]; c[1] = p1[1]; c[2] = p1[2]; c[3] = p1[3]; c[4] = p1[4]; }
       break;
   case 1:
       if( orr )
          { c[0] = p1[0]; c[1] = p1[1]; c[2] = p2[2]; c[3] = p2[3]; c[4] = p2[4]; }
       else
          { c[0] = p2[0]; c[1] = p2[1]; c[2] = p1[2]; c[3] = p1[3]; c[4] = p1[4]; }
       break;
   case 2:
       if( orr )
          { c[0] = p1[0]; c[1] = p1[1]; c[2] = p1[2]; c[3] = p2[3]; c[4] = p2[4]; }
       else
          { c[0] = p2[0]; c[1] = p2[1]; c[2] = p2[2]; c[3] = p1[3]; c[4] = p1[4]; }
       break;
   case 3:
       if( orr )
          { c[0] = p1[0]; c[1] = p1[1]; c[2] = p1[2]; c[3] = p1[3]; c[4] = p2[4]; }
       else
          { c[0] = p2[0]; c[1] = p2[1]; c[2] = p2[2]; c[3] = p2[3]; c[4] = p1[4]; }
       break;
   }
   //мутации
   int max = tSum;
   for(int i=0; i<5; i++)
     if (rand() % 101 < 5)
     {
         c[i] = tSum*(float)(rand()/RAND_MAX) ;
     }
}

double getVectorDiff(std::vector<double>& w, std::vector<double>& tw)
{
    double diff=0;
    for(int i=0; i < w.size(); i++)
    {
        diff+=fabs((w[i] - tw[i])*(w[i] - tw[i]));
    }
    return diff;
}

// средн€€ приспособленность fitness потомков
double GeneCurveFitter::get_fitness(double Population[][5])
{
    double fitness=0;
    double c[5];
    double MultInv=0;
    for(int i=0; i < MAX_POPULATION; i++) fitnesss[i] = 0;

    for(int i=0; i < MAX_POPULATION; i++)
    {
        get_from_population(i, Population, c);
/*
        int n = rand()%data_points.size();
        float x = data_points[n].x();
        float y = data_points[n].y();
        float y0 = equation(c, x);
        fitnesss[i] = fabs((y-y0)*(y-y0));
*/

        for(int j=0; j < data_points.size(); j++)
        {
            float x = data_points[j].x();
            float y = data_points[j].y();
            float y0 = equation(c, x);
            fitnesss[i]+=fabs((y-y0));
            //qDebug("x=%f y=%f y0=%f fitnesss=%f",x,y,y0,fitnesss[i]);
        }

        MultInv+= 1/fitnesss[i];
        fitness+=fitnesss[i];
    }
    double last = 0;
    for(int i=0; i < MAX_POPULATION; i++)
      likehood[i] = last = last + ((1/(fitnesss[i]) / MultInv) * 100);
    fitness = fitness/MAX_POPULATION;
    return fitness;
}

int GeneCurveFitter::get_index(double val) {
    double last = 0;
    for(int i=0;i<MAX_POPULATION;i++) {
        if (last <= val && val <= likehood[i]) return i;
        else last = likehood[i];
    }
    return 3;
}

double GeneCurveFitter::Average(double Population[][5])
{
    double avg=0;
    for(int i=0; i < MAX_POPULATION; i++)
    {
        for(int j=0; j < Ngenes; j++)
            avg+=Population[i][j];
    }
    return avg/(MAX_POPULATION*Ngenes);

}


void PrintPopulation(double Population[][5], int Ngenes)
{
    QString s="Population={\n";
    for(int i=0; i < MAX_POPULATION; i++)
    {
        for(int j=0; j < Ngenes; j++)
            s+=QString::number(Population[i][j])+" ";
        s+="\n";
        qDebug()<<s;
        s="";
    }
}

bool genes_equal(double p1[5], double p2[5])
{
    for(int i=0; i < 5; i++)
        if(p1[i]!=p2[i]) return false;
    return true;
}

void GeneCurveFitter::Solve()
{
    Ploter* plot = Ploter::getInstance();
    // коэф лежат на отрезке [0..72]
    srand((unsigned)time(NULL));

    //генерируем первую попул€цию

    for(int i=0; i < MAX_POPULATION; i++)
    {
        int max = tSum;
        a = rand()%max;
        b = rand()%max;
        c = rand()%max;
        m = rand()%max;
        n = rand()%max;
        fitnesss[i] = 0;
        likehood[i] = 0;
        setin_population(i,a,b,c,m,n,Population);
    }
    //PrintPopulation(Population);

    double fitness=20;
    double next_fitness=20;
    int generation=0;
    while(fitness > 2)
    {
        generation++;
        //рассто€ние до tSum = коэфф. выживаемости
        double c[5];

        QApplication::processEvents();

        fitness = get_fitness(Population);
        if( fitness != fitness  )
          QApplication::processEvents();

        double avg = Average(Population);
        qDebug("gen %d fitness %lf tSum=%f Average=%f", generation, fitness, tSum, avg);

        // выбираем 5 пар родителей у них будет по одному ребенку
        double p1[5];
        double p2[5];

        for(int i=0; i < MAX_POPULATION; i++)
        {
            int i1=0,i2=0,iterations=0;

            while(i1 == i2 || genes_equal(p1, p2) )
            {
                i1 = get_index((double)(rand() % 101));
                i2 = get_index((double)(rand() % 101));
                get_from_population(i1, Population, p1);
                get_from_population(i2, Population, p2);
                if (++iterations > 25) break;
            }
            //crossover(p1,p2,c);
            crossoverN(Ngenes,p1,p2,c);
            /*
            qDebug("crossover p1=%f,%f,%f,%f,%f p2=%f,%f,%f,%f,%f c=%f,%f,%f,%f,%f",
                   p1[0],p1[1],p1[2],p1[3],p1[4],
                    p2[0],p2[1],p2[2],p2[3],p2[4],
                    c[0],c[1],c[2],c[3],c[4]);
                    */
            setin_population2(i, c, NextPopulation);
        }

        // уровень мутагенеза

        if( fitness > get_fitness(NextPopulation) )
        {
            tSum = 2 * Average(Population)+1;
        }




        plot->setXY(0,generation,fitness);

        //копирование попул€ции
        for(int i=0; i < MAX_POPULATION; i++)
        {
            for(int j=0; j < Ngenes; j++)
            {
                Population[i][j] = NextPopulation[i][j];
            }
        }
        //PrintPopulation(Population, Ngenes);

        if( bStop ) break;

    }
    qDebug() << "all done!";
    PrintPopulation(Population, Ngenes);
    double* min_fitness = std::min_element(&fitnesss[0], &fitnesss[MAX_POPULATION]);
    int index = (int)(min_fitness - &fitnesss[0]);
    a = Population[index][0];
    b = Population[index][1];
    c = Population[index][2];
    m = Population[index][3];
    n = Population[index][4];
    DrawGraphic();

}

void GeneCurveFitter::DrawGraphic()
{
    Ploter* plot = Ploter::getInstance();
    double x=0;
    //for(int j=0; j < 5; j++)
    int j=0;
    {
        plot->clearvals(j);
        double cc[5];
        //a = Population[rand()%MAX_POPULATION][0];
        //b = Population[rand()%MAX_POPULATION][1];
        //c = Population[rand()%MAX_POPULATION][2];
        cc[0] = a; cc[1] = b; cc[2] = c;
        for(x=-10; x < 10; x+=0.1)
        {
            plot->setXY(j,x,equation(cc,x));
        }
        for(int i=0; i < data_points.size(); i++)
          plot->setXY(10,data_points[i].x(),data_points[i].y());
    }
}


