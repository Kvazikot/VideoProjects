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
    if (!dict) goto done;
    run_result = PyRun_String(code.c_str(), Py_file_input, dict, dict);
    if (!run_result) goto done;
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


    //qDebug() << "n_keywords = " << n_keywords;
    return 1;
    done:
        return -1;

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
/*
    pName = PyUnicode_FromString((char*)"keybert");
    pModule = PyImport_Import(pName);
    //pFunc = PyObject_GetAttrString(pModule, (char*)"KeyBERT");
    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);
    // Build the name of a callable class
    pClass = PyDict_GetItemString(pDict, "KeyBERT");

    // Create an instance of the class
    if ( PyCallable_Check(pClass) )
    {
       pInstance = PyObject_CallObject(pClass, NULL);
    }
    else
    {
       qDebug() << "Cannot instantiate the Python class";
       return 0;
    }
    PyObject* arg1 = Py_BuildValue("(s)", doc1);
    PyObject *keywords = PyDict_New();
    PyDict_SetItemString(keywords, "somearg", Py_True);
    //PyObject* pList = PyObject_CallMethod(pInstance, "extract_keywords", NULL, arg1);
    PyObject* pList = PyObject_Call(PyObject_GetAttrString(pInstance, "extract_keywords"), arg1, keywords);

    if(pList != NULL)
    {
        int n_keywords = PyList_Size(pList);
        qDebug() << n_keywords;
    }
*/
    QString code = QString(code_template_py4).arg(text);

    int result = ParsePyList(code.toStdString(), keywordsMap);
    if( result )
    {
       qDebug() << "result of code execution!\n" ;
       for(auto i=keywordsMap.begin(); i != keywordsMap.end(); i++)
          qDebug() << (*i).first.c_str();
    }
/*
    int result = ParsePyList("[('supervised', 0.6676), ('labeled', 0.4896), ('learning', 0.4813), ('training', 0.4134), ('labels', 0.3947)]", keywordsMap);
    if( result )
    {
       qDebug() << "result of code execution!\n" ;
       for(auto i=keywordsMap.begin(); i != keywordsMap.end(); i++)
          qDebug() << (*i).first.c_str();
    }


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
        keywordsMap[key] = second;
        PyObject_Print(kw, stderr, 0);
        qDebug() << " ----------------- {kw,prob) = " << first << " " << second;
        PyObject_Print(probability, stderr, 0);
        PyUnicode_FromObject(kw);

        //qDebug() << "kw " << kw << "probability " << probability;
    }
*/
     Py_Finalize();
     return keywordsMap.size();
}
