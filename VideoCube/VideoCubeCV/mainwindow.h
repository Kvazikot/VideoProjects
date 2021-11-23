#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showEvent(QShowEvent *event);
    ~MainWindow();

private slots:
    void on_markKeywordsButton_clicked();

    void print(const QString &input);

    void on_runPyButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
