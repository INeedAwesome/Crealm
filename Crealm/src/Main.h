#pragma once

#include <stdio.h>

#pragma warning(push, 2)
#include <Windows.h>
#pragma warning(pop)

#include "Game.h"

#define TOSTRING(X) #X

HWND g_hwnd;
BOOL g_Running = FALSE;
MONITORINFO g_monitorInfo;

HINSTANCE GetInstance()
{
	return GetModuleHandle(NULL);
}

LRESULT CALLBACK MainWindowProcedure(
	HWND	p_Hwnd,
	UINT	p_Msg,
	WPARAM	p_Wide,
	LPARAM	p_Low	// low — meaing short (considering the term wide)
);

int CreateMainGameWindow(void);

int GetMonitorInfoForWindow();

BOOL IsGameAlreadyRunning(void);
