#include <QPainter>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include "nnwidget.h"

NNWidget::NNWidget(QWidget *parent) :
    QWidget(parent)
{
    startTimer(100);
}

void NNWidget::drawNeurons(QPainter& painter, int neuron_size)
{
    if( network.m_layers.size() == 0 ) return;
    int max_n = *std::max_element(network.m_layers.begin(), network.m_layers.end());
    int w_gap =  (rect().width() - neuron_size * max_n ) / max_n;
    int h_gap = (rect().height() - neuron_size * network.m_layers.size()) / network.m_layers.size();

    int h_offset = rect().height() - 40;
    int n=0;

    painter.drawText(20,20,QString("current epoch = %1").arg(network.getCurrentEpoch()));
    painter.drawText(20,40,QString("max_epoch = %2").arg(network.getMaxEpoch()));

    neurons_rects.clear();
    for(uint l=0; l < network.m_layers.size(); l++ )
    {
        int w_offset =  (rect().width() - network.m_layers[l] * (neuron_size +  w_gap)) / 2;
        for(int i=0; i < network.m_layers[l]; i++)
        {
            QRect rc(w_offset, h_offset-neuron_size, neuron_size, neuron_size );

            neurons_rects.push_back(rc);
            painter.setPen(qRgb(200,200,200));
            if(l>0)
                painter.drawEllipse(rc);
            else
                painter.drawRect(rc);
            painter.setFont(QFont("Monospace", 8, QFont::Normal));

            rc.moveTop(rc.y()-neuron_size/4);
            painter.setPen(qRgb(0,100,0));
            painter.drawText( rc, Qt::AlignCenter,
                              QString::number(network.getNeuron(n)->id) );
            rc.moveTop(rc.y()+neuron_size/4);
            if( i%2 == 0 )
                rc.moveTop(rc.y()+neuron_size/4);
            painter.setPen(qRgb(0,0,0));
            painter.drawText( rc, Qt::AlignCenter,
                              QString::number(network.getNeuron(n)->a,'g',4) );
            painter.setPen(Qt::black);

            n++;
            w_offset+=w_gap+neuron_size;
        }
        h_offset-=h_gap;
    }
}

vector<int> parse_layers(QString layers_str)
{
    vector<int> layers;
    QStringList l = layers_str.split(" ");
    foreach(QString s, l)
    {
        layers.push_back(s.toInt());
    }
    return layers;
}

void NNWidget::saveNN()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                        tr("Save Neural Network"),"./nets", tr("(*.nn)"));

    if(fileName.isEmpty()) return;

    if( fileName.right(2)!="nn" )
        fileName+=".nn";

    QSettings settings(fileName, QSettings::IniFormat);
    settings.beginGroup("structure");
    QString layers_str;
    for(uint i=0; i < network.m_layers.size(); i++)
        layers_str+=QString::number(network.m_layers[i])+" ";
    layers_str=layers_str.left(layers_str.size()-1);
    settings.setValue("learning_rule","backprop");
    settings.setValue("layers", layers_str);
    settings.setValue("alpha", network.alpha);
    settings.setValue("current_epoch", network.getCurrentEpoch());
    settings.setValue("max_epoch", network.getMaxEpoch());
    settings.endGroup();

    settings.beginGroup("weights");
    for( auto wi = network.w.begin();  wi!=network.w.end(); wi++)
    {
        QString key = QString("%1_%2").arg((*wi).first.first).arg((*wi).first.second);
        settings.setValue(key, (*wi).second);
    }
    settings.endGroup();

}

void NNWidget::loadNN()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Load Neural Network"),"./nets", tr("(*.nn)"));

    if(fileName.isEmpty()) return;

    QSettings settings(fileName, QSettings::IniFormat);
    settings.beginGroup("structure");
    QString layers_str = settings.value("layers").toString();
    vector<int> layers = parse_layers(layers_str);
    network.setMaxEpoch(settings.value("max_epoch").toInt());
    network.setCurrentEpoch(settings.value("current_epoch").toInt());
    network.Clear();
    network.Generate(layers);
    settings.endGroup();
    settings.beginGroup("weights");
    QStringList keys = settings.childKeys();
    foreach(QString key, keys)
    {
        QStringList parts = key.split("_");
        network.w[make_pair(parts[0].toInt(), parts[1].toInt())] =
                settings.value(key).toDouble();
    }
    settings.endGroup();
}

void NNWidget::SlotGenerate()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Generate NNet"),
                                             tr("layers array:"), QLineEdit::Normal,
                                         "1 10 1", &ok);
   if (ok && !text.isEmpty())
   {
      network.Clear();
      network.Generate(parse_layers(text));
   }
}

void NNWidget::drawConnections(QPainter& painter, int neuron_size)
{
    for(uint c=0; c < network.connections.size(); c++)
    {
        Connect* con = network.connections[c];
        QPoint p1 = neurons_rects[con->in].center();
        QPoint p2 = neurons_rects[con->out].center();
        p1.setY(p1.y()-neuron_size/2);
        p2.setY(p2.y()+neuron_size/2);
        painter.setPen(qRgb(200,200,200));
        painter.drawLine(p1,p2);
    }
}

QString NNWidget::getNeuronInfo(Neuron* neuron)
{
    QString info;
    if(neuron)
    {
        info+="------incoming weights--------\n";
        for(uint i=0; i < neuron->incoming.size(); i++)
        {
            Connect* c = neuron->incoming[i];
            info+=QString("w[%1,%2]=%3").arg(c->in).arg(c->out).
                                         arg(network.w[make_pair(c->in,c->out)]);
            info+="\n";
        }
        info+="------outcoming weights--------\n";
        for(uint i=0; i < neuron->outcoming.size(); i++)
        {
            Connect* c = neuron->outcoming[i];
            info+=QString("w[%1,%2]=%3").arg(c->in).arg(c->out).
                                         arg(network.w[make_pair(c->in,c->out)]);
            info+="\n";
        }
        if( neuron->id < network.m_layers[0] )
            info+="type = Input \n";
        if( neuron->id >=network.getOutputsOffset() )
            info+="type = Output \n";
        if(info.size() == 0 )
            info+="type = Hidden \n";
        info+=QString("id = %1 \nvalue = %2 \ndelta = %3\n").arg(neuron->id).arg(neuron->a).arg(neuron->delta);
        info+=QString("incoming=%1 outcoming=%2 \n").arg(neuron->incoming.size()).arg(neuron->outcoming.size());

    }

    return info;
}

void NNWidget::mousePressEvent(QMouseEvent* event)
{
     if( event->button() == Qt::LeftButton )
     {
         Neuron* neuron = hitNeuron(event->pos());
         QString info = getNeuronInfo(neuron);
         clearConsole();
         print(info);
     }

     if( event->button() == Qt::RightButton )
     {
         bool ok;
         Neuron* neuron = hitNeuron(event->pos());
         if( neuron->id < network.m_layers[0] )
         {
             double d = QInputDialog::getDouble(this, tr("setValue for Neuron"),
                                                     tr("value:"), neuron->a, -1000000, 10000000, 1, &ok);
                  if (ok)
                  {
                      neuron->a = d;
                      network.evalute();
                  }
         }
     }
}

Neuron* NNWidget::hitNeuron(QPoint pos)
{
    for(uint i=0; i < neurons_rects.size(); i++)
    {
        if( neurons_rects[i].contains(pos) )
            return network.getNeuron(i);
    }
    return 0;
}

void NNWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(),Qt::white);

    drawNeurons(painter, 40);
    drawConnections(painter, 40);

}

void NNWidget::timerEvent(QTimerEvent *)
{
    repaint();
}
