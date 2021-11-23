#ifndef VCCPPARSER_H
#define VCCPPARSER_H

#include <QObject>

namespace vccp
{

enum TagType
{
   VIDEO_SOURCE_TAG = 0,
   VFX_EFFECT_TAG = 1,
   TIMECODE_TAG = 2
};


struct Tag
{
    int start_index;
    int end_index;
    TagType type;

};


class Parser : public QObject
{
    Q_OBJECT
    std::vector<Tag> tags;
public:    
    explicit Parser(QObject *parent = nullptr);
    void findtags(QString& text);
    void getPlainTextWithoutTags(QString& text);
    void parse(QString& text);

signals:

};
}

#endif // VCCPPARSER_H
