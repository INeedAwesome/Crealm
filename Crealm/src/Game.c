#include "Game.h"
#include "Common.h"

int InitializeGame(HWND p_hwnd)
{
	m_hwnd = p_hwnd;

	m_backBuffer.BitmapInfo.bmiHeader.biSize = sizeof(m_backBuffer.BitmapInfo.bmiHeader);
	m_backBuffer.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
	m_backBuffer.BitmapInfo.bmiHeader.biHeight= GAME_RES_HEIGHT;
	m_backBuffer.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
	m_backBuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_backBuffer.BitmapInfo.bmiHeader.biPlanes = 1;
	
	m_backBuffer.Memory = VirtualAlloc(NULL/*Windows will create addresses for us*/, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!m_backBuffer.Memory)
	{
		CRSetLastError(ERROR_NO_MEMORY);
		MessageBox(NULL, L"Failed to allocate memory for drawing surface!", L"Error!", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		return FAILED;
	}
	return SUCCESS;
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
	HDC deviceContext = GetDC(m_hwnd);

	ReleaseDC(m_hwnd, deviceContext);
}
