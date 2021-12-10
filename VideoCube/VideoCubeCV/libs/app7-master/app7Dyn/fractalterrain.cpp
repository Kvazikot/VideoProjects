#include <math.h>
#include <stdlib.h>
#include "../include/fractalterrain.h"

FractalTerrain::FractalTerrain(int detail)
{
    //detail = 8;
    //roughness = 0.7;
    size = (int)pow((double)2, (double)detail) + 1;
    max = size - 1;
    map = (float*)malloc((size+1) * (size+1)*sizeof(float));
}



float FractalTerrain::get(int x, int y)
{
    if (x < 0 || x > max || y < 0 || y > max) return -1;
    return map[x + size * y];
}

void FractalTerrain::set(int x, int y, float val)
{
    map[x + size * y] = val;
}


// рекурсивный алгоритм построения фрактала
void FractalTerrain::divide(int size) {
    int x, y, half = size / 2;
    float scale = roughness * size;
    if (half < 1) return;


    for (y = half; y < max; y += size) {
        for (x = half; x < max; x += size) {
            square(x, y, half, (rand()/RAND_MAX) * scale * 2 - scale);
        }
    }
    for (y = 0; y <= max; y += half) {
        for (x = (y + half) % size; x <= max; x += size) {
            diamond(x, y, half, (rand()/RAND_MAX) * scale * 2 - scale);
        }
    }
    divide(size / 2);
}

float FractalTerrain::average(float v[4]) {
    int valid=0;
    for(int i=0; i<4; i++)
        if( v[i]!=-1 ) valid++;
    float total=0;
    for(int i=0; i<4; i++)
        total+=v[i];
    return total / (float)valid;
}

void FractalTerrain::square(int x, int y, int size, float offset) {
    float v[4];
    v[0] = get(x - size, y - size); // upper left
    v[1] = get(x + size, y - size); // upper right
    v[2] = get(x + size, y + size); // lower right
    v[3] = get(x - size, y + size); // lower left
    float ave = average(v);
    set(x, y, ave + offset);
}

void FractalTerrain::diamond(int x, int y, int size, float offset) {
    float v[4];
    v[0] = get(x, y - size); // top
    v[1] = get(x + size, y); // right
    v[2] = get(x, y + size); // bottom
    v[3] = get(x - size, y); // left
    float ave = average(v);
    set(x, y, ave + offset);
}


void FractalTerrain::generate()
{
    set(0, 0, (float)max);
    set(max, 0, (float)max / 2);
    set(max, max, 0);
    set(0, max, (float)max / 2);

    divide(max);
}


