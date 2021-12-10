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
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QListWidget *varsList;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *console;
    QWidget *gl_widget;
    QListView *listView;
    QGroupBox *groupBox;
    QToolButton *moveButton;
    QToolButton *selButton;
    QToolButton *pickButton;
    QToolButton *gameButton;
    QGroupBox *groupBox_2;
    QPushButton *xB;
    QPushButton *yB;
    QPushButton *zB;
    QComboBox *camBox;
    QWidget *gl_widget2;
    QToolButton *savelButton;
    QPushButton *reloadShadersBut;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pushButton;
    QTableWidget *inspectorTable;
    QComboBox *objectBox;
    QPushButton *setVars;
    QLabel *classLabel;
    QCheckBox *addAttributes;
    QCheckBox *filter;
    QListWidget *objectList;
    QLabel *label;
    QCheckBox *bHide;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QCheckBox *bGroups;
    QCheckBox *bVeh;
    QCheckBox *bHelp;
    QCheckBox *bHidden;
    QCheckBox *bCurves;
    QCheckBox *bPrim;
    QCheckBox *bUngroop;
    QCheckBox *bGeom;
    QToolBox *dialogBox;
    QWidget *page;
    QLineEdit *objFilter;
    QWidget *tab_3;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout;
    QCheckBox *bColorRendering;
    QCheckBox *bdisable_timer2;
    QCheckBox *bWire;
    QCheckBox *bShading;
    QCheckBox *bdisable_timer1;
    QCheckBox *bChangedRenderMode;
    QCheckBox *bNeedUpdVars;
    QCheckBox *gridCB;
    QWidget *tab_2;
    QToolButton *resetSim;
    QPushButton *pushButton_3;
    QComboBox *simMode;
    QLabel *label_3;
    QToolButton *simButton;
    QPlainTextEdit *varsEdit;
    QLabel *label_2;
    QWidget *tab_4;
    QFrame *plotFrame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1672, 1182);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        varsList = new QListWidget(centralWidget);
        varsList->setObjectName(QString::fromUtf8("varsList"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(varsList->sizePolicy().hasHeightForWidth());
        varsList->setSizePolicy(sizePolicy);
        varsList->setMaximumSize(QSize(16777215, 100));
        varsList->setFlow(QListView::LeftToRight);
        varsList->setProperty("isWrapping", QVariant(true));

        gridLayout->addWidget(varsList, 4, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        console = new QTextEdit(centralWidget);
        console->setObjectName(QString::fromUtf8("console"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(console->sizePolicy().hasHeightForWidth());
        console->setSizePolicy(sizePolicy1);
        console->setMaximumSize(QSize(16777215, 333));
        console->setFocusPolicy(Qt::ClickFocus);
        console->setOverwriteMode(true);

        horizontalLayout_2->addWidget(console);


        gridLayout->addLayout(horizontalLayout_2, 5, 0, 1, 1);

        gl_widget = new QWidget(centralWidget);
        gl_widget->setObjectName(QString::fromUtf8("gl_widget"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(gl_widget->sizePolicy().hasHeightForWidth());
        gl_widget->setSizePolicy(sizePolicy2);
        gl_widget->setMinimumSize(QSize(111, 0));
        gl_widget->setMaximumSize(QSize(800, 600));
        listView = new QListView(gl_widget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(490, 160, 256, 192));

        gridLayout->addWidget(gl_widget, 0, 0, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 60));
        moveButton = new QToolButton(groupBox);
        moveButton->setObjectName(QString::fromUtf8("moveButton"));
        moveButton->setGeometry(QRect(10, 20, 41, 31));
        moveButton->setCheckable(true);
        moveButton->setAutoExclusive(true);
        moveButton->setAutoRaise(false);
        selButton = new QToolButton(groupBox);
        selButton->setObjectName(QString::fromUtf8("selButton"));
        selButton->setGeometry(QRect(90, 20, 41, 31));
        selButton->setCheckable(true);
        selButton->setChecked(false);
        selButton->setAutoExclusive(true);
        selButton->setAutoRaise(false);
        pickButton = new QToolButton(groupBox);
        pickButton->setObjectName(QString::fromUtf8("pickButton"));
        pickButton->setGeometry(QRect(50, 20, 41, 31));
        pickButton->setCheckable(true);
        pickButton->setAutoExclusive(true);
        pickButton->setAutoRaise(false);
        gameButton = new QToolButton(groupBox);
        gameButton->setObjectName(QString::fromUtf8("gameButton"));
        gameButton->setGeometry(QRect(130, 20, 41, 31));
        gameButton->setCheckable(true);
        gameButton->setChecked(false);
        gameButton->setAutoExclusive(true);
        gameButton->setAutoRaise(false);
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(360, 10, 151, 51));
        xB = new QPushButton(groupBox_2);
        xB->setObjectName(QString::fromUtf8("xB"));
        xB->setGeometry(QRect(20, 20, 31, 23));
        xB->setAutoExclusive(true);
        yB = new QPushButton(groupBox_2);
        yB->setObjectName(QString::fromUtf8("yB"));
        yB->setGeometry(QRect(50, 20, 31, 23));
        yB->setAutoExclusive(true);
        zB = new QPushButton(groupBox_2);
        zB->setObjectName(QString::fromUtf8("zB"));
        zB->setGeometry(QRect(80, 20, 31, 23));
        zB->setAutoExclusive(true);
        camBox = new QComboBox(groupBox);
        camBox->setObjectName(QString::fromUtf8("camBox"));
        camBox->setEnabled(true);
        camBox->setGeometry(QRect(210, 20, 141, 31));
        gl_widget2 = new QWidget(groupBox);
        gl_widget2->setObjectName(QString::fromUtf8("gl_widget2"));
        gl_widget2->setGeometry(QRect(690, 30, 31, 16));
        sizePolicy2.setHeightForWidth(gl_widget2->sizePolicy().hasHeightForWidth());
        gl_widget2->setSizePolicy(sizePolicy2);
        gl_widget2->setMinimumSize(QSize(0, 0));
        savelButton = new QToolButton(groupBox);
        savelButton->setObjectName(QString::fromUtf8("savelButton"));
        savelButton->setGeometry(QRect(700, 20, 91, 31));
        reloadShadersBut = new QPushButton(groupBox);
        reloadShadersBut->setObjectName(QString::fromUtf8("reloadShadersBut"));
        reloadShadersBut->setGeometry(QRect(590, 20, 91, 31));

        gridLayout->addWidget(groupBox, 3, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(690, 690, 91, 31));
        inspectorTable = new QTableWidget(tab);
        if (inspectorTable->columnCount() < 3)
            inspectorTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        inspectorTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        inspectorTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        inspectorTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (inspectorTable->rowCount() < 2)
            inspectorTable->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        inspectorTable->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        inspectorTable->setVerticalHeaderItem(1, __qtablewidgetitem4);
        inspectorTable->setObjectName(QString::fromUtf8("inspectorTable"));
        inspectorTable->setGeometry(QRect(10, 130, 641, 401));
        inspectorTable->setTabKeyNavigation(true);
        inspectorTable->horizontalHeader()->setHighlightSections(true);
        inspectorTable->verticalHeader()->setVisible(false);
        objectBox = new QComboBox(tab);
        objectBox->setObjectName(QString::fromUtf8("objectBox"));
        objectBox->setEnabled(true);
        objectBox->setGeometry(QRect(500, -2, 141, 31));
        setVars = new QPushButton(tab);
        setVars->setObjectName(QString::fromUtf8("setVars"));
        setVars->setGeometry(QRect(690, 730, 91, 31));
        classLabel = new QLabel(tab);
        classLabel->setObjectName(QString::fromUtf8("classLabel"));
        classLabel->setGeometry(QRect(760, 10, 46, 13));
        addAttributes = new QCheckBox(tab);
        addAttributes->setObjectName(QString::fromUtf8("addAttributes"));
        addAttributes->setGeometry(QRect(130, 3, 161, 17));
        addAttributes->setFocusPolicy(Qt::NoFocus);
        addAttributes->setChecked(true);
        filter = new QCheckBox(tab);
        filter->setObjectName(QString::fromUtf8("filter"));
        filter->setGeometry(QRect(240, 3, 71, 17));
        filter->setFocusPolicy(Qt::NoFocus);
        objectList = new QListWidget(tab);
        new QListWidgetItem(objectList);
        new QListWidgetItem(objectList);
        new QListWidgetItem(objectList);
        new QListWidgetItem(objectList);
        new QListWidgetItem(objectList);
        new QListWidgetItem(objectList);
        new QListWidgetItem(objectList);
        new QListWidgetItem(objectList);
        objectList->setObjectName(QString::fromUtf8("objectList"));
        objectList->setGeometry(QRect(15, 30, 631, 71));
        objectList->setFlow(QListView::LeftToRight);
        objectList->setProperty("isWrapping", QVariant(true));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 730, 261, 171));
        label->setPixmap(QPixmap(QString::fromUtf8("screenshots/1.bmp")));
        label->setScaledContents(true);
        bHide = new QCheckBox(tab);
        bHide->setObjectName(QString::fromUtf8("bHide"));
        bHide->setGeometry(QRect(310, 3, 111, 17));
        bHide->setFocusPolicy(Qt::NoFocus);
        layoutWidget = new QWidget(tab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 101, 503, 19));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        bGroups = new QCheckBox(layoutWidget);
        bGroups->setObjectName(QString::fromUtf8("bGroups"));
        bGroups->setFocusPolicy(Qt::NoFocus);
        bGroups->setChecked(true);

        horizontalLayout->addWidget(bGroups);

        bVeh = new QCheckBox(layoutWidget);
        bVeh->setObjectName(QString::fromUtf8("bVeh"));
        bVeh->setFocusPolicy(Qt::NoFocus);
        bVeh->setChecked(true);

        horizontalLayout->addWidget(bVeh);

        bHelp = new QCheckBox(layoutWidget);
        bHelp->setObjectName(QString::fromUtf8("bHelp"));
        bHelp->setFocusPolicy(Qt::NoFocus);
        bHelp->setChecked(true);

        horizontalLayout->addWidget(bHelp);

        bHidden = new QCheckBox(layoutWidget);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(bHidden);

        bCurves = new QCheckBox(layoutWidget);
        bCurves->setObjectName(QString::fromUtf8("bCurves"));
        bCurves->setFocusPolicy(Qt::NoFocus);
        bCurves->setChecked(true);

        horizontalLayout->addWidget(bCurves);

        bPrim = new QCheckBox(layoutWidget);
        bPrim->setObjectName(QString::fromUtf8("bPrim"));
        bPrim->setFocusPolicy(Qt::NoFocus);
        bPrim->setChecked(true);

        horizontalLayout->addWidget(bPrim);

        bUngroop = new QCheckBox(layoutWidget);
        bUngroop->setObjectName(QString::fromUtf8("bUngroop"));
        bUngroop->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(bUngroop);

        bGeom = new QCheckBox(layoutWidget);
        bGeom->setObjectName(QString::fromUtf8("bGeom"));
        bGeom->setFocusPolicy(Qt::NoFocus);
        bGeom->setChecked(true);

        horizontalLayout->addWidget(bGeom);

        dialogBox = new QToolBox(tab);
        dialogBox->setObjectName(QString::fromUtf8("dialogBox"));
        dialogBox->setGeometry(QRect(10, 540, 641, 431));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 641, 404));
        dialogBox->addItem(page, QString::fromUtf8("Page 1"));
        objFilter = new QLineEdit(tab);
        objFilter->setObjectName(QString::fromUtf8("objFilter"));
        objFilter->setGeometry(QRect(14, 4, 110, 20));
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        layoutWidget_2 = new QWidget(tab_3);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 10, 105, 180));
        verticalLayout = new QVBoxLayout(layoutWidget_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        bColorRendering = new QCheckBox(layoutWidget_2);
        bColorRendering->setObjectName(QString::fromUtf8("bColorRendering"));
        bColorRendering->setFocusPolicy(Qt::NoFocus);
        bColorRendering->setChecked(true);

        verticalLayout->addWidget(bColorRendering);

        bdisable_timer2 = new QCheckBox(layoutWidget_2);
        bdisable_timer2->setObjectName(QString::fromUtf8("bdisable_timer2"));
        bdisable_timer2->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(bdisable_timer2);

        bWire = new QCheckBox(layoutWidget_2);
        bWire->setObjectName(QString::fromUtf8("bWire"));
        bWire->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(bWire);

        bShading = new QCheckBox(layoutWidget_2);
        bShading->setObjectName(QString::fromUtf8("bShading"));
        bShading->setFocusPolicy(Qt::NoFocus);
        bShading->setChecked(true);

        verticalLayout->addWidget(bShading);

        bdisable_timer1 = new QCheckBox(layoutWidget_2);
        bdisable_timer1->setObjectName(QString::fromUtf8("bdisable_timer1"));
        bdisable_timer1->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(bdisable_timer1);

        bChangedRenderMode = new QCheckBox(layoutWidget_2);
        bChangedRenderMode->setObjectName(QString::fromUtf8("bChangedRenderMode"));
        bChangedRenderMode->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(bChangedRenderMode);

        bNeedUpdVars = new QCheckBox(layoutWidget_2);
        bNeedUpdVars->setObjectName(QString::fromUtf8("bNeedUpdVars"));

        verticalLayout->addWidget(bNeedUpdVars);

        gridCB = new QCheckBox(layoutWidget_2);
        gridCB->setObjectName(QString::fromUtf8("gridCB"));
        gridCB->setChecked(true);

        verticalLayout->addWidget(gridCB);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        resetSim = new QToolButton(tab_2);
        resetSim->setObjectName(QString::fromUtf8("resetSim"));
        resetSim->setGeometry(QRect(20, 60, 91, 31));
        pushButton_3 = new QPushButton(tab_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 90, 91, 41));
        simMode = new QComboBox(tab_2);
        simMode->addItem(QString());
        simMode->addItem(QString());
        simMode->addItem(QString());
        simMode->setObjectName(QString::fromUtf8("simMode"));
        simMode->setEnabled(true);
        simMode->setGeometry(QRect(20, 140, 161, 31));
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 4, 181, 20));
        simButton = new QToolButton(tab_2);
        simButton->setObjectName(QString::fromUtf8("simButton"));
        simButton->setGeometry(QRect(20, 30, 91, 31));
        varsEdit = new QPlainTextEdit(tab_2);
        varsEdit->setObjectName(QString::fromUtf8("varsEdit"));
        varsEdit->setGeometry(QRect(20, 200, 471, 381));
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 180, 251, 17));
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        plotFrame = new QFrame(tab_4);
        plotFrame->setObjectName(QString::fromUtf8("plotFrame"));
        plotFrame->setGeometry(QRect(10, 560, 21, 21));
        plotFrame->setFrameShape(QFrame::StyledPanel);
        plotFrame->setFrameShadow(QFrame::Raised);
        tabWidget->addTab(tab_4, QString());

        gridLayout->addWidget(tabWidget, 0, 1, 6, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1672, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        dialogBox->setCurrentIndex(0);
        simMode->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "app6", nullptr));
        console->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "GroupBox", nullptr));
        moveButton->setText(QApplication::translate("MainWindow", "MOVE", nullptr));
        selButton->setText(QApplication::translate("MainWindow", "SEL", nullptr));
        pickButton->setText(QApplication::translate("MainWindow", "PICK", nullptr));
        gameButton->setText(QApplication::translate("MainWindow", "GAME", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "GroupBox", nullptr));
        xB->setText(QApplication::translate("MainWindow", "X", nullptr));
        yB->setText(QApplication::translate("MainWindow", "Y", nullptr));
        zB->setText(QApplication::translate("MainWindow", "Z", nullptr));
        savelButton->setText(QApplication::translate("MainWindow", "Save", nullptr));
        reloadShadersBut->setText(QApplication::translate("MainWindow", "Reload Shaders", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Update", nullptr));
        QTableWidgetItem *___qtablewidgetitem = inspectorTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = inspectorTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "val", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = inspectorTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "type", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = inspectorTable->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = inspectorTable->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
        setVars->setText(QApplication::translate("MainWindow", "Set VARS", nullptr));
        classLabel->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        addAttributes->setText(QApplication::translate("MainWindow", "\320\224\320\236\320\237 \320\220\320\242\320\242\320\240\320\230\320\221\320\243\320\242\320\253", nullptr));
        filter->setText(QApplication::translate("MainWindow", "\320\244\320\230\320\233\320\254\320\242\320\240", nullptr));

        const bool __sortingEnabled = objectList->isSortingEnabled();
        objectList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = objectList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = objectList->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = objectList->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = objectList->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = objectList->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = objectList->item(5);
        ___qlistwidgetitem5->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = objectList->item(6);
        ___qlistwidgetitem6->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        QListWidgetItem *___qlistwidgetitem7 = objectList->item(7);
        ___qlistwidgetitem7->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
        objectList->setSortingEnabled(__sortingEnabled);

        label->setText(QString());
        bHide->setText(QApplication::translate("MainWindow", "Hide Unselected", nullptr));
        bGroups->setText(QApplication::translate("MainWindow", "bGroups", nullptr));
        bVeh->setText(QApplication::translate("MainWindow", "bVeh", nullptr));
        bHelp->setText(QApplication::translate("MainWindow", "bHelp", nullptr));
        bHidden->setText(QApplication::translate("MainWindow", "bHidden", nullptr));
        bCurves->setText(QApplication::translate("MainWindow", "bCurves", nullptr));
        bPrim->setText(QApplication::translate("MainWindow", "bPrim", nullptr));
        bUngroop->setText(QApplication::translate("MainWindow", "bUngroop", nullptr));
        bGeom->setText(QApplication::translate("MainWindow", "bGeom", nullptr));
        dialogBox->setItemText(dialogBox->indexOf(page), QApplication::translate("MainWindow", "Page 1", nullptr));
        objFilter->setText(QApplication::translate("MainWindow", "car", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\320\236\320\261\321\214\320\265\320\272\321\202", nullptr));
        bColorRendering->setText(QApplication::translate("MainWindow", "bColorRendering", nullptr));
        bdisable_timer2->setText(QApplication::translate("MainWindow", "bdisable_timer2", nullptr));
        bWire->setText(QApplication::translate("MainWindow", "bWire", nullptr));
        bShading->setText(QApplication::translate("MainWindow", "bShading", nullptr));
        bdisable_timer1->setText(QApplication::translate("MainWindow", "bdisable_timer1", nullptr));
        bChangedRenderMode->setText(QApplication::translate("MainWindow", "bRenderMode", nullptr));
        bNeedUpdVars->setText(QApplication::translate("MainWindow", "bNeedUpdVars", nullptr));
        gridCB->setText(QApplication::translate("MainWindow", "Grid", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\320\244\320\273\320\260\320\263\320\270", nullptr));
        resetSim->setText(QApplication::translate("MainWindow", "Reset Sim", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "Quaternion anim", nullptr));
        simMode->setItemText(0, QApplication::translate("MainWindow", "\320\237\320\276\320\273\320\265\321\202 \320\277\320\276 \320\267\320\260\320\272\320\276\320\275\320\260\320\274", nullptr));
        simMode->setItemText(1, QApplication::translate("MainWindow", "\320\237\320\276\320\273\320\265\321\202 \320\277\320\276 \320\277\321\203\321\202\320\265\320\262\321\213\320\274 \321\202\320\276\321\207\320\272\320\260\320\274", nullptr));
        simMode->setItemText(2, QApplication::translate("MainWindow", "\320\237\320\276\320\273\320\265\321\202 \320\264\320\266\320\276\321\201\321\202\320\270\320\272\320\276\320\274", nullptr));

        label_3->setText(QApplication::translate("MainWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\201\320\270\320\274\321\203\320\273\321\217\321\202\320\276\321\200\320\276\320\274 \320\277\320\276\320\273\320\265\321\202\320\260", nullptr));
        simButton->setText(QApplication::translate("MainWindow", "Start Sim", nullptr));
        varsEdit->setPlainText(QApplication::translate("MainWindow", "--\321\202\320\260\320\261\320\273\320\270\321\206\320\260 \320\277\320\265\321\200\320\265\320\274\320\265\320\275\320\275\321\213\321\205\n"
"dt=0.1\n"
"p0=7,4,2\n"
"angles0=0,0,20\n"
"Polet { states=HorizFly,Climb,TurnLeft     times=5,5,5 }\n"
"FlyState0{  H=23 v= 10.0,0.0,0.0 }\n"
"FlyState1 { Hup = 20  vy = 20 ay = 1 }\n"
"FlyState2 { R = 100  A = 20,40,0 }\n"
"waypoint0{ ti=0   H=10  We=1 psi0=23 }\n"
"waypoint1{ ti=600 H=20  We=2 psi0=24 }\n"
"waypoint2{ ti=900 H=30  We=3 psi0=25 }\n"
"waypoint3{ ti=1200 H=40 We=5 psi0=26 }\n"
"", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\272\320\276\320\275\321\213 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262 \321\202\321\200\320\260\320\265\320\272\321\202\320\276\321\200\320\270\320\270", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\320\241\320\270\320\274\321\203\320\273\321\217\321\202\320\276\321\200", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "\320\223\321\200\320\260\321\204\320\270\320\272\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
