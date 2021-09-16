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
#include <tchar.h>


//For shared memory with Unity
#define BUF_SIZE 256
TCHAR szMapName[] = TEXT("UnityFileMappingObject");
TCHAR szCountName[] = TEXT("UnityFileCountObject");
signed int sharedMemOut[7];

LPCTSTR pBuf;
HANDLE hMapFile;

Shared::Shared()
{

}

int Shared::Init()
{

    //     Request shared memory from the OS to share with Unity
    hMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,    // use paging file
                NULL,                    // default security
                PAGE_READWRITE,          // read/write access
                0,                       // maximum object size (high-order DWORD)
                BUF_SIZE,                // maximum object size (low-order DWORD)
                szMapName);                 // name of mapping object

    if (hMapFile == NULL)
    {
        qDebug("Could not create file mapping object (%d).\n", GetLastError());
        return -1;
    }
    pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
                                 FILE_MAP_ALL_ACCESS, // read/write permission
                                 0,
                                 0,
                                 BUF_SIZE);

    if (pBuf == NULL)
    {
        qDebug("Could not map view of file (%d).\n", GetLastError() );
        CloseHandle(hMapFile);
        return -1;
    }
    return 0;
}

int Shared::Update()
{
    int count = 0;
    sharedMemOut[0] = (int)count;
    sharedMemOut[1] = 1;
    sharedMemOut[2] = 2;
    sharedMemOut[3] = 3;
    sharedMemOut[4] = 4;
    sharedMemOut[5] = 5;
    sharedMemOut[6] = 6;
    CopyMemory((PVOID)pBuf, sharedMemOut, (7 * sizeof(signed int)));
    return count;
}
