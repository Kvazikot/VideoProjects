#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showEvent(QShowEvent *event);
    void hightlightKeywords(QString in_plain_text, QString& out_html_text, std::vector<std::string>& keywords);
    void getPlainTextWithoutTags(QString& plain_text);
    ~MainWindow();

private slots:
    void on_markKeywordsButton_clicked();

    void print(const QString &input);

    void on_runPyButton_clicked();

    void on_findVideosButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
