#include<windows.h>
#include"Header.h"

#include<gl/glew.h>

#include"vmath.h"
#include"Sphere.h"

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

GLuint gVao_Sphere;
GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

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

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f };
GLfloat lDefues[] = { 1.0f, 1.0f, 1.0f };
GLfloat lSpecular[] = { 1.0f, 1.0f, 1.0f };
GLfloat lPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 50.0f;

/*New For Sphere*/
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint gNumVertices;
GLuint gNumElements;


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
    TCHAR Appname[] = TEXT("OpenGLPP_LIGHTS");
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
        TEXT("Darshan's OpenGLPP : Sphere Rotation With per fragment Lights."),
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
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_p_matrix;" \
        "uniform int u_lKey;" \
        "uniform vec4 u_light_position;" \
        "out vec3 transformed_normal;"  \
        "out vec3 light_direction;\n" \
        "out vec3 view_vector;\n" \
        "void main(void){\n" \
        "if(u_lKey == 1){\n" \
        "vec4 eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" \
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        "light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" \
        "view_vector = normalize(-eye_cordinates.xyz );\n" \
        "}\n" \
        "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
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
        "#version 450 core\n" \
        "uniform int u_lKey;" \
        "uniform vec3 u_la;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_ls;" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_shininess;\n" \
        "in vec3 transformed_normal;\n"  \
        "in vec3 light_direction;\n" \
        "in vec3 view_vector;\n" \
        "out vec4 FragColor;\n" \
        "void main(void){\n" \
        "if(u_lKey == 1){\n" \
        "vec3 normalized_transformed_normal = transformed_normal;\n" \
        "vec3 normalized_light_direction = light_direction;\n" \
        "vec3 normalized_view_vector = view_vector;\n" \
        "vec3 reflection_vector = reflect(-normalized_light_direction , normalized_transformed_normal);\n" \
        "vec3 ambient = u_la * u_ka;\n" \
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normal), 0.0f);\n" \
        "vec3 specular = u_ls * u_ks * pow( max(dot(reflection_vector, normalized_view_vector), 0.0f), u_shininess);\n" \
        "vec3 fong_ads_light = ambient + diffuse + specular;\n" \
        "FragColor = vec4(fong_ads_light, 1.0f);\n"\
        "}\n" \
        "else{\n" \
        "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
        "}\n"
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

    LA_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_la");
    LD_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ld");
    LS_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ls");
    LPosition_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_light_position");

    KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ka");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_kd");
    KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ks");
    shininess = glGetUniformLocation(gshaderProgramObject, "u_shininess");

    /**/
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
        glUniform1f(shininess, Shininess);
        glUniform3fv(LA_Uniform_DM, 1, (GLfloat*)lAmbient);
        glUniform3fv(LD_Uniform_DM, 1, (GLfloat*)lDefues);
        glUniform3fv(LS_Uniform_DM, 1, (GLfloat*)lSpecular);
        glUniform3fv(KA_Uniform_DM, 1, (GLfloat*)material_Ambient);
        glUniform3fv(KD_Uniform_DM, 1, (GLfloat*)material_Diffuse);
        glUniform3fv(KS_Uniform_DM, 1, (GLfloat*)material_Specular);
        glUniform4fv(LPosition_Uniform_DM, 1, (GLfloat*)lPosition);
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
    RotationMatrtix = vmath::rotate(R_angle, 0.0f, 1.0f, 0.0f);
    ModelMatrix = TranslateMatrix * RotationMatrtix;

    PMatrix = gPerspectiveProjectMatix;

    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
    glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

    glBindVertexArray(gVao_Sphere);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

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

