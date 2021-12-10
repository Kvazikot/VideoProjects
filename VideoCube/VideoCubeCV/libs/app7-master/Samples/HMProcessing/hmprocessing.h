#ifndef HMPROCESSING_H
#define HMPROCESSING_H

#include <QString>
#include <QImage>

class HMProcessing
{
public:
    HMProcessing();
    unsigned int maxD;
    void process(QString fn, int porog);
    //результат в это же изображение
    void processHM(QImage& I, int porog);
    void amplify(QImage& I);

};



#endif // HMPROCESSING_H
