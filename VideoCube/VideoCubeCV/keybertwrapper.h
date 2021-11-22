#ifndef KEYBERTWRAPPER_H
#define KEYBERTWRAPPER_H

#include <QObject>

// 10 kb buffer
#define MAX_SYMBOLS (1024 * 1024 * 10)

class KeyBERTWrapper : public QObject
{
    Q_OBJECT
private:
    char* script;

public:
    explicit KeyBERTWrapper(QObject *parent = nullptr);

signals:

};

#endif // KEYBERTWRAPPER_H
