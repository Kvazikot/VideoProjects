/*
+ - - - + - + - -
+ - + - + copyright by Vladimir Baranov (Kvazikot)  <br>
+ - + - + email: vsbaranov83@gmail.com  <br>
+ - + - + github: http://github.com/Kvazikot/VideoProjects <br>
                          )            (
                         /(   (\___/)  )\
                        ( #)  \ ('')| ( #
                         ||___c\  > '__||
                         ||**** ),_/ **'|
                   .__   |'* ___| |___*'|
                    \_\  |' (    ~   ,)'|
                     ((  |' /(.  '  .)\ |
                      \\_|_/ <_ _____> \______________
                       /   '-, \   / ,-'      ______  \
              b'ger   /      (//   \\)     __/     /   \
                                          './_____/

*/

#include <shared.h>
#include <windows.h>
#include <string>
#include <QDebug>
#include <QFile>
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <tchar.h>
#include "print.h"

//For shared memory with Unity
const int HEADER_SIZE = 10;
const int BUFFER_SIZE = 1980 * 1080 * 4 + HEADER_SIZE * 4; //HD FRAME ARGB32 + HEADER SIZE
TCHAR szMapName[] = TEXT("UnityFileMappingObject");
TCHAR szCountName[] = TEXT("UnityFileCountObject");
signed int sharedMemOut[BUFFER_SIZE];

LPCTSTR pBuf;
HANDLE hMapFile;

Shared::Shared()
{
    ZeroHeader();
}

void Shared::ZeroHeader()
{
    memset(sharedMemOut, 0, HEADER_SIZE * 4 );
}

int Shared::Init()
{

    //     Request shared memory from the OS to share with Unity
    hMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,    // use paging file
                NULL,                    // default security
                PAGE_READWRITE,          // read/write access
                0,                       // maximum object size (high-order DWORD)
                BUFFER_SIZE,                // maximum object size (low-order DWORD)
                szMapName);                 // name of mapping object

    if (hMapFile == NULL)
    {
        prn("Could not create file mapping object (%d).\n", GetLastError());
        return -1;
    }
    pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
                                 FILE_MAP_ALL_ACCESS, // read/write permission
                                 0,
                                 0,
                                 BUFFER_SIZE);

    if (pBuf == NULL)
    {
        prn("Could not map view of file (%d).\n", GetLastError() );
        CloseHandle(hMapFile);
        return -1;
    }


    return 0;
}

void Shared::writeJson()
{
    QString json = "{'uuid':'01701412-0f44-11ec-a466-38b1dbc8b668','size':0,'texture_id':1262813792,'width':128,'height':128,'format':5,'job':0}";
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject data = document.object();
    QString call = data["call"].toString();
}

void Shared::writeImage(QString filename, int w, int h)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QByteArray buffer = file.readAll();
    int size = buffer.size();
    png_datasize =  size;
    sharedMemOut[0] = (int)count;
    sharedMemOut[1] = size;
    sharedMemOut[2] = -1;
    sharedMemOut[3] = w;
    sharedMemOut[4] = h;
    sharedMemOut[5] = 0; //format
    sharedMemOut[6] = 5;
    sharedMemOut[7] = 6;

    char *src = buffer.data();
    char *dst = (char*)(sharedMemOut);
    dst = dst + HEADER_SIZE * 4;
    while (size!=0) {
        *dst = *src;
        --size; dst++; src++;
    }
    prn("Shared::writeImage");
    dst = (char*)(sharedMemOut);
    dst = dst + HEADER_SIZE * 4;
    //*dst = 0x44; dst++;
    //*dst = 0x55; dst++;
    //*dst = 0x66; dst++;


}

void Shared::writeImage(QImage& image)
{
    // load into shared memory
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out.setVersion(0);



    prn("stream version %d", out.version());
    // resize image for testing purpuose
    image = image.scaled(128, 128);
    out << image;
    int size = buffer.size();
    png_datasize =  size;
    sharedMemOut[0] = (int)count;
    sharedMemOut[1] = size;
    sharedMemOut[2] = -1;
    sharedMemOut[3] = image.width();
    sharedMemOut[4] = image.height();
    sharedMemOut[5] = 0; //format
    sharedMemOut[6] = 5;
    sharedMemOut[7] = 6;

    char *src = (char*)buffer.data().data();
    char *dst = (char*)(sharedMemOut);
    dst = dst + HEADER_SIZE * 4;
    while (size!=0) {
        *dst = *src;
        --size; dst++; src++;
    }
}

int Shared::Update()
{
    sharedMemOut[0] = (int)count;
    //sharedMemOut[1] = 1;
    //sharedMemOut[2] = 2;
    //sharedMemOut[3] = 3;
    //sharedMemOut[4] = rand();
    //sharedMemOut[5] = 5;
    //sharedMemOut[6] = 6;
    //header
    CopyMemory((PVOID)pBuf, sharedMemOut, (HEADER_SIZE * sizeof(signed int) + png_datasize));
    // image pixels
    //CopyMemory((PVOID)(pBuf + HEADER_SIZE * 4), &sharedMemOut[HEADER_SIZE], sharedMemOut[1]);
    log_status("count %d writed %d", count++, (HEADER_SIZE * sizeof(signed int) + png_datasize));
    return count;
}
