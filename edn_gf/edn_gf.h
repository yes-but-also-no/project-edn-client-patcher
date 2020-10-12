#pragma once

// Thrad for our patches
DWORD WINAPI EdnGfThread(HMODULE hModule);

// Request exit function
typedef void(__cdecl * _RequestExit)(int code);
extern _RequestExit RequestExit;