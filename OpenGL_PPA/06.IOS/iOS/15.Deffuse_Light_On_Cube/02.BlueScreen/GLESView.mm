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

using namespace vmath;

enum
{
    DVM_ATTTRIBUTE_POSITION = 0,
    DVM_ATTTRIBUTE_COLOR,
    DVM_ATTTRIBUTE_NORMAL,
    DVM_ATTTRIBUTE_TEXCORD,
};

bool bAnimate = false;
bool bLights = false;

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
    
    GLuint gVertexShaderObject_DM;
    GLuint gFragmentShaderObject_DM;
    GLuint gshaderProgramObject_DM;
    
    GLuint g_Cube_Vao_DM;
    GLuint g_Cube_Vertex_Vbo_DM;
    GLuint g_Cube_Normal_Vbo_DM;
    
    /*splite 1.model view, 2.projection matrics */
        GLuint gMVMatrixUniform_DM;
        GLuint gPMatrixUniform_DM;

        GLuint LKeyPressedUniform_DM;/* key passing */
        GLuint LD_Uniform_DM;/*Deffuse*/
        GLuint KD_Uniform_DM;/*Material*/
        GLuint LPosition_Uniform_DM;/*Positon*/
    
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
                    "precision mediump float;\n" \
                    "in vec4 vPosition;" \
                    "in vec3 vNormal;" \
                    "uniform mat4 u_model_view_matrix;" \
                    "uniform mat4 u_p_matrix;" \
                    "uniform int u_lKey;" \
                    "uniform vec3 u_ld;" \
                    "uniform vec3 u_kd;" \
                    "uniform vec4 u_light_position;" \
                    "out vec3 diffuse_light;" \
                    "void main(void){\n" \
                    "if(u_lKey == 1){\n" \
                    "vec4 eye_cordinates = vec4(u_model_view_matrix * vPosition);\n" \
                    "mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));\n" \
                    "vec3 tnormal = normalize(normal_matrix * vNormal);\n" \
                    "vec3 s = normalize(vec3(u_light_position - eye_cordinates));\n" \
                    "diffuse_light = u_ld * u_kd * max( dot(s , tnormal), 0.0);\n" \
                    "}\n" \
                    "gl_Position = u_p_matrix * u_model_view_matrix * vPosition;\n" \
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
                    "precision highp int;\n"\
                    "in vec3 diffuse_light;\n" \
                    "uniform int u_lKey;\n" \
                    "out vec4 FragColor;\n" \
                    "void main(void){\n" \
                    "vec4 color;\n"\
                    "if(u_lKey == 1){\n" \
                    "color = vec4( diffuse_light, 1.0f);\n" \
                    "}\n"\
                    "else{\n"\
                    "color = vec4( 1.0f, 1.0f, 1.0f, 1.0f);\n" \
                    "}\n" \
                    "FragColor = color;\n"\
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
                glBindAttribLocation(gshaderProgramObject_DM, DVM_ATTTRIBUTE_NORMAL, "vNormal");
            
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

                gMVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_model_view_matrix");
                gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_p_matrix");
                LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_lKey");
                LD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ld");
                KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_kd");
                LPosition_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position");

        /*CUBE DATA*/
        const GLfloat CubeVertices[] =
        {
                        0.5f, 0.5f, 0.5f,
                        -0.5f, 0.5f, 0.5f,
                        -0.5f, -0.5f, 0.5f,
                        0.5f, -0.5f, 0.5f,
                        
                        0.5f, 0.5f, -0.5f,
                        0.5f, 0.5f, 0.5f,
                        0.5f, -0.5f, 0.5f,
                        0.5f, -0.5f, -0.5f,
                        
                        0.5f, 0.5f, -0.5f,
                        -0.5f, 0.5f, -0.5f,
                        -0.5f, -0.5f, -0.5f,
                        0.5f, -0.5f, -0.5f,
                        
                        -0.5f, 0.5f, -0.5f,
                        -0.5f, 0.5f, 0.5f,
                        -0.5f, -0.5f, 0.5f,
                        -0.5f, -0.5f, -0.5f,
                        
                        0.5f, 0.5f, 0.5f,
                        -0.5f, 0.5f, 0.5f,
                        -0.5f, 0.5f, -0.5f,
                        0.5f, 0.5f, -0.5f,
                         
                        0.5f, -0.5f, 0.5f,
                        -0.5f, -0.5f, 0.5f,
                        -0.5f, -0.5f, -0.5f,
                        0.5f, -0.5f, -0.5f
        };
                
        const GLfloat CubeNormals[] =
                {
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,

                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,

                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,

                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,

                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,

                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f
                };
                /* Cube */
                
                glGenVertexArrays(1, &g_Cube_Vao_DM);
                glBindVertexArray(g_Cube_Vao_DM);

                glGenBuffers(1, &g_Cube_Vertex_Vbo_DM);
                glBindBuffer(GL_ARRAY_BUFFER, g_Cube_Vertex_Vbo_DM);
                glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
                glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

                glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glGenBuffers(1, &g_Cube_Normal_Vbo_DM);
                glBindBuffer(GL_ARRAY_BUFFER, g_Cube_Normal_Vbo_DM);
                glBufferData(GL_ARRAY_BUFFER, sizeof(CubeNormals), CubeNormals, GL_STATIC_DRAW);
                glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
                glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
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
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    
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

            if (bLights)
            {
                glUniform1i(LKeyPressedUniform_DM, 1);
                glUniform3f(LD_Uniform_DM, 1.0f, 1.0f, 1.0f);
                glUniform3f(KD_Uniform_DM, 0.5f, 0.5f, 0.5f);
                GLfloat light_position[] = { 0.0f, 0.0f, 2.0f, 1.0f };
                glUniform4fv(LPosition_Uniform_DM, 1, (GLfloat *)light_position);
            }
            else
            {
                glUniform1i(LKeyPressedUniform_DM, 0);
            }
            
            mat4 TranslateMatrix = mat4::identity();
            mat4 RotaionMatrix = mat4::identity();
            mat4 ModelViewMatrix = mat4::identity();
            mat4 PMatrix;

            TranslateMatrix = vmath::translate( 0.0f, 0.0f, -3.0f);
            RotaionMatrix = vmath::rotate(C_angle, 0.0f, 1.0f, 0.0f);
            ModelViewMatrix = TranslateMatrix * RotaionMatrix;

            PMatrix = gPerspectiveProjectMatix_DM;
            glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
            glUniformMatrix4fv(gMVMatrixUniform_DM, 1, GL_FALSE, ModelViewMatrix);

            glBindVertexArray(g_Cube_Vao_DM);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
                glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
                glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
                glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
                glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

            glBindVertexArray(0);

            glUseProgram(0);

    
    [self Update];
    
    //SwapBuffer
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
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
    if (bLights == true)
    {
        bLights = false;
    }
    else
    {
        bLights = true;
    }
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
    
}

-(void)uninitialize
{
    //code
                if (g_Cube_Vao_DM)
                {
                    glDeleteVertexArrays(1, &g_Cube_Vao_DM);
                    g_Cube_Vao_DM = 0;
                }

                if (g_Cube_Vertex_Vbo_DM)
                {
                    glDeleteBuffers(1, &g_Cube_Vertex_Vbo_DM);
                    g_Cube_Vertex_Vbo_DM = 0;
                }
                if (g_Cube_Normal_Vbo_DM)
                {
                    glDeleteBuffers(1, &g_Cube_Normal_Vbo_DM);
                    g_Cube_Normal_Vbo_DM = 0;
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
