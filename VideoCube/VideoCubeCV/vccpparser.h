#ifndef VCCPPARSER_H
#define VCCPPARSER_H

#include <QObject>
#include <QTime>

namespace vccp
{

enum TagType
{
   VIDEO_SOURCE_TAG = 0,
   VFX_EFFECT_TAG = 1,
   TIMECODE_TAG = 2,
   PAUSE_TAG = 3,
   TEXT = 4 // text is also a tag :) hahaha but it is unoffisial tag
};

enum EffectType
{
    FADEOUT_VFX = 3000,
    PARATEXT_VFX = 3001,
    CUBE3D_VFX = 3002,
    GENERIC_VFX = 3003
};

enum ErrorCode
{
    NO_ERROR = 0,
    UNKNOWN_TAG = 5001,
    UNCLOSED_PARENTHESIS = 5002
};


struct Tag
{
    int start_index;
    int end_index;
    TagType type;
};

struct VfxObject
{
    QTime      time_code;
    EffectType type;
    std::vector<QVariant> parametres;
};

struct VideoSource
{
    QString name;
    QString url;
    QTime  time_code;
};

struct T
{
    int code;
    std::string docstring;
    T(){}
    T(int c, std::string doc)
        :code(c),docstring(doc)
    {}
};


class Parser : public QObject
{
    Q_OBJECT
    std::vector<Tag> tags;
    std::vector<VfxObject*> vfx_objects;
    std::vector<VideoSource*> video_sources;

public:    
    explicit Parser(QObject *parent = nullptr);
    void findtags(QString& text);
    void getPlainTextWithoutTags(QString& text);
    ErrorCode parse(QString& text);

signals:

};
}

#endif // VCCPPARSER_H
