#include "Game.h"

int InitializeGame(HWND p_hwnd)
{
	m_hwnd = p_hwnd;
	m_monitorWidth = g_gamePerformanceData.MonitorInfo.rcMonitor.right - g_gamePerformanceData.MonitorInfo.rcMonitor.left;
	m_monitorHeight = g_gamePerformanceData.MonitorInfo.rcMonitor.bottom - g_gamePerformanceData.MonitorInfo.rcMonitor.top;

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
		return CRFAILED;
	}
	// VirtualAlloc zeroes out the buffer so we don't need to use ZeroMemory()



	return CRSUCCESS;
}

void TerminateGame(void)
{
	VirtualFree(m_backBuffer.Memory, 0, MEM_RELEASE);
}

void ProcessPlayerInput(void)
{
	int16_t escapeKeyDown = GetAsyncKeyState(VK_ESCAPE);
	if (escapeKeyDown)
	{
		PostMessage(m_hwnd, WM_CLOSE, 0, 0);
	}

	int16_t debugKeyDown = GetAsyncKeyState(VK_F3);
	if (debugKeyDown)
	{
		g_gamePerformanceData.DisplayDebugInfo = !g_gamePerformanceData.DisplayDebugInfo;
	}

}

void RenderFrameGraphics(void)
{
	//memset(m_backBuffer.Memory, 0x0F, GAME_DRAWING_AREA_MEMORY_SIZE);

	PIXEL32 pixel = { 0 };
	{
		pixel.Blue = 0x0F;
		pixel.Green = 0xAF;
		pixel.Red = 0x0F;
		pixel.Alpha = 0xFF;
	}

	for (int32_t i = 0; i < GAME_RES_WIDTH * GAME_RES_HEIGHT; i++)
	{
		memcpy_s((PIXEL32*) m_backBuffer.Memory + i, sizeof(PIXEL32), &pixel, sizeof(PIXEL32));
	}

	HDC deviceContext = GetDC(m_hwnd);

	StretchDIBits(deviceContext, 
		0, 0, 
		m_monitorWidth, m_monitorHeight,
		0, 0, 
		GAME_RES_WIDTH, GAME_RES_HEIGHT,
		m_backBuffer.Memory, 
		&m_backBuffer.BitmapInfo, 
		DIB_RGB_COLORS, 
		SRCCOPY
	);

	SelectObject(deviceContext, (HFONT)GetStockObject(ANSI_FIXED_FONT));

	if (g_gamePerformanceData.DisplayDebugInfo)
	{
		char textOutBuffer[64] = { 0 };

		sprintf_s(textOutBuffer, _countof(textOutBuffer), "FPS Cooked: %.01f", g_gamePerformanceData.CookedFPSAverage);
		TextOutA(deviceContext, 10, 10, textOutBuffer, (int)strlen(textOutBuffer));

		sprintf_s(textOutBuffer, _countof(textOutBuffer), "FPS Raw: %.01f", g_gamePerformanceData.RawFPSAverage);
		TextOutA(deviceContext, 10, 34, textOutBuffer, (int)strlen(textOutBuffer));

		ReleaseDC(m_hwnd, deviceContext);
	}
	
}
