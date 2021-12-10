/********************************************************************************
** Form generated from reading UI file 'terraindlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERRAINDLG_H
#define UI_TERRAINDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_TerrainDlg
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QDoubleSpinBox *SizeX;
    QSpinBox *HeightSegs;
    QLabel *label;
    QDoubleSpinBox *SizeY;
    QLabel *label_4;
    QLabel *label_3;
    QSpinBox *widthSegs;
    QSpinBox *LengthSegs;
    QLabel *label_2;
    QDoubleSpinBox *SizeZ;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_10;
    QDoubleSpinBox *hconst;
    QLabel *label_9;
    QSpinBox *detail;
    QDoubleSpinBox *Roughness;
    QLabel *label_8;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_13;
    QSpacerItem *verticalSpacer;
    QLabel *label_12;
    QSpinBox *cellsY;
    QLabel *label_11;
    QSpinBox *cellsX;
    QSpinBox *wall_height;
    QSpinBox *wall_width;
    QLabel *label_7;
    QSpinBox *ylevel;
    QLabel *label_14;
    QSpinBox *Ncoins;
    QLabel *label_15;
    QPushButton *generateMaze;
    QPushButton *genCoins;
    QPushButton *generateButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *createButton;
    QPushButton *clear;

    void setupUi(QDialog *TerrainDlg)
    {
        if (TerrainDlg->objectName().isEmpty())
            TerrainDlg->setObjectName(QString::fromUtf8("TerrainDlg"));
        TerrainDlg->resize(545, 316);
        gridLayout_2 = new QGridLayout(TerrainDlg);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox_2 = new QGroupBox(TerrainDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        SizeX = new QDoubleSpinBox(groupBox_2);
        SizeX->setObjectName(QString::fromUtf8("SizeX"));
        SizeX->setGeometry(QRect(14, 94, 160, 20));
        SizeX->setMaximum(1000000.000000000000000);
        SizeX->setValue(100.000000000000000);
        HeightSegs = new QSpinBox(groupBox_2);
        HeightSegs->setObjectName(QString::fromUtf8("HeightSegs"));
        HeightSegs->setGeometry(QRect(14, 42, 160, 20));
        HeightSegs->setMaximum(100000);
        HeightSegs->setValue(2);
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(180, 16, 161, 20));
        SizeY = new QDoubleSpinBox(groupBox_2);
        SizeY->setObjectName(QString::fromUtf8("SizeY"));
        SizeY->setGeometry(QRect(14, 120, 160, 20));
        SizeY->setMaximum(1000000.000000000000000);
        SizeY->setValue(100.000000000000000);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(180, 94, 161, 20));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(180, 68, 161, 20));
        widthSegs = new QSpinBox(groupBox_2);
        widthSegs->setObjectName(QString::fromUtf8("widthSegs"));
        widthSegs->setGeometry(QRect(14, 16, 160, 20));
        widthSegs->setMaximum(100000);
        widthSegs->setValue(50);
        widthSegs->setDisplayIntegerBase(10);
        LengthSegs = new QSpinBox(groupBox_2);
        LengthSegs->setObjectName(QString::fromUtf8("LengthSegs"));
        LengthSegs->setGeometry(QRect(14, 68, 160, 20));
        LengthSegs->setMaximum(100000);
        LengthSegs->setValue(50);
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(180, 42, 161, 20));
        SizeZ = new QDoubleSpinBox(groupBox_2);
        SizeZ->setObjectName(QString::fromUtf8("SizeZ"));
        SizeZ->setGeometry(QRect(14, 146, 160, 20));
        SizeZ->setMaximum(1000000.000000000000000);
        SizeZ->setValue(100.000000000000000);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(180, 120, 161, 20));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(180, 146, 161, 20));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(180, 220, 161, 20));
        hconst = new QDoubleSpinBox(groupBox_2);
        hconst->setObjectName(QString::fromUtf8("hconst"));
        hconst->setGeometry(QRect(14, 220, 160, 20));
        hconst->setMaximum(100000.000000000000000);
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(180, 168, 161, 20));
        detail = new QSpinBox(groupBox_2);
        detail->setObjectName(QString::fromUtf8("detail"));
        detail->setGeometry(QRect(14, 168, 160, 20));
        detail->setMaximum(1111);
        detail->setValue(8);
        Roughness = new QDoubleSpinBox(groupBox_2);
        Roughness->setObjectName(QString::fromUtf8("Roughness"));
        Roughness->setGeometry(QRect(14, 194, 160, 20));
        Roughness->setMaximum(9911.989999999999782);
        Roughness->setValue(0.700000000000000);
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(180, 194, 161, 20));

        horizontalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(TerrainDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 6, 0, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 1, 1, 1, 1);

        cellsY = new QSpinBox(groupBox);
        cellsY->setObjectName(QString::fromUtf8("cellsY"));
        cellsY->setValue(8);

        gridLayout->addWidget(cellsY, 1, 0, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 0, 1, 1, 1);

        cellsX = new QSpinBox(groupBox);
        cellsX->setObjectName(QString::fromUtf8("cellsX"));
        cellsX->setValue(8);

        gridLayout->addWidget(cellsX, 0, 0, 1, 1);

        wall_height = new QSpinBox(groupBox);
        wall_height->setObjectName(QString::fromUtf8("wall_height"));
        wall_height->setMaximum(1000);
        wall_height->setValue(10);

        gridLayout->addWidget(wall_height, 3, 0, 1, 1);

        wall_width = new QSpinBox(groupBox);
        wall_width->setObjectName(QString::fromUtf8("wall_width"));
        wall_width->setValue(3);

        gridLayout->addWidget(wall_width, 2, 0, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 3, 1, 1, 1);

        ylevel = new QSpinBox(groupBox);
        ylevel->setObjectName(QString::fromUtf8("ylevel"));
        ylevel->setMinimum(-1000);
        ylevel->setMaximum(10000);
        ylevel->setValue(-20);

        gridLayout->addWidget(ylevel, 4, 0, 1, 1);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 4, 1, 1, 1);

        Ncoins = new QSpinBox(groupBox);
        Ncoins->setObjectName(QString::fromUtf8("Ncoins"));
        Ncoins->setMaximum(1000);
        Ncoins->setValue(20);

        gridLayout->addWidget(Ncoins, 5, 0, 1, 1);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 5, 1, 1, 1);


        horizontalLayout->addWidget(groupBox);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 7);

        generateMaze = new QPushButton(TerrainDlg);
        generateMaze->setObjectName(QString::fromUtf8("generateMaze"));

        gridLayout_2->addWidget(generateMaze, 1, 3, 1, 1);

        genCoins = new QPushButton(TerrainDlg);
        genCoins->setObjectName(QString::fromUtf8("genCoins"));

        gridLayout_2->addWidget(genCoins, 1, 4, 1, 1);

        generateButton = new QPushButton(TerrainDlg);
        generateButton->setObjectName(QString::fromUtf8("generateButton"));

        gridLayout_2->addWidget(generateButton, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(180, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 6, 1, 1);

        createButton = new QPushButton(TerrainDlg);
        createButton->setObjectName(QString::fromUtf8("createButton"));

        gridLayout_2->addWidget(createButton, 1, 0, 1, 1);

        clear = new QPushButton(TerrainDlg);
        clear->setObjectName(QString::fromUtf8("clear"));

        gridLayout_2->addWidget(clear, 1, 5, 1, 1);


        retranslateUi(TerrainDlg);

        QMetaObject::connectSlotsByName(TerrainDlg);
    } // setupUi

    void retranslateUi(QDialog *TerrainDlg)
    {
        TerrainDlg->setWindowTitle(QApplication::translate("TerrainDlg", "Dialog", nullptr));
        groupBox_2->setTitle(QApplication::translate("TerrainDlg", "Mesh propities", nullptr));
        label->setText(QApplication::translate("TerrainDlg", "Width segs ", nullptr));
        label_4->setText(QApplication::translate("TerrainDlg", "Size X", nullptr));
        label_3->setText(QApplication::translate("TerrainDlg", "Length segs ", nullptr));
        label_2->setText(QApplication::translate("TerrainDlg", "Height segs ", nullptr));
        label_5->setText(QApplication::translate("TerrainDlg", "Size Y", nullptr));
        label_6->setText(QApplication::translate("TerrainDlg", "Size Z", nullptr));
        label_10->setText(QApplication::translate("TerrainDlg", "hconst", nullptr));
        label_9->setText(QApplication::translate("TerrainDlg", "Detail", nullptr));
        label_8->setText(QApplication::translate("TerrainDlg", "Roughness", nullptr));
        groupBox->setTitle(QApplication::translate("TerrainDlg", "Maze", nullptr));
        label_13->setText(QApplication::translate("TerrainDlg", "wall width", nullptr));
        label_12->setText(QApplication::translate("TerrainDlg", "cellY", nullptr));
        label_11->setText(QApplication::translate("TerrainDlg", "cellX", nullptr));
        label_7->setText(QApplication::translate("TerrainDlg", "wall height", nullptr));
        label_14->setText(QApplication::translate("TerrainDlg", "ylevel", nullptr));
        label_15->setText(QApplication::translate("TerrainDlg", "N coins", nullptr));
        generateMaze->setText(QApplication::translate("TerrainDlg", "Create Maze", nullptr));
        genCoins->setText(QApplication::translate("TerrainDlg", "Create Coins", nullptr));
        generateButton->setText(QApplication::translate("TerrainDlg", "Generate Fractal", nullptr));
        createButton->setText(QApplication::translate("TerrainDlg", "Create", nullptr));
        clear->setText(QApplication::translate("TerrainDlg", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TerrainDlg: public Ui_TerrainDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINDLG_H
