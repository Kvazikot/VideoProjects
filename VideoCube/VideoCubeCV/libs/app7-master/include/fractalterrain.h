#ifndef FRACTALTERRAIN_H
#define FRACTALTERRAIN_H

class FractalTerrain
{
public:
    float roughness;
    int   detail;

    int size;
    int max;
    float* map;
    FractalTerrain(int detail);
    float get(int x, int y);
    void set(int x, int y, float val);
    void  divide(int size);
    float average(float v[4]);
    void  diamond(int x, int y, int size, float offset);
    void  square(int x, int y, int size, float offset);
    void  generate();
};

#endif // FRACTALTERRAIN_H
