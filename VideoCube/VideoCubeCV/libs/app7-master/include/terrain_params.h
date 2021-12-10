#pragma once

#include "vpnt.h"
#include "interp.h"


extern string Terrain_algos;

// параметры леса
enum  tree_types{ El, Dub, Bereza};
struct Forest_params
{
	float	   MaxTreeH;	// высота наибольшего дерева
	int		   NTrees;		// количество деревьев
	tree_types tree_set;	
}


// параметры ландшафта
;
struct Terrain_params
{
	Terrain_params()
	{
		forest_level = desert;
	}
	float square;			// общая полощадь
	float water_square;	   // затопленная площадь
	float water_level;	   // уровень затопления
	float forest_square;  // площадь насаждений
	enum  forest_levels{ desert, lesostep, deep_forest }
		forest_level;	  // 0 - пустыня 1 - лесостепь 2 - непроходимый лес  
	int   mountain_level; // 0 - равнина 1 - холмистый 2 - гористый 3 - сплошные горы
	int face_size;		// мин размер полигона (детализация)
	float Max;			// максимум всех пиков
	float Min;			// минимум всех впадин и озер
	float sp_step;      // шаг сплайна

	Forest_params  forest_params;	// параметры леса
	
};

struct TerrainDATA
{
	double A[1000000];
	double B[1000000];
	double C[1000000];
	double D[1000000];
	double R[1000000];

};


// геометрические параметры фронта
struct Front_params
{
	float MaxH;
	float MinH;
	float NPicks;
	enum  front_law{ gauss, quadrat, sin_waves } law;

};

// фронт вершин поверхности
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
	Front_params par; // геометрические параметры фронта
	float r; // расстояние от точки наблюдения
	float dx; // шаг полигонизации
	float dy; // шаг полигонизации
	GLfloat col[4];	// цвет фронта
	float dt;		// шаг сплайновой апроксимации (детализация фронта)
	float* vertices;
	float* texcoords;
	unsigned int*  indexes;
	int    vert_count;
	int    face_count;
		
	
};


// параметры поверхности рельефа
struct Surface_params
{
	vector<t_front> fronts;
};
