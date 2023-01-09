#pragma once

#include <stdio.h>
#include "Core.h"

#define GAME_NAME L"Crealm"

#define GAME_RES_WIDTH						384
#define GAME_RES_HEIGHT						240
#define GAME_BPP							32	// Bits Per Pixel
#define GAME_DRAWING_AREA_MEMORY_SIZE		(GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 4 /* 4 bytes per pixel */ ) )

HWND m_hwnd;
GAMEBITMAP m_backBuffer;

int32_t m_monitorWidth;
int32_t m_monitorHeight;

int32_t InitializeGame(HWND p_hwnd); // Ran after window but before loop
void TerminateGame(void);

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);

