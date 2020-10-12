#pragma once
#include <stdio.h>
#include <string>
#include "Memory.h"
#include "Console.h"

class Interface
{
public:
	Interface();
	~Interface();
	void Init();
	void Unload();

	Console con;
	void TryLogin();

private:
	void PrintIntro();
	
private:
	Memory mem;

public:
	bool bRunning = true;
};

extern Interface * g_pFace;
