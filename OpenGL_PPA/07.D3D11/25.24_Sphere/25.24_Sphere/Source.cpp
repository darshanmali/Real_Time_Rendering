/*

    3D Model Lights

*/
#include<Windows.h>
#include<stdio.h>

#include<d3d11.h>
//1st change
#include<d3dcompiler.h>

//sphere
#include"Sphere.h"

//2nd Change
#pragma warning(disable: 4838 )// jsut to remove the worning 
#include"XNAMath_204/xnamath.h"

#pragma comment(lib, "d3d11.lib")

//3rd change 
#pragma comment(lib, "D3dcompiler.lib")

#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

int gWidth = WIN_WIDTH_DM;
int gHeight = WIN_HEIGHT_DM;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


float material_Ambient[24][3] = {

{0.0215f, 0.1745f, 0.0215f},
{0.135f, 0.2225f, 0.1575f},

{0.05375f, 0.05f, 0.06625f},
{0.25f, 0.20725f, 0.20725f},

{0.1745f, 0.01175f, 0.01175f},
{1.0f, 0.18725f, 0.1745f},

{0.329412f, 0.223529f, 0.027451f},
{0.2125f, 0.1275f, 0.054f},

{0.25f, 0.25f, 0.25f},
{0.19125f, 0.0735f, 0.0225f},

{0.24725f, 0.1995f, 0.0745f},
{0.19225f, 0.19225f, 0.19225f},

//1st - 3rd
{0.0f, 0.0f, 0.0f},
{0.0f,0.1f,0.06f},

{0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f},

{0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f},

//1st - 4rd
{0.02f, 0.02f, 0.02f},
{0.0f, 0.05f, 0.05f},

{0.0f, 0.05f, 0.0f},
{0.05f, 0.0f, 0.0f},

{0.05f, 0.05f, 0.05f},
{0.05f, 0.05f, 0.0f},

};

float material_Diffuse[24][3] = {

{0.07568f, 0.61424f, 0.07568f},
{0.54f, 0.89f, 0.63f},

{0.18275f, 0.17f, 0.22525f},
{1.0f, 0.829f, 0.829f},

{0.61424f, 0.04136f, 0.04136f},
{0.396f, 0.74151f, 0.69102f},

{0.780392f, 0.568627f, 0.113725f},
{0.714f, 0.4284f, 0.18144f},

{0.4f, 0.4f, 0.4f},
{0.7038f, 0.27048f, 0.0828f},

{0.75164f, 0.60648f, 0.22648f},
{0.50754f, 0.50754f,0.50754f},

//1st - 3rd
{0.01f, 0.01f, 0.01f},
{0.0f, 0.50980392f, 0.50980392f},

{0.1f, 0.35f, 0.1f},
{0.5f, 0.0f, 0.0f},

{0.55f, 0.55f, 0.55f},
{0.5f, 0.5f, 0.0f},

//1st - 4rd
{0.01f, 0.01f, 0.01f},
{0.4f, 0.5f, 0.5f},

{0.4f, 0.5f, 0.4f},
{0.5f, 0.4f, 0.4f},

{0.5f, 0.5f, 0.5f},
{0.5f, 0.5f, 0.4f},

};

float material_Specular[24][3] = {

 {0.633f, 0.727811f, 0.633f},
 {0.316228f, 0.316228f, 0.316228f},

{0.332741f, 0.328634f, 0.346435f},
{0.296648f, 0.296648f, 0.296648f},

{0.727811f, 0.626959f, 0.626959f},
{0.297254f, 0.30829f, 0.306678f},

{0.992157f, 0.941176f, 0.807843f},
{0.393548f, 0.271906f, 0.166721f},

{0.774597f, 0.774597f, 0.774597f},
{0.256777f, 0.137622f, 0.086014f},

{0.628281f, 0.555802f, 0.366065f},
{0.508273f, 0.508273f, 0.508273f},

//1st - 3rd
{0.50f, 0.50f, 0.50f},
{0.50196078f, 0.50196078f, 0.50196078f},

{0.45f, 0.55f, 0.45f},
{0.7f, 0.6f, 0.6f},

{0.70f, 0.70f, 0.70f},
{0.60f, 0.60f, 0.50f},

//1st - 4rd
{0.4f, 0.4f, 0.4f},
{0.04f, 0.7f, 0.7f},

{0.04f, 0.7f, 0.04f},
{0.7f, 0.04f, 0.04f},

{0.7f, 0.7f, 0.7f},
{0.7f, 0.7f, 0.04f},
};

float Shininess[24] = {
    0.6f * 128.0f,
    0.1f * 128.0f,

    0.3f * 128.0f,
    0.088f * 128.0f,

    0.6f * 128.0f,
    0.1f * 128.0f,

    0.21794872f * 128.0f,
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


FILE* gpFile_DM = NULL;
char gszLogFileName[] = "Log.txt";

HWND ghwnd_DM = NULL;

DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };

bool gbFullScreen_DM = false;
bool gbEscapeKeyIsPressed = false;
bool gbActiveWindows_DM = false;

float gClearColor[4];
IDXGISwapChain* gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;

ID3D11InputLayout* gpID3D11InputLayout = NULL;
ID3D11InputLayout* gpID3D11InputLayout_PS = NULL;

ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_PS = NULL;

//Shaders variables
ID3D11VertexShader* gpID3D11VertexShader_PV = NULL;
ID3D11PixelShader* gpID3D11PixelShader_PV = NULL;

ID3D11VertexShader* gpID3D11VertexShader_Pixel = NULL;
ID3D11PixelShader* gpID3D11PixelShader_Pixel = NULL;



bool X_Rotation = true;
bool Y_Rotation = false;
bool Z_Rotation = false;

int x;
int y;
int w;
int h;
int iterator_Color;


// 2D Triangle 

//sPHERE Model Variables  

ID3D11Buffer* gpID3D11_Sphere_VertextBuffer_Position = NULL;
ID3D11Buffer* gpID3D11_Sphere_VertextBuffer_Normals = NULL;
ID3D11Buffer* gpID3D11_Sphere_Index_Buffer = NULL;


struct CBUFFER
{
    XMMATRIX WorldMatrix; //Model Matrix
    XMMATRIX ViewMatrix; //View Matrix
    XMMATRIX ProjectionMatrix;

    //Light Variables
    XMVECTOR LA[1];
    XMVECTOR LD[1];
    XMVECTOR LS[1];
    XMVECTOR Light_Position[1];

    XMVECTOR KA;
    XMVECTOR KD;
    XMVECTOR KS;

    unsigned int Key_Pressed;
    float shininess;
};

struct Light
{
    XMVECTOR lAmbient;
    XMVECTOR lDefues;
    XMVECTOR lSpecular;
    XMVECTOR lPosition;
};

struct Light Lights[3];


//Sphere
/*New For Sphere*/
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

unsigned int gNumVertices;
unsigned int gNumElements;


//light
BOOL gbLight = TRUE;

//backface culling 
ID3D11RasterizerState* Rasterization_State;

float Sphere_angle = 0.0f;

//Depth 
ID3D11DepthStencilView* gpiD3D11DepthStaincilView = NULL;

XMMATRIX gPerpectiveGraphicProjectionMatrix;

HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;

DWORD dwstyle_DM;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //declaration
    HRESULT Initialize(void);
    void unInitialize(void);
    void Display(void);

    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szClassname[] = TEXT("Direct3D11");
    bool bDone = false;
    INT Width = 800;
    INT Height = 600;

    //code
    if (fopen_s(&gpFile_DM, gszLogFileName, "w") != 0)
    {
        MessageBox(NULL, TEXT("Log File Can not be created..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_TOPMOST);
        exit(0);
    }
    else
    {
        fprintf_s(gpFile_DM, "Log File Is Successfully Opened.\n");
        fclose(gpFile_DM);
    }

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpszClassName = szClassname;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszMenuName = NULL;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);


    RegisterClassEx(&wndclass);

    x = GetSystemMetrics(SM_CXSCREEN);
    y = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
        szClassname,
        TEXT("Darshan's : Direct X 3D Sphere 3 Lights...!"),
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

    HRESULT hr;
    hr = Initialize();
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Initialize() Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Initialize() Succeeded...\n");
        fclose(gpFile_DM);
    }

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
            Display();
            if (gbActiveWindows_DM = true)
            {
                if (gbEscapeKeyIsPressed == true)
                    bDone = true;
            }
        }

    }

    unInitialize();

    return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //Function
    HRESULT Resize(int, int);
    void ToggelFullScreen(void);
    void unInitialize(void);
    HRESULT hr;

    //code
    switch (iMsg)
    {
    case WM_PAINT:

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
            if (gbEscapeKeyIsPressed == false)
                gbEscapeKeyIsPressed = true;
            break;
        case 'L':
        case 'l':
            if (gbLight == TRUE)
            {
                gbLight = FALSE;
            }
            else
            {
                gbLight = TRUE;
            }
            break;
        case 'X':
        case 'x':

            X_Rotation = true;
            Y_Rotation = false;
            Z_Rotation = false;
            break;
        case 'Y':
        case 'y':

            X_Rotation = false;
            Y_Rotation = true;
            Z_Rotation = false;
            break;
        case 'Z':
        case 'z':

            X_Rotation = false;
            Y_Rotation = false;
            Z_Rotation = true;
            break;
        case 0x46:
        case 0x66:
            if (gbFullScreen_DM == false)
            {
                ToggelFullScreen();
            }
            else
            {
                ToggelFullScreen();
            }
            break;
        default:
            break;
        }
        break;
    case WM_SIZE:
        if (gpID3D11DeviceContext)
        {
            hr = Resize(LOWORD(lParam), HIWORD(lParam));
            gWidth = LOWORD(lParam);
            gHeight = HIWORD(lParam);

            if (FAILED(hr))
            {
                fopen_s(&gpFile_DM, gszLogFileName, "a+");
                fprintf_s(gpFile_DM, "Resize() Failed. Exiting Now...\n");
                fclose(gpFile_DM);
                return(hr);
            }
            else
            {
                fopen_s(&gpFile_DM, gszLogFileName, "a+");
                fprintf_s(gpFile_DM, "Resize() Succeeded...\n");
                fclose(gpFile_DM);
            }
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
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
    if (gbFullScreen_DM == false)
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
        gbFullScreen_DM = true;
    }
    else
    {
        SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_D | WS_OVERLAPPEDWINDOW));
        SetWindowPlacement(ghwnd_DM, &wpPrev_DM);
        SetWindowPos(ghwnd_DM, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
        gbFullScreen_DM = false;
    }

}

HRESULT Initialize()
{
    //Function Declaration
    void unInitialize(void);
    HRESULT Resize(int, int);

    //Variable Declaration
    HRESULT hr;
    D3D_DRIVER_TYPE d3dDriverType;
    D3D_DRIVER_TYPE d3dDriverTypes[] =
    { D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE };
    D3D_FEATURE_LEVEL d3dFeatureLevel_required = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL d3dFeatureLevel_acquired = D3D_FEATURE_LEVEL_10_0;

    UINT createDeviceFlags = 0;
    UINT numDriverType = 0;
    UINT numFeatureLevels = 1;

    //code 
    numDriverType = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);

    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;

    ZeroMemory((void*)&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    dxgiSwapChainDesc.BufferCount = 1;
    dxgiSwapChainDesc.BufferDesc.Width = WIN_WIDTH_DM;
    dxgiSwapChainDesc.BufferDesc.Height = WIN_HEIGHT_DM;
    dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    dxgiSwapChainDesc.OutputWindow = ghwnd_DM;
    dxgiSwapChainDesc.SampleDesc.Count = 1;
    dxgiSwapChainDesc.SampleDesc.Quality = 0;
    dxgiSwapChainDesc.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverType; driverTypeIndex++)
    {
        d3dDriverType = d3dDriverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            d3dDriverType,
            NULL,
            createDeviceFlags,
            &d3dFeatureLevel_required,
            numFeatureLevels,
            D3D11_SDK_VERSION,
            &dxgiSwapChainDesc,
            &gpIDXGISwapChain,
            &gpID3D11Device,
            &d3dFeatureLevel_acquired,
            &gpID3D11DeviceContext);


        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11CreateDeviceAndSwapChain() Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11CreateDeviceAndSwapChain() Succeeded...\n");
        fprintf_s(gpFile_DM, "The Chosen Driver Is Of \n");
        if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
        {
            fprintf_s(gpFile_DM, "Hardware Type.\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
        {
            fprintf_s(gpFile_DM, "Warp Type.\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
        {
            fprintf_s(gpFile_DM, "Reference Type.\n");
        }
        else
        {
            fprintf_s(gpFile_DM, " unknown Type.\n");
        }

        fprintf_s(gpFile_DM, "\nThe Supported Highest Feature level Is \n");
        if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_11_0)
        {
            fprintf_s(gpFile_DM, "11.0.\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_1)
        {
            fprintf_s(gpFile_DM, "10.1.\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_0)
        {
            fprintf_s(gpFile_DM, "10.0.\n");
        }
        else
        {
            fprintf_s(gpFile_DM, " unknown.\n");
        }

        fclose(gpFile_DM);
    }

    /***************************************************vertex shader for per vertex light*********************/

    const char* vertexShaderSourceCode =
        "cbuffer ConstantBuffer{\n"\

        "float4x4 worldMatrix;\n"\
        "float4x4 ViewMatrix;\n"\
        "float4x4 ProjectionMatrix;\n"\

        "float4 la[1];\n"\
        "float4 ld[1];\n"\
        "float4 ls[1];\n"\
        "float4 lightPosition[1];\n"\

        "float4 ka;\n"\
        "float4 kd;\n"\
        "float4 ks;\n"\
        "uint keyPressed;\n"\
        "float shininess;\n"\

        "}\n"\

        "struct VertexOutPut{\n"\
        "float4 position      : SV_POSITION;\n"\
        "float3 defused_Light : COLOR;\n"\
        "};\n"\

        "float3 fong_ads_light = float3(0.0f,0.0f,0.0f);\n" \

        "VertexOutPut main(float4 pos : POSITION, float3 normal : NORMAL){\n"\
        "VertexOutPut outPut;\n"\
        "outPut.defused_Light = float3(0.0f, 0.0f, 0.0f);\n"\
        "if(keyPressed == 1){\n"\
        "float4 eye_Position = mul(mul(worldMatrix , ViewMatrix), pos);\n"\
        "float3 tNormal = normalize(mul((float3x3)mul(worldMatrix,ViewMatrix), (float3)normal));\n"\
        "float3 view_vector = normalize(-eye_Position.xyz);\n"\
        "for( int i = 0; i < 1; i++){\n"
        "float3 ambient[1];\n" \
        "float3 diffuse[1];\n" \
        "float3 specular[1];\n" \
        "float3 light_direction[1];\n" \
        "float3 reflection_vector[1];\n" \
        "light_direction[i] = normalize((float3)lightPosition[i] - (float3)eye_Position);\n" \
        "reflection_vector[i] = reflect(-light_direction[i], tNormal);\n" \
        "ambient[i] = la[i] * ka;\n" \
        "diffuse[i] = ld[i] * kd * max( dot(light_direction[i], tNormal), 0.0);\n" \
        "specular[i] = ls[i] * ks * pow( max(dot(reflection_vector[i], view_vector), 0.0), shininess);\n" \
        "outPut.defused_Light = outPut.defused_Light + ambient[i] + diffuse[i] + specular[i] ;\n" \
        "}\n" \

        "}\n" \
        "else{\n"\
        "outPut.defused_Light = float3(1.0f, 1.0f, 1.0f);"\
        "}"\

        "float4 position = mul(ProjectionMatrix, mul(mul(worldMatrix, ViewMatrix), pos));"\
        "outPut.position = position;"\

        "return(outPut);"\
        "}";

    //Blog is used for to keep any data inside it
    ID3DBlob* pID3DBlob_VartexShaderCode = NULL;
    ID3DBlob* pID3DBlob_Error = NULL;

    hr = D3DCompile(vertexShaderSourceCode,//code
        lstrlenA(vertexShaderSourceCode) + 1,//Length
        "VS",//VErtexShader Compiler Identifier
        NULL,//
        D3D_COMPILE_STANDARD_FILE_INCLUDE,//
        "main",//
        "vs_5_0",//version 
        0,//
        0,//
        &pID3DBlob_VartexShaderCode,//
        &pID3DBlob_Error);//

    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile_DM, gszLogFileName, "a+");
            fprintf_s(gpFile_DM, "D3D11Compile() Failed. For VErtex Shader : %s.\n",
                (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile_DM);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Compile() Succeeded For VErtex Shader.\n");
        fclose(gpFile_DM);

    }

    //CreatevertexShader Here 
    hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VartexShaderCode->GetBufferPointer(),
        pID3DBlob_VartexShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11VertexShader_PV);
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::Create VErtexShader() Failed.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::Create VErtexShader() Succeedded.\n");
        fclose(gpFile_DM);
    }


    /***************************************************Pixel shader for per vertex light*********************/
    const char* PixelShaderSourceCode =
        "struct VertexOutPut"\
        "{"\
        "float4 position      : SV_POSITION;"\
        "float3 defused_Light : COLOR;"\
        "};"\

        "float4 main(VertexOutPut input) : SV_TARGET"\
        "{"\

        "float4 color = float4(input.defused_Light, 1.0f);"\

        "return(color);"\
        "}";

    ID3DBlob* pID3DBlob_PixelShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(PixelShaderSourceCode,//code
        lstrlenA(PixelShaderSourceCode) + 1,//Length
        "PS",//VErtexShader Compiler Identifier
        NULL,//
        D3D_COMPILE_STANDARD_FILE_INCLUDE,//
        "main",//
        "ps_5_0",//version 
        0,//
        0,//
        &pID3DBlob_PixelShaderCode,//
        &pID3DBlob_Error);//

    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile_DM, gszLogFileName, "a+");
            fprintf_s(gpFile_DM, "D3D11Compile() Failed. For Pixel Shader : %s.\n", (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile_DM);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Compile() Succeeded For Pixel Shader.\n");
        fclose(gpFile_DM);

    }

    //CreatePixelShader Here 
    hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode->GetBufferPointer(),
        pID3DBlob_PixelShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11PixelShader_PV);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreatePixelShader() Failed.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreatePixelShader() Succeedded.\n");
        fclose(gpFile_DM);
    }


    /*
        inputlayout : we are telling directX...how attributes are kept in and giving to shaders.
    */

    //create and set input layout
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[2];
    inputElementDesc[0].SemanticName = "POSITION";
    inputElementDesc[0].SemanticIndex = 0;
    inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc[0].InputSlot = 0;
    inputElementDesc[0].AlignedByteOffset = 0;
    inputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputElementDesc[0].InstanceDataStepRate = 0;

    inputElementDesc[1].SemanticName = "NORMAL";
    inputElementDesc[1].SemanticIndex = 0;
    inputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc[1].InputSlot = 1;
    inputElementDesc[1].AlignedByteOffset = 0;
    inputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputElementDesc[1].InstanceDataStepRate = 0;


    hr = gpID3D11Device->CreateInputLayout(inputElementDesc,
        _ARRAYSIZE(inputElementDesc),
        pID3DBlob_VartexShaderCode->GetBufferPointer(),
        pID3DBlob_VartexShaderCode->GetBufferSize(),
        &gpID3D11InputLayout);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateInputLayout()2 Failed. \n");
        fclose(gpFile_DM);
        if (pID3DBlob_VartexShaderCode)
        {
            pID3DBlob_VartexShaderCode->Release();
            pID3DBlob_VartexShaderCode = NULL;

        }

        if (pID3DBlob_PixelShaderCode)
        {
            pID3DBlob_PixelShaderCode->Release();
            pID3DBlob_PixelShaderCode = NULL;

        }

        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateInputLayout() Succeeded. \n");
        fclose(gpFile_DM);
        if (pID3DBlob_VartexShaderCode)
        {
            pID3DBlob_VartexShaderCode->Release();
            pID3DBlob_VartexShaderCode = NULL;

        }
        if (pID3DBlob_PixelShaderCode)
        {
            pID3DBlob_PixelShaderCode->Release();
            pID3DBlob_PixelShaderCode = NULL;
        }
    }


    /***************************************************vertex shader for per Fragment  light *********************/
        //HLSL(High Level Shading language ) == GLSL()
    const char* vertexShaderSourceCode_PS =
        "cbuffer ConstantBuffer{\n"\

        "float4x4 worldMatrix;\n"\
        "float4x4 ViewMatrix;\n"\
        "float4x4 ProjectionMatrix;\n"\

        "float4 la[1];\n"\
        "float4 ld[1];\n"\
        "float4 ls[1];\n"\
        "float4 lightPosition[1];\n"\

        "float4 ka;\n"\
        "float4 kd;\n"\
        "float4 ks;\n"\
        "uint keyPressed;\n"\
        "float shininess;\n"\

        "}\n"\

        "struct VertexOutPut"\
        "{"\
        "float4 position       : SV_POSITION;"\
        "float3 tranformed_normal : NORMAL_0;"\
        "float4 light_direction : NORMAL_1;"\
        "float3 viewer_vector : NORMAL_2;"\
        "};"\

        "VertexOutPut main(float4 pos : POSITION, float3 normal : NORMAL)"\
        "{"\

        "VertexOutPut outPut;"\

        "float4 eye_Position = mul(worldMatrix, pos);"\
        "float4 eye_coordinates = mul(ViewMatrix, eye_Position);"\
        "outPut.tranformed_normal = mul((float3x3)worldMatrix, (float3)normal);"\
        "outPut.light_direction = eye_coordinates;"\

        "outPut.viewer_vector = -eye_coordinates.xyz;\n"\

        "float4 position = mul(worldMatrix, pos);"\
        "position = mul(ViewMatrix, position);"\
        "position = mul(ProjectionMatrix, position);"

        "outPut.position = position;"\
        "return(outPut);"\
        "}";

    //Blog is used for to keep any data inside it
    pID3DBlob_VartexShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(vertexShaderSourceCode_PS,//code
        lstrlenA(vertexShaderSourceCode_PS) + 1,//Length
        "VS",//VErtexShader Compiler Identifier
        NULL,//
        D3D_COMPILE_STANDARD_FILE_INCLUDE,//
        "main",//
        "vs_5_0",//version 
        0,//
        0,//
        &pID3DBlob_VartexShaderCode,//
        &pID3DBlob_Error);//

    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile_DM, gszLogFileName, "a+");
            fprintf_s(gpFile_DM, "D3D11Compile() Failed. For VErtex Shader : %s.\n",
                (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile_DM);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Compile() Succeeded For VErtex Shader.\n");
        fclose(gpFile_DM);

    }

    //CreatevertexShader Here 
    hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VartexShaderCode->GetBufferPointer(),
        pID3DBlob_VartexShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11VertexShader_Pixel);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::Create VErtexShader() for pixel shader Failed.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::Create VErtexShader() Succeedded.\n");
        fclose(gpFile_DM);
    }

    /***************************************************Pixel shader for per Fragment  light *********************/

    const char* PixelShaderSourceCode_PS =
        "cbuffer ConstantBuffer{\n"\

        "float4x4 worldMatrix;\n"\
        "float4x4 ViewMatrix;\n"\
        "float4x4 ProjectionMatrix;\n"\

        "float4 la[1];\n"\
        "float4 ld[1];\n"\
        "float4 ls[1];\n"\
        "float4 lightPosition[1];\n"\

        "float4 ka;\n"\
        "float4 kd;\n"\
        "float4 ks;\n"\
        "uint keyPressed;\n"\
        "float shininess;\n"\

        "}\n"\

        "struct VertexOutPut"\
        "{"\
        "float4 position       : SV_POSITION;"\
        "float3 tranformed_normal : NORMAL_0;"\
        "float4 light_direction : NORMAL_1;"\
        "float3 viewer_vector : NORMAL_2;"\
        "};"\

        "float4 main(VertexOutPut input) : SV_TARGET"\
        "{"\
        "float3 fong_ads_light = float3( 0.0f, 0.0f, 0.0f);;"\
        "float3 FragColor = float3( 0.0f, 0.0f, 0.0f);;"\
        /*
        "if(keyPressed == 1){\n" \
        */
        "float3 normalized_trans_light = normalize(input.tranformed_normal);"\
        "float3 normalized_viewer_vector = -normalize(input.viewer_vector);"\

        "for( int i = 0; i < 1; i++){\n"\
        "float3 ambient[1];\n" \
        "float3 diffuse[1];\n" \
        "float3 specular[1];\n" \
        "float3 reflection_vector[1];\n" \
        "float3 normalized_light_direction[1];\n" \

        "normalized_light_direction[i] = normalize((float3)lightPosition[i] - (float3)input.light_direction);"\
        "reflection_vector[i] = reflect(normalized_light_direction[i], normalized_trans_light);\n" \

        "ambient[i] = (float3)la[i] * (float3)ka;"\
        "diffuse[i] = (float3)ld[i] * (float3)kd * max(dot(normalized_light_direction[i], normalized_trans_light), 0.0);\n"\
        "specular[i] = (float3)ls[i] * (float3)ks * pow(max(dot(reflection_vector[i], normalized_viewer_vector), 0.0), shininess);\n"\

        "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];"\
        "}\n"
        "FragColor = fong_ads_light;\n"\
 

        "float4 color = float4(FragColor, 1.0);"\

        "return(color);"\
        "}";

    pID3DBlob_PixelShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(PixelShaderSourceCode_PS,//code
        lstrlenA(PixelShaderSourceCode_PS) + 1,//Length
        "PS",//VErtexShader Compiler Identifier
        NULL,//
        D3D_COMPILE_STANDARD_FILE_INCLUDE,//
        "main",//
        "ps_5_0",//version 
        0,//
        0,//
        &pID3DBlob_PixelShaderCode,//
        &pID3DBlob_Error);//

    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile_DM, gszLogFileName, "a+");
            fprintf_s(gpFile_DM, "D3D11Compile() Failed. For Pixel Shader : %s.\n", (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile_DM);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Compile() Succeeded For Pixel Shader.\n");
        fclose(gpFile_DM);

    }

    //CreatePixelShader Here 
    hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode->GetBufferPointer(),
        pID3DBlob_PixelShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11PixelShader_Pixel);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreatePixelShader() Failed.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreatePixelShader() Succeedded.\n");
        fclose(gpFile_DM);
    }

    hr = gpID3D11Device->CreateInputLayout(inputElementDesc,
        _ARRAYSIZE(inputElementDesc),
        pID3DBlob_VartexShaderCode->GetBufferPointer(),
        pID3DBlob_VartexShaderCode->GetBufferSize(),
        &gpID3D11InputLayout);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateInputLayout()2 Failed. \n");
        fclose(gpFile_DM);
        if (pID3DBlob_VartexShaderCode)
        {
            pID3DBlob_VartexShaderCode->Release();
            pID3DBlob_VartexShaderCode = NULL;

        }

        if (pID3DBlob_PixelShaderCode)
        {
            pID3DBlob_PixelShaderCode->Release();
            pID3DBlob_PixelShaderCode = NULL;

        }

        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateInputLayout() Succeeded. \n");
        fclose(gpFile_DM);
        if (pID3DBlob_VartexShaderCode)
        {
            pID3DBlob_VartexShaderCode->Release();
            pID3DBlob_VartexShaderCode = NULL;

        }
        if (pID3DBlob_PixelShaderCode)
        {
            pID3DBlob_PixelShaderCode->Release();
            pID3DBlob_PixelShaderCode = NULL;
        }
    }


  
    /***************************************************Spherre Data creation *********************/

    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    gNumVertices = getNumberOfSphereVertices();
    gNumElements = getNumberOfSphereElements();

    D3D11_BUFFER_DESC bufferDesc;

    ////=*******************************Sphere Vertex cords Creation

    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc,
        NULL,
        &gpID3D11_Sphere_VertextBuffer_Position);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed for Vertex_Rect Buffer. \n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded for Vertex_Rect Buffer. \n");
        fclose(gpFile_DM);
    }

    //Copy vertices_Rect into above created buffer
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    gpID3D11DeviceContext->Map(gpID3D11_Sphere_VertextBuffer_Position,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource);

    memcpy(mappedSubresource.pData,
        sphere_vertices,
        gNumVertices * 3 * sizeof(float));

    gpID3D11DeviceContext->Unmap(gpID3D11_Sphere_VertextBuffer_Position, 0);


    ////=*******************************Sphere Normal cords Creation

    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc,
        NULL,
        &gpID3D11_Sphere_VertextBuffer_Normals);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed for ccolor_Rectolor Buffer. \n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded for color_Rect Buffer. \n");
        fclose(gpFile_DM);
    }

    ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    gpID3D11DeviceContext->Map(gpID3D11_Sphere_VertextBuffer_Normals,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource);

    memcpy(mappedSubresource.pData,
        sphere_normals,
        gNumVertices * 3 * sizeof(float));

    gpID3D11DeviceContext->Unmap(gpID3D11_Sphere_VertextBuffer_Normals, 0);

    //=******************************* Sphere Index cords Creation

    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = gNumElements * sizeof(short);
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc,
        NULL,
        &gpID3D11_Sphere_Index_Buffer);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed for gpID3D11_Sphere_Index_Buffer Buffer. \n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded for gpID3D11_Sphere_Index_Buffer Buffer. \n");
        fclose(gpFile_DM);
    }

    ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    gpID3D11DeviceContext->Map(gpID3D11_Sphere_Index_Buffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource);

    memcpy(mappedSubresource.pData,
        sphere_elements,
        gNumElements * sizeof(short));

    gpID3D11DeviceContext->Unmap(gpID3D11_Sphere_Index_Buffer, 0);

    //define and set the contex buffer
    D3D11_BUFFER_DESC bufferDesc_ContentBuffer;
    ZeroMemory(&bufferDesc_ContentBuffer,
        sizeof(D3D11_BUFFER_DESC));
    bufferDesc_ContentBuffer.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc_ContentBuffer.ByteWidth = sizeof(CBUFFER);
    bufferDesc_ContentBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc_ContentBuffer,
        nullptr,
        &gpID3D11Buffer_ConstantBuffer);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed For Constant Buffer.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded For Constant Buffer. \n");
        fclose(gpFile_DM);
    }

    hr = gpID3D11Device->CreateBuffer(&bufferDesc_ContentBuffer,
        nullptr,
        &gpID3D11Buffer_ConstantBuffer_PS);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed For Constant Buffer.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded For Constant Buffer. \n");
        fclose(gpFile_DM);
    }


    
    /*Creata and set rasterizer state.*/

    D3D11_RASTERIZER_DESC d3d11RasterRizerDesk;
    ZeroMemory(&d3d11RasterRizerDesk, sizeof(D3D11_RASTERIZER_DESC));
    d3d11RasterRizerDesk.AntialiasedLineEnable = TRUE;
    d3d11RasterRizerDesk.CullMode = D3D11_CULL_NONE;
    d3d11RasterRizerDesk.DepthBias = 0;
    d3d11RasterRizerDesk.DepthBiasClamp = 0.0f;
    d3d11RasterRizerDesk.DepthClipEnable = TRUE;
    d3d11RasterRizerDesk.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_SOLID == GL_POLYGON_MODE (IN OPENGL )
    d3d11RasterRizerDesk.FrontCounterClockwise = FALSE;
    d3d11RasterRizerDesk.MultisampleEnable = FALSE; //this defines the quality of our model 
    d3d11RasterRizerDesk.ScissorEnable = FALSE;
    d3d11RasterRizerDesk.SlopeScaledDepthBias = 0.0f;

    hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterRizerDesk, &Rasterization_State);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "gpID3D11Device->CreateRasterizerState() Failed For Rasterize struct.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "gpID3D11Device->CreateRasterizerState() succed For Rasterize struct \n");
        fclose(gpFile_DM);
    }

    gpID3D11DeviceContext->RSSetState(Rasterization_State);

    gClearColor[0] = 0.0f;
    gClearColor[1] = 0.0f;
    gClearColor[2] = 0.0f;
    gClearColor[3] = 1.0f;

    gPerpectiveGraphicProjectionMatrix = XMMatrixIdentity();

    hr = Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM);
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Resize() Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "Resize() Succeeded...\n");
        fclose(gpFile_DM);
    }

    return (S_OK);

}

HRESULT Resize(int width, int height)
{
    //code
    HRESULT hr = S_OK;

    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    ID3D11Texture2D* pID3D11Texture2D_BackBuffer;

    gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (LPVOID*)&pID3D11Texture2D_BackBuffer);

    hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer,
        NULL,
        &gpID3D11RenderTargetView);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateRenderTargetView() Failed.Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateRenderTargetView() Succeeded...\n");
        fclose(gpFile_DM);
    }

    pID3D11Texture2D_BackBuffer->Release();
    pID3D11Texture2D_BackBuffer = NULL;

    //depth

    /*
        Depth Enable State should be RTV and before OMSet
        start writing Depth Enabling

        just like RTV DSE also needs

    */

    D3D11_TEXTURE2D_DESC  d3d11_Texture2D_Desk;
    ZeroMemory(&d3d11_Texture2D_Desk, sizeof(D3D11_TEXTURE2D_DESC));

    d3d11_Texture2D_Desk.Width = (UINT)width;
    d3d11_Texture2D_Desk.Height = (UINT)height;
    d3d11_Texture2D_Desk.Format = DXGI_FORMAT_D32_FLOAT;//Depth is 32 bit
    d3d11_Texture2D_Desk.Usage = D3D11_USAGE_DEFAULT;
    d3d11_Texture2D_Desk.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    d3d11_Texture2D_Desk.SampleDesc.Count = 1; // THIS CAN BE 1 TO 4 ( WHICH DEFINES QUALITY )
    d3d11_Texture2D_Desk.SampleDesc.Quality = 0; // 0 TELLS DEFAULT QUALITY.
    d3d11_Texture2D_Desk.ArraySize = 1;
    d3d11_Texture2D_Desk.MipLevels = 1; // 1 == DEFAULT MIP MAP LEVEL
    d3d11_Texture2D_Desk.CPUAccessFlags = 0; //0 == Default
    d3d11_Texture2D_Desk.MiscFlags = 0;

    ID3D11Texture2D* pid3d11_Texture_2D_Depth_Buffer = NULL;

    /*2nd Parrmeter : d3d11Sub_Resource_Data = This is used if you have some data right now else it has to be null.*/
    hr = gpID3D11Device->CreateTexture2D(&d3d11_Texture2D_Desk, NULL, &pid3d11_Texture_2D_Depth_Buffer);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateTexture2D() for DepthBuffer Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateTexture2D() for DepthBuffer Succeeded...\n");
        fclose(gpFile_DM);
    }

    /*
        Creating Depth Staincil Veiw buffer
    */

    D3D11_DEPTH_STENCIL_VIEW_DESC d3d11_depth_Stencil_View_Desc;

    ZeroMemory(&d3d11_depth_Stencil_View_Desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

    d3d11_depth_Stencil_View_Desc.Format = DXGI_FORMAT_D32_FLOAT;
    d3d11_depth_Stencil_View_Desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;//2dms = 2d Multi_Sample

    hr = gpID3D11Device->CreateDepthStencilView(pid3d11_Texture_2D_Depth_Buffer, &d3d11_depth_Stencil_View_Desc, &gpiD3D11DepthStaincilView);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateDepthStencilView() for Depth Staincil View Failed. Exiting Now...\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateDepthStencilView() for Depth Staincil View Succeeded...\n");
        fclose(gpFile_DM);
    }

    /*
        OutputMergerState
    */
    gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpiD3D11DepthStaincilView);//3rd : Depth_Staincil_View

    

    gPerpectiveGraphicProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(44.0f), (float)width / (float)height, 0.1f, 100.0f);
    //XMMatrixPerspectiveFovLH : fieldofviewLeftHand 

    return(hr);
}

void Display()
{
    //code

    void update(void);

    //clear render target view to a chosen Pyramid_vertices
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);

    //depth
    gpID3D11DeviceContext->ClearDepthStencilView(gpiD3D11DepthStaincilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    //select which vertex buffer to display
    UINT stride = sizeof(float) * 3;
    UINT offset = 0;



    if (gbLight == TRUE)
    {

        gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader_PV, 0, 0);
        gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader_PV, 0, 0);
        gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);
        gpID3D11DeviceContext->VSSetConstantBuffers(0,
            1,
            &gpID3D11Buffer_ConstantBuffer);
    }
    else {


        gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader_Pixel, 0, 0);
        gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader_Pixel, 0, 0);
        gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout_PS);
        gpID3D11DeviceContext->VSSetConstantBuffers(0,
            1,
            &gpID3D11Buffer_ConstantBuffer_PS);

        gpID3D11DeviceContext->PSSetConstantBuffers(0,
            1,
            &gpID3D11Buffer_ConstantBuffer_PS);


    }
    
    gpID3D11DeviceContext->IASetVertexBuffers(0,
        1,//number of buffer 
        &gpID3D11_Sphere_VertextBuffer_Position,
        &stride,
        &offset);
     
    stride = sizeof(float) * 3;
    offset = 0;

    gpID3D11DeviceContext->IASetVertexBuffers(1,
        1,//number of buffer 
        &gpID3D11_Sphere_VertextBuffer_Normals,
        &stride,
        &offset);

    gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11_Sphere_Index_Buffer,
        DXGI_FORMAT_R16_UINT,
        0);


    //select geometry primtive
    gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    //load the data  into  the  constant buffer
    CBUFFER constantBuffer;
    ZeroMemory(&constantBuffer, sizeof(CBUFFER));


    //translation is concerned with world matrix transformation
    XMMATRIX translation_matrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
    XMMATRIX translate = XMMatrixIdentity();
    XMMATRIX ModelMatrix = XMMatrixIdentity();
    XMMATRIX viewMatrix = XMMatrixIdentity();

    ModelMatrix = translation_matrix;

    constantBuffer.WorldMatrix = ModelMatrix;
    constantBuffer.ViewMatrix = viewMatrix;
    constantBuffer.ProjectionMatrix = gPerpectiveGraphicProjectionMatrix;


    if (gbLight)
    {
        constantBuffer.Key_Pressed = 1;
        
        if (X_Rotation)
        {

            constantBuffer.LA[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
            constantBuffer.LD[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.LS[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.Light_Position[0] = XMVectorSet(0.0f, 1.5f * sinf(Sphere_angle), 2.5f * cosf(Sphere_angle), 1.0f);
        }
        if (Y_Rotation)
        {

            constantBuffer.LA[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
            constantBuffer.LD[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.LS[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.Light_Position[0] = XMVectorSet(2.0f * sinf(Sphere_angle), 0.0f, 2.0f * cosf(Sphere_angle), 1.0f);
        }
        if (Z_Rotation)
        {

            constantBuffer.LA[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
            constantBuffer.LD[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.LS[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.Light_Position[0] = XMVectorSet(2.0f * sinf(Sphere_angle), 2.0f * cosf(Sphere_angle), 0.0f, 1.0f);
        }
        

    }
    else
    {
        constantBuffer.Key_Pressed = 1;

        if (X_Rotation)
        {

            constantBuffer.LA[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
            constantBuffer.LD[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.LS[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.Light_Position[0] = XMVectorSet(0.0f, 1.5f * sinf(Sphere_angle), 2.5f * cosf(Sphere_angle), 1.0f);
        }
        if (Y_Rotation)
        {

            constantBuffer.LA[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
            constantBuffer.LD[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.LS[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.Light_Position[0] = XMVectorSet(2.0f * sinf(Sphere_angle), 0.0f, 2.0f * cosf(Sphere_angle), 1.0f);
        }
        if (Z_Rotation)
        {

            constantBuffer.LA[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
            constantBuffer.LD[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.LS[0] = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            constantBuffer.Light_Position[0] = XMVectorSet(2.0f * sinf(Sphere_angle), 2.0f * cosf(Sphere_angle), 0.0f, 1.0f);
        }


        
    }

    D3D11_VIEWPORT d3dViewPort;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {

            x = j * (gWidth / 6);
            y = i * (gWidth / 6);
            w = gWidth / 6;
            h = gHeight / 6;


            d3dViewPort.TopLeftX = x;
            d3dViewPort.TopLeftY = y;
            d3dViewPort.Width = (float)w;
            d3dViewPort.Height = (float)h;
            d3dViewPort.MinDepth = 0.0f;
            d3dViewPort.MaxDepth = 1.0f;

            constantBuffer.KA = XMVectorSet(material_Ambient[i * j][0], material_Ambient[i * j][1], material_Ambient[i * j][2], 1.0f);
            constantBuffer.KD = XMVectorSet(material_Diffuse[i * j][0], material_Diffuse[i * j][1], material_Diffuse[i * j][2], 1.0f);
            constantBuffer.KS = XMVectorSet(material_Specular[i * j][0], material_Specular[i * j][1], material_Specular[i * j][2], 1.0f);
            constantBuffer.shininess = Shininess[i * j];

            if (gbLight)
            {
                //updateSubresource == glUniformMatrix4fv from Opengl
                gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer,
                    0,
                    NULL,
                    &constantBuffer,
                    0,
                    0);
            }
            else
            {
                gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_PS,
                    0,
                    NULL,
                    &constantBuffer,
                    0,
                    0);
            }
            //Risterisation state
            gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);

            gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
        }
    }

    update();
    
    //changing buffers
    gpIDXGISwapChain->Present(0, 0);

}

void update(void)
{
    Sphere_angle = Sphere_angle + 0.005f;
    if (Sphere_angle > 360.0f)
    {
        Sphere_angle = 0;
    }
}

void unInitialize()
{
    //code

    if (Rasterization_State)
    {
        Rasterization_State->Release();
        Rasterization_State = NULL;
    }

    if (gpiD3D11DepthStaincilView)
    {
        gpiD3D11DepthStaincilView->Release();
        gpiD3D11DepthStaincilView = NULL;
    }

    if (gpID3D11Buffer_ConstantBuffer)
    {
        gpID3D11Buffer_ConstantBuffer->Release();
        gpID3D11Buffer_ConstantBuffer = NULL;
    }

    if (gpID3D11InputLayout)
    {
        gpID3D11InputLayout->Release();
        gpID3D11InputLayout = NULL;
    }

    if (gpID3D11PixelShader_PV)
    {
        gpID3D11PixelShader_PV->Release();
        gpID3D11PixelShader_PV = NULL;
    }
    if (gpID3D11VertexShader_PV)
    {
        gpID3D11VertexShader_PV->Release();
        gpID3D11VertexShader_PV = NULL;
    }
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }
    if (gpIDXGISwapChain)
    {
        gpIDXGISwapChain->Release();
        gpIDXGISwapChain = NULL;
    }
    if (gpID3D11DeviceContext)
    {
        gpID3D11DeviceContext->Release();
        gpID3D11DeviceContext = NULL;
    }
    if (gpID3D11Device)
    {
        gpID3D11Device->Release();
        gpID3D11Device = NULL;
    }
    if (gpFile_DM)
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "UnInitialize() Succeeded\n");
        fprintf_s(gpFile_DM, "Log File is Successfully Closed.\n");
        fclose(gpFile_DM);
        gpFile_DM = NULL;
    }

}
