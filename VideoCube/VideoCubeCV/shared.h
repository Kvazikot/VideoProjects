#ifndef SHARED_H
#define SHARED_H

#include <QImage>

class Shared : public QObject
{
    Q_OBJECT
public:
    int count = 0;
    int png_datasize = 0;

    Shared();
    void ZeroHeader();
    int Init();
    int Update();
    void writeJson();
    void writeImage(QString filename, int w, int h);

public slots:
    void writeImage(int screen_num, QImage& image);

};

#endif // SHARED_H
