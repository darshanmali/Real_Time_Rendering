#include<Windows.h>
#include<stdio.h>

#include<d3d11.h>
//1st change
#include<d3dcompiler.h>

//2nd Change
#pragma warning(disable: 4838 )// jsut to remove the worning 
#include"XNAMath_204/xnamath.h"

#pragma comment(lib, "d3d11.lib")

//3rd change 
#pragma comment(lib, "D3dcompiler.lib")

#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

ID3D11VertexShader* gpID3D11VertexShader = NULL;
// 1
ID3D11HullShader* gpID3D11HullShader = NULL; //tessalation Control Shader
ID3D11DomainShader* gpID3D11DomainShader = NULL; // Tessalation Evaluatiion

ID3D11PixelShader* gpID3D11PixelShader = NULL;

ID3D11InputLayout* gpID3D11InputLayout = NULL;

//constant buffer must be greater or equal to 16 bites
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;

// 2
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;

ID3D11Buffer* gpID3D11Buffer_VertextBuffer_Rectangle = NULL;

ID3D11Buffer* gpID3D11Buffer_VertextBuffer_Triangle = NULL;


// change 3

//XMVECTOR == 4 array then
//XMVECTOR.0 = numberOfSegaments
//XMVECTOR.1 = numberOfStrips

struct CBUFFER_HullShader
{
    XMVECTOR Hull_Constant_Function_Params;
};

struct CBUFFER_DomainShader
{
    XMMATRIX WorldViewProjectionmatrix; //Model View Matrix
};

struct CBUFER_PixelShader
{
    XMVECTOR LineColor;
};

XMMATRIX gPerpectiveGraphicProjectionMatrix;

// change 4
unsigned int guiNumberOfLineSegament = 1;


HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;

int x, y;

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
        TEXT("Darshan's : Direct3D11 Domain and Hull Shader...!"),
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

        case VK_UP:


            guiNumberOfLineSegament++;
            if (guiNumberOfLineSegament >= 30)
            {
                guiNumberOfLineSegament = 30;
            }

            break;
        case VK_DOWN:
            guiNumberOfLineSegament--;
            if (guiNumberOfLineSegament <= 0)
            {
                guiNumberOfLineSegament = 1;
            }
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


   /* Vertex Sader */
    //HLSL(High Level Shading language ) == GLSL()
    const char* vertexShaderSourceCode =
        "struct VertexOutPut"\
        "{"\
        "float4 position : POSITION;"\
        "};"\

        //float4 == vec4
        //SV_POSITION == VPosition
        //SV_POSITION indicates that return value is equal to inbuild shader value.
        //pos : position -->> aantarpath aaahe he  
        "VertexOutPut main(float2 pos : POSITION)"\
        "{"\
        "VertexOutPut outPut;"\

        "outPut.position = float4(pos, 0.0f, 1.0f);"\
        
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
        &gpID3D11VertexShader);
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

    gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, 0, 0);

    

    //Hull Shader == Tessalation Control shader
    const char* HullShaderSourceCode =
        "cbuffer ConstantBuffer"\
        "{"\
        "float4 Hull_Constant_Function_Params;"\
        "}"\
        
        "struct VertexOutPut"\
        "{"\
        "float4 position : POSITION;"\
        "};"\

        "struct Hull_Constant_outPut"\
        "{"\
        "float edges[2] : SV_TESSFACTOR;"\
        "};"\

        "Hull_Constant_outPut Hull_Constant_Fuction(void)"\
        "{"\
        "Hull_Constant_outPut output;"\

        "float numberoflineStrip = Hull_Constant_Function_Params[0];"\
        "float numberOfSegaments = Hull_Constant_Function_Params[1];"\
        
        "output.edges[0] = numberoflineStrip;"\
        "output.edges[1] = numberOfSegaments;"\
        
        "return(output);"\
        "}"\

        "struct Hull_OutPut"\
        "{"\
        "float4 position : POSITION;"\
        "};"\
        
        "[domain(\"isoline\")]"\
        "[partitioning(\"integer\")]"\
        "[outputtopology(\"line\")]"\
        "[outputcontrolpoints(4)]"\
        "[patchconstantfunc(\"Hull_Constant_Fuction\")]"\
        
        "Hull_OutPut main(InputPatch < VertexOutPut, 4 > input_patch, uint i:SV_OUTPUTCONTROLPOINTID)"\
        "{"\
        "Hull_OutPut output;"\

        "output.position = input_patch[i].position;"\
        
        "return(output);"\
        "}";

    ID3DBlob* pID3DBlob_HullShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(HullShaderSourceCode,//code
        lstrlenA(HullShaderSourceCode) + 1,//Length
        "HS",//VErtexShader Compiler Identifier
        NULL,//
        D3D_COMPILE_STANDARD_FILE_INCLUDE,//
        "main",//
        "hs_5_0",//version 
        0,//
        0,//
        &pID3DBlob_HullShaderCode,//
        &pID3DBlob_Error);//

    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile_DM, gszLogFileName, "a+");
            fprintf_s(gpFile_DM, "D3D11Compile() Failed. For Hull Shader : %s.\n", (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile_DM);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Compile() Succeeded For Hull Shader.\n");
        fclose(gpFile_DM);

    }

    //CreatePixelShader Here 
    hr = gpID3D11Device->CreateHullShader(pID3DBlob_HullShaderCode->GetBufferPointer(),
        pID3DBlob_HullShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11HullShader);
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreateHullShader() Failed.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreateHullShader() Succeedded.\n");
        fclose(gpFile_DM);
    }

    gpID3D11DeviceContext->HSSetShader(gpID3D11HullShader, 0, 0);
    pID3DBlob_HullShaderCode->Release();
    pID3DBlob_HullShaderCode = NULL;


    //Domain Shader == Tessalation Evaluation shader
    const char* DomainShaderSourceCode =
        "cbuffer ConstantBuffer"\
        "{"\
        "float4x4 worldViewProjectionMatrix;"\
        "}"\

        "struct Hull_OutPut"\
        "{"\
        "float4 position : POSITION;"\
        "};"\

        "struct Hull_Constant_outPut"\
        "{"\
        "float edges[2] : SV_TESSFACTOR;"\
        "};"\


        "struct Domain_OutPut"\
        "{"\
        "float4 position : SV_POSITION;"\
        "};"\

        "[domain(\"isoline\")]"\



        "Domain_OutPut main(Hull_Constant_outPut input,OutputPatch < Hull_OutPut, 4 > OutPut_patch, float2 tess_Coord:SV_DOMAINLOCATION)"\
        "{"\
        "Domain_OutPut output;"\
        "float3 p0 = OutPut_patch[0].position.xyz;\n"\
        "float3 p1 = OutPut_patch[1].position.xyz;\n"\
        "float3 p2 = OutPut_patch[2].position.xyz;\n"\
        "float3 p3 = OutPut_patch[3].position.xyz;\n"\

        "float3 p = p0 * (1.0 - tess_Coord.x) * (1.0 - tess_Coord.x) * (1.0 - tess_Coord.x) + " \
               "p1 * (3.0 * tess_Coord.x) * (1.0 - tess_Coord.x) * (1.0 - tess_Coord.x) + " \
               "p2 * (3.0 * tess_Coord.x * tess_Coord.x) * (1.0 - tess_Coord.x) + " \
               "p3 * (tess_Coord.x * tess_Coord.x * tess_Coord.x);"

        "float4 position = float4(p, 1.0f);"\
        "output.position = mul(worldViewProjectionMatrix, position);"\

        "return(output);"\
        "}";

    ID3DBlob* pID3DBlob_DomainShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(DomainShaderSourceCode,//code
        lstrlenA(DomainShaderSourceCode) + 1,//Length
        "DS",//VErtexShader Compiler Identifier
        NULL,//
        D3D_COMPILE_STANDARD_FILE_INCLUDE,//
        "main",//
        "ds_5_0",//version 
        0,//
        0,//
        &pID3DBlob_DomainShaderCode,//
        &pID3DBlob_Error);//

    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile_DM, gszLogFileName, "a+");
            fprintf_s(gpFile_DM, "D3D11Compile() Failed. For Domain Shader : %s.\n", (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile_DM);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Compile() Succeeded For Domain Shader.\n");
        fclose(gpFile_DM);

    }

    //CreatePixelShader Here 
    hr = gpID3D11Device->CreateDomainShader(pID3DBlob_DomainShaderCode->GetBufferPointer(),
        pID3DBlob_DomainShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11DomainShader);
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreateDomainShader() Failed.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "D3D11Devices::CreateHullShader() Succeedded.\n");
        fclose(gpFile_DM);
    }

    gpID3D11DeviceContext->DSSetShader(gpID3D11DomainShader, 0, 0);
    pID3DBlob_DomainShaderCode->Release();
    pID3DBlob_DomainShaderCode = NULL;


    //Pixel Shader == Fragment shader 
    const char* PixelShaderSourceCode =
        "cbuffer ConstantBuffer"\
        "{"\
        "float4 lineColor;"\
        "}"\

        "float4 main(void) : SV_TARGET"\
        "{"\
        "float4 color;"\
        "color = lineColor;"\
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
        &gpID3D11PixelShader);
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

    gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, 0, 0);
    pID3DBlob_PixelShaderCode->Release();
    pID3DBlob_PixelShaderCode = NULL;


    //create and set input layout
    D3D11_INPUT_ELEMENT_DESC inputElementDesc;
    inputElementDesc.SemanticName = "POSITION";
    inputElementDesc.SemanticIndex = 0;
    inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElementDesc.InputSlot = 0;
    inputElementDesc.AlignedByteOffset = 0;
    inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputElementDesc.InstanceDataStepRate = 0;


    hr = gpID3D11Device->CreateInputLayout(&inputElementDesc,
        1,
        pID3DBlob_VartexShaderCode->GetBufferPointer(),
        pID3DBlob_VartexShaderCode->GetBufferSize(),
        &gpID3D11InputLayout);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateInputLayout() Failed. \n");
        fclose(gpFile_DM);
        if (pID3DBlob_VartexShaderCode)
        {
            pID3DBlob_VartexShaderCode->Release();
            pID3DBlob_VartexShaderCode = NULL;

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
    }

    gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

    //this vertices are taken as clock vise but openGL use anti clock vise
    float vertices_Triangle[] =
    {
        -1.0f, -1.0f,

        -0.5f, 1.0f,
        
        0.5f, -1.0f,
        
        1.0f, 1.0f
    };


    D3D11_BUFFER_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(vertices_Triangle);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc,
        NULL,
        &gpID3D11Buffer_VertextBuffer_Triangle);
    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed for Vertex Buffer. \n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded for Vertex Buffer. \n");
        fclose(gpFile_DM);
    }

    //Copy vertices into above created buffer
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    gpID3D11DeviceContext->Map(gpID3D11Buffer_VertextBuffer_Triangle,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource);

    memcpy(mappedSubresource.pData,
        vertices_Triangle,
        sizeof(vertices_Triangle));

    gpID3D11DeviceContext->Unmap(gpID3D11Buffer_VertextBuffer_Triangle, 0);



    //define and set the contex buffer
    D3D11_BUFFER_DESC bufferDesc_ContentBuffer;
    
    ZeroMemory(&bufferDesc_ContentBuffer,
        sizeof(D3D11_BUFFER_DESC));
    bufferDesc_ContentBuffer.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc_ContentBuffer.ByteWidth = sizeof(CBUFFER_HullShader);
    bufferDesc_ContentBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = gpID3D11Device->CreateBuffer(&bufferDesc_ContentBuffer,
        nullptr,
        &gpID3D11Buffer_ConstantBuffer_HullShader);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed For Hull Constant Buffer.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded For Hull Constant Buffer. \n");
        fclose(gpFile_DM);
    }

    gpID3D11DeviceContext->HSSetConstantBuffers(0,
        1,
        &gpID3D11Buffer_ConstantBuffer_HullShader);



    ZeroMemory(&bufferDesc_ContentBuffer,
        sizeof(D3D11_BUFFER_DESC));
    bufferDesc_ContentBuffer.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc_ContentBuffer.ByteWidth = sizeof(CBUFFER_DomainShader);
    bufferDesc_ContentBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = gpID3D11Device->CreateBuffer(&bufferDesc_ContentBuffer,
        nullptr,
        &gpID3D11Buffer_ConstantBuffer_DomainShader);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed For Domain Constant Buffer.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded For Domain Constant Buffer. \n");
        fclose(gpFile_DM);
    }

    gpID3D11DeviceContext->DSSetConstantBuffers(0,
        1,
        &gpID3D11Buffer_ConstantBuffer_DomainShader);




    ZeroMemory(&bufferDesc_ContentBuffer,
        sizeof(D3D11_BUFFER_DESC));
    bufferDesc_ContentBuffer.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc_ContentBuffer.ByteWidth = sizeof(CBUFER_PixelShader);
    bufferDesc_ContentBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = gpID3D11Device->CreateBuffer(&bufferDesc_ContentBuffer,
        nullptr,
        &gpID3D11Buffer_ConstantBuffer_PixelShader);

    if (FAILED(hr))
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Failed For pixel Constant Buffer.\n");
        fclose(gpFile_DM);
        return(hr);
    }
    else
    {
        fopen_s(&gpFile_DM, gszLogFileName, "a+");
        fprintf_s(gpFile_DM, "ID3D11Device::CreateBuffer() Succeeded For Pixel Constant Buffer. \n");
        fclose(gpFile_DM);
    }

    gpID3D11DeviceContext->PSSetConstantBuffers(0,
        1,
        &gpID3D11Buffer_ConstantBuffer_PixelShader);


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

    gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);

    D3D11_VIEWPORT d3dViewPort;
    d3dViewPort.TopLeftX = 0;
    d3dViewPort.TopLeftY = 0;
    d3dViewPort.Width = (float)width;
    d3dViewPort.Height = (float)height;
    d3dViewPort.MinDepth = 0.0f;
    d3dViewPort.MaxDepth = 1.0f;

    //Risterisation state
    gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);


    gPerpectiveGraphicProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(44.0f), (float)width / (float)height, 0.1f, 100.0f);


    return(hr);
}

void Display()
{
    //code
    //clear render target view to a chosen color
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);

    //select which vertex buffer to display
    UINT stride = sizeof(float) * 2;
    UINT offset = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(0,
        1,
        &gpID3D11Buffer_VertextBuffer_Triangle,
        &stride,
        &offset);

    //select geometry primtive
    gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

    //translation is concerned with world matrix transformation
    XMMATRIX worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 6.0f);
    XMMATRIX translate = XMMatrixIdentity();
    XMMATRIX viewMatrix = XMMatrixIdentity();

    XMMATRIX wvpmatrix = worldMatrix * viewMatrix * gPerpectiveGraphicProjectionMatrix;

    //load the data  into  the  constant buffer
    CBUFFER_DomainShader constantBuffer_DomainShader;

    constantBuffer_DomainShader.WorldViewProjectionmatrix = wvpmatrix;

    //updateSubresource == glUniformMatrix4fv from Opengl
    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_DomainShader,
        0,
        NULL,
        &constantBuffer_DomainShader,
        0,
        0);

    //load the data  into  the  constant buffer
    CBUFFER_HullShader constantBuffer_HullShader;

    constantBuffer_HullShader.Hull_Constant_Function_Params = XMVectorSet(1.0f, (FLOAT)guiNumberOfLineSegament, 0.0f, 1.0f);

    //updateSubresource == glUniformMatrix4fv from Opengl
    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_HullShader,
        0,
        NULL,
        &constantBuffer_HullShader,
        0,
        0);

    //load the data  into  the  constant buffer
    CBUFER_PixelShader constantBuffer_PixelShader;

    constantBuffer_PixelShader.LineColor = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);

    //updateSubresource == glUniformMatrix4fv from Opengl
    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_PixelShader,
        0,
        NULL,
        &constantBuffer_PixelShader,
        0,
        0);

    gpID3D11DeviceContext->Draw(4, 0);

    gpIDXGISwapChain->Present(0, 0);
}

void unInitialize()
{
    //code

    if (gpID3D11Buffer_ConstantBuffer_PixelShader)
    {
        gpID3D11Buffer_ConstantBuffer_PixelShader->Release();
        gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;
    }
    if (gpID3D11Buffer_ConstantBuffer_DomainShader)
    {
        gpID3D11Buffer_ConstantBuffer_DomainShader->Release();
        gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;
    }
    if (gpID3D11Buffer_ConstantBuffer_HullShader)
    {
        gpID3D11Buffer_ConstantBuffer_HullShader->Release();
        gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
    }
    
    if (gpID3D11InputLayout)
    {
        gpID3D11InputLayout->Release();
        gpID3D11InputLayout = NULL;
    }
    if (gpID3D11Buffer_VertextBuffer_Rectangle)
    {
        gpID3D11Buffer_VertextBuffer_Rectangle->Release();
        gpID3D11Buffer_VertextBuffer_Rectangle = NULL;
    }
    if (gpID3D11PixelShader)
    {
        gpID3D11PixelShader->Release();
        gpID3D11PixelShader = NULL;
    }
    if (gpID3D11VertexShader)
    {
        gpID3D11VertexShader->Release();
        gpID3D11VertexShader = NULL;
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

