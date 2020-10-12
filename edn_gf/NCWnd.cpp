#include "stdafx.h"
#include "NCWnd.h"
#include "Interface.h"
#include "IATHook.h"
#include "FPacketSender.h"

// Original function
_DispatchWndMsg oDispatchWndMsg;

// Original function
_AddEventInternal oAddEventInternal;

// Callback
_DispatchWndMsgCallback DispatchWndMsgCallback;

// Hook
IATHook * wndHook;

// Hook
IATHook * eventHook;

// Module base
uintptr_t nwindowModule;

// Find child function
NCWnd::_FindChildControl NCWnd::FindChildControl;

// Injected function
UINT __fastcall hDispatchWndMsg(void *that, void *Unknown, UINT param1, UINT param2, LONG param3)
{
	// Check that callback 
	if (DispatchWndMsgCallback) 
	{
		bool cont = DispatchWndMsgCallback(param1, param2);
		if (!cont)
			return 1;
	}

	return oDispatchWndMsg(that, param1, param2, param3);
}

// Injected function
void __cdecl hAddEventInternal(float curTime, int eventId, int param_3, void * param_4, char * param_5, int param_6)
{
	// Log if available
	// TODO: Debug UI with status view
	if (g_pFace)
	{
		g_pFace->con.WriteF("Add event internal: [%f] [%d] [%d] [%d] [%d] [%d]\n", curTime, eventId, param_3, param_4, param_5, param_6);

	}

	// Login event
	if (eventId == 0x1773)
	{
		// Find login data
		uintptr_t* dataAddr = (uintptr_t*)(nwindowModule + 0x1fe3b0);

		// Find the user name and pass
		wchar_t* user = *(wchar_t**)(*dataAddr + 0x38);
		g_pFace->con.WriteF("Login request: %s\n", user);

		wchar_t* pass = *(wchar_t**)(*dataAddr + 0x50);

		// Create packet definition
		CConnectClient_packet packet;

		// packet data
		packet.userName = user;
		packet.passWord = pass;

		packet.userNameSize = wcslen(user) * 2;
		packet.passWordSize = wcslen(pass) * 2;

		packet.unknownA = 0;
		packet.unknownB = 0;
		packet.unknownC = 0;

		// Send packet
		GPacketSender.SendCConnectClient(GPacketSender.pThis, &packet);

		return;
	}

	return oAddEventInternal(curTime, eventId, param_3, param_4, param_5, param_6);
}

// Perfoms a hook to UGFBoss::SetState
void HookDispatchWndMsg(uintptr_t moduleBase)
{
	// Console info
	g_pFace->con.Write("[===================================================================]\n");
	g_pFace->con.Write("Hooking into UGFBoss::SetState...\n");
	g_pFace->con.Write("[===================================================================]\n");

	// Pointer to vtable entry
	uintptr_t pDispatchWndMsg = moduleBase + 0x108ef8;

	// Initialize the hook and memory addresses
	wndHook = &IATHook(&g_pFace->con);
	wndHook->Init((HMODULE)moduleBase, &hDispatchWndMsg, pDispatchWndMsg);

	// Save the original function
	oDispatchWndMsg = (_DispatchWndMsg)wndHook->GetOriginalFunction();

	// Activate the hook
	wndHook->Hook();

	// Setup calls
	NCWnd::FindChildControl = (NCWnd::_FindChildControl)(moduleBase + 0x38cb0);

	// Console info
	g_pFace->con.Write("[===================================================================]\n");
	g_pFace->con.Write("Done!\n");
	g_pFace->con.Write("[===================================================================]\n");
}

void HookAddEventInternal(uintptr_t moduleBase) 
{
	// Pointer to vtable entry
	uintptr_t pAddEventInternal = moduleBase + 0x136324;

	// Initialize the hook and memory addresses
	eventHook = &IATHook(&g_pFace->con);
	eventHook->Init((HMODULE)moduleBase, &hAddEventInternal, pAddEventInternal);

	// Save the original function
	oAddEventInternal = (_AddEventInternal)eventHook->GetOriginalFunction();

	// Save module base
	nwindowModule = moduleBase;

	// Activate the hook
	eventHook->Hook();
}

// Cleanup
void UnHookDispatchWndMsg()
{
	wndHook->Unload();
}

void UnHookAddEventInternal()
{
	eventHook->Unload();
}