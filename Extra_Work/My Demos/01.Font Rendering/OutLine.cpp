#include<windows.h>
#include"Header.h"
#include<gl/GL.h>
#include<stdio.h>
#include<gl/GLU.h>
#include<math.h>

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Glu32.lib")


/*
    
    1) I have Used Some Global variables.
        1. GLuint  base;  
        2. GLYPHMETRICSFLOAT gmf[256];
        3. HDC ghdc_DM = NULL;                          // This is Not New it is already inside our templete code.



    2) There Are Totally 3 Fuctions Which will be used for rendering the Font.
        1. BuildFont()
        2. Print_Font()
        3. Killfont()
        4. Display_For_Font()

    3) Where to use them 
        1. BuildFont() :
                        Here we are building our Font, so we need to be ready before our drowing for this we will call this Fuction inside Initialise.
                        We can call this fuction any where inside Initialise() function.

        2. print_Font() :
                        Here we are actually printing the rules so obiouslly we will call this fuction inside inside our Display_For_Font() function.

        3.Display_For_Font():
                        This is the fuction where we are Displaying the Font on the screen so we will call this inside the Our Original Display Function.

        4.KillFont() :
                        Now We Free to delete our Font. I have Deleted it in side UnInitialize() function.


*/
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

// Globle avriables 
HINSTANCE   hInstance;                 
GLuint  base;                          

GLYPHMETRICSFLOAT gmf[256];                 

//Local Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);
void Display_For_Font(void);


//New
GLvoid BuildFont(GLvoid);
GLvoid Print_Font(const char*, ...);
GLvoid KillFont(GLvoid);


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
        TEXT("MY Font !"),
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


    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //Functions for Depth

    //for getting anti_Allies
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 

    Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM);

    BuildFont();                        // Build The Font               ( ADD )
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


///                                                                                             Build Function Definition 
GLvoid BuildFont(GLvoid)                    // Build Our Bitmap Font
{
    HFONT   font;                           // Windows Font ID

    base = glGenLists(256);                 // Storage For 256 Characters
    font = CreateFont(-12,                  // Height Of Font
        0,                                  // Width Of Font
        0,                                  // Angle Of Escapement
        0,                                  // Orientation Angle
        100,                                // Font Weight
        FALSE,                              // Italic
        FALSE,                              // Underline
        FALSE,                              // Strikeout
        ANSI_CHARSET,                       // Character Set Identifier
        OUT_TT_PRECIS,                      // Output Precision
        CLIP_DEFAULT_PRECIS,                // Clipping Precision
        ANTIALIASED_QUALITY,                // Output Quality
        FF_DONTCARE | DEFAULT_PITCH,        // Family And Pitch
        TEXT("Palatino Linotype"));         // Font Name

    SelectObject(ghdc_DM, font);            // Selects The Font We Created

    wglUseFontOutlines(ghdc_DM,             // Select The Current DC
        0,                                  // Starting Character
        255,                                // Number Of Display Lists To Build
        base,                               // Starting Display Lists
        0.0f,                               // Deviation From The True Outlines
        0.05f,                              // Font Thickness In The Z Direction
        WGL_FONT_POLYGONS,                  // Use Polygons, Not Lines
        gmf);                               // Address Of Buffer To Recieve Data
}

///                                                                                             Kill Font Function Definition 
GLvoid KillFont(GLvoid)                     // Delete The Font
{
    glDeleteLists(base, 256);               // Delete All 256 Characters
}

///                                                                                             Print Font Function Definition 
GLvoid Print_Font(const char* fmt, ...)     // Custom GL "Print" Routine
{
    char    *text;                          // Holds Our String
    
    if(fmt == NULL)                         // If There's No Text
        return;    

    // Do Nothing
    text = (char *)fmt;
    
    glPushAttrib(GL_LIST_BIT);                          // Pushes The Display List Bits
    glListBase(base);                                   // Sets The Base Character to 0
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text
    glPopAttrib();                                      // Pops The Display List Bits
}

void Display()
{
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //                                                          Display_For_Font() Calling here.
    Display_For_Font();

    SwapBuffers(ghdc_DM);

}

///                                                                                             DisplayForfont Function Definition 
void Display_For_Font()
{

    //X And Y for All Models(Fonts)
    static GLfloat X_for_Fix_Left_Pararent_Trans = -8.0f,
        Z_for_Fix_Parent_Depth_Trans = -30.0f;

    static GLfloat X_for_Fix_Font_child = -3.3f,
        Y_for_Fix_Child = -1.0f;

    //Chaging the positions of Fonts Incremetal variables
    static GLfloat Trans_X = X_for_Fix_Left_Pararent_Trans,
        Trans_Y = 0.0f,
        Trans_Z = 0.0f;

    //Flags for Transformation


    //color chaging varibles
    static GLfloat Col_varient = 0.0f;


    //Flags for Transformation Color Visibility
    static bool flag_for_1_Rule = true;
    static bool flag_for_2_Rule = false;
    static bool flag_for_3_Rule = false;
    static bool flag_for_4_Rule = false;
    static bool flag_for_5_Rule = false;

    static bool flag_for_Default_2 = false;
    static bool flag_for_Default_3 = false;
    static bool flag_for_Default_4 = false;
    static bool flag_for_Default_5 = false;


   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(X_for_Fix_Left_Pararent_Trans, 5.0f, Z_for_Fix_Parent_Depth_Trans);             // Move Ten Units Into The Screen
    if (flag_for_1_Rule)
    {
        if (Col_varient > 0.9f)
        {
            Col_varient = 0.0f;
            flag_for_2_Rule = true;
            flag_for_1_Rule = false;

        }
        else
        {
            glColor3f(Col_varient, Col_varient, Col_varient);
        }
    }
    else
    {
        glColor3f(0.9f, 0.9f, 0.9f);
    }
    Print_Font("1. Always compare yourself with yourself, ");             // Print GL Text To The Screen
    glTranslatef(X_for_Fix_Font_child, Y_for_Fix_Child, 0.0f);
    Print_Font("not with others.");

    /////////////////////////////////                                                                                                                                       2222222222
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(X_for_Fix_Left_Pararent_Trans, 3.0f, Z_for_Fix_Parent_Depth_Trans);
    if (flag_for_2_Rule)
    {
        if (Col_varient > 0.9f)
        {
            Col_varient = 0.0f;
            flag_for_3_Rule = true;
            flag_for_2_Rule = false;
            flag_for_Default_2 = true;
        }
        else
        {
            glColor3f(Col_varient, Col_varient, Col_varient);
        }
    }
    else if (flag_for_Default_2)
    {
        glColor3f(0.9f, 0.9f, 0.9f);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    Print_Font("2. Never ever forget the basics.");


    /////////////////////////////////                                                                                                                                       333333333
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(X_for_Fix_Left_Pararent_Trans, 1.0f, Z_for_Fix_Parent_Depth_Trans);             // Move Ten Units Into The Screen
    if (flag_for_3_Rule)
    {
        if (Col_varient > 0.9f)
        {
            Col_varient = 0.0f;
            flag_for_4_Rule = true;
            flag_for_3_Rule = false;
            flag_for_Default_3 = true;
        }
        else
        {
            glColor3f(Col_varient, Col_varient, Col_varient);
        }
    }
    else if (flag_for_Default_3)
    {
        glColor3f(0.9f, 0.9f, 0.9f);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    Print_Font("3. Master more than one field.");


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(X_for_Fix_Left_Pararent_Trans, -1.0f, Z_for_Fix_Parent_Depth_Trans);             // Move Ten Units Into The Screen
    if (flag_for_4_Rule)
    {
        if (Col_varient > 0.9f)
        {
            Col_varient = 0.0f;
            flag_for_5_Rule = true;
            flag_for_4_Rule = false;
            flag_for_Default_4 = true;
        }
        else
        {
            glColor3f(Col_varient, Col_varient, Col_varient);
        }
    }
    else if (flag_for_Default_4)
    {
        glColor3f(0.9f, 0.9f, 0.9f);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    Print_Font("4. Money is fleeting instead Worship knowledge.");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(X_for_Fix_Left_Pararent_Trans, -3.0f, Z_for_Fix_Parent_Depth_Trans);             // Move Ten Units Into The Screen
    if (flag_for_5_Rule)
    {
        if (Col_varient > 0.9f)
        {
            Col_varient = 0.0f;
            flag_for_5_Rule = false;
            flag_for_Default_5 = true;
        }
        else
        {
            glColor3f(Col_varient, Col_varient, Col_varient);
        }
    }
    else if (flag_for_Default_5)
    {

        glColor3f(0.9f, 0.9f, 0.9f);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    Print_Font("5. Whatever you do,");

    glTranslatef(X_for_Fix_Font_child, Y_for_Fix_Child, 0.0f);
    Print_Font("  do it from the bottom of your heart.");


    if (Col_varient < 0.9f)
        Col_varient += 0.002f;
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
    KillFont();
}

