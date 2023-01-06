#pragma once

#include <stdio.h>

#pragma warning(push, 2)
#include <Windows.h>
#pragma warning(pop)

#define GAME_NAME L"Crealm"

#define TOSTRING(X) #X

HWND g_hwnd;
BOOL g_Running = FALSE;

HINSTANCE GetInstance()
{
	return GetModuleHandle(NULL);
}

LRESULT CALLBACK MainWindowProcedure(
	HWND	p_Hwnd,
	UINT	p_Msg,
	WPARAM	p_Wide,
	LPARAM	p_Low	// low � meaing short (considering the term wide)
);

DWORD CreateMainGameWindow(void);

BOOL IsGameAlreadyRunning(void);

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);