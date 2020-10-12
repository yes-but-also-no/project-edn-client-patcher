#include "stdafx.h"
#include "Memory.h"



Memory::Memory()
{
}


Memory::~Memory()
{
}

uintptr_t Memory::GetDMA(uintptr_t pBase, UINT * pOffsets, UINT iNumOffsets)
{
	uintptr_t pResolved = *(uintptr_t*)pBase;

	for (UINT i = 0; i < iNumOffsets - 1; i++)
		pResolved = *(uintptr_t*)(pResolved + pOffsets[i]);
	
	return (pResolved + pOffsets[iNumOffsets - 1]);
}

void Memory::WriteBytes(uintptr_t pAddress, BYTE * pBytes, UINT szSize)
{
	VirtualProtect((void*)pAddress, szSize, PAGE_EXECUTE_READWRITE, &dwOldProt);
	{
		if (pBytes != nullptr)
			memcpy((void*)pAddress, pBytes, szSize);
		else
			memset((void*)pAddress, 0x90, szSize);
	}
	VirtualProtect((void*)pAddress, szSize, dwOldProt, nullptr);
}

bool Memory::Hook(void * toHook, void * ourFunct, int len)
{
	if (len < 5)
	{
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
	return true;
}

bool Memory::Hook(void * toHook, void * ourFunct, EHOOKTYPE eHookType, int len)
{
	if (len < 5)
	{
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

	*(BYTE*)toHook = eHookType;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
	return true;
}

uintptr_t Memory::HookWithTrampoline(uintptr_t pSource, uintptr_t pHook, Registers reg, unsigned short length)
{
	uintptr_t hPos = pSource;
	uintptr_t pLoc = hPos - (0x400 * 5);
	void* trampLocation = nullptr;

	while (trampLocation == nullptr)
	{
		trampLocation = VirtualAlloc((void*)pLoc, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		pLoc += 0x400;
	}

	// Write to our trampoline
	// mov <register>, address
	// jmp <register>
	*(BYTE*)trampLocation = 0x90;
	*((BYTE*)trampLocation + 1) = 0x90;
	*((BYTE*)trampLocation + 2) = 0xB8 | reg;
	*(uintptr_t*)((UCHAR*)trampLocation + 3) = pHook;
	*((BYTE*)trampLocation + sizeof(uintptr_t) + 3) = 0xFF;
	*((BYTE*)trampLocation + sizeof(uintptr_t) + 4) = 0xE0 | reg;

	DWORD dwOld;
	void* hookCopy = (void*)hPos;
	SIZE_T len = length;

	if (VirtualProtect(hookCopy, len, PAGE_EXECUTE_READWRITE, &dwOld))
	{
		*(BYTE*)hPos = 0xE9;
		long relative = ((uintptr_t)trampLocation - (uintptr_t)hPos - 5);
		*(long*)((BYTE*)hPos + 1) = relative;
		memset((void*)(hPos + 5), 0x90, length - 5);
		return hPos + length;
	}
	return 0;
}


