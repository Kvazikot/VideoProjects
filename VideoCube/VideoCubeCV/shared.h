#ifndef SHARED_H
#define SHARED_H

#include <QImage>

class Shared
{
public:
    int count = 0;

    Shared();
    void ZeroHeader();
    int Init();
    int Update();
    void writeJson();
    void writeImage(QImage& image);

};

#endif // SHARED_H
