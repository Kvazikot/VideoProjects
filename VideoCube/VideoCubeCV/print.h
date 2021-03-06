#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QString>

class ConsolePrn : public QObject
{
	Q_OBJECT
public:
    int   nHooks;
    double time_last_prn;
    ConsolePrn();
    ConsolePrn(const ConsolePrn& cam):QObject()    { cam;    }

    void prn(char* str);
    void prn2(QString str);
    void log(char* str);
signals:
	void print_sig(const QString & str);
    void print_status_sig(const QString & str);
};

void init_prn();
void map_to_prn(QObject* w, int slot_n=0);
void prn(const char* fmt,...);
void prn_skip(const char* fmt,...);
void log_status(const char* fmt,...);
#define mu_printf prn
void prn2(QString str);
void print(QString str);
void unmap_from_prn(QObject* w);

#endif
