#include "object.h"

// ласс определ€ющий обход ключей
class Data  : public Object
{
public:
    Data();
    Data(const Data& B);
    int bits;
    int addr1;
	int addr2;
    int stribe;
    int format;
    int RES_X;
    int RES_Y;
	void Declare();
    void Slot1();
};
