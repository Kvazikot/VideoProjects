#include <QMap>
#include <QVariant>
#include <QVector>
#include "print.h"
#include "vccpparser.h"
using namespace vccp;

QMap<QString, T> tagNameMap =
    {{"v",T(VIDEO_SOURCE_TAG,
       "video source tag(v) defines particular video \n"
       "source that need to insert at this time")},
     {"e",T(VFX_TAG,
      "effect tag(e) defines video effect or transition. \n"
      "May take parametres for effects in parenthess.")},
     {"c",T(TIMECODE_TAG,
      "timecode tag(c) defines exact time when particular effect apply or video source inserted")},
     {"p",T(PAUSE_TAG,
      "pause tag(p) defines pause beetwen paragraphs of text")},
     {"fadeout",T(FADEOUT_VFX,
      "fadeout(time_sec): fading transition effect. \n"
      "Parametres: time of effect in seconds(float). ")},
     {"paratext",T(PARATEXT_VFX,
      "paratext(text_color, background_color): create text layer on a opaque background with current paragraph text. \n"
      "Parametres: text_color, background_color can be in html format like #FF00FF. ")},
     {"cube3d",T(CUBE3D_VFX,
      "Creates 3d transition effect with rotating cube in space with video textures on each side")},
     };




Parser::Parser(QObject *parent) : QObject(parent)
{

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
/*
______________________________________________________
# Текст ролика. О вероятности реализации сценария аля "Терминатор".
Kvazikot
июль 2021

<c 00:00:50>
<e fade_out(12.2,color1,color2,23),para_text>
<v wheeler_1><e interlace_fx(40,640,480)>
В известном мысленном эксперименте Джона Уиллера
с двумя щелями и отложенном выборе был предложена[1] проверка гипотезы о том что прошлое можно изменить стирая информацию о наблюдении в будущем.
*/

ErrorCode Parser::parse_timecode(QString body)
{
    QStringList parts = body.split(":");
    if(parts.size() == 3)
    {
        bool ok1,ok2,ok3;
        int h = parts[0].toInt(&ok1);
        int m = parts[1].toInt(&ok2);
        int s = parts[2].toInt(&ok3);
        if(ok1 && ok2 && ok3)
            this->current_time_code.setHMS(h,m,s);
        else
            return ErrorCode::INT_PARSING_ERROR;
    }
    else
        return ErrorCode::FORMAT_ERROR;
}

ErrorCode Parser::parse_video_src(QString body, QStringList& video_list)
{
    video_list = body.split(",");
    if(video_list.size() == 0 )
        return ErrorCode::VIDEOLIST_PARSING_ERROR;
    return ErrorCode::NO_ERROR;
}

// example: fade_out(12.2,color1,color2,23)
ErrorCode Parser::parse_vfx_args(QString str, EffectType& type, QVector<QVariant>& out_args)
{
    //QRegExp rx("\((.)+\)");
    QRegExp rx("[(]([\\d\\.\\,\\w]+)[)]");
    int pos = 0;
    prn("text str : %s", str.toStdString().c_str());

    if( tagNameMap.find(str)!= tagNameMap.end())
        type = (EffectType)tagNameMap[str].code;

    if((pos = rx.indexIn(str, pos)) != -1)
    {
        print("regexp capture in brackets: " + rx.cap(1));
        print("before brackets: " + str.left(pos));
        QString vfx_name = str.left(pos);
        if( tagNameMap.find(vfx_name)!= tagNameMap.end())
            type = (EffectType)tagNameMap[vfx_name].code;
        else
            type = (EffectType)LAST_VFX;
    }
    else
        return NO_ARGS;

    QStringList  args = rx.cap(1).split(",");
    if( args.size() == 0 )
        return NO_ARGS;
    foreach(QString arg, args)
    {
        bool doubleOK, intOK;
        double valD = arg.toDouble(&doubleOK);
        int valI = arg.toInt(&intOK);
        if(intOK)
            out_args.append(QVariant(valI));
        else if(doubleOK)
            out_args.append(QVariant(valD));
        else
            out_args.append(arg);
    }

    return NO_ERROR;

}


// example: fade_out(12.2,color1,color2,23) para_text 3dcube
ErrorCode Parser::parse_effects(QString body)
{
    ErrorCode err;
    QVector<QVariant> args;
    print("body " + body);
    QStringList effects_list = body.split(" ");
    if(effects_list.size() == 0 )
        return ErrorCode::EFFECTSLIST_PARSING_ERROR;
    foreach (QString vfx_item, effects_list)
    {
        VfxObject* vfx_obj = new VfxObject();
        EffectType type;
        err = parse_vfx_args(vfx_item, type, args);
        vfx_obj->parametres = args;
        vfx_obj->type = type;
        prn("parse_vfx_args args.size() = %d", args.size());
        vfx_objects.append(vfx_obj);
    }
    return err;
}

ErrorCode Parser::parse(QString& in_text)
{
    tags.clear();
    findtags(in_text);
    Tag tag;
    ErrorCode err = ErrorCode::NO_ERROR;

    foreach (tag, tags)
    {
        QString body = in_text.mid(tag.start_index + 1, tag.end_index - tag.start_index - 1 );
        QStringList tokens = body.split(" ");
        if(tokens.size() < 2) continue;
        QString tagName = tokens[0];
        if( tagNameMap.find(tagName) != tagNameMap.end() )
        {
            T tag_info = tagNameMap[tagName];
            print(QString("tag: %1").arg(tagName));
            //print(QString("Parser tag docstring: %1").arg(tag_info.docstring));
            switch(tag_info.code)
            {
                case TIMECODE_TAG:                
                    err = parse_timecode(tokens[1]);
                    //prn("parse_timecode return %d, current_time_code = %s",
                    //    err, current_time_code.toString("h:m:s").toStdString().c_str());
                break;
                case VFX_TAG:
                    prn("vfx ");
                    tokens.removeAt(0);
                    err = parse_effects(tokens.join(" "));
                    prn("parse_effects return %d list size=%d", err, vfx_objects.size());

                break;
                case VIDEO_SOURCE_TAG:
                    QStringList video_list;
                    err = parse_video_src(tokens[1], video_list);
                    //prn("parse_video_src return %d list size=%d", err, video_list.size());
                break;

           }
        }
        else
        {
            prn("Parse_error at %d: Unknown tag ", tag.start_index);
            return ErrorCode::UNKNOWN_TAG;
        }

        prn(body.toStdString().c_str());

    }
    return err;
}
