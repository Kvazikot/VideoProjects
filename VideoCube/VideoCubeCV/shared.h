#ifndef SHARED_H
#define SHARED_H

#include <QImage>

class Shared
{
public:
    int count = 0;
    int png_datasize = 0;

    Shared();
    void ZeroHeader();
    int Init();
    int Update();
    void writeJson();
    void writeImage(QImage& image);
    void writeImage(QString filename, int w, int h);

};

#endif // SHARED_H
