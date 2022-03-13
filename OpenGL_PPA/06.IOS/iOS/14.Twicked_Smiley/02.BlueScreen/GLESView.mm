//
//  MyView.m
//  texture 3D shapes
//
//  Created by Darshan Mali on 04/11/21.


#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import "GLESView.h"

#include "vmath.h"

using namespace vmath;

int KeyPressed = 1;


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
    
    GLuint g_Cube_Vao_DM;
    GLuint g_Cube_Vertex_Vbo_DM;
    GLuint g_Cube_Texture_Vbo_DM;
    
    GLuint gMVPMatrixUniform;
    
    GLuint texture_Sampler_Uniform;
    
    GLuint Smiley_Texture;
    
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
                    "in vec2 VTexCoord;    \n" \
                    "out vec2 out_TexCoord;    \n" \
                    "uniform mat4 u_mvp_matrix;" \
                    "void main(void){\n" \
                    "gl_Position = u_mvp_matrix * vPosition;\n" \
                    "out_TexCoord = VTexCoord;  \n" \
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
                    "uniform sampler2D u_Texture_Sampler;" \
                    "in vec2 out_TexCoord;  \n" \
                    "void main(void){\n" \
                    "FragColor = texture(u_Texture_Sampler, out_TexCoord);\n" \
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
                glBindAttribLocation(gshaderProgramObject_DM, DVM_ATTTRIBUTE_TEXCORD, "VTexCoord");
                
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
                texture_Sampler_Uniform = glGetUniformLocation(gshaderProgramObject_DM, "u_Texture_Sampler");

        
        /*CUBE DATA*/
        const GLfloat CubeVertices[] =
        {
                        0.5f, 0.5f, 0.5f,
                        -0.5f, 0.5f, 0.5f,
                        -0.5f, -0.5f, 0.5f,
                        0.5f, -0.5f, 0.5f,
                        
        };
               
                /* cube */
                glGenVertexArrays(1, &g_Cube_Vao_DM);
                glBindVertexArray(g_Cube_Vao_DM);

                glGenBuffers(1, &g_Cube_Vertex_Vbo_DM);
                glBindBuffer(GL_ARRAY_BUFFER, g_Cube_Vertex_Vbo_DM);
                glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
                glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

                glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                
        glGenBuffers(1, &g_Cube_Texture_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Cube_Texture_Vbo_DM);
        
        glBufferData(GL_ARRAY_BUFFER, (4 * 2) * sizeof(float), nil, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_TEXCORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_TEXCORD);
            
        glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);

                     
        gPerspectiveProjectMatix_DM = mat4::identity();
        
        Smiley_Texture = [self LoadTextureFromBMP:@"Smiley" :@"bmp"];
                if(!Smiley_Texture)
                {
                    printf(" Smiley Loading Image Failed..!\n");
                    [self release];
                    
                }
        
        KeyPressed = 0;
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
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

-(GLuint)LoadTextureFromBMP:(NSString *)ImageFileName :(NSString*) extanion
    {
        //code
        
        /*In mac we were loading image from path here we are loading it from signedbox */
        //NSBundle *appBundle = [NSBundle mainBundle];//packege is internally bundle
        //NSString *appDirPath = [appBundle bundlePath];//taking bundle's path
        //NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];//Bundle chya dir madhun
        //NSString *ImageFileNameWithPath = [NSString stringWithFormat:@"%@/%s",parentDirPath, ImageFileName];
        
        NSString *ImageFilenameWithPath = [[NSBundle mainBundle]pathForResource:ImageFileName ofType:extanion];
        
        UIImage *BMP_Image = [[UIImage alloc] initWithContentsOfFile:ImageFilenameWithPath];
        if(!BMP_Image)
        {
            printf(" NSImage Conversion of Image Failed..!\n");
            return(0);
        }

        //getCGImageRepresentationOfNSImage do Error Checking
        CGImageRef CGImage = [BMP_Image CGImage];

        int imageWidth = (int)CGImageGetWidth(CGImage);
        int imageHeight = (int)CGImageGetHeight(CGImage);

        //get data from image
        CFDataRef ImageData = CGDataProviderCopyData(CGImageGetDataProvider(CGImage));

        //Convert CFData Ref into void* Generic data
        //aapan aata CFDataref madhalya buffer madhun pixel cha fakt address ghet aahot pixels madhe
        void* pixels = (void *)CFDataGetBytePtr(ImageData);

        GLuint texture;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //Setting Texture Param
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        CFRelease(ImageData);
        return (texture);
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
    GLfloat quad_Texture[8];
    if (KeyPressed == 0)
    {
        quad_Texture[0] = 0.0f;
        quad_Texture[1] = 1.0f;
        
        quad_Texture[2] = 1.0f;
        quad_Texture[3] = 1.0f;
        
        quad_Texture[4] = 1.0f;
        quad_Texture[5] = 0.0f;
        
        quad_Texture[6] = 0.0f;
        quad_Texture[7] = 0.0f;
    }
    else if (KeyPressed == 1 ){
        quad_Texture[0] = 0.5f;
        quad_Texture[1] = 0.5f;
        
        quad_Texture[2] = 0.0f;
        quad_Texture[3] = 0.5f;
        
        quad_Texture[4] = 0.0f;
        quad_Texture[5] = 0.0f;
        
        quad_Texture[6] = 0.5f;
        quad_Texture[7] = 0.0f;

    }
    else if (KeyPressed == 2) {
        quad_Texture[0] = 2.0f;
        quad_Texture[1] = 2.0f;
        quad_Texture[2] = 0.0f;
        quad_Texture[3] = 2.0f;
        quad_Texture[4] = 0.0f;
        quad_Texture[5] = 0.0f;
        quad_Texture[6] = 2.0f;
        quad_Texture[7] = 0.0f;

    }
    else if (KeyPressed == 3) {

        quad_Texture[0] = 0.5f;
        quad_Texture[1] = 0.5f;
        quad_Texture[2] = 0.5f;
        quad_Texture[3] = 0.5f;
        quad_Texture[4] = 0.5f;
        quad_Texture[5] = 0.5f;
        quad_Texture[6] = 0.5f;
        quad_Texture[7] = 0.5f;
    }
    //code
    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, defaultframebuffer);
    
    //regular Code
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindRenderbuffer(GL_RENDERBUFFER, colorrenderbuffer);
    
    glUseProgram(gshaderProgramObject_DM);
            
    glBindBuffer(GL_ARRAY_BUFFER, g_Cube_Texture_Vbo_DM);
    glBufferData(GL_ARRAY_BUFFER, (4 * 2) * sizeof(float), quad_Texture, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_TEXCORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_TEXCORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


            mat4 TranslateMatrix = vmath::translate( 0.0f, 0.0f, -3.0f);
            mat4 modelViewMatrix = mat4::identity();
            mat4 modelViewProjectionMatrix = mat4::identity();

            modelViewMatrix = TranslateMatrix;

            modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

            glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Smiley_Texture);
            glUniform1i(texture_Sampler_Uniform, 0);

            glBindVertexArray(g_Cube_Vao_DM);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            
            glBindVertexArray(0);
            glUseProgram(0);

            //Update fuction call before CGFlush
            [self Update];

    
    //SwapBuffer
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void)Update
  {

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
    
    KeyPressed++;
    if(KeyPressed > 3)
    {
        KeyPressed = 0;
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
                if (g_Cube_Texture_Vbo_DM)
                {
                    glDeleteBuffers(1, &g_Cube_Texture_Vbo_DM);
                    g_Cube_Texture_Vbo_DM = 0;
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
