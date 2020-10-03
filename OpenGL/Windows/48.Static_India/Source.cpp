#include<windows.h>
#include"Header.h"
#include<gl/GL.h>
#include<stdio.h>
#include<gl/GLU.h>

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Glu32.lib")

#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

//Global Fuction 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Global Variables Declaration 
DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };
bool gdFullScreen_DM = false;
HWND ghwnd_DM = NULL;
bool gbActiveWindows_DM = false;
HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;
FILE* gpFile_DM = NULL;
const int circle_Points = 1000;
int Color_flag = 0;


//Local Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);
void Vertexcall(GLfloat, GLfloat, GLfloat, GLfloat);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpszCmdLine, int iCmdShow)
{
    //Function 

    void Initialize(void);

    WNDCLASSEX wndclassex;
    HWND hwnd;
    MSG msg;
    TCHAR Appname[] = TEXT("MY_SAMPLE");
    bool bDone = false;
    INT x = 0, y = 0;
    INT Width = 800;
    INT Height = 600;

    if (fopen_s(&gpFile_DM, "log.txt", "w"))
    {
        MessageBox(NULL, TEXT(" File Can Not Created!"), TEXT("Error"), MB_OK);
    }


    wndclassex.cbSize = sizeof(WNDCLASSEX);
    wndclassex.cbClsExtra = 0;
    wndclassex.cbWndExtra = 0;
    wndclassex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclassex.lpszClassName = Appname;
    wndclassex.lpfnWndProc = WndProc;
    wndclassex.lpszMenuName = NULL;
    wndclassex.hInstance = hInstance;
    wndclassex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclassex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclassex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassEx(&wndclassex);

    x = GetSystemMetrics(SM_CXSCREEN);
    y = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
        Appname,
        TEXT("MY consentric Cicle Assignment !"),
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

    Initialize();


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
            if (bool gbActiveWindow = true)
            {
                // Here You should call Update Fuction for openGl Redering

               // Here You should call Display Fuction for openGl Redering

                Display();
            }
        }

    }

    return(msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rc;


    //Function
    void ToggelFullScreen(void);


    switch (iMsg)
    {
    case WM_PAINT:
        GetClientRect(hwnd, &rc);
        Resize(rc.right, rc.bottom);
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
            DestroyWindow(hwnd);
            break;

        case 0x46:
        case 0x66:
            ToggelFullScreen();
            break;
        default:
            break;
        }
        break;
    case WM_SIZE:
        Resize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        unInitialize();
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
    if (gdFullScreen_DM == false)
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
        gdFullScreen_DM = true;
    }
    else
    {
        SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_D | WS_OVERLAPPEDWINDOW));
        SetWindowPlacement(ghwnd_DM, &wpPrev_DM);
        SetWindowPos(ghwnd_DM, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
        gdFullScreen_DM = false;
    }

}

void Initialize()
{
    PIXELFORMATDESCRIPTOR pfd_DM;
    int iPixelFormatIndex_DM;

    ghdc_DM = GetDC(ghwnd_DM);

    ZeroMemory(&pfd_DM, sizeof(PIXELFORMATDESCRIPTOR));

    pfd_DM.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd_DM.nVersion = 1;
    pfd_DM.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
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

    if (SetPixelFormat(ghdc_DM, iPixelFormatIndex_DM, &pfd_DM) == FALSE)
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

    if (wglMakeCurrent(ghdc_DM, ghrc_DM) == FALSE)
    {
        fprintf_s(gpFile_DM, "wglMakeCurrent() Faild.\n");
        DestroyWindow(ghwnd_DM);
    }


    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM);
}
void Resize(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(44.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void Display()
{

    //code
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(-0.3f, 0.0f, -3.0f);

    glBegin(GL_QUADS);
    
    //Top of I
    glColor3f( 1.0f, 0.5f, 0.0f);
    glVertex3f( -1.0f, 0.4f, 0.0f);
    glVertex3f( -0.9f, 0.3f, 0.0f);

    glVertex3f( -0.7f, 0.3f, 0.0f);
    glVertex3f( -0.6f, 0.4f, 0.0f);

    //Middle of I part 1.
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-0.85f, 0.3f, 0.0f);
    glVertex3f(-0.75f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.75f, 0.0f, 0.0f);
    glVertex3f(-0.85f, 0.0f, 0.0f);

    //Middle of I part 2.
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.85f, 0.0f, 0.0f);
    glVertex3f(-0.75f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.75f, -0.3f, 0.0f);
    glVertex3f(-0.85f, -0.3f, 0.0f);

    //Bottom of I
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -0.4f, 0.0f);
    glVertex3f(-0.9f, -0.3f, 0.0f);
    
    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(-0.6f, -0.4f, 0.0f);
    
    glEnd();

   
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    //left of N
    glVertex3f(-0.55f, 0.4f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.45f, 0.1f, 0.0f);
    glVertex3f(-0.45f, 0.0f, 0.0f);
    glVertex3f(-0.55f, 0.0f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.55f, 0.0f, 0.0f);
    glVertex3f(-0.45f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.45f, -0.3f, 0.0f);
    glVertex3f(-0.55f, -0.4f, 0.0f);

    //Middle of N
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-0.44f, 0.1f, 0.0f);
    glVertex3f(-0.54f, 0.4f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.28f, 0.0f, 0.0f);
    glVertex3f(-0.38f, 0.0f, 0.0f);

    glVertex3f(-0.38f, 0.0f, 0.0f);
    glVertex3f(-0.28f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.21f, -0.1f, 0.0f);
    glVertex3f(-0.11f, -0.4f, 0.0f);

    //Right of N
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-0.1f, 0.4f, 0.0f);
    glVertex3f(-0.2f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.2f, 0.0f, 0.0f);
    glVertex3f(-0.1f, 0.0f, 0.0f);

    glVertex3f(-0.1f, 0.0f, 0.0f);
    glVertex3f(-0.2f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.2f, -0.1f, 0.0f);
    glVertex3f(-0.1f, -0.4f, 0.0f);
    
    glEnd();

    //D Began
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
    
    //D Top
    glVertex3f(0.44f, 0.4f, 0.0f);
    glVertex3f(0.05f, 0.4f, 0.0f);

    glVertex3f(-0.05f, 0.3f, 0.0f);
    glVertex3f(0.34f, 0.3f, 0.0f);
    
    //D Middle
    glVertex3f(0.1f, 0.3f, 0.0f);
    glVertex3f(0.2f, 0.3f, 0.0f);

    //White
    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f(0.2f, 0.0f, 0.0f);
    glVertex3f(0.1f, 0.0f, 0.0f);

    glVertex3f(0.1f, 0.0f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.2f, -0.3f, 0.0f);
    glVertex3f(0.1f, -0.3f, 0.0f);

    //D Right
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(0.45f, 0.4f, 0.0f);
    glVertex3f(0.35f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);
    glVertex3f(0.45f, 0.0f, 0.0f);

    glVertex3f(0.45f, 0.0f, 0.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.35f, -0.3f, 0.0f);
    glVertex3f(0.45f, -0.4f, 0.0f);

    //D bOTTOM
    glVertex3f(0.44f, -0.4f, 0.0f);
    glVertex3f(0.05f, -0.4f, 0.0f);

    glVertex3f(-0.05f, -0.3f, 0.0f);
    glVertex3f(0.34f, -0.3f, 0.0f);

    glEnd();

    glBegin(GL_QUADS);
    //Top of I
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(0.9f, 0.4f, 0.0f);
    glVertex3f(0.8f, 0.3f, 0.0f);

    glVertex3f(0.6f, 0.3f, 0.0f);
    glVertex3f(0.5f, 0.4f, 0.0f);

    //Middle of I part 1.
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(0.75f, 0.3f, 0.0f);
    glVertex3f(0.65f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.65f, 0.0f, 0.0f);
    glVertex3f(0.75f, 0.0f, 0.0f);

    //Middle of I part 2.
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.75f, 0.0f, 0.0f);
    glVertex3f(0.65f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.65f, -0.3f, 0.0f);
    glVertex3f(0.75f, -0.3f, 0.0f);

    //Bottom of I
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.9f, -0.4f, 0.0f);
    glVertex3f(0.8f, -0.3f, 0.0f);

    glVertex3f(0.6f, -0.3f, 0.0f);
    glVertex3f(0.5f, -0.4f, 0.0f);

    glEnd();


    //                                              AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

    // - //
    glBegin(GL_QUADS);
    //L B
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.06f, 0.4f, 0.0f);
    glVertex3f(1.3F, 0.4f, 0.0f);
    glVertex3f(1.2f, 0.3f, 0.0f);
    glVertex3f(1.16f, 0.3f, 0.0f);

    glEnd();
    
    //A Right 1
    glBegin(GL_POLYGON);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.31f, 0.4f, 0.0f);
    glVertex3f(1.21f, 0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.3f, 0.05f, 0.0f);
    glVertex3f(1.45f, 0.0f, 0.0f);

    glEnd();
    
    //A right 2
    glBegin(GL_QUADS);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.6f, -0.4f, 0.0f);
    glVertex3f(1.45f, -0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.33f, -0.05f, 0.0f);
    glVertex3f(1.45f, 0.0f, 0.0f);

    glEnd();

    // Left 
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.05f, 0.4f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.97f, 0.0f, 0.0f);
    glVertex3f(1.11f, 0.05f, 0.0f);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.15f, 0.3f, 0.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.97f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.91f, -0.4f, 0.0f);
    glVertex3f(1.05f, -0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.1f, -0.05f, 0.0f);

    glEnd();
  
    //  Center
    glBegin(GL_POLYGON);

    //L CORNER
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.98f, 0.0f, 0.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.11f, 0.04f, 0.0f);
    glVertex3f(1.32f, 0.04f, 0.0f);

    ////R CORNER
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.44f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.32f, -0.04f, 0.0f);
    glVertex3f(1.11f,-0.04f, 0.0f);

    glEnd();
    
    SwapBuffers(ghdc_DM);

}

void unInitialize()
{
    //code
    if (gdFullScreen_DM == TRUE)
    {
        dwStyle_D = GetWindowLong(ghwnd_DM, GWL_STYLE);
        SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_D | WS_OVERLAPPEDWINDOW));
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


