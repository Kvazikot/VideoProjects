#ifndef KEYBERTWRAPPER_H
#define KEYBERTWRAPPER_H

#include <QObject>
#include <map>

// 10 kb buffer
#define MAX_SYMBOLS (1024 * 1024 * 10)

class KeyBERTWrapper : public QObject
{
    Q_OBJECT
private:
    char* script;
    std::map<std::string, double>  keywordsMap;

public:
    explicit KeyBERTWrapper(QObject *parent = nullptr);

signals:

};

#endif // KEYBERTWRAPPER_H
