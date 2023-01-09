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
int32_t g_monitorWidth = 0;
int32_t g_monitorHeight = 0;

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
BOOL IsGameAlreadyRunning(void);

int32_t CRCreateMainGameWindow(void);

int32_t CRGetMonitorInfoForWindow();

int32_t CRSetWindowPosition();

