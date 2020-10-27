
#pragma once

// Hook vtable for UGFBoss set state to monitor our state
typedef void(__thiscall* _OnPacket)(void* pThis, char msgId, int* data);

// Callback for set state
typedef void(__stdcall* _OnPacketCallback)(char msgId, int* data);
extern _OnPacketCallback OnPacketCallback;

// Functions for hooking and unhooking
void HookOnPacket(uintptr_t moduleBase);
void UnHookOnPacket();