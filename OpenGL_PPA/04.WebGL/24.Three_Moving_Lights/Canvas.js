
var canvas = null;
var gl = null;

var original_width;
var original_height;

var angle = 0.05;


const WebGLMacros =
{
    DVM_ATRIBUTE_VERTEX : 0,
    DVM_ATRIBUTE_NORMALS : 1,
    DVM_ATRIBUTE_COLOR : 2,
    DVM_ATRIBUTE_TEXTURE0 : 3,

}

var a_LA_Uniform_DM = [];
var a_LD_Uniform_DM = [];
var a_LS_Uniform_DM = [];
var a_LPosition_Uniform_DM = [];

var a_KA_Uniform_DM;
var a_KD_Uniform_DM;
var a_KS_Uniform_DM;
var a_Shininess_DM;

var u_LA_Uniform_DM_PV = [];
var u_LD_Uniform_DM_PV = [];
var u_LS_Uniform_DM_PV = [];
var u_LPosition_Uniform_DM_PV = [];

var u_KA_Uniform_DM_PV;
var u_KD_Uniform_DM_PV;
var u_KS_Uniform_DM_PV;
var u_Shininess_DM_PV;

var vertexShaderObject_DM_PV;
var fragmentShaderObject_DM_PV;
var ShaderProgramObject_DM_PV;

var mUniform_DM_PV;
var vUniform_DM_PV;
var pUniform_DM_PV;

var u_key_Pressed_DM_PV;

var u_LA_Uniform_DM_PF = [];
var u_LD_Uniform_DM_PF = [];
var u_LS_Uniform_DM_PF = [];
var u_LPosition_Uniform_DM_PF = [];

var u_KA_Uniform_DM_PF;
var u_KD_Uniform_DM_PF;
var u_KS_Uniform_DM_PF;
var u_Shininess_DM_PF;

var vertexShaderObject_DM_PF;
var fragmentShaderObject_DM_PF;
var ShaderProgramObject_DM_PF;

var mUniform_DM_PF;
var vUniform_DM_PF;
var pUniform_DM_PF;

var u_key_Pressed_DM_PF;

var light_Switch = false;
var Sphere_DM = null;
var bfullscreen;

var Per_Vertex = true;

var perspectiveProjectionMatrix;

var requestAnimation = window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    windows.mozRequestAnimationFrame ||
    windows.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

var cancleAnimationFrame =
    window.cancleAnimationFrame ||
    window.webkitCancelRequestAnimationFrame || window.webkitCancelAnimationFrame ||
    window.mozCancelRequestAnimationFrame || window.mozCancelAnimationFrame ||
    window.msCancelRequestAnimationFrame || window.msCancelAnimationFrame ||
    window.oCancelRequestAnimationFrame || window.oCancelAnimationFrame;


function main() {

    canvas = document.getElementById("DVM");

    if (!canvas) {
        console.log("obtaining canvas faild.");
    }
    else {
        console.log("obtaining canvas succeded.");
    }

    original_width = canvas.width;
    original_height = canvas.height;


    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("resize", resize, false);

    init();
    resize();//warm up
    display();//warm up cause repaint nahi hot.


}

function keyDown(event) {
    switch (event.keyCode) {
        case 70:
            ToggleFullscreen();
            break;

        case 76:
            if(light_Switch)
                light_Switch = false;
            else
                light_Switch = true;
        break;
        case 86:
            if(Per_Vertex)
            {
                Per_Vertex = false;
            }
            else{
                Per_Vertex = true;
            }
            break;
        case 27:
            unInitilize();
            window.close();
            break;
    }
}

function ToggleFullscreen(params) {

    //variable dec
    var fullscreen_element =
        document.fullscreenElement || // chrome, opera mini
        document.webkitFullscreenElement || // (apple)safari
        document.mozFullScreenElement || //modzilla fire fox
        document.msFullscreenElement || //MS edge
        null;

    // aapan screen la full screen aata kartoy.
    if (fullscreen_element == null) {

        if (canvas.requestFullscreen) {
            console.log("Inside the Chrome.");
            canvas.requestFullscreen();
            bfullscreen = true;
        }
        else if (canvas.mozRequestFullScreen) {
            console.log("Inside the Moz.");
            canvas.mozRequestFullScreen();
            fullscreen = true;
        }
        else if (canvas.webkitRequestFullscreen) {
            console.log("Inside the MacOS.");
            canvas.webkitRequestFullscreen();
            bfullscreen = true;
        }
        else if (canvas.msRequestFullscreen) {
            console.log("Inside the MS.");
            canvas.msRequestFullscreen();
            bfullscreen = true;
        }
    }
    else {
        if (document.exitFullscreen) {
            document.exitFullscreen();
            bfullscreen = false;
        }
        else if (document.webkitExitFullscreen) {
            document.webkitExitFullscreen();
            bfullscreen = false;
        }
        else if (document.mozCancelFullScreen) {
            document.mozCancelFullScreen();
            bfullscreen = false;
        }
        else if (document.msExitFullscreen) {
            document.msExitFullscreen();
            bfullscreen = false;
        }
        bfullscreen = false;
    }

}

function init() {

    //this is the most imp change 2d to webgl2
    gl = canvas.getContext("webgl2");

    if (!gl) {
        console.log("obtaining WebGL2 faild.");
    }
    else {
        console.log("obtaining WebGL2 succeded.");
    }

    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;

    /* ******************************************** VERTEX SHADER ********************************************** */

    var VertexShaderSourceCode_PV =
        "#version 300 es                \n" +
        "in vec4 vPosition;" +
		"in vec3 vNormal;" +
		"uniform mat4 u_model_matrix;" +
        "uniform mat4 u_view_matrix;" +
        "uniform mat4 u_projection_matrix;" +
	    "uniform int u_lKey;" +
        "uniform vec3 u_la[3];" +
        "uniform vec3 u_ld[3];" +
        "uniform vec3 u_ls[3];" +
        "uniform vec4 u_light_position[3];" +
        "uniform vec3 u_ka;" +
        "uniform vec3 u_kd;" +
        "uniform vec3 u_ks;" +
        "uniform float u_shininess;\n" +
        "out vec3 fong_ads_light;\n" +
        "void main(void){\n" +
        "if(u_lKey == 1){\n" +
        "vec4 eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" +
        "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" +
        "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" +
        "for( int i = 0; i < 3; i++){\n"  +
        "vec3 ambient[3];\n" +
        "vec3 diffuse[3];\n" +
        "vec3 specular[3];\n" +
        "vec3 light_direction[3];\n" +
        "vec3 reflection_vector[3];\n" +
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" +
        "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);\n" +
        "ambient[i] = u_la[i] * u_ka;\n" +
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" +
        "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector), 0.0), u_shininess);\n" +
        "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" +
        "}\n" +
        "}\n" +
        "else{\n" +
        "fong_ads_light = vec3(1.0f, 1.0f, 1.0f);" +
        "}\n" +
        "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;\n" +    
        "}";



    vertexShaderObject_DM_PV = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShaderObject_DM_PV, VertexShaderSourceCode_PV);
    gl.compileShader(vertexShaderObject_DM_PV);

    if (gl.getShaderParameter(vertexShaderObject_DM_PV, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject_DM_PV);
        if (error.length > 0) {
            console.log("after vertex shader code compile.");

            alert(error)
            unInitilize();
        }
    }
    /* Fragment shader code */

    var fragmentShaderSourceCode_PV =
        "#version 300 es                \n" +
        "precision highp float;         \n" +
        "precision highp int;           \n" +
		"in vec3 fong_ads_light;        \n" +
		"out vec4 FragColor;            \n" +
		"void main(void){               \n" +
		"FragColor = vec4( fong_ads_light, 1.0);\n" +
        "}";

    fragmentShaderObject_DM_PV = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject_DM_PV, fragmentShaderSourceCode_PV);
    gl.compileShader(fragmentShaderObject_DM_PV);

    if (gl.getShaderParameter(fragmentShaderObject_DM_PV, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject_DM_PV);
        if (error.length > 0) {
            alert(error)
            unInitilize();
        }
    }

    /* Shader Linking  */

    ShaderProgramObject_DM_PV = gl.createProgram();
    gl.attachShader(ShaderProgramObject_DM_PV, vertexShaderObject_DM_PV);
    gl.attachShader(ShaderProgramObject_DM_PV, fragmentShaderObject_DM_PV);

    gl.bindAttribLocation(ShaderProgramObject_DM_PV, WebGLMacros.DVM_ATRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(ShaderProgramObject_DM_PV, WebGLMacros.DVM_ATRIBUTE_NORMALS, "vNormal");


    gl.linkProgram(ShaderProgramObject_DM_PV);
    if (!gl.getProgramParameter(ShaderProgramObject_DM_PV, gl.LINK_STATUS)) {
        var error = gl.getProgramInfoLog(ShaderProgramObject_DM_PV);
        if (error.length > 0) {
            alert(error);
            unInitilize();
        }
    }

    mUniform_DM_PV = gl.getUniformLocation(ShaderProgramObject_DM_PV, "u_model_matrix");
    vUniform_DM_PV = gl.getUniformLocation(ShaderProgramObject_DM_PV, "u_view_matrix");
    pUniform_DM_PV = gl.getUniformLocation(ShaderProgramObject_DM_PV, "u_projection_matrix");

    u_key_Pressed_DM_PV = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_lKey");
    
    u_LA_Uniform_DM_PV[0] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_la[0]");
    u_LD_Uniform_DM_PV[0] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ld[0]");
    u_LS_Uniform_DM_PV[0] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ls[0]");
    u_LPosition_Uniform_DM_PV[0] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_light_position[0]");

    u_LA_Uniform_DM_PV[1] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_la[1]");
    u_LD_Uniform_DM_PV[1] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ld[1]");
    u_LS_Uniform_DM_PV[1] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ls[1]");
    u_LPosition_Uniform_DM_PV[1] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_light_position[1]");

    u_LA_Uniform_DM_PV[2] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_la[2]");
    u_LD_Uniform_DM_PV[2] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ld[2]");
    u_LS_Uniform_DM_PV[2] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ls[2]");
    u_LPosition_Uniform_DM_PV[2] = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_light_position[2]");
    
    u_KA_Uniform_DM_PV = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ka");
    u_KD_Uniform_DM_PV = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_kd");
    u_KS_Uniform_DM_PV = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_ks");

    u_Shininess_DM_PV = gl.getUniformLocation( ShaderProgramObject_DM_PV, "u_shininess");
    
     /* ******************************************** FRAGMENT SHADER ********************************************** */

    var VertexShaderSourceCode_PF =
        "#version 300 es                \n" +
        "in vec4 vPosition;" +
		"in vec3 vNormal;" +
		"uniform mat4 u_model_matrix;" +
        "uniform mat4 u_view_matrix;" +
        "uniform mat4 u_projection_matrix;" +
        "out vec4 eye_cordinates;\n" +
        "out vec3 transformed_normal;\n"  +
        "void main(void){\n" +
        "eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" +
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" +
        "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;\n" +
        "}";

    vertexShaderObject_DM_PF = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShaderObject_DM_PF, VertexShaderSourceCode_PF);
    gl.compileShader(vertexShaderObject_DM_PF);

    if (gl.getShaderParameter(vertexShaderObject_DM_PF, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject_DM_PF);
        if (error.length > 0) {
            console.log("after vertex shader code compile.");

            alert(error)
            unInitilize();
        }
    }
    /* Fragment shader code */

    var fragmentShaderSourceCode_PF =
        "#version 300 es                \n" +
        "precision highp float;         \n" +
        "precision highp int;           \n" +
        "uniform int u_lKey;\n" +
        "uniform vec3 u_la[3];\n" +
        "uniform vec3 u_ld[3];\n" +
        "uniform vec3 u_ls[3];\n" +
        "uniform vec4 u_light_position[3];\n" +
        "uniform vec3 u_ka;\n" +
        "uniform vec3 u_kd;\n" +
        "uniform vec3 u_ks;\n" +
        "uniform float u_shininess;\n" +
        "in vec4 eye_cordinates;\n" +
        "in vec3 transformed_normal;\n"  +
        "out vec4 FragColor;\n" +
        "vec3 fong_ads_light;\n" +
        "void main(void){\n" +
        "if(u_lKey == 1){\n" +
        "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" +
        "for( int i = 0; i < 3; i++){\n" +
        "vec3 ambient[3];\n" +
        "vec3 diffuse[3];\n" +
        "vec3 specular[3];\n" +
        "vec3 light_direction[3];\n" +
        "vec3 reflection_vector[3];\n" +
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" +
        "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);\n" +
        "ambient[i] = u_la[i] * u_ka;\n" +
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" +
        "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector), 0.0), u_shininess);\n" +
        "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" +
        "}\n" +
        "FragColor = vec4(fong_ads_light, 1.0f);\n"+
        "}\n" +
        "else{\n" +
        "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"+
        "}\n" +
        "}";

    fragmentShaderObject_DM_PF = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject_DM_PF, fragmentShaderSourceCode_PF);
    gl.compileShader(fragmentShaderObject_DM_PF);

    if (gl.getShaderParameter(fragmentShaderObject_DM_PF, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject_DM_PF);
        if (error.length > 0) {
            alert(error)
            unInitilize();
        }
    }

    /* Shader Linking  */

    ShaderProgramObject_DM_PF = gl.createProgram();
    gl.attachShader(ShaderProgramObject_DM_PF, vertexShaderObject_DM_PF);
    gl.attachShader(ShaderProgramObject_DM_PF, fragmentShaderObject_DM_PF);

    gl.bindAttribLocation(ShaderProgramObject_DM_PF, WebGLMacros.DVM_ATRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(ShaderProgramObject_DM_PF, WebGLMacros.DVM_ATRIBUTE_NORMALS, "vNormal");

    gl.linkProgram(ShaderProgramObject_DM_PF);
    if (!gl.getProgramParameter(ShaderProgramObject_DM_PF, gl.LINK_STATUS)) {
        var error = gl.getProgramInfoLog(ShaderProgramObject_DM_PF);
        if (error.length > 0) {
            alert(error);
            unInitilize();
        }
    }

    mUniform_DM_PF = gl.getUniformLocation(ShaderProgramObject_DM_PF, "u_model_matrix");
    vUniform_DM_PF = gl.getUniformLocation(ShaderProgramObject_DM_PF, "u_view_matrix");
    pUniform_DM_PF = gl.getUniformLocation(ShaderProgramObject_DM_PF, "u_projection_matrix");

    u_key_Pressed_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_lKey");
    
    u_LA_Uniform_DM_PF[0] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_la[0]");
    u_LD_Uniform_DM_PF[0] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ld[0]");
    u_LS_Uniform_DM_PF[0] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ls[0]");
    u_LPosition_Uniform_DM_PF[0] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_light_position[0]");

    u_LA_Uniform_DM_PF[1] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_la[1]");
    u_LD_Uniform_DM_PF[1] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ld[1]");
    u_LS_Uniform_DM_PF[1] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ls[1]");
    u_LPosition_Uniform_DM_PF[1] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_light_position[1]");

    u_LA_Uniform_DM_PF[2] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_la[2]");
    u_LD_Uniform_DM_PF[2] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ld[2]");
    u_LS_Uniform_DM_PF[2] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ls[2]");
    u_LPosition_Uniform_DM_PF[2] = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_light_position[2]");
    
    u_KA_Uniform_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ka");
    u_KD_Uniform_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_kd");
    u_KS_Uniform_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ks");

    u_Shininess_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_shininess");   

  
    Sphere_DM = new Mesh();
    makeSphere(Sphere_DM, 0.25, 30, 30);

    gl.enable(gl.DEPTH_TEST);
    
    gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
    
    perspectiveProjectionMatrix = mat4.create();
    mat4.identity(perspectiveProjectionMatrix);

    resize();
}

function resize() {

    if (bfullscreen == true) {

        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else {
        canvas.width = original_width;
        canvas.height = original_height;
    }

    gl.viewport(0, 0, canvas.width, canvas.height);
    
    mat4.perspective(perspectiveProjectionMatrix, 
        45.0, 
        parseFloat(canvas.width) / parseFloat(canvas.height), 
        0.1, 
        100.0);
}

function display() {
    

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    
    if(Per_Vertex)
    {        
        gl.useProgram(ShaderProgramObject_DM_PV);

        if(light_Switch)
        {
            a_LA_Uniform_DM [0] = new Float32Array([0.0, 0.0, 0.0]);
            a_LD_Uniform_DM [0] = new Float32Array([1.0, 0.0, 0.0]);
            a_LS_Uniform_DM [0] = new Float32Array([1.0, 0.0, 0.0]);
            a_LPosition_Uniform_DM [0] = new Float32Array([0.0, Math.sin(angle), Math.cos(angle), 1.0]);

            a_LA_Uniform_DM [1] = new Float32Array([0.0, 0.0, 0.0]);
            a_LD_Uniform_DM [1] = new Float32Array([0.0, 0.0, 1.0]);
            a_LS_Uniform_DM [1] = new Float32Array([0.0, 0.0, 1.0]);
            a_LPosition_Uniform_DM [1] = new Float32Array([ Math.sin(angle), 0.0, Math.cos(angle), 1.0]);
        
            a_LA_Uniform_DM [2] = new Float32Array([0.0, 0.0, 0.0]);
            a_LD_Uniform_DM [2] = new Float32Array([0.0, 1.0, 0.0]);
            a_LS_Uniform_DM [2] = new Float32Array([0.0, 1.0, 0.0]);
            a_LPosition_Uniform_DM [2] = new Float32Array([ Math.sin(angle), Math.cos(angle), 0.0, 1.0]);
        
            a_KA_Uniform_DM = new Float32Array([0.0, 0.0, 0.0]);
            a_KD_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
            a_KS_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
            a_Shininess_DM = 50.0;
        
            gl.uniform1i(u_key_Pressed_DM_PV, 1);
        
            gl.uniform3fv(u_LA_Uniform_DM_PV[0], a_LA_Uniform_DM[0]);
            gl.uniform3fv(u_LD_Uniform_DM_PV[0], a_LD_Uniform_DM[0]);
            gl.uniform3fv(u_LS_Uniform_DM_PV[0], a_LS_Uniform_DM[0]);
            gl.uniform4fv(u_LPosition_Uniform_DM_PV[0], a_LPosition_Uniform_DM[0]);
        
            gl.uniform3fv(u_LA_Uniform_DM_PV[1], a_LA_Uniform_DM[1]);
            gl.uniform3fv(u_LD_Uniform_DM_PV[1], a_LD_Uniform_DM[1]);
            gl.uniform3fv(u_LS_Uniform_DM_PV[1], a_LS_Uniform_DM[1]);
            gl.uniform4fv(u_LPosition_Uniform_DM_PV[1], a_LPosition_Uniform_DM[1]);
        
            gl.uniform3fv(u_LA_Uniform_DM_PV[2], a_LA_Uniform_DM[2]);
            gl.uniform3fv(u_LD_Uniform_DM_PV[2], a_LD_Uniform_DM[2]);
            gl.uniform3fv(u_LS_Uniform_DM_PV[2], a_LS_Uniform_DM[2]);
            gl.uniform4fv(u_LPosition_Uniform_DM_PV[2], a_LPosition_Uniform_DM[2]);
        
            gl.uniform3fv(u_KA_Uniform_DM_PV, a_KA_Uniform_DM);
            gl.uniform3fv(u_KD_Uniform_DM_PV, a_KD_Uniform_DM);
            gl.uniform3fv(u_KS_Uniform_DM_PV, a_KS_Uniform_DM);
            gl.uniform1f(u_Shininess_DM_PV, a_Shininess_DM);
        
        }
        else{
            gl.uniform1i(u_key_Pressed_DM_PV, 0);
        }
    }
    else
    {
        gl.useProgram(ShaderProgramObject_DM_PF);
        if(light_Switch)
        {
            a_LA_Uniform_DM [0] = new Float32Array([0.0, 0.0, 0.0]);
            a_LD_Uniform_DM [0] = new Float32Array([1.0, 0.0, 0.0]);
            a_LS_Uniform_DM [0] = new Float32Array([1.0, 0.0, 0.0]);
            a_LPosition_Uniform_DM [0] = new Float32Array([0.0, Math.sin(angle), Math.cos(angle), 1.0]);

            a_LA_Uniform_DM [1] = new Float32Array([0.0, 0.0, 0.0]);
            a_LD_Uniform_DM [1] = new Float32Array([0.0, 0.0, 1.0]);
            a_LS_Uniform_DM [1] = new Float32Array([0.0, 0.0, 1.0]);
            a_LPosition_Uniform_DM [1] = new Float32Array([ Math.sin(angle), 0.0, Math.cos(angle), 1.0]);
        
            a_LA_Uniform_DM [2] = new Float32Array([0.0, 0.0, 0.0]);
            a_LD_Uniform_DM [2] = new Float32Array([0.0, 1.0, 0.0]);
            a_LS_Uniform_DM [2] = new Float32Array([0.0, 1.0, 0.0]);
            a_LPosition_Uniform_DM [2] = new Float32Array([ Math.sin(angle), Math.cos(angle), 0.0, 1.0]);
        
            a_KA_Uniform_DM = new Float32Array([0.0, 0.0, 0.0]);
            a_KD_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
            a_KS_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
            a_Shininess_DM = 50.0;
        
            gl.uniform1i(u_key_Pressed_DM_PF, 1);
        
            gl.uniform3fv(u_LA_Uniform_DM_PF[0], a_LA_Uniform_DM[0]);
            gl.uniform3fv(u_LD_Uniform_DM_PF[0], a_LD_Uniform_DM[0]);
            gl.uniform3fv(u_LS_Uniform_DM_PF[0], a_LS_Uniform_DM[0]);
            gl.uniform4fv(u_LPosition_Uniform_DM_PF[0], a_LPosition_Uniform_DM[0]);
        
            gl.uniform3fv(u_LA_Uniform_DM_PF[1], a_LA_Uniform_DM[1]);
            gl.uniform3fv(u_LD_Uniform_DM_PF[1], a_LD_Uniform_DM[1]);
            gl.uniform3fv(u_LS_Uniform_DM_PF[1], a_LS_Uniform_DM[1]);
            gl.uniform4fv(u_LPosition_Uniform_DM_PF[1], a_LPosition_Uniform_DM[1]);
        
            gl.uniform3fv(u_LA_Uniform_DM_PF[2], a_LA_Uniform_DM[2]);
            gl.uniform3fv(u_LD_Uniform_DM_PF[2], a_LD_Uniform_DM[2]);
            gl.uniform3fv(u_LS_Uniform_DM_PF[2], a_LS_Uniform_DM[2]);
            gl.uniform4fv(u_LPosition_Uniform_DM_PF[2], a_LPosition_Uniform_DM[2]);
    
            gl.uniform3fv(u_KA_Uniform_DM_PF, a_KA_Uniform_DM);
            gl.uniform3fv(u_KD_Uniform_DM_PF, a_KD_Uniform_DM);
            gl.uniform3fv(u_KS_Uniform_DM_PF, a_KS_Uniform_DM);
            gl.uniform1f(u_Shininess_DM_PF, a_Shininess_DM);
        
        }
        else{
            gl.uniform1i(u_key_Pressed_DM_PF, 0);
        }

    }

    var ModelMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    var ViewMatrix = mat4.create();
    var RotationMatrix = mat4.create();
    
    mat4.identity(TranlationMatrix);
    mat4.identity(ModelMatrix);
    mat4.identity(ViewMatrix);
    mat4.identity(RotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 0.0, 0.0, -1.0]);
    //mat4.rotateY(RotationMatrix, RotationMatrix, degToRad(angle));
    mat4.rotateX(RotationMatrix, RotationMatrix, 90);
    
    mat4.multiply(ModelMatrix, TranlationMatrix, RotationMatrix);
    
    if(Per_Vertex)
    {
        gl.uniformMatrix4fv(mUniform_DM_PV, false, ModelMatrix);
        gl.uniformMatrix4fv(vUniform_DM_PV, false, ViewMatrix);
        gl.uniformMatrix4fv(pUniform_DM_PV, false, perspectiveProjectionMatrix);
    }
    else
    {
        gl.uniformMatrix4fv(mUniform_DM_PF, false, ModelMatrix);
        gl.uniformMatrix4fv(vUniform_DM_PF, false, ViewMatrix);
        gl.uniformMatrix4fv(pUniform_DM_PF, false, perspectiveProjectionMatrix);
    }

    Sphere_DM.draw();

    gl.useProgram(null);

    update();

    requestAnimationFrame(display, canvas);
}

function degToRad(Degrees)
{
    return (Degrees * Math.PI / 180.0);
}

function update()
{
    angle += 0.075;
    if(angle > 360.0)
    {
        angle = 0.0;
    }
}

function unInitilize() {
    if(Sphere_DM)
    {
        Sphere_DM.deallocate();
    }
    if (ShaderProgramObject_DM_PV) {
        if (fragmentShaderObject_DM_PV) {
            gl.detachShader(ShaderProgramObject_DM_PV, fragmentShaderObject_DM_PV);
            gl.deleteShader(fragmentShaderObject_DM_PV);
            fragmentShaderObject_DM_PV = null;

        }

        if (vertexShaderObject_DM_PV) {
            gl.detachShader(ShaderProgramObject_DM_PV, vertexShaderObject_DM_PV);
            gl.deleteShader(vertexShaderObject_DM_PV);
            vertexShaderObject_DM_PV = null;
        }
        gl.deleteProgram(ShaderProgramObject_DM_PV);
        ShaderProgramObject_DM_PV = null;
    }
}
