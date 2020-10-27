#include "stdafx.h"
#include "UGFBoss.h"
#include "Interface.h"
#include "IATHook.h"

#include <iostream>
#include <iomanip>

using namespace std;

// Original function
_OnPacket oOnPacket;

// Callback
_OnPacketCallback OnPacketCallback;

// Hook
IATHook* packetHook;

// Injected function
void __fastcall hOnPacket(void* that, void* Unknown, char param1, int* param2)
{
	// Log if available
	// TODO: Debug UI with status view
	// TODO: COnfigurable filters
	//  63 is move
#ifdef DEBUG
	if (g_pFace && param1 != 0x63)
	{
		//LOG_F(INFO, "BattleManager::OnPacket - ID: [%d]\n", param1);

		// Create packet reading
		//std::string s1 = "BattleManager::OnPacket - ID: [%#04X]: ";

		LOG_S(INFO) << "BattleManager::OnPacket - ID: [" << setfill('0') << setw(2) << right << hex << (int) param1 << "] ";

		// find size of data-ish
		int i = 0;

		while (((BYTE*)param2 + i) != nullptr && i < 500) {
			LOG_S(INFO) << setfill('0') << setw(2) << right << hex << (int) *((BYTE*)param2 + i) << " ";
			i++;
		}

		// Make array
		/*BYTE* arr = new BYTE[i + 1];

		arr[0] = (BYTE)param1;

		int x = 1;

		while (x < i) {
			arr[x] = *((BYTE*)param2 + i - 1);
			x++;
		}

		// Build string
		const char* cstr = s1.c_str();
		LOG_F(INFO, cstr, arr);*/

		LOG_F(INFO, "\n\n");

		//if (param1 == 0x63) {
		//	// Movement packet
		//	LOG_F(INFO, "  > Unit Moved: Time: [%d], UnitId: [%d], Move: [%d], Unknown: [%d], Boost: [%d], X: [%f], Y: [%f], Z: [%f], vX: [%f], vY: [%f], vZ: [%f], Aim Y: [%d], Aim X: [%d]\n",
		//		*param2, // Time
		//		*(int*)((BYTE*)param2 + 4), // UnitId
		//		*(BYTE*)((BYTE*)param2 + 8), // Move
		//		*(BYTE*)((BYTE*)param2 + 9), // Unknown
		//		*(BYTE*)((BYTE*)param2 + 10), // Boost
		//		*(float*)((BYTE*)param2 + 11), // X
		//		*(float*)((BYTE*)param2 + 15), // Y
		//		*(float*)((BYTE*)param2 + 19), // Z
		//		*(float*)((BYTE*)param2 + 23), // vX
		//		*(float*)((BYTE*)param2 + 27), // vY
		//		*(float*)((BYTE*)param2 + 31), // vZ
		//		*(short*)((BYTE*)param2 + 35), // aimY
		//		*(short*)((BYTE*)param2 + 37) // aimX
		//	);
		//}

		//if (param1 == 0x68) {
		//	// Movement packet
		//	LOG_F(INFO, "  > Unit Blade Attack: Time: [%d], Result Code: [%d], UnitId: [%d], arm: [%d], VictimId: [%d], Damage: [%d], ComboStep: [%d], Aim Y: [%d], Aim X: [%d], X: [%f], Y: [%f], Z: [%f], Air: [%d], Overheat: [%f]\n",
		//		*param2, // Time
		//		*(int*)((BYTE*)param2 + 4), // Result
		//		*(int*)((BYTE*)param2 + 8), // Attacker
		//		*(int*)((BYTE*)param2 + 12), // Arm
		//		*(int*)((BYTE*)param2 + 16), // Victim
		//		*(int*)((BYTE*)param2 + 20), // Damage
		//		*(int*)((BYTE*)param2 + 24), // Combo

		//		*(short*)((BYTE*)param2 + 28), // aimY
		//		*(short*)((BYTE*)param2 + 30), // aimX

		//		*(float*)((BYTE*)param2 + 32), // X
		//		*(float*)((BYTE*)param2 + 36), // Y
		//		*(float*)((BYTE*)param2 + 40), // Z

		//		*(BYTE*)((BYTE*)param2 + 44), // Air

		//		*(float*)((BYTE*)param2 + 45) // Overheat

		//	);
		//}

		if (OnPacketCallback)
			OnPacketCallback(param1, param2);
	}

#endif

	return oOnPacket(that, param1, param2);
}

// Perfoms a hook to UGFBoss::SetState
void HookOnPacket(uintptr_t moduleBase)
{
	// Console info
	LOG_F(INFO, "[===================================================================]\n");
	LOG_F(INFO, "Hooking into BattleManager::OnPacket...\n");
	LOG_F(INFO, "[===================================================================]\n");

	// Pointer to vtable entry
	uintptr_t pOnPacket = moduleBase + 0x16b9ac;

	// Initialize the hook and memory addresses
	packetHook = &IATHook(&g_pFace->con);
	packetHook->Init((HMODULE)moduleBase, &hOnPacket, pOnPacket);

	// Save the original function
	oOnPacket = (_OnPacket)packetHook->GetOriginalFunction();

	// Activate the hook
	packetHook->Hook();

	// Console info
	LOG_F(INFO, "[===================================================================]\n");
	LOG_F(INFO, "Done!\n");
	LOG_F(INFO, "[===================================================================]\n");
}

// Cleanup
void UnHookOnPacket()
{
	packetHook->Unload();
}