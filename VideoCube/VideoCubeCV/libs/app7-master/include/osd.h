#ifndef __OSD_H__
#define __OSD_H__

#include "object.h"

//====================================================================
//                  ВЫВОД ЭКРАННЫХ СООБЩЕНИЙ
//====================================================================
struct t_strsett
{
	int   timeout;
	int   cnt;
	float alpha;
	float astep;
	void* font;
	int  fade_state; //0-начало 1-середина 2-конец
};


class COSD : public Object
{
	public:
		COSD();
	
	vector<string> strings;
	int Nstr;                  //количество строк на экране
	t_strsett conf;
    static COSD  instance;
    static COSD& getInstance();
	void SetTopText(string s, int timeout); // на первой же строке сразу же
	void SetText(string s, int timeout);    // постановка в очередь
    void Render(int RES_X, int RES_Y);
	void Test();
    void Slot1(); //timer slot
};


#endif
