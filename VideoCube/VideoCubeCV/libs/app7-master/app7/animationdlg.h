#ifndef ANIMATIONDLG_H
#define ANIMATIONDLG_H

#include <QDialog>

namespace Ui {
class AnimationDlg;
}

class AnimationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AnimationDlg(QWidget *parent = 0);
    ~AnimationDlg();

private:
    Ui::AnimationDlg *ui;
public slots:
    void OnObjsLoad();

private slots:
    void on_horizontalSlider_sliderMoved(int position);
};

#endif // ANIMATIONDLG_H
