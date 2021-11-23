#ifndef VCCPPARSER_H
#define VCCPPARSER_H

#include <QObject>

namespace vccp
{

enum TagType
{
   VIDEO_SOURCE_TAG = 0,
   VFX_EFFECT_TAG = 1,
   TIMECODE_TAG = 2,
   PAUSE_TAG = 3
};

enum Effects
{
    FADEOUT_VFX = 3000,
    PARATEXT_VFX = 3001,
    CUBE3D_VFX = 3002
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
    std::map<int, char> codeToTagnameMap;
    std::map<std::string, int> tagNameToCodeMap;

public:    
    explicit Parser(QObject *parent = nullptr);
    void Init()
    {
        std::map<int, char> m = {{VIDEO_SOURCE_TAG, 'v'},
                                 {VFX_EFFECT_TAG, 'e'},
                                 {TIMECODE_TAG, 'c'},
                                 {7, 'd'}};
        codeToTagnameMap = m;
        std::map<std::string, int> m2 = {{"fadeout", FADEOUT_VFX},
                                 {"paratext", PARATEXT_VFX},
                                 {"cube3d", CUBE3D_VFX},
                                 {"v", VIDEO_SOURCE_TAG},
                                 {"e", VFX_EFFECT_TAG},
                                 {"c", TIMECODE_TAG},
                                 {"p", PAUSE_TAG},
                                 };

        tagNameToCodeMap = m2;


    }
    void findtags(QString& text);
    void getPlainTextWithoutTags(QString& text);
    void parse(QString& text);

signals:

};
}

#endif // VCCPPARSER_H
