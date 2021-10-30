#include<windows.h>
#include"Header.h"
#include<gl/glew.h>
#include<C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.4\include\cuda_gl_interop.h>
#include<C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.4\include\cuda_runtime.h>

#include"vmath.h"

#include<gl/GL.h>
#include<stdio.h>

#include"Header1.cu.h"

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"cudart.lib")

#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

#define MESS_WIDTH 1024
#define MESS_HEIGHT 1024
#define MY_ARRAY_SIZE  MESS_WIDTH * MESS_HEIGHT * 4

//Cuda Variables
cudaError_t DM_CudaResult;

//
struct cudaGraphicsResource* cuda_Graphics_Resource;

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

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gshaderProgramObject;

float pos[MESS_WIDTH][MESS_HEIGHT][4];
float AnimationTime = 0.0f;


//Local Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);

GLuint gVao;
GLuint gVbo_Position;
GLuint gMVPMatrixUniform;
//vbo for GPU
GLuint gVbo_GPU;

bool bOnGPU = false;


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
    wndclassex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wndclassex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wndclassex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassEx(&wndclassex);

    x = GetSystemMetrics(SM_CXSCREEN);
    y = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
        Appname,
        TEXT("OpenGL : Sine Wave."),
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
        case'c':
        case 'C':
            bOnGPU = false;
            break;

        case'G':
        case 'g':
            bOnGPU = true;
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

    int devCount = 0;

    //Get the Device Count
    DM_CudaResult = cudaGetDeviceCount(&devCount);

    //
    if (DM_CudaResult != cudaSuccess)
    {
        DestroyWindow(ghwnd_DM);
    }
    else if(devCount == 0)
    {
        DestroyWindow(ghwnd_DM);
    }
    else
    {
        //setting default devices
        cudaSetDevice(0);
    }

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
        "uniform mat4 u_mvp_matrix;" \
        "void main(void){\n" \
        "gl_Position = u_mvp_matrix * vPosition;" \
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
        "#version 450 core\n" \
        "out vec4 FragColor;" \
        "void main(void){\n" \
        "FragColor = vec4( 1.0f, 0.5f, 0.0f, 1.0f);\n" \
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

    gMVPMatrixUniform = glGetUniformLocation(gshaderProgramObject, "u_mvp_matrix");

    /*New for traingle */
    const GLfloat triangleVertices[] =
    {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    for (int i = 0; i < MESS_WIDTH; i++)
    {
        for (int j = 0; j < MESS_HEIGHT; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                pos[i][j][k] = 0.0f;
            }
        }
    }

    glGenVertexArrays(1, &gVao);
    glBindVertexArray(gVao);

    glGenBuffers(1, &gVbo_Position);

    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position);
    glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * sizeof(float), pos, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &gVbo_GPU);

    glBindBuffer(GL_ARRAY_BUFFER, gVbo_GPU);
    glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);


    //OpenGL Code InterOparability
    DM_CudaResult = cudaGraphicsGLRegisterBuffer(&cuda_Graphics_Resource, gVbo_GPU, cudaGraphicsMapFlagsWriteDiscard);
    if (DM_CudaResult != cudaSuccess)
    {
        DestroyWindow(ghwnd_DM);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glShadeModel(GL_SMOOTH);
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
    //Declaration 
    void LaunchCPUKernal(unsigned int, unsigned int, float);
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(gshaderProgramObject);

    mat4 modelViewMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();

    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;

   
    glBindVertexArray(gVao);

    if (bOnGPU == true)
    {
        //code
        DM_CudaResult = cudaGraphicsMapResources(1, &cuda_Graphics_Resource, 0);
        if (DM_CudaResult != cudaSuccess)
        {
            DestroyWindow(ghwnd_DM);
        }

        float4* pPos = NULL;
        size_t numBites;

        //take my resouce and map it to pPos
        DM_CudaResult = cudaGraphicsResourceGetMappedPointer((void**)&pPos, &numBites, cuda_Graphics_Resource);
        if (DM_CudaResult != cudaSuccess)
        {
            DestroyWindow(ghwnd_DM);
        }

        LaunchCUDAKernal(pPos, MESS_WIDTH, MESS_HEIGHT, AnimationTime);
        if (DM_CudaResult != cudaSuccess)
        {
            DestroyWindow(ghwnd_DM);
        }
        //
        cudaGraphicsUnmapResources(1, &cuda_Graphics_Resource, 0);
        if (DM_CudaResult != cudaSuccess)
        {
            DestroyWindow(ghwnd_DM);
        }
        //
        glBindBuffer(GL_ARRAY_BUFFER, gVbo_GPU);

    }
    else
    {
        LaunchCPUKernal(MESS_WIDTH, MESS_HEIGHT, AnimationTime);
        glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position);
        glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * sizeof(float), pos, GL_DYNAMIC_DRAW);

    }

    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glDrawArrays(GL_POINTS, 0, MESS_WIDTH * MESS_HEIGHT);
    
    glBindVertexArray(0);

    glUseProgram(0);

    AnimationTime = AnimationTime + 0.01f;

    SwapBuffers(ghdc_DM);

}



void LaunchCPUKernal(unsigned int MessWidth, unsigned int MessHeight, float time)
{
    for (int i = 0; i < MESS_WIDTH; i++)
    {
        for (int j = 0; j < MESS_HEIGHT; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                float u = i / (float)MESS_WIDTH;
                float v = j / (float)MESS_HEIGHT;
                u = u * 2.0f - 1.0f;
                v = v * 2.0f - 1.0f;
                float freq = 4.0f;
                float w = sinf(u * freq + time) + cosf(v * freq + time) * 1.0f;
                if (k == 0)
                {
                    pos[i][j][k] = u;
                }
                if (k == 1)
                {
                    pos[i][j][k] = w;
                }
                if (k == 2)
                {
                    pos[i][j][k] = v;
                }
                if (k == 3)
                {
                    pos[i][j][k] = 1.0f;
                }
            }
        }
    }
}

void unInitialize()
{
    if (cuda_Graphics_Resource)
    {
        cudaGraphicsUnregisterResource(cuda_Graphics_Resource);
        cuda_Graphics_Resource = NULL;
    }

    if (gVbo_GPU)
    {
        glDeleteVertexArrays(1, &gVbo_GPU);
        gVbo_GPU = 0;
    }

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


