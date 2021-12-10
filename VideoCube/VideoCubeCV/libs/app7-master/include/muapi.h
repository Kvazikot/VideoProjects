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

// ����� ������� � ������
int  WaitForEvent(char* name);
int  WaitForEvents(std::vector<std::string>& names);
int  WaitDefaultEvents();
int  CreateNamedEvent(char* name);
int  TriggerEvent(char* name);
int  WaitWriteEvent();
void CreateDefaultEvents();


//1 - ���� ������� mu.exe �����������
//0 - ���� ������� mu.exe �� �����������
#define IS_MU_ACTIVE_ID 0
int IS_MU_ACTIVE();


//	������ ����������� �������� ����:
//		���������.����������
//	"-1" ������� mu.exe �� �����������
//	"nil" ��� ������ ������ (��� ��������)
int GET_OBJ_LIST();


// ������ ���������� ������� ����:
//	���.���
//   -2 - ���� ������ �����������
//	"-1" - ������� mu.exe �� �����������
//	"nil" - ��� ������ ������ (��� ��������)
int GET_ATTRS_LIST(char* objname);


//����������� �������� ������� �� ������.
//���������� 
//-1 - ������� mu.exe �� �����������
//-2 - ���� ������ �����������
//-3 - ���� �������� �����������
 //0 - ���� ��������� ������ ��� ����������
 //1 - ���� �������� ��������� ��������
int SET_OBJATTR(char* objname, char* attrname, char* attrval);


//������� ������ ��������� ������
//���������� 
//-1 - ���� ������� mu.exe �� �����������
//0 - ���� ������ �� ������, ��������� ������
//1 - ���� ������ ������
int CREATE_OBJ(char* classname, char* objname);


//������� ������ �� �����
//���������� 
//-1 - ���� ������� mu.exe �� �����������
//-2 - ���� ������ ����������
//1 - ���� ������ ������
int DELETE_OBJ(char* objname);


//������ �������� ��������� (��� ����� ������������ � ������ ����������)
//���������� 
//-1 - ���� ������� mu.exe �� �����������
//-3 - ���� �������� �����������
//-2 - ���� ������ ����������
//1 - ���� ������ ������
int SET_DESCR(char* objname, char* objattr);

//�������� ����� ������� ���� �� ��� ���������������
//���������� 
//-1 - ���� ������� mu.exe �� �����������
//-3 - ���� ����� �����������
//-2 - ���� ������ ����������
//1 - ���� ������ ������
int CALL_METHOD(char* objname, char* methname, char* args);


//��������� �������� �������
//���������� 
//-1 - ������� mu.exe �� �����������
//-2 - ���� ������ �����������
//-3 - ���� �������� ��� ����
//1 - ���� �������� ��������� ��������
int ADD_OBJATTR(char* objname, char* attrtype, char* attrname);


//��������� � ������� � ������� regnum 0-255 ����� ��������� �������
//����������� ������ ����������� �� ������� mudll ������� ����� ������ ������ ������
//����������
//-1 - ������� mu.exe �� �����������
//-2 - ���� ������ �����������
//-3 - ���� �������� �����������
//-4 - ������������ ����� ��������
 //1 - ���� ������� ��������
int GET_ATTR_ADDR(char* objname, char* objattr);

int USERCMD(char* cmd);

//���������� � heap  ������ �����:
//	objname1_time1.mudem objname1_time2.mudem objname2_time1.mudem objname2_time2.mudem
//-1 - ������� mu.exe �� �����������
//-2 - ���� ������ DemoRecorder ����������
//-3 - ���� ����� ���
int GET_DEMO_LIST();

//���������� � heap  ������ ����� ��� ������� ����:
//	objname1_time1.mudem objname1_time2.mudem objname1_time3.mudem
int GET_DEMO_LIST(char* objname);

// �������� ������� OnAttrChange � ��������� �������
int SET_ATTR_CHANGE(char* objname, char* atrname);

#endif
