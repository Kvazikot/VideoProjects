#define MUAPI_DLL_API extern "C"
//__declspec(dllexport)
#include <windows.h>

void GetHeap(char* buf, int size);
HANDLE ReadInt(unsigned long lpAddress, int* i);
HANDLE ReadData(unsigned long lpAddress, void* p, int size);
MUAPI_DLL_API int SET_KEYUP(int k);
MUAPI_DLL_API int SET_KEYDOWN(int k);
MUAPI_DLL_API int GET_ATTRS_LIST(char* objname);
MUAPI_DLL_API int StartDll();
MUAPI_DLL_API int IS_MU_ACTIVE();
MUAPI_DLL_API int GET_OBJ_LIST();
MUAPI_DLL_API int fGETREG(unsigned int regnum);
MUAPI_DLL_API int fGETREG2(unsigned int regnum, float* p);

#pragma warning (disable : 4786)
