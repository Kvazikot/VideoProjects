#pragma once

#include "geometry.h"

class VertexBuffer
{
public:
	VertexBuffer()	{		vert_count = 0; 	};
	void SetSize(int n_vertex)	{ vert_count = n_vertex;	}
	void Clear(){ vert_count = 0; }
	void Draw(float r, float g, float b, float a);

	void		  AddVertex(Pnt S);
	float		  vertices[200000];
	unsigned int  indexes[200000];
	int			  vert_count;

};