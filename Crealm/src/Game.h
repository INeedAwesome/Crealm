#pragma once

#include <stdio.h>
#pragma warning(push, 2)
#include <Windows.h>
#pragma warning(pop)

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

HWND m_hwnd;
GAMEBITMAP m_backBuffer;

int InitializeGame(HWND p_hwnd); // Ran after window but before loop

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);

