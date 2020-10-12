// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "edn_gf.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)EdnGfThread, hModule, 0, nullptr));
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// Dummy export to allow us to add this DLL to the PE of the target dll
__declspec(dllexport) int APIENTRY Func(LPVOID lpParam)
{
	return 0;
}