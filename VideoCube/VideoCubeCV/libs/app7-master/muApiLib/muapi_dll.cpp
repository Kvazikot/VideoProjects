// Реализует механизм системных вызовов, по сути команд к mu
// Также описывает регистровую машину с прямым доступом к памяти mu


#include <windows.h>
#include <string>
#include <map>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include "muapi_dll.h"

using namespace std;
//#pragma comment(lib, "user32.lib")

#define STACK_SIZE 1024*16
#define HEAP_SIZE 10*1024*1024

HANDLE hStackFile;
HANDLE hHeapFile = 0;
LPCTSTR pStack = 0; // 16 k
LPCTSTR pHeap = 0; // 10 M
HANDLE ghWriteEvent = 0;  // событие о запросе на вызов
HANDLE ghReadEvent = 0;  // событие о возвращенных данных

HWND hwnd = 0;		 //хендл главного окна app5
HANDLE hProcess = 0; // хендл процесса app5

#define WAITTIME 500 


//--------------------REGISTER MACHINE------------------------------------
unsigned long regs[1024*1024];
std::map<std::string, int> regsmap;
std::map<unsigned long, std::string> adr2name;

void ReadProcMem(unsigned long lpAddress)
{
	char buf[255];
	int len = 4;
	
	//ReadProcessBYTES(lpAddress, buf, len);

	printf("lpAddress=%x f=%f i=%d str=%s ", lpAddress, *(float*)buf, *(int*)buf, buf );
}


// Забор хендла процесса app5.exe
//+Апдейт адресов переменных обьекта 
HANDLE GetApp5Handle()
{
    hwnd = FindWindowA(NULL,"app6");
	if (hwnd)
   {
      
	  DWORD proc_id;
      GetWindowThreadProcessId(hwnd, &proc_id);
      hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);
	  return hProcess;
   }
  return NULL;
}

HANDLE WriteFloat(unsigned long lpAddress, float f)
{
   if(hProcess==NULL)
	   hProcess = GetApp5Handle();
   if(hProcess==NULL)
	   return 0;
   WriteProcessMemory(hProcess, (void *) lpAddress, &f, sizeof(float), 0);
   printf("WriteFloat %f", f);
   if(hProcess == 0)
	   printf("Cannot Write float %x %s\n",lpAddress, adr2name[lpAddress].c_str());
   return hProcess;
}

HANDLE WriteInt(unsigned long lpAddress, int i)
{
   if(hProcess==NULL)
	   hProcess = GetApp5Handle();
   if(hProcess==NULL)
	   return 0;
   WriteProcessMemory(hProcess, (void *) lpAddress, &i, sizeof(int), 0);
   if(hProcess == 0)
	   printf("Cannot Write Int %x %s\n",lpAddress, adr2name[lpAddress].c_str());

   return hProcess;
}

void qsort2(int (*compar)(int*,int*))
{
	int a,b,c;
	a=10;
	a=20;
	compar(&a,&b);
	compar(&a,&b);
	c = compar(&a,&b);
	printf("from c c=%d",c);
}

HANDLE ReadFloat(unsigned long lpAddress, float* f)
{
   if(hProcess==NULL)
	   hProcess = GetApp5Handle();
   if(hProcess==NULL)
	   return 0;
   ReadProcessMemory(hProcess, (void *) lpAddress, f, sizeof(float), 0);
   return hProcess;
}

HANDLE ReadData(unsigned long lpAddress, void* p, int size)
{
   if(hProcess==NULL)
	   hProcess = GetApp5Handle();
   if(hProcess==NULL)
	   return 0;
   ReadProcessMemory(hProcess, (void *) lpAddress, p, size, 0);
   return hProcess;
}


HANDLE WriteData(unsigned long lpAddress, void* p, int size)
{
   if(hProcess==NULL)
	   hProcess = GetApp5Handle();
   if(hProcess==NULL)
	   return 0;
   WriteProcessMemory(hProcess, (void *) lpAddress, p, size, 0);
   if(hProcess == 0)
	   printf("Cannot Write Data %x\n",lpAddress);

   return hProcess;
}

HANDLE ReadInt(unsigned long lpAddress, int* i)
{
   if(hProcess==NULL)
	   hProcess = GetApp5Handle();
   if(hProcess==NULL)
	   return 0;
   ReadProcessMemory(hProcess, (void *) lpAddress, i, sizeof(int), 0);
   return hProcess;
}


//--------------------MUAPI DLL------------------------------------

//------------------------------------------------------------------------------

#define IS_MU_ACTIVE_ID 0
int id;

MUAPI_DLL_API int IS_MU_ACTIVE()
{
	DWORD dwWaitResult;

	if(pStack==0) return 0;

	id = IS_MU_ACTIVE_ID;
	
	//запись в стек id вызова
	memcpy((void*)pStack, &id, sizeof(int));

	//параметров нет

   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) ) 
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 1;
   }

   //дождаться ответа
	dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
	if( dwWaitResult!=WAIT_OBJECT_0 )
		*(int*)pStack = -1;
	//возвратить значение
	return *(int*)pStack;
	
}

//------------------------------------------------------------------------------

#define GET_OBJ_LIST_ID 1
MUAPI_DLL_API int GET_OBJ_LIST()
{
	DWORD dwWaitResult;

	id = GET_OBJ_LIST_ID;
	
	if(pStack==0) return 0;
	
	//запись в стек id вызова
	memcpy((void*)pStack, &id, sizeof(int));

	//параметров нет

   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) ) 
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 1;
   }

   //дождаться ответа
	dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
	if( dwWaitResult!=WAIT_OBJECT_0 )
		*(int*)pStack = -1;

	//возвратить значение из stack
	return *(int*)pStack;
	
}

//------------------------------------------------------------------------------

#define GET_ATTRS_LIST_ID 2

MUAPI_DLL_API int GET_ATTRS_LIST(char* objname)
{
	DWORD dwWaitResult;

	if(pStack==0) return 0;

	id = GET_ATTRS_LIST_ID;
	
	//запись в стек id вызова
	memcpy((void*)pStack, &id, sizeof(int));

	//1 параметр
	//sprintf(pStack+sizeof(int),"%s",objname);
	memcpy((void*)(pStack+sizeof(int)), objname, 255);
	

   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) ) 
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 0;
   }

   //дождаться ответа
	dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
	if( dwWaitResult!=WAIT_OBJECT_0 )
		*(int*)pStack = -1;

	//возвратить значение из stack
	return *(int*)pStack;
	
}
//------------------------------------------------------------------------------


#define GET_DEMO_LIST_ID 11
MUAPI_DLL_API int GET_DEMO_LIST()
{
	DWORD dwWaitResult;

	//printf("GET_DEMO_LIST_ID");
	id = GET_DEMO_LIST_ID;
	
	if(pStack==0) return 0;
	
	//запись в стек id вызова
	memcpy((void*)pStack, &id, sizeof(int));

	//параметров нет

   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) ) 
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 1;
   }

   //дождаться ответа
	dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
	if( dwWaitResult!=WAIT_OBJECT_0 )
		*(int*)pStack = -1;

	//возвратить значение из stack
	return *(int*)pStack;
	
}

//-----------------------------------------------------------------

#define GET_DEMO_LIST2_ID 12
MUAPI_DLL_API int GET_DEMO_LIST2(char* objname)
{
	DWORD dwWaitResult;

	if(pStack==0) return 0;

	id = GET_DEMO_LIST2_ID;
	
	//запись в стек id вызова
	memcpy((void*)pStack, &id, sizeof(int));

	//1 параметр
	//sprintf(pStack+sizeof(int),"%s",objname);
	memcpy((void*)(pStack+sizeof(int)), objname, 255);
	

   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) ) 
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 0;
   }

   //дождаться ответа
	dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
	if( dwWaitResult!=WAIT_OBJECT_0 )
		*(int*)pStack = -1;

	//возвратить значение из stack
	return *(int*)pStack;
	
}

//-----------------------------------------------------------------
#define SET_ATTR_CHANGE_ID 14
// Вызывает событие OnAttrChange у заданного обьекта
int SET_ATTR_CHANGE(char* objname, char* atrname)
{
	DWORD dwWaitResult;


	if(pStack==0) return 0;

	id = SET_ATTR_CHANGE_ID;
	
	//запись в стек id вызова
	memcpy((void*)pStack, &id, sizeof(int));

	//1 и 2 параметр
	//sprintf(pStack+sizeof(int),"%s",objname);
	sprintf((char*)(pStack+sizeof(int)),"%s,%s",objname,atrname) ;

   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) ) 
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 0;
   }

   //дождаться ответа
	dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
	if( dwWaitResult!=WAIT_OBJECT_0 )
		*(int*)pStack = -1;

	//возвратить значение из stack
	return *(int*)pStack;
}


//-----------------------------------------------------------------

#define SET_KEYDOWN_ID 15

MUAPI_DLL_API int SET_KEYDOWN(int k)
{
    DWORD dwWaitResult;

    if(pStack==0) return 0;

    id = SET_KEYDOWN_ID;

    //запись в стек id вызова
    memcpy((void*)pStack, &id, sizeof(int));

    //1 параметр
    //sprintf(pStack+sizeof(int),"%s",objname);
    memcpy((void*)(pStack+sizeof(int)), &k, sizeof(int));


   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) )
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 0;
   }

   //дождаться ответа
    dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
    if( dwWaitResult!=WAIT_OBJECT_0 )
        *(int*)pStack = -1;

    //возвратить значение из stack
    return *(int*)pStack;

}

//-----------------------------------------------------------------

#define SET_KEYUP_ID 16

MUAPI_DLL_API int SET_KEYUP(int k)
{
    DWORD dwWaitResult;

    if(pStack==0) return 0;

    id = SET_KEYUP_ID;

    //запись в стек id вызова
    memcpy((void*)pStack, &id, sizeof(int));

    //1 параметр
    //sprintf(pStack+sizeof(int),"%s",objname);
    memcpy((void*)(pStack+sizeof(int)), &k, sizeof(int));


   //отослать событие записи вызова
   if (! SetEvent(ghWriteEvent) )
   {
       printf("Set ghWriteEvent failed (%d)\n", GetLastError());
       return 0;
   }

   //дождаться ответа
    dwWaitResult = WaitForSingleObject( ghReadEvent, WAITTIME );
    if( dwWaitResult!=WAIT_OBJECT_0 )
        *(int*)pStack = -1;

    //возвратить значение из stack
    return *(int*)pStack;

}


//копирует кучу в буффер 
void GetHeap(char* buf, int size)
{
	if(pHeap==0) return;
	memcpy(buf, pHeap, size);
}

void ClearHeap(int size)
{
	if(pHeap==0) return;
	memset((void*)pHeap, 0, size);
}

void PrintHeap()
{
	if(pHeap==0) return;
	*(char*)(pHeap + HEAP_SIZE - 1) = 0;
	printf("%s", pHeap);
}

void PerformTests()
{
   return;
	printf("IS_MU_ACTIVE() return %d\n",IS_MU_ACTIVE());
   printf("GET_OBJ_LIST() return %d\n",GET_OBJ_LIST());
   PrintHeap();
   printf("GET_ATTRS_LIST() return %d\n",GET_ATTRS_LIST("cam01"));
   PrintHeap();
}


//-----------------------------------------------------------------

MUAPI_DLL_API int StartDll()
{

	//обнулить регистры
	for(int i=0; i<1024; i++)
	  regs[i] = 0;

	//открыть процесс (олучить хендл)
	hProcess = GetApp5Handle();
	if(hProcess==NULL)
	{
		printf("app5.exe is not started");
		return 0;
	}

   // Событие ghWriteEvent устанавливает эта длл при записи вызова в общую память
   ghWriteEvent = OpenEvent( 
        EVENT_ALL_ACCESS ,            // request full access
        FALSE,                       // handle not inheritable
        TEXT("MuWriteEvent"));  // object name

   if (ghWriteEvent == NULL)
   {
      printf("app5 not started? Could not open  MuWriteEvent (%d).\n", GetLastError());
	  //getch();
      return 1;
   }

   // Событие ghReadEvent устанавливает app5.exe при отработке вызова, результаты из общей памяти
   ghReadEvent = OpenEvent( 
        EVENT_ALL_ACCESS ,            // request full access
        FALSE,                       // handle not inheritable
        TEXT("MuReadEvent"));  // object name

   if (ghReadEvent == NULL)
   {
      printf("app5 not started? Could not open  MuReadEvent (%d).\n", GetLastError());
	  //getch();
      return 1;
   }

//-----------------------------------------------------------------------------------
   hStackFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   TEXT("Global\\MuapiStack"));               // name of mapping object

   if (hStackFile == NULL)
   {
      printf("Could not open file mapping object (%d).\n", GetLastError());
	  //getch();
      return 1;
   }

   pStack = (LPTSTR) MapViewOfFile(hStackFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               STACK_SIZE);

   if (pStack == NULL)
   {
      printf("Could not map view of file (%d).\n", GetLastError());

      CloseHandle(hStackFile);
	  //getch();	
      return 1;
   }

//-----------------------------------------------------------------------------------
   hHeapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD)
                 HEAP_SIZE,                // maximum object size (low-order DWORD)
                 TEXT("Global\\MuapiHeap"));                 // name of mapping object

   if (hHeapFile == NULL)
   {
      printf("app5 not started? Could not create hHeapFile file mapping object (%d).\n", GetLastError());
      return 0;
   }

   pHeap = (LPTSTR) MapViewOfFile(hHeapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        HEAP_SIZE);

   if (pHeap == NULL)
   {
      printf("Could not map view of file (%d).\n", GetLastError());

       CloseHandle(hHeapFile);

      return 0;
   }

     
//-----------------------------------------------------------------------------------
//--------------------------------------------------------
   //готов работать
   //MessageBox(NULL, pStack, TEXT("Process2"), MB_OK);
   PerformTests();   
   

//---------------------------------------------------------
      

   //getch();

	return 0;
}

MUAPI_DLL_API void EndDll()
{
	//printf("ending dll");
   UnmapViewOfFile(pStack);
   pStack = 0;
   UnmapViewOfFile(pHeap);
   pHeap = 0;
   CloseHandle(hStackFile);
   hStackFile = 0;
   CloseHandle(hHeapFile);
   hHeapFile = 0;
   CloseHandle(hProcess);

}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			StartDll();
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			EndDll();
			break;
		}

//		case DLL_THREAD_ATTACH:
//		case DLL_THREAD_DETACH:
    }
    return TRUE;
}


