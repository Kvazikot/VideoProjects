#ifndef MATRIX_H
#define MATRIX_H
#include "geometry.h"
/** Fill out 3x3 matrix to 4x4 **/
#define mat_pad(A) (A[3][0]=A[0][3]=A[3][1]=A[1][3]=A[3][2]=A[2][3]=0,A[3][3]=1)

/** Copy nxn matrix A to C using "gets" for assignment **/
#define mat_copy(C,A,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
    C[i][j] = A[i][j]; }
#define mat3344(mat33, mat44) {mat_copy(mat44, mat33, 3); mat_pad(mat44); }


// умножение матрицы на  матрицу
void mmult(int n, int m, double a[][3], double bt[][3], double res[][3]);
void mmult(int n, int m, double a[][4], double bt[][4], double res[][4]);
void mvmultM(int n, double b[3][3], vector<Pnt>& a);

void minvert(double a[3][3], double res[3][3]);
void minvert(double a[4][4], double res[4][4]);
//void mvmult(int n, HMatrix b, double* a, double* res);
// умножение матрицы на  вектор
void mvmult(int n, double b[][3], float* a, float* res);
void mvmult(int n, double b[4][4], float* a, float* res);
void mprn(string name, double m[3][3]);
void mprn(string name, double m[4][4]);
inline void ClearMatrix(double b[3][3]);
inline void ClearMatrix(double b[4][4]);


void mrotz(double in_Psi, double b[3][3]);
void mrotx(double in_Psi, double b[3][3]);
void mroty(double in_Psi, double b[3][3]);

void mrotxs(double sin_b, double cos_b, double b[3][3]);
void mrotzs(double sin_b, double cos_b, double b[3][3]);
void mrotys(double sin_a, double cos_a,  double b[3][3]);

void mtrans(Pnt& TranslateVec, double res[3][3]);
void mtrans44(Pnt& TranslateVec, double res[4][4]);
void mscale(Pnt& ScaleVec, double res[4][4]);
//void mabg(float a, float b, float g, double u0[3][3]);


#endif