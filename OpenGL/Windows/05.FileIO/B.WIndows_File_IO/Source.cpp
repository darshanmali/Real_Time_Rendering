#include<windows.h>
#include<stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//File pointer Declaration
FILE* gpFile = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpszCmdLine, int iCmdShow)
{
    WNDCLASSEX wndclassex;
    HWND hwnd;
    MSG msg;
    TCHAR Appname[] = TEXT("MY_File_IO");
       
    if (fopen_s(&gpFile, "DVM_Log.txt", "w") != 0)
    {
        MessageBox(NULL, TEXT("Can Not Create Desired File !"), TEXT("ERROR."), MB_OK);
        exit(0);
    }

    wndclassex.cbSize = sizeof(WNDCLASSEX);
    wndclassex.cbClsExtra = 0;
    wndclassex.cbWndExtra = 0;
    wndclassex.style = CS_HREDRAW | CS_VREDRAW;
    wndclassex.lpszClassName = Appname;
    wndclassex.lpfnWndProc = WndProc;
    wndclassex.lpszMenuName = NULL;
    wndclassex.hInstance = hInstance;
    wndclassex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclassex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclassex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);


    RegisterClassEx(&wndclassex);


    hwnd = CreateWindow(Appname,
        TEXT("ICon Setting !"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    fclose(gpFile);
    gpFile = NULL;
    return(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

    switch (iMsg)
    {
    case WM_CREATE:
        fprintf_s(gpFile, "India Is My country!\n");
        break;
    case WM_DESTROY:
        fprintf(gpFile, "Jay Hind !\n");
        PostQuitMessage(0);
        break;
    }

    return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}