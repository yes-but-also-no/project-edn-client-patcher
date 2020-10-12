#include "stdafx.h"
#include "Interface.h"



Console::Console()
{
	if (!AllocConsole())
		MessageBox(NULL, L"Failed to allocate a console...", L"Error!", MB_OK);
	else
	{
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}
}


Console::~Console()
{
	FreeConsole();
}

void Console::Clear()
{	
	DWORD dwW = 0;
	FillConsoleOutputCharacterA(hOut, ' ', dwSize.X * dwSize.Y, COORD{ 0 }, &dwW);
}

void Console::SetCursorPos(COORD dwPos)
{
	SetConsoleCursorPosition(hOut, dwPos);
	GetConsoleInfo();
}

void Console::GetConsoleInfo()
{
	GetConsoleScreenBufferInfo(hOut, &csbi);
	dwSize = csbi.dwSize;
	dwCursorPos = csbi.dwCursorPosition;
	srWindow = csbi.srWindow;
}

void Console::Write(const char * input)
{
	WriteConsoleA(hOut, input, strlen(input), nullptr, NULL);
	//GetConsoleInfo();
}

void Console::WritePointer(uintptr_t input)
{
	static const size_t size = 20;
	char buffer[size]{ 0 };
	sprintf_s(buffer, size,"%p", input);
	Write(buffer);
}

void Console::WriteF(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[250]{ 0 };
	vsprintf_s(buffer, fmt, args);
	va_end(args);
	Write(buffer);
}
