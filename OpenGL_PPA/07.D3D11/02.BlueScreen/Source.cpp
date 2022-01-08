#include<Windows.h>
#include<stdio.h>

#include<d3d11.h>

#pragma comment(lib, "d3d11.lib")

#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

FILE* gpFile_DM = NULL;
char gszLogFileName[] = "Log.txt";

HWND ghwnd_DM = NULL;

DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };

bool gbFullScreen_DM = false;
bool gbEscapeKeyIsPressed = false;
bool gbActiveWindows_DM = false;
bool gFullScreen = false;

float gClearColor[4];
IDXGISwapChain *gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;

HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;

int x, y;

DWORD dwstyle_DM;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //declaration
	HRESULT Initialize(void);
	void unInitialize(void);
	void Display(void);

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassname[] = TEXT("Direct3D11");
	bool bDone = false;
    INT Width = 800;
    INT Height = 600;
    
    //code
	if (fopen_s(&gpFile_DM, gszLogFileName, "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Can not be created..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_TOPMOST);
		exit(0);
	}
	else
	{
        fprintf_s(gpFile_DM, "Log File Is Successfully Opened.\n");
		fclose(gpFile_DM);
	}

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpszClassName = szClassname;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszMenuName = NULL;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);


    RegisterClassEx(&wndclass);

    x = GetSystemMetrics(SM_CXSCREEN);
    y = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
        szClassname,
        TEXT("Darshan's : Direct3D11 Blue Window!"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
        (x / 2) - (Width / 2),
        (y / 2) - (Height / 2),
        Width,
        Height,
        NULL,
        NULL,
        hInstance,
        NULL);

    ghwnd_DM = hwnd;


    ShowWindow(hwnd, iCmdShow);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    HRESULT hr;
    hr = Initialize();
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Initialize() Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Initialize() Succeeded...\n");
        fclose(gpFile_DM);
    }

    while (bDone == false)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bDone = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Display();
            if (gbActiveWindows_DM = true)
            {
                if (gbEscapeKeyIsPressed == true)
                    bDone = true;
            }
        }

    }

    unInitialize();

    return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //Function
    HRESULT Resize(int, int);
    void ToggelFullScreen(void);
    void unInitialize(void);
    HRESULT hr;

    //code
    switch (iMsg)
    {
    case WM_PAINT:
       
        break;

    case WM_SETFOCUS:
        gbActiveWindows_DM = true;
        break;
    case WM_KILLFOCUS:
        gbActiveWindows_DM = false;
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            if (gbEscapeKeyIsPressed == false)
                gbEscapeKeyIsPressed = true;
            break;

        case 0x46:
        case 0x66:
            if (gFullScreen == false)
            {
                ToggelFullScreen();
                gbFullScreen_DM = true;
            }
            else
            {
                ToggelFullScreen();
                gbFullScreen_DM = false;
            }
            break;
        default:
            break;
        }
        break;
    case WM_SIZE:
        if (gpID3D11DeviceContext)
        {
            hr = Resize(LOWORD(lParam), HIWORD(lParam));
            if (FAILED(hr))
            {
                fopen_s(&gpFile_DM, gszLogFileName, "a+");
                fprintf_s(gpFile_DM, "Resize() Failed. Exiting Now...\n");
                fclose(gpFile_DM);
                return(hr);
            }
            else
            {
                fopen_s(&gpFile_DM, gszLogFileName, "a+");
                fprintf_s(gpFile_DM, "Resize() Succeeded...\n");
                fclose(gpFile_DM);
            }


        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
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
    if (gbFullScreen_DM == false)
    {
        dwStyle_D = GetWindowLong(ghwnd_DM, GWL_STYLE);
        if (dwStyle_D & WS_OVERLAPPEDWINDOW)
        {
            if (GetWindowPlacement(ghwnd_DM, &wpPrev_DM) && GetMonitorInfo(MonitorFromWindow(ghwnd_DM, MONITORINFOF_PRIMARY), &mi_D))
            {
                SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_D & ~WS_OVERLAPPEDWINDOW));
                SetWindowPos(ghwnd_DM, HWND_TOP, mi_D.rcMonitor.left, mi_D.rcMonitor.top, mi_D.rcMonitor.right - mi_D.rcMonitor.left, mi_D.rcMonitor.bottom - mi_D.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // SWP_FRAMECHANGED = WM_NCCALCSIZE      
            }

        }
        ShowCursor(FALSE);
        gbFullScreen_DM  = true;
    }
    else
    {
        SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_D | WS_OVERLAPPEDWINDOW));
        SetWindowPlacement(ghwnd_DM, &wpPrev_DM);
        SetWindowPos(ghwnd_DM, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
        gbFullScreen_DM  = false;
    }

}

HRESULT Initialize()
{
    //Function Declaration
    void unInitialize(void);
    HRESULT Resize(int, int);

    //Variable Declaration
    HRESULT hr;
    D3D_DRIVER_TYPE d3dDriverType;
    D3D_DRIVER_TYPE d3dDriverTypes[] =
    { D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE };
    D3D_FEATURE_LEVEL d3dFeatureLevel_required = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL d3dFeatureLevel_acquired = D3D_FEATURE_LEVEL_10_0;

    UINT createDeviceFlags = 0;
    UINT numDriverType = 0;
    UINT numFeatureLevels = 1;


    //code 
    numDriverType = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);

    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;

    ZeroMemory((void*)&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    dxgiSwapChainDesc.BufferCount = 1;
    dxgiSwapChainDesc.BufferDesc.Width = WIN_WIDTH_DM;
    dxgiSwapChainDesc.BufferDesc.Height = WIN_HEIGHT_DM;
    dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    dxgiSwapChainDesc.OutputWindow = ghwnd_DM;
    dxgiSwapChainDesc.SampleDesc.Count = 1;
    dxgiSwapChainDesc.SampleDesc.Quality = 0;
    dxgiSwapChainDesc.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverType; driverTypeIndex++)
    {
        d3dDriverType = d3dDriverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            d3dDriverType,
            NULL,
            createDeviceFlags,
            &d3dFeatureLevel_required,
            numFeatureLevels,   
            D3D11_SDK_VERSION,
            &dxgiSwapChainDesc,
            &gpIDXGISwapChain,
            &gpID3D11Device,
            &d3dFeatureLevel_acquired,
            &gpID3D11DeviceContext);


        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11CreateDeviceAndSwapChain() Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11CreateDeviceAndSwapChain() Succeeded...\n");
        fprintf_s(gpFile_DM, "The Chosen Driver Is Of \n");
        if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
        {
            fprintf_s(gpFile_DM, "Hardware Type.\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
        {
            fprintf_s(gpFile_DM, "Warp Type.\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
        {
            fprintf_s(gpFile_DM, "Reference Type.\n");
        }
        else
        {
            fprintf_s(gpFile_DM, " unknown Type.\n");
        }

        fprintf_s(gpFile_DM, "\nThe Supported Highest Feature level Is \n");
        if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_11_0)
        {
            fprintf_s(gpFile_DM, "11.0.\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_1)
        {
            fprintf_s(gpFile_DM, "10.1.\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_0)
        {
            fprintf_s(gpFile_DM, "10.0.\n");
        }
        else
        {
            fprintf_s(gpFile_DM, " unknown.\n");
        }

        fclose(gpFile_DM);
    }
    gClearColor[0] = 0.0f;
    gClearColor[1] = 0.0f;
    gClearColor[2] = 1.0f;
    gClearColor[3] = 1.0f;

    hr = Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM);
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Resize() Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Resize() Succeeded...\n");
        fclose(gpFile_DM);
    }

    return (S_OK);
    
}

HRESULT Resize(int width, int height)
{
    //code
    HRESULT hr = S_OK;

    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    ID3D11Texture2D* pID3D11Texture2D_BackBuffer;

    gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (LPVOID*)&pID3D11Texture2D_BackBuffer);
    
    hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer,
        NULL,
        &gpID3D11RenderTargetView);
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateRenderTargetView() Failed.Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateRenderTargetView() Succeeded...\n");
        fclose(gpFile_DM);
    }
    pID3D11Texture2D_BackBuffer->Release();
    pID3D11Texture2D_BackBuffer = NULL;
        
    gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);

    D3D11_VIEWPORT d3dViewPort;
    d3dViewPort.TopLeftX = 0;
    d3dViewPort.TopLeftY = 0;
    d3dViewPort.Width = (float)width;
    d3dViewPort.Height = (float)height;
    d3dViewPort.MinDepth = 0.0f;
    d3dViewPort.MaxDepth = 1.0f;
    
    gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);

    return(hr);
}

void Display()
{
    //code
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);

  

    gpIDXGISwapChain->Present(0, 0);
}

void unInitialize()
{
    //code
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }
    if (gpIDXGISwapChain)
    {
        gpIDXGISwapChain->Release();
        gpIDXGISwapChain = NULL;
    }
    if (gpID3D11Device)
    {
        gpID3D11Device->Release();
        gpID3D11Device = NULL;
    }
    if (gpFile_DM)
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "UnInitialize() Succeeded.\n");
        fprintf_s(gpFile_DM, "Log File is Successfully Close.\n");
        fclose(gpFile_DM);
        gpFile_DM = NULL;
    }

}


