#include "Main.h"
#include "Common.h"

int APIENTRY WinMain(	// Main entrypoint for the application
	_In_     HINSTANCE p_Instance,
	_In_opt_ HINSTANCE p_PreviousInstance,
	_In_     LPSTR p_CommandLine,
	_In_     int p_ShowCommand)
{
	if (IsGameAlreadyRunning())
		return ERROR_GAME_ALREADY_RUNNING;
	
	if (CRCreateMainGameWindow() == FAILED)
		return CRGetLastError();

	if (CRGetMonitorInfoForWindow() == FAILED)
		return CRGetLastError();	

	if (CRSetWindowPosition() == FAILED)
		return CRGetLastError();

	CRQueryPerformanceFrequency();
	
	if (InitializeGame(g_hwnd, g_gamePerformanceData.g_monitorInfo) == FAILED)
		return CRGetLastError();

	int64_t FrameStart = 0;
	int64_t FrameEnd = 0;
	int64_t ElapsedMicrosecondsPerFrame = 0;
	int64_t ElapsedMicrosecondsPerFrameAccumulator = 0;

	MSG message = { 0 };
	g_Running = TRUE;

	while (g_Running)	// Message / Window loop
	{
		QueryPerformanceCounter(&FrameStart);

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

		QueryPerformanceCounter(&FrameEnd);

		ElapsedMicrosecondsPerFrame = (FrameEnd - FrameStart);

		ElapsedMicrosecondsPerFrame *= 1000000;
		ElapsedMicrosecondsPerFrame /= g_gamePerformanceData.PerformanceFrequency;

		Sleep(1);

		g_gamePerformanceData.TotalFramesRendered++;
		ElapsedMicrosecondsPerFrameAccumulator += ElapsedMicrosecondsPerFrame;
		
		if ((g_gamePerformanceData.TotalFramesRendered % CALCULATE_AVG_FPS_EVERY_X_FPS) == 0)
		{
			int64_t averageMicrosecondsPerFrame = ElapsedMicrosecondsPerFrameAccumulator / CALCULATE_AVG_FPS_EVERY_X_FPS;

			char str[64] = { 0 };
			_snprintf_s(str, _countof(str), _TRUNCATE, "Average milliseconds/frame: %02f\n", (float)(averageMicrosecondsPerFrame * 0.001f));
			OutputDebugStringA(str);

			ElapsedMicrosecondsPerFrameAccumulator = 0;
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
		return FAILED;
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
		return FAILED;
	}
	ShowWindow(g_hwnd, SW_SHOW);
	
	return SUCCESS;
}

int32_t CRGetMonitorInfoForWindow()
{
	g_gamePerformanceData.g_monitorInfo.cbSize = sizeof(g_gamePerformanceData.g_monitorInfo);
	if (GetMonitorInfoW(MonitorFromWindow(g_hwnd, MONITOR_DEFAULTTOPRIMARY), &g_gamePerformanceData.g_monitorInfo) == 0/*failed*/)
	{
		CRSetLastError(ERROR_WINDOW_MONITOR_INFO);
		return FAILED;
	}

	g_gamePerformanceData.g_monitorWidth = g_gamePerformanceData.g_monitorInfo.rcMonitor.right - g_gamePerformanceData.g_monitorInfo.rcMonitor.left;
	g_gamePerformanceData.g_monitorHeight = g_gamePerformanceData.g_monitorInfo.rcMonitor.bottom - g_gamePerformanceData.g_monitorInfo.rcMonitor.top;

	return SUCCESS;
}

int32_t CRSetWindowPosition()
{
	// WS_OVERLAPPEDWINDOW — example 0x01101101
	// ~WS_OVERLAPPED // flipping
	// // 0x10010010 // its flipped


	if (SetWindowLongPtr(g_hwnd, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_OVERLAPPEDWINDOW) == 0)
	{
		CRSetLastError(ERROR_WINDOW_SET_STYLE);
		return FAILED;
	}

	if (SetWindowPos(g_hwnd, HWND_TOP,
		g_gamePerformanceData.g_monitorInfo.rcMonitor.left, g_gamePerformanceData.g_monitorInfo.rcMonitor.top, 
		g_gamePerformanceData.g_monitorWidth, g_gamePerformanceData.g_monitorHeight,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
	{
		CRSetLastError(ERROR_WINDOW_SET_POS);
		return FAILED;
	}
	return SUCCESS;
}

int32_t CRQueryPerformanceFrequency(void)
{
	QueryPerformanceFrequency(&g_gamePerformanceData.PerformanceFrequency);
	
}

