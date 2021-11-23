#include "print.h"
#include "vccpparser.h"
using namespace vccp;

/*
______________________________________________________
# Текст ролика. О вероятности реализации сценария аля "Терминатор".
Kvazikot
июль 2021

<00:00:50>
<v wheeler_1><e fade_out,para_text>
В известном мысленном эксперименте Джона Уиллера
с двумя щелями и отложенном выборе был предложена[1] проверка гипотезы о том что прошлое можно изменить стирая информацию о наблюдении в будущем.
*/


Parser::Parser(QObject *parent) : QObject(parent)
{
    Init();
}

void Parser::getPlainTextWithoutTags(QString& in_text)
{
    Tag tag;
    int start = 0;
    QString out_text;
    foreach (tag, tags)
    {
        //if((tag.start_index - start) > 1)
        {
            QString str = in_text.mid(start, tag.start_index - start );
            start = tag.end_index + 1;
            out_text+=str;
        }
    }
    in_text = out_text;
}

void Parser::findtags(QString& text)
{
    bool newTag = false;
    Tag current_tag;
    for(int i=0; i < text.size(); i++)
    {
        QChar c = text[i];
        if( c == '<' )
        {
            newTag = true;
            current_tag.start_index = i;
        }

        if( c == '>' &&  newTag)
        {
            current_tag.end_index = i;
            tags.push_back(current_tag);
            newTag = false;
        }
    }
}

void Parser::parse(QString& in_text)
{
    tags.clear();
    findtags(in_text);
    Tag tag;
    foreach (tag, tags)
    {
        QString body = in_text.mid(tag.start_index + 1, tag.end_index - tag.start_index - 1 );
        std::string tagName = body.mid(0,1).toStdString();
        prn("tag %s", tagName.c_str());
        if( tagNameToCodeMap.find(tagName) != tagNameToCodeMap.end() )
        {
            prn("Parser found tag %s", tagName.c_str());
        }
        prn(body.toStdString().c_str());

    }
}
