#ifndef NNWIDGET_H
#define NNWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <vector>
#include "netbp.h"

class NNWidget : public QWidget
{
    Q_OBJECT
public:
    Network         network;
    vector<QRect>   neurons_rects;

    explicit NNWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    Neuron* hitNeuron(QPoint pos);
    
    void drawNeurons(QPainter& painter, int neuron_size);
    void drawConnections(QPainter& painter, int neuron_size);
    void timerEvent(QTimerEvent *);
    QString getNeuronInfo(Neuron* neuron);
signals:
    void clearConsole();
    void print(QString s);
    
public slots:
    void saveNN();
    void loadNN();
    void SlotGenerate();
    
};

#endif // NNWIDGET_H
