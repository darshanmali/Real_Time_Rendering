//Header Files

#include<windows.h>
#include<stdio.h>
#include "Header.h"
#include<gl/GL.h>

#pragma comment(lib,"OpenGL32.lib")


//New Micros
#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

//Global Function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Initialize(void);
void Display(void);
void Resize(int, int);
void unInitialize(void);

///Global Variable Declaration For Full screen.
DWORD dwStyle_DM;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };
bool gdFullScreen_DM = false;
HWND ghwnd_DM = NULL;
HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;
bool gbActiveWindow_DM = false;
FILE* gpFile_DM = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpszCmdLine, int iCmdShow)
{
    //Function Delcaration
  
    //Variable Delcaration
    WNDCLASSEX wndclassex_DM;
    HWND hwnd_DM;
    MSG msg_DM;
    TCHAR Appname[] = TEXT("My Propriotary");
    
    bool bDone_DM = false;
    INT x = 0, y = 0;

    if (fopen_s(&gpFile_DM, "Log.txt", "w"))
    {
        MessageBox(NULL, TEXT("Can not open Desired File !"), TEXT("Error!"), MB_OK);
        exit(0);
    }

    wndclassex_DM.cbSize = sizeof(WNDCLASSEX);
    wndclassex_DM.cbClsExtra = 0;
    wndclassex_DM.cbWndExtra = 0;
    wndclassex_DM.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclassex_DM.lpszClassName = Appname;
    wndclassex_DM.lpfnWndProc = WndProc;
    wndclassex_DM.lpszMenuName = NULL;
    wndclassex_DM.hInstance = hInstance;
    wndclassex_DM.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclassex_DM.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclassex_DM.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclassex_DM.hCursor = LoadCursor(NULL, IDC_ARROW);


    RegisterClassEx(&wndclassex_DM);


    x = GetSystemMetrics(SM_CXSCREEN);
    y = GetSystemMetrics(SM_CYSCREEN);

    hwnd_DM = CreateWindowEx(WS_EX_APPWINDOW,
        Appname,
        TEXT("proprietary !"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
        100,
        100,
        WIN_WIDTH_DM,
        WIN_HEIGHT_DM,
        NULL,
        NULL,
        hInstance,
        NULL);
    
    ghwnd_DM = hwnd_DM;

    Initialize();


    ShowWindow(hwnd_DM, iCmdShow);
    SetForegroundWindow(hwnd_DM);
    SetFocus(hwnd_DM);

    //Game Loop
    while (bDone_DM == false)
    {
        if (PeekMessage(&msg_DM, NULL, 0, 0, PM_REMOVE))
        {
            if (msg_DM.message == WM_QUIT)
            {
                bDone_DM = true;
            }
            else
            {
                TranslateMessage(&msg_DM);
                DispatchMessage(&msg_DM);
            }
        }
        else
        {
            
            if (gbActiveWindow_DM == true)
            {
                // Here You should call Update Fuction for openGl Redering

                // Here You should call Display Fuction for openGl Redering

                //Display();
            }
        }
    }

    return(msg_DM.wParam);

}


LRESULT CALLBACK WndProc(HWND hwnd_DM, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //Local Declaration

   


    //Local Function 
    void ToggelFullScreen(void);
    

    switch (iMsg)
    {
    case WM_PAINT:
        Display();
        break;
   
    case WM_SETFOCUS:
        gbActiveWindow_DM = true;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow_DM = false;
        break;
  //  case WM_ERASEBKGND:
   //     return(0);

    case WM_SIZE:
        Resize(LOWORD(lParam), HIWORD(lParam));
        break;

    case VK_ESCAPE:
        DestroyWindow(hwnd_DM);
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

    case WM_CLOSE:
        DestroyWindow(hwnd_DM);
        break;

    case WM_DESTROY:
        unInitialize();
        PostQuitMessage(0);
        break;
    }

    return(DefWindowProc(hwnd_DM, iMsg, wParam, lParam));
}


void ToggelFullScreen()
{
    //Local Variable Declare
    MONITORINFO mi_DM = { sizeof(MONITORINFO) };

    //Code
    if (gdFullScreen_DM == false)
    {
        dwStyle_DM = GetWindowLong(ghwnd_DM, GWL_STYLE);
        if (dwStyle_DM & WS_OVERLAPPEDWINDOW)
        {
            if (GetWindowPlacement(ghwnd_DM, &wpPrev_DM) && GetMonitorInfo(MonitorFromWindow(ghwnd_DM, MONITORINFOF_PRIMARY), &mi_DM))
            {
                SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_DM &~ WS_OVERLAPPEDWINDOW));

                SetWindowPos(ghwnd_DM, HWND_TOP, mi_DM.rcMonitor.left, mi_DM.rcMonitor.top, mi_DM.rcMonitor.right - mi_DM.rcMonitor.left, mi_DM.rcMonitor.bottom - mi_DM.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // SWP_FRAMECHANGED = WM_NCCALCSIZE      
            }

        }
        ShowCursor(FALSE);
        gdFullScreen_DM = true;
    }
    else
    {
        SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_DM | WS_OVERLAPPEDWINDOW));
        SetWindowPlacement(ghwnd_DM, &wpPrev_DM);
        SetWindowPos(ghwnd_DM, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
        gdFullScreen_DM = false;
    }

}


void Initialize()
{
    //Function Declaration
   
    //Variable Declaration
    PIXELFORMATDESCRIPTOR pfd_DM;
    int iPixelFormatIndex_DM;

    //Code
    ghdc_DM = GetDC(ghwnd_DM);
    
    ZeroMemory(&pfd_DM, sizeof(PIXELFORMATDESCRIPTOR));
    
    pfd_DM.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd_DM.nVersion = 1;
    pfd_DM.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd_DM.iPixelType = PFD_TYPE_RGBA;
    pfd_DM.cColorBits = 32;
    pfd_DM.cRedBits = 8;
    pfd_DM.cGreenBits = 8;
    pfd_DM.cBlueBits = 8;
    pfd_DM.cAlphaBits = 8;

    iPixelFormatIndex_DM = ChoosePixelFormat(ghdc_DM, &pfd_DM);

    if (iPixelFormatIndex_DM == 0)
    {
        fprintf_s(gpFile_DM, "ChoosePixelFormat faild.\n");
        DestroyWindow(ghwnd_DM);
    }
    
    if (SetPixelFormat(ghdc_DM, iPixelFormatIndex_DM,&pfd_DM) == FALSE)
    {
        fprintf_s(gpFile_DM, "SetPixelFormat Faild.\n");
        DestroyWindow(ghwnd_DM);
    }

    ghrc_DM = wglCreateContext(ghdc_DM);
    if (ghdc_DM == NULL)
    {
        fprintf_s(gpFile_DM, "wglCreateContext() Faild.\n");
        DestroyWindow(ghwnd_DM);
    }

    if (wglMakeCurrent(ghdc_DM,ghrc_DM) == FALSE)
    {
        fprintf_s(gpFile_DM, "wglMakeCurrent() Faild.\n");
        DestroyWindow(ghwnd_DM);
    }


    // SetClearColor 

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM); //Worm Up call to Resize

}

void Resize(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void Display()
{
    //code
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
}

void unInitialize()
{
    //code

    if(gdFullScreen_DM == TRUE)
    {
        dwStyle_DM = GetWindowLong(ghwnd_DM, GWL_STYLE);
        SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_DM | WS_OVERLAPPEDWINDOW));
        SetWindowPlacement(ghwnd_DM, &wpPrev_DM);
        SetWindowPos(ghwnd_DM, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
    }
    
    if (wglGetCurrentContext() == ghrc_DM)
    {
        wglMakeCurrent(NULL, NULL);
    }

    if (ghrc_DM)
    {
        wglDeleteContext(ghrc_DM);
        ghrc_DM = NULL;
    }

    if (ghdc_DM)
    {
        ReleaseDC(ghwnd_DM, ghdc_DM);
        ghdc_DM = NULL;
    }

    if (gpFile_DM)
    {
        fclose(gpFile_DM);
        gpFile_DM = NULL;
    }

}








