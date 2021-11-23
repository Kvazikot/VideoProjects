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
    int extractKeywordsFromText(QString text);
    void print(const QString &input);
    void getKeywords(std::vector<std::string>& keywords_vector);


signals:

};

#endif // KEYBERTWRAPPER_H
