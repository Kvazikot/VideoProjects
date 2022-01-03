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
#include <QFile>
#include <QProcess>
#include <QApplication>
#include <string>
#include <iostream>
#include <sstream>
#include "stdout_module.h"
#include "print.h"
#include "windows.h"

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
kw_model = KeyBERT() \n\
list = kw_model.extract_keywords(doc) \n\
print(list)\
";

static PyObject* redirection_stdoutredirect(PyObject *self, PyObject *args)
{
    const char *string;
    if(!PyArg_ParseTuple(args, "s", &string))
        return NULL;
    //pass string onto somewhere
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef RedirectionMethods[] = {
{"stdoutredirect", redirection_stdoutredirect, METH_VARARGS,
        "stdout redirection helper"}, {NULL, NULL, 0, NULL} };

int KeyBERTWrapper::ParsePyList(std::string code, std::map<std::string, double>& result_map)
{
    PyObject *dict = NULL,
             *run_result = NULL,
             *pList = NULL;

    dict = PyDict_New();
    executionError = 0;
    if (!dict)  return -1;
    run_result = PyRun_String(code.c_str(), Py_file_input, dict, dict);
    if (!run_result)
    {
        PyErr_Print();
        executionError = 1;
        return 1;

        PyObject *sysmodule;
        PyObject *pystdout;
        PyObject *pystdoutdata;
        char string1[10000];
        char* string = &string1[0];
        sysmodule = PyImport_ImportModule("sys");
        pystdout = PyObject_GetAttrString(sysmodule, "stdout");
        pystdoutdata = PyObject_GetAttrString(pystdout, "data");
        if(pystdoutdata == NULL )
        {
            prn("no data!");
            return -1;
        }
        //return -1;

        //PyObject * obj = PyUnicode_AsEncodedString(pystdoutdata,"UTF8",NULL);
        Py_ssize_t len = 0;
        const char* str =  PyUnicode_AsUTF8AndSize(pystdoutdata, &len);
        //prn(str);
        //print(string);

        //QString out(QProcess::readAllStandardOutput());
        //print(out);
        return -1;

        std::streambuf * pbuf = buffer.rdbuf();
        std::streamsize size = pbuf->pubseekoff(0,buffer.end);
        pbuf->pubseekoff(0,buffer.beg);       // rewind
        auto contents = new char [size];
        pbuf->sgetn (contents,size);
        prn(contents);
        prn("size buffer %d", (int)size);

        // When done redirect cout to its old self
        std::cout.rdbuf(sbuf);

        err_string = contents;

        return -1;
    }
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

    }

    // When done redirect cout to its old self
    std::cout.rdbuf(sbuf);

    return 1;

}
static struct PyModuleDef cModPyDem =
{
    PyModuleDef_HEAD_INIT,
    "redirection", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    RedirectionMethods
};

static struct PyModuleDef moduledef = {
     PyModuleDef_HEAD_INIT,
     "redirection",     /* m_name */
     "This is a module",  /* m_doc */
     -1,                  /* m_size */
     RedirectionMethods,    /* m_methods */
     NULL,                /* m_reload */
     NULL,                /* m_traverse */
     NULL,                /* m_clear */
     NULL,                /* m_free */
 };

KeyBERTWrapper::KeyBERTWrapper(QObject *parent)
    : QObject(parent)
{
    //in main...
    //Py_Initialize();
    PyInit_hello();
    auto hellomodule = PyImport_ImportModule("hello");
    prn("PyModule_Create= %d",PyModule_Create(&moduledef));
    PyRun_SimpleString("hello.hello()\n\
import sys\n\
class StdoutCatcher:\n\
    def write(self, stuff):\n\
        redirection.stdoutredirect(stuff)\n\
sys.stdout = StdoutCatcher()\n\
print(\"STDOUT!!!!\")\n\
");


//    Py_Finalize();

}

void KeyBERTWrapper::getKeywords(std::vector<QString>& keywords_vector)
{
    for(auto i=keywordsMap.begin(); i != keywordsMap.end(); i++)
       keywords_vector.push_back((*i).first.c_str());
}

int KeyBERTWrapper::extractKeywordsFromText(QString text)
{

    // Save cout's buffer here
    //sbuf = std::cerr.rdbuf();

    // Redirect cout to our stringstream buffer or any other ostream
    //std::cerr.rdbuf(buffer.rdbuf());

    script = (char*)malloc(MAX_SYMBOLS);
    PyObject *pName, *pModule, *pDict, *pClass, *pInstance;

    // Initialize the Python interpreter
    //Py_Initialize();
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
    //if(msb==0xba && lsb ==0xb8 && lsb > 2 && msb > 1)
    {
        if(text.size() < MAX_SYMBOLS)
        {
            QString code = QString(code_template_py4).arg(text);
            code = "# -*- coding: UTF-8 -*-\n" + code;
            //print("=====================");
            //print(code);
            //print("=====================");

            int result = ParsePyList(code.toStdString(), keywordsMap);
            if( result )
            {
               print("keywords list:");
               print("======================================");
               for(auto i=keywordsMap.begin(); i != keywordsMap.end(); i++)
                  print(QString((*i).first.c_str()) + "\t" +  QString::number((*i).second) + "\n");
               print("======================================");
            }
        }
    }

    Py_Finalize();
    return keywordsMap.size();
}
