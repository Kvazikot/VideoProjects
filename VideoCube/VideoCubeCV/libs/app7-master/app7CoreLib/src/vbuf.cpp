/**********************************************************************
 *  Project App6
 *
 *  vbuf.cpp
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

#include "../include/vbuf.h"
#include "../include/glcode.h"
#include "../include/geometry.h"

void VertexBuffer::AddVertex(Pnt S)
{
	vertices[vert_count*3]=S.x;
	vertices[vert_count*3+1]=S.y;
	vertices[vert_count*3+2]=S.z;
	indexes[vert_count*3] = vert_count;
	indexes[vert_count*3+1] = vert_count+1;
	indexes[vert_count*3+2] = vert_count+2;
	vert_count++;
}


void VertexBuffer::Draw(float r, float g, float b, float a)
{
	if( vert_count == 0 ) return;

	glColor4f(r,g,b,a);

	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);	

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, vertices );

	glVertex3f(vertices[0],vertices[1],vertices[2]);
	glVertexPointer( 3, GL_FLOAT, 0, vertices );
	glEnable (GL_LINE_SMOOTH); 
	glDrawElements(GL_LINES, (vert_count-1)*3, GL_UNSIGNED_INT, indexes);
	glDisable (GL_LINE_SMOOTH); 

	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();


}
