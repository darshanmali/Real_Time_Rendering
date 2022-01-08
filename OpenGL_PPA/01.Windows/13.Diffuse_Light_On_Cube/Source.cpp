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
    DVM_ATTTRIBUTE_NORMAL,
    DVM_ATTTRIBUTE_COLOR,
    DVM_ATTTRIBUTE_TEXCORD
};

//Global Fuction 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Global Variables Declaration 
FILE* gpFile_DM = NULL;

HWND ghwnd_DM = NULL;
HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;
GLfloat T_angle = 0.0f;
GLfloat R_angle = 0.0f;

DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };


bool gdFullScreen_DM = false;
bool gbEscapekeyIsPressed_DM = false;
bool gbActiveWindows_DM = false;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gshaderProgramObject;

//Local Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);

GLuint gVao_Cube;
GLuint gVbo_Cube_Position;
GLuint gVbo_Cube_Normals;

/*splite 1.model view, 2.projection matrics */
GLuint gMVMatrixUniform_DM;
GLuint gPMatrixUniform_DM;

GLuint LKeyPressedUniform_DM;/* key passing */
GLuint LD_Uniform_DM;/*Deffuse*/
GLuint KD_Uniform_DM;/*Material*/
GLuint LPosition_Uniform_DM;/*Positon*/

bool bAnimate = false;
bool bLights = false;

mat4 gPerspectiveProjectMatix;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpszCmdLine, int iCmdShow)
{
    //Function 

    void Initialize(void);

    WNDCLASSEX wndclassex;
    HWND hwnd;
    MSG msg;
    TCHAR Appname[] = TEXT("OpenGLPP_LIGHT");
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
        TEXT("Darshan's OpenGLPP : Cube Rotation With Lights"),
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
        case 'A':
        case 'a':
            if (bAnimate == true)
            {
                bAnimate = false;
            }
            else
            {
                bAnimate = true;
            }
            break;
        case 'L':
        case 'l':
            if (bLights == true)
            {
                bLights = false;
            }
            else
            {
                bLights = true;
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
        "in vec3 vNormal;" \
        "uniform mat4 u_model_view_matrix;" \
        "uniform mat4 u_p_matrix;" \
        "uniform int u_lKey;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_kd;" \
        "uniform vec4 u_light_position;" \
        "out vec3 diffuse_light;" \
        "void main(void){\n" \
        "if(u_lKey == 1){\n" \
        "vec4 eye_cordinates = vec4(u_model_view_matrix * vPosition);\n" \
        "mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));\n" \
        "vec3 tnormal = normalize(normal_matrix * vNormal);\n" \
        "vec3 s = normalize(vec3(u_light_position - eye_cordinates));\n" \
        "diffuse_light = u_ld * u_kd * max( dot(s , tnormal), 0.0);\n" \
        "}\n" \
        "gl_Position = u_p_matrix * u_model_view_matrix * vPosition;\n" \
        "}";

    glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


    glCompileShader(gVertexShaderObject);

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
        "in vec3 diffuse_light;\n" \
        "uniform int u_lKey;\n" \
        "out vec4 FragColor;\n" \
        "void main(void){\n" \
        "vec4 color;\n"\
        "if(u_lKey == 1){\n" \
        "color = vec4( diffuse_light, 1.0f);\n" \
        "}\n"\
        "else{\n"\
        "color = vec4( 1.0f, 1.0f, 1.0f, 1.0f);\n" \
        "}\n" \
        "FragColor = color;\n"\
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
    glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_NORMAL, "vNormal");
    
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

    gMVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_model_view_matrix");
    gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_p_matrix");
    LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_lKey");
    LD_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ld");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_kd");
    LPosition_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_light_position");

    /*New for Cube */
    const GLfloat CubeVertices[] =
    {
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f

    };

    const GLfloat CubeNormals[] =
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };

    /*Cube Begen*/
    glGenVertexArrays(1, &gVao_Cube);
    glBindVertexArray(gVao_Cube);

    /*Position */
    glGenBuffers(1, &gVbo_Cube_Position);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Cube_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &gVbo_Cube_Normals);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Cube_Normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeNormals), CubeNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    gPerspectiveProjectMatix = mat4::identity();

    Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM);

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
    void Update(void);
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gshaderProgramObject);

    if (bLights)
    {
        glUniform1i(LKeyPressedUniform_DM, 1);
        glUniform3f(LD_Uniform_DM, 1.0f, 1.0f, 1.0f);
        glUniform3f(KD_Uniform_DM, 0.5f, 0.5f, 0.5f);
        GLfloat light_position[] = { 0.0f, 0.0f, 2.0f, 1.0f };
        glUniform4fv(LPosition_Uniform_DM, 1, (GLfloat *)light_position);
    }
    else
    {
        glUniform1i(LKeyPressedUniform_DM, 0);
    }

    mat4 ModelViewMatrix;
    mat4 TranslateMatrix;
    mat4 RotationMatrtix;
    mat4 PMatrix;

    ModelViewMatrix = mat4::identity();
    RotationMatrtix = mat4::identity();
    TranslateMatrix = mat4::identity();
    PMatrix = mat4::identity();
    
    TranslateMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
    RotationMatrtix = vmath::rotate(R_angle, 0.0f, 1.0f, 0.0f);
    ModelViewMatrix = TranslateMatrix * RotationMatrtix;
    
    PMatrix = gPerspectiveProjectMatix;

    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
    glUniformMatrix4fv(gMVMatrixUniform_DM, 1, GL_FALSE, ModelViewMatrix);

    glBindVertexArray(gVao_Cube);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    glBindVertexArray(0);

    glUseProgram(0);

    if (bAnimate)
    {
        Update();
    }

    SwapBuffers(ghdc_DM);

}
void Update()
{
    if (R_angle >= 360.0f)
    {
        R_angle = 0.0f;
    }
    R_angle = R_angle + 0.1f;

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

    if (gVao_Cube)
    {
        glDeleteVertexArrays(1, &gVao_Cube);
        gVao_Cube = 0;
    }

    if (gVbo_Cube_Position)
    {
        glDeleteBuffers(1, &gVbo_Cube_Position);
        gVbo_Cube_Position = 0;
    }

    if (gVbo_Cube_Normals)
    {
        glDeleteBuffers(1, &gVbo_Cube_Normals);
        gVbo_Cube_Normals = 0;
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
        for (GLuint i = 0; i < (GLuint)shaderCount; i++)
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


