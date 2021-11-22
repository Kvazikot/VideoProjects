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
#include <iostream>

#include "keybertwrapper.h"

char doc[] = "¬ известном мысленном эксперименте ƒжона ”иллера \
с двум€ щел€ми и отложенном выборе был предложена[1] проверка гипотезы о том что прошлое можно изменить стира€ информацию о наблюдении в будущем.\
Ќеизвестна точна€ дата когда ”илеру пришла иде€ эксперимента с квантовым ластиком. \
я сейчас не говорю о буквальном сценарии когда из машины времени выход€т голые терминаторы. \
Ёто сделано ради потехи публики как и батарейки в \"матрице\", ведь голивудские фильмы расчитанны на массового зрител€. \
я говорю о возможности предоставл€емые квантовой теорией по стиранию информации в будущем, чтобы вли€ть на прошлое не имеет значени€ насколько отдаленное. \
ƒельта t может быть 5 миллисекунд дл€ обнаружени€ раковой клетки, а может быть возраст рождени€ вселенной.";

std::string code_py =
R"(
def test():
    return 'hi';
)";

int ParsePyList(std::string code, std::map<std::string, double>& result_map)
{
    PyObject *dict = NULL,
             *run_result = NULL,
             *pList = NULL;

    dict = PyDict_New();
    if (!dict) goto done;
    code = "list=" + code;
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
       return;
    }
*/

    const Py_ssize_t tuple_length = 2;
    const unsigned some_limit = 4;

    PyObject *my_list = PyList_New(0);
    if(my_list == NULL) {
        // ...
    }

    for(unsigned i = 0; i < some_limit; i++) {
        PyObject *the_tuple = PyTuple_New(tuple_length);
        if(the_tuple == NULL) {
            // ...
        }

        PyObject *kw = PyUnicode_DecodeASCII("supervised", strlen("supervised"), NULL);
        PyObject *the_object = PyFloat_FromDouble(rand());
        if(the_object == NULL) {
            // ...
        }

        PyTuple_SET_ITEM(the_tuple, 0, kw);
        PyTuple_SET_ITEM(the_tuple, 1, the_object);


        if(PyList_Append(my_list, the_tuple) == -1) {
            // ...
        }
    }

    PyObject *pList = my_list;//PyObject_CallMethod(pInstance, "extract_keywords", doc);
    int result = ParsePyList("[('supervised', 0.6676), ('labeled', 0.4896), ('learning', 0.4813), ('training', 0.4134), ('labels', 0.3947)]", keywordsMap);
    if( result )
    {
       qDebug() << "result of code execution!\n" ;
       for(auto i=keywordsMap.begin(); i != keywordsMap.end(); i++)
          qDebug() << (*i).first.c_str();
    }
    return;

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
        PyObject_Print(kw, stderr, 0);
        qDebug() << " ----------------- {kw,prob) = " << first << " " << second;
        PyObject_Print(probability, stderr, 0);
        PyUnicode_FromObject(kw);

        //qDebug() << "kw " << kw << "probability " << probability;
    }


     Py_Finalize();
}
