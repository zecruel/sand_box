#undef RC_INVOKED
#define _WINGDI_H
#define _WINUSER_H
#define _WINNLS_H
#define _WINVER_H
#define _WINNETWK_H
#define _WINREG_H
#define _WINSVC_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern double fmodels_(double *arg);

__declspec(dllexport) int cdllfun_(double argv[])
{
	return (int)fmodels_(argv);
}

BOOL APIENTRY DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
	switch (reason){
	case DLL_PROCESS_ATTACH:
	  break;
	case DLL_PROCESS_DETACH:
	  break;
	case DLL_THREAD_ATTACH:
	  break;
	case DLL_THREAD_DETACH:
	  break;
	}
	return TRUE;
}
