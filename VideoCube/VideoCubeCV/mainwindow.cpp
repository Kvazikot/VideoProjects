#include "mainwindow.h"
#include "keybertwrapper.h"
#include "ui_mainwindow.h"
#include "print.h"
#include "vccpparser.h"

static char code_template_py4[] = "from keybert import KeyBERT \n\
doc = \"\"\" \n\
%1 \n\
      \"\"\" \n\
doc = 'Supervised learning is the machine learning task of learning a function that' \n\
kw_model = KeyBERT() \n\
list = kw_model.extract_keywords(doc) \n\
print(list)\
";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    map_to_prn(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    event->accept();
    on_markKeywordsButton_clicked();
}

void MainWindow::print(const QString &input)
{
    ui->console->appendPlainText(input);
    ui->console->moveCursor(QTextCursor::End);
}

void MainWindow::hightlightKeywords(QString in_plain_text, QString& out_html_text, std::vector<QString>& keywords)
{
    QStringList words = in_plain_text.split(" ");
    out_html_text = in_plain_text;
    int offset = 0;
    foreach(QString word, keywords)
    {
        int index = 1;
        while(index > 0)
        {
            index = out_html_text.indexOf(word, index+1);
            if( index == -1) break;
            QString insertion = "<font style=\"color:red\">";
            out_html_text.insert(index, insertion);
            index+=word.size()+insertion.size();
            insertion = "</font>";
            out_html_text.insert(index, insertion);
            index+=insertion.size();

        }

    }

    return;
}

void MainWindow::getPlainTextWithoutTags(QString& plain_text)
{
    QString text = ui->textEditor->toPlainText();
    vccp::Parser parser;
    parser.parse(text);
    parser.findtags(text);
    parser.getPlainTextWithoutTags(text);
    plain_text = text;
}

void MainWindow::on_markKeywordsButton_clicked()
{
    QString text, html_text;
    std::vector<QString> keywords;

    ui->console->clear();
    getPlainTextWithoutTags(text);
    ui->textEditor->setText(text);

    //hightlightKeywords(text, html_text, keywords);
    //ui->textEditor->setHtml(html_text);
    //return;
    KeyBERTWrapper bert_object;

    if( bert_object.extractKeywordsFromText(text) > 0 )
    {
        bert_object.getKeywords(keywords);
        hightlightKeywords(text, html_text, keywords);
        ui->textEditor->setHtml(html_text);
    }
}

void MainWindow::on_runPyButton_clicked()
{
}

void MainWindow::on_findVideosButton_clicked()
{

}
