#ifndef __OSD_H__
#define __OSD_H__

#include "object.h"

//====================================================================
//                  ����� �������� ���������
//====================================================================
struct t_strsett
{
	int   timeout;
	int   cnt;
	float alpha;
	float astep;
	void* font;
	int  fade_state; //0-������ 1-�������� 2-�����
};


class COSD : public Object
{
	public:
		COSD();
	
	vector<string> strings;
	int Nstr;                  //���������� ����� �� ������
	t_strsett conf;
    static COSD  instance;
    static COSD& getInstance();
	void SetTopText(string s, int timeout); // �� ������ �� ������ ����� ��
	void SetText(string s, int timeout);    // ���������� � �������
    void Render(int RES_X, int RES_Y);
	void Test();
    void Slot1(); //timer slot
};


#endif
