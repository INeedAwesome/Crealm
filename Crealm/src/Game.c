#include "Game.h"

void Init(HWND p_hwnd)
{
	m_hwnd = p_hwnd;
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
