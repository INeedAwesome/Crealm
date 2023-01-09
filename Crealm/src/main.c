#include "Main.h"
#include "Common.h"

int32_t APIENTRY WinMain(	// Main entrypoint for the application
	_In_     HINSTANCE p_Instance,
	_In_opt_ HINSTANCE p_PreviousInstance,
	_In_     LPSTR p_CommandLine,
	_In_     int p_ShowCommand)
{
	if (IsGameAlreadyRunning())
		return ERROR_GAME_ALREADY_RUNNING;
	
	if (CRCreateMainGameWindow() == CRFAILED)
		return CRGetLastError();

	if (CRGetMonitorInfoForWindow() == CRFAILED)
		return CRGetLastError();	

	if (CRSetWindowPosition() == CRFAILED)
		return CRGetLastError();

	CRQueryPerformanceFrequency();
	
	if (InitializeGame(g_hwnd) == CRFAILED)
		return CRGetLastError();

	int64_t frameStart = 0;
	int64_t frameEnd = 0;
	int64_t elapsedMicrosecondsPerFrame = 0;
	int64_t elapsedMicrosecondsPerFrameAccumulatorRaw = 0;
	int64_t elapsedMicrosecondsPerFrameAccumulatorCooked = 0;

	MSG message = { 0 };
	g_Running = TRUE;

	while (g_Running)	// Message / Window loop
	{
		QueryPerformanceCounter(&frameStart);

		while (PeekMessage(&message, g_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		
		// process player input
		ProcessPlayerInput();

		// update game

		// render game
		RenderFrameGraphics();

		QueryPerformanceCounter(&frameEnd);

		elapsedMicrosecondsPerFrame = (frameEnd - frameStart);
		elapsedMicrosecondsPerFrame *= 1000000;
		elapsedMicrosecondsPerFrame /= g_gamePerformanceData.PerformanceFrequency;

		g_gamePerformanceData.TotalFramesRendered++;
		elapsedMicrosecondsPerFrameAccumulatorRaw += elapsedMicrosecondsPerFrame;

		while (elapsedMicrosecondsPerFrame <= TARGET_MICROSECONDS_PER_FRAME)
		{
			Sleep(0);
			elapsedMicrosecondsPerFrame = frameEnd - frameStart;
			elapsedMicrosecondsPerFrame *= 1000000;
			elapsedMicrosecondsPerFrame /= g_gamePerformanceData.PerformanceFrequency;

			QueryPerformanceCounter(&frameEnd);
		}

		elapsedMicrosecondsPerFrameAccumulatorCooked += elapsedMicrosecondsPerFrame;

		if ((g_gamePerformanceData.TotalFramesRendered % CALCULATE_AVG_FPS_EVERY_X_FPS) == 0)
		{
			g_gamePerformanceData.RawFPSAverage = (1.0f / ((elapsedMicrosecondsPerFrameAccumulatorRaw / CALCULATE_AVG_FPS_EVERY_X_FPS) * 0.000001f));
			g_gamePerformanceData.CookedFPSAverage = (1.0f / ((elapsedMicrosecondsPerFrameAccumulatorCooked / CALCULATE_AVG_FPS_EVERY_X_FPS) * 0.000001f));

			elapsedMicrosecondsPerFrameAccumulatorRaw = 0;
			elapsedMicrosecondsPerFrameAccumulatorCooked = 0;
		}
	}

	TerminateGame();
	return 0; 
}

LRESULT CALLBACK MainWindowProcedure(
	HWND	p_Hwnd, 
	UINT	p_Msg, 
	WPARAM	p_Wide, 
	LPARAM	p_Low )
{
	switch (p_Msg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			g_Running = FALSE;
		}
	default:
		return DefWindowProc(p_Hwnd, p_Msg, p_Wide, p_Low);
	}
}

BOOL IsGameAlreadyRunning(void)
{
	HANDLE mutex = { 0 };
	mutex = CreateMutex(NULL, GAME_NAME, L"_GameMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 1; // true
	}
	return 0; // false
}


int32_t CRCreateMainGameWindow()
{
	WNDCLASS windowClass; // initialize window class
	ZeroMemory(&windowClass, sizeof(windowClass));
	{
		windowClass.lpfnWndProc = MainWindowProcedure;
		windowClass.style = 0;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetInstance();
		windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = GAME_NAME;
	}

	if (!(RegisterClass(&windowClass))) // register the window class
	{
		CRSetLastError(ERROR_WINDOW_REGISTRATION);
		MessageBox(NULL, L"Window registration failed!", L"Error", MB_OK | MB_ICONERROR);
		return CRFAILED;
	}

	g_hwnd = CreateWindow(					// Creating the window
		windowClass.lpszClassName,
		windowClass.lpszClassName,			// Window name
		WS_OVERLAPPEDWINDOW,				// Styles 
		CW_USEDEFAULT, CW_USEDEFAULT,		// Pos x, y
		768, 432,							// Width, Height
		NULL, NULL,
		GetInstance(),
		NULL
	);

	if (!g_hwnd) // return if the window creation wasn't successful
	{
		CRSetLastError(ERROR_WINDOW_CREATION);
		MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
		return CRFAILED;
	}
	ShowWindow(g_hwnd, SW_SHOW);
	
	return CRSUCCESS;
}

int32_t CRGetMonitorInfoForWindow()
{
	g_gamePerformanceData.MonitorInfo.cbSize = sizeof(g_gamePerformanceData.MonitorInfo);
	if (GetMonitorInfoW(MonitorFromWindow(g_hwnd, MONITOR_DEFAULTTOPRIMARY), &g_gamePerformanceData.MonitorInfo) == 0/*failed*/)
	{
		CRSetLastError(ERROR_WINDOW_MONITOR_INFO);
		return CRFAILED;
	}

	g_gamePerformanceData.MonitorWidth = g_gamePerformanceData.MonitorInfo.rcMonitor.right - g_gamePerformanceData.MonitorInfo.rcMonitor.left;
	g_gamePerformanceData.MonitorHeight = g_gamePerformanceData.MonitorInfo.rcMonitor.bottom - g_gamePerformanceData.MonitorInfo.rcMonitor.top;

	return CRSUCCESS;
}

int32_t CRSetWindowPosition()
{
	// WS_OVERLAPPEDWINDOW — example 0x01101101
	// ~WS_OVERLAPPED // flipping
	// // 0x10010010 // its flipped


	if (SetWindowLongPtr(g_hwnd, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_OVERLAPPEDWINDOW) == 0)
	{
		CRSetLastError(ERROR_WINDOW_SET_STYLE);
		return CRFAILED;
	}

	if (SetWindowPos(g_hwnd, HWND_TOP,
		g_gamePerformanceData.MonitorInfo.rcMonitor.left, g_gamePerformanceData.MonitorInfo.rcMonitor.top,
		g_gamePerformanceData.MonitorWidth, g_gamePerformanceData.MonitorHeight,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
	{
		CRSetLastError(ERROR_WINDOW_SET_POS);
		return CRFAILED;
	}
	return CRSUCCESS;
}

int32_t CRQueryPerformanceFrequency(void)
{
	QueryPerformanceFrequency(&g_gamePerformanceData.PerformanceFrequency);
	return CRSUCCESS;
}

