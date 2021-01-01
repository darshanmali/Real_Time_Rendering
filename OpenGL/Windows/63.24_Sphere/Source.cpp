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
GLfloat angle = 0.0f;
static GLfloat x = 0.0;

//                                      New For Light
bool gbLight = true;

//                                      Black and White
GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lDefues[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lPosition[4];

GLfloat L_Model_Ambient[] = { 0.2f,0.2f,0.2f,1.0f };
GLfloat L_Model_Local_Viewer[] = { 0.0f };

//X Y Z Access Rotation

GLfloat angle_for_X_Rotation;
GLfloat angle_for_Y_Rotation;
GLfloat angle_for_Z_Rotation;

GLuint Key_Pressed = 0;

GLUquadric* quadric[24];


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
        TEXT("MY Light !"),
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
            //                                                      New For Light

        case 'X':
            angle_for_X_Rotation = 0.0f;
            Key_Pressed = 1;
            break;
        case 'Y':
            angle_for_Y_Rotation = 0.0f;
            Key_Pressed = 2;
            break;
        case 'Z':
            angle_for_Z_Rotation = 0.0f;
            Key_Pressed = 3;
            break;
        case 'L':
            if (gbLight == true)
            {
                glEnable(GL_LIGHTING);

                gbLight = false;
            }
            else
            {
                glDisable(GL_LIGHTING);
                gbLight = true;
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


    glClearColor(0.25f, 0.25f, 0.25f, 0.0f);

    //Functions for Depth

    //for getting anti_Allies
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//Preventing From Perspective Destortion

    //New
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, L_Model_Ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, L_Model_Local_Viewer);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lDefues);
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);

    glEnable(GL_LIGHT0);

    for (int i = 0; i < 24; i++)
    {
        quadric[i] = gluNewQuadric();
    }

    

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

    if (width <= height)
    {
        glOrtho(-3.0f,
            15.0f,
            2.0f,
            15.0f * ((GLfloat)WIN_WIDTH_DM / (GLfloat)WIN_HEIGHT_DM),
            -10.0f,
            10.0f);
    }
    else
    {
        glOrtho(-3.0f,
            15.0f,
            15.0f * ((GLfloat)WIN_WIDTH_DM / (GLfloat)WIN_HEIGHT_DM),
            2.0f,
            -10.0f,
            10.0f);
    }

}

void Display()
{

    void draw_24_Sphere(void);
    void update(void);


    // for Depth Add GL_DEPTH_BUFFER_BIT 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (Key_Pressed == 1)
    {
        glRotatef(angle_for_X_Rotation, 1.0f, 0.0f, 0.0f);
        lPosition[1] = angle_for_X_Rotation;
    }
    else if (Key_Pressed == 2)
    {
        glRotatef(angle_for_Y_Rotation, 0.0f, 1.0f, 0.0f);
        lPosition[2] = angle_for_Y_Rotation;
    }
    else if (Key_Pressed == 3)
    {
        glRotatef(angle_for_Z_Rotation, 0.0f, 0.0f, 1.0f);
        lPosition[0] = angle_for_Z_Rotation;
    }
    lPosition[3] = 1.0f;
                                                                
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);

    draw_24_Sphere();
    
    update();
    
    SwapBuffers(ghdc_DM);

}

void draw_24_Sphere(void) 
{

    //Emereable
    GLfloat material_Ambient[4];
    material_Ambient[0] = { 0.0215f };
     material_Ambient[1] = { 0.1745f };
     material_Ambient[2] = { 0.0215f };
     material_Ambient[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient);
        
    GLfloat material_Diffuse[4];
    material_Diffuse[0] = { 0.07568f };
    material_Diffuse[1] = { 0.61424f };
    material_Diffuse[2] = { 0.07568f };
    material_Diffuse[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse);

    GLfloat material_Specular[4];
    material_Specular[0] =  { 0.633f };
    material_Specular[1] = { 0.727811f };
    material_Specular[2] = { 0.633f };
    material_Specular[3] = { 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular);

    GLfloat material_Shininess = 0.6f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 14.0f, 0.0f);

    gluSphere(quadric[0], 1.0f, 30, 30);

    //jade 
    GLfloat material_Ambient_1[4];
    material_Ambient_1[0] = { 0.135f };
    material_Ambient_1[1] = { 0.2225f };
    material_Ambient_1[2] = { 0.1575f };
    material_Ambient_1[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_1);

    GLfloat material_Diffuse_1[4];
     material_Diffuse_1[0] = { 0.54f };
     material_Diffuse_1[1] = { 0.89f };
     material_Diffuse_1[2] = { 0.63f };
     material_Diffuse_1[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_1);

    GLfloat material_Specular_1[4];
     material_Specular_1[0] = { 0.316228f };
     material_Specular_1[1] = { 0.316228f };
     material_Specular_1[2] = { 0.316228f };
     material_Specular_1[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_1);

    GLfloat material_Shininess_1 = 0.1f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 14.0f, 0.0f);

    gluSphere(quadric[1], 1.0f, 30, 30);

    //obsidian 
    GLfloat material_Ambient_2[4];
    material_Ambient_2[0] = { 0.05375f };
    material_Ambient_2[1] = { 0.05f };
    material_Ambient_2[2] = { 0.06625f };
    material_Ambient_2[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_2);
    
    GLfloat material_Diffuse_2[4];
    material_Diffuse_2[0] = { 0.18275f };
    material_Diffuse_2[1] = { 0.17f };
    material_Diffuse_2[2] = { 0.22525f };
    material_Diffuse_2[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_2);

    GLfloat material_Specular_2[4];
    material_Specular_2[0] = { 0.332741f };
     material_Specular_2[1] = { 0.328634f };
     material_Specular_2[2] = { 0.346435f };
     material_Specular_2[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_2);

    GLfloat material_Shininess_2 = 0.3f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 14.0f, 0.0f);

    gluSphere(quadric[2], 1.0f, 30, 30);

    //pearl
    GLfloat material_Ambient_3[4];
    material_Ambient_3[0] = { 0.25f };
     material_Ambient_3[1] = { 0.20725f };
     material_Ambient_3[2] = { 0.20725f };
     material_Ambient_3[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_3);

    GLfloat material_Diffuse_3[4];
    material_Diffuse_3[0] = { 1.0f };
     material_Diffuse_3[1] = { 0.829f };
     material_Diffuse_3[2] = { 0.829f };
     material_Diffuse_3[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_3);

    GLfloat material_Specular_3[4];
    material_Specular_3[0] = {  0.296648f };
     material_Specular_3[1] = { 0.296648f };
     material_Specular_3[2] = { 0.296648f };
     material_Specular_3[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_3);

    GLfloat material_Shininess_3 = 0.88f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_3);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 14.0f, 0.0f);

    gluSphere(quadric[3], 1.0f, 30, 30);

    //ruby
    GLfloat material_Ambient_4[4];
    material_Ambient_4[0] = { 0.1745f };
     material_Ambient_4[1] = { 0.01175f };
     material_Ambient_4[2] = { 0.01175f };
     material_Ambient_4[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_4);

    GLfloat material_Diffuse_4[4];
    material_Diffuse_4[0] = { 0.61424f };
     material_Diffuse_4[1] = { 0.04136f };
     material_Diffuse_4[2] = { 0.04136f };
     material_Diffuse_4[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_4);

    GLfloat material_Specular_4[4];
    material_Specular_4[0] = { 0.727811f };
     material_Specular_4[1] = { 0.626959f };
     material_Specular_4[2] = { 0.626959f };
     material_Specular_4[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_4);

    GLfloat material_Shininess_4 = 0.6f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_4);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 14.0f, 0.0f);

    gluSphere(quadric[4], 1.0f, 30, 30);

    //turquoise
    GLfloat material_Ambient_5[4];
    material_Ambient_5[0] = { 0.1f };
     material_Ambient_5[1] = { 0.18725f };
     material_Ambient_5[2] = { 0.1745f };
     material_Ambient_5[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_5);

    GLfloat material_Diffuse_5[4];
    material_Diffuse_5[0] = { 0.396f };
     material_Diffuse_5[1] = { 0.74151f };
     material_Diffuse_5[2] = { 0.69102f };
     material_Diffuse_5[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_5);

    GLfloat material_Specular_5[4];
    material_Specular_5[0] = { 0.297254f };
     material_Specular_5[1] = { 0.30829f };
     material_Specular_5[2] = { 0.306678f };
     material_Specular_5[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_5);

    GLfloat material_Shininess_5 = 0.1f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_5);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 14.0f, 0.0f);

    gluSphere(quadric[5], 1.0f, 30, 30);


    //brass
    GLfloat material_Ambient_6[4];
    material_Ambient_6[0] = { 0.329412f };
     material_Ambient_6[1] = { 0.223529f };
     material_Ambient_6[2] = { 0.027451f };
     material_Ambient_6[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_6);

    GLfloat material_Diffuse_6[4];
    material_Diffuse_6[0] = { 0.780392f };
     material_Diffuse_6[1] = { 0.568627f };
     material_Diffuse_6[2] = { 0.113725f };
     material_Diffuse_6[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_6);

    GLfloat material_Specular_6[4];
    material_Specular_6[0] = { 0.992157f };
     material_Specular_6[1] = { 0.941176f };
     material_Specular_6[2] = { 0.807843f };
     material_Specular_6[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_6);

    GLfloat material_Shininess_6 = 0.21794872f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_6);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 12.0f, 0.0f);

    gluSphere(quadric[6], 1.0f, 30, 30);

    //bronze
    GLfloat material_Ambient_7[4];
    material_Ambient_7[0] = { 0.2125f };
     material_Ambient_7[1] = { 0.1275f };
     material_Ambient_7[2] = { 0.054f };
     material_Ambient_7[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_7);

    GLfloat material_Diffuse_7[4];
    material_Diffuse_7[0] = { 0.714f };
     material_Diffuse_7[1] = { 0.4284f };
     material_Diffuse_7[2] = { 0.18144f };
     material_Diffuse_7[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_7);

    GLfloat material_Specular_7[4];
    material_Specular_7[0] = { 0.393548f };
     material_Specular_7[1] = { 0.271906f };
     material_Specular_7[2] = { 0.166721f };
     material_Specular_7[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_7);

    GLfloat material_Shininess_7 = 0.2f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_7);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 12.0f, 0.0f);

    gluSphere(quadric[7], 1.0f, 30, 30);


    //chroma
    GLfloat material_Ambient_8[4];
    material_Ambient_8[0] = { 0.25f };
     material_Ambient_8[1] = { 0.25f };
     material_Ambient_8[2] = { 0.25f };
     material_Ambient_8[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_8);

    GLfloat material_Diffuse_8[4];
    material_Diffuse_8[0] = { 0.4f };
     material_Diffuse_8[1] = { 0.4f };
     material_Diffuse_8[2] = { 0.4f };
     material_Diffuse_8[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_8);

    GLfloat material_Specular_8[4];
    material_Specular_8[0] = { 0.774597f };
     material_Specular_8[1] = { 0.774597f };
     material_Specular_8[2] = { 0.774597f };
     material_Specular_8[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_8);

    GLfloat material_Shininess_8 = 0.6f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_8);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 12.0f, 0.0f);

    gluSphere(quadric[8], 1.0f, 30, 30);

    //Copper
    GLfloat material_Ambient_9[4];
    material_Ambient_9[0] = { 0.19125f };
     material_Ambient_9[1] = { 0.0735f };
     material_Ambient_9[2] = { 0.0225f };
     material_Ambient_9[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_9);

    GLfloat material_Diffuse_9[4];
    material_Diffuse_9[0] = { 0.7038f };
     material_Diffuse_9[1] = { 0.27048f };
     material_Diffuse_9[2] = { 0.0828f };
     material_Diffuse_9[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_9);

    GLfloat material_Specular_9[4];
    material_Specular_9[0] = { 0.256777f };
     material_Specular_9[1] = { 0.137622f };
     material_Specular_9[2] = { 0.086014f };
     material_Specular_9[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_9);

    GLfloat material_Shininess_9 = 0.1f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_9);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 12.0f, 0.0f);

    gluSphere(quadric[9], 1.0f, 30, 30);

    //Gold
    GLfloat material_Ambient_10[4];
    material_Ambient_10[0] = { 0.24725f };
     material_Ambient_10[1] = { 0.1995f };
     material_Ambient_10[2] = { 0.0745f };
     material_Ambient_10[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_10);

    GLfloat material_Diffuse_10[4];
    material_Diffuse_10[0] = { 0.75164f };
     material_Diffuse_10[1] = { 0.60648f };
     material_Diffuse_10[2] = { 0.22648f };
     material_Diffuse_10[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_10);

    GLfloat material_Specular_10[4];
    material_Specular_10[0] = { 0.628281f };
     material_Specular_10[1] = { 0.555802f };
     material_Specular_10[2] = { 0.366065f };
     material_Specular_10[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_10);

    GLfloat material_Shininess_10 = 0.4f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 12.0f, 0.0f);

    gluSphere(quadric[10], 1.0f, 30, 30);

    //Silver
     GLfloat material_Ambient_11[4];
     material_Ambient_11[0] = { 0.19225f };
     material_Ambient_11[1] = { 0.19225f };
     material_Ambient_11[2] = { 0.19225f };
     material_Ambient_11[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_11);

    GLfloat material_Diffuse_11[4];
    material_Diffuse_11[0] = { 0.50754f };
     material_Diffuse_11[1] = { 0.50754f };
     material_Diffuse_11[2] = { 0.50754f };
     material_Diffuse_11[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_11);

    GLfloat material_Specular_11[4];
    material_Specular_11[0] = { 0.508273f };
     material_Specular_11[1] = { 0.508273f };
     material_Specular_11[2] = { 0.508273f };
     material_Specular_11[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_11);

    GLfloat material_Shininess_11 = 0.4f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_11);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 12.0f, 0.0f);

    gluSphere(quadric[11], 1.0f, 30, 30);

    //Black
    GLfloat material_Ambient_12[4];
    material_Ambient_12[0] = { 0.0f };
     material_Ambient_12[1] = { 0.0f };
     material_Ambient_12[2] = { 0.0f };
     material_Ambient_12[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_12);

    GLfloat material_Diffuse_12[4];
    material_Diffuse_12[0] = { 0.01f };
     material_Diffuse_12[1] = { 0.01f };
     material_Diffuse_12[2] = { 0.01f };
     material_Diffuse_12[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_12);

    GLfloat material_Specular_12[4];
    material_Specular_12[0] = { 0.50f };
     material_Specular_12[1] = { 0.50f };
     material_Specular_12[2] = { 0.50f };
     material_Specular_12[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_12);

    GLfloat material_Shininess_12 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_12);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 10.0f, 0.0f);

    gluSphere(quadric[12], 1.0f, 30, 30);

    //cyan
    GLfloat material_Ambient_13[4];
     material_Ambient_13[0] = { 0.0f };
     material_Ambient_13[1] = { 0.1f };
     material_Ambient_13[2] = { 0.06f };
     material_Ambient_13[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_13);

    GLfloat material_Diffuse_13[4];
    material_Diffuse_13[0] = { 0.0f };
     material_Diffuse_13[1] = { 0.50980392f };
     material_Diffuse_13[2] = { 0.50980392f };
     material_Diffuse_13[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_13);

    GLfloat material_Specular_13[4];
    material_Specular_13[0] = { 0.50196078f };
     material_Specular_13[1] = { 0.50196078f };
     material_Specular_13[2] = { 0.50196078f };
     material_Specular_13[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_13);

    GLfloat material_Shininess_13 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_13);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 10.0f, 0.0f);

    gluSphere(quadric[13], 1.0f, 30, 30);

    //green
    GLfloat material_Ambient_14[4];
    material_Ambient_14[0] = { 0.0f };
     material_Ambient_14[1] = { 0.0f };
     material_Ambient_14[2] = { 0.0f };
     material_Ambient_14[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_14);

    GLfloat material_Diffuse_14[4];
    material_Diffuse_14[0] = { 0.1f };
     material_Diffuse_14[1] = { 0.35f };
     material_Diffuse_14[2] = { 0.1f };
     material_Diffuse_14[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_14);

    GLfloat material_Specular_14[4];
    material_Specular_14[0] = { 0.45f };
     material_Specular_14[1] = { 0.45f };
     material_Specular_14[2] = { 0.45f };
     material_Specular_14[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_14);

    GLfloat material_Shininess_14 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_14);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 10.0f, 0.0f);

    gluSphere(quadric[14], 1.0f, 30, 30);

    //red
    GLfloat material_Ambient_15[4];
    material_Ambient_15[0] = { 0.0f };
     material_Ambient_15[1] = { 0.0f };
     material_Ambient_15[2] = { 0.0f };
     material_Ambient_15[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_15);

    GLfloat material_Diffuse_15[4];
    material_Diffuse_15[0] = { 0.5f };
     material_Diffuse_15[1] = { 0.0f };
     material_Diffuse_15[2] = { 0.0f };
     material_Diffuse_15[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_15);

    GLfloat material_Specular_15[4] ;
    material_Specular_15[0] = { 0.7f };
     material_Specular_15[1] = { 0.6f };
     material_Specular_15[2] = { 0.6f };
     material_Specular_15[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_15);

    GLfloat material_Shininess_15 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_15);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 10.0f, 0.0f);

    gluSphere(quadric[15], 1.0f, 30, 30);

    //Samle
    GLfloat material_Ambient_16[4];
    material_Ambient_16[0] = { 0.0f };
     material_Ambient_16[1] = { 0.0f };
     material_Ambient_16[2] = { 0.0f };
     material_Ambient_16[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_16);

    GLfloat material_Diffuse_16[4];
    material_Diffuse_16[0] = { 0.55f };
     material_Diffuse_16[1] = { 0.55f };
     material_Diffuse_16[2] = { 0.55f };
     material_Diffuse_16[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_16);

    GLfloat material_Specular_16[4];
     material_Specular_16[0] = { 0.70f };
     material_Specular_16[1] = { 0.70f };
     material_Specular_16[2] = { 0.70f };
     material_Specular_16[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_16);

    GLfloat material_Shininess_16 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_16);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 10.0f, 0.0f);

    gluSphere(quadric[16], 1.0f, 30, 30);

    //yellow plastic
    GLfloat material_Ambient_17[4];
    material_Ambient_17[0] = { 0.0f };
     material_Ambient_17[1] = { 0.0f };
     material_Ambient_17[2] = { 0.0f };
     material_Ambient_17[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_17);

    GLfloat material_Diffuse_17[4];
    material_Diffuse_17[0] = { 0.5f };
     material_Diffuse_17[1] = { 0.5f };
     material_Diffuse_17[2] = { 0.0f };
     material_Diffuse_17[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_17);

    GLfloat material_Specular_17[4];
    material_Specular_17[0] = { 0.60f };
     material_Specular_17[1] = { 0.60f };
     material_Specular_17[2] = { 0.50f };
     material_Specular_17[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_17);

    GLfloat material_Shininess_17 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_17);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 10.0f, 0.0f);

    gluSphere(quadric[17], 1.0f, 30, 30);

    //Samle
    GLfloat material_Ambient_18[4];
    material_Ambient_18[0] = { 0.02f };
     material_Ambient_18[1] = { 0.02f };
     material_Ambient_18[2] = { 0.02f };
     material_Ambient_18[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_18);

    GLfloat material_Diffuse_18[4];
    material_Diffuse_18[0] = { 0.01f };
     material_Diffuse_18[1] = { 0.01f };
     material_Diffuse_18[2] = { 0.01f };
     material_Diffuse_18[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_18);

    GLfloat material_Specular_18[4];
    material_Specular_18[0] = { 0.4f };
     material_Specular_18[1] = { 0.4f };
     material_Specular_18[2] = { 0.4f };
     material_Specular_18[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_18);

    GLfloat material_Shininess_18 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_18);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 8.0f, 0.0f);

    gluSphere(quadric[18], 1.0f, 30, 30);

    //cyan
    GLfloat material_Ambient_19[4];
    material_Ambient_19[0] = { 0.0f };
     material_Ambient_19[1] = { 0.05f };
     material_Ambient_19[2] = { 0.05f };
     material_Ambient_19[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_19);

    GLfloat material_Diffuse_19[4];
    material_Diffuse_19[0] = { 0.4f };
     material_Diffuse_19[1] = { 0.5f };
     material_Diffuse_19[2] = { 0.5f };
     material_Diffuse_19[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_19);

    GLfloat material_Specular_19[4];
    material_Specular_19[0] = { 0.04f };
     material_Specular_19[1] = { 0.7f };
     material_Specular_19[2] = { 0.7f };
     material_Specular_19[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_19);

    GLfloat material_Shininess_19 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_19);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 8.0f, 0.0f);

    gluSphere(quadric[19], 1.0f, 30, 30);

    //green
    GLfloat material_Ambient_20[4];
    material_Ambient_20[0] = { 0.0f };
     material_Ambient_20[1] = { 0.05f };
     material_Ambient_20[2] = { 0.0f };
     material_Ambient_20[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_20);

    GLfloat material_Diffuse_20[4];
    material_Diffuse_20[0] = { 0.4f };
     material_Diffuse_20[1] = { 0.5f };
     material_Diffuse_20[2] = { 0.4f };
     material_Diffuse_20[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_20);

    GLfloat material_Specular_20[4];
    material_Specular_20[0] = { 0.04f };
     material_Specular_20[1] = { 0.7f };
     material_Specular_20[2] = { 0.04f };
     material_Specular_20[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_20);

    GLfloat material_Shininess_20 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 8.0f, 0.0f);

    gluSphere(quadric[20], 1.0f, 30, 30);

    //red
    GLfloat material_Ambient_21[4];
    material_Ambient_21[0] = { 0.05f };
     material_Ambient_21[1] = { 0.0f };
     material_Ambient_21[2] = { 0.0f };
     material_Ambient_21[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_21);

    GLfloat material_Diffuse_21[4];
    material_Diffuse_21[0] = { 0.5f };
     material_Diffuse_21[1] = { 0.4f };
     material_Diffuse_21[2] = { 0.4f };
     material_Diffuse_21[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_21);

    GLfloat material_Specular_21[4];
    material_Specular_21[0] = { 0.7f };
    material_Specular_21[1] = { 0.04f };
     material_Specular_21[2] = { 0.04f };
     material_Specular_21[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_21);

    GLfloat material_Shininess_21 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_21);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 8.0f, 0.0f);

    gluSphere(quadric[21], 1.0f, 30, 30);

    //White
    GLfloat material_Ambient_22[4];
    material_Ambient_22[0] = { 0.05f };
     material_Ambient_22[1] = { 0.05f };
     material_Ambient_22[2] = { 0.05f };
     material_Ambient_22[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_22);

    GLfloat material_Diffuse_22[4];
     material_Diffuse_22[0] = { 0.5f };
     material_Diffuse_22[1] = { 0.5f };
     material_Diffuse_22[2] = { 0.5f };
     material_Diffuse_22[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_22);

    GLfloat material_Specular_22[4];
    material_Specular_22[0] = { 0.7f };
     material_Specular_22[1] = { 0.7f };
     material_Specular_22[2] = { 0.7f };
     material_Specular_22[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_22);

    GLfloat material_Shininess_22 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_22);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 8.0f, 0.0f);

    gluSphere(quadric[22], 1.0f, 30, 30);

    //yellow rubber
    GLfloat material_Ambient_23[4];
    material_Ambient_23[0] = {  0.0f };
     material_Ambient_23[1] = { 0.0f };
     material_Ambient_23[2] = { 0.0f };
     material_Ambient_23[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_23);

    GLfloat material_Diffuse_23[4];
    material_Diffuse_23[0] = {  0.0f };
     material_Diffuse_23[1] = { 0.0f };
     material_Diffuse_23[2] = { 0.0f };
     material_Diffuse_23[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_23);

    GLfloat material_Specular_23[4];
    material_Specular_23[0] = {  0.0f };
     material_Specular_23[1] = { 0.0f };
     material_Specular_23[2] = { 0.0f };
     material_Specular_23[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_23);

    GLfloat material_Shininess_23 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_23);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 8.0f, 0.0f);

    gluSphere(quadric[23], 1.0f, 30, 30);




}

void update(void)
{
    if (Key_Pressed == 1)
    {
        angle_for_X_Rotation += 1.0f;
    }
    else if (Key_Pressed == 2)
    {
        angle_for_Y_Rotation += 1.0f;
    }
    else if (Key_Pressed == 3)
    {
        angle_for_Z_Rotation += 1.0f;
    }

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

    for (int i = 0; i < 24; i++)
    {
        gluDeleteQuadric(quadric[i]);
        quadric[i] = NULL;
        
    }

    
    if (gpFile_DM)
    {
        fclose(gpFile_DM);
        gpFile_DM = NULL;
    }

}


