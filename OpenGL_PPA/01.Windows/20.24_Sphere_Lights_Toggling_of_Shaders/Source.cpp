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

RECT rc_DM;

GLfloat L_angle_1 = 0.0f;
GLfloat L_angle_2 = 0.0f;
GLfloat L_angle_3 = 0.0f;

GLuint count = 0;

DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };


bool gdFullScreen_DM = false;
bool gbEscapekeyIsPressed_DM = false;
bool gbActiveWindows_DM = false;

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

GLuint gVao_Lines;
GLuint gVbo_Lines_position;
GLuint gVbo_Lines_Color;


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

GLuint LA_Uniform_DM;/*Ambient*/
GLuint LD_Uniform_DM;/*Deffuse*/
GLuint LS_Uniform_DM;/*Specular*/
GLuint LPosition_Uniform_DM;/*Positon*/

GLuint KA_Uniform_DM;/*Material Ambient*/
GLuint KD_Uniform_DM;/*Material Deffuse*/
GLuint KS_Uniform_DM;/*Material Specular*/
GLuint shininess;


GLfloat material_Ambient[3][24] = { 

 0.0215f, 0.1745f, 0.0215f,
0.135f, 0.2225f, 0.1575f,

0.05375f, 0.05f, 0.06625f,
0.25f, 0.20725f, 0.20725f,

0.1745f, 0.01175f, 0.01175f,
1.0f, 0.18725f, 0.1745f,

0.329412f, 0.223529f, 0.027451f,
0.2125f, 0.1275f, 0.054f,

0.25f, 0.25f, 0.25f,
0.19125f, 0.0735f, 0.0225f,

0.24725f, 0.1995f, 0.0745f,
0.19225f, 0.19225f, 0.19225f,

//1st - 3rd
0.0f, 0.0f, 0.0f,
0.0f,0.1f,0.06f,

0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f,

0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f,

//1st - 4rd
0.02f, 0.02f, 0.02f,
0.0f, 0.05f, 0.05f,

0.0f, 0.05f, 0.0f,
0.05f, 0.0f, 0.0f,

0.05f, 0.05f, 0.05f,
0.05f, 0.05f, 0.0f,

};

GLfloat material_Diffuse[3][24] = { 
    0.07568f, 0.61424f, 0.07568f,
    0.54f, 0.89f, 0.63f, 
0.18275f, 0.17f, 0.22525f,
1.0f, 0.829f, 0.829f,

0.61424f, 0.04136f, 0.04136f,
0.396f, 0.74151f, 0.69102f, 

0.780392f, 0.568627f, 0.113725f,
0.714f, 0.4284f, 0.18144f,

0.4f, 0.4f, 0.4f,
0.7038f, 0.27048f, 0.0828f,

0.75164f, 0.60648f, 0.22648f,
0.50754f, 0.50754f,0.50754f,

//1st - 3rd
0.01f, 0.01f, 0.01f,
0.0f, 0.50980392f, 0.50980392f,

0.1f, 0.35f, 0.1f,
0.5f, 0.0f, 0.0f,

0.55f, 0.55f, 0.55f,
0.5f, 0.5f, 0.0f,

//1st - 4rd
0.01f, 0.01f, 0.01f,
0.4f, 0.5f, 0.5f,

0.4f, 0.5f, 0.4f,
0.5f, 0.4f, 0.4f,

0.5f, 0.5f, 0.5f,
0.5f, 0.5f, 0.4f,

};

GLfloat material_Specular[3][24] = {
    
 0.633f, 0.727811f, 0.633f,
 0.316228f, 0.316228f, 0.316228f,

0.332741f, 0.328634f, 0.346435f,
0.296648f, 0.296648f, 0.296648f,

0.727811f, 0.626959f, 0.626959f,
0.297254f, 0.30829f, 0.306678f,

0.992157f, 0.941176f, 0.807843f,
0.393548f, 0.271906f, 0.166721f,

0.774597f, 0.774597f, 0.774597f,
0.256777f, 0.137622f, 0.086014f,

0.628281f, 0.555802f, 0.366065f,
0.508273f, 0.508273f, 0.508273f,

//1st - 3rd
0.50f, 0.50f, 0.50f,
0.50196078f, 0.50196078f, 0.50196078f,

0.45f, 0.55f, 0.45f,
0.7f, 0.6f, 0.6f,

0.70f, 0.70f, 0.70f,
0.60f, 0.60f, 0.50f,

//1st - 4rd
0.4f, 0.4f, 0.4f,
0.04f, 0.7f, 0.7f,

0.04f, 0.7f, 0.04f,
0.7f, 0.04f, 0.04f,

0.7f, 0.7f, 0.7f,
0.7f, 0.7f, 0.04f

};

GLfloat Shininess[24] = { 
    0.6f * 128.0f,
    0.1f * 128.0f,
 
    0.3f * 128.0f,
    0.088f * 128.0f,
    
    0.6f * 128.0f,
    0.1f * 128.0f,
    
    0.21794872f *128.0f,
    0.2f * 128.0f,
    
    0.6f * 128.0f,
    0.1f * 128.0f,

    0.4f * 128.0f,
    0.4f * 128.0f,

    //1st - 3rd
    0.25f * 128.0f,
    0.25f * 128.0f,

    0.25f * 128.0f,
    0.25f * 128.0f,
    
    0.25f * 128.0f,
    0.25f * 128.0f,

    //1st - 4rd
    0.078125f * 128.0f,
    0.078125f * 128.0f,

    0.078125f * 128.0f,
    0.078125f * 128.0f,
    
    0.078125f * 128.0f,
    0.078125f * 128.0f,

};

struct Light
{
    vec3 lAmbient;
    vec3 lDefues;
    vec3 lSpecular;
    vec4 lPosition;
};

struct Light Lights;

bool bAnimate = false;
bool bLights = false;
bool X_Rotation = true;
bool Y_Rotation = false;
bool Z_Rotation = false;

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
        TEXT("Darshan's OpenGLPP : 24 Sphere with Material."),
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
        case 'X':
        case'x':
            X_Rotation = true;
            Y_Rotation = false;
            Z_Rotation = false;
            break;
        case 'Y':
        case'y':
            X_Rotation = false;
            Y_Rotation = true;
            Z_Rotation = false;
            break;
        case 'Z':
        case'z':
            X_Rotation = false;
            Y_Rotation = false;
            Z_Rotation = true;
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
    Lights.lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights.lDefues = { 1.0f, 1.0f, 1.0f };
    Lights.lSpecular = { 1.0f, 1.0f, 1.0f };
    Lights.lPosition = { -1.0f, 0.0f, 0.0f, 0.0f};
    
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
        "eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" \
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
        "}";

    glShaderSource(gVertexShaderObject_PF, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);


    glCompileShader(gVertexShaderObject_PF);

    GLint iInfoLogLength = 0;
    GLint iShaderCompileStatus = 0;
    char* szInfoLog = NULL;

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
        "uniform vec3 u_la;\n" \
        "uniform vec3 u_ld;\n" \
        "uniform vec3 u_ls;\n" \
        "uniform vec4 u_light_position;\n" \
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
        "vec3 ambient;\n" \
        "vec3 diffuse;\n" \
        "vec3 specular;\n" \
        "vec3 light_direction;\n" \
        "vec3 reflection_vector;\n" \
        "light_direction = normalize(vec3(u_light_position));\n" \
        "reflection_vector = reflect(-light_direction, transformed_normal);\n" \
        "ambient = u_la * u_ka;\n" \
        "diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);\n" \
        "specular = u_ls * u_ks * pow( max(dot(reflection_vector, view_vector), 0.0), u_shininess);\n" \
        "fong_ads_light = ambient + diffuse + specular;\n" \
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

    int iShaderProgramLinkStatus = 0;
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

    LA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_la");
    LD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ld");
    LS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ls");
    LPosition_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position");

    KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ka");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_kd");
    KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ks");
    shininess = glGetUniformLocation(gshaderProgramObject_PF, "u_shininess");

    /*sphere */
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    gNumVertices = getNumberOfSphereVertices();
    gNumElements = getNumberOfSphereElements();

    /*Lines */

    GLfloat Lines_vertices[] = {
        -0.8f, 0.8f, 0.0f,
        -0.8f, 0.8f, 0.0f,

        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
    
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f

    };
        
    GLfloat Lines_Color[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &gVao_Lines);
    glBindVertexArray(gVao_Lines);

    /*For Vertex */
    glGenBuffers(1, &gVbo_Lines_position);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Lines_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Lines_vertices), Lines_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*For Color */
    glGenBuffers(1, &gVbo_Lines_Color);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Lines_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Lines_Color), Lines_Color, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

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

    //glViewport(10, 10 +((rc_DM.bottom / 6) * 5), rc_DM.right / 4, rc_DM.bottom / 6);

   
    gPerspectiveProjectMatix = vmath::perspective(44.0f,  (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    
}

void Display()
{
    void update(void);
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gshaderProgramObject_PF);

    mat4 ModelMatrix;
    mat4 ViewMatrix;
    mat4 TranslateMatrix;
    mat4 ScaleMatrix;
    mat4 PMatrix;
    
    ModelMatrix = mat4::identity();
    ViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    ScaleMatrix = mat4::identity();
    PMatrix = mat4::identity();
    GLfloat y = 0.0f,
            x = 0.0f;

    PMatrix = gPerspectiveProjectMatix;

    TranslateMatrix = vmath::translate(x, y, -1.0f);
    ModelMatrix = TranslateMatrix * ScaleMatrix;

    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);
    glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);

    if (bLights)
    {
        glUniform1i(LKeyPressedUniform_DM, 1);
        

        if (X_Rotation)
        {
            Lights.lPosition[0] = 0.0f;
            Lights.lPosition[1] = 0.1f * sinf(L_angle_1);
            Lights.lPosition[2] = 0.1f * cosf(L_angle_1);
            Lights.lPosition[3] = 0.0f;
        }
        else if (Y_Rotation)
        {
            Lights.lPosition[0] = 0.25f * sinf(L_angle_1); 
            Lights.lPosition[1] = 0.0f;
            Lights.lPosition[2] = 0.25f * cosf(L_angle_1);
            Lights.lPosition[3] = 0.25f;
        }
        else if (Z_Rotation)
        {
            Lights.lPosition[0] = 0.25f * sinf(L_angle_1); 
            Lights.lPosition[1] = 0.25f * cosf(L_angle_1);
            Lights.lPosition[2] = 0.0f;
            Lights.lPosition[3] = 0.25f;
        }

        glUniform3fv(LA_Uniform_DM, 1, Lights.lAmbient);
        glUniform3fv(LD_Uniform_DM, 1, Lights.lDefues);
        glUniform3fv(LS_Uniform_DM, 1, Lights.lSpecular);
        glUniform4fv(LPosition_Uniform_DM, 1, Lights.lPosition);
 
        update();
    }
    else
    {
        glUniform1i(LKeyPressedUniform_DM, 0);
    }
   
   
   //0
    glViewport(0, 0, (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);
   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate( x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
   
   glBindVertexArray(0);
   count++;

   //1
   glViewport(0, (GLsizei)rc_DM.bottom / 6, (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);
   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //2
   glViewport(0, (GLsizei)rc_DM.bottom / 6 + (GLsizei)rc_DM.bottom / 6, (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);
   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //3
   glViewport(0, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);
   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //4
   glViewport(0, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);
   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //5
   glViewport(0, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //6
   glViewport((GLsizei)rc_DM.right / 4, 0, (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;
   

   //7
   glViewport((GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //8
   glViewport((GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;
   
   
   //9
   glViewport((GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //10
   glViewport((GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //11
   glViewport((GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //12
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, 0, (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //13
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //14
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;
   

   //15
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //16
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //17
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //18
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, 0, (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);
   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //19
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //20
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //21
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //22
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //23
   glViewport((GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4 + (GLsizei)rc_DM.right / 4, ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6) + ((GLsizei)rc_DM.bottom / 6), (GLsizei)rc_DM.right / 6, (GLsizei)rc_DM.bottom / 6);

   glUniform1f(shininess, Shininess[0]);

   glUniform3f(KA_Uniform_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   count = 0;

   //y += 0.4f;
   if (y > 1.0f)
   {
       x -= 0.8f;
       y = -1.0f;
   }
   

    glBindVertexArray(gVao_Lines);

    glDrawArrays(GL_LINES, 0, 2);

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

    if (gVao_Lines)
    {
        glDeleteVertexArrays(1, &gVao_Lines);
        gVao_Lines = 0;
    }

    if (gVbo_Lines_position)
    {
        glDeleteBuffers(1, &gVbo_Lines_position);
        gVbo_Lines_position = 0;
    }

    if (gVbo_Lines_Color)
    {
        glDeleteBuffers(1, &gVbo_Lines_Color);
        gVbo_Lines_Color = 0;
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

    if (gshaderProgramObject_PF)
    {
        glUseProgram(gshaderProgramObject_PF);
        GLsizei shaderCount;
        glGetProgramiv(gshaderProgramObject_PF, GL_ATTACHED_SHADERS, &shaderCount);
        GLuint* pShader = NULL;
        pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
        if (pShader == NULL)
        {
            fprintf_s(gpFile_DM, "\nUnable to allocate memory to pShader.\n");
        }
        glGetAttachedShaders(gshaderProgramObject_PF, shaderCount, &shaderCount, pShader);
        for (GLsizei i = 0; i < shaderCount; i++)
        {
            glDetachShader(gshaderProgramObject_PF, pShader[i]);
        }
        free(pShader);

        glDeleteShader(gshaderProgramObject_PF);
        gVertexShaderObject_PF = 0;

        glDeleteShader(gFragmentShaderObject_PF);
        gFragmentShaderObject_PF = 0;

        glDeleteProgram(gshaderProgramObject_PF);
        gshaderProgramObject_PF = 0;

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
