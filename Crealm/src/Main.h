#pragma once

#include <stdio.h>

#pragma warning(push, 2)
#include <Windows.h>
#pragma warning(pop)

#include "Game.h"

#define TOSTRING(X) #X
#define CALCULATE_AVG_FPS_EVERY_X_FPS 100


typedef struct GAME_PERFORMANCE_DATA
{
	uint64_t TotalFramesRendered;
	uint32_t RawFPSAverage; // game loop speed
	uint32_t CookedFPSAverage; // as close to 60/144/165/user value
	int64_t PerformanceFrequency;
	int64_t ElapsedMicrosecondsPerFrame;

	MONITORINFO g_monitorInfo;
	int32_t g_monitorWidth;
	int32_t g_monitorHeight;

} GAME_PERFORMANCE_DATA;

HWND g_hwnd;
BOOL g_Running = FALSE;
GAME_PERFORMANCE_DATA g_gamePerformanceData;

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
