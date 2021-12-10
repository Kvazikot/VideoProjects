/********************************************************************************
** Form generated from reading UI file 'animationdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATIONDLG_H
#define UI_ANIMATIONDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_AnimationDlg
{
public:
    QSlider *horizontalSlider;
    QLabel *label;
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLCDNumber *frameNumber;

    void setupUi(QDialog *AnimationDlg)
    {
        if (AnimationDlg->objectName().isEmpty())
            AnimationDlg->setObjectName(QString::fromUtf8("AnimationDlg"));
        AnimationDlg->resize(417, 355);
        horizontalSlider = new QSlider(AnimationDlg);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(45, 30, 271, 19));
        horizontalSlider->setMinimum(0);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setSliderPosition(4);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setTickPosition(QSlider::TicksBelow);
        horizontalSlider->setTickInterval(33);
        label = new QLabel(AnimationDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 10, 291, 16));
        tableWidget = new QTableWidget(AnimationDlg);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(40, 70, 321, 201));
        pushButton = new QPushButton(AnimationDlg);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(40, 280, 75, 23));
        pushButton_2 = new QPushButton(AnimationDlg);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(130, 280, 75, 23));
        frameNumber = new QLCDNumber(AnimationDlg);
        frameNumber->setObjectName(QString::fromUtf8("frameNumber"));
        frameNumber->setGeometry(QRect(331, 25, 64, 23));
        frameNumber->setProperty("value", QVariant(1.000000000000000));

        retranslateUi(AnimationDlg);

        QMetaObject::connectSlotsByName(AnimationDlg);
    } // setupUi

    void retranslateUi(QDialog *AnimationDlg)
    {
        AnimationDlg->setWindowTitle(QApplication::translate("AnimationDlg", "Dialog", nullptr));
        label->setText(QApplication::translate("AnimationDlg", "Global Slider. PRESS HOME for Start  END for end selection", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AnimationDlg", "Clip", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AnimationDlg", "Time Interval", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("AnimationDlg", "Selection Set", nullptr));
        pushButton->setText(QApplication::translate("AnimationDlg", "New Clip", nullptr));
        pushButton_2->setText(QApplication::translate("AnimationDlg", "Delete Clip", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AnimationDlg: public Ui_AnimationDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATIONDLG_H
