#include<windows.h>
#include"Header.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdline, int iCmdShow)
{
	WNDCLASSEX wndclassex;
	MSG Msg;
	HWND hwnd;
	TCHAR szClassName[] = TEXT("Implicite Linking..!");
	

	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.style = CS_HREDRAW | CS_VREDRAW;
	wndclassex.cbClsExtra = 0;
	wndclassex.cbWndExtra = 0;
	wndclassex.lpfnWndProc = WndProc;
	wndclassex.hInstance = hInstance;   
	wndclassex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); 
	wndclassex.hCursor = LoadCursor(NULL, IDC_HAND);
	wndclassex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclassex.lpszClassName = szClassName;
	wndclassex.lpszMenuName = NULL;
	wndclassex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	RegisterClassEx(&wndclassex);

	hwnd = CreateWindow(szClassName,
		TEXT("Hello Darshan..!"),
		WS_OVERLAPPEDWINDOW,//WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_THICKFRAME,// WS_OVERLAPPEDWINDOW  *********************************TYPE IS DWORD
		200,
		200,
		500,
		500,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow); 
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0))
	{

		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return((int)Msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)
{
	int x = 0, y = 0;
	TCHAR str[255];
	
	// code
	switch (imsg)
	{
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, imsg, wParam, lParam));
}


