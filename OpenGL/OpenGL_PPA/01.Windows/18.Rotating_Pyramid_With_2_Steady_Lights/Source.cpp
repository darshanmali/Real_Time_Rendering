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

GLuint gVao_Pyramid;
GLuint gVbo_Pyramid_Position;
GLuint gVbo_Pyramid_Normal;

/*splite 1.model view, 2.projection matrics */
GLuint gMMatrixUniform_DM;
GLuint gVMatrixUniform_DM;
GLuint gPMatrixUniform_DM;

GLuint LKeyPressedUniform_DM;/* key passing */

GLuint LA_Uniform_DM[2];/*Ambient*/
GLuint LD_Uniform_DM[2];/*Deffuse*/
GLuint LS_Uniform_DM[2];/*Specular*/
GLuint LPosition_Uniform_DM[2];/*Positon*/

GLuint KA_Uniform_DM;/*Material Ambient*/
GLuint KD_Uniform_DM;/*Material Deffuse*/
GLuint KS_Uniform_DM;/*Material Specular*/
GLuint shininess;

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f };
GLfloat lDefues[] = { 1.0f, 1.0f, 1.0f };
GLfloat lSpecular[] = { 1.0f, 1.0f, 1.0f };
GLfloat lPosition[] = { 0.0f, 10.0f, 20.0f, 1.0f };

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 128.0f;

struct Light
{
    vec3 lAmbient;
    vec3 lDefues;
    vec3 lSpecular;
    vec4 lPosition;
};

struct Light Lights[2];

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
        TEXT("Darshan's OpenGLPP : Two Lights on Pyramid."),
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

    /*Lights properties initialization */
    Lights[0].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[0].lDefues = { 1.0f, 0.0f, 0.0f };
    Lights[0].lSpecular = { 1.0f, 0.0f, 0.0f };
    Lights[0].lPosition = { 2.0f, 0.0f, 0.0f, 1.0f};
    
    Lights[1].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[1].lDefues = { 0.0f, 0.0f, 1.0f };
    Lights[1].lSpecular = { 0.0f, 0.0f, 1.0f };
    Lights[1].lPosition = { -2.0f, 0.0f, 0.0f, 1.0f};


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
        "in vec4 vPosition\n;" \
        "in vec3 vNormal\n;" \
        "uniform mat4 u_model_matrix;\n" \
        "uniform mat4 u_view_matrix;\n" \
        "uniform mat4 u_p_matrix;\n" \
        "uniform int u_lKey;\n" \
        "uniform vec3 u_la[2];\n" \
        "uniform vec3 u_ld[2];\n" \
        "uniform vec3 u_ls[2];\n" \
        "uniform vec4 u_light_position[2];\n" \
        "uniform vec3 u_ka;\n" \
        "uniform vec3 u_kd;\n" \
        "uniform vec3 u_ks;\n" \
        "uniform float u_shininess;\n" \
        "out vec3 fong_ads_light;\n" \
        "void main(void){\n" \
        "if(u_lKey == 1){\n" \
        "vec4 eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" \
        "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" \
        "for( int i = 0; i < 2; i++){\n"
        "vec3 ambient[2];\n" \
        "vec3 diffuse[2];\n" \
        "vec3 specular[2];\n" \
        "vec3 light_direction[2];\n" \
        "vec3 reflection_vector[2];\n" \
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" \
        "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);\n" \
        "ambient[i] = u_la[i] * u_ka;\n" \
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" \
        "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector), 0.0), u_shininess);\n" \
        "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" \
        "}\n" \
        "}\n" \
        "else{\n" \
        "fong_ads_light = vec3(1.0f, 1.0f, 1.0f);" \
        "}\n" \
        "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
        "}";

    glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


    glCompileShader(gVertexShaderObject);
    /* Here there should be vertex shader complication error Cheking */

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
        "#version 450 core\n" \
        "in vec3 fong_ads_light;\n" \
        "out vec4 FragColor;\n" \
        "void main(void){\n" \
        "FragColor = vec4(fong_ads_light, 1.0f);\n"\
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

    gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_model_matrix");
    gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_view_matrix");
    gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_p_matrix");

    LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_lKey");

    LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_la[0]");
    LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_ld[0]");
    LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_ls[0]");
    LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_light_position[0]");
    
    LA_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_la[1]");
    LD_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_ld[1]");
    LS_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_ls[1]");
    LPosition_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_light_position[1]");

    KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ka");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_kd");
    KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ks");
    shininess = glGetUniformLocation(gshaderProgramObject, "u_shininess");

    /*New for Cube*/
    const GLfloat PyramidVertices[] =
    {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f
    };

    const GLfloat PyramidNormals[] =
    {
        0.0f,0.447214f,0.894427f,
        0.0f,0.447214f,0.894427f,
        0.0f,0.447214f,0.894427f,

        0.894427f, 0.447214f, 0.0f,
        0.894427f, 0.447214f, 0.0f,
        0.894427f, 0.447214f, 0.0f,

        0.0f, 0.447214f, -0.894427f,
        0.0f, 0.447214f, -0.894427f,
        0.0f, 0.447214f, -0.894427f,

        -0.894427f, 0.447214f, 0.0f,
        -0.894427f, 0.447214f, 0.0f,
        -0.894427f, 0.447214f, 0.0f
    };

    /*Pyramid Begen*/
    glGenVertexArrays(1, &gVao_Pyramid);
    glBindVertexArray(gVao_Pyramid);

    /*Position */
    glGenBuffers(1, &gVbo_Pyramid_Position);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Pyramid_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidVertices), PyramidVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &gVbo_Pyramid_Normal);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Pyramid_Normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidNormals), PyramidNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
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
    void Update(void);
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gshaderProgramObject);

    if (bLights)
    {
        glUniform1i(LKeyPressedUniform_DM, 1);
        glUniform1f(shininess, Shininess);
        glUniform3fv(LA_Uniform_DM[0], 1, Lights[0].lAmbient);
        glUniform3fv(LD_Uniform_DM[0], 1, Lights[0].lDefues);
        glUniform3fv(LS_Uniform_DM[0], 1, Lights[0].lSpecular);
        glUniform4fv(LPosition_Uniform_DM[0], 1, Lights[0].lPosition);
        
        glUniform3fv(LA_Uniform_DM[1], 1, Lights[1].lAmbient);
        glUniform3fv(LD_Uniform_DM[1], 1, Lights[1].lDefues);
        glUniform3fv(LS_Uniform_DM[1], 1, Lights[1].lSpecular);
        glUniform4fv(LPosition_Uniform_DM[1], 1, Lights[1].lPosition);

        glUniform3fv(KA_Uniform_DM, 1, (GLfloat*)material_Ambient);
        glUniform3fv(KD_Uniform_DM, 1, (GLfloat*)material_Diffuse);
        glUniform3fv(KS_Uniform_DM, 1, (GLfloat*)material_Specular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform_DM, 0);
    }

    mat4 ModelMatrix;
    mat4 ViewMatrix;
    mat4 TranslateMatrix;
    mat4 RotationMatrtix;
    mat4 PMatrix;

    ModelMatrix = mat4::identity();
    ViewMatrix = mat4::identity();
    RotationMatrtix = mat4::identity();
    TranslateMatrix = mat4::identity();
    PMatrix = mat4::identity();

    TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
    RotationMatrtix = vmath::rotate(T_angle, 0.0f, 1.0f, 0.0f);
    ModelMatrix = TranslateMatrix * RotationMatrtix;

    PMatrix = gPerspectiveProjectMatix;

    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
    glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

    glBindVertexArray(gVao_Pyramid);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);
    glDrawArrays(GL_TRIANGLES, 6, 3);
    glDrawArrays(GL_TRIANGLES, 9, 3);

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
    if (T_angle >= 360.0f)
    {
        T_angle = 0.0f;
    }
    T_angle = T_angle + 0.1f;

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

    if (gVao_Pyramid)
    {
        glDeleteVertexArrays(1, &gVao_Pyramid);
        gVao_Pyramid = 0;
    }

    if (gVbo_Pyramid_Position)
    {
        glDeleteBuffers(1, &gVbo_Pyramid_Position);
        gVbo_Pyramid_Position = 0;
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
