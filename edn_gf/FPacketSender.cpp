#include "stdafx.h"
#include "FPacketSender.h"
#include "Interface.h"

// Global packet sender reference
FPacketSender GPacketSender;

// Function to load the packet sender;
void LoadPacketSender(uintptr_t moduleBase)
{
	const UINT offset = 0x1e7a0;

	// Console info
	LOG_F(INFO, "[===================================================================]\n");
	LOG_F(INFO, "Aquiring handle to GPacketSender...\n");
	LOG_F(INFO, "[===================================================================]\n");

	uintptr_t packetSenderBase = moduleBase + offset;

	// Get the packet sender functions from the offset

	// Send_C_LOG
	GPacketSender.SendCLog = (FPacketSender::_SendCLog)(packetSenderBase + 0x490);

	// Send_C_CONNECT_CLIENT
	GPacketSender.SendCConnectClient = (FPacketSender::_SendCConnectClient)(packetSenderBase + 0x3C3F0);

	// Console info
	LOG_F(INFO, "[===================================================================]\n");
	LOG_F(INFO, "Done!\n");
	LOG_F(INFO, "[===================================================================]\n");
}