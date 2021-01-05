#include<windows.h>
#include"Header.h"
#include<gl/GL.h>
#include<stdio.h>
#include<gl/GLU.h>

#define _USE_MATH_DEFINES
#include<math.h>

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Glu32.lib")

#define WIN_WIDTH_DM 800
#define WIN_HEIGHT_DM 600

//Global Fuction 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Global Variables Declaration 
DWORD dwStyle_D;
WINDOWPLACEMENT wpPrev_DM = { sizeof(WINDOWPLACEMENT) };
bool gdFullScreen_DM = false;
HWND ghwnd_DM = NULL;
bool gbActiveWindows_DM = false;
HDC ghdc_DM = NULL;
HGLRC ghrc_DM = NULL;
FILE* gpFile_DM = NULL;
GLfloat angle = 0.0f;
GLfloat x = 0.0f;
GLfloat y = 10.0f;

GLUquadric* quadric = NULL;

//Land And Sky Globle Variables
GLuint Tree_Texture;
GLuint Leaf_Texture;
GLuint Sky_Texture;
GLuint Grass_Texture;


//House Globle Variables
GLuint Wood_Texture;
GLuint Snow_Texture;
GLuint Boarder_Texture;
GLuint Door_Texture;
GLuint Steel_Texture;
GLuint Glass_Texture;
GLuint Tree_Glass_Texture;
GLfloat Door_angle = 0.0f;

//Bushes
GLuint Light_Green;
GLuint Dim_Green;
GLuint Dark_Green;

//Tree
GLuint tree_Trunk;
GLuint tree_Light_Green;
GLuint tree_Dim_Green;
GLuint tree_Dark_Green;

//Girl
//**************One********Texture Variables
GLuint girl_Texture;
GLuint girl_Face_Texture;
GLuint Girls_Foots_Texture;
GLuint Girls_Foot_Down_Texture;
GLuint Girls_Nose_Texture;

//*************Two*************Transformation variables
int sholder_Left = 180;
int Lbow_Left = 355;

int sholder_Right = 180;
int Lbow_Right = 350;

int Thigh_Right = 180;
int Calf_Right = 5;

int Thigh_Left = 180;
int Calf_Left = 5;

GLfloat Model_Y = 0;

GLfloat eye_x = 0.0f;
GLfloat eye_y = 0.0f;
GLfloat eye_z = 2.0f;

//Santa
GLuint Santa_Texture;
GLuint Santa_Foots_Texture;
GLuint Santa_Face_Texture;

//Girl
GLuint Bird_Texture;
GLuint Bird_Bill_Texture;
GLuint Nest_Texture;

//Snow
int rand_val[10];
int rand_val_2[10];
int rand_val_3[20];


HBITMAP hBitmap;
BITMAP bmp;

//Globle Function 
void Resize(int, int);
void unInitialize(void);
void Display(void);
//Function
void ToggelFullScreen(void);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpszCmdLine, int iCmdShow)
{
    //Variable Declaration
    WNDCLASSEX wndclassex;
    HWND hwnd;
    MSG msg;
    TCHAR Appname[] = TEXT("MY_SAMPLE");
    bool bDone = false;
    INT x = 0, y = 0;
    INT Width = 800;
    INT Height = 600;
    MONITORINFO mi_D = { sizeof(MONITORINFO) };
    
    //Function Declaration
    void Initialize(void);
    
    if (fopen_s(&gpFile_DM, "log.txt", "w"))
    {
        MessageBox(NULL, TEXT(" File Can Not Created!"), TEXT("Error"), MB_OK);
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
        TEXT("My Christmas !"),
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
    


    switch (iMsg)
    {

    case WM_PAINT:
        
        if (!gdFullScreen_DM)
        {
            ToggelFullScreen();
            gdFullScreen_DM = true;
        }
        
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
        case VK_UP:
            x += 1.0f;
            break;

        case VK_DOWN:
            x -= 1.0f;
            break;

        case VK_LEFT:
            y += 1.0f;
            break;

        case VK_RIGHT:
            y -= 1.0f;
            break;

        case VK_ESCAPE:
            DestroyWindow(hwnd);
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

    dwStyle_D = GetWindowLong(ghwnd_DM, GWL_STYLE);
    if (GetWindowPlacement(ghwnd_DM, &wpPrev_DM) && GetMonitorInfo(MonitorFromWindow(ghwnd_DM, MONITORINFOF_PRIMARY), &mi_D))
    {
        SetWindowLong(ghwnd_DM, GWL_STYLE, (dwStyle_D & ~WS_OVERLAPPEDWINDOW));
        SetWindowPos(ghwnd_DM, HWND_TOP, mi_D.rcMonitor.left, mi_D.rcMonitor.top, mi_D.rcMonitor.right - mi_D.rcMonitor.left, mi_D.rcMonitor.bottom - mi_D.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // SWP_FRAMECHANGED = WM_NCCALCSIZE      
    }

    ShowCursor(FALSE);

}

void Initialize()
{
    PIXELFORMATDESCRIPTOR pfd_DM;
    int iPixelFormatIndex_DM;


    bool LoadGLTexture(GLuint*, TCHAR[]);


    ghdc_DM = GetDC(ghwnd_DM);

    ZeroMemory(&pfd_DM, sizeof(PIXELFORMATDESCRIPTOR));

    pfd_DM.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd_DM.nVersion = 1;
    pfd_DM.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd_DM.iPixelType = PFD_TYPE_RGBA;
    pfd_DM.cColorBits = 32;
   
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


    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    //ToggelFullScreen();

    //**************************************Part One *****************************************
    //Sky
    LoadGLTexture(&Sky_Texture, MAKEINTRESOURCE(MYSKY));

    //Land
    LoadGLTexture(&Grass_Texture, MAKEINTRESOURCE(MYGRASS));
    
    //Tree
    LoadGLTexture(&Tree_Texture, MAKEINTRESOURCE(MYTREE));
    LoadGLTexture(&Leaf_Texture, MAKEINTRESOURCE(MYTREE_LEAF));

    
    //**************************************Part Two *****************************************
    //House
    LoadGLTexture(&Wood_Texture, MAKEINTRESOURCE(MYWOOD));
    LoadGLTexture(&Snow_Texture, MAKEINTRESOURCE(MYSNOW));
    LoadGLTexture(&Boarder_Texture, MAKEINTRESOURCE(MYBOARDER));
    LoadGLTexture(&Door_Texture, MAKEINTRESOURCE(MYDOOR));
    LoadGLTexture(&Steel_Texture, MAKEINTRESOURCE(MYSTEEL));
    LoadGLTexture(&Glass_Texture, MAKEINTRESOURCE(MYGLASS));
    LoadGLTexture(&Tree_Glass_Texture, MAKEINTRESOURCE(MYTREEGLASS));

    //Bushes
    LoadGLTexture(&Light_Green, MAKEINTRESOURCE(GREEN_LIGHT));
    LoadGLTexture(&Dim_Green, MAKEINTRESOURCE(GREEN_DIM));
    LoadGLTexture(&Dark_Green, MAKEINTRESOURCE(GREEN_DARK));

    //tree
    LoadGLTexture(&tree_Light_Green, MAKEINTRESOURCE(TREE_GREEN_LIGHT));
    LoadGLTexture(&tree_Dim_Green, MAKEINTRESOURCE(TREE_GREEN_DIM));
    LoadGLTexture(&tree_Dark_Green, MAKEINTRESOURCE(TREE_GREEN_DARK));
    LoadGLTexture(&tree_Trunk, MAKEINTRESOURCE(TREE_TRUNK));

    //**************************************Part Three *****************************************
    //Girl
    LoadGLTexture(&girl_Texture, MAKEINTRESOURCE(MYCLOTHS));
    LoadGLTexture(&Girls_Foots_Texture, MAKEINTRESOURCE(MYFOOTS));
    LoadGLTexture(&Girls_Foot_Down_Texture, MAKEINTRESOURCE(MYSHOOSE));
    LoadGLTexture(&girl_Face_Texture, MAKEINTRESOURCE(MYFACE));
    LoadGLTexture(&Girls_Nose_Texture, MAKEINTRESOURCE(MYNOSE));

    //**************************************Part Four *****************************************
    //Santa
    LoadGLTexture(&Santa_Texture, MAKEINTRESOURCE(MYSANTA));
    LoadGLTexture(&Santa_Foots_Texture, MAKEINTRESOURCE(MYSANTAFOOT));
    LoadGLTexture(&Santa_Face_Texture, MAKEINTRESOURCE(MYSANTAFACE));

    //**************************************Part Five *****************************************
    //Birld
    LoadGLTexture(&Bird_Texture, MAKEINTRESOURCE(MYBIRD));
    LoadGLTexture(&Bird_Bill_Texture, MAKEINTRESOURCE(MYBILL));
    LoadGLTexture(&Nest_Texture, MAKEINTRESOURCE(MYNEST));

    //**************************************Part Six *****************************************
    //Snow
    
    int j = 0;
    srand(2);
    while (j < 10)
    {
        rand_val[j] = 1 - rand() % 20;
        j++;
    }

    srand(4);
    j = 0;
    while (j < 10)
    {
        rand_val_2[j] = 2 - rand() % 30;
        j++;
    }

    srand(6);
    j = 0;
    while (j < 20)
    {
        rand_val_3[j] = 2 - rand() % 20;
        j++;
    }

    //Rand Function for Snow

    /*
    int j = 0;
    srand(2);
    while (j < 10)
    {
        rand_val[j] = 1 - rand() % 20;
        j++;
    }

    srand(4);
    j = 0;
    while (j < 10)
    {
        rand_val_2[j] = 2 - rand() % 30;
        j++;
    }

    srand(6);
    j = 0;
    while (j < 10)
    {
        rand_val_3[j] = 2 - rand() % 10;
        j++;
    }
    */
    Resize(WIN_WIDTH_DM, WIN_HEIGHT_DM);
}

bool LoadGLTexture(GLuint* Texture, TCHAR Resource_ID[])
{
    bool bResult = false;

    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), Resource_ID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hBitmap != NULL)
    {
        bResult = true;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glGenTextures(1, Texture);
        glBindTexture(GL_TEXTURE_2D, *Texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
        DeleteObject(hBitmap);
    }
    return (bResult);
}

void Resize(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(44.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


}

void Display()
{

    //Local Fuction Declaration Part 1
    void Tree(void);
    void Sky(void);
    void Land(void);

    //Local Function Declaration Part 2
    void House(void);
    void Bushes(void);
    void House_Tree(void);

    //Local Function Declaration Part 3
    void Girl(void);

    //Local Function Declaration Part 4
    void Santa(void);
    void Santa_Ferrari(void);

    //Local Function Declaration Part 5
    void Bird(void);

    //Local Function Declaration Part 6
    void Snow(void);


    //Code Began
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(x, 2.0f, y, 0.0f, -0.2f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    //glTranslatef(x, 0.0f, -y);

#pragma region Error_Checking

  /* glLineWidth(5.0f);

    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(10.0f, 0.0f, 0.0f);
    glVertex3f(-10.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 10.0f, 0.0f);
    glVertex3f(0.0f, -10.0f, 0.0f);

    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    */

#pragma endregion Error_Checking

    // Here we are dealing with Land, Sky and Tree
#pragma region Part_1
    
    glPushMatrix();

#pragma region Tree

    //Left side
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glTranslatef(-8.0f, 1.0f, 3.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, Tree_Texture);

    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

    //Function Call
    Tree();

    glPopMatrix();
    
    //Right side
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glTranslatef(10.0f, 1.0f, 4.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, Tree_Texture);

    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

    //glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

    //Function Call
    Tree();

    glPopMatrix();

#pragma endregion Tree

#pragma region Sky

    glPushMatrix();
    
    Sky();

    glPopMatrix();

#pragma endregion Sky

#pragma region Land
    
    glPushMatrix();

    Land();

    glPopMatrix();

#pragma endregion Land

    glPopMatrix();

#pragma endregion Part_1

    //House Bushes House_Tree
#pragma region Part_2
    
    glPushMatrix();

    glTranslatef(0.0f, 2.0f, -10.0f);

#pragma region House
    
    glPushMatrix();

    House();
    
    glPopMatrix();

#pragma endregion House

#pragma region Bushes


    //From House 1
    glPushMatrix();

    glTranslatef(5.0f, -2.4f, 7.0f);
    glScalef(1.5f, 1.5f, 1.5f);
    
    Bushes();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(-5.0f, -2.4f, 7.0f);

    glScalef(1.5f, 1.5f, 1.5f);
    Bushes();

    glPopMatrix();
    
    //From House 2
    glPushMatrix();

    glTranslatef(5.0f, -2.4f, 11.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(1.5f, 1.5f, 1.5f);

    Bushes();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(-5.0f, -2.4f, 11.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(1.5f, 1.5f, 1.5f);
    Bushes();

    glPopMatrix();

    //From House 3
    glPushMatrix();

    glTranslatef(5.0f, -2.4f, 15.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(1.5f, 1.5f, 1.5f);

    Bushes();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(-5.0f, -2.4f, 15.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(1.5f, 1.5f, 1.5f);
    Bushes();

    glPopMatrix();

    //From House 4
    glPushMatrix();

    glTranslatef(6.0f, -2.4f, 19.0f);
    glRotatef(200.0f, 0.0f, 1.0f, 0.0f);
    glScalef(1.5f, 1.5f, 1.5f);

    Bushes();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(-6.0f, -2.4f, 19.0f);
    glRotatef(200.0f, 0.0f, 1.0f, 0.0f);
    glScalef(1.5f, 1.5f, 1.5f);
    Bushes();

    glPopMatrix();

#pragma endregion Bushes

#pragma region House_Tree
   //This is commented bacouse of High Load On Computer ,
   //if you comment out it you will see the trees around the House.
    
    //Thank you.
    
    
    //right side
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 10; j++)
        {

            glPushMatrix();

            glTranslatef(20.0f + ((GLfloat)rand_val_2[j]), -3.0f, 10.0f + ((GLfloat)rand_val[j]));

            glTranslatef(20.0f, 0.0f, 5.0f);

            glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

            glScalef(4.0f, 4.0f, 4.0f);

            House_Tree();

            glPopMatrix();

        }

        glTranslatef(5.0f, 0.0f, 10.0f);
    }
    
    glTranslatef(-10.0f, 0.0f, -40.0f);
    //Right Back
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 10; j++)
        {

            glPushMatrix();

            glTranslatef(20.0f + ((GLfloat)rand_val_2[j]), -3.0f, 10.0f + ((GLfloat)rand_val[j]));

            glTranslatef(20.0f, 0.0f, 5.0f);

            glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

            glScalef(4.0f, 4.0f, 4.0f);

            House_Tree();

            glPopMatrix();

        }

        glTranslatef(5.0f, 0.0f, 10.0f);
    }

    glTranslatef(-30.0f, 0.0f, 20.0f);
    //Left Front
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 10; j++)
        {

            glPushMatrix();

            glTranslatef(-20.0f + ((GLfloat)rand_val_2[j]), -3.0f, -10.0f + ((GLfloat)rand_val[j]));

            glTranslatef(20.0f, 0.0f, 5.0f);

            glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

            glScalef(4.0f, 4.0f, 4.0f);

            House_Tree();

            glPopMatrix();

        }

        glTranslatef(5.0f, 0.0f, 10.0f);
    }

    glTranslatef(-20.0f, 0.0f, -50.0f);
    //Left Back
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 10; j++)
        {

            glPushMatrix();

            glTranslatef(-20.0f + ((GLfloat)rand_val_2[j]), -3.0f, -10.0f + ((GLfloat)rand_val[j]));

            glTranslatef(20.0f, 0.0f, 5.0f);

            glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

            glScalef(4.0f, 4.0f, 4.0f);

            House_Tree();

            glPopMatrix();

        }

        glTranslatef(10.0f, 0.0f, 10.0f);
    }

    glTranslatef(0.0f, 0.0f, -20.0f);
    //Behind the House
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 10; j++)
        {

            glPushMatrix();

            glTranslatef(-10.0f + ((GLfloat)rand_val_2[j]), -3.0f, -20.0f + ((GLfloat)rand_val[j]));

            glTranslatef(20.0f, 0.0f, 5.0f);

            glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

            glScalef(4.0f, 4.0f, 4.0f);

            House_Tree();

            glPopMatrix();

        }

        glTranslatef(10.0f, 0.0f, 5.0f);
    }
   
#pragma endregion House_Tree

    glPopMatrix();

#pragma endregion Part_2
    
    //Girl
#pragma region Part_3

    glPushMatrix();

    glTranslatef(0.0f, 0.2f, -2.0f);
    
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);

    Girl();

    glPopMatrix();
#pragma endregion Part_3

    //Santa
#pragma region Part_4

#pragma region santa

    glPushMatrix();

    glTranslatef(3.0f, 0.2f, 0.0f);

    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);

    Santa();

    glPopMatrix();

#pragma endregion santa

#pragma region santa_Ferrari

    glPushMatrix();

    glTranslatef(-2.0f, 0.2f, 0.0f);

    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);

    Santa_Ferrari();

    glPopMatrix();

#pragma endregion santa_Ferrari


#pragma endregion Part_4

    //Birld
#pragma region Part_5

    glPushMatrix();

    glTranslatef(-8.4f, 3.1f, 2.7f);

    Bird();

    glPopMatrix();

#pragma endregion Part_5

    //Snow
#pragma region Part_6

    glPushMatrix();

    glTranslatef(0.0f, 10.0f, -1.0f);

    //Snow();

    glPopMatrix();

#pragma endregion Part_6
    
    SwapBuffers(ghdc_DM);

}

void Snow(void)
{

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            glPushMatrix();

            if (i < 5)
            {
                glTranslatef(-(GLfloat)rand_val[i], 0.0f, -(GLfloat)rand_val[j]);
            }
            else
            {
                glTranslatef((GLfloat)rand_val[i], 0.0f, (GLfloat)rand_val[j]);
            }

            quadric = gluNewQuadric();
            gluSphere(quadric, 0.05f, 30, 30);
            glPopMatrix();

            glPushMatrix();

            if (i < 5)
            {
                glTranslatef((GLfloat)rand_val[i], 0.0f, -(GLfloat)rand_val[j]);
            }
            else
            {
                glTranslatef(-(GLfloat)rand_val[i], 0.0f, (GLfloat)rand_val[j]);
            }

            quadric = gluNewQuadric();
            gluSphere(quadric, 0.05f, 30, 30);
            glPopMatrix();
        }

    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            glPushMatrix();

            if (i < 5)
            {
                glTranslatef(-(GLfloat)rand_val_2[i], 0.0f, (GLfloat)1 - rand_val_2[j]);
            }
            else
            {
                glTranslatef((GLfloat)rand_val_2[i], 0.0f, -(GLfloat)1 + rand_val_2[j]);
            }

            quadric = gluNewQuadric();
            gluSphere(quadric, 0.05f, 30, 30);

            glPopMatrix();

            glPushMatrix();

            if (i < 5)
            {
                glTranslatef((GLfloat)rand_val_2[i], 0.0f, (GLfloat)1 + rand_val_2[j]);
            }
            else
            {
                glTranslatef(-(GLfloat)rand_val_2[i], 0.0f, (GLfloat)1 - rand_val_2[j]);
            }

            quadric = gluNewQuadric();
            gluSphere(quadric, 0.05f, 30, 30);

            glPopMatrix();

        }

    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            glPushMatrix();

            if (i < 5)
            {
                glTranslatef(-(GLfloat)rand_val_3[i], 0.0f, (GLfloat)2 - rand_val_3[j]);
            }
            else
            {
                glTranslatef((GLfloat)rand_val_3[i], 0.0f, -(GLfloat)2 + rand_val_3[j]);
            }

            quadric = gluNewQuadric();
            gluSphere(quadric, 0.05f, 30, 30);

            glPopMatrix();

            glPushMatrix();

            if (i < 5)
            {
                glTranslatef(-(GLfloat)rand_val_3[i], 0.0f, -(GLfloat)3 + rand_val_3[j]);
            }
            else
            {
                glTranslatef((GLfloat)rand_val_3[i], 0.0f, (GLfloat)3 - rand_val_3[j]);
            }

            quadric = gluNewQuadric();
            gluSphere(quadric, 0.05f, 30, 30);

            glPopMatrix();

            glPushMatrix();

            if (i < 8)
            {
                glTranslatef(-(GLfloat)rand_val_3[i], 0.0f, (GLfloat)2 + rand_val_3[j]);
            }
            else
            {
                glTranslatef((GLfloat)rand_val_3[i], 0.0f, -(GLfloat)3 - rand_val_3[j]);
            }

            quadric = gluNewQuadric();
            gluSphere(quadric, 0.05f, 30, 30);

            glPopMatrix();

        }

    }
}

void Bird(void)
{

#pragma region Start

    static GLfloat D = 0;
    //Body Begans
    //glTranslatef(0.0f, (GLfloat)0.05f * sinf(D), -3.0f);
    //glRotatef(angle, 0.0f, 1.0f, 0.0f);
     //glColor3f(0.7f, 1.0f, 1.0f);
     //glRotatef(10, 0.0f, 0.0f, 1.0f);


    glBegin(GL_LINES);

    glVertex3f(2.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 2.0f, 0.0f);
    glVertex3f(.0f, -2.0f, 0.0f);

    glEnd();

    //Bird head
#pragma region Bird_Head

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Bird_Texture);

    glTranslatef(0.7f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 30, 30);

    glPushMatrix();

    //Eyes Texture

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Bird_Bill_Texture);

    glTranslatef(0.0f, 0.0f, 0.1f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 30, 30);

    glTranslatef(0.0f, 0.0f, -0.2f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 30, 30);


    glPopMatrix();


    //Bill
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Bird_Bill_Texture);

    glTranslatef(0.08f, -0.02f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.03f, 0.0f, 0.1f, 10, 10);

    glPopMatrix();

    glPopMatrix();
#pragma endregion Bird_Head

    //Middle Body
#pragma region Middle_Body_Part
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Bird_Texture);
    glTranslatef(0.4f, 0.0f, 0.0f);
    glScalef(0.8f, 0.5f, 0.5f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.3f, 30, 30);

    glPopMatrix();
#pragma endregion Middle_Body_Part

    //Tail - (Puch(Sheput))
#pragma region BirdTail

    glPushMatrix();
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f);

    glPushMatrix();

    glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.1f);
    glScalef(2.0f, 0.2f, 0.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.05f);
    glScalef(2.0f, 0.2f, 0.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(2.0f, 0.2f, 0.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -0.05f);
    glScalef(2.0f, 0.2f, 0.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glRotatef(350.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -0.1f);
    glScalef(2.0f, 0.2f, 0.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1, 10, 10);

    glPopMatrix();

    glPopMatrix();
#pragma endregion BirdTail

    //Bird Leg
#pragma region Bird_Legs

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Bird_Bill_Texture);

    glTranslatef(0.4f, -0.1f, 0.05f);

    

    glRotatef(90, 1.0f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.01f, 0.01f, 0.2f, 10, 10);


    glPopMatrix();


    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Bird_Bill_Texture);

    glTranslatef(0.4f, -0.1f, -0.05f);

    glRotatef(90, 1.0f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.01f, 0.01f, 0.2f, 10, 10);

    glPopMatrix();
#pragma endregion Bird_Legs
   
#pragma region Bird_Nest

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Nest_Texture);
    //glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glTranslatef(0.7f, -0.2f, 0.0f);

    glPushMatrix();

    for (int i = 0; i < 40; i++)
    {
        glTranslatef(0.0f, 0.0f, 0.05f);
        glRotatef(350.0f, 0.0f, 1.0f, 0.0f);

        //glColor3f(0.0f, 1.0f, 0.0f);
        quadric = gluNewQuadric();
        gluQuadricTexture(quadric, true);
        gluCylinder(quadric, 0.1f, 0.1f, 0.2f, 20, 20);

    }

    glPopMatrix();

    glTranslatef(-0.3f, -0.08f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    //glColor3f(0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.4f, 0.1f, 0.2f, 20, 20);

    glTranslatef(0.0f, 0.0f, 0.15f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

#pragma endregion Bird_Nest

#pragma endregion Start

}

void Santa_Ferrari(void)
{
    glDisable(GL_TEXTURE_2D);

    //Middle Sit
    glPushMatrix();

    glColor3f(0.7f, 0.7f, 0.7f);

    glScalef(0.6f, 0.3f, 1.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric, 0.5f, 20, 20);

    glPopMatrix();

    //Back (Tekayala)
    glPushMatrix();

    glTranslatef(-0.1f, 0.1f, 0.0f);

    glColor3f(1.0f, 0.7f, 0.7f);

    glScalef(0.3f, 0.6f, 1.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric, 0.5f, 20, 20);

    glPopMatrix();

    //Below Sit Cover
    glPushMatrix();

    glBegin(GL_QUADS);

    glColor3f(0.8f, 0.5f, 0.8f);
    glVertex3f(0.1f, 0.0f, -0.5f);
    glVertex3f(0.1f, 0.0f, 0.5f);
    glVertex3f(0.1f, -0.35f, 0.5f);
    glVertex3f(0.1f, -0.35f, -0.5f);

    glEnd();

    glPopMatrix();

    //Left Side big Sphere
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.5f);

    glColor3f(1.0f, 0.0f, 0.0f);

    glScalef(0.7f, 0.7f, 0.1f);
    quadric = gluNewQuadric();
    gluSphere(quadric, 0.5f, 20, 20);

    glPopMatrix();

    //Left Cover Triangle
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.5f);

    glBegin(GL_TRIANGLES);

    glVertex3f(1.0f, -0.35f, 0.0f);
    glVertex3f(0.0f, 0.35f, 0.0f);
    glVertex3f(0.0f, -0.35f, 0.0f);

    glEnd();

    glPopMatrix();

    //Left Cover Square
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.5f);

    glBegin(GL_QUADS);

    glVertex3f(-0.9f, 0.3f, 0.0f);
    glVertex3f(-0.9f, -0.35f, 0.0f);
    glVertex3f(0.0f, -0.35f, 0.0f);
    glVertex3f(0.0f, 0.3f, 0.0f);

    glEnd();

    glPopMatrix();

    //Right Side big Sphere
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -0.5f);

    glColor3f(1.0f, 0.0f, 0.0f);

    glScalef(0.7f, 0.7f, 0.1f);
    quadric = gluNewQuadric();
    gluSphere(quadric, 0.5f, 20, 20);

    glPopMatrix();

    //Right Cover Triangle
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.5);

    glBegin(GL_TRIANGLES);

    glVertex3f(1.0f, -0.35f, 0.0f);
    glVertex3f(0.0f, 0.35f, 0.0f);
    glVertex3f(0.0f, -0.35f, 0.0f);

    glEnd();

    glPopMatrix();

    //Right Cover Square
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.5f);
    glBegin(GL_QUADS);

    glVertex3f(-0.9f, 0.3f, 0.0f);
    glVertex3f(-0.9f, -0.35f, 0.0f);
    glVertex3f(0.0f, -0.35f, 0.0f);
    glVertex3f(0.0f, 0.3f, 0.0f);

    glEnd();
    glPopMatrix();

    //Back Cover
    glPushMatrix();

    glBegin(GL_QUADS);

    glVertex3f(-1.0f, 0.3f, -0.5f);
    glVertex3f(-1.0f, 0.3f, 0.5f);
    glVertex3f(-1.0f, -0.35f, 0.5f);
    glVertex3f(-1.0f, -0.35f, -0.5f);

    glEnd();

    glPopMatrix();

    //Ball On Tip Left back
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(-0.95f, 0.35f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //Ball on tip Right Back
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(-0.95f, 0.35f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //Ball On Tip Left Front Top 
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, 0.35f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //Ball On Tip Right Front Top
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, 0.35f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //Ball On Tip Left Front Down
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, -0.35f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //Ball On Tip Right Front Down
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, -0.35f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //Ball On Tip Left Front Down
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.0f, -0.7f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //Ball On Tip Right Front Down
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.0f, -0.7f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

    //left Corner Curving
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(-0.95f, -0.35f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 0.7f, 20, 20);

    glPopMatrix();

    //left Corner Curving
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(-0.95f, -0.35f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 0.7f, 20, 20);

    glPopMatrix();

    //Foot Stand (Cover)
    glPushMatrix();

    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);

    glVertex3f(1.5f, -0.35f, 0.5f);
    glVertex3f(1.5f, -0.35f, -0.5f);
    glVertex3f(-1.0f, -0.35f, -0.5f);
    glVertex3f(-1.0f, -0.35f, 0.5f);

    glEnd();

    glPopMatrix();

    //Front Stand (Cover)
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    glVertex3f(1.5f, 0.0f, 0.5f);
    glVertex3f(1.5f, 0.0f, -0.5f);
    glVertex3f(1.5f, -0.35f, -0.5f);
    glVertex3f(1.5f, -0.35f, 0.5f);

    glEnd();

    glPopMatrix();

    //Grill to the car (Main Big one)
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, 0.0f, -0.5f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 20, 20);

    glPopMatrix();

    //Small Left
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, 0.0f, -0.25f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.025f, 0.025f, 0.35f, 20, 20);

    glPopMatrix();

    //Small Right
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, 0.0f, 0.25f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.025f, 0.025f, 0.35f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, 0.35f, -0.5f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 20, 20);

    glPopMatrix();

    //Left Corner |
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, -0.35f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 0.7f, 20, 20);

    glPopMatrix();

    //Right Corner |
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, -0.35f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 0.7f, 20, 20);

    glPopMatrix();

    //left corner down /
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, -0.35f, 0.5f);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(35.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 0.6f, 20, 20);

    glPopMatrix();

    //left corner down ---
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.0f, -0.7f, 0.5f);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 2.0f, 20, 20);

    glPopMatrix();

    //Middle of Down Weel
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(0.5f, -0.7f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.025f, 0.025f, 0.35f, 20, 20);

    glPopMatrix();

    //Middle of Down Weel
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(-0.5f, -0.7f, 0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.025f, 0.025f, 0.35f, 20, 20);

    glPopMatrix();

    //Right corner down /
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.5f, -0.35f, -0.5f);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(35.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 0.6f, 20, 20);

    glPopMatrix();

    //left corner down ---
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(1.0f, -0.7f, -0.5f);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 2.0f, 20, 20);

    glPopMatrix();

    //Middle of Down Weel
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(0.5f, -0.7f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.025f, 0.025f, 0.35f, 20, 20);

    glPopMatrix();

    //Middle of Down Weel
    glPushMatrix();

    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(-0.5f, -0.7f, -0.5f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.025f, 0.025f, 0.35f, 20, 20);

    glPopMatrix();

}

void Santa(void)
{

#pragma region Upper_Body

    //Head
#pragma region Head
    glPushMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindTexture(GL_TEXTURE_2D, Santa_Face_Texture);

    glTranslatef(0.0f, 0.15f, 0.0f);

#pragma region Face

    glPushMatrix();

    glScalef(0.7f, 0.8f, 0.7f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.15f, 30, 30);

    glPopMatrix();

#pragma endregion Face    

#pragma region Mouth

    glPushMatrix();

    glTranslatef(0.0f, -0.03f, -0.11f);

    glBindTexture(GL_TEXTURE_2D, Santa_Texture);

    glScalef(2.0f, 1.0f, 1.5f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPopMatrix();

#pragma endregion Mouth

#pragma region Eyes
   
    glDisable(GL_TEXTURE_2D);

    //Right
    glPushMatrix();

    glTranslatef(-0.04f, 0.0f, -0.09f);

    glColor3f(1.0f, 0.644f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPopMatrix();


    //Left
    glPushMatrix();

    glTranslatef(0.04f, 0.0f, -0.09f);

    glColor3f(1.0f, 0.644f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

#pragma endregion Eyes

#pragma region Back_head

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.03f);
    glScalef(1.1f, 1.0f, 0.8f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 30, 30);

    glPopMatrix();

#pragma endregion Back_head

#pragma region Ears
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Santa_Face_Texture);
    
    //Left Hear
    glPushMatrix();

    glTranslatef(0.105f, -0.01f, 0.0f);
    glScalef(0.8f, 0.9f, 0.7f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.035f, 30, 30);

    glPopMatrix();

    //Right Hear
    glPushMatrix();

    glTranslatef(-0.105f, -0.01f, 0.0f);
    glScalef(0.8f, 0.9f, 0.7f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.035f, 30, 30);

    glPopMatrix();

#pragma endregion Ears
  
#pragma region Cap
    //Cap

    glDisable(GL_TEXTURE_2D);
    //White Boarder of Hatt
    glTranslatef(0.0f, 0.05f, 0.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

    glPushMatrix();

    glScalef(0.6f, 0.35f, 0.55f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.2f, 30, 30);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Santa_Texture);

    //Red Hatt
    glPushMatrix();

    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.1f, 0.09f, 0.15f, 20, 20);

    glTranslatef(0.0f, 0.0f, 0.14f);

    glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.09f, 0.06f, 0.1f, 20, 20);

    glTranslatef(0.0f, 0.0f, 0.09f);

    glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.06f, 0.0f, 0.1f, 20, 20);

    glDisable(GL_TEXTURE_2D);

    //Hatt Top Sphere
    glTranslatef(0.0f, 0.0f, 0.1f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.05f, 20, 20);

    glPopMatrix();

#pragma endregion Cap

    glPopMatrix();

#pragma endregion Head

    //Beard
#pragma region Beard
    glPushMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glTranslatef(0.0f, 0.07f, -0.09f);
    glRotatef(310.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.5f, 0.3f, 0.8f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.2f, 30, 30);

    glPopMatrix();

#pragma endregion Beard
    
    //Neck, Body, Patta
#pragma region Upper_Body_Cover
    glPushMatrix();

    glTranslatef(0.0f, 0.1f, 0.0f);
    glRotatef(100.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.65f, 0.6f, 0.35f);

    //Neck
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.2f, 30, 30);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

    //Santa Body 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindTexture(GL_TEXTURE_2D, Santa_Texture);

    glPushMatrix();

    glTranslatef(0.0f, -0.15f, 0.0f);
    glScalef(0.5f, 0.6f, 0.5f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.4f, 30, 30);

    glPopMatrix();

    //Santa Kambar Patta
    glBindTexture(GL_TEXTURE_2D, Santa_Foots_Texture);

    glPushMatrix();

    glTranslatef(0.19f, -0.26f, -0.027f);
    glScalef(1.0f, 2.5f, 1.0f);

    for (size_t i = 0; i < 30; i++)
    {

        glTranslatef(0.0f, 0.0f, 0.05f);
        glRotatef(345.0f, 0.0f, 1.0f, 0.0f);


        quadric = gluNewQuadric();
        gluQuadricTexture(quadric, true);
        gluCylinder(quadric, 0.01f, 0.01, 0.07, 20, 20);

    }
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

#pragma endregion Upper_Body_Cover

#pragma endregion Upper_Body

#pragma region Left_hand

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Santa_Texture);

    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glRotatef(15, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -0.17f, 0.0f);
    glRotatef((GLfloat)sholder_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.02f, 0.0f, 0.0f);

    //Main Top Sphere for joining hand And Body
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPushMatrix();

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();


    //LBow Joint
    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();

    //Hands
    glBindTexture(GL_TEXTURE_2D, Santa_Foots_Texture);

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glTranslatef(0.15f, 0.0f, 0.0f);
    glScalef(0.5f, 0.2f, 0.15f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    glPopMatrix();

#pragma endregion Left_hand

#pragma region Right_hand


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Santa_Texture);

    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glRotatef(345, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.17f, 0.0f);
    glRotatef((GLfloat)sholder_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.02f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glBindTexture(GL_TEXTURE_2D, Santa_Texture);
    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glBindTexture(GL_TEXTURE_2D, Santa_Texture);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.0205f, 0.0205f, 0.15f, 20, 20);

    glPopMatrix();


    //LBow Joint
    glBindTexture(GL_TEXTURE_2D, Santa_Texture);
    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();



    glBindTexture(GL_TEXTURE_2D, Santa_Foots_Texture);

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glTranslatef(0.15f, 0.0f, 0.0f);
    glScalef(0.5f, 0.2f, 0.15f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    glPopMatrix();

#pragma endregion Right_hand

#pragma region Left_Leg


    glPushMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Santa_Texture);

    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.25f, -0.1f, 0.07f);
    glRotatef((GLfloat)Thigh_Right, 0.0f, 1.0f, 0.0f);

    glPushMatrix();

    glTranslatef(0.05f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, Santa_Foots_Texture);

    glTranslatef(0.2f, 0.0f, 0.0f);
    glRotatef((GLfloat)Calf_Right, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.01f, 0.0f, 0.0f);

    glPushMatrix();

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.045f);

    glScalef(0.3f, 0.4f, 0.7f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    glPopMatrix();

#pragma endregion Left_Leg

#pragma region Right_Leg
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Santa_Texture);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.25f, 0.1f, 0.07f);
    glRotatef((GLfloat)Thigh_Right, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.05f, 0.0f, 0.0f);
    //glScalef(1.0f, 0.2f, 0.2f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, Santa_Foots_Texture);

    glTranslatef(0.2f, 0.0f, 0.0f);
    glRotatef((GLfloat)Calf_Right, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.01f, 0.0f, 0.0f);

    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.045f);

    glScalef(0.3f, 0.4f, 0.7f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();


    glPopMatrix();
#pragma endregion Right_Leg


}

void Girl(void)
{

#pragma region Upper_Body

    //Head
#pragma region Head
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glTranslatef(0.0f, 0.15f, 0.0f);
#pragma region Face

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);
    glPushMatrix();

    glScalef(0.7f, 0.8f, 0.7f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.15f, 30, 30);

    glPopMatrix();


#pragma endregion Face    

#pragma region Eyes

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);

    glTranslatef(-0.04f, -0.02f, -0.09f);
    glScalef(0.1f, 0.2f, 0.1f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);

    glTranslatef(0.04f, -0.02f, -0.09f);
    glScalef(0.1f, 0.2f, 0.1f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

#pragma endregion Eyes

#pragma region Nose

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Girls_Nose_Texture);

    glTranslatef(0.0f, -0.065f, -0.08f);
    glScalef(0.035f, 0.025f, 0.02f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f, 0.11f, -0.088f);
    glRotatef(330.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    glVertex2f(0.0f, 0.025f);
    glVertex2f(-0.025f, -0.0125f);
    glVertex2f(0.025f, -0.0125f);


    glVertex2f(0.025f, 0.0125f);
    glVertex2f(-0.025f, 0.0125f);
    glVertex2f(0.0f, -0.025f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);

    glPopMatrix();

#pragma endregion Nose


#pragma region Back_head

    glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.03f);
    glScalef(1.1f, 1.0f, 0.8f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 30, 30);

    glPopMatrix();

#pragma endregion Back_head

#pragma region Ears
    //Left Hear
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);
    glTranslatef(0.105f, -0.01f, 0.0f);
    glScalef(0.8f, 0.9f, 0.7f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 30, 30);

    glPopMatrix();

    //Right Hear
    glPushMatrix();

    glTranslatef(-0.105f, -0.01f, 0.0f);
    glScalef(0.8f, 0.9f, 0.7f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 30, 30);

    glPopMatrix();

#pragma endregion Ears

#pragma region Cap
    //Cap
    glPushMatrix();
    //glBindTexture(GL_TEXTURE_2D, girl_Texture);
    glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);


    glTranslatef(0.0f, 0.06f, 0.0f);
    glRotatef(10, 1.0f, 0.0f, 0.0f);
    glScalef(0.55f, 0.6f, 0.55f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.2f, 30, 30);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f, -0.03f, 0.07f);
    glRotatef(345, 1.0f, 0.0f, 0.0f);
    glScalef(0.6f, 1.1f, 0.3f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.2f, 30, 30);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

#pragma endregion Cap

    glPopMatrix();

#pragma endregion Head

    //Neck
#pragma region Neck

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();

    glScalef(0.4f, 0.2f, 0.4f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.2f, 30, 30);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f, -0.09f, -0.04f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(340.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f);

    glScalef(0.3f, 0.6f, 0.2f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.2f, 30, 30);

    glPopMatrix();


    glEnable(GL_TEXTURE_2D);


#pragma endregion Neck

#pragma region Main

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, girl_Texture);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.15f, 0.3f, 20, 20);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //


    //White Line bottom of cloth

    /*glPushMatrix();

    glTranslatef(0.19f, -0.26f, -0.027f);
    glScalef(1.0f, 2.5f, 1.0f);

    for (size_t i = 0; i < 30; i++)
    {

        glTranslatef(0.0f, 0.0f, 0.05f);
        glRotatef(345.0f, 0.0f, 1.0f, 0.0f);


        quadric = gluNewQuadric();
        gluQuadricTexture(quadric, true);
        gluCylinder(quadric, 0.01f, 0.01, 0.07, 20, 20);

    }
    glPopMatrix();*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#pragma endregion Main

#pragma endregion Upper_Body

#pragma region Left_hand

    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, girl_Texture);

    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glRotatef(10, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -0.1f, 0.0f);
    glRotatef((GLfloat)sholder_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.02f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);
    
    glPushMatrix();
    
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glBindTexture(GL_TEXTURE_2D, girl_Texture);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.0205f, 0.0205f, 0.15f, 20, 20);

    glPopMatrix();


    //LBow Joint
    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);

    //glTranslatef(0.0f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPushMatrix();
    
    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, Girls_Foots_Texture);

    glPushMatrix();
    
    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glTranslatef(0.15f, 0.0f, 0.0f);
    glScalef(0.5f, 0.2f, 0.15f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    glPopMatrix();
#pragma endregion Left_hand

#pragma region Right_hand

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);

    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glRotatef(350, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.1f, 0.0f);
    glRotatef((GLfloat)sholder_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.02f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, girl_Texture);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);


    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);
    gluCylinder(quadric, 0.0205f, 0.0205f, 0.08f, 20, 20);

    glBindTexture(GL_TEXTURE_2D, girl_Texture);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.0205f, 0.0205f, 0.05f, 20, 20);

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);

    //LBow Joint
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.02f, 20, 20);

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);
    glPopMatrix();

    //Watch
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);

    //change the girl watch position x will take it down. Z will take in front and back
    glTranslatef(0.28f, 0.0f, 0.02f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.0205f, 0.0205f, 0.03f, 20, 20);

    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 0.0f);

    glTranslatef(-0.008f, 0.0f, 0.015f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.0205f, 20, 20);

    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindTexture(GL_TEXTURE_2D, Girls_Foots_Texture);

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef((GLfloat)Lbow_Left, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glTranslatef(0.15f, 0.0f, 0.0f);
    glScalef(0.5f, 0.2f, 0.15f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    //glPopMatrix();
    
    glPopMatrix();

#pragma endregion Right_hand

#pragma region Left_Leg

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);

    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.22f, -0.07f, 0.0f);
    glRotatef((GLfloat)Thigh_Right, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.05f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);
    glPopMatrix();


    glTranslatef(0.2f, 0.0f, 0.0f);
    glRotatef((GLfloat)Calf_Right, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.01f, 0.0f, 0.0f);


    glPushMatrix();
    //glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);

    glTranslatef(0.002f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.020f, 20, 20);

    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);
    glPopMatrix();

    glPushMatrix();

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.045f);

    glScalef(0.3f, 0.4f, 0.7f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    glPopMatrix();

#pragma endregion Left_Leg

#pragma region Right_Leg
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.22f, 0.07f, 0.0f);
    glRotatef((GLfloat)Thigh_Right, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.05f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);
    glPopMatrix();


    glTranslatef(0.2f, 0.0f, 0.0f);
    glRotatef((GLfloat)Calf_Right, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.01f, 0.0f, 0.0f);

    glPushMatrix();
    //glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);

    glTranslatef(0.002f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.020f, 20, 20);

    glBindTexture(GL_TEXTURE_2D, girl_Face_Texture);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.02f, 0.02f, 0.15f, 20, 20);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindTexture(GL_TEXTURE_2D, Girls_Foot_Down_Texture);

    glPushMatrix();

    glTranslatef(0.15f, 0.0f, 0.045f);

    glScalef(0.3f, 0.4f, 0.7f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glPopMatrix();

    glPopMatrix();

#pragma endregion Right_Leg

}

void House(void)
{
    void wall(void);
    void Top_Shade(void);
    void Grill(void);
    void Windows(void);
    void Door(void);
    void Compaund(void);

#pragma region Walls

    wall();

#pragma endregion Walls
    
#pragma region Front_Triangle_Wall

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Wood_Texture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.2f, 0.4f);
    glVertex3f(0.0f, 3.5f, 5.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.0f, 1.8f, 5.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.0f, 1.8f, 5.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
#pragma endregion Front_Triangle_Wall

#pragma region Top_Chapar

    Top_Shade();

#pragma endregion Top_Chapar

#pragma region Pols
   
    Grill();

#pragma endregion Pols
        
#pragma region Windows

    Windows();

#pragma endregion Windows
        
#pragma region Doar

    Door();
#pragma endregion Doar

#pragma region Compaund

    glBindTexture(GL_TEXTURE_2D, Door_Texture);

    glTranslatef(15.0f, -2.0f, -10.0f);
    //back Side
    for (int i = 1; i <= 30; i++)
    {
        glTranslatef(-1.0f, 0.0f, 0.0f);
        Compaund();
    }

    //Left side
    
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    for (int i = 1; i < 30; i++)
    {
        glTranslatef(-1.0f, 0.0f, 0.0f);
        Compaund();

    }

    //Left side
    glTranslatef(0.0f, 0.0f, 30.0f);
    for (int i = 1; i < 30; i++)
    {
        glTranslatef(1.0f, 0.0f, 0.0f);
        Compaund();
    }

#pragma endregion Compaund

}

void Compaund(void)
{
    //front
    glBegin(GL_POLYGON);

    glTexCoord2f(0.25f, 1.5f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-0.3f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.3f, -1.5f, 0.0f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.3f, -1.5f, 0.0f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.3f, 0.5f, 0.0f);

    glEnd();

    //Right
    glBegin(GL_QUADS);

    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 1.0f, -0.2f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.3f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(0.3f, 0.5f, -0.2f);

    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.3f, 0.5f, -0.2f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.3f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.3f, -1.5f, 0.0f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(0.3f, -1.5f, -0.2f);

    glEnd();

    //Middle Line 
  /* glBegin(GL_QUADS);

    //Left Border

    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.1f, -0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.1f, -0.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.1f, -0.1f);

    ////Right
    //glTexCoord2f(1.0f, 1.0f);
    //glVertex3f(-1.25f, 2.0f, 2.2f);
    //glTexCoord2f(0.0f, 1.0f);
    //glVertex3f(-1.25f, 2.0f, 2.0f);
    //glTexCoord2f(0.0f, 0.0f);
    //glVertex3f(-1.25f, -1.5f, 2.0f);
    //glTexCoord2f(1.0f, 0.0f);
    //glVertex3f(-1.25f, -1.5f, 2.2f);

    //Left

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.9f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.9f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.9f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.9f, 2.2f);


    glEnd();
    */

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -0.2f);

    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 20, 20);

    glPopMatrix();

    //Left
    glBegin(GL_QUADS);

    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 1.0f, -0.2f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.3f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-0.3f, 0.5f, -0.2f);

    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-0.3f, 0.5f, -0.2f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-0.3f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.3f, -1.5f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.3f, -1.5f, -0.2f);

    glEnd();

    //Back
    glBegin(GL_POLYGON);

    glTexCoord2f(0.25f, 1.5f);
    glVertex3f(0.0f, 1.0f, -0.2f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-0.3f, 0.5f, -0.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.3f, -1.5f, -0.2f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.3f, -1.5f, -0.2f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.3f, 0.5f, -0.2f);

    glEnd();
}

void wall(void)
{
    //Texturing Done
#pragma region Back_Walls

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Wood_Texture);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(-7.0f, 4.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-7.0f, 2.0f, 2.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-7.0f, 2.0f, -2.5f);

    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(7.0f, 4.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(7.0f, 2.0f, 2.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(7.0f, 2.0f, -2.5f);

    glEnd();

    //Bottom of Triangle
    glBegin(GL_QUADS);

    //left 
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(7.0f, 2.0f, -2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.0f, 2.0f, -2.0f);
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(6.0f, 2.0f, 2.0f);
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(7.0f, 2.0f, 2.0f);

    //Right
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(-7.0f, 2.0f, -2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, 2.0f, -2.0f);
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(-6.0f, 2.0f, 2.0f);
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(-7.0f, 2.0f, 2.0f);

    glEnd();

    glBegin(GL_QUADS);

    // Back side Wall
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-6.0f, 2.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.0f, 2.0f, -2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.0f, -3.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.0f, -3.0f, -2.0f);

    //Left Side wall
    glTexCoord2f(0.6f, 0.6f);
    glVertex3f(-6.0f, 2.0f, 2.0f);
    glTexCoord2f(0.6f, 0.0f);
    glVertex3f(-6.0f, 2.0f, -2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, -3.0f, -2.0f);
    glTexCoord2f(0.0f, 0.6f);
    glVertex3f(-6.0f, -3.0f, 2.0f);

    //Right Side wall
    glTexCoord2f(0.6f, 0.6f);
    glVertex3f(6.0f, 2.0f, 2.0f);
    glTexCoord2f(0.6f, 0.0f);
    glVertex3f(6.0f, 2.0f, -2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.0f, -3.0f, -2.0f);
    glTexCoord2f(0.0f, 0.6f);
    glVertex3f(6.0f, -3.0f, 2.0f);


    //Front Left
    glTexCoord2f(0.6f, 0.6f);
    glVertex3f(-6.0f, 2.0f, 2.0f);
    glTexCoord2f(0.6f, 0.0f);
    glVertex3f(-1.25f, 2.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.25f, -2.0f, 2.0f);
    glTexCoord2f(0.0f, 0.6f);
    glVertex3f(-6.0f, -2.0f, 2.0f);

    //Front Right
    glTexCoord2f(0.6f, 0.6f);
    glVertex3f(6.0f, 2.0f, 2.0f);
    glTexCoord2f(0.6f, 0.0f);
    glVertex3f(1.25f, 2.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.25f, -2.0f, 2.0f);
    glTexCoord2f(0.0f, 0.6f);
    glVertex3f(6.0f, -2.0f, 2.0f);

    //Upon Door

    glTexCoord2f(0.4f, 0.4f);
    glVertex3f(-1.75f, 2.5f, 2.0f);
    glTexCoord2f(0.4f, 0.0f);
    glVertex3f(1.75f, 2.5f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.75f, 2.0f, 2.0f);
    glTexCoord2f(0.0f, 0.4f);
    glVertex3f(-1.75f, 2.0f, 2.0f);

    glEnd();

    glPopMatrix();

#pragma endregion Back_Walls
    //Texturing Done
#pragma region Ground_Wall

    glPushMatrix();
    //Ground Front Land
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, Wood_Texture);

    glBegin(GL_QUADS);

    //main Wood land
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.0f, -2.0f, 5.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.0f, -2.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.0f, -2.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-6.0f, -2.0f, 5.0f);

    //Front Small Wood Near to steps
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(6.0f, -3.0f, 5.0f);
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(6.0f, -2.0f, 5.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, -2.0f, 5.0f);
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(-6.0f, -3.0f, 5.0f);

    //Left cover
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-6.0f, -3.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-6.0f, -3.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, -2.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.0f, -2.0f, 5.0f);

    //Right cover
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(6.0f, -3.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.0f, -3.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.0f, -2.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(6.0f, -2.0f, 5.0f);

    //Steps Top 

    //Top
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.0f, -2.0f, 6.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.0f, -2.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.0f, -2.0f, 5.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.0f, -2.0f, 6.0f);

    //Front
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(2.0f, -2.0f, 6.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.0f, -2.0f, 6.0f);
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(-2.0f, -2.5f, 6.0f);
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(2.0f, -2.5f, 6.0f);

    //Right
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(2.0f, -2.0f, 6.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.0f, -2.0f, 5.0f);
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(2.0f, -2.5f, 5.0f);
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(2.0f, -2.5f, 6.0f);

    //Left
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(-2.0f, -2.0f, 6.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.0f, -2.0f, 5.0f);
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(-2.0f, -2.5f, 5.0f);
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(-2.0f, -2.5f, 6.0f);

    //Step Bottom

    //Top
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.0f, -2.5f, 7.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.0f, -2.5f, 5.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.0f, -2.5f, 5.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.0f, -2.5f, 7.0f);

    //Front
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(2.0f, -2.5f, 7.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.0f, -2.5f, 7.0f);
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(-2.0f, -3.0f, 7.0f);
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(2.0f, -3.0f, 7.0f);

    //Right
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(2.0f, -2.5f, 7.0f);
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(2.0f, -3.0f, 7.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.0f, -3.0f, 5.0f);
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(2.0f, -2.5f, 5.0f);

    //Left
    glTexCoord2f(0.2f, 0.2f);
    glVertex3f(-2.0f, -2.5f, 7.0f);
    glTexCoord2f(0.2f, 0.0f);
    glVertex3f(-2.0f, -3.0f, 7.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.0f, -3.0f, 5.0f);
    glTexCoord2f(0.0f, 0.2f);
    glVertex3f(-2.0f, -2.5f, 5.0f);

    glEnd();

    glPopMatrix();

#pragma endregion Ground_Wall

#pragma region Front_Small_Walls

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Snow_Texture);

    glBegin(GL_QUADS);

    //Top Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 3.85f, 5.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, 3.85f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.0f, 2.3f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.0f, 2.3f, 5.0f);

    //Top Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 3.85f, 5.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, 3.85f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.0f, 2.3f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.0f, 2.3f, 5.0f);

    glEnd();

    glPopMatrix();

#pragma endregion Front_Small_Walls
    //Texturing Done
#pragma region Snow_Cylender_Poarch

    //Middle Small Cylenf=der
    glPushMatrix();

    glTranslatef(0.0f, 3.5f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 5.0f, 30, 30);

    glPopMatrix();

    //Middle Left Cylenf=der
    glPushMatrix();

    glTranslatef(2.0f, 2.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 5.0f, 30, 30);

    glPopMatrix();

    //Middle Left Cylenf=der
    glPushMatrix();

    glTranslatef(-2.0f, 2.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 5.0f, 30, 30);

    glPopMatrix();

    //Front Right Cylenf=der
    glPushMatrix();

    glTranslatef(0.0f, 3.5f, 5.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(55.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 2.5f, 30, 30);

    glPopMatrix();

    //Front Left Cylenf=der
    glPushMatrix();

    glTranslatef(0.0f, 3.5f, 5.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(305.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 2.5f, 30, 30);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

#pragma endregion Snow_Cylender_Poarch

}

void Top_Shade(void) {
    //Texturing Done
#pragma region Left_Top_Chapar
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Snow_Texture);

    glTranslatef(-7.0f, 4.0f, 0.0f);

    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glRotated(50.0f, 1.0f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 3.0f, 30, 30);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-7.0f, 4.0f, 0.0f);

    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glRotated(310.0f, 1.0f, 0.0f, 0.0f);

    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 3.0f, 30, 30);

    glPopMatrix();

#pragma endregion Left_Top_Chapar

    //Texturing Done
#pragma region Right_Top_Chapar

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Snow_Texture);

    glTranslatef(7.0f, 4.0f, 0.0f);

    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glRotated(50.0f, 1.0f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 3.0f, 30, 30);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(7.0f, 4.0f, 0.0f);

    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glRotated(310.0f, 1.0f, 0.0f, 0.0f);

    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 3.0f, 30, 30);

    glPopMatrix();

#pragma endregion Right_Top_Chapar

    //Texturing Done
#pragma region Chapar_Main_Cylender

    glPushMatrix();

    glTranslatef(-7.0f, 4.0f, 0.0f);
    glRotated(90.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 14.0f, 30, 30);

    glPopMatrix();


    glPushMatrix();

    glRotated(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(2.3f, 2.0f, -7.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 14.0f, 30, 30);

    glPopMatrix();


    glPushMatrix();

    glRotated(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-2.3f, 2.0f, -7.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 5.0f, 30, 30);


    glTranslatef(0.0f, 0.0f, 9.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.3f, 5.0f, 30, 30);

    glPopMatrix();


    glPushMatrix();

    glBegin(GL_QUADS);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(7.0f, 4.2f, 0.2f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-7.0f, 4.2f, 0.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-7.0f, 2.2f, 2.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(7.0f, 2.2f, 2.5f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(7.0f, 4.2f, -0.2f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-7.0f, 4.2f, -0.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-7.0f, 2.2f, -2.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(7.0f, 2.2f, -2.5f);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

#pragma endregion Chapar_Main_Cylender

    //Sphere to join the shade
    //Texturing Done
#pragma region Sphere

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Snow_Texture);

    //top four Main Corners
    glPushMatrix();

    //Left Front
    glTranslatef(-7.0f, 2.02f, 2.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.3f, 30, 30);

    glPopMatrix();

    //Left back
    glPushMatrix();

    glTranslatef(-7.0f, 2.02f, -2.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.3f, 30, 30);

    glPopMatrix();

    //Right Front
    glPushMatrix();

    glTranslatef(7.0f, 2.02f, 2.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.3f, 30, 30);

    glPopMatrix();

    //Right Back
    glPushMatrix();

    glTranslatef(7.0f, 2.02f, -2.3f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.3f, 30, 30);

    glPopMatrix();

    //Right Middle 
    glPushMatrix();

    glTranslatef(7.0f, 4.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.3f, 30, 30);

    glPopMatrix();

    //Left Middle 
    glPushMatrix();

    glTranslatef(-7.0f, 4.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.3f, 30, 30);

    glPopMatrix();

    //small Shade's Corners
    //middle
    glPushMatrix();

    glTranslatef(0.0f, 3.47f, 5.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.35f, 30, 30);

    glPopMatrix();

    //Left
    glPushMatrix();

    glTranslatef(-2.1f, 2.0f, 5.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.35f, 30, 30);

    glPopMatrix();

    //Right
    glPushMatrix();

    glTranslatef(2.1f, 2.0f, 5.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.35f, 30, 30);

    glPopMatrix();


#pragma endregion Sphere

}

void Grill(void)
{
    //Front First Pols Y Big One 
   //For Incresing the hight of the grill increse 0.5 value in vise versa 
    glPushMatrix();

    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(2.0f, 5.0f, -2.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.05f, 0.05f, 4.0f, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(-2.0f, 5.0f, -2.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric, 0.05f, 0.05f, 4.0f, 30, 30);

    glPopMatrix();

    //Front First Pols Y Small One 

    glPushMatrix();
    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(6.0f, 5.0f, 0.5f);
    quadric = gluNewQuadric();
    gluCylinder(quadric, 0.05f, 0.05f, 1.5f, 30, 30);

    glPopMatrix();

    glPushMatrix();
    glRotated(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(-6.0f, 5.0f, 0.5f);
    quadric = gluNewQuadric();
    gluCylinder(quadric, 0.05f, 0.05f, 1.5f, 30, 30);

    glPopMatrix();


    //Middle Verticla Pols X
    glPushMatrix();
    glRotated(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-5.0f, -0.5f, 2.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric, 0.05f, 0.05f, 4.0f, 30, 30);

    glPopMatrix();

    glPushMatrix();
    glRotated(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-5.0f, -0.5f, -6.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric, 0.05f, 0.05f, 4.0f, 30, 30);

    glPopMatrix();


    //Middle Verticla Pols Z
    glPushMatrix();
    glRotated(90.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.5f, 6.0f, 2.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric, 0.05f, 0.05f, 3.0f, 30, 30);

    glPopMatrix();

    glPushMatrix();
    glRotated(90.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.5f, -6.0f, 2.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric, 0.05f, 0.05f, 3.0f, 30, 30);

    glPopMatrix();

}

void Windows(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);


#pragma region Left_Window

    //Left Window
    glBindTexture(GL_TEXTURE_2D, Tree_Glass_Texture);


    glBegin(GL_QUADS);

    //back Light left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.01f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);


    glBegin(GL_QUADS);
    //Front Top Border
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, 1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.0f);

    //Front Bottom Border
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, -1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.0f);


    //Right Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);

    //left Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.9f, -1.3f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);

    //Middle of window Rods 1
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.7f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.7f, 2.0f);

    //Middle of window Rods 2
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.0f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.0f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.0f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.1f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.1f, 2.0f);

    //Middle of window Rods 3
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -0.6f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, -0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -0.7f, 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, -0.7f, 2.0f);

    //Middle of window Rods Vertical 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.8f, -1.2f, 2.1f);

    //Right
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.8f, -1.2f, 2.1f);

    //Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.9f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.9f, -1.2f, 2.1f);

    glEnd();

#pragma endregion Left_Window


#pragma region Right_Window

    //Right Window
    glBindTexture(GL_TEXTURE_2D, Glass_Texture);


    glBegin(GL_QUADS);

    //back Light left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.01f);

    glEnd();


    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);

    glBegin(GL_QUADS);
    //Front Top Border
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, 1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.0f);



    //Front Bottom Border
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, -1.2f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.0f);


    //Right Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);

    //left Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);

    //Middle of window Rods 1
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.7f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.7f, 2.0f);

    //Middle of window Rods 2
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.0f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.0f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.0f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.1f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.1f, 2.0f);

    //Middle of window Rods 3
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -0.6f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, -0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -0.7f, 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, -0.7f, 2.0f);

    //Middle of window Rods Vertical 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.8f, -1.2f, 2.1f);

    //Right
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.8f, -1.2f, 2.1f);

    //Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.9f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.9f, -1.2f, 2.1f);

    glEnd();
#pragma endregion Right_Window

#pragma region Left_Side_Window

    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, 4.0f);

    glRotatef(270, 0.0f, 1.0f, 0.0f);


    glBindTexture(GL_TEXTURE_2D, Glass_Texture);
    glBegin(GL_QUADS);

    //back Light Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.2f, 2.01f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);

    glBegin(GL_QUADS);

    //Left Window
     //Front Top Border
     //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, 1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.0f);

    //Front Bottom Border
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, -1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.0f);


    //Right Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);

    //left Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.9f, -1.3f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);

    //Middle of window Rods 1
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.7f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.7f, 2.0f);

    //Middle of window Rods 2
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.0f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.0f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.0f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.1f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.1f, 2.0f);

    //Middle of window Rods 3
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -0.6f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, -0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -0.7f, 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, -0.7f, 2.0f);

    //Middle of window Rods Vertical 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.8f, -1.2f, 2.1f);

    //Right
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.8f, -1.2f, 2.1f);

    //Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.9f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.9f, -1.2f, 2.1f);

    glEnd();
    glPopMatrix();


#pragma endregion Left_Side_Window

#pragma region Right_Side_Window
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 4.0f);

    glRotatef(90, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, Glass_Texture);
    glBegin(GL_QUADS);

    //back Light Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, -1.2f, 2.01f);


    glEnd();

    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);


    glBegin(GL_QUADS);

    //Right Window
    //Front Top Border
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, 1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.0f);



    //Front Bottom Border
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, -1.2f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.0f);


    //Right Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);

    //left Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);

    //Middle of window Rods 1
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.7f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.7f, 2.0f);

    //Middle of window Rods 2
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.0f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.0f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.0f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.1f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.1f, 2.0f);

    //Middle of window Rods 3
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -0.6f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, -0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -0.7f, 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, -0.7f, 2.0f);

    //Middle of window Rods Vertical 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.8f, -1.2f, 2.1f);

    //Right
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.8f, -1.2f, 2.1f);

    //Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.9f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.9f, -1.2f, 2.1f);

    glEnd();
    glPopMatrix();


#pragma endregion Right_Side_Window

#pragma region Back_Left_Side_Window

    glPushMatrix();

    glRotatef(180, 0.0f, 1.0f, 0.0f);

    glTranslatef(7.5f, 0.0f, 0.01f);


    glBindTexture(GL_TEXTURE_2D, Glass_Texture);
    glBegin(GL_QUADS);

    //back Light Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.2f, 2.01f);


    glEnd();
    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);

    glBegin(GL_QUADS);

    //Left Window

     //Front Top Border
     //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, 1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.0f);

    //Front Bottom Border
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, -1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.0f);


    //Right Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.8f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.8f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -1.2f, 2.1f);

    //left Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.9f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.9f, -1.3f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.9f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -1.2f, 2.1f);

    //Middle of window Rods 1
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.7f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.7f, 2.0f);

    //Middle of window Rods 2
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.0f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.0f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.0f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, 0.1f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, 0.1f, 2.0f);

    //Middle of window Rods 3
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -0.6f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-4.8f, -0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.9f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.8f, -0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.9f, -0.7f, 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-4.8f, -0.7f, 2.0f);

    //Middle of window Rods Vertical 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.8f, -1.2f, 2.1f);

    //Right
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.8f, -1.2f, 2.1f);

    //Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.9f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.9f, -1.2f, 2.1f);

    glEnd();
    glPopMatrix();


#pragma endregion Back_Left_Side_Window

#pragma region Back_Right_Side_Window
    glPushMatrix();

    glRotatef(180, 0.0f, 1.0f, 0.0f);

    glTranslatef(-7.5f, 0.0f, 0.01f);

    glBindTexture(GL_TEXTURE_2D, Glass_Texture);
    glBegin(GL_QUADS);

    //back Light Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, -1.2f, 2.01f);


    glEnd();
    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);


    glBegin(GL_QUADS);

    //Right Window
    //Front Top Border
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 1.2f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, 1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.0f);



    //Front Bottom Border
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, -1.2f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.0f);


    //Right Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.8f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.8f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -1.2f, 2.1f);

    //left Border 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.9f, 1.3f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.9f, 1.3f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.9f, -1.3f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.9f, -1.3f, 2.1f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 1.2f, 2.01f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -1.2f, 2.01f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -1.2f, 2.1f);

    //Middle of window Rods 1
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.7f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.7f, 2.0f);

    //Middle of window Rods 2
    //front
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.0f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.0f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.0f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, 0.1f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, 0.1f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, 0.1f, 2.0f);

    //Middle of window Rods 3
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -0.6f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.8f, -0.6f, 2.1f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.9f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -0.6f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.8f, -0.6f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -0.6f, 2.0f);

    //Top
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.8f, -0.7f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.9f, -0.7f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.9f, -0.7f, 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.8f, -0.7f, 2.0f);

    //Middle of window Rods Vertical 
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.9f, -1.2f, 2.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.8f, -1.2f, 2.1f);

    //Right
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.8f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.8f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.8f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.8f, -1.2f, 2.1f);

    //Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3.9f, 1.2f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.9f, 1.2f, 2.01f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.9f, -1.2f, 2.01f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3.9f, -1.2f, 2.1f);

    glEnd();
    glPopMatrix();


#pragma endregion Back_Right_Side_Window

}

void Door(void)
{

#pragma region Doar_Boarder

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Boarder_Texture);

    glBegin(GL_QUADS);

    //Left Border 

    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.25f, 2.0f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.9f, 2.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.9f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.25f, -2.0f, 2.2f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.25f, 2.0f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.25f, 2.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.25f, -2.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.25f, -2.0f, 2.2f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.9f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.9f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.9f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.9f, 2.2f);

    //Right Border
    //front
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.25f, 2.0f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.9f, 2.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.9f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.25f, -2.0f, 2.2f);

    //Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.25f, 2.0f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.25f, 2.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.25f, -2.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.25f, -2.0f, 2.2f);

    //Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.9f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.9f, 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.9f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.9f, 2.2f);


    //Front Top Border
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.25f, 2.0f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.8f, 2.2f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.8f, 2.2f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.25f, 2.0f, 2.2f);

    // //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.8f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.8f, 2.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.8f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.8f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.25f, 2.0f, 2.1f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.25f, 2.0f, 2.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.25f, 2.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.25f, 2.0f, 2.0f);

    //Front Bottom Border
    //front
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.25f, -2.0f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.25f, -2.0f, 2.2f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.8f, 2.2f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.8f, 2.2f);

    //bottom
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.8f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.8f, 2.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.8f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.8f, 2.0f);

    //Top
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.25f, -2.0f, 2.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.25f, -2.0f, 2.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.25f, -2.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.25f, -2.0f, 2.0f);

    glEnd();

    glPopMatrix();
#pragma endregion Doar_Boarder

#pragma region Main_Doar

    glPushMatrix();



    glTranslatef(-1.2f, 0.0f, 1.9f);

    glRotatef(Door_angle, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, Door_Texture);

    glBegin(GL_QUADS);

    //Front side
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.2f, 1.8f, 0.2f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 1.8f, 0.2f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, -1.8f, 0.2f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.2f, -1.8f, 0.2f);

    //right side
    glTexCoord2f(0.1f, 0.0f);
    glVertex3f(2.2f, 1.8f, 0.2f);
    glTexCoord2f(0.1f, 0.1f);
    glVertex3f(2.2f, 1.8f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.2f, -1.8f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.2f, -1.8f, 0.2f);

    //Bottom side
    glTexCoord2f(0.0f, 0.1f);
    glVertex3f(2.2f, -1.8f, 0.2f);
    glTexCoord2f(0.1f, 0.1f);
    glVertex3f(2.2f, -1.8f, 0.0f);
    glTexCoord2f(0.0f, 0.1f);
    glVertex3f(0.0f, -1.8f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.8f, 0.2f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, Steel_Texture);

    glTranslatef(1.8f, 0.0f, 0.5f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.1f, 20, 20);

    glTranslatef(0.0f, 0.0f, -0.3f);
    gluCylinder(quadric, 0.07f, 0.03f, 0.3f, 10, 10);

    glPopMatrix();

#pragma endregion Main_Doar

}

void Bushes(void)
{
    glBindTexture(GL_TEXTURE_2D, Light_Green);

    glPushMatrix();

    glTranslatef(-0.6f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, Dark_Green);

    glPushMatrix();

    glTranslatef(0.6f, 0.0f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, Dim_Green);

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.6f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, Dark_Green);
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -0.6f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Light_Green);

    glTranslatef(0.3f, 0.6f, 0.4f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();



    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Dark_Green);

    glTranslatef(-0.3f, 0.6f, 0.4f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Dim_Green);

    glTranslatef(0.0f, 0.6f, -0.4f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Dim_Green);

    glTranslatef(0.0f, 1.2f, 0.0f);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5f, 30, 30);

    glPopMatrix();
}

void House_Tree(void)
{
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, tree_Trunk);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.1f, 0.1f, 0.5f, 20, 20);

    glTranslatef(0.0f, 0.0f, 0.3f);

    glBindTexture(GL_TEXTURE_2D, Dark_Green);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.1f, 0.5f, 20, 20);

    glTranslatef(0.0f, 0.0f, 0.3f);

    glBindTexture(GL_TEXTURE_2D, Dim_Green);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.1f, 0.5f, 20, 20);

    glTranslatef(0.0f, 0.0f, 0.3f);

    glBindTexture(GL_TEXTURE_2D, Light_Green);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.0f, 0.5f, 20, 20);

    glDisable(GL_TEXTURE_2D);
    glTranslatef(0.0f, 0.0f, 0.25f);

    //glBindTexture(GL_TEXTURE_2D, Light_Green);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.15f, 0.0f, 0.3f, 20, 20);


    glPopMatrix();
}

void Tree(void)
{
    //Bark Began
#pragma region Bark_Texture

   //(Bark)Khod left
    glPushMatrix();

    glTranslatef(-0.12f, 0.0f, 2.5f);
    glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.15f, 0.1f, 1.0f, 15, 15);

    glPopMatrix();

    //(Bark)Khod left
    glPushMatrix();

    glTranslatef(-0.12f, 0.0f, 2.5f);
    glRotatef(340.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.15f, 0.1f, 1.0f, 15, 15);

    glPopMatrix();

    //(Bark)Khod Fifth
    glPushMatrix();

    glTranslatef(-0.05f, 0.0f, 1.55f);
    glRotatef(357.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.2f, 0.15f, 1.0f, 15, 15);


    //Sub Bark
    glTranslatef(0.0f, 0.0f, 0.5f);
    glRotatef(300.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.15f, 0.1f, 0.8f, 15, 15);

    glTranslatef(0.0f, 0.0f, 0.75f);
    glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.1f, 0.1f, 0.4f, 15, 15);

    glTranslatef(0.0f, 0.0f, 0.35f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.1f, 0.1f, 0.4f, 15, 15);

    glPopMatrix();

    //(Bark)Khod Forth
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.59f);
    glRotatef(357.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.2f, 1.0f, 15, 15);


    //Sub bark
    glTranslatef(0.0f, 0.0f, 0.59f);
    glRotatef(60.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.25f, 0.2f, 1.0f, 15, 15);


    glTranslatef(0.0f, 0.0f, 0.95f);
    glRotatef(350.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.2f, 0.15f, 1.0f, 15, 15);

    glTranslatef(0.0f, 0.0f, 0.95f);
    glRotatef(350.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.15f, 0.1f, 0.7f, 15, 15);


    glPopMatrix();


    //Khod Third 
    glPushMatrix();

    glTranslatef(0.09f, 0.0f, -0.4f);
    glRotatef(355.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.4f, 0.3f, 1.0f, 15, 15);


    //Sub bark
    glTranslatef(0.0f, 0.0f, 0.75f);
    glRotatef(300.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.3f, 0.25f, 1.0f, 15, 15);


    glTranslatef(0.0f, 0.0f, 0.88f);
    glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.27f, 0.15f, 1.0f, 15, 15);

    glTranslatef(0.0f, 0.0f, 0.95f);
    glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.15f, 0.1f, 1.0f, 15, 15);

    glPopMatrix();

    //Khod  Second
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -1.3f);
    glRotatef(5.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.5f, 0.4f, 1.0f, 15, 15);

    glPopMatrix();

    //Khod Bottom 
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -2.3f);
    //glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluCylinder(quadric, 0.6f, 0.48f, 1.2f, 15, 15);

    glPopMatrix();

#pragma endregion Bark_Texture

    //Leaf Began 
#pragma region Leaf_Texture

    //Bark Right
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Leaf_Texture);
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 3.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.8f, 30, 30);

    glPopMatrix();

    //Bark Left
    glPushMatrix();

    glTranslatef(-2.0f, 0.0f, 3.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.8f, 30, 30);

    glPopMatrix();

    //Bark Big One
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, Leaf_Texture);
    glPushMatrix();
    glTranslatef(0.0f, -0.3f, 5.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 1.0f, 30, 30);


    glTranslatef(0.55f, 1.23f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 1.0f, 30, 30);

    glTranslatef(-0.9f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 1.0f, 30, 30);

    glPopMatrix();

    //Middle Left 
    glPushMatrix();

    glTranslatef(0.65f, 0.0f, 4.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.9f, 30, 30);

    glPopMatrix();

    //Middle Right 
    glPushMatrix();

    glTranslatef(-0.65f, 0.0f, 4.0f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.9f, 30, 30);

    glPopMatrix();


    //Middle back
    glPushMatrix();

    glTranslatef(0.0f, 1.3f, 3.7f);
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.9f, 30, 30);

    glPopMatrix();


#pragma endregion Leaf_Texture    

   
}

void Sky(void)
{
    
    glBindTexture(GL_TEXTURE_2D, Sky_Texture);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-100.0f, 20.0f, -150.0f);
    glTexCoord2f(100.0f, 0.0f);
    glVertex3f(-100.0f, 20.0f, 150.0f);
    glTexCoord2f(100.0f, 100.0f);
    glVertex3f(100.0f, 20.0f, 150.0f);
    glTexCoord2f(0.0f, 100.0f);
    glVertex3f(100.0f, 20.0f, -150.0f);

    glEnd();
   
}

void Land(void)
{
    glBindTexture(GL_TEXTURE_2D, Grass_Texture);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-100.0f, -1.0f, -150.0f);
    glTexCoord2f(100.0f, 0.0f);
    glVertex3f(-100.0f, -1.0f, 150.0f);
    glTexCoord2f(100.0f, 100.0f);
    glVertex3f(100.0f, -1.0f, 150.0f);
    glTexCoord2f(0.0f, 100.0f);
    glVertex3f(100.0f, -1.0f, -150.0f);

    glEnd();
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

    if (quadric)
    {
        gluDeleteQuadric(quadric);
        quadric = NULL;
    }

    if (gpFile_DM)
    {
        fclose(gpFile_DM);
        gpFile_DM = NULL;
    }

}

