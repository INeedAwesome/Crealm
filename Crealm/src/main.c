#include <stdio.h>

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

HWND g_hwnd;

LRESULT CALLBACK MainWindowProcedure(
	HWND	p_Hwnd,
	UINT	p_Msg,
	WPARAM	p_Wide,
	LPARAM	p_Low
);

int APIENTRY WinMain(
	_In_     HINSTANCE p_Instance,
	_In_opt_ HINSTANCE p_PreviousInstance,
	_In_     LPSTR p_CommandLine,
	_In_     int p_ShowCommand)
{
	UNREFERENCED_PARAMETER(p_Instance);
	UNREFERENCED_PARAMETER(p_PreviousInstance);
	UNREFERENCED_PARAMETER(p_CommandLine);
	UNREFERENCED_PARAMETER(p_ShowCommand);

	WNDCLASS windowClass;
	ZeroMemory(&windowClass, sizeof(windowClass));
	{
		windowClass.lpfnWndProc = MainWindowProcedure;
		windowClass.style = 0;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = p_Instance;
		windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		windowClass.hCursor = LoadCursor(NULL, IDC_WAIT);
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = L"Hello Win32";
	}

	if (!(RegisterClass(&windowClass)))
	{
		MessageBox(NULL, L"Window registration failed!", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	g_hwnd = CreateWindow(windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, p_Instance, NULL);

	if (!g_hwnd)
	{
		MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
		return 2;
	}
	ShowWindow(g_hwnd, SW_SHOW);


	MSG message;
	ZeroMemory(&message, sizeof(message));

	while (GetMessage(&message, NULL, NULL, NULL) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

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
		}
		case WM_PAINT: 
		{

		}

	default:
		return DefWindowProc(p_Hwnd, p_Msg, p_Wide, p_Low);
	}
}