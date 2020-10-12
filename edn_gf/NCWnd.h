#pragma once

// Struct for NcWnd
struct NCWnd
{
	// Sends a C_LOG to the server
	typedef uintptr_t(__thiscall * _FindChildControl)(void* pThis, int objId, bool recursive);
	static _FindChildControl  FindChildControl;

};

// Hook vtable for NCWnd::DispatchWndMsg to monitor our events
typedef UINT(__thiscall* _DispatchWndMsg)(void* pThis, UINT param1, UINT param2, LONG param3);

// Callback
typedef bool( __stdcall* _DispatchWndMsgCallback)(UINT eventId, UINT objectId);
extern _DispatchWndMsgCallback DispatchWndMsgCallback;

// Hook for add event internal
typedef void(__cdecl * _AddEventInternal)(float param_1, int param_2, int param_3, void * param_4, char * param_5, int param_6);

// Functions for hooking and unhooking
void HookDispatchWndMsg(uintptr_t moduleBase);
void UnHookDispatchWndMsg();

void HookAddEventInternal(uintptr_t moduleBase);
void UnHookAddEventInternal();