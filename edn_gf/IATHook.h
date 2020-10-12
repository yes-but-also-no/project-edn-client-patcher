#pragma once
#include "Console.h"

class IATHook
{
public:
	IATHook(Console * con);
	IATHook(char* sModule, void* pHook, char* sSymbol);
	IATHook(HMODULE hModule, void* pHook, char* sSymbol);
	~IATHook();
	void SetConsole(Console * con);
	void Init(char* sModule, void* pHook, char* sSymbol);
	void Init(HMODULE hModule, void* pHook, char* sSymbol);
	void Init(HMODULE hModule, void * pHook, uintptr_t pFunc);
	static void** FindInIAT(const char* sFunction, HMODULE hModule, Console * con);
	bool Hook();
	bool Unload();
	uintptr_t GetOriginalFunction() { return pOldFunc; }

private:
	Console * con = nullptr;
	DWORD dwOldPrt;
	HMODULE hModule;
	char* sModule;
	char* sSymbol;
	void* pHook;
	uintptr_t pFunctionPtr, pOldFunc;
};

