#include<windows.h>
#include"Header.h"
#include"OGL.h"
#include<gl/GL.h>
#include<gl/GLU.h>
#include<stdio.h>

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
GLfloat angle = 0.0f;
static GLfloat x = 0.0;

GLuint TeaPot_Texture;
HBITMAP hBitmap;
BITMAP bmp;

//                                      New For Light
bool gbLight = false;

//                                                                      Black and White
GLfloat lights_Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lights_Defues[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lights_Position[] = { 100.0f, 100.0f, 100.0f, 1.0f };
GLfloat lights_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_Shininess = 128.0f;

//Local Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);


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
        TEXT("MY Cube !"),
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
    //Function
    void ToggelFullScreen(void);


    switch (iMsg)
    {
    case WM_PAINT:
        // Display();
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
        case 'L':
            if (gbLight == false)
            {
                glEnable(GL_LIGHTING);

                gbLight = true;
            }
            else
            {
                glDisable(GL_LIGHTING);
                gbLight = false;
            }
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
    
    //                                                                                                                      1 Change                       
    bool LoadGLTexture(GLuint*, TCHAR[]);


    ghdc_DM = GetDC(ghwnd_DM);

    ZeroMemory(&pfd_DM, sizeof(PIXELFORMATDESCRIPTOR));

    pfd_DM.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd_DM.nVersion = 1;
    pfd_DM.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd_DM.iPixelType = PFD_TYPE_RGBA;
    pfd_DM.cColorBits = 32;
    //Depth
    pfd_DM.cDepthBits = 32;
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

    //Functions for Depth

    //for getting anti_Allies
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//Preventing From Perspective Destortion
    //                                                                                                  New 
    glEnable(GL_TEXTURE_2D);
    LoadGLTexture(&TeaPot_Texture, MAKEINTRESOURCE(STONE_BITMAP));
    glBindTexture(GL_TEXTURE_2D, TeaPot_Texture);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lights_Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lights_Defues);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lights_Specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lights_Position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular);

    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess);;

    glEnable(GL_LIGHT0);

    Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM);
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

bool LoadGLTexture(GLuint* Texture, TCHAR Resource_ID[])
{
    bool bResult = false;

    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), Resource_ID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hBitmap != NULL)
    {
        bResult = true;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        //From here We are Starting the Texturing Code
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glGenTextures(1, Texture);
        glBindTexture(GL_TEXTURE_2D, *Texture);
        //Setting Texture Param
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
        DeleteObject(hBitmap);
    }
    return (bResult);
}

void Display()
{

    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -1.0f);

    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    

    glBegin(GL_TRIANGLES);
    
    for (int i = 0; i < sizeof(face_indicies) / sizeof(face_indicies[0]); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int vi = face_indicies[i][j];
            int ni = face_indicies[i][j + 3];
            int ti = face_indicies[i][j + 6];

            glNormal3f(normals[ni][0], normals[ni][1], normals[ni][2]);

            glTexCoord2f(textures[ti][0], textures[ti][1]);

            glVertex3f(vertices[vi][0], vertices[vi][1], vertices[vi][2]);

        }
    }

    glEnd();

    angle = angle + 0.1f;
    if (angle >= 360.0f)
    {
        angle = 0.0f;
    }

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
    glDeleteTextures(1, &TeaPot_Texture);
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
