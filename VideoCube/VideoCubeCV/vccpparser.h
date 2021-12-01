#ifndef VCCPPARSER_H
#define VCCPPARSER_H

#include <QObject>
#include <QVector>
#include <QTime>

namespace vccp
{

enum TagType
{
   VIDEO_SOURCE_TAG = 0,
   VFX_TAG = 1,
   TIMECODE_TAG = 2,
   PAUSE_TAG = 3,
   TEXT = 4 // text is also a tag :) hahaha but it is unoffisial tag
};

enum EffectType
{
    FADEOUT_VFX = 30000,
    PARATEXT_VFX = 30001,
    CUBE3D_VFX = 30002,
    GENERIC_VFX = 30003,
    LAST_VFX = 31000
};

enum ErrorCode
{
    NO_ERROR = 0,
    UNKNOWN_TAG = 5001,
    FORMAT_ERROR = 5003,
    INT_PARSING_ERROR = 5004,
    UNCLOSED_PARENTHESIS = 5002,
    VIDEOLIST_PARSING_ERROR = 5006,
    EFFECTSLIST_PARSING_ERROR = 5007,
    NO_ARGS = 5008,
    UNKNOW_VFX_NAME = 5009
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
    QVector<QVariant> parametres;
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
    QString docstring;
    QString plugin_name;
    T(){}
    T(int c, QString doc)
        :code(c),docstring(doc),plugin_name("internal")
    {
    }
};


class Parser : public QObject
{
    Q_OBJECT
    QTime      current_time_code;
    std::vector<Tag> tags;
    QVector<VfxObject*> vfx_objects;
    std::vector<VideoSource*> video_sources;

public:    
    explicit Parser(QObject *parent = nullptr);
    void findtags(QString& text);
    void getPlainTextWithoutTags(QString& text);
    ErrorCode parse(QString& text);
    ErrorCode parse_timecode(QString body);
    ErrorCode parse_effects(QString body);
    ErrorCode parse_video_src(QString body, QStringList& video_list);
    ErrorCode parse_vfx_args(QString str, EffectType& type, QVector<QVariant>& out_args);
signals:

};
}

#endif // VCCPPARSER_H
