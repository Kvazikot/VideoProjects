#pragma once

#include "vpnt.h"
#include "interp.h"


extern string Terrain_algos;

// ��������� ����
enum  tree_types{ El, Dub, Bereza};
struct Forest_params
{
	float	   MaxTreeH;	// ������ ����������� ������
	int		   NTrees;		// ���������� ��������
	tree_types tree_set;	
}


// ��������� ���������
;
struct Terrain_params
{
	Terrain_params()
	{
		forest_level = desert;
	}
	float square;			// ����� ��������
	float water_square;	   // ����������� �������
	float water_level;	   // ������� ����������
	float forest_square;  // ������� ����������
	enum  forest_levels{ desert, lesostep, deep_forest }
		forest_level;	  // 0 - ������� 1 - ��������� 2 - ������������ ���  
	int   mountain_level; // 0 - ������� 1 - ��������� 2 - �������� 3 - �������� ����
	int face_size;		// ��� ������ �������� (�����������)
	float Max;			// �������� ���� �����
	float Min;			// ������� ���� ������ � ����
	float sp_step;      // ��� �������

	Forest_params  forest_params;	// ��������� ����
	
};

struct TerrainDATA
{
	double A[1000000];
	double B[1000000];
	double C[1000000];
	double D[1000000];
	double R[1000000];

};


// �������������� ��������� ������
struct Front_params
{
	float MaxH;
	float MinH;
	float NPicks;
	enum  front_law{ gauss, quadrat, sin_waves } law;

};

// ����� ������ �����������
struct t_front
{
	t_front()
	{
		r=0;
		vert_count = 10000;
		face_count = 10000;
		vertices = (float*)malloc((vert_count+1)*3*sizeof(float));
		indexes = (unsigned int*)malloc((face_count+1)*3*sizeof(int));
		texcoords = (float*)malloc((vert_count+1)*2*sizeof(float));
	}
	Front_params par; // �������������� ��������� ������
	float r; // ���������� �� ����� ����������
	float dx; // ��� �������������
	float dy; // ��� �������������
	GLfloat col[4];	// ���� ������
	float dt;		// ��� ���������� ������������ (����������� ������)
	float* vertices;
	float* texcoords;
	unsigned int*  indexes;
	int    vert_count;
	int    face_count;
		
	
};


// ��������� ����������� �������
struct Surface_params
{
	vector<t_front> fronts;
};
