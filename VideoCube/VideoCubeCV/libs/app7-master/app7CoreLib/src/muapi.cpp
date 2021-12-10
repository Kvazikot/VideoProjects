/**********************************************************************
 *  Project App6
 *
 *  muapi.cpp
 *
 *  Author: Vladimir Baranov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
**********************************************************************/

#include "../include/cobjregistry.h"
#include "../include/object.h"
#include "../include/group.h"
#include "../include/cinput.h"
#include "../include/print.h"
#include "../include/muapi.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <windows.h>

map<int,string> id2callname;
std::string 	currentevent;
std::vector<HANDLE> events;
std::vector<string> eventNames;
std::map<string, HANDLE> eventsMap;

std::map<int, string>eventsIdxMap;

static CObjRegistry& MREG = CObjRegistry::getInstance2();
static CInput* iinput = CInput::getInstance();
static char gstr[10000];
static char gstr2[10000];

//#define DEBUG_CALLS 1
#define STACK_SIZE 1024*16
#define HEAP_SIZE 10*1024*1024



wchar_t szMsg[]=TEXT("Message from first process.");
LPCTSTR pStack = 0; // 16 k
unsigned char* pHeap = 0; // 10 M
HANDLE hStackFile = 0;
HANDLE hHeapFile = 0;

HANDLE ghWriteEvent = 0; 
HANDLE ghReadEvent = 0;
HANDLE ghGrafEvent = 0;
HANDLE ghdummyEvent = 0;  

//--------------------------------------------------------------------------------


//--------------- Работа с shared memory -----------------------------------------
int CreateSharedMemory()
{

//----------------------------------------------------------------------------------
    hStackFile = CreateFileMapping(
                    INVALID_HANDLE_VALUE,    // use paging file
                    NULL,                    // default security
                    PAGE_READWRITE,          // read/write access
                    0,                       // maximum object size (high-order DWORD)
                    STACK_SIZE,                // maximum object size (low-order DWORD)
                    TEXT("Global\\MuapiStack"));                 // name of mapping object


   if (hStackFile == NULL)
   {
      prn("Could not create hStackFile file mapping object (%x).\n",
             GetLastError());
      return 0;
   }

   pStack = (LPTSTR) MapViewOfFile(hStackFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        STACK_SIZE);

   if (pStack == NULL)
   {
      prn("Could not map view of file (%d).\n",
             GetLastError());

       CloseHandle(hStackFile);

      return 0;
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
      prn("Could not create hHeapFile file mapping object (%d).\n",
             GetLastError());
      return 0;
   }

   pHeap = (unsigned char*) MapViewOfFile(hHeapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        HEAP_SIZE);

   if (pHeap == NULL)
   {
      prn("Could not map view of file (%d).\n",
             GetLastError());

       CloseHandle(hHeapFile);

      return 0;
   }

     
//-----------------------------------------------------------------------------------
   
   //CopyMemory((PVOID)pStack, (TCHAR*)szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));

   prn("Shared memory stack created in Global\\MuapiStack");
   prn("Shared memory heap created in Global\\MuapiHeap");
   prn("Message from first process");

   sprintf(gstr,"obj,atr,val");
   sprintf(gstr2,"");
   strff(gstr, gstr2, ',',1);
   //prn(gstr2);

//-----------------------------------------------------------------------------------
 
   	// Событие ghWriteEvent устанавливает muapi.dll при записи вызова в общую память
   
    ghWriteEvent = CreateEvent( 
        NULL,               // default security attributes
        FALSE,               // manual-reset event
        FALSE,              // initial state
        TEXT("MuWriteEvent")  // object name
        ); 

    if (ghWriteEvent == NULL) 
    { 
        prn("MUAPI Create Write Event failed (%d)\n", GetLastError());
        return 0;
    }

    // Событие ghReadEvent устанавливает app6.exe при отработке вызова, результаты из общей памяти
    ghReadEvent = CreateEvent( 
        NULL,               // default security attributes
        FALSE,               // manual-reset event
        FALSE,              // initial state
        TEXT("MuReadEvent")  // object name
        ); 

    if (ghReadEvent == NULL) 
    { 
        prn("MUAPI Create Read Event failed (%d)\n", GetLastError());
        return 0;
    }


    // Событие ghMUEvent устанавливает app6.exe при обращении к приложению графиков
    ghGrafEvent = CreateEvent( 
        NULL,               // default security attributes
        FALSE,               // manual-reset event
        FALSE,              // initial state
        TEXT("MuGrafEvent")  // object name
        ); 

    if (ghGrafEvent == NULL) 
    { 
        prn("MUAPI Create MuGrafEvent failed (%d)\n", GetLastError());
        return 0;
    }


    // Событие ghMUEvent устанавливает app6.exe при обращении к приложению графиков
    ghdummyEvent = CreateEvent( 
        NULL,               // default security attributes
        FALSE,               // manual-reset event
        FALSE,              // initial state
        TEXT("MuDummyEvent")  // object name
        ); 

    if (ghdummyEvent == NULL) 
    { 
        prn("MUAPI Create MuDummyEvent failed (%d)\n", GetLastError());
        return 0;
    }

	//----------------------------------------------------------------------
	id2callname[IS_MU_ACTIVE_ID] =	 "IS_MU_ACTIVE";
	id2callname[GET_OBJ_LIST_ID] =	 "GET_OBJ_LIST";
	id2callname[GET_ATTRS_LIST_ID] = "GET_ATTRS_LIST";
	id2callname[SET_OBJATTR_ID] =	 "SET_OBJATTR";
	id2callname[CREATE_OBJ_ID] =	 "CREATE_OBJ";
	id2callname[DELETE_OBJ_ID] =	 "DELETE_OBJ";
	id2callname[SET_DESCR_ID] =		 "SET_DESCR";
	id2callname[CALL_METHOD_ID] =	 "CALL_METHOD";
	id2callname[GET_ATTR_ADDR_ID] =	 "GET_ATTR_ADDR";
	id2callname[GET_DEMO_LIST_ID] =	 "GET_DEMO_LIST";
	id2callname[GET_DEMO_LIST2_ID] = "GET_DEMO_LIST2";


   return 1;
}

int CloseSharedMemory()
{
   if(pStack!=NULL)
     UnmapViewOfFile(pStack);
   if(hStackFile!=0)
     CloseHandle(hStackFile);
   if(hHeapFile!=0)
     CloseHandle(hHeapFile);

   if(ghWriteEvent!=0)	
     CloseHandle(ghWriteEvent);
   if(ghReadEvent!=0)	
     CloseHandle(ghReadEvent);
	if(ghGrafEvent!=0)	
     CloseHandle(ghGrafEvent);
   if(ghdummyEvent!=0)
     CloseHandle(ghdummyEvent);
   
   
   pStack = NULL;
   pHeap = NULL;
   hStackFile = 0;
   hHeapFile = 0;
   ghWriteEvent = 0;
   ghReadEvent = 0;
   ghGrafEvent = 0;

   
   return 0;
}


int CreateNamedEvent(char* name)
{
	HANDLE hEvent;

	hEvent = CreateEventA(NULL, // default security attributes
		FALSE, // manual-reset event
		FALSE, // initial state
		(name) // object name
		);
	// ghWriteEvent |= MY_SHARE_FULL_ACCESS;

	if (hEvent == NULL)
	{
		prn("SharedMemoryManager Create %s Event failed (%d)\n", name, GetLastError());
		return 0;
	}
	else
	{
		// prn1("ghWriteEvent opened!");
		events.push_back(hEvent);
		eventNames.push_back(name);
		eventsMap[name] = hEvent;
		eventsIdxMap[events.size()] = string(name);
		return 1;
	}

}

// вызывается во втором процессе
int OpenSharedMemory()
{
	
	
	return 0;
}

int TriggerEvent(char* name)
{
	 HANDLE hEvent;
	 if ( eventsMap.find(name)!= eventsMap.end())
	 {
		hEvent = eventsMap[name];
		SetEvent(hEvent);
		return 1;
	 }
	 else
	 	return 0;
}

int WaitDefaultEvents()
{
	DWORD dwEvent;
	DWORD   N;
	HANDLE ahWaitEvents[255];

	if(hStackFile == 0 ) return 0;
	N = events.size();

    for(int i=0; i<(int)events.size(); i++)
	   ahWaitEvents[i] = events[i];

	dwEvent = WaitForMultipleObjects(N, ahWaitEvents, FALSE, INFINITE) - WAIT_OBJECT_0;

	if (dwEvent < (DWORD)N)
	{
	   prn("Сработало событие %s", eventNames[dwEvent].c_str());
	   currentevent =  eventNames[dwEvent];
	   return dwEvent;
	}
    return 0;
}

int WaitForEvents(std::vector<std::string>& names)
{
	DWORD dwEvent;
	DWORD   N;
	HANDLE ahWaitEvents[255];

	N = names.size();

    for(int i=0; i<(int)names.size(); i++)
	   ahWaitEvents[i] = eventsMap[names[i]];

	dwEvent = WaitForMultipleObjects(N, ahWaitEvents, FALSE, INFINITE) - WAIT_OBJECT_0;

	if (dwEvent < (DWORD)N)
	{
	   prn("Сработало событие %s", eventsIdxMap[dwEvent].c_str());
	   return dwEvent;
	}
    return 0;
}

int WaitForEvent(char* name)
{
	HANDLE hEvent;
	if(hStackFile == 0 ) return 0;
	if ( eventsMap.find(name)!= eventsMap.end())
	{
		hEvent = eventsMap[name];
		return WaitForSingleObject(hEvent, INFINITE);
	}
	else return -1;
}

void CreateDefaultEvents()
{
	CreateNamedEvent("scriptReady");
	CreateNamedEvent("resultReady");
	CreateNamedEvent("closeEvent1");
}


void strff(char* str, char* field, char sep, int num)
{
	int s = 0;
	num++;
    for(int i=0; i<(int)strlen(str); i++)
	{
		if(str[i] == sep)
		{
			num--;
			if( num == 0 )
			{
				strncpy(field, str+s,i-s); 
				return;
			}
			s = i;	
		}
		
		
	}
	strncpy(field, str+s+1,strlen(str)-s);


}


//---Отработка системного вызова----------------------------------------------------------
char* arg[3];				

void parse_args()
{
	arg[0] = (char*) (pStack + sizeof(int));
				
	int len = strlen(arg[0]);
	int num = 0;
	
	for(int i=0; i<len; i++)
	{
		if(arg[0][i] == ',')
		{
			num++;
			arg[0][i]=0;
			arg[num]=&arg[0][i+1];
		}			
	}
}

int ProcessCall()
{
	int call_id;
	void* stack = (void*)pStack;

	string s;

	call_id = *(int*)stack; 
	
	switch( call_id )
	{
			case IS_MU_ACTIVE_ID:
			{
				*(int*)stack = IS_MU_ACTIVE();
				break;
			}
			case GET_OBJ_LIST_ID:
			{
				*(int*)stack = GET_OBJ_LIST();	 //процедура кладет список в heap
				//возвр в STACK
				break;
			}
			case GET_ATTRS_LIST_ID:
			{
				arg[0] = (char*) (pStack + sizeof(int));
				*(int*)stack = GET_ATTRS_LIST(arg[0]);
				break;
			}
			
			// сюда передается вначале int, потом строка вида "objname,attrname,attrval" разделители - запятые)
			//этот метод заранее не эффективен, но для нек. приложений сойдет
			//по хорошему надо делать что-то типа state-machine
			// с конструкцией ассемблерного типа LOADREG(0, obj.attr)
			// запоминается адрес, дальше 
			// 
			case SET_OBJATTR_ID:
			{
				parse_args();

				*(int*)stack = SET_OBJATTR(arg[0], arg[1], arg[2]);
				
				break;
			}
	
		
			case CREATE_OBJ_ID:
			{
				break;
			}
		
			case DELETE_OBJ_ID:
			{
				break;
			}		
			case SET_DESCR_ID:
			{
				break;
			}
			case CALL_METHOD_ID:
			{
				break;
			}
			case GET_ATTR_ADDR_ID:
			{
				parse_args();
				*(int*)stack = GET_ATTR_ADDR(arg[0],arg[1]); // arg[0] - строка вида objname.atrname
				break;
			}

			case SET_ATTR_CHANGE_ID:
			{
				parse_args();
				*(int*)stack = SET_ATTR_CHANGE(arg[0],arg[1]); // arg[0] - строка вида objname.atrname
				break;
			}

            case SET_KEYDOWN_ID:
            {
                int key = *(int*) (pStack + sizeof(int));
                iinput->key_pressed(key);
                break;

            }
            case SET_KEYUP_ID:
            {
                int key = *(int*) (pStack + sizeof(int));
                iinput->key_upressed(key);
                break;

            }

			case GET_DEMO_LIST_ID:
			{
                //*(int*)stack = GET_DEMO_LIST();
				break;
			}
			case GET_DEMO_LIST2_ID:
			{
                //arg[0] = (char*) (pStack + sizeof(int));
                //*(int*)stack = GET_DEMO_LIST(arg[0]);
				break;
			}

	}

   #ifdef DEBUG_CALLS
	 
	if( id2callname.find(call_id)!= id2callname.end())
	{
		prn("%s() return = %d", id2callname[call_id].c_str(), *(int*)stack);
	}
	else
	{
		prn("unknow call %d  return = %d", call_id, *(int*)stack);
	}
   #endif

	
	return 0;

}

void SetGrafEvent()
{
	if (! SetEvent(ghGrafEvent) ) 
	{
		printf("Set ghGrafEvent failed (%d)\n", GetLastError());
		return;
	}
}

//-------- Поток обслуживания системных вызовов -------------------------------------------


UINT MyApiThread( LPVOID pParam )
{
	DWORD dwWaitResult;
	int callres;
    pParam=0;
	
	while(1)
	{
		if ( (dwWaitResult = WaitForSingleObject( ghWriteEvent,INFINITE )) == WAIT_OBJECT_0 )
		{
			
			//Какой вызов?
			//  Прочитать адрес вызова CALL_ID и параметры из STACK
			//  Размэппить параметры
	
			//Вызов
			callres = ProcessCall();

			//Вернуть ответ
			//	Положить результат в STACK или HEAP

		
			//  Установить событие ghReadEvent готовности результата
			//if( callres == 1 )
			if (! SetEvent(ghReadEvent) ) 
			{
				printf("Set ghReadEvent failed (%d)\n", GetLastError());
				return 1;
			}

		}
		//prn("No Api call!");
		//Sleep(100);
	}
	return 0;
}

void ApiThread::run()
{
    MyApiThread(0);
}


//------------------------------- САМИ СИСТЕМНЫЕ ВЫЗОВЫ -------------------------------

//1 - если процесс app6.exe откликается
//0 - если процесс app6.exe не откликается
int IS_MU_ACTIVE()
{
	return 1;
}

//	Список загруженных обьектов вида:
//		ИмяКласса.ИмяОбьекта
//	"-1" процесс app6.exe не откликается
//	"nil" при пустом списке (нет обьектов)
int GET_OBJ_LIST()
{
	string s;
	vector<Object*>::iterator i;

	int cnt = 0;
	for(i=MREG.allObj.begin(); i!=MREG.allObj.end(); i++)
	{
		s =(*i)->classname + "." + (*i)->name + " ";
		sprintf((char*)(pHeap+cnt),"%s",s.c_str());
		cnt+=s.size();
	}
	if(cnt ==0 )
		sprintf((char*)(pHeap),"nil");
	//memcpy(pHeap, list.c_str(), list.size());
	
	//mu_printf("MUVM updating references...");
	//print_script(script.c_str());
	return 1;

}

// Список аттрибутов обьекта вида:
//	Тип.Имя
//   -2 - если обьект отсутствует
//	"-1" - процесс app6.exe не откликается
//	"nil" - при пустом списке (нет обьектов)
int GET_ATTRS_LIST(char* objname)
{
	Object* o;
	string s;

	if( strstr(objname,".")!=NULL )
	{
		vector<string> l = StrSplitE(objname,".",0);
		string group = l[0];
		string objnm = l[1];
		vector<string> l2 = StrSplitE(l[1],",",0);
		objnm = l2[0];
		Group* g = (Group*)MREG.FindObj(group);
		o = g->FindObj(objnm);
		//if( o->atrs.size() == 0)			
		o->Declare();
	
	}
	else
		o = MREG.FindObj(objname);

//prn("GET_ATTRS_LIST %s", objname);
	//o = MREG.FindObj(objname);
	
	if( o == NULL)
	{
		sprintf((char*)(pHeap),"nil");
		return -2;
	}
	if( o->atrs.size() == 0)
	{
		sprintf((char*)(pHeap),"nil");
		return 1;
	}

	int cnt=0;
	static char addr_str[100];
    for(int i=0; i<(int)o->atrs.size(); i++)
	{
		
		
		sprintf(addr_str," %lx,",o->atrs[i].addr);
		s =o->atrs[i].type + " " + o->atrs[i].name + string(addr_str);
		sprintf((char*)(pHeap+cnt),"%s",s.c_str());
		cnt+=s.size();
        //prn(s.c_str());
	}
	return 1;
}


//Создает обьект заданного класса
//Возвращает 
//-1 - если процесс app6.exe не откликается
//0 - если обьект не создан, произошла ошибка
//1 - если обьект создан
int CREATE_OBJ(char* classname, char* objname)
{
	Object* o;

	o = MREG.CreateObj(objname, classname);

	if(o!=NULL)
	  sprintf((char*)(pHeap),"%s",o->name.c_str());
	else
    	return 0;
	
	return 1;
}


//Удаляет обьект по имени
//Возвращает 
//-1 - если процесс app6.exe не откликается
//-2 - если обьект отсутсвует
//1 - если обьект удален
int DELETE_OBJ(char* objname)
{
	Object* o;
	o = MREG.FindObj(objname);
	if( o == NULL)
		return -2;
	MREG.RemObj(o);
	return 1;
}

//Добавляет свойство обьекту
//Возвращает 
//-1 - процесс app6.exe не откликается
//-2 - если обьект отсутствует
//-3 - если свойство уже есть
//1 - если значение присвоено свойству
int ADD_OBJATTR(char* objname, char* attrtype, char* attrname)
{
	Object* o;
	o = MREG.DeepFindObj(objname);
    attrtype=0;
	if( o == NULL)
		return -2;
	
	if( o->atrsmap.find(attrname)!= o->atrsmap.end() )
		return -3;

	//TODO: Здесь вместо gstr необходимо находить свободный адрес в хипе
	//TODO: За это должен отвечать отдельный класс
	//o->SET_ATTR(attrname, gstr, attrtype);

    //o->SetPyAttr(attrname, attrtype, "");
	
	return 1;
}


//Присвайвает свойство обьекта из строки.
//Возвращает 
//-1 - процесс app6.exe не откликается
//-2 - если обьект отсутствует
//-3 - если свойство отсутствует
 //0 - если произошла ошибка при присвоении
 //1 - если значение присвоено свойству
int SET_OBJATTR(char* objname, char* attrname, char* attrval)
{
	Object* o;

	o = MREG.DeepFindObj(objname);
	
	if( o == NULL)
		return -2;
	
	//лишняя проверка
	//if( o->atrsmap.find(attrname)!= o->atrsmap.end() )
	//	return -3;

    o->SET_ATTR(attrname, attrval,false);

	return 1;
}


//Задает описание аттрибута (это штобы отображалось в Обжект инспекторе) 
//Возвращает 
//-1 - если процесс app6.exe не откликается
//-3 - если свойство отсутствует
//-2 - если обьект отсутсвует
//1 - если обьект удален
int SET_DESCR(char* objname, char* objattr)
{
    objname = objattr;
	return 1;
}

//Вызывает метод обьекта если он был зарегестрирован
//Возвращает 
//-1 - если процесс app6.exe не откликается
//-3 - если метод отсутствует
//-2 - если обьект отсутсвует
//1 - если обьект удален
int CALL_METHOD(char* objname, char* methname, char* args)
{
    objname = methname;
    args = 0;
	return 1;
}


int GET_ATTR_ADDR(char* objname, char* objattr)
{
	Object* o;
	if( strstr(objname,".")!=NULL )
	{
		vector<string> l = StrSplitE(objname,".",0);
		string group = l[0];
		string objnm = l[1];
		vector<string> l2 = StrSplitE(l[1],",",0);
		objnm = l2[0];
		Group* g = (Group*)MREG.FindObj(group);
		o = g->FindObj(objnm);
		//if( o->atrs.size() == 0)			
		//o->Declare();
	
	}
	else
		o = MREG.FindObj(objname);
	
	if( o == NULL)
		return -2;
	
	if( o->atrsmap.find(objattr)== o->atrsmap.end() )
		return -3;

	return (int)o->atrs[o->atrsmap[objattr]].addr;
	

}

// Вызывает событие OnAttrChange у заданного обьекта
int SET_ATTR_CHANGE(char* objname, char* atrname)
{
	Object* o;
	o = MREG.DeepFindObj(objname);
	
	if( o == NULL)
		return -2;
	
	if( o->atrsmap.find(atrname)== o->atrsmap.end() )
		return -3;

	o->OnAttrChange(o->atrs[o->atrsmap[atrname]]);
	return 1;
}

//Возвращает список демок:
//	objname1_time1.mudem objname1_time2.mudem objname2_time1.mudem objname2_time2.mudem
//-1 - процесс app6.exe не откликается
//-2 - если обьект DemoRecorder отсутсвует
//-3 - если демок нет
/*int GET_DEMO_LIST()
{
	DemoRecorder* demorecorder;
	string s;
	int cnt=0;
	demorecorder = (DemoRecorder*)MREG.FindObj("DemoRecorder");
		
	if( demorecorder == NULL)
		return -2;
	
	vector<Demo*>::iterator it;
	for(it = demorecorder->demos.begin(); it!= demorecorder->demos.end(); it++)
	{
		s =(*it)->demoname + " ";
		sprintf((char*)(pHeap+cnt),"%s",s.c_str());
		cnt+=s.size();
	}

	return cnt;
}
*/

//Возвращает список демок для обьекта вида:
//	objname1_time1.mudem objname1_time2.mudem objname1_time3.mudem
int GET_DEMO_LIST(char* objname)
{
    objname=0;
	return 0;
}
