/********************************************************************************
** Form generated from reading UI file 'terraindlg.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERRAINDLG_H
#define UI_TERRAINDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_TerrainDlg
{
public:
    QGridLayout *gridLayout;
    QSpinBox *widthSegs;
    QLabel *label_4;
    QDoubleSpinBox *SizeZ;
    QLabel *label_6;
    QPushButton *generateMaze;
    QLabel *label_9;
    QLabel *label_8;
    QDoubleSpinBox *SizeX;
    QDoubleSpinBox *Roughness;
    QLabel *label_3;
    QPushButton *createButton;
    QDoubleSpinBox *hconst;
    QPushButton *generateButton;
    QSpinBox *detail;
    QLabel *label_2;
    QSpinBox *LengthSegs;
    QLabel *label_5;
    QLabel *label_7;
    QDoubleSpinBox *SizeY;
    QLabel *label;
    QSpinBox *HeightSegs;
    QLabel *label_10;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QSpinBox *cellsX;
    QLabel *label_12;
    QLabel *label_11;
    QSpinBox *cellsY;
    QSpinBox *wall_width;
    QLabel *label_13;
    QPushButton *pushButton;

    void setupUi(QDialog *TerrainDlg)
    {
        if (TerrainDlg->objectName().isEmpty())
            TerrainDlg->setObjectName(QStringLiteral("TerrainDlg"));
        TerrainDlg->resize(212, 477);
        gridLayout = new QGridLayout(TerrainDlg);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widthSegs = new QSpinBox(TerrainDlg);
        widthSegs->setObjectName(QStringLiteral("widthSegs"));
        widthSegs->setMaximum(100000);
        widthSegs->setValue(50);
        widthSegs->setDisplayIntegerBase(10);

        gridLayout->addWidget(widthSegs, 1, 0, 1, 1);

        label_4 = new QLabel(TerrainDlg);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 1, 1, 1);

        SizeZ = new QDoubleSpinBox(TerrainDlg);
        SizeZ->setObjectName(QStringLiteral("SizeZ"));
        SizeZ->setMaximum(1e+06);
        SizeZ->setValue(100);

        gridLayout->addWidget(SizeZ, 6, 0, 1, 1);

        label_6 = new QLabel(TerrainDlg);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 6, 1, 1, 1);

        generateMaze = new QPushButton(TerrainDlg);
        generateMaze->setObjectName(QStringLiteral("generateMaze"));

        gridLayout->addWidget(generateMaze, 14, 0, 1, 3);

        label_9 = new QLabel(TerrainDlg);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 7, 1, 1, 1);

        label_8 = new QLabel(TerrainDlg);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 8, 1, 1, 1);

        SizeX = new QDoubleSpinBox(TerrainDlg);
        SizeX->setObjectName(QStringLiteral("SizeX"));
        SizeX->setMaximum(1e+06);
        SizeX->setValue(100);

        gridLayout->addWidget(SizeX, 4, 0, 1, 1);

        Roughness = new QDoubleSpinBox(TerrainDlg);
        Roughness->setObjectName(QStringLiteral("Roughness"));
        Roughness->setMaximum(9911.99);
        Roughness->setValue(0.7);

        gridLayout->addWidget(Roughness, 8, 0, 1, 1);

        label_3 = new QLabel(TerrainDlg);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        createButton = new QPushButton(TerrainDlg);
        createButton->setObjectName(QStringLiteral("createButton"));

        gridLayout->addWidget(createButton, 11, 0, 1, 3);

        hconst = new QDoubleSpinBox(TerrainDlg);
        hconst->setObjectName(QStringLiteral("hconst"));
        hconst->setMaximum(100000);

        gridLayout->addWidget(hconst, 9, 0, 1, 1);

        generateButton = new QPushButton(TerrainDlg);
        generateButton->setObjectName(QStringLiteral("generateButton"));

        gridLayout->addWidget(generateButton, 13, 0, 1, 3);

        detail = new QSpinBox(TerrainDlg);
        detail->setObjectName(QStringLiteral("detail"));
        detail->setMaximum(1111);
        detail->setValue(8);

        gridLayout->addWidget(detail, 7, 0, 1, 1);

        label_2 = new QLabel(TerrainDlg);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        LengthSegs = new QSpinBox(TerrainDlg);
        LengthSegs->setObjectName(QStringLiteral("LengthSegs"));
        LengthSegs->setMaximum(100000);
        LengthSegs->setValue(50);

        gridLayout->addWidget(LengthSegs, 3, 0, 1, 1);

        label_5 = new QLabel(TerrainDlg);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 5, 1, 1, 1);

        label_7 = new QLabel(TerrainDlg);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_7, 0, 0, 1, 4);

        SizeY = new QDoubleSpinBox(TerrainDlg);
        SizeY->setObjectName(QStringLiteral("SizeY"));
        SizeY->setMaximum(1e+06);
        SizeY->setValue(10);

        gridLayout->addWidget(SizeY, 5, 0, 1, 1);

        label = new QLabel(TerrainDlg);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);

        HeightSegs = new QSpinBox(TerrainDlg);
        HeightSegs->setObjectName(QStringLiteral("HeightSegs"));
        HeightSegs->setMaximum(100000);
        HeightSegs->setValue(2);

        gridLayout->addWidget(HeightSegs, 2, 0, 1, 1);

        label_10 = new QLabel(TerrainDlg);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 9, 1, 1, 1);

        groupBox = new QGroupBox(TerrainDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        cellsX = new QSpinBox(groupBox);
        cellsX->setObjectName(QStringLiteral("cellsX"));
        cellsX->setValue(8);

        gridLayout_2->addWidget(cellsX, 0, 0, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_2->addWidget(label_12, 1, 1, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 0, 1, 1, 1);

        cellsY = new QSpinBox(groupBox);
        cellsY->setObjectName(QStringLiteral("cellsY"));
        cellsY->setValue(8);

        gridLayout_2->addWidget(cellsY, 1, 0, 1, 1);

        wall_width = new QSpinBox(groupBox);
        wall_width->setObjectName(QStringLiteral("wall_width"));
        wall_width->setValue(14);

        gridLayout_2->addWidget(wall_width, 2, 0, 1, 1);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 2, 1, 1, 1);


        gridLayout->addWidget(groupBox, 10, 0, 1, 3);

        pushButton = new QPushButton(TerrainDlg);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 12, 0, 1, 3);


        retranslateUi(TerrainDlg);

        QMetaObject::connectSlotsByName(TerrainDlg);
    } // setupUi

    void retranslateUi(QDialog *TerrainDlg)
    {
        TerrainDlg->setWindowTitle(QApplication::translate("TerrainDlg", "Dialog", 0));
        label_4->setText(QApplication::translate("TerrainDlg", "Size X", 0));
        label_6->setText(QApplication::translate("TerrainDlg", "Size Z", 0));
        generateMaze->setText(QApplication::translate("TerrainDlg", "Generate Maze", 0));
        label_9->setText(QApplication::translate("TerrainDlg", "Detail", 0));
        label_8->setText(QApplication::translate("TerrainDlg", "Roughness", 0));
        label_3->setText(QApplication::translate("TerrainDlg", "Length segs ", 0));
        createButton->setText(QApplication::translate("TerrainDlg", "Create", 0));
        generateButton->setText(QApplication::translate("TerrainDlg", "Generate Fractal", 0));
        label_2->setText(QApplication::translate("TerrainDlg", "Height segs ", 0));
        label_5->setText(QApplication::translate("TerrainDlg", "Size Y", 0));
        label_7->setText(QApplication::translate("TerrainDlg", "MESH PROPERITIES", 0));
        label->setText(QApplication::translate("TerrainDlg", "Width segs ", 0));
        label_10->setText(QApplication::translate("TerrainDlg", "hconst", 0));
        groupBox->setTitle(QApplication::translate("TerrainDlg", "Maze", 0));
        label_12->setText(QApplication::translate("TerrainDlg", "cellY", 0));
        label_11->setText(QApplication::translate("TerrainDlg", "cellX", 0));
        label_13->setText(QApplication::translate("TerrainDlg", "wall width", 0));
        pushButton->setText(QApplication::translate("TerrainDlg", "Send to GPU", 0));
    } // retranslateUi

};

namespace Ui {
    class TerrainDlg: public Ui_TerrainDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINDLG_H
