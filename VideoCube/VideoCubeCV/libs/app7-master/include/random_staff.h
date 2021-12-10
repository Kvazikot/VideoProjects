#pragma once

#define FixL(v,L) (v<L?L:v)
#define FixH(v,H) (v>H?H:v)
#define FixV(v,H,L) FixH((FixL(v,H)),L)

float rand_range(float min, float max);
double triangular(double a, double b, double c); 
int rand_sign(float P);
double randf(float w);
double randLawByTable(std::map<double, double> probOfLvl);
double exponential(double lambda);
double normal3(double stddev, double mean);
void normal_distr(double y[], double stddev, double mean, int Ni);
