﻿#ifndef KEYBERTWRAPPER_H
#define KEYBERTWRAPPER_H

#include <QObject>
#include <QString>
#include <map>
#include <sstream>

// 10 kb buffer
#define MAX_SYMBOLS (1024 * 1024 * 10)

class KeyBERTWrapper : public QObject
{
    Q_OBJECT
private:
    char* script;
    std::map<std::string, double>  keywordsMap;
    int executionError;
    QString err_string;
    std::stringstream buffer;
    std::streambuf* sbuf;


public:
    explicit KeyBERTWrapper(QObject *parent = nullptr);
    int ParsePyList(std::string code, std::map<std::string, double>& result_map);
    int extractKeywordsFromText(QString text);
    void getKeywords(std::vector<QString>& keywords_vector);


signals:

};

#endif // KEYBERTWRAPPER_H
