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

enum
{
    DVM_ATTTRIBUTE_POSITION = 0,
    DVM_ATTTRIBUTE_COLOR,
    DVM_ATTTRIBUTE_NORMAL,
    DVM_ATTTRIBUTE_TEXCORD,
};

bool bAnimate = false;
bool bLights = false;
bool Per_Fragment = false;

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

    /* Vertex shader */
    GLuint gVertexShaderObject_PV_DM;
    GLuint gFragmentShaderObject_PV_DM;
    GLuint gshaderProgramObject_PV_DM;
    
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
        GLuint gMMatrixUniform_PV_DM;
        GLuint gPMatrixUniform_PV_DM;
        GLuint gVMatrixUniform_PV_DM;
    
    GLuint LKeyPressedUniform_PV_DM;/* key passing */
        
    GLuint LA_Uniform_PV_DM[3];/*Ambient*/
    GLuint LD_Uniform_PV_DM[3];/*Deffuse*/
    GLuint LS_Uniform_PV_DM[3];/*Specular*/
    GLuint LPosition_Uniform_PV_DM[3];/*Positon*/
    GLuint Shininess_PV_DM[3];
    
    GLuint KA_Uniform_PV_DM;/*Material Ambient*/
    GLuint KD_Uniform_PV_DM;/*Material Deffuse*/
    GLuint KS_Uniform_PV_DM;/*Material Specular*/
   
    /*splite 1.model view, 2.projection matrics */
        GLuint gMMatrixUniform_DM;
        GLuint gPMatrixUniform_DM;
        GLuint gVMatrixUniform_DM;
    
    GLuint LKeyPressedUniform_DM;/* key passing */
        
    GLuint LA_Uniform_DM[3];/*Ambient*/
    GLuint LD_Uniform_DM[3];/*Deffuse*/
    GLuint LS_Uniform_DM[3];/*Specular*/
    GLuint LPosition_Uniform_DM[3];/*Positon*/

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
        gVertexShaderObject_PV_DM = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexShaderSourceCode =
            "#version 300 es\n" \
            "precision mediump float;\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
                    
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_p_matrix;" \
            "uniform int u_lKey;" \
                    
            "uniform vec3 u_la[3];" \
            "uniform vec3 u_ld[3];" \
            "uniform vec3 u_ls[3];" \
            "uniform vec4 u_light_position[3];" \
        
            "uniform vec3 u_ka;" \
            "uniform vec3 u_kd;" \
            "uniform vec3 u_ks;" \
            "uniform float u_Shininess_PV_DM[3];\n" \
                    
            "out vec3 fong_ads_light;" \
            "vec3 fong_light[3];" \
                    
            "void main(void){\n" \
                    
            "if(u_lKey == 1){\n" \
                "vec4 eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" \
                "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
                
                "vec3 light_direction[3];"\
                "vec3 reflection_vector[3];"\
                "vec3 specular[3];"\
                "vec3 diffuse[3];"\
                "vec3 ambient[3];"\
                "vec3 view_vector[3];"\
                
                "for(int i = 0; i< 3; i++){\n"\
                
                "view_vector[i] = normalize(-eye_cordinates.xyz);\n" \

                "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" \
                "reflection_vector[i] = reflect(-light_direction[i] , transformed_normal);\n" \
                "ambient[i] = u_la[i] * u_ka;\n" \
                "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" \
                "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector[i]), 0.0), u_Shininess_PV_DM[i]);\n" \
        
                "fong_light[i] = ambient[i] + diffuse[i] + specular[i];\n" \
                
                //"fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" \
        
                "}\n" \
                "fong_ads_light = fong_light[0] + fong_light[1] + fong_light[2];"\
            "}\n" \
            "else{\n" \
                "fong_ads_light = vec3(1.0f, 1.0f, 1.0f);" \
            "}\n" \
        
            "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
            "}";

                glShaderSource(gVertexShaderObject_PV_DM, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


                glCompileShader(gVertexShaderObject_PV_DM);
                /*Here there should be vertex shader complication error Cheking */

                GLint iInfoLogLength = 0;
                GLint iShaderCompileStatus = 0;
                char* szInfoLog = NULL;

                glGetShaderiv(gVertexShaderObject_PV_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
                if (iShaderCompileStatus == GL_FALSE)
                {
                    glGetShaderiv(gVertexShaderObject_PV_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

                    if (iInfoLogLength > 0)
                    {
                        szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                        if (szInfoLog != NULL)
                        {
                            GLsizei written;
                            glGetShaderInfoLog(gVertexShaderObject_PV_DM, iInfoLogLength, &written, szInfoLog);
                            printf("Vertex Shader Compilation Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self uninitialize];
                            
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
                    "uniform int u_lKey;\n" \
                    "out vec4 FragColor;\n" \
                    "void main(void){\n" \
                    "vec4 color;\n"\
                    "color = vec4( fong_ads_light, 1.0f);\n" \
                    "FragColor = color;\n"\
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
                            [self uninitialize];//Myview release hoil
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
                            glGetShaderInfoLog(gFragmentShaderObject_PV_DM, iInfoLogLength, &written, szInfoLog);
                            printf("Shader program Link Log : %s \n", szInfoLog);
                            free(szInfoLog);
                            [self uninitialize];//Myview release hoil
                            
                        }
                    }
                }

                gMMatrixUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_model_matrix");
                gVMatrixUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_view_matrix");
                gPMatrixUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_p_matrix");
                LKeyPressedUniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_lKey");
                
                
        LA_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_la[0]");
        LD_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ld[0]");
        LS_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ls[0]");
        LPosition_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_light_position[0]");

        LA_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_la[1]");
        LD_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ld[1]");
        LS_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ls[1]");
        LPosition_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_light_position[1]");

        LA_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_la[2]");
        LD_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ld[2]");
        LS_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ls[2]");
        LPosition_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_light_position[2]");

        KA_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ka");
        KD_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_kd");
        KS_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_ks");
        Shininess_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_Shininess_PV_DM[0]");
        Shininess_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_Shininess_PV_DM[1]");
        Shininess_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV_DM, "u_Shininess_PV_DM[2]");
        
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
            "uniform int u_lKey;" \
        
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

                iInfoLogLength = 0;
                iShaderCompileStatus = 0;
                szInfoLog = NULL;

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
                    
        "uniform vec3 u_la[3];" \
        "uniform vec3 u_ld[3];" \
        "uniform vec3 u_ls[3];" \
        "uniform vec4 u_light_position[3];" \
        
        
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_shininess;\n" \
        
        "in vec3 transformed_normal;\n"  \
        "in vec4 eye_cordinates;\n" \
    
        "uniform int u_lKey;\n" \
        "out vec4 FragColor;\n" \
        "vec3 fong_light[3];" \
        
        "void main(void){\n" \
        "vec3 color;\n"\
        
        "if(u_lKey == 1){\n" \
       
        "vec3 normalized_transformed_normal = transformed_normal;\n" \
        
        "vec3 light_direction[3];"\
        "vec3 view_vector[3];"\
        "vec3 reflection_vector[3];"\
        "vec3 specular[3];"\
        "vec3 diffuse[3];"\
        "vec3 ambient[3];"\
        
        "for(int i = 0; i< 3; i++){\n"\
        
        "view_vector[i] =normalize(-eye_cordinates.xyz);\n"\
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" \
        "reflection_vector[i] = reflect(-light_direction[i] , transformed_normal);\n" \
        
        "ambient[i] = u_la[i] * u_ka;\n" \
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" \
        "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector[i]), 0.0), u_shininess);\n" \

        "fong_light[i] = ambient[i] + diffuse[i] + specular[i];\n" \

        "}\n" \
    
        "color = fong_light[0] + fong_light[1] + fong_light[2];"\
    
        "}\n" \
        "else{\n" \
        "color = vec3( 1.0f, 1.0f, 1.0f);\n"\
        "}\n"
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

                iShaderProgramLinkStatus = 0;
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
                LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_lKey");
                
        LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_la[0]");
        LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_ld[0]");
        LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_ls[0]");
        LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position[0]");

        LA_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_la[1]");
        LD_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_ld[1]");
        LS_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_ls[1]");
        LPosition_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position[1]");

        LA_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_DM, "u_la[2]");
        LD_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_DM, "u_ld[2]");
        LS_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_DM, "u_ls[2]");
        LPosition_Uniform_DM[2] = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position[2]");

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
    
    
    if(Per_Fragment)
    {
    glUseProgram(gshaderProgramObject_DM);

            if (bLights)
            {
                glUniform1i(LKeyPressedUniform_DM, 1);
                glUniform1f(shininess, 58.0f);
                glUniform3f(LA_Uniform_DM[0], 0.0f, 0.0f, 0.0f);
                glUniform3f(LD_Uniform_DM[0], 1.0f, 0.0f, 0.0f);
                glUniform3f(LS_Uniform_DM[0], 1.0f, 0.0f, 0.0f);
                GLfloat light_position[] = { 0.0f, 1.5f * sinf(C_angle), 2.5f * cosf(C_angle), 1.0f };
                
                glUniform4fv(LPosition_Uniform_DM[0], 1, (GLfloat *)light_position);
                
                glUniform3f(LA_Uniform_DM[1], 0.0f, 0.0f, 0.0f);
                glUniform3f(LD_Uniform_DM[1], 0.0f, 0.0f, 1.0f);
                glUniform3f(LS_Uniform_DM[1], 0.0f, 0.0f, 1.0f);
                GLfloat light_position2[] = { 1.5f * sinf(C_angle), 0.0f, 2.5f * cosf(C_angle), 1.0f };
                
                glUniform4fv(LPosition_Uniform_DM[1], 1, (GLfloat *)light_position2);
                
                glUniform3f(LA_Uniform_DM[2], 0.0f, 0.0f, 0.0f);
                glUniform3f(LD_Uniform_DM[2], 0.0f, 1.0f, 0.0f);
                glUniform3f(LS_Uniform_DM[2], 0.0f, 1.0f, 0.0f);
                GLfloat light_position3[] = { 1.5f * sinf(C_angle), 2.5f * cosf(C_angle), 0.0f, 1.0f };
                
                glUniform4fv(LPosition_Uniform_DM[2], 1, (GLfloat *)light_position3);

                glUniform3f(KA_Uniform_DM, 0.0f, 0.0f, 0.0f);
                glUniform3f(KD_Uniform_DM, 1.0f, 1.0f, 1.0f);
                glUniform3f(KS_Uniform_DM, 1.0f, 1.0f, 1.0f);
           }
            else
            {
                glUniform1i(LKeyPressedUniform_DM, 0);
            }
            
            mat4 TranslateMatrix = mat4::identity();
            mat4 RotaionMatrix = mat4::identity();
            mat4 ModelViewMatrix = mat4::identity();
            mat4 ViewMatrix  = mat4::identity();;
            mat4 PMatrix;

            TranslateMatrix = vmath::translate( 0.0f, 0.0f, -2.0f);
            RotaionMatrix = vmath::rotate(C_angle, 0.0f, 1.0f, 0.0f);
            ModelViewMatrix = TranslateMatrix * RotaionMatrix;

            PMatrix = gPerspectiveProjectMatix_DM;
            glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
            glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelViewMatrix);
            glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);

            glBindVertexArray(g_Sphere_Vao_DM);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
            glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

            glBindVertexArray(0);

            glUseProgram(0);
    }
    else{
        glUseProgram(gshaderProgramObject_PV_DM);

                if (bLights)
                {
                    glUniform1i(LKeyPressedUniform_PV_DM, 1);
                    glUniform1f(Shininess_PV_DM[0], 128.0f);
                    glUniform3f(LA_Uniform_PV_DM[0], 0.0f, 0.0f, 0.0f);
                    glUniform3f(LD_Uniform_PV_DM[0], 1.0f, 0.0f, 0.0f);
                    glUniform3f(LS_Uniform_PV_DM[0], 1.0f, 0.0f, 0.0f);
                    GLfloat light_position[] = { 0.0f, 1.5f * sinf(C_angle), 2.5f * cosf(C_angle), 1.0f };
                    
                    glUniform4fv(LPosition_Uniform_PV_DM[0], 1, (GLfloat *)light_position);
                    
                    glUniform1f(Shininess_PV_DM[1], 128.0f);
                    glUniform3f(LA_Uniform_PV_DM[1], 0.0f, 0.0f, 0.0f);
                    glUniform3f(LD_Uniform_PV_DM[1], 0.0f, 0.0f, 1.0f);
                    glUniform3f(LS_Uniform_PV_DM[1], 0.0f, 0.0f, 1.0f);
                    GLfloat light_position2[] = { 1.5f * sinf(C_angle), 0.0f, 2.5f * cosf(C_angle), 1.0f };
                    
                    glUniform4fv(LPosition_Uniform_PV_DM[1], 1, (GLfloat *)light_position2);
                    
                    glUniform1f(Shininess_PV_DM[2], 128.0f);
                    glUniform3f(LA_Uniform_PV_DM[2], 0.0f, 0.0f, 0.0f);
                    glUniform3f(LD_Uniform_PV_DM[2], 0.0f, 1.0f, 0.0f);
                    glUniform3f(LS_Uniform_PV_DM[2], 0.0f, 1.0f, 0.0f);
                    GLfloat light_position3[] = { 1.5f * sinf(C_angle), 2.5f * cosf(C_angle), 0.0f, 1.0f };
                    
                    glUniform4fv(LPosition_Uniform_PV_DM[2], 1, (GLfloat *)light_position3);

                    glUniform3f(KA_Uniform_PV_DM, 0.0f, 0.0f, 0.0f);
                    glUniform3f(KD_Uniform_PV_DM, 1.0f, 1.0f, 1.0f);
                    glUniform3f(KS_Uniform_PV_DM, 1.0f, 1.0f, 1.0f);
                    
                }
                else
                {
                    glUniform1i(LKeyPressedUniform_PV_DM, 0);
                }
                
                mat4 TranslateMatrix = mat4::identity();
                mat4 RotaionMatrix = mat4::identity();
                mat4 ModelViewMatrix = mat4::identity();
                mat4 ViewMatrix  = mat4::identity();;
                mat4 PMatrix;

                TranslateMatrix = vmath::translate( 0.0f, 0.0f, -2.0f);
                RotaionMatrix = vmath::rotate(C_angle, 0.0f, 1.0f, 0.0f);
                ModelViewMatrix = TranslateMatrix * RotaionMatrix;

                PMatrix = gPerspectiveProjectMatix_DM;
                glUniformMatrix4fv(gPMatrixUniform_PV_DM, 1, GL_FALSE, PMatrix);
                glUniformMatrix4fv(gMMatrixUniform_PV_DM, 1, GL_FALSE, ModelViewMatrix);
                glUniformMatrix4fv(gVMatrixUniform_PV_DM, 1, GL_FALSE, ViewMatrix);

                glBindVertexArray(g_Sphere_Vao_DM);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
                glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);


                glBindVertexArray(0);

                glUseProgram(0);

    }
    
    
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

    if (Per_Fragment == true)
    {
        Per_Fragment = false;
    }
    else
    {
        Per_Fragment = true;
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

                if (gshaderProgramObject_PV_DM)
                {
                    glUseProgram(gshaderProgramObject_PV_DM);
                    GLsizei shaderCount;
                    glGetProgramiv(gshaderProgramObject_PV_DM, GL_ATTACHED_SHADERS, &shaderCount);
                    GLuint* pShader = NULL;
                    pShader = (GLuint*)malloc(sizeof(GLuint) * shaderCount);
                    if (pShader == NULL)
                    {
                        printf("\nUnable to allocate memory to pShader.\n");
                    }
                    glGetAttachedShaders(gshaderProgramObject_PV_DM, shaderCount, &shaderCount, pShader);
                    for (GLsizei i = 0; i < shaderCount; i++)
                    {
                        glDetachShader(gshaderProgramObject_PV_DM, pShader[i]);
                    }
                    free(pShader);

                    glDeleteShader(gVertexShaderObject_PV_DM);
                    gVertexShaderObject_PV_DM = 0;

                    glDeleteShader(gFragmentShaderObject_PV_DM);
                    gFragmentShaderObject_PV_DM = 0;

                    glDeleteProgram(gshaderProgramObject_PV_DM);
                    gshaderProgramObject_PV_DM = 0;

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

