/**********************************************************************
 *  Project App6
 *
 *  shared_data.cpp
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

#include "../include/shared_data.h"
#include "../include/glwidget.h"
#include "../include/print.h"


Data::Data()
{

}

Data::Data(const Data& B)
{

}

void Data::Declare()
{
    SetName("data");
	SET_ATTR("top", &addr1, "int", 0);
	SET_ATTR("pers", &addr2, "int", 0);
    SET_ATTR("bits", &bits, "int", 0);
    SET_ATTR("stribe", &stribe, "int", 0);
    SET_ATTR("format", &format, "int", 0);
    SET_ATTR("RES_X", &RES_X, "int", 0);
    SET_ATTR("RES_Y", &RES_Y, "int", 0);
    //prn("addr RES_X = %lx",&RES_X);
    //prn("addr RES_Y = %lx",&RES_Y);
    //GetBmpProps(stribe, RES_X, RES_Y );

}

void Data::Slot1()
{
    //char* top=0;
    //char* pers=0;
    //GetBmpPtrs(&top, &pers);
    //при изменении размера окон мен€ютс€ резолюции
    //GetBmpProps(stribe, RES_X, RES_Y );
}
