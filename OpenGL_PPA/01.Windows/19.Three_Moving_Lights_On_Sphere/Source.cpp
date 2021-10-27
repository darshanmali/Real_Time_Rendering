#include<windows.h>
#include"Header.h"
#include"Sphere.h"

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
GLfloat L_angle_1 = 0.0f;
GLfloat L_angle_2 = 0.0f;
GLfloat L_angle_3 = 0.0f;

DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };


bool gdFullScreen_DM = false;
bool gbEscapekeyIsPressed_DM = false;
bool gbActiveWindows_DM = false;

GLuint gVertexShaderObject_PV;
GLuint gFragmentShaderObject_PV;
GLuint gshaderProgramObject_PV;

/* Fragment shader */
GLuint gshaderProgramObject_PF;
GLuint gVertexShaderObject_PF;
GLuint gFragmentShaderObject_PF;

//Local Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);

GLuint gVao_Sphere;
GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

/*New For Sphere*/
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint gNumVertices;
GLuint gNumElements;

/*splite 1.model view, 2.projection matrics */
GLuint gMMatrixUniform_DM;
GLuint gVMatrixUniform_DM;
GLuint gPMatrixUniform_DM;

GLuint LKeyPressedUniform_DM;/* key passing */

GLuint LA_Uniform_DM[3];/*Ambient*/
GLuint LD_Uniform_DM[3];/*Deffuse*/
GLuint LS_Uniform_DM[3];/*Specular*/
GLuint LPosition_Uniform_DM[3];/*Positon*/

GLuint KA_Uniform_DM;/*Material Ambient*/
GLuint KD_Uniform_DM;/*Material Deffuse*/
GLuint KS_Uniform_DM;/*Material Specular*/
GLuint shininess;

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 58.0f;

struct Light
{
    vec3 lAmbient;
    vec3 lDefues;
    vec3 lSpecular;
    vec4 lPosition;
};

struct Light Lights[3];

bool bAnimate = false;
bool bLights = false;

bool per_Vertex = true;
bool per_Fragment = false;

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
        TEXT("Darshan's OpenGLPP : Three Lights on Sphere."),
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
        GetClientRect(hwper_Vertexnd, &rc_DM);
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
            ToggelFullScreen();
            break;
        case 'Q':
        case 'q':
            DestroyWindow(hwnd);
            break;
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
        case 'V':
        case 'v':
            if (per_Vertex == false)
            {
                per_Vertex = true;
                per_Fragment = false;
            }
            else
            {
                per_Vertex = false;
            }
            break;
            break;
        case 0x46:
        case 0x66:
            if (per_Fragment == false)
            {
                per_Fragment = true;
                per_Vertex = false;
            }
            else
            {
                per_Fragment = false;
            }
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
    Lights[0].lPosition = { 0.0f, 0.0f, 0.0f, 1.0f};
    
    Lights[1].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[1].lDefues = { 0.0f, 1.0f, 0.0f };
    Lights[1].lSpecular = { 0.0f, 1.0f, 0.0f };
    Lights[1].lPosition = { -0.0f, 0.0f, 0.0f, 1.0f};
    
    Lights[2].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[2].lDefues = { 0.0f, 0.0f, 1.0f };
    Lights[2].lSpecular = { 0.0f, 0.0f, 1.0f };
    Lights[2].lPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
    
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
    gVertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode =
        "#version 450 core\n" \
        "in vec4 vPosition;\n" \
        "in vec3 vNormal;\n" \
        "uniform mat4 u_model_matrix;\n" \
        "uniform mat4 u_view_matrix;\n" \
        "uniform mat4 u_p_matrix;\n" \
        "uniform int u_lKey;\n" \
        "uniform vec3 u_la[3];\n" \
        "uniform vec3 u_ld[3];\n" \
        "uniform vec3 u_ls[3];\n" \
        "uniform vec4 u_light_position[3];\n" \
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
        "for( int i = 0; i < 3; i++){\n"
        "vec3 ambient[3];\n" \
        "vec3 diffuse[3];\n" \
        "vec3 specular[3];\n" \
        "vec3 light_direction[3];\n" \
        "vec3 reflection_vector[3];\n" \
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

    glShaderSource(gVertexShaderObject_PV, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


    glCompileShader(gVertexShaderObject_PV);
    /* Here there should be vertex shader complication error Cheking */

    GLint iInfoLogLength = 0;
    GLint iShaderCompileStatus = 0;
    char* szInfoLog = NULL;

    glGetShaderiv(gVertexShaderObject_PV, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(gVertexShaderObject_PV, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject_PV, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Vertex Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                unInitialize();
                exit(0);
            }
        }
    }

    /* Fragment Shader Object */
    gFragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* fragmentShaderSourceCode =
        "#version 450 core\n" \
        "in vec3 fong_ads_light;\n" \
        "out vec4 FragColor;\n" \
        "void main(void){\n" \
        "FragColor = vec4(fong_ads_light, 1.0f);\n"\
        "}";

    glShaderSource(gFragmentShaderObject_PV, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

    glCompileShader(gFragmentShaderObject_PV);
    /*Here there should be Fragment shader complication error Cheking */

    glGetShaderiv(gFragmentShaderObject_PV, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PV, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Fragment Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd_DM);
            }
        }
    }

    gshaderProgramObject_PV = glCreateProgram();

    glAttachShader(gshaderProgramObject_PV, gVertexShaderObject_PV);
    glAttachShader(gshaderProgramObject_PV, gFragmentShaderObject_PV);

    glBindAttribLocation(gshaderProgramObject_PV, DVM_ATTTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(gshaderProgramObject_PV, DVM_ATTTRIBUTE_NORMAL, "vNormal");
    glLinkProgram(gshaderProgramObject_PV);

    GLint iShaderProgramLinkStatus = 0;
    glGetProgramiv(gshaderProgramObject_PV, GL_LINK_STATUS, &iShaderProgramLinkStatus);
    if (iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gshaderProgramObject_PV, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PV, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Shader program Link Log : %s \n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd_DM);
            }
        }
    }

    gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_model_matrix");
    gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_view_matrix");
    gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_p_matrix");

    LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_lKey");

    LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_la[0]");
    LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_ld[0]");
    LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_ls[0]");
    LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_light_position[0]");
    
    LA_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_la[1]");
    LD_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_ld[1]");
    LS_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_ls[1]");
    LPosition_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_light_position[1]");

    
    LA_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_la[2]");
    LD_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_ld[2]");
    LS_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_ls[2]");
    LPosition_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_light_position[2]");
    
    KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_ka");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_kd");
    KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_ks");
    shininess = glGetUniformLocation(gshaderProgramObject_PV, "u_shininess");


    /*************************** Fragment  shader ****************************************************/

    /* Vertex Shader Object */
    gVertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode_PF =
        "#version 450 core\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_p_matrix;" \
        "out vec4 eye_cordinates;\n" \
        "out vec3 transformed_normal;\n"  \
        "void main(void){\n" \
        "eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" \
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
        "}";

    glShaderSource(gVertexShaderObject_PF, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);


    glCompileShader(gVertexShaderObject_PF);

    iInfoLogLength = 0;
    iShaderCompileStatus = 0;


    glGetShaderiv(gVertexShaderObject_PF, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(gVertexShaderObject_PF, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject_PF, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Vertex Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                unInitialize();
                exit(0);
            }
        }
    }

    /* Fragment Shader Object */
    gFragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* fragmentShaderSourceCode_PF =
        "#version 450 core\n" \
        "uniform int u_lKey;\n" \
        "uniform vec3 u_la[3];\n" \
        "uniform vec3 u_ld[3];\n" \
        "uniform vec3 u_ls[3];\n" \
        "uniform vec4 u_light_position[3];\n" \
        "uniform vec3 u_ka;\n" \
        "uniform vec3 u_kd;\n" \
        "uniform vec3 u_ks;\n" \
        "uniform float u_shininess;\n" \
        "in vec4 eye_cordinates;\n" \
        "in vec3 transformed_normal;\n"  \
        "out vec4 FragColor;\n" \
        "vec3 fong_ads_light;\n" \
        "void main(void){\n" \
        "if(u_lKey == 1){\n" \
        "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" \
        "for( int i = 0; i < 3; i++){\n"
        "vec3 ambient[3];\n" \
        "vec3 diffuse[3];\n" \
        "vec3 specular[3];\n" \
        "vec3 light_direction[3];\n" \
        "vec3 reflection_vector[3];\n" \
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" \
        "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);\n" \
        "ambient[i] = u_la[i] * u_ka;\n" \
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" \
        "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector), 0.0), u_shininess);\n" \
        "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" \
        "}\n" \
        "FragColor = vec4(fong_ads_light, 1.0f);\n"\
        "}\n" \
        "else{\n" \
        "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
        "}\n"
        "}";

    glShaderSource(gFragmentShaderObject_PF, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);

    glCompileShader(gFragmentShaderObject_PF);
    /*Here there should be Fragment shader complication error Cheking */

    glGetShaderiv(gFragmentShaderObject_PF, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PF, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Fragment Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd_DM);
            }
        }
    }

    gshaderProgramObject_PF = glCreateProgram();

    glAttachShader(gshaderProgramObject_PF, gVertexShaderObject_PF);
    glAttachShader(gshaderProgramObject_PF, gFragmentShaderObject_PF);

    glBindAttribLocation(gshaderProgramObject_PF, DVM_ATTTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(gshaderProgramObject_PF, DVM_ATTTRIBUTE_NORMAL, "vNormal");

    glLinkProgram(gshaderProgramObject_PF);

    iShaderProgramLinkStatus = 0;
    glGetProgramiv(gshaderProgramObject_PF, GL_LINK_STATUS, &iShaderProgramLinkStatus);
    if (iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gshaderProgramObject_PF, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PF, iInfoLogLength, &written, szInfoLog);
                fprintf_s(gpFile_DM, "Shader program Link Log : %s \n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd_DM);
            }
        }
    }

    gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_model_matrix");
    gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_view_matrix");
    gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_p_matrix");

    LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_lKey");

    LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_la[0]");
    LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_ld[0]");
    LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_ls[0]");
    LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position[0]");

    LA_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_la[1]");
    LD_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_ld[1]");
    LS_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_ls[1]");
    LPosition_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position[1]");

    LA_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_la[2]");
    LD_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_ld[2]");
    LS_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_ls[2]");
    LPosition_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position[2]");

    KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ka");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_kd");
    KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ks");
    shininess = glGetUniformLocation(gshaderProgramObject_PF, "u_shininess");

    /*sphere */
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    gNumVertices = getNumberOfSphereVertices();
    gNumElements = getNumberOfSphereElements();

    glGenVertexArrays(1, &gVao_Sphere);
    glBindVertexArray(gVao_Sphere);

    /*For Vertex */
    glGenBuffers(1, &gVbo_sphere_position);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*For Normals */
    glGenBuffers(1, &gVbo_sphere_normal);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*For elements */
    glGenBuffers(1, &gVbo_sphere_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
    void update(void);
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if (per_Fragment)
    {
        glUseProgram(gshaderProgramObject_PF);
    }
    else if (per_Vertex)
    {
        glUseProgram(gshaderProgramObject_PV);
    }
    else {
        glUseProgram(gshaderProgramObject_PV);
    }

    if (bLights)
    {
        glUniform1i(LKeyPressedUniform_DM, 1);
        glUniform1f(shininess, Shininess);

        Lights[0].lPosition[0] = 0.0f;
        Lights[0].lPosition[1] = 1.5f * sinf(L_angle_1);
        Lights[0].lPosition[2] = 2.5f * cosf(L_angle_1);
        Lights[0].lPosition[3] = 1.0f;
 
        glUniform3fv(LA_Uniform_DM[0], 1, Lights[0].lAmbient);
        glUniform3fv(LD_Uniform_DM[0], 1, Lights[0].lDefues);
        glUniform3fv(LS_Uniform_DM[0], 1, Lights[0].lSpecular);
        glUniform4fv(LPosition_Uniform_DM[0], 1, Lights[0].lPosition);
 
        Lights[1].lPosition[0] = 2.0f * sinf(L_angle_1);
        Lights[1].lPosition[1] = 0.0f;
        Lights[1].lPosition[2] = 2.0f * cosf(L_angle_1);
        Lights[1].lPosition[3] = 1.0f;

        glUniform3fv(LA_Uniform_DM[1], 1, Lights[1].lAmbient);
        glUniform3fv(LD_Uniform_DM[1], 1, Lights[1].lDefues);
        glUniform3fv(LS_Uniform_DM[1], 1, Lights[1].lSpecular);
        glUniform4fv(LPosition_Uniform_DM[1], 1, Lights[1].lPosition);
 
        Lights[2].lPosition[0] = 2.0f * sinf(L_angle_1);
        Lights[2].lPosition[1] = 2.0f * cosf(L_angle_1);
        Lights[2].lPosition[2] = 0.0f;
        Lights[2].lPosition[3] = 1.0f;

        glUniform3fv(LA_Uniform_DM[2], 1, Lights[2].lAmbient);
        glUniform3fv(LD_Uniform_DM[2], 1, Lights[2].lDefues);
        glUniform3fv(LS_Uniform_DM[2], 1, Lights[2].lSpecular);
        glUniform4fv(LPosition_Uniform_DM[2], 1, Lights[2].lPosition);
        
        glUniform3fv(KA_Uniform_DM, 1, (GLfloat*)material_Ambient);
        glUniform3fv(KD_Uniform_DM, 1, (GLfloat*)material_Diffuse);
        glUniform3fv(KS_Uniform_DM, 1, (GLfloat*)material_Specular);

        update();

    }
    else
    {
        glUniform1i(LKeyPressedUniform_DM, 0);
    }
 
    mat4 ModelMatrix;
    mat4 ViewMatrix;
    mat4 TranslateMatrix;
    mat4 PMatrix;
 
    ModelMatrix = mat4::identity();
    ViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    PMatrix = mat4::identity();
 
    TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
    ModelMatrix = TranslateMatrix;
 
    PMatrix = gPerspectiveProjectMatix;
 
    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
    glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);
 
    glBindVertexArray(gVao_Sphere);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
 
    glBindVertexArray(0);
 
    glUseProgram(0);
 
    SwapBuffers(ghdc_DM);
}

void update(void)
{
    L_angle_1 += 0.005f;
    if (L_angle_1 > 360.0f)
    {
        L_angle_1 = 0.0f;
    }
    L_angle_2 += 0.005f;
    if (L_angle_2 > 360.0f)
    {
        L_angle_2 = 0.0f;
    }
    L_angle_3 += 0.005f;
    if (L_angle_3 > 360.0f)
    {
        L_angle_3 = 0.0f;
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

    if (gVao_Sphere)
    {
        glDeleteVertexArrays(1, &gVao_Sphere);
        gVao_Sphere = 0;
    }

    if (gVbo_sphere_position)
    {
        glDeleteBuffers(1, &gVbo_sphere_position);
        gVbo_sphere_position = 0;
    }

    if (gVbo_sphere_normal)
    {
        glDeleteBuffers(1, &gVbo_sphere_normal);
        gVbo_sphere_normal = 0;
    }

    if (gshaderProgramObject_PV)
    {
        glUseProgram(gshaderProgramObject_PV);
        GLsizei shaderCount;
        glGetProgramiv(gshaderProgramObject_PV, GL_ATTACHED_SHADERS, &shaderCount);
        GLuint* pShader = NULL;
        pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
        if (pShader == NULL)
        {
            fprintf_s(gpFile_DM, "\nUnable to allocate memory to pShader.\n");
        }
        glGetAttachedShaders(gshaderProgramObject_PV, shaderCount, &shaderCount, pShader);
        for (GLsizei i = 0; i < shaderCount; i++)
        {
            glDetachShader(gshaderProgramObject_PV, pShader[i]);
        }
        free(pShader);

        glDeleteShader(gVertexShaderObject_PV);
        gVertexShaderObject_PV = 0;

        glDeleteShader(gFragmentShaderObject_PV);
        gFragmentShaderObject_PV = 0;

        glDeleteProgram(gshaderProgramObject_PV);
        gshaderProgramObject_PV = 0;

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
