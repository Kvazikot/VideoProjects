/********************************************************************************
** Form generated from reading UI file 'trajectorydialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAJECTORYDIALOG_H
#define UI_TRAJECTORYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrajectoryDialog
{
public:
    QFrame *qwtframe2;
    QFrame *qwtframe3;
    QFrame *qwtframe4;
    QFrame *qwtframe1;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QPlainTextEdit *varEdit;
    QPlainTextEdit *console;
    QPushButton *pushButton;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *clearButton;
    QCheckBox *autoscale1;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QCheckBox *autoscale2;
    QLabel *label_10;
    QDoubleSpinBox *step;
    QLabel *label_8;
    QDoubleSpinBox *yminSB;
    QLabel *label_9;
    QDoubleSpinBox *ymaxSB;

    void setupUi(QDialog *TrajectoryDialog)
    {
        if (TrajectoryDialog->objectName().isEmpty())
            TrajectoryDialog->setObjectName(QString::fromUtf8("TrajectoryDialog"));
        TrajectoryDialog->resize(931, 672);
        qwtframe2 = new QFrame(TrajectoryDialog);
        qwtframe2->setObjectName(QString::fromUtf8("qwtframe2"));
        qwtframe2->setGeometry(QRect(464, 28, 448, 200));
        qwtframe2->setMinimumSize(QSize(0, 200));
        qwtframe2->setMaximumSize(QSize(16777215, 1000));
        qwtframe2->setFrameShape(QFrame::StyledPanel);
        qwtframe2->setFrameShadow(QFrame::Raised);
        qwtframe3 = new QFrame(TrajectoryDialog);
        qwtframe3->setObjectName(QString::fromUtf8("qwtframe3"));
        qwtframe3->setGeometry(QRect(9, 253, 449, 200));
        qwtframe3->setMinimumSize(QSize(0, 200));
        qwtframe3->setMaximumSize(QSize(16777215, 1000));
        qwtframe3->setFrameShape(QFrame::StyledPanel);
        qwtframe3->setFrameShadow(QFrame::Raised);
        qwtframe4 = new QFrame(TrajectoryDialog);
        qwtframe4->setObjectName(QString::fromUtf8("qwtframe4"));
        qwtframe4->setGeometry(QRect(464, 253, 448, 200));
        qwtframe4->setMinimumSize(QSize(0, 200));
        qwtframe4->setMaximumSize(QSize(16777215, 1000));
        qwtframe4->setFrameShape(QFrame::StyledPanel);
        qwtframe4->setFrameShadow(QFrame::Raised);
        qwtframe1 = new QFrame(TrajectoryDialog);
        qwtframe1->setObjectName(QString::fromUtf8("qwtframe1"));
        qwtframe1->setGeometry(QRect(9, 28, 449, 200));
        qwtframe1->setMinimumSize(QSize(0, 200));
        qwtframe1->setMaximumSize(QSize(16777215, 1000));
        qwtframe1->setFrameShape(QFrame::StyledPanel);
        qwtframe1->setFrameShadow(QFrame::Raised);
        label_2 = new QLabel(TrajectoryDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(464, 9, 54, 16));
        label = new QLabel(TrajectoryDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 48, 16));
        label_3 = new QLabel(TrajectoryDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(9, 234, 26, 16));
        label_4 = new QLabel(TrajectoryDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(464, 234, 64, 16));
        varEdit = new QPlainTextEdit(TrajectoryDialog);
        varEdit->setObjectName(QString::fromUtf8("varEdit"));
        varEdit->setGeometry(QRect(9, 459, 449, 142));
        console = new QPlainTextEdit(TrajectoryDialog);
        console->setObjectName(QString::fromUtf8("console"));
        console->setGeometry(QRect(464, 459, 448, 142));
        console->setMinimumSize(QSize(300, 0));
        pushButton = new QPushButton(TrajectoryDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(9, 607, 75, 23));
        label_5 = new QLabel(TrajectoryDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(820, 630, 60, 16));
        label_5->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(TrajectoryDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(760, 630, 60, 16));
        label_6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(TrajectoryDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(700, 630, 60, 16));
        label_7->setAutoFillBackground(false);
        label_7->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
        label_7->setScaledContents(false);
        label_7->setAlignment(Qt::AlignCenter);
        label_7->setWordWrap(false);
        clearButton = new QPushButton(TrajectoryDialog);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(89, 607, 75, 23));
        autoscale1 = new QCheckBox(TrajectoryDialog);
        autoscale1->setObjectName(QString::fromUtf8("autoscale1"));
        autoscale1->setGeometry(QRect(73, 7, 70, 17));
        layoutWidget = new QWidget(TrajectoryDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(493, 2, 415, 22));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        autoscale2 = new QCheckBox(layoutWidget);
        autoscale2->setObjectName(QString::fromUtf8("autoscale2"));

        horizontalLayout->addWidget(autoscale2);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout->addWidget(label_10);

        step = new QDoubleSpinBox(layoutWidget);
        step->setObjectName(QString::fromUtf8("step"));
        step->setMaximum(10000.989999999999782);

        horizontalLayout->addWidget(step);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_8);

        yminSB = new QDoubleSpinBox(layoutWidget);
        yminSB->setObjectName(QString::fromUtf8("yminSB"));
        yminSB->setMinimum(-100000.000000000000000);
        yminSB->setMaximum(100000.000000000000000);
        yminSB->setSingleStep(0.100000000000000);
        yminSB->setValue(-1.000000000000000);

        horizontalLayout->addWidget(yminSB);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setScaledContents(false);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_9);

        ymaxSB = new QDoubleSpinBox(layoutWidget);
        ymaxSB->setObjectName(QString::fromUtf8("ymaxSB"));
        ymaxSB->setMinimum(-100000.000000000000000);
        ymaxSB->setMaximum(100000.000000000000000);
        ymaxSB->setValue(1.000000000000000);

        horizontalLayout->addWidget(ymaxSB);


        retranslateUi(TrajectoryDialog);

        QMetaObject::connectSlotsByName(TrajectoryDialog);
    } // setupUi

    void retranslateUi(QDialog *TrajectoryDialog)
    {
        TrajectoryDialog->setWindowTitle(QApplication::translate("TrajectoryDialog", "TrajectoryDialog", nullptr));
        label_2->setText(QApplication::translate("TrajectoryDialog", "\320\243\321\201\320\272\320\276\321\200\320\265\320\275\320\270\321\217", nullptr));
        label->setText(QApplication::translate("TrajectoryDialog", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\320\270", nullptr));
        label_3->setText(QApplication::translate("TrajectoryDialog", "\320\243\320\263\320\273\321\213", nullptr));
        label_4->setText(QApplication::translate("TrajectoryDialog", "\320\232\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202\321\213", nullptr));
        varEdit->setPlainText(QApplication::translate("TrajectoryDialog", "Ntest=2\n"
"--\321\200\320\260\320\267\321\200\320\265\321\210\320\265\320\275\320\270\320\265 \320\264\320\273\321\217 \320\267\320\260\320\272\320\276\320\275\320\276\320\262 \321\201\320\272\320\276\321\200\320\276\321\201\321\202\320\265\320\271 \320\270 \321\203\321\201\320\272\320\276\321\200\320\265\320\275\320\270\320\271\n"
"N=500\n"
"--\320\222\320\276 \320\262\321\201\320\265\321\205 \321\202\320\265\321\201\321\202\320\260\321\205 \320\262\321\200\320\265\320\274\321\217 \320\263\320\265\320\275\320\265\321\200\320\260\321\206\320\270\320\270 10 \321\201\320\265\320\272\n"
"t_rascheta=20.0\n"
"--\320\247\320\260\321\201\321\202\320\276\321\202\320\260 \321\201\321\212\320\265\320\274\320\260 \320\264\320\260\320\275\320\275\321\213\321\205 \320\223\321\206\n"
"f_dat=20\n"
"-- \320\276\321\202\321\201\320\265\321\207\320\265\320\275\320\270\320\265\n"
"--xmin = 0 xmax = 200 split_level = 0\n"
"--\320\275\320\260\321\207 \320\277\320\276\320\267\320\270\321\206\320\270\321\217\n"
"pos0=0.0,0.1,0.3444"
                        "\n"
"\n"
"----------------\320\242\320\265\321\201\321\202 3. \320\222\320\265\321\200\321\202\320\270\320\272\320\260\320\273\321\214\320\275\321\213\320\271 \320\262\320\267\320\273\320\265\321\202 \320\270 \320\277\320\276\321\201\320\260\320\264\320\272\320\260.\n"
"//\320\222\320\265\321\200\321\202\320\270\320\272\320\260\320\273\321\214\320\275\320\260\321\217 \321\201\320\272\320\276\321\200\320\276\321\201\321\202\321\214 vy \321\200\320\260\321\201\321\202\320\265\321\202 \320\277\320\276 \321\215\320\272\321\201\320\277\320\276\320\275\320\265\320\275\321\202\320\265\n"
"//\320\264\320\276 5 \321\201\320\265\320\272. \320\267\320\260\321\202\320\265\320\274 \320\277\320\260\320\264\320\260\320\265\321\202 \321\202\320\260\320\272\320\266\320\265 \320\277\320\276 \321\215\320\272\321\201\320\277\320\276\320\275\320\265\320\275\321\202\320\265. \320\243\321\201\320\272\320\276\321\200\320\265\320\275\320\270\320\265 ay \320\277\320\276\321\201\321\202\320\276\321\217\320\275\320\275\320\276\n"
"AYlaw3"
                        "{type=SIN Amin=2 Amax=13 }\n"
"AXlaw3{ type=SIN  Amin=-45 Amax=45 T=1 phi0=90 }\n"
"VYlaw3{type=KUS_SPL PTS_XY=[0,0,2,5,3,0,5,0,7,0,8,-5,10,0] Res=12 }\n"
"Rolllaw3{ type=TREUG_NOISE Amin=0 Amax=90 phi0=90 Res=10}\n"
"Pitchlaw3{ type=TREUG_NOISE Amin=0 Amax=30 phi0=30 Res=10}\n"
"\n"
"----------------\320\242\320\265\321\201\321\202 4. \320\237\320\276\320\273\320\265\321\202 \320\267\320\274\320\265\320\271\320\272\320\276\320\271 \320\262 \320\277\320\273\320\276\321\201\320\272\320\276\321\201\321\202\320\270 XZ.\n"
"// \320\232\321\203\321\200\321\201 \320\270\320\267\320\274\320\265\320\275\321\217\320\265\321\202\321\201\321\217 \320\277\320\276 \320\263\320\260\321\200\320\274\320\276\320\275\320\270\321\207\320\265\321\201\320\272\320\276\320\274\321\203 \320\267\320\260\320\272\320\276\320\275\321\203 \321\201 \320\277\320\265\321\200\320\270\320\276\320\264\320\276\320\274 1 \321\201\320\265\320\272.\n"
"// \320\241\320\272\320\276\321\200\320\276\321\201\321\202\320\270 \320\277\320\276\321\201\321\202"
                        "\320\276\321\217\320\275\320\275\321\213. \320\243\321\201\320\272\320\276\321\200\320\265\320\275\320\270\321\217 \320\277\320\276\321\201\321\202\320\276\321\217\320\275\320\275\321\213\n"
"//Rolllaw4{ type=KUS_SPL PTS_XY=[0,0,0,2,4,8,6,8,8,2,10,0] Res=333     }\n"
"Pitchlaw4 { type=PAD_Y_AXIS }\n"
"Yawlaw4{ type=SIN  Amin=-45 Amax=45 T=1 phi0=90 }\n"
"//Rolllaw4{ type=SIN  Amin=-45 Amax=45 T=1 phi0=90 }\n"
"VZlaw4{ type=SIN  Amin=-2 Amax=2 T=1 }\n"
"VXlaw4{ type=SIN  Amin=-2 Amax=2 T=1 phi0=90}\n"
"\n"
"\n"
"----------------\320\242\320\265\321\201\321\202 1 - \320\263\320\270\320\277\320\265\321\200\320\261\320\276\320\273\320\270\321\207\320\265\321\201\320\272\320\270\320\271 \321\200\320\276\321\201\321\202 \320\264\320\273\321\217 \320\272\321\200\320\265\320\275\320\260 \320\276\321\202 0 - 90 \320\273\320\270\320\275\320\265\320\271\320\275\321\213\320\271 \321\200\320\276\321\201\321\202 \320\264\320\273\321\217 \321\202\320\260\320\275\320\263\320\260\320\266\320\260 \320\276\321\202 0 - 45\n"
"//R"
                        "olllaw1{ type=HOLM  Amin=0 Amax=90 tmax=5 Res=10}\n"
"Rolllaw1{ type=KUS_SPL PTS_XY=[0,0,0,2,4,8,6,8,8,2,10,0] Res=333     }\n"
"//Rolllaw1{ type=UP_QUAD  Amin=0 Amax=90 tmax=5 Res=12}\n"
"VYlaw1{type=TREUG_NOISE Amin=0 Amax=90 phi0=91 Res=10 }\n"
"VZlaw1{ type=SIN  Amin=-45 Amax=45 T=1 }\n"
"VXlaw1{ type=SIN  Amin=-45 Amax=45 T=1 phi0=90}\n"
"//AXlaw1{ type=TREUG_NOISE Amin=0 Amax=90 phi0=90 Res=10}\n"
"\n"
"\n"
"----------------\320\242\320\265\321\201\321\202 2. \320\223\320\260\321\200\320\274\320\276\320\275\320\270\321\207\320\265\321\201\320\272\320\276\320\265 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 \320\272\321\200\320\265\320\275\320\260 \320\270 \320\272\321\203\321\200\321\201\320\260 \320\276\321\202 -45 \320\264\320\276 45 \321\201 \320\277\320\265\321\200\320\270\320\276\320\264\320\276\320\274 1 \321\201\320\265\320\272.\n"
"Rolllaw2{type=SIN Amin=0 Amax=91 T=1 phi0=0 }\n"
"Yawlaw2{ type=SIN  Amin=-45 Amax=45 T=1 phi0=90}"
                        "\n"
"\n"
"\n"
"//----------------\320\242\320\265\321\201\321\202 5. \320\237\320\276\320\273\320\265\321\202 \320\277\320\276 \320\277\321\200\321\217\320\274\320\276\320\271 \320\270\320\267 \321\202\320\276\321\207\320\272\320\270 \320\220 \320\262 \321\202\320\276\321\207\320\272\321\203 B. \320\230\321\201\320\277\320\276\320\273\321\214\320\267\321\203\320\265\321\202\321\201\321\217 line_eq. \320\222\321\200\320\265\320\274\321\217 10 \321\201\320\265\320\272.\n"
"// \320\276\320\264\320\270\320\275 \320\267\320\260\320\272\320\276\320\275\n"
"law5{ type=LINEEQ A=0,0,0 B=10,10,10  }\n"
"\n"
"//\321\200\320\260\320\267\321\200\320\265\321\210\320\265\320\275\320\270\320\265 \320\272\321\203\321\200\321\201\320\260 \320\272\321\200\320\265\320\275\320\260 \321\202\320\260\320\275\320\263\320\260\320\266\320\260\n"
"ResolutionRoll=100\n"
"ResolutionYaw=100\n"
"ResolutionPitch=100 ", nullptr));
        pushButton->setText(QApplication::translate("TrajectoryDialog", "update", nullptr));
        label_5->setText(QApplication::translate("TrajectoryDialog", "<html><head/><body><p><span style=\" color:#ffffff;\">z</span></p></body></html>", nullptr));
        label_6->setText(QApplication::translate("TrajectoryDialog", "<html><head/><body><p><span style=\" color:#ffffff;\">x</span></p></body></html>", nullptr));
        label_7->setText(QApplication::translate("TrajectoryDialog", "<html><head/><body><p><span style=\" color:#ffffff;\">y</span></p></body></html>", nullptr));
        clearButton->setText(QApplication::translate("TrajectoryDialog", "clear", nullptr));
        autoscale1->setText(QApplication::translate("TrajectoryDialog", "Autoscale", nullptr));
        autoscale2->setText(QApplication::translate("TrajectoryDialog", "Autoscale", nullptr));
        label_10->setText(QApplication::translate("TrajectoryDialog", "Step", nullptr));
        label_8->setText(QApplication::translate("TrajectoryDialog", "Ymin", nullptr));
        label_9->setText(QApplication::translate("TrajectoryDialog", "Ymax", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrajectoryDialog: public Ui_TrajectoryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAJECTORYDIALOG_H
