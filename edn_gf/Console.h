#pragma once

class Console
{
public:
	Console();
	~Console();
	void Clear();
	void SetCursorPos(COORD dwPos);
	void GetConsoleInfo();
	void Write(const char * input);
	void WritePointer(uintptr_t input);
	void WriteF(const char * fmt, ...);

private:
	HANDLE hOut;
	COORD dwSize;
	COORD dwCursorPos;
	SMALL_RECT srWindow;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
};

