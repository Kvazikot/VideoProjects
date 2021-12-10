#ifndef MUAPI_H
#define MUAPI_H

#include <vector>
#include <string>
#include <QThread>

void strff(char* str, char* field, char sep, int num);

class ApiThread : public QThread
{
public:
    void run();
};

//---------------------------------------------------------------------------------------------------------
//	MU API CALL IDs
//---------------------------------------------------------------------------------------------------------

#define GET_OBJ_LIST_ID		1
#define GET_ATTRS_LIST_ID	2
#define SET_OBJATTR_ID		3
#define CREATE_OBJ_ID		4
#define DELETE_OBJ_ID		5
#define SET_DESCR_ID		6
#define CALL_METHOD_ID		7
#define ADD_OBJATTR_ID		8
#define GET_ATTR_ADDR_ID	9
#define USERCMD_ID			10
#define GET_DEMO_LIST_ID	11
#define GET_DEMO_LIST2_ID	12
#define SET_ATTR_CHANGE_ID	14
#define SET_KEYDOWN_ID      15
#define SET_KEYUP_ID        16

//---------------------------------------------------------------------------------------------------------
//	MU API
//---------------------------------------------------------------------------------------------------------

void SetGrafEvent();
int CreateSharedMemory();
int CloseSharedMemory();

// ждать события с именем
int  WaitForEvent(char* name);
int  WaitForEvents(std::vector<std::string>& names);
int  WaitDefaultEvents();
int  CreateNamedEvent(char* name);
int  TriggerEvent(char* name);
int  WaitWriteEvent();
void CreateDefaultEvents();


//1 - если процесс mu.exe откликается
//0 - если процесс mu.exe не откликается
#define IS_MU_ACTIVE_ID 0
int IS_MU_ACTIVE();


//	Список загруженных обьектов вида:
//		ИмяКласса.ИмяОбьекта
//	"-1" процесс mu.exe не откликается
//	"nil" при пустом списке (нет обьектов)
int GET_OBJ_LIST();


// Список аттрибутов обьекта вида:
//	Тип.Имя
//   -2 - если обьект отсутствует
//	"-1" - процесс mu.exe не откликается
//	"nil" - при пустом списке (нет обьектов)
int GET_ATTRS_LIST(char* objname);


//Присвайвает свойство обьекта из строки.
//Возвращает 
//-1 - процесс mu.exe не откликается
//-2 - если обьект отсутствует
//-3 - если свойство отсутствует
 //0 - если произошла ошибка при присвоении
 //1 - если значение присвоено свойству
int SET_OBJATTR(char* objname, char* attrname, char* attrval);


//Создает обьект заданного класса
//Возвращает 
//-1 - если процесс mu.exe не откликается
//0 - если обьект не создан, произошла ошибка
//1 - если обьект создан
int CREATE_OBJ(char* classname, char* objname);


//Удаляет обьект по имени
//Возвращает 
//-1 - если процесс mu.exe не откликается
//-2 - если обьект отсутсвует
//1 - если обьект удален
int DELETE_OBJ(char* objname);


//Задает описание аттрибута (это чтобы отображалось в Обжект инспекторе)
//Возвращает 
//-1 - если процесс mu.exe не откликается
//-3 - если свойство отсутствует
//-2 - если обьект отсутсвует
//1 - если обьект удален
int SET_DESCR(char* objname, char* objattr);

//Вызывает метод обьекта если он был зарегестрирован
//Возвращает 
//-1 - если процесс mu.exe не откликается
//-3 - если метод отсутствует
//-2 - если обьект отсутсвует
//1 - если обьект удален
int CALL_METHOD(char* objname, char* methname, char* args);


//Добавляет свойство обьекту
//Возвращает 
//-1 - процесс mu.exe не откликается
//-2 - если обьект отсутствует
//-3 - если свойство уже есть
//1 - если значение присвоено свойству
int ADD_OBJATTR(char* objname, char* attrtype, char* attrname);


//Загружает в регистр с номером regnum 0-255 адрес аттрибута обьекта
//Регистровая машина реализована на стороне mudll поэтому здесь только взятие адреса
//Возвращает
//-1 - процесс mu.exe не откликается
//-2 - если обьект отсутствует
//-3 - если свойство отсутствует
//-4 - недопустимый номер регистра
 //1 - если регистр загружен
int GET_ATTR_ADDR(char* objname, char* objattr);

int USERCMD(char* cmd);

//Возвращает в heap  список демок:
//	objname1_time1.mudem objname1_time2.mudem objname2_time1.mudem objname2_time2.mudem
//-1 - процесс mu.exe не откликается
//-2 - если обьект DemoRecorder отсутсвует
//-3 - если демок нет
int GET_DEMO_LIST();

//Возвращает в heap  список демок для обьекта вида:
//	objname1_time1.mudem objname1_time2.mudem objname1_time3.mudem
int GET_DEMO_LIST(char* objname);

// Вызывает событие OnAttrChange у заданного обьекта
int SET_ATTR_CHANGE(char* objname, char* atrname);

#endif
