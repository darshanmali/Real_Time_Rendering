//
//  MyView.m
//  01.Window
//
//  Created by Darshan Mali on 04/11/21.
// Per Fragment per vertex toggling.

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

bool bAnimate = false;
bool bLights = false;

bool per_Vertex = true;
bool per_Fragment = false;

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f };
GLfloat lDefues[] = { 1.0f, 1.0f, 1.0f };
GLfloat lSpecular[] = { 1.0f, 1.0f, 1.0f };
GLfloat lPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };
GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 50.0f;

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
    
    /* Fragment shader */
    GLuint gshaderProgramObject_PF_DM;
    GLuint gVertexShaderObject_PF_DM;
    GLuint gFragmentShaderObject_PF_DM;
    
    /* Vertex shader */
    GLuint gshaderProgramObject_PV_DM;
    GLuint gVertexShaderObject_PV_DM;
    GLuint gFragmentShaderObject_PV_DM;
    
    
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
    GLuint gVMatrixUniform_DM;
    GLuint gPMatrixUniform_DM;

    GLuint LKeyPressedUniform_DM;/* key passing */

    GLuint LA_Uniform_DM;/*Ambient*/
    GLuint LD_Uniform_DM;/*Deffuse*/
    GLuint LS_Uniform_DM;/*Specular*/
    GLuint LPosition_Uniform_DM;/*Positon*/

    GLuint KA_Uniform_DM;/*Material Ambient*/
    GLuint KD_Uniform_DM;/*Material Deffuse*/
    GLuint KS_Uniform_DM;/*Material Specular*/
    GLuint shininess;

/*splite 1.model view, 2.projection matrics */
    GLuint gMMatrixUniform_PV_DM;
    GLuint gVMatrixUniform_PV_DM;
    GLuint gPMatrixUniform_PV_DM;

    GLuint LKeyPressedUniform_PV_DM;/* key passing */

    GLuint LA_Uniform_PV_DM;/*Ambient*/
    GLuint LD_Uniform_PV_DM;/*Deffuse*/
    GLuint LS_Uniform_PV_DM;/*Specular*/
    GLuint LPosition_Uniform_PV_DM;/*Positon*/

    GLuint KA_Uniform_PV_DM;/*Material Ambient*/
    GLuint KD_Uniform_PV_DM;/*Material Deffuse*/
    GLuint KS_Uniform_PV_DM;/*Material Specular*/
    GLuint shininess_PV_DM;
    
    
    
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
        
        /*************************** vertex shader ****************************************************/

        /* Vertex Shader Object */
        gVertexShaderObject_PV_DM = glCreateShader(GL_VERTEX_SHADER);
    
        const GLchar* vertexShaderSourceCode_PV =
            "#version 300 es\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_p_matrix;" \
            "uniform int u_lKey;" \
            "uniform vec3 u_la;" \
            "uniform vec3 u_ld;" \
            "uniform vec3 u_ls;" \
            "uniform vec3 u_ka;" \
            "uniform vec3 u_kd;" \
            "uniform vec3 u_ks;" \
            "uniform vec4 u_light_position;" \
            "uniform float u_shininess;\n" \
            "out vec3 fong_ads_light;\n" \
            "void main(void){\n" \
            "if(u_lKey == 1){\n" \
            "vec4 eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" \
            "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
            "vec3 light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" \
            "vec3 reflection_vector = reflect(-light_direction , transformed_normal);\n" \
            "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" \
            "vec3 ambient = u_la * u_ka;\n" \
            "vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);\n" \
            "vec3 specular = u_ls * u_ks * pow( max(dot(reflection_vector, view_vector), 0.0), u_shininess);\n" \
            "fong_ads_light = ambient + diffuse + specular;\n" \
            "}\n" \
            "else{\n" \
            "fong_ads_light = vec3(1.0f, 1.0f, 1.0f);" \
            "}\n" \
            "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
            "}";
    
        glShaderSource(gVertexShaderObject_PV_DM, 1, (const GLchar**)&vertexShaderSourceCode_PV, NULL);
    
    
        glCompileShader(gVertexShaderObject_PV_DM);
    
        GLint iInfoLogLength = 0;
        GLint iShaderCompileStatus = 0;
        char* szInfoLog = NULL;
    
        glGetShaderiv(   gVertexShaderObject_PV_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(   gVertexShaderObject_PV_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(   gVertexShaderObject_PV_DM, iInfoLogLength, &written, szInfoLog);
                    printf("Vertex Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    
                }
            }
        }
    
        /* Fragment Shader Object */
        gFragmentShaderObject_PV_DM = glCreateShader(GL_FRAGMENT_SHADER);
    
        const GLchar* fragmentShaderSourceCode =
        "#version 300 es\n" \
        "precision highp float;"\
        "precision highp int;\n"\
        
            "in vec3 fong_ads_light;\n" \
            "out vec4 FragColor;\n" \
            "void main(void){\n" \
            "FragColor = vec4(fong_ads_light, 1.0f);\n"\
            "}";
    
        glShaderSource(gFragmentShaderObject_PV_DM, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
    
        glCompileShader(gFragmentShaderObject_PV_DM);
        /*Here there should be Fragment shader complication error Cheking */
    
        glGetShaderiv(gFragmentShaderObject_PV_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(gFragmentShaderObject_PV_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gFragmentShaderObject_PV_DM, iInfoLogLength, &written, szInfoLog);
                    printf( "Fragment Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    
                }
            }
        }

    
    
        gshaderProgramObject_PV_DM = glCreateProgram();
    
        glAttachShader(gshaderProgramObject_PV_DM, gVertexShaderObject_PV_DM);
        glAttachShader(gshaderProgramObject_PV_DM, gFragmentShaderObject_PV_DM);
    
        glBindAttribLocation(gshaderProgramObject_PV_DM, DVM_ATTTRIBUTE_POSITION, "vPosition");
        glBindAttribLocation(gshaderProgramObject_PV_DM, DVM_ATTTRIBUTE_NORMAL, "vNormal");
    
        glLinkProgram(gshaderProgramObject_PV_DM);
    
        GLint iShaderProgramLinkStatus = 0;
        glGetProgramiv(gshaderProgramObject_PV_DM, GL_LINK_STATUS, &iShaderProgramLinkStatus);
        if (iShaderProgramLinkStatus == GL_FALSE)
        {
            glGetProgramiv(gshaderProgramObject_PV_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gshaderProgramObject_PV_DM, iInfoLogLength, &written, szInfoLog);
                    printf( "Shader program Link Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    
                }
            }
        }
    
        gMMatrixUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_model_matrix");
        gVMatrixUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_view_matrix");
        gPMatrixUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_p_matrix");
    
        LKeyPressedUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_lKey");
    
        LA_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_la");
        LD_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ld");
        LS_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ls");
        LPosition_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_light_position");
    
        KA_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ka");
        KD_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_kd");
        KS_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ks");
        shininess_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_shininess");

        /* New for Sphere */
        Sphere sphere;
        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        gNumVertices = sphere.getNumberOfSphereVertices();
        gNumElements = sphere.getNumberOfSphereElements();

        /* Sphere */
        glGenVertexArrays(1, &g_Sphere_Vao_DM);
        glBindVertexArray(g_Sphere_Vao_DM);

        glGenBuffers(1, &g_Sphere_Vertex_Vbo_DM);

        glBindBuffer(GL_ARRAY_BUFFER, g_Sphere_Vertex_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

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
        
        glBindVertexArray(0);
        
        /*************************************Fragment shader *********/
        
        
        
        /* Vertex Shader Object */
        gVertexShaderObject_PF_DM = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexShaderSourceCode_PF =
            "#version 300 es\n" \
            "precision mediump float;\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
                    
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_p_matrix;" \
            "uniform int u_lKey;" \
        
        "uniform vec4 u_light_position;" \
                
        "out vec3 transformed_normal;"  \
        "out vec3 light_direction;\n" \
        "out vec3 view_vector;\n" \
        
            "void main(void){\n" \
 
        "vec4 eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" \
        
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        
        "light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" \
        
        "view_vector = normalize(-eye_cordinates.xyz); \n" \
    
        "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
            "}";

                glShaderSource(gVertexShaderObject_PF_DM, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);


                glCompileShader(gVertexShaderObject_PF_DM);
                /*Here there should be vertex shader complication error Cheking */

                iInfoLogLength = 0;
                iShaderCompileStatus = 0;
                 szInfoLog = NULL;

                glGetShaderiv(gVertexShaderObject_PF_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
                if (iShaderCompileStatus == GL_FALSE)
                {
                    glGetShaderiv(gVertexShaderObject_PF_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

                    if (iInfoLogLength > 0)
                    {
                        szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                        if (szInfoLog != NULL)
                        {
                            GLsizei written;
                            glGetShaderInfoLog(gVertexShaderObject_PF_DM, iInfoLogLength, &written, szInfoLog);
                            printf("Vertex Shader Compilation Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self uninitialize];
                            
                        }
                    }
                }

                /* Fragment Shader Object */
                gFragmentShaderObject_PF_DM = glCreateShader(GL_FRAGMENT_SHADER);

                const GLchar* fragmentShaderSourceCode_PF =
                    "#version 300 es\n" \
                    "precision highp float;"\
                    "precision highp int;\n"\
                    
        "uniform vec3 u_la;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_ls;" \
        "uniform vec4 u_light_position;" \
    
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_shininess;\n" \
        
        "in vec3 transformed_normal;\n"  \
        "in vec3 light_direction;\n" \
        "in vec3 view_vector;\n" \
         
        "uniform int u_lKey;\n" \
        "out vec4 FragColor;\n" \
        
        "void main(void){\n" \
        "vec4 color;\n"\
        
        "if(u_lKey == 1){\n" \
       
        "vec3 normalized_transformed_normal = transformed_normal;\n" \
        "vec3 normalized_light_direction = light_direction;\n" \
        "vec3 normalized_view_vector = view_vector;\n" \
        "vec3 reflection_vector = reflect(-light_direction , transformed_normal);\n" \
        
        "vec3 ambient = u_la * u_ka;\n" \
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normal), 0.0f);\n" \
        "vec3 specular = u_ls * u_ks * pow( max(dot(reflection_vector, normalized_view_vector), 0.0f), u_shininess);\n" \
        "vec3 light = ambient + diffuse + specular;\n" \

        "color = vec4( light, 1.0f);\n" \
        "}\n" \
        "else{\n" \
        "color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
        "}\n"
        "FragColor = color;\n"\
        "}";

                glShaderSource(gFragmentShaderObject_PF_DM, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);

                glCompileShader(gFragmentShaderObject_PF_DM);
                /*Here there should be Fragment shader complication error Cheking */

                glGetShaderiv(gFragmentShaderObject_PF_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
                if (iShaderCompileStatus == GL_FALSE)
                {
                    glGetShaderiv(gFragmentShaderObject_PF_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

                    if (iInfoLogLength > 0)
                    {
                        szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                        if (szInfoLog != NULL)
                        {
                            GLsizei written;
                            glGetShaderInfoLog(gFragmentShaderObject_PF_DM, iInfoLogLength, &written, szInfoLog);
                            printf( "Fragment Shader Compilation Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self uninitialize];//Myview release hoil
                        }
                    }
                }

                gshaderProgramObject_PF_DM = glCreateProgram();

                glAttachShader(gshaderProgramObject_PF_DM, gVertexShaderObject_PF_DM);
                glAttachShader(gshaderProgramObject_PF_DM, gFragmentShaderObject_PF_DM);

                glBindAttribLocation(gshaderProgramObject_PF_DM, DVM_ATTTRIBUTE_POSITION, "vPosition");
                glBindAttribLocation(gshaderProgramObject_PF_DM, DVM_ATTTRIBUTE_NORMAL, "vNormal");
            
                glLinkProgram(gshaderProgramObject_PF_DM);

                iShaderProgramLinkStatus = 0;
                glGetProgramiv(gshaderProgramObject_PF_DM, GL_LINK_STATUS, &iShaderProgramLinkStatus);
                if (iShaderProgramLinkStatus == GL_FALSE)
                {
                    glGetProgramiv(gshaderProgramObject_PF_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);
                    if (iInfoLogLength > 0)
                    {
                        szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                        if (szInfoLog != NULL)
                        {
                            GLsizei written;
                            glGetShaderInfoLog(gFragmentShaderObject_PF_DM, iInfoLogLength, &written, szInfoLog);
                            printf("Shader program Link Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self release];//Myview release hoil
                            
                        }
                    }
                }

                gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_model_matrix");
                gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_view_matrix");
                gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_p_matrix");
                LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_lKey");
                
                
        LA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_la");
        LD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ld");
        LS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ls");
        LPosition_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_light_position");

        KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ka");
        KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_kd");
        KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ks");
        shininess = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_shininess");
        
        /* Sphere */
        glGenVertexArrays(1, &g_Sphere_Vao_DM);
        
        /*For Vertex */
        glBindVertexArray(g_Sphere_Vao_DM);
        
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

        glBindVertexArray(0);

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
    
    if (per_Vertex)
   {
       glUseProgram(gshaderProgramObject_PV_DM);

       if (bLights)
       {
           glUniform1i(LKeyPressedUniform_PV_DM, 1);
           glUniform1f(shininess_PV_DM, Shininess);
           glUniform3fv(LA_Uniform_PV_DM, 1, (GLfloat*)lAmbient);
           glUniform3fv(LD_Uniform_PV_DM, 1, (GLfloat*)lDefues);
           glUniform3fv(LS_Uniform_PV_DM, 1, (GLfloat*)lSpecular);
           glUniform3fv(KA_Uniform_PV_DM, 1, (GLfloat*)material_Ambient);
           glUniform3fv(KD_Uniform_PV_DM, 1, (GLfloat*)material_Diffuse);
           glUniform3fv(KS_Uniform_PV_DM, 1, (GLfloat*)material_Specular);
           glUniform4fv(LPosition_Uniform_PV_DM, 1, (GLfloat*)lPosition);
       }
       else
       {
           glUniform1i(LKeyPressedUniform_PV_DM, 0);
       }
   }
   if (per_Fragment)
   {
       glUseProgram(gshaderProgramObject_PF_DM);

       if (bLights)
       {
           glUniform1i(LKeyPressedUniform_DM, 1);
           glUniform1f(shininess, Shininess);
           glUniform3fv(LA_Uniform_DM, 1, (GLfloat*)lAmbient);
           glUniform3fv(LD_Uniform_DM, 1, (GLfloat*)lDefues);
           glUniform3fv(LS_Uniform_DM, 1, (GLfloat*)lSpecular);
           glUniform3fv(KA_Uniform_DM, 1, (GLfloat*)material_Ambient);
           glUniform3fv(KD_Uniform_DM, 1, (GLfloat*)material_Diffuse);
           glUniform3fv(KS_Uniform_DM, 1, (GLfloat*)material_Specular);
           glUniform4fv(LPosition_Uniform_DM, 1, (GLfloat*)lPosition);
       }
       else
       {
           glUniform1i(LKeyPressedUniform_DM, 0);
       }
   }
            mat4 TranslateMatrix = mat4::identity();
            mat4 RotaionMatrix = mat4::identity();
            mat4 ViewMatrix  = mat4::identity();;
            mat4 PMatrix;
            mat4 ModelMatrix;
   
            ModelMatrix = mat4::identity();
    
            TranslateMatrix = vmath::translate( 0.0f, 0.0f, -3.0f);
            RotaionMatrix = vmath::rotate(C_angle, 0.0f, 1.0f, 0.0f);
            ModelMatrix = TranslateMatrix * RotaionMatrix;

            PMatrix = gPerspectiveProjectMatix_DM;
    if(per_Vertex){
        glUniformMatrix4fv(gPMatrixUniform_PV_DM, 1, GL_FALSE, PMatrix);
        glUniformMatrix4fv(gVMatrixUniform_PV_DM, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(gMMatrixUniform_PV_DM, 1, GL_FALSE, ModelMatrix);
    }

    if(per_Fragment){
        glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
        glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);
    }

            glBindVertexArray(g_Sphere_Vao_DM);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
            glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

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
    
    if (per_Vertex == false)
    {
        per_Vertex = true;
        per_Fragment = false;
    }
    else
    {
        per_Vertex = true;
    }
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
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
    if (per_Fragment == false)
    {
        per_Fragment = true;
        per_Vertex = false;
    }
    else
    {
        per_Fragment = true;
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
                if (g_Sphere_Normal_Vbo_DM)
                {
                    glDeleteBuffers(1, &g_Sphere_Normal_Vbo_DM);
                    g_Sphere_Normal_Vbo_DM = 0;
                }

                if (gshaderProgramObject_PF_DM)
                {
                    glUseProgram(gshaderProgramObject_PF_DM);
                    GLsizei shaderCount;
                    glGetProgramiv(gshaderProgramObject_PF_DM, GL_ATTACHED_SHADERS, &shaderCount);
                    GLuint* pShader = NULL;
                    pShader = (GLuint*)malloc(sizeof(GLuint) * shaderCount);
                    if (pShader == NULL)
                    {
                        printf("\nUnable to allocate memory to pShader.\n");
                    }
                    glGetAttachedShaders(gshaderProgramObject_PF_DM, shaderCount, &shaderCount, pShader);
                    for (GLsizei i = 0; i < shaderCount; i++)
                    {
                        glDetachShader(gshaderProgramObject_PF_DM, pShader[i]);
                    }
                    free(pShader);

                    glDeleteShader(gVertexShaderObject_PF_DM);
                    gVertexShaderObject_PF_DM = 0;

                    glDeleteShader(gFragmentShaderObject_PF_DM);
                    gFragmentShaderObject_PF_DM = 0;

                    glDeleteProgram(gshaderProgramObject_PF_DM);
                    gshaderProgramObject_PF_DM = 0;

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
