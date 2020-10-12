// edn_gf.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "edn_gf.h"
#include "FPacketSender.h"
#include "Interface.h"
#include "NCWnd.h"

// Function to exit the game
_RequestExit RequestExit;

// Main thread for our patches
DWORD WINAPI EdnGfThread(HMODULE hModule) 
{
	// Activate UI
	Interface ui = Interface();

	// Get module base
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"GF.dll");

	// Get the packet sender
	LoadPacketSender(moduleBase);

	// Hook NWindow.dll External AddEventInternal
	HookAddEventInternal((uintptr_t)GetModuleHandle(L"NWindow.dll"));

	// cleanup & eject
#ifndef _DEBUG
	ui.~Interface();
#endif
	
	FreeLibraryAndExitThread(hModule, 0);
	
	return 0;
}
