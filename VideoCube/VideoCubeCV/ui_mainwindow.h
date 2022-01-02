/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *action_2;
    QAction *action_3;
    QAction *action_4;
    QAction *action_youtube;
    QAction *action_brighteon;
    QAction *actionPlay_video;
    QAction *actionDelete_Video;
    QAction *actionDownload_all;
    QAction *actionSet_Temp_directory;
    QAction *action_5;
    QAction *action_6;
    QAction *action_7;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTextEdit *textEditor;
    QLabel *label;
    QLabel *label_5;
    QPushButton *findVideosButton;
    QTabWidget *tabWidget_2;
    QWidget *tab1;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPlainTextEdit *console;
    QWidget *tab_3;
    QListWidget *listWidget_3;
    QLabel *label_4;
    QLabel *label_6;
    QTableWidget *tableWidget_2;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QListWidget *urlsList;
    QListWidget *hashtagList;
    QLabel *label_3;
    QLabel *label_2;
    QProgressBar *progressBar;
    QWidget *widgetBrowser;
    QComboBox *comboBox;
    QSplitter *splitter;
    QPushButton *markKeywordsButton;
    QPushButton *playVideoButton;
    QPushButton *deleteVideoButton;
    QWidget *VFX;
    QPushButton *runPyButton;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menuUpload;
    QMenu *menu_URLS;
    QMenu *menu_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1315, 813);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        action_4 = new QAction(MainWindow);
        action_4->setObjectName(QString::fromUtf8("action_4"));
        action_youtube = new QAction(MainWindow);
        action_youtube->setObjectName(QString::fromUtf8("action_youtube"));
        action_brighteon = new QAction(MainWindow);
        action_brighteon->setObjectName(QString::fromUtf8("action_brighteon"));
        actionPlay_video = new QAction(MainWindow);
        actionPlay_video->setObjectName(QString::fromUtf8("actionPlay_video"));
        actionDelete_Video = new QAction(MainWindow);
        actionDelete_Video->setObjectName(QString::fromUtf8("actionDelete_Video"));
        actionDownload_all = new QAction(MainWindow);
        actionDownload_all->setObjectName(QString::fromUtf8("actionDownload_all"));
        actionSet_Temp_directory = new QAction(MainWindow);
        actionSet_Temp_directory->setObjectName(QString::fromUtf8("actionSet_Temp_directory"));
        action_5 = new QAction(MainWindow);
        action_5->setObjectName(QString::fromUtf8("action_5"));
        action_6 = new QAction(MainWindow);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        action_7 = new QAction(MainWindow);
        action_7->setObjectName(QString::fromUtf8("action_7"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        font.setKerning(false);
        tabWidget->setFont(font);
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(0, 0, 0, 0), stop:0.52 rgba(0, 0, 0, 0), stop:0.565 rgba(82, 121, 76, 33), stop:0.65 rgba(159, 235, 148, 64), stop:0.721925 rgba(255, 238, 150, 129), stop:0.77 rgba(255, 128, 128, 204), stop:0.89 rgba(191, 128, 255, 64), stop:1 rgba(0, 0, 0, 0));\n"
"color: rgb(0, 255, 0);"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        textEditor = new QTextEdit(tab);
        textEditor->setObjectName(QString::fromUtf8("textEditor"));
        textEditor->setGeometry(QRect(0, 80, 451, 331));
        QPalette palette;
        QBrush brush(QColor(0, 255, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        QBrush brush2(QColor(0, 255, 0, 128));
        brush2.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush3(QColor(0, 255, 0, 128));
        brush3.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        QBrush brush4(QColor(0, 255, 0, 128));
        brush4.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush4);
#endif
        textEditor->setPalette(palette);
        textEditor->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 201, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Magneto"));
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(9);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("color: rgb(60, 238, 33);\n"
"font: 75 12pt \"Magneto\";"));
        label->setFrameShape(QFrame::NoFrame);
        label->setFrameShadow(QFrame::Raised);
        label->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(590, 40, 421, 39));
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(60, 238, 33);\n"
"font: 75 12pt \"Magneto\";"));
        label_5->setAlignment(Qt::AlignCenter);
        findVideosButton = new QPushButton(tab);
        findVideosButton->setObjectName(QString::fromUtf8("findVideosButton"));
        findVideosButton->setGeometry(QRect(240, 5, 251, 61));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(findVideosButton->sizePolicy().hasHeightForWidth());
        findVideosButton->setSizePolicy(sizePolicy);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QLinearGradient gradient(0, 0, 1, 0);
        gradient.setSpread(QGradient::PadSpread);
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient.setColorAt(0, QColor(9, 41, 4, 255));
        gradient.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush5(gradient);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QLinearGradient gradient1(0, 0, 1, 0);
        gradient1.setSpread(QGradient::PadSpread);
        gradient1.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient1.setColorAt(0, QColor(9, 41, 4, 255));
        gradient1.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient1.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient1.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient1.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient1.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient1.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient1.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient1.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient1.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush6(gradient1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush6);
        QLinearGradient gradient2(0, 0, 1, 0);
        gradient2.setSpread(QGradient::PadSpread);
        gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient2.setColorAt(0, QColor(9, 41, 4, 255));
        gradient2.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient2.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient2.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient2.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient2.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient2.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient2.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient2.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient2.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush7(gradient2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush7);
        QBrush brush8(QColor(255, 0, 127, 128));
        brush8.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Active, QPalette::PlaceholderText, brush8);
#endif
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QLinearGradient gradient3(0, 0, 1, 0);
        gradient3.setSpread(QGradient::PadSpread);
        gradient3.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient3.setColorAt(0, QColor(9, 41, 4, 255));
        gradient3.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient3.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient3.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient3.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient3.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient3.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient3.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient3.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient3.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush9(gradient3);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        QLinearGradient gradient4(0, 0, 1, 0);
        gradient4.setSpread(QGradient::PadSpread);
        gradient4.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient4.setColorAt(0, QColor(9, 41, 4, 255));
        gradient4.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient4.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient4.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient4.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient4.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient4.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient4.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient4.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient4.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush10(gradient4);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush10);
        QLinearGradient gradient5(0, 0, 1, 0);
        gradient5.setSpread(QGradient::PadSpread);
        gradient5.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient5.setColorAt(0, QColor(9, 41, 4, 255));
        gradient5.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient5.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient5.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient5.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient5.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient5.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient5.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient5.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient5.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush11(gradient5);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush8);
#endif
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        QLinearGradient gradient6(0, 0, 1, 0);
        gradient6.setSpread(QGradient::PadSpread);
        gradient6.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient6.setColorAt(0, QColor(9, 41, 4, 255));
        gradient6.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient6.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient6.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient6.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient6.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient6.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient6.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient6.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient6.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush12(gradient6);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        QLinearGradient gradient7(0, 0, 1, 0);
        gradient7.setSpread(QGradient::PadSpread);
        gradient7.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient7.setColorAt(0, QColor(9, 41, 4, 255));
        gradient7.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient7.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient7.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient7.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient7.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient7.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient7.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient7.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient7.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush13(gradient7);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush13);
        QLinearGradient gradient8(0, 0, 1, 0);
        gradient8.setSpread(QGradient::PadSpread);
        gradient8.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient8.setColorAt(0, QColor(9, 41, 4, 255));
        gradient8.setColorAt(0.085, QColor(2, 79, 0, 255));
        gradient8.setColorAt(0.19, QColor(50, 147, 22, 255));
        gradient8.setColorAt(0.275, QColor(236, 191, 49, 255));
        gradient8.setColorAt(0.39, QColor(243, 61, 34, 255));
        gradient8.setColorAt(0.555, QColor(135, 81, 60, 255));
        gradient8.setColorAt(0.667, QColor(121, 75, 255, 255));
        gradient8.setColorAt(0.825, QColor(164, 255, 244, 255));
        gradient8.setColorAt(0.885, QColor(104, 222, 71, 255));
        gradient8.setColorAt(1, QColor(93, 128, 0, 255));
        QBrush brush14(gradient8);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush14);
        QBrush brush15(QColor(0, 0, 0, 128));
        brush15.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush15);
#endif
        findVideosButton->setPalette(palette1);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Magneto"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setItalic(false);
        font2.setUnderline(false);
        font2.setWeight(75);
        findVideosButton->setFont(font2);
        findVideosButton->setAutoFillBackground(false);
        findVideosButton->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(9, 41, 4, 255), stop:0.085 rgba(2, 79, 0, 255), stop:0.19 rgba(50, 147, 22, 255), stop:0.275 rgba(236, 191, 49, 255), stop:0.39 rgba(243, 61, 34, 255), stop:0.555 rgba(135, 81, 60, 255), stop:0.667 rgba(121, 75, 255, 255), stop:0.825 rgba(164, 255, 244, 255), stop:0.885 rgba(104, 222, 71, 255), stop:1 rgba(93, 128, 0, 255));"));
        findVideosButton->setCheckable(false);
        findVideosButton->setAutoDefault(false);
        findVideosButton->setFlat(false);
        tabWidget_2 = new QTabWidget(tab);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(10, 460, 428, 271));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        gridLayout_3 = new QGridLayout(tab1);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_2 = new QPushButton(tab1);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        lineEdit = new QLineEdit(tab1);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(tab1);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);

        console = new QPlainTextEdit(tab1);
        console->setObjectName(QString::fromUtf8("console"));
        console->setStyleSheet(QString::fromUtf8("background-color: rgb(203, 231, 255);\n"
"color: rgb(0, 0, 0);"));

        gridLayout_3->addWidget(console, 1, 0, 1, 1);

        tabWidget_2->addTab(tab1, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        listWidget_3 = new QListWidget(tab_3);
        listWidget_3->setObjectName(QString::fromUtf8("listWidget_3"));
        listWidget_3->setGeometry(QRect(30, 40, 291, 101));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 20, 91, 16));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(350, 20, 291, 20));
        tableWidget_2 = new QTableWidget(tab_3);
        if (tableWidget_2->columnCount() < 2)
            tableWidget_2->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget_2->setObjectName(QString::fromUtf8("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(350, 40, 281, 101));
        tabWidget_2->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        urlsList = new QListWidget(tab_2);
        new QListWidgetItem(urlsList);
        urlsList->setObjectName(QString::fromUtf8("urlsList"));
        urlsList->setMaximumSize(QSize(200, 16777215));
        urlsList->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);"));

        gridLayout_2->addWidget(urlsList, 1, 0, 2, 1);

        hashtagList = new QListWidget(tab_2);
        new QListWidgetItem(hashtagList);
        new QListWidgetItem(hashtagList);
        new QListWidgetItem(hashtagList);
        new QListWidgetItem(hashtagList);
        new QListWidgetItem(hashtagList);
        new QListWidgetItem(hashtagList);
        new QListWidgetItem(hashtagList);
        hashtagList->setObjectName(QString::fromUtf8("hashtagList"));
        hashtagList->setMaximumSize(QSize(200, 16777215));
        hashtagList->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);"));

        gridLayout_2->addWidget(hashtagList, 2, 1, 1, 1);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(60, 238, 33);\n"
"font: 75 12pt \"Magneto\";"));
        label_3->setScaledContents(false);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(60, 238, 33);\n"
"font: 75 12pt \"Magneto\";"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_2, 0, 1, 2, 1);

        tabWidget_2->addTab(tab_2, QString());
        progressBar = new QProgressBar(tab);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(540, 10, 751, 23));
        QFont font3;
        font3.setBold(true);
        font3.setWeight(75);
        progressBar->setFont(font3);
        progressBar->setToolTipDuration(1);
        progressBar->setStyleSheet(QString::fromUtf8("color: rgb(170, 0, 127);"));
        progressBar->setValue(24);
        progressBar->setAlignment(Qt::AlignCenter);
        widgetBrowser = new QWidget(tab);
        widgetBrowser->setObjectName(QString::fromUtf8("widgetBrowser"));
        widgetBrowser->setGeometry(QRect(480, 80, 801, 641));
        widgetBrowser->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
        comboBox = new QComboBox(tab);
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(1070, 50, 72, 22));
        splitter = new QSplitter(tab);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(0, 420, 441, 28));
        splitter->setOrientation(Qt::Horizontal);
        markKeywordsButton = new QPushButton(splitter);
        markKeywordsButton->setObjectName(QString::fromUtf8("markKeywordsButton"));
        sizePolicy.setHeightForWidth(markKeywordsButton->sizePolicy().hasHeightForWidth());
        markKeywordsButton->setSizePolicy(sizePolicy);
        QFont font4;
        font4.setFamily(QString::fromUtf8("Magneto"));
        font4.setPointSize(12);
        font4.setBold(false);
        font4.setItalic(false);
        font4.setUnderline(false);
        font4.setWeight(9);
        markKeywordsButton->setFont(font4);
        markKeywordsButton->setAutoFillBackground(false);
        markKeywordsButton->setStyleSheet(QString::fromUtf8("color: rgb(60, 238, 33);\n"
"font: 75 12pt \"Magneto\";"));
        markKeywordsButton->setCheckable(false);
        markKeywordsButton->setFlat(true);
        splitter->addWidget(markKeywordsButton);
        playVideoButton = new QPushButton(splitter);
        playVideoButton->setObjectName(QString::fromUtf8("playVideoButton"));
        sizePolicy.setHeightForWidth(playVideoButton->sizePolicy().hasHeightForWidth());
        playVideoButton->setSizePolicy(sizePolicy);
        playVideoButton->setFont(font4);
        playVideoButton->setStyleSheet(QString::fromUtf8("color: rgb(60, 238, 33);\n"
"font: 75 12pt \"Magneto\";"));
        playVideoButton->setCheckable(false);
        playVideoButton->setFlat(true);
        splitter->addWidget(playVideoButton);
        deleteVideoButton = new QPushButton(splitter);
        deleteVideoButton->setObjectName(QString::fromUtf8("deleteVideoButton"));
        sizePolicy.setHeightForWidth(deleteVideoButton->sizePolicy().hasHeightForWidth());
        deleteVideoButton->setSizePolicy(sizePolicy);
        deleteVideoButton->setFont(font4);
        deleteVideoButton->setStyleSheet(QString::fromUtf8("color: rgb(60, 238, 33);\n"
"font: 75 12pt \"Magneto\";"));
        deleteVideoButton->setCheckable(false);
        deleteVideoButton->setFlat(true);
        splitter->addWidget(deleteVideoButton);
        tabWidget->addTab(tab, QString());
        VFX = new QWidget();
        VFX->setObjectName(QString::fromUtf8("VFX"));
        runPyButton = new QPushButton(VFX);
        runPyButton->setObjectName(QString::fromUtf8("runPyButton"));
        runPyButton->setGeometry(QRect(70, 450, 111, 31));
        tabWidget->addTab(VFX, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1315, 21));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menuUpload = new QMenu(menubar);
        menuUpload->setObjectName(QString::fromUtf8("menuUpload"));
        menu_URLS = new QMenu(menubar);
        menu_URLS->setObjectName(QString::fromUtf8("menu_URLS"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menuUpload->menuAction());
        menubar->addAction(menu_URLS->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(action);
        menu->addAction(action_2);
        menu->addAction(action_3);
        menu->addAction(action_4);
        menu->addAction(action_7);
        menuUpload->addAction(action_youtube);
        menuUpload->addAction(action_brighteon);
        menu_URLS->addAction(actionPlay_video);
        menu_URLS->addAction(actionDelete_Video);
        menu_URLS->addAction(actionDownload_all);
        menu_URLS->addAction(actionSet_Temp_directory);
        menu_3->addAction(action_5);
        menu_3->addAction(action_6);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        findVideosButton->setDefault(false);
        tabWidget_2->setCurrentIndex(0);
        playVideoButton->setDefault(false);
        deleteVideoButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action->setText(QApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270 \320\270 \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\270\321\202\321\214 \320\272\320\273\321\216\321\207\320\265\320\262\321\213\320\265 \321\201\320\273\320\276\320\262\320\260 (\320\270\320\273\320\270 \321\205\320\265\321\210\321\202\320\265\320\263\320\270).", nullptr));
        action_2->setText(QApplication::translate("MainWindow", "\320\241\320\272\320\260\321\207\320\260\321\202\321\214 \320\270\321\201\321\202\320\276\321\207\320\275\320\272\320\270 \320\270\320\267\320\270 \321\216\321\202\321\214\321\216\320\261\320\260, \320\263\320\264\320\265 \320\262\321\201\321\202\321\200\320\265\321\207\320\260\321\216\321\202\321\201\321\217 \320\277\320\276\320\264\321\201\320\262\320\265\321\207\320\265\320\275\320\275\321\213\320\265 \321\201\320\273\320\276\320\262\320\260.", nullptr));
        action_3->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\320\270\321\202\321\214 \320\276\320\267\320\262\321\203\321\207\320\272\321\203 \320\262 \321\204\320\276\321\200\320\274\320\265 \321\200\320\265\321\207\320\270 + \320\274\321\203\320\267\321\213\320\272\320\260 ", nullptr));
        action_4->setText(QApplication::translate("MainWindow", "\320\241\320\276\320\261\321\200\320\260\321\202\321\214 \320\272\320\273\320\270\320\277", nullptr));
        action_youtube->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\275\320\260 youtube", nullptr));
        action_brighteon->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\275\320\260 brighteon", nullptr));
        actionPlay_video->setText(QApplication::translate("MainWindow", "Play video", nullptr));
        actionDelete_Video->setText(QApplication::translate("MainWindow", "Delete video", nullptr));
        actionDownload_all->setText(QApplication::translate("MainWindow", "Download all", nullptr));
        actionSet_Temp_directory->setText(QApplication::translate("MainWindow", "Set Temp directory", nullptr));
        action_5->setText(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\202\320\265\320\263", nullptr));
        action_6->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\202\320\265\320\263", nullptr));
        action_7->setText(QApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \321\201\320\277\320\265\321\206. \321\215\321\204\321\204\320\265\320\272\321\202\321\213 ", nullptr));
        textEditor->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\"># \320\242\320\265\320\272\321\201\321\202 \321\200\320\276\320\273\320\270\320\272\320\260. \320\236 \320\262\320\265\321\200\320\276\321\217\321\202\320\275\320\276\321\201\321\202\320\270 \321\200\320\265\320\260\320\273\320\270\320\267\320\260\321\206\320\270\320\270 \321\201\321\206\320\265\320\275\320\260\321\200\320\270\321\217 \320\260\320\273\321\217 &quot"
                        ";\320\242\320\265\321\200\320\274\320\270\320\275\320\260\321\202\320\276\321\200&quot;.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">Kvazikot</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\270\321\216\320\273\321\214 2021</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; color:#008000;\">&lt;c</span><span style=\" font-size:8pt; color:#c0c0c0;\"> </span><span style=\" font-size:8pt; color:#008000;\">00:00:50&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margi"
                        "n-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New'; font-size:8pt; color:#008000;\">&lt;e</span><span style=\" font-family:'Courier New'; font-size:8pt; color:#c0c0c0;\"> </span><span style=\" font-family:'Courier New'; font-size:8pt; color:#008000;\">fade_out(12.2,color1,color2,23) para_text </span><span style=\" font-size:8pt; color:#008000;\">3d_cube</span><span style=\" font-family:'Courier New'; font-size:8pt; color:#008000;\">&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New'; font-size:8pt; color:#008000;\">&lt;v</span><span style=\" font-family:'Courier New'; font-size:8pt; color:#c0c0c0;\"> </span><span style=\" font-family:'Courier New'; font-size:8pt; color:#008000;\">wheeler_1&gt;&lt;e</span><span style=\" font-family:'Courier New'; font-size:8pt; color:#c0c0c0;\"> </span><span style=\" font-family:'Courier New'"
                        "; font-size:8pt; color:#008000;\">interlace_fx(40,640,480) vfx1(12)&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\222 \320\270\320\267\320\262\320\265\321\201\321\202\320\275\320\276\320\274 \320\274\321\213\321\201\320\273\320\265\320\275\320\275\320\276\320\274 \321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\265 \320\224\320\266\320\276\320\275\320\260 \320\243\320\270\320\273\320\273\320\265\321\200\320\260</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\321\201 \320\264\320\262\321\203\320\274\321\217 \321\211\320\265\320\273\321\217\320\274\320\270 \320\270 \320\276\321\202\320\273\320\276\320\266\320\265\320\275\320\275\320\276\320\274 \320\262\321\213\320\261\320\276\321\200\320\265 \320\261\321\213\320\273 \320"
                        "\277\321\200\320\265\320\264\320\273\320\276\320\266\320\265\320\275\320\260[1] \320\277\321\200\320\276\320\262\320\265\321\200\320\272\320\260 \320\263\320\270\320\277\320\276\321\202\320\265\320\267\321\213 \320\276 \321\202\320\276\320\274 \321\207\321\202\320\276 \320\277\321\200\320\276\321\210\320\273\320\276\320\265 \320\274\320\276\320\266\320\275\320\276 \320\270\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \321\201\321\202\320\270\321\200\320\260\321\217 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\216 \320\276 \320\275\320\260\320\261\320\273\321\216\320\264\320\265\320\275\320\270\320\270 \320\262 \320\261\321\203\320\264\321\203\321\211\320\265\320\274.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0"
                        "px;\"><span style=\" font-weight:600; color:#00ff00;\">&lt;p 00:01:00&gt;</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00ff00;\">&lt;c 00:01:06&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00ff00;\">&lt;v wheeler_1,terminator_nude,nanobot,big_bang1&gt; &lt;e 3d_cube fade_in para_text&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\235\320\265\320\270\320\267\320\262\320\265\321\201\321\202\320\275\320\260 \321\202\320\276\321\207\320\275\320\260\321\217"
                        " \320\264\320\260\321\202\320\260 \320\272\320\276\320\263\320\264\320\260 \320\243\320\270\320\273\320\265\321\200\321\203 \320\277\321\200\320\270\321\210\320\273\320\260 \320\270\320\264\320\265\321\217 \321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\260 \321\201 \320\272\320\262\320\260\320\275\321\202\320\276\320\262\321\213\320\274 \320\273\320\260\321\201\321\202\320\270\320\272\320\276\320\274. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\257 \321\201\320\265\320\271\321\207\320\260\321\201 \320\275\320\265 \320\263\320\276\320\262\320\276\321\200\321\216 \320\276 \320\261\321\203\320\272\320\262\320\260\320\273\321\214\320\275\320\276\320\274 \321\201\321\206\320\265\320\275\320\260\321\200\320\270\320\270 \320\272\320\276\320\263\320\264\320\260 \320\270\320\267 \320\274\320\260\321\210\320\270\320\275\321\213 \320\262\321\200\320"
                        "\265\320\274\320\265\320\275\320\270 \320\262\321\213\321\205\320\276\320\264\321\217\321\202 \320\263\320\276\320\273\321\213\320\265 \321\202\320\265\321\200\320\274\320\270\320\275\320\260\321\202\320\276\321\200\321\213.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\255\321\202\320\276 \321\201\320\264\320\265\320\273\320\260\320\275\320\276 \321\200\320\260\320\264\320\270 \320\277\320\276\321\202\320\265\321\205\320\270 \320\277\321\203\320\261\320\273\320\270\320\272\320\270 \320\272\320\260\320\272 \320\270 \320\261\320\260\321\202\320\260\321\200\320\265\320\271\320\272\320\270 \320\262 &quot;\320\274\320\260\321\202\321\200\320\270\321\206\320\265&quot;, \320\262\320\265\320\264\321\214 \320\263\320\276\320\273\320\270\320\262\321\203\320\264\321\201\320\272\320\270\320\265 \321\204\320\270\320\273\321\214\320\274\321\213 \321\200\320\260\321\201\321\207\320\270\321\202\320\260"
                        "\320\275\320\275\321\213 \320\275\320\260 \320\274\320\260\321\201\321\201\320\276\320\262\320\276\320\263\320\276 \320\267\321\200\320\270\321\202\320\265\320\273\321\217.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\257 \320\263\320\276\320\262\320\276\321\200\321\216 \320\276 \320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276\321\201\321\202\320\270 \320\277\321\200\320\265\320\264\320\276\321\201\321\202\320\260\320\262\320\273\321\217\320\265\320\274\321\213\320\265 \320\272\320\262\320\260\320\275\321\202\320\276\320\262\320\276\320\271 \321\202\320\265\320\276\321\200\320\270\320\265\320\271 \320\277\320\276 \321\201\321\202\320\270\321\200\320\260\320\275\320\270\321\216 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270 \320\262 \320\261\321\203\320\264\321\203\321\211\320\265\320\274, \321\207\321\202\320\276\320\261\321\213 \320\262"
                        "\320\273\320\270\321\217\321\202\321\214 \320\275\320\260 \320\277\321\200\320\276\321\210\320\273\320\276\320\265 \320\275\320\265 \320\270\320\274\320\265\320\265\321\202 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\321\217 \320\275\320\260\321\201\320\272\320\276\320\273\321\214\320\272\320\276 \320\276\321\202\320\264\320\260\320\273\320\265\320\275\320\275\320\276\320\265. \320\224\320\265\320\273\321\214\321\202\320\260 t \320\274\320\276\320\266\320\265\321\202 \320\261\321\213\321\202\321\214 5 \320\274\320\270\320\273\320\273\320\270\321\201\320\265\320\272\321\203\320\275\320\264 \320\264\320\273\321\217 \320\276\320\261\320\275\320\260\321\200\321\203\320\266\320\265\320\275\320\270\321\217 \321\200\320\260\320\272\320\276\320\262\320\276\320\271 \320\272\320\273\320\265\321\202\320\272\320\270, \320\260 \320\274\320\276\320\266\320\265\321\202 \320\261\321\213\321\202\321\214 \320\262\320\276\320\267\321\200\320\260\321\201\321\202 \321\200\320\276\320\266\320\264\320\265\320\275\320\270"
                        "\321\217 \320\262\321\201\320\265\320\273\320\265\320\275\320\275\320\276\320\271.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00ff00;\">&lt;p 00:02:35&gt;</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00ff00;\">&lt;c 00:02:35&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\247\321\202"
                        "\320\276 \320\270\320\267\320\262\320\265\321\201\321\202\320\275\320\276 \321\202\320\276\321\207\320\275\320\276 \321\215\321\202\320\276 \321\202\320\276 \321\207\321\202\320\276 \321\200\320\265\320\260\320\273\320\270\320\267\320\276\320\262\320\260\321\202\321\214 \321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202 \321\201 \320\276\321\202\320\273\320\276\320\266\320\265\320\275\320\275\321\213\320\274 \320\262\321\213\320\261\320\276\321\200\320\276\320\274 \321\203\320\264\320\260\320\273\320\276\321\201\321\214 \321\202\320\276\320\273\321\214\320\272\320\276 \320\262 \320\262\320\276\321\201\321\214\320\274\320\270\320\264\320\265\321\201\321\217\321\202\321\213\321\205 1984. At the University of Maryland, Carroll Alley, Oleg Jakubowicz, and William Wickes\342\200\224on a laboratory bench.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\241"
                        "\321\202\321\200\320\260\320\275\320\275\320\276\320\265 \321\201\320\276\320\262\320\277\320\260\320\264\320\265\320\275\320\270\320\265 - \321\204\320\270\320\273\321\214\320\274 \320\242\320\265\321\200\320\274\320\270\320\275\320\260\321\202\320\276\321\200 \320\262\321\210\320\265\320\273 \320\262 1984 \320\263\320\276\320\264\321\203. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00ff00;\">&lt;00:03:03&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\243\321\207\320\270\321\202\321\213\320\262\320\260\321\217 \321\202\320\276\321\202 \321\204\320\260\320\272\321\202 \321\207\321"
                        "\202\320\276 deep state \321\203\320\266\320\265 \320\262\320\276 \320\262\321\201\321\216 \321\204\321\203\320\275\320\272\321\206\320\270\320\276\320\275\320\270\321\200\320\276\320\262\320\260\320\273 \320\270 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\260 DAPRA \321\201\321\203\321\211\320\265\321\201\321\202\320\262\320\276\320\262\320\260\320\273\320\260. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\222\320\265\321\200\320\276\321\217\321\202\320\275\320\276 \321\203\320\266\320\265 \321\202\320\276\320\263\320\264\320\260 \320\261\321\213\320\273\320\270 \320\262\321\213\321\200\320\260\320\261\320\276\321\202\320\260\320\275\321\213 \320\276\321\201\320\276\320\261\321\213\320\265 \320\277\321\200\320\276\321\202\320\276\320\272\320\276\320\273\321\213 \320\276\321\204\320\276\321\200\320\274\320\273\320\265\320\275\320\270\321\217 \320\275\320\260\321"
                        "\203\321\207\320\275\321\213\321\205 \321\201\321\202\320\260\321\202\320\265\320\271 \321\201 \320\275\320\265\321\201\320\272\320\276\320\273\321\214\320\272\320\270\320\274\320\270 \321\201\321\202\320\260\320\264\320\270\321\217\320\274\320\270 \320\277\321\200\320\276\320\262\320\265\321\200\320\272\320\270 \320\270 \321\200\320\265\321\206\320\265\320\275\320\267\320\270\321\200\320\276\320\262\320\260\320\275\320\270\321\217 \321\201\321\202\320\260\321\202\320\265\320\271 \320\264\320\276 \321\201\320\260\320\274\320\276\320\263\320\276 \320\274\320\276\320\274\320\265\320\275\321\202\320\260 \320\264\320\276\320\277\321\203\321\201\320\272\320\260 \320\262 \320\277\320\265\321\207\320\260\321\202\321\214. \320\222\320\260\320\274 \320\275\320\265\320\276\320\261\321\205\320\276\320\264\320\270\320\274\320\276 \320\277\321\200\320\276\320\271\321\202\320\270 \320\275\320\265\321\201\320\272\320\276\320\273\321\214\320\272\320\276 \320\272\321\200\321\203\320\263\320\276\320\262 \320\260\320\264\320\260"
                        " - \321\215\321\202\320\276 \321\201\320\275\320\270\320\267\320\270\321\202 \320\262\320\265\321\200\320\276\321\217\321\202\320\275\320\276\321\201\321\202\321\214 \320\277\320\276\320\277\320\260\320\264\320\260\320\275\320\270\321\217 \320\262 \321\201\320\260\320\274\321\203\321\216 \321\201\320\265\321\200\320\264\321\206\320\265\320\262\320\270\320\275\321\203 \321\207\321\203\320\264\320\276\320\262\320\270\321\211\320\260. \320\242\320\276\321\207\320\275\321\213\320\265 \320\264\320\260\321\202\321\213 \320\272\320\260\320\272 \320\270 \320\275\320\260\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265 \321\201\321\202\320\260\321\202\320\265\320\271 \320\274\320\276\320\263\321\203\321\202 \320\274\320\265\320\275\321\217\321\202\321\214\321\201\321\217, </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\276\321\201\320\276\320\261\320\265\320\275\320\275\320\276 \321"
                        "\203\321\207\320\270\321\202\321\213\320\262\320\260\321\217 \321\202\320\276\321\202 \321\204\320\260\320\272\321\202 \321\207\321\202\320\276 \321\201\320\276\320\262\321\200\320\265\320\274\320\265\320\275\320\275\321\213\320\265 \321\201\320\265\321\202\320\270 \320\263\320\273\321\203\320\261\320\276\320\272\320\276\320\263\320\276 \320\276\320\261\321\203\321\207\320\265\320\275\320\270\321\217 \320\274\320\276\320\263\321\203\321\202 \321\201\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \321\201\321\202\320\260\321\202\321\214\321\216 \320\277\320\276\321\205\320\276\320\266\321\203\321\216 \320\275\320\260 \321\200\321\203\320\272\320\276\320\277\320\270\321\201\321\214 \320\270\320\273\320\270 \321\201\320\264\320\265\320\273\320\260\320\275\320\275\321\203\321\216 \320\275\320\260 \321\201\321\202\320\260\321\200\320\276\320\274 \320\277\321\200\320\270\320\275\321\202\320\265\321\200\320\265 \320\270\320\273\320\270 \320\277\320\265\321\207\320\260\321"
                        "\202\320\275\320\271 \320\274\320\260\321\210\320\270\320\275\320\272\320\265. \320\222\320\260\320\266\320\275\320\276 \320\262\321\213\321\217\320\262\320\273\321\217\321\202\321\214 \320\277\320\276\320\264\320\273\320\270\320\275\320\275\320\276\321\201\321\202\320\270 \321\201\321\202\320\260\321\202\321\214\320\270, \321\207\321\202\320\276 \320\272\320\276\320\275\320\265\321\207\320\275\320\276 \320\275\320\265\320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276 \321\201\320\264\320\265\320\273\320\260\321\202\321\214 \320\270\320\274\320\265\321\217 \321\201\320\272\320\260\320\275, \320\260 \320\275\320\265 \320\261\321\203\320\274\320\260\320\266\320\275\321\203\321\216 \320\262\320\265\321\200\321\201\320\270\321\216. \320\237\320\276\321\202\320\276\320\274\321\203 \321\207\321\202\320\276 \320\262 \320\261\321\203\320\274\320\266\320\275\320\276\320\271 \320\262\320\265\321\200\321\201\320\270\320\270 \320\277\320\276\320\274\320\276\320\263\320\260\320\265\321\202 \320\270\320\267\320"
                        "\276\321\202\320\276\320\277\320\275\321\213\320\265 \320\274\320\265\321\202\320\276\320\264 \320\264\320\260\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\321\217. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00ff00;\">&lt;00:04:05&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\230\320\264\320\265\321\217 \320\264\320\260\320\277\321\200\321\213 \320\267\320\260\320\272\320\273\321\216\321\207\320\260\320\265\321\202\321\201\321\217 \320\262 \321\200\320\260\320\267\321\200\320\260\320\261\320\276\321\202\320\272\320\265 \320\263\320\270\320\277\320\276\321\202\320"
                        "\265\321\202\320\270\321\207\320\265\321\201\320\272\320\270\321\205 \320\277\321\200\320\276\320\265\320\272\321\202\320\276\320\262, \320\275\320\276 \320\275\320\265\321\202 \320\267\320\260\320\272\320\276\320\275\320\276\320\262 \321\204\320\270\320\267\320\270\320\272\320\270, \320\272\320\276\321\202\320\276\321\200\321\213\320\265 \320\267\320\260\320\277\321\200\320\265\321\211\320\260\320\273\320\270 \320\261\321\213 \320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276\321\201\321\202\321\214 \321\200\320\260\320\267\321\200\320\260\320\261\320\276\321\202\320\272\320\270 \320\274\320\276\320\264\320\265\320\274\320\260 \320\264\320\273\321\217 \320\277\321\200\320\270\320\265\320\274\320\260 &quot;\320\275\320\276\320\262\320\276\321\201\321\202\320\265\320\271&quot; \320\270\320\267 \320\261\321\203\320\264\321\203\321\211\320\265\320\263\320\276. \320\235\320\260 \320\276\321\201\320\275\320\276\320\262\320\265 \321\207\320\265\320\263\320\276 \320\274\320\276\320\266\320\275\320\276 \320"
                        "\262\321\213\320\270\320\263\321\200\320\260\321\202\321\214 \320\273\321\216\320\261\321\203\321\216 \320\262\320\276\320\271\320\275\321\203, \320\262\320\265\320\264\321\214 \321\200\320\265\321\202\321\200\320\276\320\272\320\260\321\203\320\267\321\203\320\260\320\273\321\214\320\275\321\213\320\265 \320\276\320\261\321\200\320\260\321\202\320\275\321\213\320\265 \321\201\320\262\321\217\320\267\320\270 \320\260\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270 \320\262\320\265\320\264\321\203\321\202 \320\272 \320\266\320\265\320\273\320\260\320\265\320\274\320\276\320\274\321\203 \321\201\321\206\320\265\320\275\320\260\321\200\320\270\321\216 \320\261\321\203\320\264\321\203\321\211\320\265\320\263\320\276. \320\232\320\260\320\272-\321\202\320\276 \320\240\321\215\320\271 \320\232\321\200\321\206\320\262\320\265\320\271\320\273 \321\203\320\277\320\276\320\274\320\270\320\275\321\203\320\273 \320\272\320\260\320\272-\321\202\320\276 \320\276 \321\202\320\276"
                        "\320\274 \321\207\321\202\320\276 \320\241\320\276\320\262\320\265\321\202\321\201\320\272\320\270\320\271 \320\241\320\276\321\216\320\267 \320\261\321\213\320\273 \321\200\320\260\320\267\320\262\320\260\320\273\320\265\320\275 \320\261\320\273\320\260\320\263\320\276\320\264\320\260\321\200\321\217 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\276\320\275\320\275\321\213\320\274 \321\202\320\265\321\205\320\275\320\276\320\273\320\276\320\263\320\270\321\217\320\274. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\223\320\276\320\262\320\276\321\200\321\217 \320\276 \320\232\321\203\321\200\321\206\320\262\320\265\320\271\320\273\320\265 \320\275\320\265\320\276\320\261\321\205\320\264\320\270\320\274\320\276 \320\276\321\202\320\274\320\265\321\202\320\270\321\202\321\214 \320\264\320\276\320\262\320\276\320\273\321\214\320\275\320\276 \321\201\321\202"
                        "\321\200\320\260\320\275\320\275\321\203\321\216 \321\202\320\276\321\207\320\275\320\276\321\201\321\202\321\214 \320\265\320\263\320\276 \320\277\321\200\320\276\320\263\320\275\320\276\320\267\320\276\320\262, \321\207\321\202\320\276 \321\202\320\260\320\272\320\266\320\265 \320\277\320\276\320\264\321\202\320\262\320\265\321\200\320\266\320\264\320\260\320\265\321\202 \320\262\320\265\321\201 \320\263\320\270\320\277\320\276\321\202\320\265\320\267\321\213 \320\276 \320\274\320\276\320\264\320\265\320\274\320\265 \320\270\320\267 \320\261\321\203\320\264\321\203\321\211\320\265\320\263\320\276. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\232\321\201\321\202\320\260\321\202\320\270 \320\263\320\276\320\262"
                        "\320\276\321\200\321\217 \320\276\320\261 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270 \320\270\320\267 \320\261\321\203\320\264\321\203\321\211\320\265\320\263\320\276, \320\274\320\276\320\266\320\275\320\276 \320\276\321\202\320\274\320\265\321\202\320\270\321\202\321\214 \321\207\321\202\320\276 \321\215\321\202\320\276 \321\201\320\277\320\276\321\201\320\276\320\261 \320\264\320\273\321\217 \321\202\320\276\320\263\320\276 \321\207\321\202\320\276\320\261\321\213 \321\203\321\201\320\272\320\276\321\200\320\270\321\202\321\214 \321\200\320\260\321\201\321\207\320\265\321\202\321\213. \320\222\320\265\320\264\321\214 \320\274\321\213 \320\277\320\276\320\264\320\260\320\265\320\274 \320\275\320\260 \320\262\321\205\320\276\320\264 \320\274\320\260\321\210\320\270\320\275\321\213 \320\242\321\214\321\216\321\200\320\270\320\275\320\263\320\260 \320\265\320\265 \320\261\321\203\320\264\321\203\321\211\320\270\320\265 \321\200\320\265\320\267\321\203\320\273\321\214\321\202"
                        "\320\260\321\202\321\213[5]. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\235\320\265\320\276\320\261\321\205\320\276\320\264\320\270\320\274\320\276 \320\262\320\276\320\267\321\200\320\276\320\264\320\270\321\202\321\214 \321\207\321\202\320\276-\321\202\320\276 \320\275\320\260\320\277\320\276\320\264\320\276\320\261\320\270\320\270 \321\202\320\265\321\205\320\275\320\265\321\202\320\270\320\272\320\270 \320\277\321\200\320\265\320\264\320\273\320\276\320\266\320\265\320\275\320\275\320\276\320\271 \320\232\321\203\320\264\321\200\320\270\320\275\321\213\320\274 \320\264\320\273\321\217 \320\277\320\276\321\201\321\202\321\200\320\276\320\265\320\275\320\270\321\217 \321\204\320\270\320\273\320\276\320\263\320"
                        "\265\320\275\320\265\321\202\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\264\320\265\321\200\320\265\320\262\320\260 \321\201\320\274\320\260\321\200\321\202\321\204\320\276\320\275\320\276\320\262. [2]</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\235\320\260 \320\277\321\200\320\270\320\274\320\265\321\200\320\265 \320\276\320\264\320\275\320\276\320\263\320\276 \320\270\320\267 \321\201\320\260\320\274\321\213\321\205 \320\262\320\265\320\273\320\270\321\207\320\260\320\271\321\210\320\270\321\205 \321\202\320\265\321\205\320\275\320\270\321\207\320\265\321\201\320\272\320\270\321\205 \320\264\320\276\321\201\321\202\320\270\320\266\320\265\320\275\320\270\320\271 deep state - \321\201\320\274\320\260\321\200\321\202\321\204\320\276\320\275\320\265 \320\275\320\260\320\274 \320\275\320\265\320\276\320\261\321\205\320\276\320\264\320\270\320\274\320\276 \320\276"
                        "\321\202\321\201\320\273\320\265\320\264\320\270\321\202\321\214 \320\277\320\276\321\217\320\262\320\273\320\265\320\275\320\270\320\265  \320\262\321\201\320\265\321\205 \320\277\321\200\320\276\321\202\320\276\321\202\320\270\320\277\320\276\320\262 \320\260\320\271\321\204\320\276\320\275\320\260. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\235\320\260\320\270\320\261\320\276\320\273\320\265\320\265 \321\200\320\260\320\275\320\265\320\265 \321\203\320\277\320\276\320\274\320\270\320\275\320\260\320\275\320\270\320\265 \320\274\320\275\320\265 \321\203\320\264\320\260\320\273\320\276\321\201\321\214 \320\275\320\260\320\271\321\202\320\270 \320\262 2021 \320\263\320\276\320\264\321\203 \320\275\320\260 \321\201\320\260\320\271\321\202\320\265 arhive.org </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-inde"
                        "nt:0px;\"><span style=\" color:#00ff00;\">\320\262 \321\201\320\272\320\260\320\275\320\260\321\205 \321\204\321\200\320\260\320\275\321\206\321\203\320\267\321\201\320\272\320\276\320\263\320\276 \320\266\321\203\321\200\320\275\320\260\320\273\320\260 DESIGN \320\267\320\260 1987. \320\235\320\260\320\267\321\213\320\262\320\260\320\273\321\201\321\217 \320\276\320\275 pocket blue brain.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\222 \320\277\320\276\320\270\321\201\320\272\320\265 \320\263\321\203\320\263\320\273\320\260 \320\275\320\260 \320\274\320\276\320\274\320\265\320\275\321\202 \320\275\320\260\320\277\320\270\321\201\320\260\320\275\320\270\321\217 \321\215\321\202\320\276 \321\201\321\202\320\260\321\202\321\214\320\270 \320\276\321\202\321\201\321\203\321\202\320\262\321\203\320\265\321\202 \320\272\320\260\320\272\320\260\321\217-\320\273\320\270\320\261\320\276 \320\270"
                        "\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217 \320\276\320\261 \321\215\321\202\320\276\320\274 \320\277\321\200\320\276\321\202\320\276\321\202\320\270\320\277\320\265, \320\262\320\276\320\274\320\276\320\266\320\275\320\276 \320\276\320\275 \320\261\321\213\320\273 \320\276\320\264\320\270\320\275 \320\270\320\267 \320\277\320\265\321\200\320\262\321\213\321\205.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\236\320\275 \321\203\320\266\320\265 \321\202\320\276\320\263\320\264\320\260 \320\270\320\274\320\265\320\273 \320\275\320\260\320\261\320\276\321\200 \321\201\321\202\320\260\320\275\320\264\320\260\321\200\321\202\320\275\321\213\321\205 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\320\271 \320\270 \321\217\320\262\320\273\321\217\320\273\321\201\321\217 \321\203\320\275\320\270\320\262\320\265\321\200\321\201\320\260\320"
                        "\273\321\214\320\275\321\213\320\274 \320\272\320\276\320\274\320\277\321\214\321\216\321\202\320\265\321\200\320\276\320\274, \321\202\320\276\320\273\321\214\320\272\320\276 \321\215\320\272\321\200\320\260\320\275 \320\261\321\213\320\273 \321\207\320\265\321\200\320\275\320\276-\320\261\320\265\320\273\321\213\320\271.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\237\320\276\320\267\320\266\320\265 \320\263\320\264\320\265-\321\202\320\276 \320\275\320\260 \321\200\321\203\320\261\320\265\320\266\320\265 1992 \320\263\320\276\320\264\320\260 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260 \320\275\320\260 \320\277\320\276\320\264\320\276\320\261\320\270\320\270 \320\272\320\260\321\200\320\274\320\260\320\275\320\275\320\276\320\263\320\276 \320\272\320\276\320\274\320\277\321\214\321\216\321\202\320\265\321\200\320\260 \320\224\320\266\320\265\321"
                        "\204\320\260 \320\245\320\276\321\203\320\272\320\270\320\275\321\201\320\260 \320\275\320\260\321\207\320\260\320\273\320\270 \321\200\320\260\320\267\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\321\202\321\214\321\201\321\217 \320\272\321\200\321\203\320\277\320\275\320\265\320\271\321\210\320\270\320\274\320\270 \320\272\320\276\321\200\320\277\320\276\321\200\320\260\321\206\320\270\321\217\320\274\320\270 \321\202\320\276\320\263\320\276 \320\262\321\200\320\265\320\274\320\265\320\275\320\270 NEC, Samsung</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#00ff00;\">\320\237\320\276\321\201\320\272\320\276\320\273\321\214\320\272\321\203 \321\201\320\262\320\270\320\264\320\265\321\202\320\265\320\273\321\214"
                        "\321\201\321\202\320\262\320\260 \321\201\320\276\320\261\321\213\321\202\320\270\320\271 \320\277\321\200\320\276\321\210\320\273\320\276\320\263\320\276 \320\262\321\201\320\265 \320\262\321\200\320\265\320\274\321\217 \321\201\320\273\320\265\320\263\320\272\320\260 \320\274\320\265\320\275\321\217\321\216\321\202\321\201\321\217, \320\272\320\260\320\272\320\270\320\265-\321\202\320\276 \321\203\320\275\320\270\321\207\321\202\320\276\320\266\320\260\321\216\321\202\321\201\321\217 \321\204\320\270\320\267\320\270\321\207\320\265\321\201\320\272\320\270, \320\272\320\260\320\272\320\270\320\265-\321\202\320\276 \320\270\320\267\320\267\320\260 \320\275\320\265\321\201\320\276\320\262\320\265\321\200\321\210\320\265\320\275\321\201\321\202\320\262\320\260 \320\277\320\260\320\274\321\217\321\202\320\270 \321\207\320\265\320\273\320\276\320\262\320\265\320\272\320\260 \320\267\320\260\320\261\321\213\320\262\320\260\321\216\321\202\321\201\321\217 \320\270\320\273\320\270 \320\262\321\201\320\277\320\276\320"
                        "\274\320\270\320\275\320\260\321\216\321\202\321\201\321\217 \320\262 \320\270\321\201\320\272\320\260\320\266\320\265\320\275\320\275\320\276\320\274 \320\262\320\270\320\264\320\265, \320\276 \321\201\320\276\320\261\321\213\321\202\320\270\321\217\321\205 \320\277\321\200\320\276\321\210\320\273\320\276\320\263\320\276 \320\274\320\276\320\266\320\275\320\276 \320\263\320\276\320\262\320\276\321\200\320\270\321\202\321\214 \321\202\320\276\320\273\321\214\320\272\320\276 \320\262 \320\262\320\265\321\200\320\276\321\217\321\202\320\275\320\276\321\201\321\202\320\275\320\276\320\274 \321\201\320\274\321\213\321\201\320\273\320\265. \320\234\321\213 \320\262\321\201\320\265 \320\262\321\200\320\265\320\274\321\217 \320\275\320\260\321\205\320\276\320\264\320\270\320\274\321\201\321\217 \320\262 \320\275\320\265\320\272\320\276\320\265\320\271 \320\272\320\262\320\260\320\275\321\202\320\276\320\262\320\276-\320\274\320\265\321\205\320\260\320\275\320\270\321\207\320\265\321\201\320\272\320\276\320\271 \320\277"
                        "\320\265\320\275\320\265, \320\274\321\213 \320\262\320\270\320\264\320\270\320\274 \320\276\320\261\321\200\320\260\320\267 \320\275\320\265 \320\262\321\201\320\265\320\263\320\276 \320\264\321\200\320\260\320\272\320\276\320\275\320\260 \320\262 \320\275\320\265\320\261\320\265. \320\230\320\275\320\276\320\263\320\264\320\260 \320\277\320\276\321\217\320\262\320\273\321\217\320\265\321\202\321\201\321\217 \320\276\321\202\320\264\320\265\320\273\321\214\320\275\321\213\320\265 \321\207\320\260\321\201\321\202\320\270 \320\265\320\263\320\276 \321\202\320\265\320\273\320\260, \320\277\320\276 \320\272\320\276\321\202\320\276\321\200\321\213\320\274 \320\274\320\276\320\266\320\275\320\276 \320\276\320\277\320\276\320\267\320\275\320\260\321\202\321\214 \321\207\321\202\320\276 \321\215\321\202\320\276 \320\264\321\200\320\260\320\272\320\276\320\275, \320\275\320\276 \320\277\320\276\321\202\320\276\320\274 \320\276\320\275 \320\276\320\277\321\217\321\202\321\214 \321\201\320\272\321\200\321\213\320\262\320"
                        "\260\320\265\321\202\321\201\321\217 \320\262 \321\202\321\203\320\274\320\260\320\275\320\265 \320\270\320\273\320\270 \320\267\320\260 \320\276\320\261\320\273\320\260\320\272\320\260\320\274\320\270. \320\237\320\276\321\201\320\273\320\265 \320\275\320\265\320\272\320\276\321\202\320\276\321\200\320\276\320\263\320\276 \320\262\321\200\320\265\320\274\320\265\320\275\320\270 \321\205\320\260\320\276\321\201\320\260 \320\270 \320\275\320\265\320\276\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275\320\275\320\276\321\201\321\202\320\270 \320\275\320\260\321\201\321\202\321\203\320\277\320\260\321\216\321\202 \321\200\320\265\320\267\320\272\320\270\320\265 \321\201\320\272\320\260\321\207\320\272\320\270 \320\262 \321\200\320\260\320\267\320\262\320\270\321\202\320\270\320\270. \320\241\321\200\320\260\320\262\320\275\320\270\321\202\320\265 \321\201\320\270\321\202\321\203\320\260\321\206\320\270\321\216 \320\264\320\276 \320\275\320\260\321\207\320\260\320\273\320\260 II \320\274\320\270\321"
                        "\200\320\276\320\262\320\276\320\271 \320\262\320\276\320\271\320\275\321\213 \320\270 \320\277\320\276\321\201\320\273\320\265[4]. \320\224\320\276\320\262\320\276\320\273\321\214\320\275\320\276 \321\201\321\202\321\200\320\260\320\275\320\275\321\213\320\274 \321\217\320\262\320\273\321\217\320\265\321\202\321\201\321\217 \321\204\320\260\320\272\321\202 \321\207\320\265\321\200\320\265\320\267\320\264\320\276\320\262\320\260\320\275\320\270\321\217 \320\267\320\270\320\274 \320\262 \320\276\320\261\320\273\320\260\321\201\321\202\320\270 \320\230\320\230. \320\235\320\260\320\277\321\200\320\270\320\274\320\265\321\200 \321\204\320\260\320\272\321\202 \321\202\320\276\320\263\320\276 \321\207\321\202\320\276 \320\234\320\260\321\200\320\262\320\270\320\275 \320\234\320\270\320\275\321\201\320\272\320\270 \320\262\321\213\321\200\320\260\320\266\320\260\320\273 \320\272\321\200\320\270\321\202\320\270\320\272\321\203 \320\262 \320\276\321\202\320\275\320\276\321\210\320\265\320\275\320\270\320\270 supervise"
                        " learning \320\275\320\260 \320\276\321\201\320\275\320\276\320\262\320\265 \320\275\320\265\320\271\321\200\320\276\320\275\320\275\321\213\321\205 \321\201\320\265\321\202\320\265\320\271 \320\270 \320\272\321\200\320\270\321\202\320\270\320\272\321\203 \320\237\320\265\320\275\321\200\320\276\321\203\320\267\320\260 \320\270 \320\241\320\265\321\200\320\273\321\217 \320\262 \320\276\321\202\320\275\320\276\321\210\320\265\320\275\320\270\320\270 \321\201\320\270\320\273\321\214\320\275\320\276\320\263\320\276 \320\230\320\230 \320\262 \321\206\320\265\320\273\320\276\320\274, \320\272\320\276\320\263\320\264\320\260 \320\276\320\275\320\270 \320\272\320\260\320\267\320\260\320\273\320\276\321\201\321\214 \320\261\321\213 \320\277\321\200\320\276\320\264\320\262\320\270\320\275\321\203\320\273\320\270 \321\200\320\260\321\201\320\277\320\276\320\267\320\275\320\260\320\262\320\260\320\275\320\270\320\265 \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262. \320\255\321\202\320\260 \320\272\321\200"
                        "\320\270\321\202\320\270\320\272\320\260 \321\203\321\207\320\270\321\202\321\213\320\262\320\260\321\217 \320\260\320\262\321\202\320\276\321\200\320\270\321\202\320\265\321\202 \321\203\321\207\320\265\320\275\320\276\320\263\320\276 \320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276 \320\277\320\276\320\262\320\273\320\270\321\217\320\273\320\260 \320\275\320\260 \321\200\320\260\320\267\320\262\320\270\321\202\320\265 \321\201\320\265\321\202\320\265\320\271 \320\263\320\273\321\203\320\261\320\276\320\272\320\276\320\263\320\276 \320\276\320\261\321\203\321\207\320\265\320\275\320\270\321\217 \320\270 \320\274\320\265\321\202\320\276\320\264\320\270\320\272 \320\277\320\276\321\217\320\262\320\270\320\262\321\210\320\270\321\205\321\201\321\217 \320\277\320\276\321\201\320\273\320\265 2007 \320\263\320\276\320\264\320\260 \320\275\320\260 \320\276\321\201\320\275\320\276\320\262\320\265 big data. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-l"
                        "eft:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#00ff00;\"><br /></p></body></html>", nullptr));
        label->setText(QApplication::translate("MainWindow", "Clip text", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Render", nullptr));
        findVideosButton->setText(QApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270 \320\262\320\270\320\264\320\265\320\276 \n"
" \320\264\320\273\321\217 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\275\320\276\320\263\320\276 \321\202\320\265\320\272\321\201\321\202\320\260", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "Execute", nullptr));
        lineEdit->setText(QApplication::translate("MainWindow", "cam01.pos = (3,2,1)", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Clear", nullptr));
        console->setPlainText(QApplication::translate("MainWindow", "333\n"
"22\n"
"\n"
"222", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab1), QApplication::translate("MainWindow", "\320\232\320\276\320\275\321\201\320\276\320\273\321\214", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Object name", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00ff00;\">Property   |  Value</span></p></body></html>", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Property", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Value", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("MainWindow", "Object Inspector", nullptr));

        const bool __sortingEnabled = urlsList->isSortingEnabled();
        urlsList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = urlsList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "http://youtube.com/wdwdwd", nullptr));
        urlsList->setSortingEnabled(__sortingEnabled);


        const bool __sortingEnabled1 = hashtagList->isSortingEnabled();
        hashtagList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem1 = hashtagList->item(0);
        ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "#\321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = hashtagList->item(1);
        ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "#\320\261\320\260\321\202\320\260\321\200\320\265\320\271\320\272\320\260", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = hashtagList->item(2);
        ___qlistwidgetitem3->setText(QApplication::translate("MainWindow", "#\320\243\320\270\320\273\320\273\320\265\321\200", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = hashtagList->item(3);
        ___qlistwidgetitem4->setText(QApplication::translate("MainWindow", "#delayed choice experiment", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = hashtagList->item(4);
        ___qlistwidgetitem5->setText(QApplication::translate("MainWindow", "#quantum mechanics", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = hashtagList->item(5);
        ___qlistwidgetitem6->setText(QApplication::translate("MainWindow", "#physics", nullptr));
        QListWidgetItem *___qlistwidgetitem7 = hashtagList->item(6);
        ___qlistwidgetitem7->setText(QApplication::translate("MainWindow", "#Terminator movie", nullptr));
        hashtagList->setSortingEnabled(__sortingEnabled1);

        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00ff00;\">URLS</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Hashtags", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("MainWindow", "Urls & Hashtags", nullptr));
        progressBar->setFormat(QApplication::translate("MainWindow", "%p%", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "cam01", nullptr));

        markKeywordsButton->setText(QApplication::translate("MainWindow", "Mark keywords", nullptr));
        playVideoButton->setText(QApplication::translate("MainWindow", "Play video", nullptr));
        deleteVideoButton->setText(QApplication::translate("MainWindow", "Delete video", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\320\242\320\265\320\272\321\201\321\202 \320\272\320\273\320\270\320\277\320\260 \320\270 \320\270\321\201\321\202\320\276\321\207\320\275\320\270\320\272\320\270", nullptr));
        runPyButton->setText(QApplication::translate("MainWindow", "Run in python", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(VFX), QApplication::translate("MainWindow", "VFX", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\320\230\320\275\321\201\321\202\321\200\321\203\320\274\320\265\320\275\321\202\321\213", nullptr));
        menu_2->setTitle(QApplication::translate("MainWindow", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200", nullptr));
        menuUpload->setTitle(QApplication::translate("MainWindow", "Upload", nullptr));
        menu_URLS->setTitle(QApplication::translate("MainWindow", "\320\232\320\276\320\275\321\202\320\265\320\272\321\201\321\202\320\275\320\276\320\265 \320\274\320\265\320\275\321\216 URLS", nullptr));
        menu_3->setTitle(QApplication::translate("MainWindow", "\320\232\320\276\320\275\321\202\320\265\320\272\321\201\321\202\320\275\320\276\320\265 \320\274\320\265\320\275\321\216 \320\245\320\265\321\210\321\202\320\265\320\263\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
