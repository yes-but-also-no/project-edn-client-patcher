#pragma once
#include <stdio.h>
#include <string>
#include "Memory.h"
#include "Console.h"
#include "loguru.hpp"

class Interface
{
public:
	Interface();
	~Interface();
	void Init();
	void Unload();

	Console con;
	void TryLogin();
	bool HandleInput();

private:
	void PrintIntro();
	
private:
	Memory mem;

public:
	bool bRunning = true;
};

extern Interface * g_pFace;
