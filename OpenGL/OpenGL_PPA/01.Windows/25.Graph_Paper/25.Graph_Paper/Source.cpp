#include<windows.h>
#include"Header.h"

#include<gl/glew.h>

#include"vmath.h"

#include<gl/GL.h>
#include<stdio.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")


#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

using namespace vmath;

enum
{
    DVM_ATTTRIBUTE_POSITION = 0,
    DVM_ATTTRIBUTE_COLOR,
    DVM_ATTTRIBUTE_NORMAL,
    DVM_ATTTRIBUTE_TEXCORD,
};

//Global Fuction 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Global Variables Declaration 
FILE* gpFile_DM = NULL;

HWND ghwnd_DM = NULL;
HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;


DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };


bool gdFullScreen_DM = false;
bool gbEscapekeyIsPressed_DM = false;
bool gbActiveWindows_DM = false;

//Graph
GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gshaderProgramObject;

//Local Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);

//graph
GLuint gVao;
GLuint gVbo_Position;
GLuint gVbo_Color;

//Square
GLuint S_gVao;
GLuint S_gVbo_Position;
GLuint S_gVbo_Color;

//Triangle
GLuint T_gVao;
GLuint T_gVbo_Position;
GLuint T_gVbo_Color;

//circle
GLuint C_gVao;
GLuint C_gVbo_Position;
GLuint C_gVbo_Color;

GLuint gMVPMatrixUniform;
mat4 gPerspectiveProjectMatix;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpszCmdLine, int iCmdShow)
{
    //Function 

    void Initialize(void);

    WNDCLASSEX wndclassex;
    HWND hwnd;
    MSG msg;
    TCHAR Appname[] = TEXT("OpenGLPP");
    bool bDone = false;

    INT x = 0, y = 0;
    INT Width = 800;
    INT Height = 600;

    if (fopen_s(&gpFile_DM, "log.txt", "w") != 0)
    {
        MessageBox(NULL, TEXT(" File Can Not Created!"), TEXT("Error"), MB_OK);
    }
    else
    {
        fprintf_s(gpFile_DM, "Log File is Successfully Opened.\n");
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
        TEXT("OpenGL Programmable Pipeline : Mulri Color Triangle "),
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

    RECT rc_DM;

    //Function
    void ToggelFullScreen(void);


    switch (iMsg)
    {
    case WM_PAINT:
        GetClientRect(hwnd, &rc_DM);
        Resize(rc_DM.right, rc_DM.bottom);
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

    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        wglDeleteContext(ghrc_DM);
        ghdc_DM = NULL;
        ReleaseDC(ghwnd_DM, ghdc_DM);
        ghdc_DM = NULL;
    }

    fprintf_s(gpFile_DM, "\n**************************OpenGL Log ******************************\n");
    fprintf_s(gpFile_DM, "OpenGL Vender : %s\n", glGetString(GL_VENDOR));
    fprintf_s(gpFile_DM, "OpenGL Render : %s\n", glGetString(GL_RENDER));
    fprintf_s(gpFile_DM, "OpenGL Version : %s\n", glGetString(GL_VERSION));
    fprintf_s(gpFile_DM, "OpenGL GLSL(Graphic language Shading language) : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    fprintf_s(gpFile_DM, "\n**************************OpenGL Enable Extations ******************************\n");
    GLint num_Ext;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_Ext);
    for (GLint i = 0; i < num_Ext; i++)
    {
        fprintf_s(gpFile_DM, "\n%s\n", glGetStringi(GL_EXTENSIONS, i));
    }

    /* Vertex Shader Object */
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode =
        "#version 450 core\n" \
        "in vec4 vPosition;" \
        "in vec4 vColor;" \
        "uniform mat4 u_mvp_matrix;" \
        "out vec4 out_Color;" \
        "void main(void){\n" \
        "gl_Position = u_mvp_matrix * vPosition;" \
        "out_Color = vColor;" \
        "\n" \
        "}";

    glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


    glCompileShader(gVertexShaderObject);
    /*Here there should be vertex shader complication error Cheking */

    GLint iInfoLogLength = 0;
    GLint iShaderCompileStatus = 0;
    char* szInfoLog = NULL;

    glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Vertex Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                unInitialize();
                exit(0);
            }
        }
    }

    /* Fragment Shader Object */
    gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* fragmentShaderSourceCode =
        "#version 450 core" \
        "\n" \
        "in vec4 out_Color;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "\n" \
        "{" \
        "\n" \
        "FragColor = out_Color;" \
        "\n"
        "}";

    glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

    glCompileShader(gFragmentShaderObject);
    /*Here there should be Fragment shader complication error Cheking */

    glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Fragment Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd_DM);
            }
        }
    }


    gshaderProgramObject = glCreateProgram();

    glAttachShader(gshaderProgramObject, gVertexShaderObject);
    glAttachShader(gshaderProgramObject, gFragmentShaderObject);

    glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_COLOR, "vColor");

    glLinkProgram(gshaderProgramObject);

    GLint iShaderProgramLinkStatus = 0;
    glGetProgramiv(gshaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
    if (iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gshaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Shader program Link Log : %s \n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd_DM);
            }
        }
    }



  
    const GLfloat GraphVertices[] =
    {
        1.0f, 1.00f, 0.0f,
        -1.0f, 1.00f, 0.0f,
        1.0f, 0.95f, 0.0f,
        -1.0f, 0.95f, 0.0f,
        1.0f, 0.90f, 0.0f,
        -1.0f, 0.90f, 0.0f,
        1.0f, 0.85f, 0.0f,
        -1.0f, 0.85f, 0.0f,
        1.0f, 0.80f, 0.0f,
        -1.0f, 0.80f, 0.0f,
        1.0f, 0.75f, 0.0f,
        -1.0f, 0.75f, 0.0f,
        1.0f, 0.70f, 0.0f,
        -1.0f, 0.70f, 0.0f,
        1.0f, 0.65f, 0.0f,
        -1.0f, 0.65f, 0.0f,
        1.0f, 0.60f, 0.0f,
        -1.0f, 0.60f, 0.0f,
        1.0f, 0.55f, 0.0f,
        -1.0f, 0.55f, 0.0f,
        1.0f, 0.50f, 0.0f,
        -1.0f, 0.50f, 0.0f,
        1.0f, 0.45f, 0.0f,
        -1.0f, 0.45f, 0.0f,
        1.0f, 0.40f, 0.0f,
        -1.0f, 0.40f, 0.0f,
        1.0f, 0.35f, 0.0f,
        -1.0f, 0.35f, 0.0f,
        1.0f, 0.30f, 0.0f,
        -1.0f, 0.30f, 0.0f,
        1.0f, 0.25f, 0.0f,
        -1.0f, 0.25f, 0.0f,
        1.0f, 0.20f, 0.0f,
        -1.0f, 0.20f, 0.0f,
        1.0f, 0.15f, 0.0f,
        -1.0f, 0.15f, 0.0f,
        1.0f, 0.10f, 0.0f,
        -1.0f, 0.10f, 0.0f,
        1.0f, 0.05f, 0.0f,
        -1.0f, 0.05f, 0.0f,
        1.0f, -0.00f, 0.0f,
        -1.0f, -0.00f, 0.0f,
        1.0f, -0.05f, 0.0f,
        -1.0f, -0.05f, 0.0f,
        1.0f, -0.10f, 0.0f,
        -1.0f, -0.10f, 0.0f,
        1.0f, -0.15f, 0.0f,
        -1.0f, -0.15f, 0.0f,
        1.0f, -0.20f, 0.0f,
        -1.0f, -0.20f, 0.0f,
        1.0f, -0.25f, 0.0f,
        -1.0f, -0.25f, 0.0f,
        1.0f, -0.30f, 0.0f,
        -1.0f, -0.30f, 0.0f,
        1.0f, -0.35f, 0.0f,
        -1.0f, -0.35f, 0.0f,
        1.0f, -0.40f, 0.0f,
        -1.0f, -0.40f, 0.0f,
        1.0f, -0.45f, 0.0f,
        -1.0f, -0.45f, 0.0f,
        1.0f, -0.50f, 0.0f,
        -1.0f, -0.50f, 0.0f,
        1.0f, -0.55f, 0.0f,
        -1.0f, -0.55f, 0.0f,
        1.0f, -0.60f, 0.0f,
        -1.0f, -0.60f, 0.0f,
        1.0f, -0.65f, 0.0f,
        -1.0f, -0.65f, 0.0f,
        1.0f, -0.70f, 0.0f,
        -1.0f, -0.70f, 0.0f,
        1.0f, -0.75f, 0.0f,
        -1.0f, -0.75f, 0.0f,
        1.0f, -0.80f, 0.0f,
        -1.0f, -0.80f, 0.0f,
        1.0f, -0.85f, 0.0f,
        -1.0f, -0.85f, 0.0f,
        1.0f, -0.90f, 0.0f,
        -1.0f, -0.90f, 0.0f,
        1.0f, -0.95f, 0.0f,
        -1.0f, -0.95f, 0.0f,
        1.0f, -1.00f, 0.0f,
        -1.0f, -1.00f, 0.0f

    };

    GLfloat Graph_Color[82][3];
    for (int i = 0; i < 82; i++)
    {
        Graph_Color[i][0] = 0.0f;
        Graph_Color[i][1] = 0.0f;
        Graph_Color[i][2] = 1.0f;

    }
    

    glGenVertexArrays(1, &gVao);
    glBindVertexArray(gVao);

    glGenBuffers(1, &gVbo_Position);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GraphVertices), GraphVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &gVbo_Color);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graph_Color), Graph_Color, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*New for Circle */
    const GLfloat circleVertices[] =
    {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    const GLfloat CircleColor[] =
    {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &C_gVao);
    glBindVertexArray(C_gVao);

    glGenBuffers(1, &C_gVbo_Position);
    glBindBuffer(GL_ARRAY_BUFFER, C_gVbo_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &C_gVbo_Color);
    glBindBuffer(GL_ARRAY_BUFFER, C_gVbo_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CircleColor), CircleColor, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*New for traingle */
    const GLfloat triangleVertices[] =
    {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    const GLfloat triangleColor[] =
    {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &T_gVao);
    glBindVertexArray(T_gVao);

    glGenBuffers(1, &T_gVbo_Position);
    glBindBuffer(GL_ARRAY_BUFFER, T_gVbo_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &T_gVbo_Color);
    glBindBuffer(GL_ARRAY_BUFFER, T_gVbo_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gPerspectiveProjectMatix = mat4::identity();

}

void Resize(int width, int height)
{
    if (height <= 0)
    {
        height = 1;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    gPerspectiveProjectMatix = vmath::perspective(44.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    
}

void Display()
{
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gshaderProgramObject);

    mat4 modelViewMatrix = mat4::identity();
    mat4 TranslateMatrix = vmath::translate( 0.0f, 0.0f, -3.0f);
    mat4 RotateMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
    mat4 modelViewProjectionMatrix = mat4::identity();

    modelViewMatrix = TranslateMatrix ;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glBindVertexArray(gVao);

    for (int i = 0; i <= 80; i += 2)
    {
        glDrawArrays(GL_LINES, i, 2);
    }

    glBindVertexArray(0);

   modelViewMatrix = mat4::identity();
   TranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
   RotateMatrix = vmath::rotate(90.0f, 0.0f, 0.0f, 1.0f);
   modelViewProjectionMatrix = mat4::identity();

    modelViewMatrix = TranslateMatrix * RotateMatrix;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glBindVertexArray(gVao);

    for (int i = 0; i <= 80; i += 2)
    {
        glDrawArrays(GL_LINES, i, 2);
    }

    glBindVertexArray(0);

#pragma region TRIANGLE
    //modelViewMatrix = mat4::identity();
    //TranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    //modelViewProjectionMatrix = mat4::identity();

    //modelViewMatrix = TranslateMatrix ;
    //modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    //glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //
    //glBindVertexArray(T_gVao);

    //glDrawArrays(GL_TRIANGLES, 0, 9);
    //
    //glBindVertexArray(0);

#pragma endregion TRIANGLE



#pragma region CIRCLE
    //modelViewMatrix = mat4::identity();
    //TranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    //modelViewProjectionMatrix = mat4::identity();

    //modelViewMatrix = TranslateMatrix ;
    //modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    //glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //
    //glBindVertexArray(T_gVao);

    //glDrawArrays(GL_TRIANGLES, 0, 9);
    //
    //glBindVertexArray(0);

#pragma endregion TRIANGLE

    glUseProgram(0);

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

    if (gVao)
    {
        glDeleteVertexArrays(1, &gVao);
        gVao = 0;
    }

    if (gVbo_Position)
    {
        glDeleteBuffers(1, &gVbo_Position);
        gVbo_Position = 0;
    }

   
    if (gVao)
    {
        glDeleteVertexArrays(1, &gVao);
        gVao = 0;
    }

    if (gVbo_Position)
    {
        glDeleteBuffers(1, &gVbo_Position);
        gVbo_Position = 0;
    }

    if (gshaderProgramObject)
    {
        glUseProgram(gshaderProgramObject);
        GLsizei shaderCount;
        glGetProgramiv(gshaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);
        GLuint* pShader = NULL;
        pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
        if (pShader == NULL)
        {
            fprintf_s(gpFile_DM, "\nUnable to allocate memory to pShader.\n");
        }
        glGetAttachedShaders(gshaderProgramObject, shaderCount, &shaderCount, pShader);
        for (GLsizei i = 0; i < shaderCount; i++)
        {
            glDetachShader(gshaderProgramObject, pShader[i]);
        }
        free(pShader);

        glDeleteShader(gVertexShaderObject);
        gVertexShaderObject = 0;

        glDeleteShader(gFragmentShaderObject);
        gFragmentShaderObject = 0;

        glDeleteProgram(gshaderProgramObject);
        gshaderProgramObject = 0;

        glUseProgram(0);
    }

    if (S_gVao)
    {
        glDeleteVertexArrays(1, &S_gVao);
        S_gVao = 0;
    }

    if (S_gVbo_Position)
    {
        glDeleteBuffers(1,&S_gVbo_Position);
        S_gVbo_Position = 0;
    }
     if (T_gVao)
    {
        glDeleteVertexArrays(1, &T_gVao);
        T_gVao = 0;
    }

    if (T_gVbo_Position)
    {
        glDeleteBuffers(1, &T_gVbo_Position);
        T_gVbo_Position = 0;
    }

    if (C_gVao)
    {
        glDeleteVertexArrays(1, &C_gVao);
        C_gVao = 0;
    }

    if (C_gVbo_Position)
    {
        glDeleteBuffers(1, &C_gVbo_Position);
        C_gVbo_Position = 0;
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
        fprintf_s(gpFile_DM, "Log File is Successfully closed.\n");
        fclose(gpFile_DM);
        gpFile_DM = NULL;
    }

}


