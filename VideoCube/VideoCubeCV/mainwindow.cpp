#include "mainwindow.h"
#include "keybertwrapper.h"
#include "ui_mainwindow.h"
#include "print.h"
#include "vccpparser.h"

char doc11[] = "Supervised learning is the machine learning task of learning a function that \n \
maps an input to an output based on example input-output pairs. It infers a \n \
function from labeled training data consisting of a set of training examples. \n \
In supervised learning, each example is a pair consisting of an input object \n \
(typically a vector) and a desired output value (also called the supervisory signal).  \n \
A supervised learning algorithm analyzes the training data and produces an inferred function, \n \
which can be used for mapping new examples. An optimal scenario will allow for the \n \
algorithm to correctly determine the class labels for unseen instances. This requires \n \
the learning algorithm to generalize from the training data to unseen situations in a \n \
reasonable way (see inductive bias).";

char doc22[] = "В известном мысленном эксперименте Джона Уиллера \n \
с двумя щелями и отложенном выборе был предложена[1] проверка гипотезы о том что прошлое можно изменить стирая информацию о наблюдении в будущем. \n\
Неизвестна точная дата когда Уилеру пришла идея эксперимента с квантовым ластиком. \n \
Я сейчас не говорю о буквальном сценарии когда из машины времени выходят голые терминаторы. \n \
Это сделано ради потехи публики как и батарейки в \"матрице\", ведь голивудские фильмы расчитанны на массового зрителя. \n \
Я говорю о возможности предоставляемые квантовой теорией по стиранию информации в будущем, чтобы влиять на прошлое не имеет значения насколько отдаленное. \n \
Дельта t может быть 5 миллисекунд для обнаружения раковой клетки, а может быть возраст рождения вселенной. \n";


char code_template_py4[] = "from keybert import KeyBERT \n\
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
    QString code = QString(code_template_py4).arg(doc11);
    ui->console->setPlainText(code);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    event->accept();

}

void MainWindow::print(const QString &input)
{
    ui->console->setPlainText(input);

}

void MainWindow::hightlightKeywords(QString in_plain_text, QString& out_html_text, std::vector<std::string>& keywords)
{

}

void MainWindow::getPlainTextWithoutTags(QString& plain_text)
{
    QString text = ui->textEditor->toPlainText();
    vccp::Parser parser;
    parser.findtags(text);
    parser.getPlainTextWithoutTags(text);
    ui->textEditor->setText(text);
}

void MainWindow::on_markKeywordsButton_clicked()
{
    QString text, html_text;
    getPlainTextWithoutTags(text);
    return;
    KeyBERTWrapper bert_object;
    std::vector<std::string> keywords;
    if( bert_object.extractKeywordsFromText(doc11) > 0 )
    {
        bert_object.getKeywords(keywords);
        hightlightKeywords(text, html_text, keywords);
        ui->textEditor->setHtml(html_text);
    }
}

void MainWindow::on_runPyButton_clicked()
{
    KeyBERTWrapper bert_object;
    std::vector<std::string> keywords;
    if( bert_object.extractKeywordsFromText(doc11) > 0 )
    {
        bert_object.getKeywords(keywords);
    }
}

void MainWindow::on_findVideosButton_clicked()
{

}
