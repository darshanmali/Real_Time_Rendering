//
//  MyView.m
//  01.Window
//
//  Created by Darshan Mali on 04/11/21.
// 24 sphere


#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import "GLESView.h"

#include "vmath.h"
#import "Sphere.h"

using namespace vmath;

enum
{
    DVM_ATTTRIBUTE_POSITION = 0,
    DVM_ATTTRIBUTE_COLOR,
    DVM_ATTTRIBUTE_NORMAL,
    DVM_ATTTRIBUTE_TEXCORD,
};

bool X_Rotation = true;
bool Y_Rotation = false;
bool Z_Rotation = false;


int x;
int y;
int w;
int h;
int iterator_Color;

GLsizei gwidth;
GLsizei gheight;



GLfloat material_Ambient[24][3] = {

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

GLfloat material_Diffuse[24][3] = {
    
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

GLfloat material_Specular[24][3] = {
    
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

GLfloat Shininess[24] = {
    0.6f * 128.0f,
    0.1f * 128.0f,
 
    0.3f * 128.0f,
    0.088f * 128.0f,
    
    0.6f * 128.0f,
    0.1f * 128.0f,
    
    0.21794872f *128.0f,
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

    /*Fragment shader */
    GLuint gVertexShaderObject_DM;
    GLuint gFragmentShaderObject_DM;
    GLuint gshaderProgramObject_DM;
    

    GLuint g_Sphere_Vao_DM;
    GLuint g_Sphere_Vertex_Vbo_DM;
    GLuint g_Sphere_Normal_Vbo_DM;
    GLuint g_Sphere_Element_Vbo_DM;

    /*New For Sphere*/
    float sphere_vertices[1146];
    float sphere_normals[1146];
    float sphere_textures[764];
    unsigned short sphere_elements[2280];

    GLuint gNumVertices;
    GLuint gNumElements;

    /*splite 1.model view, 2.projection matrics */
        GLuint gMMatrixUniform_DM;
        GLuint gPMatrixUniform_DM;
        GLuint gVMatrixUniform_DM;
        
    GLuint LA_Uniform_DM[1];/*Ambient*/
    GLuint LD_Uniform_DM[1];/*Deffuse*/
    GLuint LS_Uniform_DM[1];/*Specular*/
    GLuint LPosition_Uniform_DM[1];/*Positon*/

    GLuint KA_Uniform_DM;/*Material Ambient*/
    GLuint KD_Uniform_DM;/*Material Deffuse*/
    GLuint KS_Uniform_DM;/*Material Specular*/
    GLuint shininess;

    
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

        const GLchar* vertexShaderSourceCode_PF =
            "#version 300 es\n" \
            "precision mediump float;\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
                    
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_p_matrix;" \
            
        "uniform vec4 u_light_position;" \
                
        "out vec3 transformed_normal;"  \
        "out vec4 eye_cordinates;\n" \
        
            "void main(void){\n" \
 
        "eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" \
        
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        
        "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
            "}";

                glShaderSource(gVertexShaderObject_DM, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);


                glCompileShader(gVertexShaderObject_DM);
                /*Here there should be vertex shader complication error Cheking */

                int iInfoLogLength = 0;
                int iShaderCompileStatus = 0;
                char *szInfoLog = NULL;

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

                const GLchar* fragmentShaderSourceCode_PF =
                    "#version 300 es\n" \
                    "precision highp float;"\
                    "precision highp int;\n"\
                    
        "uniform vec3 u_la[1];" \
        "uniform vec3 u_ld[1];" \
        "uniform vec3 u_ls[1];" \
        "uniform vec4 u_light_position[1];" \
        
        
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_shininess;\n" \
        
        "in vec3 transformed_normal;\n"  \
        "in vec4 eye_cordinates;\n" \
    
        "out vec4 FragColor;\n" \
        "vec3 fong_light[1];" \
        
        "void main(void){\n" \
        "vec3 color;\n"\
       
        "vec3 normalized_transformed_normal = transformed_normal;\n" \
        
        "vec3 light_direction[1];"\
        "vec3 view_vector[1];"\
        "vec3 reflection_vector[1];"\
        "vec3 specular[1];"\
        "vec3 diffuse[1];"\
        "vec3 ambient[1];"\
        
        "for(int i = 0; i< 1; i++){\n"\
        
        "view_vector[i] =normalize(-eye_cordinates.xyz);\n"\
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" \
        "reflection_vector[i] = reflect(-light_direction[i] , transformed_normal);\n" \
        
        "ambient[i] = u_la[i] * u_ka;\n" \
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" \
        "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector[i]), 0.0), u_shininess);\n" \

        "fong_light[i] = ambient[i] + diffuse[i] + specular[i];\n" \

        "}\n" \
    
        "color = fong_light[0];"\
    
        "FragColor = vec4(color, 1.0f);\n"\
        "}";

                glShaderSource(gFragmentShaderObject_DM, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);

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

                int iShaderProgramLinkStatus = 0;
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
                            [self uninitialize];
                        }
                    }
                }

                gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_model_matrix");
                gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_view_matrix");
                gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_p_matrix");
                
        LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_la[0]");
        LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_ld[0]");
        LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_ls[0]");
        LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position[0]");

        KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ka");
        KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_kd");
        KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ks");
        shininess = glGetUniformLocation(gshaderProgramObject_DM, "u_shininess");
        
        /* New for Sphere */
        Sphere sphere;
        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        gNumVertices = sphere.getNumberOfSphereVertices();
        gNumElements = sphere.getNumberOfSphereElements();

            /* Sphere */
        glGenVertexArrays(1, &g_Sphere_Vao_DM);
        glBindVertexArray(g_Sphere_Vao_DM);
 
        /* For Vertices */
        glGenBuffers(1, &g_Sphere_Vertex_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Sphere_Vertex_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /* For Normals */
        glGenBuffers(1, &g_Sphere_Normal_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Sphere_Normal_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        /* For elements */
        glGenBuffers(1, &g_Sphere_Element_Vbo_DM);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        

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
    gwidth = Width;
    gheight = Height;
    
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

                if(X_Rotation)
                {
                
                GLfloat light_position[] = { 0.0f, 1.5f * sinf(C_angle), 2.5f * cosf(C_angle), 1.0f };
                
                glUniform4fv(LPosition_Uniform_DM[0], 1, (GLfloat *)light_position);
                
                }

                if(Y_Rotation)
                {
                    
                GLfloat light_position2[] = { 1.5f * sinf(C_angle), 0.0f, 2.5f * cosf(C_angle), 1.0f };
                
                glUniform4fv(LPosition_Uniform_DM[0], 1, (GLfloat *)light_position2);
                
                }

                if(Z_Rotation)
                {
                    
                GLfloat light_position3[] = { 1.5f * sinf(C_angle), 2.5f * cosf(C_angle), 0.0f, 1.0f };
                
                glUniform4fv(LPosition_Uniform_DM[0], 1, (GLfloat *)light_position3);

                }
    
    glUniform3f(LA_Uniform_DM[0], 0.0f, 0.0f, 0.0f);
    glUniform3f(LD_Uniform_DM[0], 1.0f, 1.0f, 1.0f);
    glUniform3f(LS_Uniform_DM[0], 1.0f, 1.0f, 1.0f);
    
    
    mat4 ModelMatrix;
    mat4 ViewMatrix;
    mat4 PMatrix;
    mat4 TranslateMatrix;
   
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {

        x = j * (gwidth / 6);
        y = i * (gwidth / 6);
        w = gwidth / 6;
        h = gheight / 6;
        
        glViewport( x, y, w, h);
    
        ModelMatrix = mat4::identity();
        ViewMatrix = mat4::identity();
        TranslateMatrix = mat4::identity();
        PMatrix = mat4::identity();

        glUniform3fv(KA_Uniform_DM, 1,  material_Ambient[iterator_Color]);
        glUniform3fv(KD_Uniform_DM, 1,  material_Diffuse[iterator_Color]);
        glUniform3fv(KS_Uniform_DM, 1,  material_Specular[iterator_Color]);
        glUniform1f(shininess,Shininess[iterator_Color]);


        TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
        
        ModelMatrix = TranslateMatrix;

        PMatrix = gPerspectiveProjectMatix_DM;

        glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
        glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

        glBindVertexArray(g_Sphere_Vao_DM);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
        glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);
        
        iterator_Color++;
            
        if(iterator_Color > 24)
        {
            iterator_Color = 0;
        }
        }
    }
    if(iterator_Color != 0)
    {
        iterator_Color = 0;
    }

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
      C_angle = C_angle + 0.05f;
  
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
    X_Rotation = true;
    Y_Rotation = false;
    Z_Rotation = false;
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    //code

    X_Rotation = false;
    Y_Rotation = true;
    Z_Rotation = false;
    
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
    X_Rotation = false;
    Y_Rotation = false;
    Z_Rotation = true;
    
    
    
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
                if (g_Sphere_Normal_Vbo_DM)
                {
                    glDeleteBuffers(1, &g_Sphere_Normal_Vbo_DM);
                    g_Sphere_Normal_Vbo_DM = 0;
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

