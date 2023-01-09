#pragma once


#pragma warning(push, 2)
#include <Windows.h>
#pragma warning(pop)

#include "Common.h"

typedef struct GAME_PERFORMANCE_DATA
{
	uint64_t TotalFramesRendered;
	float RawFPSAverage; // game loop speed
	float CookedFPSAverage; // as close to 60/144/165/user value
	int64_t PerformanceFrequency;

	MONITORINFO MonitorInfo;
	int32_t MonitorWidth;
	int32_t MonitorHeight;
	BOOL DisplayDebugInfo;

} GAME_PERFORMANCE_DATA;


typedef struct GAMEBITMAP
{
	BITMAPINFO BitmapInfo;
	void* Memory;
} GAMEBITMAP;

typedef struct PIXEL32
{
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;
} PIXEL32;


// variables
GAME_PERFORMANCE_DATA g_gamePerformanceData;