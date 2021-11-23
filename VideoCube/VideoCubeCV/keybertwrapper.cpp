/*
+ - - - + - + - -
+ - + - + copyright by Vladimir Baranov (Kvazikot)  <br>
+ - + - + email: vsbaranov83@gmail.com  <br>
+ - + - + github: http://github.com/Kvazikot/VideoProjects <br>
                          )            (
                         /(   (\___/)  )\
                        ( #)  \ ('')| ( #
                         ||___c\  > '__||
                         ||**** ),_/ **'|
                   .__   |'* ___| |___*'|
                    \_\  |' (    ~   ,)'|
                     ((  |' /(.  '  .)\ |
                      \\_|_/ <_ _____> \______________
                       /   '-, \   / ,-'      ______  \
              b'ger   /      (//   \\)     __/     /   \
                                          './_____/

*/

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <QDebug>
#include <string>
#include <iostream>
#include "print.h"

#include "keybertwrapper.h"


static char doc11[] = "Supervised learning is the machine learning task of learning a function that \n \
maps an input to an output based on example input-output pairs. It infers a \n \
function from labeled training data consisting of a set of training examples. \n \
In supervised learning, each example is a pair consisting of an input object \n \
(typically a vector) and a desired output value (also called the supervisory signal).  \n \
A supervised learning algorithm analyzes the training data and produces an inferred function, \n \
which can be used for mapping new examples. An optimal scenario will allow for the \n \
algorithm to correctly determine the class labels for unseen instances. This requires \n \
the learning algorithm to generalize from the training data to unseen situations in a \n \
reasonable way (see inductive bias).";

static char doc22[] = "В известном мысленном эксперименте Джона Уиллера \n \
с двумя щелями и отложенном выборе был предложена[1] проверка гипотезы о том что прошлое можно изменить стирая информацию о наблюдении в будущем. \n\
Неизвестна точная дата когда Уилеру пришла идея эксперимента с квантовым ластиком. \n \
Я сейчас не говорю о буквальном сценарии когда из машины времени выходят голые терминаторы. \n \
Это сделано ради потехи публики как и батарейки в \"матрице\", ведь голивудские фильмы расчитанны на массового зрителя. \n \
Я говорю о возможности предоставляемые квантовой теорией по стиранию информации в будущем, чтобы влиять на прошлое не имеет значения насколько отдаленное. \n \
Дельта t может быть 5 миллисекунд для обнаружения раковой клетки, а может быть возраст рождения вселенной. \n";


static char code_template_py4[] = "from keybert import KeyBERT \n\
doc = \"\"\" \n\
%1 \n\
      \"\"\" \n\
doc = 'Supervised learning is the machine learning task of learning a function that' \n\
kw_model = KeyBERT() \n\
list = kw_model.extract_keywords(doc) \n\
print(list)\
";


int ParsePyList(std::string code, std::map<std::string, double>& result_map)
{
    PyObject *dict = NULL,
             *run_result = NULL,
             *pList = NULL;

    dict = PyDict_New();
    if (!dict)  return -1;
    run_result = PyRun_String(code.c_str(), Py_file_input, dict, dict);
    if (!run_result)  return -1;
    pList = PyDict_GetItemString(dict,"list");
    int n_keywords = PyList_Size(pList);

    for(int i=0; i < n_keywords; i++)
    {
        PyObject* item = PyList_GetItem(pList, i);
        PyObject* kw = PyTuple_GetItem(item, 0);
        //PyLong_FromLong()
        PyObject* probability = PyTuple_GetItem(item, 1);
        double second = PyFloat_AsDouble(probability);
        Py_ssize_t len = 0;
        const char* first =  PyUnicode_AsUTF8AndSize(kw, &len);
        std::string key = first;
        result_map[key] = second;
        qDebug() << key.c_str();
    }

    return 1;

}


KeyBERTWrapper::KeyBERTWrapper(QObject *parent)
    : QObject(parent)
{

}

int KeyBERTWrapper::getKeywordsFromText(QString text)
{
    script = (char*)malloc(MAX_SYMBOLS);
    PyObject *pName, *pModule, *pDict, *pClass, *pInstance;

    // Initialize the Python interpreter
    Py_Initialize();
    // Calculating checksum of code
    auto data = QByteArray::fromRawData(reinterpret_cast<const char *>(code_template_py4), sizeof(code_template_py4));
    quint16 sum = 0;
    int i = 0;
    int len = data.length() - 3;
    for (i = 0; i < len; ++i)
        sum += quint8(data.at(i));
    sum = -sum;
    qDebug("sum = %04x", sum);
    quint8 msb = sum >> 8;
    quint8 lsb = sum & 0xFF;
    qDebug("MSB = %02x, LSB = %02x", msb, lsb);
    if(msb==0xba && lsb ==0xb8 && lsb > 2 && msb > 1)
    {
        QString code = QString(code_template_py4).arg(text);

        int result = ParsePyList(code.toStdString(), keywordsMap);
        if( result )
        {
           qDebug() << "result of code execution!\n" ;
           for(auto i=keywordsMap.begin(); i != keywordsMap.end(); i++)
              qDebug() << (*i).first.c_str();
        }
    }

    Py_Finalize();
    return keywordsMap.size();
}
