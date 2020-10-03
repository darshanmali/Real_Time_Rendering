#include<windows.h>
#include"Header.h"
#include<gl/GL.h>
#include<stdio.h>
#include<gl/GLU.h>
#include<math.h>

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Glu32.lib")
#pragma comment(lib,"Winmm.lib")

#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600
#define PI 3.141592653589793238

#define ALPHA_FIRST_I 1
#define ALPHA_N 2
#define ALPHA_D 3
#define ALPHA_SECOND_I 4
#define ALPHA_A 5
#define COLOR_ORANGE 6
#define COLOR_GREEN 7
#define COLOR_WHITE 8


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
int i = 1;
GLfloat x = 0.0f, y = 0.0f;
bool flag = false;
GLfloat Jangle_T = 310.0f;
GLfloat Jangle_B = 50.0f;
bool Direct_T = false;
bool Direct_B = false;

// Flag for Music
bool first_I = false;
GLfloat N_P_3 = 1.21f;
GLfloat N_P_4 = 1.08f;
GLfloat N_P_6 = 1.08f;
GLfloat N_P_7 = 1.21f;

// Most Jet Params...
GLfloat fRedius = 0.0f,
fTrans_x = 0.0f,
fTrans_y = 0.0f,
fTrans_z = 0.0f,
fScale_x = 0.0f,
fScale_y = 0.0f,
fScale_z = 0.0f;

// Alphabets Parameters...
//I_1
GLfloat X_i = -1.7f,
Y_i = 0.0f,
Z_i = -3.0f;

//N
GLfloat X_n = -0.4f,
Y_n = 2.3f,
Z_n = -3.0f;

//D
GLfloat X_d = -0.35f,
Y_d = 0.0f,
Z_d = -3.0f;

//I Second
GLfloat X_i_2 = -0.3f,
Y_i_2 = -1.7f,
Z_i_2 = -3.0f;

//A 
GLfloat X_a = 1.45f,
Y_a = 0.0f,
Z_a = -3.0f;

//Printing Flags
bool flag_for_Jets = false;
bool flag_for_I_1 = false;
bool flag_for_n = false;
bool flag_for_d = false;
bool flag_for_i_2 = false;
bool flag_for_a = false;

//Close Call Flag
bool flag_Exit = false;


//D_Animated Colors
GLfloat Col_x = 0.0f;
GLfloat Col_y = 0.0f;
GLfloat Col_z = 0.0f;

//Green
GLfloat Col_y_G = 0.0f;

//White
GLfloat Col_x_w = 0.0f;
GLfloat Col_y_w = 0.0f;
GLfloat Col_z_w = 0.0f;


//Increment of X 
GLfloat fIncremet_x = 0.0f;
GLfloat fIncremet_y = 0.0f;

GLfloat fIncremet_x_Middle = 0.0f;

GLfloat fIncremet_x_Bottom = 0.0f;
GLfloat fIncremet_y_Bottom = 0.0f;

//Global Function 

void Resize(int, int);
void unInitialize(void);
void Display(void);
void Update(int, int);
void Moving(GLfloat *, GLfloat*, GLfloat*, int);

void Color_fads(GLfloat *,GLfloat *,GLfloat*,int, int);
void Size_Inc(GLfloat *, int);

//Alphabates Printing
void Print_I_1();
void Print_N();
void Print_D();
void Print_I_2();
void Print_A();
void Print_Top();
void Print_Bottom();


// My New Functions
void CircleDrawing(GLfloat *,GLfloat *, GLfloat*, GLfloat*, GLfloat*, GLfloat*,GLfloat*, bool, int);

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
    MONITORINFO mi_D = { sizeof(MONITORINFO) };

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
        TEXT("MY Dynamic India !"),
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

    ghwnd_DM = hwnd;
    //Function
    void ToggelFullScreen(void);

    switch (iMsg)
    {
    case WM_CREATE:
        first_I = true;
        break;
    
    case WM_TIMER:
        KillTimer(hwnd, MYTIMER);
        
        if (first_I == true)
        {
            PlaySoundW(TEXT("New_3.wav"), NULL, SND_ASYNC);
            first_I = false;
            flag_for_I_1 = true;
        }
        if (flag_Exit == true)
        {
            DestroyWindow(hwnd);
        }
        
        break;
    case WM_PAINT:
        GetClientRect(hwnd, &rc);
        Resize(rc.right, rc.bottom);

        if (first_I == true)
        {
            SetTimer(hwnd, MYTIMER, 1000, NULL);
        }
       
        break;

    case WM_SETFOCUS:
        gbActiveWindows_DM = true;
        break;
    case WM_KILLFOCUS:
        gbActiveWindows_DM = false;
        break;
    case WM_LBUTTONDOWN:
        if (flag_Exit == true)
        {
            SetTimer(hwnd, MYTIMER, 1000, NULL);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hwnd);
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
    }    return(DefWindowProc(hwnd, iMsg, wParam, lParam));
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
    //variable Declaration
    GLfloat x_new = 0.0f, y_new = 0.0f;

    //Function Declaration 
    void TopJetCall(void);
    void MiddleJetCall(void);
    void BottomJetCall(void);
    
    //code
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


   
    if (flag_for_I_1)
    {
        // Printing I 1
        Print_I_1();
    }
    if (flag_for_n)
    {
        // Printing N
        Print_N();
    }
    if (flag_for_d)
    {
        Print_D();
    }
    if (flag_for_i_2)
    {
        // Printing I 2
        Print_I_2();
    }
    if (flag_for_a)
    {
        // Printing A
        Print_A();
    }
    if (flag_for_Jets == true)
    {
        //Top jet 1.
        TopJetCall();

        //Bottom jet 3.
        BottomJetCall();

        // Middle Jet 2.
        MiddleJetCall();
    }
     
    SwapBuffers(ghdc_DM);
}

void Update( int flag , int direction)
{
    if (flag == 1)
    {
        if (direction == 1)
        {
            if (Jangle_T >= 360.0f)
            {
                Jangle_T = 0.0f;
            }
            if (Jangle_T >= 345.0f)
            {
                Jangle_T = Jangle_T + 0.015f;
            }
            else
            {
                Jangle_T = Jangle_T + 0.01f;
            }
        }
        else if (direction == 2)
        {
            if (Jangle_T >= 335.0f)
            {
                Jangle_T = Jangle_T + 0.015f;
            }
            else
            {
                Jangle_T = Jangle_T + 0.02f;
            }
        }
    }
    if (flag == 3)
    {
        if (direction == 1)
        {
            if (Jangle_B >= 15.0f)
            {
                Jangle_B = Jangle_B - 0.01f;
            }
            else
            {
                Jangle_B = Jangle_B - 0.015f;
            }
        }
        else if (direction == 2)
        {
            if (Jangle_B >= 15.0f)
            {
                Jangle_B = Jangle_B - 0.015f;
            }
            else
            {
                Jangle_B = Jangle_B - 0.02f;
            }
        }
    }
}

void Moving(GLfloat *x, GLfloat* y, GLfloat* z,int alphabet)
{
    switch (alphabet)
    {
    case ALPHA_FIRST_I:
        if (*x <= -0.45f)
        {
            *x = *x + 0.0005f;
        }
        else
        {
            flag_for_n = true;
        }
    break;
    case ALPHA_N:
        if (*y >= 0.0f)
        {
            *y = *y - 0.0005f;
           
        }
        else
        {
            flag_for_a = true;
        }
        break;
    case ALPHA_D:
        if (*z <= -3.0f)
        {
            *z = *z + 0.002f;
        }
        break;
    case ALPHA_SECOND_I:
        if (*y <= 0.0f)
        {
            *y = *y + 0.0005f;
        }
        else
        {
            flag_for_d = true;
        }
        break;
    case ALPHA_A:
        if (*x >= -0.2f)
        {
            *x = *x - 0.0005f;
        }
        else
        {
            flag_for_i_2 = true;
        }
        break;

    default:
        break;
    }
    
}

void Color_fads(GLfloat* x, GLfloat* y, GLfloat* z, int Color,int Alpha)
{
    if (Alpha == 1)
    {
        if (Color == COLOR_ORANGE)
        {
            *x += 0.0001f;
            *y += 0.00005f;
            *z += 0.0f;
        }
        if (Color == COLOR_GREEN)
        {
            *y += 0.0001f;
        }

        if (Color == COLOR_WHITE)
        {
            *x += 0.0001f;
            *y += 0.0001f;
            *z += 0.0001f;
        }
    }
    else if (Alpha == 2)
    {

    }
}

void Size_Inc( GLfloat *increment, int flag)
{
    if (flag == 1)
    {
        if (*increment <= 1.45f && N_P_4 >= 1.21f )
        {
            *increment = *increment + 0.0005f;
        }
    }
    if (flag == 2)
    {
        if (*increment <= 1.59f)
        {
            *increment = *increment + 0.0005f;
        }
    }
    if (flag == 3)
    {
        if (*increment <= 1.46f && N_P_6 >= 1.21f)
        {
            *increment = *increment + 0.0005f;
        }
    }
    if (flag == 4)
    {
        if (*increment <= 1.59f)
        {
            *increment = *increment + 0.0005f;
        }
    }
}

void Print_I_1()
{

    Moving( &X_i, &Y_i, &Z_i, ALPHA_FIRST_I);

    glTranslatef(X_i, Y_i, Z_i);
    glBegin(GL_QUADS);

    //Top of I
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-1.0f, 0.4f, 0.0f);
    glVertex3f(-0.9f, 0.3f, 0.0f);

    glVertex3f(-0.7f, 0.3f, 0.0f);
    glVertex3f(-0.6f, 0.4f, 0.0f);

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
}

void Print_N()
{
    Moving(&X_n, &Y_n, &Z_n, ALPHA_N);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(X_n, Y_n, Z_n);

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
}

void Print_D()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    glTranslatef(X_d, Y_d, Z_d);
    

    //D Began
    glBegin(GL_QUADS);
    Color_fads(&Col_x, &Col_y, &Col_z, COLOR_ORANGE,1);

   // glColor3f(Col_x, Col_y, Col_z);
    if (Col_x <= 1.0f || Col_y < 0.5f)
    {
        glColor3f(Col_x, Col_y, Col_z);
    }
    else
    {
        flag_for_Jets = true;
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    
    //D Top
    glVertex3f(0.44f, 0.4f, 0.0f);
    glVertex3f(0.05f, 0.4f, 0.0f);

    glVertex3f(-0.05f, 0.3f, 0.0f);
    glVertex3f(0.34f, 0.3f, 0.0f);

    //D Middle
    glVertex3f(0.1f, 0.3f, 0.0f);
    glVertex3f(0.2f, 0.3f, 0.0f);

    //White
    Color_fads(&Col_x_w, &Col_y_w, &Col_z_w, COLOR_WHITE, 1);

    if (Col_x_w <= 1.0f && Col_y_w <= 1.0f && Col_z_w <= 1.0f)
    {
        glColor3f(Col_x_w, Col_y_w, Col_z_w);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glVertex3f(0.2f, 0.0f, 0.0f);
    glVertex3f(0.1f, 0.0f, 0.0f);

    glVertex3f(0.1f, 0.0f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.0f);

    Color_fads(&Col_x, &Col_y_G, &Col_z, COLOR_GREEN, 1);

    if (Col_y_G <= 1.0f)
    {
        glColor3f(0.0f, Col_y_G, 0.0f);
    }
    else
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    glVertex3f(0.2f, -0.3f, 0.0f);
    glVertex3f(0.1f, -0.3f, 0.0f);

    //D Right
    if (Col_x <= 1.0f || Col_y < 0.5f)
    {
        glColor3f(Col_x, Col_y, Col_z);
    }
    else
    {
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    glVertex3f(0.45f, 0.4f, 0.0f);
    glVertex3f(0.35f, 0.3f, 0.0f);

    Color_fads(&Col_x_w, &Col_y_w, &Col_z_w, COLOR_WHITE, 1);

    if (Col_x_w <= 1.0f && Col_y_w <= 1.0f && Col_z_w <= 1.0f)
    {
        glColor3f(Col_x_w, Col_y_w, Col_z_w);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glVertex3f(0.35f, 0.0f, 0.0f);
    glVertex3f(0.45f, 0.0f, 0.0f);

    glVertex3f(0.45f, 0.0f, 0.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);

    Color_fads(&Col_x, &Col_y_G, &Col_z, COLOR_GREEN, 1);

    if (Col_y_G <= 1.0f)
    {
        glColor3f(0.0f, Col_y_G, 0.0f);
    }
    else
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    glVertex3f(0.35f, -0.3f, 0.0f);
    glVertex3f(0.45f, -0.4f, 0.0f);

    //D bOTTOM
    glVertex3f(0.44f, -0.4f, 0.0f);
    glVertex3f(0.05f, -0.4f, 0.0f);

    glVertex3f(-0.05f, -0.3f, 0.0f);
    glVertex3f(0.34f, -0.3f, 0.0f);

    glEnd();
}

void Print_I_2()
{
    Moving(&X_i_2, &Y_i_2, &Z_i_2, ALPHA_SECOND_I);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(X_i_2, Y_i_2, Z_i_2);

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
}

void Print_A()
{
   Moving(&X_a, &Y_a, &Z_a, ALPHA_A);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(0.8f, 0.9f, 0.9f);

    glTranslatef(X_a, Y_a, Z_a);
    
    glBegin(GL_QUADS);
    //A Right 1

    glColor3f(1.1f, 0.5f, 0.0f);
    glVertex3f(1.46f, 0.4f, 0.0f);
    glVertex3f(1.36f, 0.3f, 0.0f);

    glColor3f(1.1f, 1.0f, 1.0f);
    glVertex3f(1.45f, 0.05f, 0.0f);
    glVertex3f(1.6f, 0.0f, 0.0f);

    //A right 2

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.75f, -0.4f, 0.0f);
    glVertex3f(1.6f, -0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.48f, -0.04f, 0.0f);
    glVertex3f(1.6f, 0.0f, 0.0f);

    glEnd();

    //Left 1
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.18f, 0.4f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.07f, 0.0f, 0.0f);
    glVertex3f(1.21f, 0.05f, 0.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.28f, 0.3f, 0.0f);

    //Left 2

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.07f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.97f, -0.4f, 0.0f);
    glVertex3f(1.105f, -0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.19f, -0.045f, 0.0f);

    glEnd();
    //Top Line
    Print_Top();
    //Bottom Line
    //Print_Bottom();

}

void Print_Top()
{
    // - //
    glBegin(GL_QUADS);
    //L B
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.19f, 0.4f, 0.0f);
    glVertex3f(1.45F, 0.4f, 0.0f);
    glVertex3f(1.35f, 0.3f, 0.0f);
    glVertex3f(1.29f, 0.3f, 0.0f);
   
    glEnd();
}

void Print_Bottom()
{
    
    // Center
    glBegin(GL_POLYGON);
/*
    //Color_fads(&Col_x, &Col_y_G, &Col_z, COLOR_ORANGE, 1);

    //if (Col_y_G <= 1.0f)
    //{
    //    glColor3f(0.0f, Col_y_G, 0.0f);
    //}
    //else
    //{
    //    glColor3f(1.0f, 1.0f, 1.0f);
    //}
    */
    //L CORNER
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.08f, 0.0f, 0.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.21f, 0.04f, 0.0f);
    
    Size_Inc(&N_P_3, 1);
    glVertex3f(N_P_3, 0.04f, 0.0f);
    
    Size_Inc(&N_P_4, 2);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(N_P_4, 0.0f, 0.0f);
    glEnd();

    ////R CORNER
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.08f, 0.0f, 0.0f);
    
    Size_Inc(&N_P_6, 4);
    glVertex3f(N_P_6, 0.0f, 0.0f);//59

    glColor3f(0.0f, 1.0f, 0.0f);

    Size_Inc(&N_P_7, 3);
    glVertex3f(N_P_7, -0.04f, 0.0f);//08

    glVertex3f(1.2f, -0.04f, 0.0f);
    
    glEnd();
}

void TopJetCall(void)
{
    //Local Variables 
    fRedius = 0.3f;
    fTrans_x = -14.0f;
    fTrans_y = 9.0f;
    fTrans_z = -16.0f;
    fScale_x = 5.0f;
    fScale_y = 0.55f;
    fScale_z = 0.0f;

    if (fIncremet_x <= 28.0f)
    {
        if (fIncremet_x >= 21.0f && fIncremet_x <= 26.0f)
        {
            Update(1,2);
            Direct_T = true;
            fIncremet_x = fIncremet_x + 0.002f;//0.002
            fTrans_x = fTrans_x + fIncremet_x;
            
        }
        else
        {
            fIncremet_x = fIncremet_x + 0.002f;
            fTrans_x = fTrans_x + fIncremet_x;
        }
        if (fIncremet_x >= 20.0f)
        {
            Print_Bottom();
        }
    }
    else
    {
        Print_Bottom();
        fTrans_x = 14.0f;
        flag_Exit = true;
        SendMessage(ghwnd_DM, WM_LBUTTONDOWN, 0, 0);
    }

    if (fIncremet_y >= 0.0f && Direct_T == true)
    {
        fIncremet_y = fIncremet_y - 0.002f;//0.002
        fTrans_y = fTrans_y - fIncremet_y;
    }
    else if(fIncremet_y <= 9.0f && Direct_T == false)
    {
        fIncremet_y = fIncremet_y + 0.002f;//0.002
        fTrans_y = fTrans_y - fIncremet_y;
        Update(1,1);
    }
    else if(fIncremet_x <= 21.0f && fIncremet_x >= -4.0f)
    {
        fTrans_y = 0.0f;
    }
    else if (fIncremet_x <= 21.0f && fIncremet_x >= 28.0f)
    {
        fTrans_y = 9.0f;
    }
    
    if (fTrans_x <= 28.0f)
        CircleDrawing(&fRedius, &fTrans_x, &fTrans_y, &fTrans_z, &fScale_x, &fScale_y, &fScale_z, true, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (fTrans_x <= 28.0f)
    {
        glTranslatef(fTrans_x, fTrans_y, fTrans_z);
        glRotatef(Jangle_T, 0.0f, 0.0f, 1.0f);
    }
    //top most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, 0.965f, 0.0f);
    glVertex3f(0.25f, 0.965f, 0.0f);
    glVertex3f(0.4f, 1.005f, 0.0f);
    glVertex3f(0.4f, 1.01f, 0.0f);
    glVertex3f(0.25f, 1.05f, 0.0f);
    glVertex3f(-0.15f, 1.05f, 0.0f);

    glEnd();

    //second top
    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, 0.635f, 0.0f);
    glVertex3f(0.25f, 0.635f, 0.0f);
    glVertex3f(0.4f, 0.675f, 0.0f);
    glVertex3f(0.4f, 0.685f, 0.0f);
    glVertex3f(0.25f, 0.725f, 0.0f);
    glVertex3f(-0.1f, 0.725f, 0.0f);

    glEnd();

    //Second bottom  

    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, -0.635f, 0.0f);
    glVertex3f(0.25f, -0.635f, 0.0f);
    glVertex3f(0.4f, -0.675f, 0.0f);
    glVertex3f(0.4f, -0.685f, 0.0f);
    glVertex3f(0.25f, -0.725f, 0.0f);
    glVertex3f(-0.1f, -0.725f, 0.0f);

    glEnd();

    //Bottomf most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, -0.965f, 0.0f);
    glVertex3f(0.25f, -0.965f, 0.0f);
    glVertex3f(0.4f, -1.005f, 0.0f);
    glVertex3f(0.4f, -1.01f, 0.0f);
    glVertex3f(0.25f, -1.05f, 0.0f);
    glVertex3f(-0.15f, -1.05f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(1.09f, 0.01f, 0.0f);
    glVertex3f(1.08f, 0.03f, 0.0f);
    glVertex3f(1.04f, 0.067f, 0.0f);
    glVertex3f(1.02f, 0.07f, 0.0f);
    glVertex3f(1.02f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.08f, 0.0f);

    glVertex3f(0.7f, 0.08f, 0.0f);
    glVertex3f(0.65f, 0.07f, 0.0f);
    glVertex3f(0.64f, 0.06f, 0.0f);
    glVertex3f(0.63f, 0.05f, 0.0f);
    glVertex3f(0.62f, 0.025f, 0.0f);
    glVertex3f(0.61f, 0.01f, 0.0f);
    glVertex3f(0.61f, -0.01f, 0.0f);
    glVertex3f(0.62f, -0.025f, 0.0f);
    glVertex3f(0.63f, -0.05f, 0.0f);
    glVertex3f(0.64f, -0.06f, 0.0f);
    glVertex3f(0.65f, -0.07f, 0.0f);
    glVertex3f(0.7f, -0.08f, 0.0f);

    glVertex3f(1.0f, -0.08f, 0.0f);
    glVertex3f(1.01f, -0.08f, 0.0f);
    glVertex3f(1.02f, -0.07f, 0.0f);
    glVertex3f(1.04f, -0.067f, 0.0f);
    glVertex3f(1.06f, -0.05f, 0.0f);
    glVertex3f(1.08f, -0.03f, 0.0f);
    glVertex3f(1.09f, -0.01f, 0.0f);
    glEnd();

    //Door Lines For Window.
    glColor3f(0.55f, 0.6f, 0.7f);
    glLineWidth(2.0f);


    glBegin(GL_LINES);

    glVertex3f(1.5f, 0.0f, 0.0f);
    glVertex3f(1.7f, 0.0f, 0.0f);

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.75f, 0.08f, 0.0f);
    glVertex3f(0.8f, 0.05f, 0.0f);

    glVertex3f(0.8f, -0.05f, 0.0f);
    glVertex3f(0.75f, -0.08f, 0.0f);

    glEnd();
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.95f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.05f, 0.0f);

    glVertex3f(1.0f, -0.05f, 0.0f);
    glVertex3f(0.95f, -0.08f, 0.0f);

    glEnd();
    glColor3f(0.55f, 0.6f, 0.7f);

    // These are Two main Big Wings of jet.........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, 0.2f, 0.0f);//5
    glVertex3f(-0.15f, 1.05f, 0.0f);//-05
    glVertex3f(-0.4f, 1.05f, 0.0f);//-3
    glVertex3f(-0.175f, 0.2f, 0.0f);//-075

    glEnd();
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, -0.2f, 0.0f);//5
    glVertex3f(-0.15f, -1.05f, 0.0f);//-05
    glVertex3f(-0.4f, -1.05f, 0.0f);//-3
    glVertex3f(-0.175f, -0.2f, 0.0f);//-075


    glEnd();


    // Middle body of the Jet..........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.6f, 0.15f, 0.0f);
    glVertex3f(0.4f, 0.3f, 0.0f);
    glVertex3f(-0.7f, 0.3f, 0.0f);

    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(0.4f, -0.3f, 0.0f);
    glVertex3f(0.6f, -0.15f, 0.0f);

    glEnd();

    //These are two wings of the Jet (last Small)..............
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, 0.3f, 0.0f);//45
    glVertex3f(-0.65f, 0.8f, 0.0f);//65
    glVertex3f(-0.85f, 0.7f, 0.0f);//9
    glVertex3f(-0.84f, 0.43f, 0.0f);//89
    glVertex3f(-0.7f, 0.3f, 0.0f);//7

    glEnd();

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, -0.3f, 0.0f);//45
    glVertex3f(-0.65f, -0.8f, 0.0f);//65
    glVertex3f(-0.85f, -0.7f, 0.0f);//9
    glVertex3f(-0.84f, -0.43f, 0.0f);//89
    glVertex3f(-0.7f, -0.3f, 0.0f);//7

    glEnd();


    //These are three Last burnes of the jett.................
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.29f, 0.0f);//25
    glVertex3f(-1.0f, 0.34f, 0.0f);//4
    glVertex3f(-1.0f, 0.09f, 0.0f);//5
    glVertex3f(-0.7f, 0.13f, 0.0f);//4

    glEnd();
   
    
    
    //smoke
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-1.0f, 0.05f, 0.0f);//25
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.6f, 0.05f, 0.0f);//4
    glVertex3f(-1.6f, -0.025f, 0.0f);//4
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-1.0f, -0.025f, 0.0f);//4
    glEnd();


    glColor3f(0.55f, 0.6f, 0.7f);

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, -0.29f, 0.0f);//25
    glVertex3f(-1.0f, -0.34f, 0.0f);//4
    glVertex3f(-1.0f, -0.09f, 0.0f);//5
    glVertex3f(-0.7f, -0.13f, 0.0f);//4

    glEnd();

    //3.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.05f, 0.0f);//25
    glVertex3f(-1.1f, 0.05f, 0.0f);//4
    glVertex3f(-1.2f, 0.025f, 0.0f);//4
    glVertex3f(-1.2f, -0.025f, 0.0f);//4
    glVertex3f(-1.1f, -0.05f, 0.0f);//5
    glVertex3f(-0.7f, -0.05f, 0.0f);//4

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    // I
    glVertex3f(-0.35f, 0.08f, 0.0f);
    glVertex3f(-0.25f, 0.08f, 0.0f);

    glVertex3f(-0.35f, -0.08f, 0.0f);
    glVertex3f(-0.25f, -0.08f, 0.0f);

    glVertex3f(-0.3f, 0.08f, 0.0f);
    glVertex3f(-0.3f, -0.08f, 0.0f);

    // A
    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.2f, -0.08f, 0.0f);

    glVertex3f(-0.17f, 0.006f, 0.0f);
    glVertex3f(-0.1f, 0.006f, 0.0f);

    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.08f, -0.08f, 0.0f);

    // F
    glVertex3f(0.0f, -0.08f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);

    glVertex3f(0.0f, 0.09f, 0.0f);
    glVertex3f(0.1f, 0.09f, 0.0f);

    glVertex3f(0.0f, 0.015f, 0.0f);
    glVertex3f(0.1f, 0.015f, 0.0f);


    glEnd();

}

void MiddleJetCall(void)
{
    fRedius = 0.3f;
    fTrans_x = -14.0f;
    fTrans_y = 0.0f;
    fTrans_z = -16.0f;
    fScale_x = 5.0f;
    fScale_y = 0.55f;
    fScale_z = 0.0f;

    if (fIncremet_x_Middle <= 28.0f)
    {
        fIncremet_x_Middle = fIncremet_x_Middle + 0.002f;
        fTrans_x = fTrans_x + fIncremet_x_Middle;
    }
    else
    {
        fTrans_x = 14.0f;
    }

    CircleDrawing(&fRedius, &fTrans_x, &fTrans_y, &fTrans_z, &fScale_x, &fScale_y, &fScale_z, false, 2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(fTrans_x, fTrans_y, fTrans_z);

    //top most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, 0.965f, 0.0f);
    glVertex3f(0.25f, 0.965f, 0.0f);
    glVertex3f(0.4f, 1.005f, 0.0f);
    glVertex3f(0.4f, 1.01f, 0.0f);
    glVertex3f(0.25f, 1.05f, 0.0f);
    glVertex3f(-0.15f, 1.05f, 0.0f);

    glEnd();

    //second top
    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, 0.635f, 0.0f);
    glVertex3f(0.25f, 0.635f, 0.0f);
    glVertex3f(0.4f, 0.675f, 0.0f);
    glVertex3f(0.4f, 0.685f, 0.0f);
    glVertex3f(0.25f, 0.725f, 0.0f);
    glVertex3f(-0.1f, 0.725f, 0.0f);

    glEnd();

    //Second bottom  

    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, -0.635f, 0.0f);
    glVertex3f(0.25f, -0.635f, 0.0f);
    glVertex3f(0.4f, -0.675f, 0.0f);
    glVertex3f(0.4f, -0.685f, 0.0f);
    glVertex3f(0.25f, -0.725f, 0.0f);
    glVertex3f(-0.1f, -0.725f, 0.0f);

    glEnd();

    //Bottomf most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, -0.965f, 0.0f);
    glVertex3f(0.25f, -0.965f, 0.0f);
    glVertex3f(0.4f, -1.005f, 0.0f);
    glVertex3f(0.4f, -1.01f, 0.0f);
    glVertex3f(0.25f, -1.05f, 0.0f);
    glVertex3f(-0.15f, -1.05f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(1.09f, 0.01f, 0.0f);
    glVertex3f(1.08f, 0.03f, 0.0f);
    glVertex3f(1.04f, 0.067f, 0.0f);
    glVertex3f(1.02f, 0.07f, 0.0f);
    glVertex3f(1.02f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.08f, 0.0f);

    glVertex3f(0.7f, 0.08f, 0.0f);
    glVertex3f(0.65f, 0.07f, 0.0f);
    glVertex3f(0.64f, 0.06f, 0.0f);
    glVertex3f(0.63f, 0.05f, 0.0f);
    glVertex3f(0.62f, 0.025f, 0.0f);
    glVertex3f(0.61f, 0.01f, 0.0f);
    glVertex3f(0.61f, -0.01f, 0.0f);
    glVertex3f(0.62f, -0.025f, 0.0f);
    glVertex3f(0.63f, -0.05f, 0.0f);
    glVertex3f(0.64f, -0.06f, 0.0f);
    glVertex3f(0.65f, -0.07f, 0.0f);
    glVertex3f(0.7f, -0.08f, 0.0f);

    glVertex3f(1.0f, -0.08f, 0.0f);
    glVertex3f(1.01f, -0.08f, 0.0f);
    glVertex3f(1.02f, -0.07f, 0.0f);
    glVertex3f(1.04f, -0.067f, 0.0f);
    glVertex3f(1.06f, -0.05f, 0.0f);
    glVertex3f(1.08f, -0.03f, 0.0f);
    glVertex3f(1.09f, -0.01f, 0.0f);
    glEnd();

    //Door Lines For Window.
    glColor3f(0.55f, 0.6f, 0.7f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(1.5f, 0.0f, 0.0f);
    glVertex3f(1.7f, 0.0f, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.75f, 0.08f, 0.0f);
    glVertex3f(0.8f, 0.05f, 0.0f);

    glVertex3f(0.8f, -0.05f, 0.0f);
    glVertex3f(0.75f, -0.08f, 0.0f);

    glEnd();
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.95f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.05f, 0.0f);

    glVertex3f(1.0f, -0.05f, 0.0f);
    glVertex3f(0.95f, -0.08f, 0.0f);

    glEnd();
    glColor3f(0.55f, 0.6f, 0.7f);

    // These are Two main Big Wings of jet.........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, 0.2f, 0.0f);//5
    glVertex3f(-0.15f, 1.05f, 0.0f);//-05
    glVertex3f(-0.4f, 1.05f, 0.0f);//-3
    glVertex3f(-0.175f, 0.2f, 0.0f);//-075

    glEnd();
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, -0.2f, 0.0f);//5
    glVertex3f(-0.15f, -1.05f, 0.0f);//-05
    glVertex3f(-0.4f, -1.05f, 0.0f);//-3
    glVertex3f(-0.175f, -0.2f, 0.0f);//-075


    glEnd();


    // Middle body of the Jet..........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.6f, 0.15f, 0.0f);
    glVertex3f(0.4f, 0.3f, 0.0f);
    glVertex3f(-0.7f, 0.3f, 0.0f);

    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(0.4f, -0.3f, 0.0f);
    glVertex3f(0.6f, -0.15f, 0.0f);

    glEnd();

    //These are two wings of the Jet (last Small)..............
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, 0.3f, 0.0f);//45
    glVertex3f(-0.65f, 0.8f, 0.0f);//65
    glVertex3f(-0.85f, 0.7f, 0.0f);//9
    glVertex3f(-0.84f, 0.43f, 0.0f);//89
    glVertex3f(-0.7f, 0.3f, 0.0f);//7

    glEnd();

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, -0.3f, 0.0f);//45
    glVertex3f(-0.65f, -0.8f, 0.0f);//65
    glVertex3f(-0.85f, -0.7f, 0.0f);//9
    glVertex3f(-0.84f, -0.43f, 0.0f);//89
    glVertex3f(-0.7f, -0.3f, 0.0f);//7

    glEnd();


    //These are three Last burnes of the jett.................
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.29f, 0.0f);//25
    glVertex3f(-1.0f, 0.34f, 0.0f);//4
    glVertex3f(-1.0f, 0.09f, 0.0f);//5
    glVertex3f(-0.7f, 0.13f, 0.0f);//4

    glEnd();

    if (fIncremet_x >= 9.0f && fIncremet_x <= 22.0f)
    {
        //smoke
        glBegin(GL_QUADS);

        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-1.0f, 0.125f, 0.0f);//25

        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-1.6f, 0.125f, 0.0f);//4
        glVertex3f(-1.6f, 0.05f, 0.0f);//4

        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-0.8f, 0.05f, 0.0f);//4
        glEnd();
    }

    //smoke
    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 0.05f, 0.0f);//25
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.6f, 0.05f, 0.0f);//4
    glVertex3f(-1.6f, -0.025f, 0.0f);//4
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.8f, -0.025f, 0.0f);//4

    glEnd();
    if (fIncremet_x >= 9.0f && fIncremet_x <= 22.0f)
    {
        //smoke
        glBegin(GL_QUADS);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -0.025f, 0.0f);//25
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-1.6f, -0.025f, 0.0f);//4
        glVertex3f(-1.6f, -0.095f, 0.0f);//4
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.8f, -0.095f, 0.0f);//4
        
        glEnd();
    }

    glColor3f(0.55f, 0.6f, 0.7f);
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, -0.29f, 0.0f);//25
    glVertex3f(-1.0f, -0.34f, 0.0f);//4
    glVertex3f(-1.0f, -0.09f, 0.0f);//5
    glVertex3f(-0.7f, -0.13f, 0.0f);//4

    glEnd();

    //3.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.05f, 0.0f);//25
    glVertex3f(-1.1f, 0.05f, 0.0f);//4
    glVertex3f(-1.2f, 0.025f, 0.0f);//4
    glVertex3f(-1.2f, -0.025f, 0.0f);//4
    glVertex3f(-1.1f, -0.05f, 0.0f);//5
    glVertex3f(-0.7f, -0.05f, 0.0f);//4

    glEnd();

    

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    // I
    glVertex3f(-0.35f, 0.08f, 0.0f);
    glVertex3f(-0.25f, 0.08f, 0.0f);

    glVertex3f(-0.35f, -0.08f, 0.0f);
    glVertex3f(-0.25f, -0.08f, 0.0f);

    glVertex3f(-0.3f, 0.08f, 0.0f);
    glVertex3f(-0.3f, -0.08f, 0.0f);

    // A
    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.2f, -0.08f, 0.0f);

    glVertex3f(-0.17f, 0.006f, 0.0f);
    glVertex3f(-0.1f, 0.006f, 0.0f);

    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.08f, -0.08f, 0.0f);

    // F
    glVertex3f(0.0f, -0.08f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);

    glVertex3f(0.0f, 0.09f, 0.0f);
    glVertex3f(0.1f, 0.09f, 0.0f);

    glVertex3f(0.0f, 0.015f, 0.0f);
    glVertex3f(0.1f, 0.015f, 0.0f);

    glEnd();
}

void BottomJetCall(void)
{
    fRedius = 0.3f;
    fTrans_x = -14.0f;
    fTrans_y = -9.0f;
    fTrans_z = -16.0f;
    fScale_x = 5.0f;
    fScale_y = 0.55f;
    fScale_z = 0.0f;

    if (fIncremet_x_Bottom <= 28.0f)
    {
        if (fIncremet_x_Bottom >= 21.0f && fIncremet_x_Bottom <= 28.0f)
        {
            Update(3, 2);
            Direct_B = true;
            fIncremet_x_Bottom = fIncremet_x_Bottom + 0.002f;
            fTrans_x = fTrans_x + fIncremet_x_Bottom;
        }
        else
        {
            fIncremet_x_Bottom = fIncremet_x_Bottom + 0.002f;
            fTrans_x = fTrans_x + fIncremet_x_Bottom;
        }
    }
    else
    {
        fTrans_x = 18.0f;
    }

    if (fIncremet_y_Bottom >= 0.0f && Direct_B == true)
    {
        fIncremet_y_Bottom = fIncremet_y_Bottom - 0.002f;
        fTrans_y = fTrans_y + fIncremet_y_Bottom;
    }
    else if (fIncremet_y_Bottom <= 9.0f && Direct_B == false)
    {
        fIncremet_y_Bottom = fIncremet_y_Bottom + 0.002f;
        fTrans_y = fTrans_y + fIncremet_y_Bottom;
        Update(3,1);
    }
    else if (fIncremet_x_Bottom <= 21.0f && fIncremet_x_Bottom >= -4.0f)
    {
        fTrans_y = 0.0f;
    }
    else if (fIncremet_x_Bottom <= 28.0f && fIncremet_x_Bottom >= 21.0f)
    {
        fTrans_y = -9.0f;
    }

    if (fTrans_x <= 14.0f)
        CircleDrawing(&fRedius, &fTrans_x, &fTrans_y, &fTrans_z, &fScale_x, &fScale_y, &fScale_z, true , 3);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (fTrans_x <= 14.0f)
    {
        glTranslatef(fTrans_x, fTrans_y, fTrans_z);
        glRotatef(Jangle_B, 0.0f, 0.0f, 1.0f);
    }

    //top most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, 0.965f, 0.0f);
    glVertex3f(0.25f, 0.965f, 0.0f);
    glVertex3f(0.4f, 1.005f, 0.0f);
    glVertex3f(0.4f, 1.01f, 0.0f);
    glVertex3f(0.25f, 1.05f, 0.0f);
    glVertex3f(-0.15f, 1.05f, 0.0f);

    glEnd();

    //second top
    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, 0.635f, 0.0f);
    glVertex3f(0.25f, 0.635f, 0.0f);
    glVertex3f(0.4f, 0.675f, 0.0f);
    glVertex3f(0.4f, 0.685f, 0.0f);
    glVertex3f(0.25f, 0.725f, 0.0f);
    glVertex3f(-0.1f, 0.725f, 0.0f);

    glEnd();

    //Second bottom  

    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, -0.635f, 0.0f);
    glVertex3f(0.25f, -0.635f, 0.0f);
    glVertex3f(0.4f, -0.675f, 0.0f);
    glVertex3f(0.4f, -0.685f, 0.0f);
    glVertex3f(0.25f, -0.725f, 0.0f);
    glVertex3f(-0.1f, -0.725f, 0.0f);

    glEnd();

    //Bottomf most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, -0.965f, 0.0f);
    glVertex3f(0.25f, -0.965f, 0.0f);
    glVertex3f(0.4f, -1.005f, 0.0f);
    glVertex3f(0.4f, -1.01f, 0.0f);
    glVertex3f(0.25f, -1.05f, 0.0f);
    glVertex3f(-0.15f, -1.05f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(1.09f, 0.01f, 0.0f);
    glVertex3f(1.08f, 0.03f, 0.0f);
    glVertex3f(1.04f, 0.067f, 0.0f);
    glVertex3f(1.02f, 0.07f, 0.0f);
    glVertex3f(1.02f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.08f, 0.0f);

    glVertex3f(0.7f, 0.08f, 0.0f);
    glVertex3f(0.65f, 0.07f, 0.0f);
    glVertex3f(0.64f, 0.06f, 0.0f);
    glVertex3f(0.63f, 0.05f, 0.0f);
    glVertex3f(0.62f, 0.025f, 0.0f);
    glVertex3f(0.61f, 0.01f, 0.0f);
    glVertex3f(0.61f, -0.01f, 0.0f);
    glVertex3f(0.62f, -0.025f, 0.0f);
    glVertex3f(0.63f, -0.05f, 0.0f);
    glVertex3f(0.64f, -0.06f, 0.0f);
    glVertex3f(0.65f, -0.07f, 0.0f);
    glVertex3f(0.7f, -0.08f, 0.0f);

    glVertex3f(1.0f, -0.08f, 0.0f);
    glVertex3f(1.01f, -0.08f, 0.0f);
    glVertex3f(1.02f, -0.07f, 0.0f);
    glVertex3f(1.04f, -0.067f, 0.0f);
    glVertex3f(1.06f, -0.05f, 0.0f);
    glVertex3f(1.08f, -0.03f, 0.0f);
    glVertex3f(1.09f, -0.01f, 0.0f);
    glEnd();

    //Door Lines For Window.
    glColor3f(0.55f, 0.6f, 0.7f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(1.5f, 0.0f, 0.0f);
    glVertex3f(1.7f, 0.0f, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.75f, 0.08f, 0.0f);
    glVertex3f(0.8f, 0.05f, 0.0f);

    glVertex3f(0.8f, -0.05f, 0.0f);
    glVertex3f(0.75f, -0.08f, 0.0f);

    glEnd();
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.95f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.05f, 0.0f);

    glVertex3f(1.0f, -0.05f, 0.0f);
    glVertex3f(0.95f, -0.08f, 0.0f);

    glEnd();
    glColor3f(0.55f, 0.6f, 0.7f);

    // These are Two main Big Wings of jet.........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, 0.2f, 0.0f);//5
    glVertex3f(-0.15f, 1.05f, 0.0f);//-05
    glVertex3f(-0.4f, 1.05f, 0.0f);//-3
    glVertex3f(-0.175f, 0.2f, 0.0f);//-075

    glEnd();
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, -0.2f, 0.0f);//5
    glVertex3f(-0.15f, -1.05f, 0.0f);//-05
    glVertex3f(-0.4f, -1.05f, 0.0f);//-3
    glVertex3f(-0.175f, -0.2f, 0.0f);//-075


    glEnd();


    // Middle body of the Jet..........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.6f, 0.15f, 0.0f);
    glVertex3f(0.4f, 0.3f, 0.0f);
    glVertex3f(-0.7f, 0.3f, 0.0f);

    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(0.4f, -0.3f, 0.0f);
    glVertex3f(0.6f, -0.15f, 0.0f);

    glEnd();

    //These are two wings of the Jet (last Small)..............
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, 0.3f, 0.0f);//45
    glVertex3f(-0.65f, 0.8f, 0.0f);//65
    glVertex3f(-0.85f, 0.7f, 0.0f);//9
    glVertex3f(-0.84f, 0.43f, 0.0f);//89
    glVertex3f(-0.7f, 0.3f, 0.0f);//7

    glEnd();

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, -0.3f, 0.0f);//45
    glVertex3f(-0.65f, -0.8f, 0.0f);//65
    glVertex3f(-0.85f, -0.7f, 0.0f);//9
    glVertex3f(-0.84f, -0.43f, 0.0f);//89
    glVertex3f(-0.7f, -0.3f, 0.0f);//7

    glEnd();


    //These are three Last burnes of the jett.................
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.29f, 0.0f);//25
    glVertex3f(-1.0f, 0.34f, 0.0f);//4
    glVertex3f(-1.0f, 0.09f, 0.0f);//5
    glVertex3f(-0.7f, 0.13f, 0.0f);//4

    glEnd();

    //smoke
    glBegin(GL_QUADS);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.05f, 0.0f);//25
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.6f, 0.05f, 0.0f);//4
    glVertex3f(-1.6f, -0.025f, 0.0f);//4
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -0.025f, 0.0f);//4
    glEnd();

    glColor3f(0.55f, 0.6f, 0.7f);

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, -0.29f, 0.0f);//25
    glVertex3f(-1.0f, -0.34f, 0.0f);//4
    glVertex3f(-1.0f, -0.09f, 0.0f);//5
    glVertex3f(-0.7f, -0.13f, 0.0f);//4

    glEnd();

    //3.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.05f, 0.0f);//25
    glVertex3f(-1.1f, 0.05f, 0.0f);//4
    glVertex3f(-1.2f, 0.025f, 0.0f);//4
    glVertex3f(-1.2f, -0.025f, 0.0f);//4
    glVertex3f(-1.1f, -0.05f, 0.0f);//5
    glVertex3f(-0.7f, -0.05f, 0.0f);//4

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    // I
    glVertex3f(-0.35f, 0.08f, 0.0f);
    glVertex3f(-0.25f, 0.08f, 0.0f);

    glVertex3f(-0.35f, -0.08f, 0.0f);
    glVertex3f(-0.25f, -0.08f, 0.0f);

    glVertex3f(-0.3f, 0.08f, 0.0f);
    glVertex3f(-0.3f, -0.08f, 0.0f);

    // A
    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.2f, -0.08f, 0.0f);

    glVertex3f(-0.17f, 0.006f, 0.0f);
    glVertex3f(-0.1f, 0.006f, 0.0f);

    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.08f, -0.08f, 0.0f);

    // F
    glVertex3f(0.0f, -0.08f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);

    glVertex3f(0.0f, 0.09f, 0.0f);
    glVertex3f(0.1f, 0.09f, 0.0f);

    glVertex3f(0.0f, 0.015f, 0.0f);
    glVertex3f(0.1f, 0.015f, 0.0f);

    glEnd();

}

void CircleDrawing(GLfloat* redius, GLfloat* fTrans_x, GLfloat* fTrans_y, GLfloat* fTrans_z, GLfloat* fScale_x, GLfloat* fScale_y, GLfloat* fScale_z, bool flag, int position)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

        glTranslatef(*fTrans_x, *fTrans_y, *fTrans_z);
        if (flag == true)
        {
            if(position == 1)
                glRotatef(Jangle_T, 0.0f, 0.0f, 1.0f);
            if (position == 2)
            {
                //glRotatef(Jangle_M, 0.0f, 0.0f, 1.0f);
            }
            if (position == 3)
            {
                glRotatef(Jangle_B, 0.0f, 0.0f, 1.0f);
            }
        }
        glScalef(*fScale_x, *fScale_y, *fScale_z);
    
        glBegin(GL_POLYGON);

    glColor3f(0.55f, 0.6f, 0.7f);
    for (GLfloat angle = 0.0f; angle <= 2 * PI; angle = angle + 0.001f)
    {
        x = (GLfloat)cos(angle) * (*redius);
        y = (GLfloat)sin(angle) * (*redius);

        if (x <= -0.0f)
        {
        }
        else
        {
            glVertex3f(x, y, 0.0f);
        }
    }
    glEnd();
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


