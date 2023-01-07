#include "Game.h"

int InitializeGame(HWND p_hwnd)
{
	m_hwnd = p_hwnd;

	m_drawingSurface.BitmapInfo.bmiHeader.biSize = sizeof(m_drawingSurface.BitmapInfo.bmiHeader);
	m_drawingSurface.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
	m_drawingSurface.BitmapInfo.bmiHeader.biHeight= GAME_RES_HEIGHT;
	m_drawingSurface.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
	m_drawingSurface.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_drawingSurface.BitmapInfo.bmiHeader.biPlanes = 1;
	
	m_drawingSurface.Memory = VirtualAlloc(NULL/*Windows will create addresses for us*/, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!m_drawingSurface.Memory)
	{
		MessageBox(NULL, L"Failed to allocate memory for drawing surface!", L"Error!", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		return 10;
	}
	return 0;
}

void ProcessPlayerInput(void)
{
	short escapeKeyDown = GetAsyncKeyState(VK_ESCAPE);
	if (escapeKeyDown)
	{
		PostMessage(m_hwnd, WM_CLOSE, 0, 0);
	}

}

void RenderFrameGraphics(void)
{

}
