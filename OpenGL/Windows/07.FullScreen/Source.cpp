
//Header File Declaration
#include<Windows.h>

//Global Function Declaration 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//First step for fullscreen														Step : 1
// Global Variables Declaration 3
DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_D = {sizeof(WINDOWPLACEMENT)};//store the previous windows placement inform 
bool gbFullScreen_D = false;

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

	//change in winMain																				step 2
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

	//Declare the function or signature or prototype												step 3

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
				case 0x46://																		step3.B
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
//\																								step 4 last 
void ToggelFullScreen()
{
	//Local Variable Declare
	MONITORINFO mi_D = { sizeof(MONITORINFO) };

	//Code
	if (gbFullScreen_D == false)
	{
		//WS_CAPTION | WS_MINIMIZE | WS_MAXIMIZE | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED
		dwStyle_D = GetWindowLong(ghwnd_D, GWL_STYLE);
		if (dwStyle_D & WS_OVERLAPPEDWINDOW)
		{
			  //MonitorFromWindow()       1. ghwnd (windows cha handle ), 2. Monitorinfo_primary ( jyala majha current adapter lavalay to tya monitor chi info de. )
			if (GetWindowPlacement(ghwnd_D, &wpPrev_D) && GetMonitorInfo(MonitorFromWindow(ghwnd_D,MONITORINFOF_PRIMARY),&mi_D))
			{
				SetWindowLong(ghwnd_D, GWL_STYLE, (dwStyle_D &~ WS_OVERLAPPEDWINDOW));

				//										X					Y								WIDTH											
				SetWindowPos(ghwnd_D, HWND_TOP, mi_D.rcMonitor.left, mi_D.rcMonitor.top, mi_D.rcMonitor.right - mi_D.rcMonitor.left,
		//						HEIGHT					Don'tchange Z Order  | SWP_ WM_NCCALCSIZE
				 mi_D.rcMonitor.bottom - mi_D.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // SWP_FRAMECHANGED = WM_NCCALCSIZE      
				 // 														" WM_NCCALSIZE " Ha Msg aaplya window la pathvala jato i.e. aas mhanto ki baba 
				//																hya window cha nonclient are calculate kara 
			}
			
		}
		ShowCursor(FALSE);
		gbFullScreen_D = true;
	}    
	else
	{
		SetWindowLong(ghwnd_D, GWL_STYLE, (dwStyle_D | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd_D, &wpPrev_D);
		//x,ywidth,hight aapan 0 dilet karan aapan wpPrevmadhe aapan dillet  
		SetWindowPos(ghwnd_D, HWND_TOP, 0, 0, 0, 0,
		
		//dont move wnd position (x,y)
		 SWP_NOMOVE |
		//dont change its size (width, height)
		 SWP_NOSIZE |
		//if dady changes its order still you don't changed your.
		 SWP_NOOWNERZORDER |
		//
		SWP_NOZORDER |
		//
		SWP_FRAMECHANGED);
		
		ShowCursor(TRUE);
		gbFullScreen_D = false;
	}

}


