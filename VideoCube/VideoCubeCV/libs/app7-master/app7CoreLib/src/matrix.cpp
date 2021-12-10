/**********************************************************************
 *  Project App6
 *
 *  matrix.cpp
 *
 *  Author: Vladimir Baranov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
**********************************************************************/

#include "../include/matrix.h"
#include "../include/print.h"
#include <math.h>
// »—ѕќЋ№«”≈ћџ≈ ћј“–»÷џ 

//  ¬ OpenGL

//       | 0  4  8  12 |
//       | 1  5  9  13 |
// 1d=   | 2  6  10 14 |
//       | 3  7  11 15 |

//       | 00 01 02 03 |
// 2d  = | 10 11 12 13 |
//       | 20 21 22 23 |
//       | 30 31 32 33 |

//      ¬  Ќ»√ј’

//        | 0  1  2  3  |  
//        | 4  5  6  7  |  
//   M =  | 8  9  10 11 |  
//        | 12 13 14 15 |  

//	      | 00 10 20 30 |
//	 M =  | 01 11 21 31 |
//        | 02 12 22 32 |
//        | 03 13 23 33 |

//		   |  1  0       0       0 |
//	  Rx = |  0  cos(A) -sin(A)  0 |
//		   |  0  sin(A)  cos(A)  0 |
//		   |  0  0       0       1 |

//         |  cos(A)  0   sin(A)  0 |
//    Ry = |  0       1   0       0 |
//         |  -sin(A) 0   cos(A)  0 |
//         |  0       0   0       1 |

//         |  cos(A)  -sin(A)   0   0 |
//    Rz = |  sin(A)   cos(A)   0   0 |
//         |  0        0        1   0 |
//         |  0        0        0   1 |

// умножение матрицы на  матрицу
void mmult(int n, int m, double a[][3], double b[][3], double res[][3])
{
    int i, j, k;
    double d;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            d = 0.;
            for (k = 0; k < m; k++)
                d = d + a[i][k] * b[k][j];
            res[i][j] = d;
        }
    }
}

void mmult(int n, int m, double a[][4], double b[][4], double res[][4])
{
    int i, j, k;
    double d;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            d = 0.;
            for (k = 0; k < m; k++)
                d = d + a[i][k] * b[k][j];
            res[i][j] = d;
        }
    }
}

// умножение матрицы на  вектор 
void mvmult(int n, double b[3][3], float* a, float* res)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        res[i] = 0;
        for (j = 0; j < n; j++)
            res[i] = res[i] + b[i][j] * a[j];
    }
  
}

// умножение матрицы на  t_vector
void mvmultM(int n, double b[3][3], vector<Pnt>& a)
{
    int i, j, k;
	float res[3];
	float* aa;

    n=0;
	
    for (k = 0; k < (int)a.size(); k++)
	{
		aa = (float*)&a[k];
		for (i = 0; i < 3; i++)
		{
			res[i] = 0;
			for (j = 0; j < 3; j++)
				res[i] = res[i] + b[i][j] * aa[j];
		}
		aa[0] = res[0]; aa[1] = res[1]; aa[2] = res[2];
	}
  
}

// умножение матрицы на  вектор 
void mvmult(int n, double b[4][4], float* a, float* res)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        res[i] = 0;
        for (j = 0; j < n; j++)
            res[i] = res[i] + b[i][j] * a[j];
    }
  
}



//матрица направл€ющих косинусов
void umnk(double in_Psi, double in_Teta, double in_Gam, double b[3][3])
{
    b[0][0] = cos(in_Psi) * cos(in_Gam) - sin(in_Psi) * sin(in_Gam) * sin(in_Teta);
    b[0][1] = sin(in_Psi) * cos(in_Gam) + cos(in_Psi) * sin(in_Teta) * sin(in_Gam);
    b[0][2] = -cos(in_Teta) * sin(in_Gam);
    b[1][0] = -cos(in_Teta) * sin(in_Psi);
    b[1][1] = cos(in_Teta) * cos(in_Psi);
    b[1][2] = sin(in_Teta);
    b[2][0] = cos(in_Psi) * sin(in_Gam) + sin(in_Psi) * sin(in_Teta) * cos(in_Gam);
    b[2][1] = sin(in_Psi) * sin(in_Gam) - cos(in_Psi) * sin(in_Teta) * cos(in_Gam);
    b[2][2] = cos(in_Teta) * cos(in_Gam);
}



inline void ClearMatrix(double b[3][3])
{
  unsigned char i,j; 
  for(i=0;i<3;i++)
  for(j=0;j<3;j++)
   if(i==j)  
     b[i][j] = 1;
   else b[i][j] = 0;	
}

inline void ClearMatrix(double b[4][4])
{
  unsigned char i,j; 
  for(i=0;i<4;i++)
  for(j=0;j<4;j++)
   if(i==j)  
     b[i][j] = 1;
   else b[i][j] = 0;	
}


void minvert(double a[3][3], double res[3][3])
{
  unsigned char i,j; 
  for(i=0;i<3;i++)
  for(j=0;j<3;j++)
     res[i][j] = a[j][i];
}

void minvert(double a[4][4], double res[4][4])
{
  unsigned char i,j; 
  for(i=0;i<4;i++)
  for(j=0;j<4;j++)
     res[i][j] = a[j][i];
}


//матрица направл€ющих косинусов вокруг Z
void mrotz(double in_Psi, double b[3][3])
{
    ClearMatrix(b);
	b[0][0] = cos(in_Psi);
    b[0][1] = -sin(in_Psi);
    b[1][0] = sin(in_Psi);
    b[1][1] = cos(in_Psi);
}

//матрица направл€ющих косинусов вокруг Z
void mrotzs(double sin_b, double cos_b, double b[3][3])
{
    ClearMatrix(b);
	b[0][0] = cos_b;
    b[0][1] = -sin_b;
    b[1][0] = sin_b;
    b[1][1] = cos_b;
}


//матрица направл€ющих косинусов вокруг X
void mrotxs(double sin_a, double cos_a, double b[3][3])
{
    ClearMatrix(b);
	b[1][1] = cos_a;
    b[1][2] = -sin_a;
    b[2][1] = sin_a;
    b[2][2] = cos_a;
}


//матрица направл€ющих косинусов вокруг Y
void mrotys(double sin_a, double cos_a,  double b[3][3])
{
    ClearMatrix(b);
	b[0][0] = cos_a;
    b[0][2] = sin_a;
    b[2][0] = -sin_a;
    b[2][2] = cos_a;
}

//матрица направл€ющих косинусов вокруг X
void mrotx(double in_Psi, double b[3][3])
{
    ClearMatrix(b);
	b[1][1] = cos(in_Psi);
    b[1][2] = -sin(in_Psi);
    b[2][1] = sin(in_Psi);
    b[2][2] = cos(in_Psi);
}


//матрица направл€ющих косинусов вокруг Y
void mroty(double in_Psi, double b[3][3])
{
    ClearMatrix(b);
	b[0][0] = cos(in_Psi);
    b[0][2] = sin(in_Psi);
    b[2][0] = -sin(in_Psi);
    b[2][2] = cos(in_Psi);
}

void mscale(Pnt& ScaleVec, double res[3][3])
{
  ClearMatrix(res);
  res[0][0] = ScaleVec.x;
  res[1][1] = ScaleVec.y;
  res[2][2] = ScaleVec.z;
}

void mtrans(Pnt& TranslateVec, double res[3][3])
{
	ClearMatrix(res);
	res[0][2] = TranslateVec.x;
	res[1][2] = TranslateVec.y;
	res[2][2] = TranslateVec.z;
}

void mtrans44(Pnt& TranslateVec, double res[4][4])
{
	ClearMatrix(res);
	res[0][3] = TranslateVec.x;
	res[1][3] = TranslateVec.y;
	res[2][3] = TranslateVec.z;
}

void mprn(string name, double m[4][4])
{
	prn("{ %s } = ",name.c_str());
	prn("| %0.4f  %0.4f  %0.4f  %0.4f |", m[0][0],m[1][0],m[2][0],m[3][0]);
	prn("| %0.4f  %0.4f  %0.4f  %0.4f |", m[0][1],m[1][1],m[2][1],m[3][1]);
	prn("| %0.4f  %0.4f  %0.4f  %0.4f |", m[0][2],m[1][2],m[2][2],m[3][2]);
	prn("| %0.4f  %0.4f  %0.4f  %0.4f |", m[0][3],m[1][3],m[2][3],m[3][3]);
}

void mprn(string name, double m[3][3])
{
	prn("| %s | = ",name.c_str());
	prn("| %0.4f  %0.4f  %0.4f |", m[0][0],m[1][0],m[2][0]);
	prn("| %0.4f  %0.4f  %0.4f |", m[0][1],m[1][1],m[2][1]);
	prn("| %0.4f  %0.4f  %0.4f |", m[0][2],m[1][2],m[2][2]);
}


// ћатрица поворота на три угла
void mabg(float a, float b, float g, double u0[3][3])
{
    u0[0][0] = cos(a) * cos(b);
    u0[1][0] = cos(a) * sin(b) * sin(g) - sin(a) * cos(g);
	u0[2][0] = cos(a) * sin(b) * cos(g) + sin(a) * sin(g);
	u0[0][1] = sin(a) * cos(b);
	u0[1][1] = sin(a) * sin(b) * sin(g) + cos(a) * cos(g);
	u0[2][1] = sin(a) * sin(b) * cos(g) - cos(a) * sin(g);
	u0[0][2] = -sin(b);
	u0[1][2] = cos(b) * sin(g);
	u0[2][2] = cos(b) * cos(g);
//	      | 00 10 20 30 |
//	 M =  | 01 11 21 31 |
//        | 02 12 22 32 |
//        | 03 13 23 33 |
}


void mtest()
{
    static double Mat[4][4],Tmp[4][4],Tb[4][4];
    static double mat[3][3],r[3][3];
    
  
    ClearMatrix(r);
    ClearMatrix(mat);
    ClearMatrix(Mat);
    ClearMatrix(Tb);
    
    
    mrotx(45, mat);
    mroty(45, mat);
    mrotz(45, mat);
    mabg(45,45,45,mat);
    mprn("mabg",mat);

    Pnt Pb = Pnt(2,2,2);
    mtrans44(Pb, Mat);
    mprn("Pb",Mat);
    
    mat3344(mat, Tmp);
    mmult(4,4,Tmp,Mat,Tb);
    mprn("Tb в базисе Ob",Tb);
    

}
