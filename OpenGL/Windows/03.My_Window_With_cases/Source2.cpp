#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdLine, int CmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG imsg;
	TCHAR AppName[] = TEXT("MyClass");

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = AppName;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(AppName,
		TEXT("Windows Cases."),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, CmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&imsg, NULL,0,0))
	{
		TranslateMessage(&imsg);
		DispatchMessage(&imsg);
	}
	return((int)imsg.wParam);
	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)
{
	switch (imsg)
	{
	case WM_CREATE:
		MessageBox(hwnd, TEXT("WM_CREATE Is Reised..!"), TEXT("MY Message"), MB_OK);
		break;
	case WM_KEYDOWN:
		MessageBox(hwnd, TEXT("WM_KEYDOWN Is Reised..!"), TEXT("MY Message"), MB_OK);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("WM_LBUTTONDOWN Is Reised..!"), TEXT("MY Message"), MB_OK);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	}

	return(DefWindowProc(hwnd, imsg, wParam, lParam));
}

