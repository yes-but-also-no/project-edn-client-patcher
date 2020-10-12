#pragma once
#include <Windows.h>
#include "IATHook.h"
#define RC reinterpret_cast

enum Registers : unsigned char
{
	eax,
	ecx,
	edx,
	ebx,
	esp,
	ebp,
	esi,
	edi
};

enum EHOOKTYPE : unsigned char
{
	CALL = 0xE8,
	JMP
};

class Memory
{
public:
	Memory();
	~Memory();
	uintptr_t GetDMA(uintptr_t pBase, UINT * pOffsets, UINT iNumOffsets);
	void WriteBytes(uintptr_t pAddress, BYTE * pBytes, UINT szSize);
	//templated functions
	template <class T>
	T ReadValue(uintptr_t pAddress);

	template <class T>
	T ReadValue(uintptr_t pBase, UINT * pOffsets, UINT iNumOffsets);

	template <class T>
	bool WriteValue(uintptr_t pAddress, T data);

	template <class T>
	bool WriteValue(uintptr_t pBase, UINT * pOffsets, UINT iNumOffsets, T data);

	bool Hook(void * toHook, void * ourFunct, int len);
	bool Hook(void * toHook, void * ourFunct, EHOOKTYPE eHookType, int len);
	
	//this shit dont work.. lol
	uintptr_t HookWithTrampoline(uintptr_t pSource, uintptr_t pHook, Registers reg, unsigned short len);
	
private:
	DWORD dwOldProt;
};

template<class T>
inline T Memory::ReadValue(uintptr_t pAddress)
{
	return *RC<T*>(pAddress);
}

template<class T>
inline T Memory::ReadValue(uintptr_t pBase, UINT * pOffsets, UINT iNumOffsets)
{
	return *RC<T*>(GetDMA(pBase, pOffsets, iNumOffsets));
}

template<class T>
inline bool Memory::WriteValue(uintptr_t pAddress, T data)
{
	if(VirtualProtect((void*)pAddress, sizeof(T), PAGE_EXECUTE_READWRITE, &dwOldProt))
		*RC<T*>(pAddress) = data;
	else	
		return false;
	
	VirtualProtect((void*)pAddress, sizeof(T), dwOldProt, nullptr);
	if (*RC<T*>(pAddress) != data)
		return false;
	else
		return true;
}

template<class T>
inline bool Memory::WriteValue(uintptr_t pBase, UINT * pOffsets, UINT iNumOffsets, T data)
{	
	T* dmaAddy = RC<T*>(GetDMA(pBase, pOffsets, iNumOffsets));	
	if (VirtualProtect((void*)dmaAddy, sizeof(T), PAGE_EXECUTE_READWRITE, &dwOldProt))
		*dmaAddy = data;
	else
		return false;
	VirtualProtect((void*)dmaAddy, sizeof(T), dwOldProt, nullptr);
	if (*dmaAddy != data)
		return false;
	else
		return true;
}
