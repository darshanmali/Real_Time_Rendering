/*

    3D Model sphere diffuse Lights

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

ID3D11InputLayout* gpID3D11InputLayout = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;

//Shaders variables
ID3D11VertexShader* gpID3D11VertexShader = NULL;
ID3D11PixelShader* gpID3D11PixelShader = NULL;

// 2D Triangle 

//Cube Model Variables  

ID3D11Buffer* gpID3D11_Sphere_VertextBuffer_Position = NULL;
ID3D11Buffer* gpID3D11_Sphere_VertextBuffer_Normals = NULL;
ID3D11Buffer* gpID3D11_Sphere_Index_Buffer = NULL;


struct CBUFFER
{
    XMMATRIX WorldMatrix; //Model View Matrix
    XMMATRIX ViewMatrix; //View Matrix (camera view)
    XMMATRIX ProjectionMatrix; //Projection Matrix

    XMVECTOR LA;
    XMVECTOR LD;
    XMVECTOR LS;
    XMVECTOR Light_Position;

    XMVECTOR KA;
    XMVECTOR KD;
    XMVECTOR KS;
    float material_shininess;

    unsigned int Key_Pressed;
};

float lAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lDefues[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lPosition[] = { 100.0f, 100.0f, -100.0f, 1.0f };

float material_Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float material_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float material_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float Shininess = 50.0f;


//light
BOOL gbLight = FALSE;

//backface culling 
ID3D11RasterizerState* Rasterization_State;

float angle_Cube = 0.0f;

//Depth 
ID3D11DepthStencilView* gpiD3D11DepthStaincilView = NULL;

//Sphere
/*New For Sphere*/
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

unsigned int gNumVertices;
unsigned int gNumElements;

XMMATRIX gPerpectiveGraphicProjectionMatrix;

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
        TEXT("Darshan's : Direct X 3D sphere Lights...!"),
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


    //HLSL(High Level Shading language ) == GLSL()
    const char* vertexShaderSourceCode =
        "cbuffer ConstantBuffer"\
        "{"\
        "float4x4 worldMatrix;"\
        "float4x4 viewMatrix;"\
        "float4x4 ProjectionMatrix;"\

        "float4 la;"\
        "float4 ld;"\
        "float4 ls;"\
        "float4 lightPosition;"\

        "float4 ka;"\
        "float4 kd;"\
        "float4 ks;"\
        "float material_shininess;"\
        "uint keyPressed;"\
        "}"\

        "struct VertexOutPut"\
        "{"\
        "float4 position       : SV_POSITION;"\
        "float4 fong_ads_light : COLOR;"\
        "};"\

        "VertexOutPut main(float4 pos : POSITION, float3 normal : NORMAL)"\
        "{"\

        "VertexOutPut outPut;"\

        "if(keyPressed == 1)"\
        "{"\
        "float4 eye_Position = mul(worldMatrix, pos);"\
        "float4 eye_coordinates = mul(viewMatrix, eye_Position);"\
        "float3 transformed_Normal = (float3)normalize(mul((float3x3)worldMatrix, (float3)normal));"\
        "float3 light_direction = (float3)normalize(lightPosition - eye_coordinates);"\

        "float4 ambient = la * ka;\n"\

        "float4 diffuse = ld * kd * max(dot(light_direction, transformed_Normal), 0.0);\n"\

        "float3 reflection_vector = reflect(-light_direction, transformed_Normal);\n"\
        "float3 viewer_vector = normalize(-eye_coordinates.xyz);\n"\
        "float4 specular = ls * ks * pow(max(dot(reflection_vector, viewer_vector), 0.0), material_shininess);\n"\


        "outPut.fong_ads_light = ambient + diffuse + specular;"\
        "}"\

        "else"\
        "{"\
        "outPut.fong_ads_light = float4(1.0f, 1.0f, 1.0f, 1.0f);"\
        "}"\

        "float4 position = mul(worldMatrix, pos);"\
        "position = mul(viewMatrix, position);"\
        "position = mul(ProjectionMatrix, position);"

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

    //Pixel Shader == Fragment shader 
    const char* PixelShaderSourceCode =
        "struct VertexOutPut"\
        "{"\
        "float4 position      : SV_POSITION;"\
        "float4 fong_ads_light : COLOR;"\
        "};"\

        "float4 main(VertexOutPut input) : SV_TARGET"\
        "{"\

        "float4 color = input.fong_ads_light;"\
        
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

    gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

    //this Sphere are taken as clock vise but openGL use anti clock vise

    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    gNumVertices = getNumberOfSphereVertices();
    gNumElements = getNumberOfSphereElements();


    // ************************Sphere Buffer Vertex Creation 

    D3D11_BUFFER_DESC bufferDesc;

    //Vertex buffer Creation
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


    //Normal buffer Creation
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

    // Index Bufffer Creation
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

    gpID3D11DeviceContext->VSSetConstantBuffers(0,
        1,
        &gpID3D11Buffer_ConstantBuffer);

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

    //translation is concerned with world matrix transformation
    XMMATRIX translation_matrix = XMMatrixTranslation(0.0f, 0.0f, 3.0f);
    XMMATRIX rotation_matrix_y = XMMatrixRotationY(angle_Cube);
    XMMATRIX worldMatrix = rotation_matrix_y * translation_matrix;
    XMMATRIX translate = XMMatrixIdentity();
    XMMATRIX viewMatrix = XMMatrixIdentity();

    //load the data  into  the  constant buffer
    CBUFFER constantBuffer;
    ZeroMemory(&constantBuffer, sizeof(CBUFFER));

    constantBuffer.WorldMatrix = worldMatrix;
    constantBuffer.ViewMatrix = viewMatrix;
    constantBuffer.ProjectionMatrix = gPerpectiveGraphicProjectionMatrix;

    if (gbLight == TRUE)
    {
        constantBuffer.Key_Pressed = 1;
        constantBuffer.LA = XMVectorSet(lAmbient[0], lAmbient[1], lAmbient[2], lAmbient[3]);
        constantBuffer.LD = XMVectorSet(lDefues[0], lDefues[1], lDefues[2], lDefues[3]);
        constantBuffer.LS = XMVectorSet(lSpecular[0], lSpecular[1], lSpecular[2], lSpecular[3]);
        constantBuffer.Light_Position = XMVectorSet(lPosition[0], lPosition[1], lPosition[2], lPosition[3]);

        constantBuffer.KA = XMVectorSet(material_Ambient[0], material_Ambient[1], material_Ambient[2], material_Ambient[3]);
        constantBuffer.KD = XMVectorSet(material_Diffuse[0], material_Diffuse[1], material_Diffuse[2], material_Diffuse[3]);
        constantBuffer.KS = XMVectorSet(material_Specular[0], material_Specular[1], material_Specular[2], material_Specular[3]);
        constantBuffer.material_shininess = Shininess;
    }
    else
    {
        constantBuffer.Key_Pressed = 0;
    }


    //updateSubresource == glUniformMatrix4fv from Opengl
    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer,
        0,
        NULL,
        &constantBuffer,
        0,
        0);


    gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

    gpIDXGISwapChain->Present(0, 0);

    update();

}

void update(void)
{
    angle_Cube = angle_Cube + 0.0005f;
    if (angle_Cube > 360.0f)
    {
        angle_Cube = 0;
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

    if (gpID3D11_Sphere_Index_Buffer)
    {
        gpID3D11_Sphere_Index_Buffer->Release();
        gpID3D11_Sphere_Index_Buffer = NULL;
    }

    if (gpID3D11_Sphere_VertextBuffer_Normals)
    {
        gpID3D11_Sphere_VertextBuffer_Normals->Release();
        gpID3D11_Sphere_VertextBuffer_Normals = NULL;
    }

    if (gpID3D11_Sphere_VertextBuffer_Position)
    {
        gpID3D11_Sphere_VertextBuffer_Position->Release();
        gpID3D11_Sphere_VertextBuffer_Position = NULL;
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
