
//Header File Declaration
#include<Windows.h>

//Global Function Declaration 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Global Variables Declaration 
DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_D = {sizeof(WINDOWPLACEMENT)};
bool gdFullScreen_D = false;
HWND ghwnd_D = NULL;



//WinMain Start
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR LpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyFullScreen");
	INT x, y;
	INT widtth = 800;
	INT Height = 600;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);
	x = GetSystemMetrics(SM_CXSCREEN);
	y = GetSystemMetrics(SM_CYSCREEN);



	hwnd = CreateWindow(szAppName,
		TEXT("My Full Screen"),
		WS_OVERLAPPEDWINDOW,
		(x / 2) - (widtth / 2),
		(y / 2) - (Height / 2),
		widtth,
		Height,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ghwnd_D = hwnd;

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//Local Declaration
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;
	TCHAR hw[] = TEXT("Hello World !");


	//Local Function 
	void ToggelFullScreen(void);


	switch (iMsg)
	{
		case WM_PAINT:
			GetClientRect(hwnd, &rc);
			hdc = BeginPaint(hwnd, &ps);
			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(0, 255, 0));
			DrawText(hdc, hw, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hwnd, &ps);
		break;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case 0x46:
				case 0x66:
					ToggelFullScreen();
					break;
			default:
				break;
			}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggelFullScreen()
{
	//Local Variable Declare
	MONITORINFO mi_D = { sizeof(MONITORINFO) };

	//Code
	if (gdFullScreen_D == false)
	{
		dwStyle_D = GetWindowLong(ghwnd_D, GWL_STYLE);
		if (dwStyle_D & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd_D, &wpPrev_D) && GetMonitorInfo(MonitorFromWindow(ghwnd_D,MONITORINFOF_PRIMARY),&mi_D))
			{
				SetWindowLong(ghwnd_D, GWL_STYLE, (dwStyle_D &~ WS_OVERLAPPEDWINDOW));
				SetWindowPos(ghwnd_D, HWND_TOP, mi_D.rcMonitor.left, mi_D.rcMonitor.top, mi_D.rcMonitor.right - mi_D.rcMonitor.left, mi_D.rcMonitor.bottom - mi_D.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // SWP_FRAMECHANGED = WM_NCCALCSIZE      
			}
			
		}
		ShowCursor(FALSE);
		gdFullScreen_D = true;
	}
	else
	{
		SetWindowLong(ghwnd_D, GWL_STYLE, (dwStyle_D | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd_D, &wpPrev_D);
		SetWindowPos(ghwnd_D, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gdFullScreen_D = false;
	}

}


