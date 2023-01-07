#pragma once

#include <stdio.h>
#pragma warning(push, 2)
#include <Windows.h>
#pragma warning(pop)

#define GAME_NAME L"Crealm"

HWND m_hwnd;

void Init(HWND p_hwnd);

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);

