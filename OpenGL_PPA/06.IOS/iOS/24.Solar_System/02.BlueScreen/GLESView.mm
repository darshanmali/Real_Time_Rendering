//
//  MyView.m
//  01.Window
//
//  Created by Darshan Mali on 04/11/21.
//

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import "GLESView.h"

#include "vmath.h"

#import "Sphere.h"

using namespace vmath;

mat4 PushMatrix_DM(mat4);
void PopMatrix_DM(void);
struct Matrix_Stack_DM* CreateMatrix_DM(void);

struct Matrix_Stack_DM
{
    mat4 Matrix;
    struct Matrix_Stack_DM* Next;
}*Start = NULL, *Top = NULL, *Prev = NULL;

int PushToStack_DM = 0;
GLfloat year_DM = 0.0;
GLfloat day_DM = 0.0;
GLfloat moon_DM = 0.0;


@implementation GLESView
{
    @private
    EAGLContext *eaglContext;
    
    GLuint defaultframebuffer;
    GLuint colorrenderbuffer;
    GLuint depthRenderbuffer;
    
    id displayLink;
    NSInteger animationFrameInterval;
    BOOL isAnimation;
    
    enum
    {
        DVM_ATTTRIBUTE_POSITION = 0,
        DVM_ATTTRIBUTE_COLOR,
        DVM_ATTTRIBUTE_NORMAL,
        DVM_ATTTRIBUTE_TEXCORD,
    };
    
    GLuint gVertexShaderObject_DM;
    GLuint gFragmentShaderObject_DM;
    GLuint gshaderProgramObject_DM;
    
    GLuint g_Sphere_Vao_DM;
    GLuint g_Sphere_Vertex_Vbo_DM;
    GLuint g_Sphere_Color_Vbo_DM;
    GLuint g_Sphere_Element_Vbo_DM;
    

    /*New For Sphere*/
    float sphere_vertices[1146];
    float sphere_normals[1146];
    float sphere_textures[764];
    unsigned short sphere_elements[2280];

    GLuint gNumVertices;
    GLuint gNumElements;

    GLuint gMVPMatrixUniform;
    
    GLfloat C_angle;
    
    mat4 gPerspectiveProjectMatix_DM;
    
}


-(id)initWithFrame:(CGRect)frame
{
        
    self = [super initWithFrame:frame];
    
    if(self)
    {
        //Embeded Apple Graphics Layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)[super layer];
        [eaglLayer setOpaque:YES];//apar darshak mhanun
        [eaglLayer setDrawableProperties:[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool: NO], kEAGLDrawablePropertyRetainedBacking,
                                          kEAGLColorFormatRGBA8,kEAGLDrawablePropertyColorFormat, nil]];
        
        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if (eaglContext == nil)
        {
            printf("OpenGL-ES Context Creation Failed.\n");
            return (nil);
        }
        else
        {
            printf("OpenGL-ES Context 2 Created.\n");
        }
        
        [EAGLContext setCurrentContext:eaglContext];
        
        // Creating main buffer frame buffer
        glGenFramebuffers(1, &defaultframebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultframebuffer);
        
        //creating color buffer
        glGenRenderbuffers(1, &colorrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorrenderbuffer);
        
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorrenderbuffer);
        
        GLint backingWidth;
        GLint backingHeight;
        
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH, &backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        
        // Creating depth buffer
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE) {
            printf("Frame Buffer is not complete...!\n");
            [self uninitialize];
            return(nil);
        }
        
        printf("%s\n", glGetString(GL_RENDERBUFFER));
        printf("%s\n", glGetString(GL_VERSION));
        printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        animationFrameInterval = 60;
        isAnimation = NO;
        
        //Shader Codes should be written here...
        
        /* Vertex Shader Object */
                gVertexShaderObject_DM = glCreateShader(GL_VERTEX_SHADER);

                const GLchar* vertexShaderSourceCode =
                    "#version 300 es\n" \
                    "in vec4 vPosition; \n" \
                    "out vec3 OutColor;    \n" \
                    "uniform mat4 u_mvp_matrix;" \
                    "void main(void){\n" \
                    "gl_Position = u_mvp_matrix * vPosition;\n" \
                    "OutColor = vec3(1.0f, 1.0f, 1.0f);  \n" \
                    "}";

                glShaderSource(gVertexShaderObject_DM, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


                glCompileShader(gVertexShaderObject_DM);
                /*Here there should be vertex shader complication error Cheking */

                GLint iInfoLogLength = 0;
                GLint iShaderCompileStatus = 0;
                char* szInfoLog = NULL;

                glGetShaderiv(gVertexShaderObject_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
                if (iShaderCompileStatus == GL_FALSE)
                {
                    glGetShaderiv(gVertexShaderObject_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

                    if (iInfoLogLength > 0)
                    {
                        szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                        if (szInfoLog != NULL)
                        {
                            GLsizei written;
                            glGetShaderInfoLog(gVertexShaderObject_DM, iInfoLogLength, &written, szInfoLog);
                            printf("Vertex Shader Compilation Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self uninitialize];
                            
                        }
                    }
                }

                /* Fragment Shader Object */
                gFragmentShaderObject_DM = glCreateShader(GL_FRAGMENT_SHADER);

                const GLchar* fragmentShaderSourceCode =
                    "#version 300 es\n" \
                    "precision highp float;"\
                    "out vec4 FragColor;\n" \
                    "in vec3 OutColor;  \n" \
                    "void main(void){\n" \
                    "FragColor = vec4(OutColor, 1.0);\n" \
                    "}";

                glShaderSource(gFragmentShaderObject_DM, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

                glCompileShader(gFragmentShaderObject_DM);
                /*Here there should be Fragment shader complication error Cheking */

                glGetShaderiv(gFragmentShaderObject_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
                if (iShaderCompileStatus == GL_FALSE)
                {
                    glGetShaderiv(gFragmentShaderObject_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

                    if (iInfoLogLength > 0)
                    {
                        szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                        if (szInfoLog != NULL)
                        {
                            GLsizei written;
                            glGetShaderInfoLog(gFragmentShaderObject_DM, iInfoLogLength, &written, szInfoLog);
                            printf( "Fragment Shader Compilation Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self uninitialize];//Myview release hoil
                            
                        }
                    }
                }


                gshaderProgramObject_DM = glCreateProgram();

                glAttachShader(gshaderProgramObject_DM, gVertexShaderObject_DM);
                glAttachShader(gshaderProgramObject_DM, gFragmentShaderObject_DM);

                glBindAttribLocation(gshaderProgramObject_DM, DVM_ATTTRIBUTE_POSITION, "vPosition");

                glLinkProgram(gshaderProgramObject_DM);

                GLint iShaderProgramLinkStatus = 0;
                glGetProgramiv(gshaderProgramObject_DM, GL_LINK_STATUS, &iShaderProgramLinkStatus);
                if (iShaderProgramLinkStatus == GL_FALSE)
                {
                    glGetProgramiv(gshaderProgramObject_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);
                    if (iInfoLogLength > 0)
                    {
                        szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                        if (szInfoLog != NULL)
                        {
                            GLsizei written;
                            glGetShaderInfoLog(gFragmentShaderObject_DM, iInfoLogLength, &written, szInfoLog);
                            printf("Shader program Link Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self release];//Myview release hoil
                            
                        }
                    }
                }

                gMVPMatrixUniform = glGetUniformLocation(gshaderProgramObject_DM, "u_mvp_matrix");

            /* New for Sphere */
            Sphere sphere;
            sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
            gNumVertices = sphere.getNumberOfSphereVertices();
            gNumElements = sphere.getNumberOfSphereElements();

                /* Cube */
                
            glGenVertexArrays(1, &g_Sphere_Vao_DM);
            glBindVertexArray(g_Sphere_Vao_DM);

            glGenBuffers(1, &g_Sphere_Vertex_Vbo_DM);
            glBindBuffer(GL_ARRAY_BUFFER, g_Sphere_Vertex_Vbo_DM);
            glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
        
            /* For elements */
            glGenBuffers(1, &g_Sphere_Element_Vbo_DM);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


            glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
                
            //glClearDepth(1.0f);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
                

        gPerspectiveProjectMatix_DM = mat4::identity();
                    
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        //Gesture
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipeGesturerecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [swipeGesturerecognizer setDelegate:self];
        [self addGestureRecognizer:swipeGesturerecognizer];
        
        UILongPressGestureRecognizer *longPressGesturerecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongPress:)];
        [longPressGesturerecognizer setDelegate:self];
        [self addGestureRecognizer:longPressGesturerecognizer];
        
    }
    return (self);
}


+(Class)layerClass
{
    //code
    return ([CAEAGLLayer class]);
}


/*
-(void)drawRect:(CGRect)rect
{
    
}
*/


//openGL's Resize
-(void)layoutSubviews
{
    //code
    glBindRenderbuffer(GL_RENDERBUFFER, colorrenderbuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)[self layer]];
    
    GLint Width;
    GLint Height;
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH, &Width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT, &Height);
    
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Width, Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    glViewport(0,0, (GLsizei)Width, (GLsizei)Height);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Frame Buffer is not complete...!\n");
    }
    if(Height<0)
        Height = 1;
    
    [self drawView:nil];
    
    gPerspectiveProjectMatix_DM = vmath::perspective(44.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
            
    // ortho and percpective
}

-(void)drawView:(id)sender
{
    //code
    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, defaultframebuffer);
    
    //regular Code
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindRenderbuffer(GL_RENDERBUFFER, colorrenderbuffer);
    
    glUseProgram(gshaderProgramObject_DM);
            
    mat4 modelViewMatrix = mat4::identity();
    mat4 TranslateMatrix = mat4::identity();
    mat4 ScaleMatrix =  mat4::identity();
    mat4 RotationMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();


    TranslateMatrix = vmath::translate( 0.0f, 0.0f, -8.0f);
    ScaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);
    RotationMatrix = vmath::rotate(year_DM, 0.0f, 1.0f, 0.0f);
    
    modelViewMatrix = TranslateMatrix * ScaleMatrix * RotationMatrix ;
    modelViewMatrix = PushMatrix_DM(modelViewMatrix);
    
    modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    
    glBindVertexArray(g_Sphere_Vao_DM);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    //Earth
    modelViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    RotationMatrix = mat4::identity();
    modelViewProjectionMatrix = mat4::identity();
    ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);

    TranslateMatrix = vmath::translate(1.0f, 0.0f, 0.0f);
    RotationMatrix = vmath::rotate(day_DM, 0.0f, 1.0f, 0.0f);
    RotationMatrix = vmath::rotate(day_DM, 0.0f, 1.0f, 0.0f);

    modelViewMatrix = TranslateMatrix * ScaleMatrix * RotationMatrix ;
    modelViewMatrix = PushMatrix_DM(modelViewMatrix);
    
    modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glBindVertexArray(g_Sphere_Vao_DM);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    //Moon
    modelViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    RotationMatrix = mat4::identity();
    modelViewProjectionMatrix = mat4::identity();
    ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);

    TranslateMatrix = vmath::translate(1.0f, 0.0f, -0.0f);
    RotationMatrix = vmath::rotate(moon_DM, 0.0f, 1.0f, 0.0f);

    modelViewProjectionMatrix = mat4::identity();
    modelViewMatrix = TranslateMatrix * ScaleMatrix * RotationMatrix;
    modelViewMatrix = PushMatrix_DM(modelViewMatrix);
    
    PopMatrix_DM();
    PopMatrix_DM();
    PopMatrix_DM();

    modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glBindVertexArray(g_Sphere_Vao_DM);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    glBindVertexArray(g_Sphere_Vao_DM);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    glUseProgram(0);
    
    [self Update];
    
    //SwapBuffer
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

mat4 PushMatrix_DM(mat4 matrix)
{
    struct Matrix_Stack_DM* Temp = NULL;

    Temp = CreateMatrix_DM();
    if (Temp == NULL)
    {
        printf( "Node is empty");
    }

    Temp->Matrix = matrix;
    Temp->Next = NULL;

    if (PushToStack_DM < 1)
    {
        Top = Temp;
        Start = Temp;
        Prev = Temp;
        PushToStack_DM++;
    }
    else if(Top != NULL && PushToStack_DM > 0)
    {
        matrix = Prev->Matrix * matrix;
        Temp->Matrix = matrix;
        Prev = Temp;
        Top->Next = Temp;
        Top = Temp;
        PushToStack_DM++;
    }

    return matrix;
}

struct Matrix_Stack_DM* CreateMatrix_DM()
{
    struct Matrix_Stack_DM* Temp = NULL;
    Temp = (struct Matrix_Stack_DM*)malloc(sizeof(struct Matrix_Stack_DM));
    return(Temp);
}

void PopMatrix_DM()
{
    struct Matrix_Stack_DM* Temp = NULL;
    struct Matrix_Stack_DM* Prev_Matrix = NULL;
    Temp = Start;

    while (Temp->Next != NULL)
    {
        Prev_Matrix = Temp;
        Temp = Temp->Next;
    }
    if(Prev_Matrix != NULL)
        Prev_Matrix->Next = NULL;

    Top = Prev_Matrix;
    free(Temp);
    Temp = NULL;
    PushToStack_DM--;
}

-(void)Update
  {
      if (C_angle >= 360.0f)
      {
          C_angle = 0.0f;
      }
      C_angle = C_angle + 0.5f;
  
  }


-(void)startAnimation
{
    //code
    if(isAnimation == NO)
    {
        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink setPreferredFramesPerSecond:animationFrameInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        isAnimation = YES;
        
    }
    
}

-(void)stopAnimation
{
        //code
    if(isAnimation == YES)
    {
        [displayLink invalidate];
        displayLink = nil;
        
        isAnimation = NO;
    }
    
    
}

-(void)onSingleTap:(UITapGestureRecognizer *)gr
{
    //code

}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    //code

}

-(void)onSwipe:(UISwipeGestureRecognizer *)gr
{
    //code
    [self uninitialize];
    [self release];
    
    exit (0);
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gr
{
    //code
    
    year_DM += 2.0f;
    if (year_DM > 360.0f)
    {
        year_DM = 0.0f;
    }
}

-(void)uninitialize
{
    //code
    
                if (g_Sphere_Vao_DM)
                {
                    glDeleteVertexArrays(1, &g_Sphere_Vao_DM);
                    g_Sphere_Vao_DM = 0;
                }

                if (g_Sphere_Vertex_Vbo_DM)
                {
                    glDeleteBuffers(1, &g_Sphere_Vertex_Vbo_DM);
                    g_Sphere_Vertex_Vbo_DM = 0;
                }
                if (g_Sphere_Color_Vbo_DM)
                {
                    glDeleteBuffers(1, &g_Sphere_Color_Vbo_DM);
                    g_Sphere_Color_Vbo_DM = 0;
                }

                if (gshaderProgramObject_DM)
                {
                    glUseProgram(gshaderProgramObject_DM);
                    GLsizei shaderCount;
                    glGetProgramiv(gshaderProgramObject_DM, GL_ATTACHED_SHADERS, &shaderCount);
                    GLuint* pShader = NULL;
                    pShader = (GLuint*)malloc(sizeof(GLuint) * shaderCount);
                    if (pShader == NULL)
                    {
                        printf("\nUnable to allocate memory to pShader.\n");
                    }
                    glGetAttachedShaders(gshaderProgramObject_DM, shaderCount, &shaderCount, pShader);
                    for (GLsizei i = 0; i < shaderCount; i++)
                    {
                        glDetachShader(gshaderProgramObject_DM, pShader[i]);
                    }
                    free(pShader);

                    glDeleteShader(gVertexShaderObject_DM);
                    gVertexShaderObject_DM = 0;

                    glDeleteShader(gFragmentShaderObject_DM);
                    gFragmentShaderObject_DM = 0;

                    glDeleteProgram(gshaderProgramObject_DM);
                    gshaderProgramObject_DM = 0;

                    glUseProgram(0);
                }
    
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    
    if(colorrenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorrenderbuffer);
        colorrenderbuffer = 0;
    }
    
    if(defaultframebuffer)
    {
        glDeleteRenderbuffers(1, &defaultframebuffer);
        defaultframebuffer = 0;
    }
    
    if(eaglContext)
    {
        if([EAGLContext currentContext] == eaglContext)
        {
            [EAGLContext setCurrentContext:nil];
            [eaglContext release];
            eaglContext = nil;
        }
    }
    
}

-(void)dealloc
{
    //code
    [self uninitialize];
    
    [super dealloc];
}
@end
