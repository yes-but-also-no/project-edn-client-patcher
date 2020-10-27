#include "stdafx.h"
#include "Interface.h"
#include "FPacketSender.h"
#include "NCWnd.h"
#include "edn_gf.h"

// Externals
Interface * g_pFace;

HWND window;
bool initialized = false;

static char userEntry[20];
static char passEntry[20];

// Input handler
bool Interface::HandleInput()
{
	if (GetAsyncKeyState(VK_F6) & 1)
	{
		Unload();
	}
	else if (GetAsyncKeyState(VK_F7) & 1)
	{
		//GPacketSender.SendCLog(GPacketSender.pThis, (wchar_t*)message);
	}
	else if (GetAsyncKeyState(VK_F8) & 1)
	{
		//RequestExit(0);
	}
	else if (GetAsyncKeyState(VK_F9) & 1)
	{
		//GPacketSender.SendCConnectClient(GPacketSender.pThis, &packet);
	}
	else
		return true;
}

void DuplicateToConsole(void* user_data, const loguru::Message& message)
{
	Console* console = reinterpret_cast<Console*>(user_data);
	console->WriteF("%s%s", message.prefix, message.message);
}

// Constructor
Interface::Interface()
{
	Init();

	loguru::add_file("edn.log", loguru::Append, loguru::Verbosity_INFO);
	loguru::add_callback("console_logger",
		DuplicateToConsole, &con, loguru::Verbosity_MAX);
}

// Clean on shutdown
Interface::~Interface()
{
}

// Setup basic settings
void Interface::Init()
{
	con.Clear();
	con.SetCursorPos(COORD{ 0,0 });
	PrintIntro();
	g_pFace = this;
	bRunning = true;
}


void Interface::Unload()
{
	con.~Console();
	bRunning = false;
}

// Atempts a login
void Interface::TryLogin()
{
	// Create packet definition
	CConnectClient_packet packet;

	// Convert to wchar_t
	const wchar_t* pUser;
	const wchar_t* pPass;

	// Calculate size
	int szUser = MultiByteToWideChar(CP_ACP, 0, userEntry, -1, NULL, 0);
	int szPass = MultiByteToWideChar(CP_ACP, 0, passEntry, -1, NULL, 0);

	// Allocate memory
	pUser = new wchar_t[szUser];
	pPass = new wchar_t[szPass];
	MultiByteToWideChar(CP_ACP, 0, userEntry, -1, (LPWSTR)pUser, szUser);
	MultiByteToWideChar(CP_ACP, 0, passEntry, -1, (LPWSTR)pPass, szPass);

	// packet data
	packet.userName = (wchar_t*)pUser;
	packet.passWord = (wchar_t*)pPass;

	packet.userNameSize = szUser*2;
	packet.passWordSize = szPass*2;

	packet.unknownA = 0;
	packet.unknownB = 0;
	packet.unknownC = 0;

	// Send packet
	GPacketSender.SendCConnectClient(GPacketSender.pThis, &packet);

	// Free up memory
	delete[] pUser;
	delete[] pPass;
}

void Interface::PrintIntro()
{
	// Console info
	LOG_F(INFO, "[===================================================================]\n");
	LOG_F(INFO, "Activating Exteel.Net client patches...\n");
	LOG_F(INFO, "[===================================================================]\n");
}
