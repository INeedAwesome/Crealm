#pragma once

#include <stdio.h>
#pragma warning(push, 2)
#include <Windows.h>
#pragma warning(pop)

#include "Common.h"

#define GAME_NAME L"Crealm"

#define GAME_RES_WIDTH						384
#define GAME_RES_HEIGHT						240
#define GAME_BPP							32	// Bits Per Pixel
#define GAME_DRAWING_AREA_MEMORY_SIZE		(GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 4 /* 4 bytes per pixel */ ) )

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

HWND m_hwnd;
GAMEBITMAP m_backBuffer;
MONITORINFO m_monitorInfo;

int32_t m_monitorWidth;
int32_t m_monitorHeight;

int32_t InitializeGame(HWND p_hwnd, MONITORINFO p_monitorInfo); // Ran after window but before loop
void TerminateGame(void);

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);

