#pragma once
#include "Game.h"
#include "Core.h"

#define TOSTRING(X) #X
#define CALCULATE_AVG_FPS_EVERY_X_FPS 30 
#define TARGET_MICROSECONDS_PER_FRAME 6060

HWND g_hwnd;
BOOL g_Running = FALSE;

int APIENTRY WinMain(	// Main entrypoint for the application
	_In_     HINSTANCE p_Instance,
	_In_opt_ HINSTANCE p_PreviousInstance,
	_In_     LPSTR p_CommandLine,
	_In_     int p_ShowCommand
);

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

int32_t CRGetMonitorInfoForWindow(void);

int32_t CRSetWindowPosition(void);

int32_t CRQueryPerformanceFrequency(void);
