
var canvas = null;
var gl = null;

var original_width;
var original_height;

var angle = 0.05;

var bfullscreen;
var u_key_Pressed;

const WebGLMicros =
{
    DVM_ATTTRIBUTE_VERTEX : 0,
    DVM_ATTTRIBUTE_NORMAL : 1,
    DVM_ATTTRIBUTE_COLOR : 2,
    DVM_ATTTRIBUTE_TEXTURE0 : 3,

}

var u_LA_Uniform_DM = [];
var u_LD_Uniform_DM = [];
var u_LS_Uniform_DM = [];
var u_LPosition_Uniform_DM = [];

var u_KA_Uniform_DM;
var u_KD_Uniform_DM;
var u_KS_Uniform_DM;
var u_Shininess_DM;

var a_LA_Uniform_DM = [];
var a_LD_Uniform_DM = [];
var a_LS_Uniform_DM = [];
var a_LPosition_Uniform_DM = [];

var a_KA_Uniform_DM;
var a_KD_Uniform_DM;
var a_KS_Uniform_DM;
var a_Shininess_DM;

var vertexShaderObject;
var fragmentShaderObject;
var ShaderProgramObject;

var light_Switch = true;

var vao_DM;
var Vertices_Vbo_DM;
var Color_Vbo_DM;
var mvUniform_DM;
var pUniform_DM;

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

            case 71:
                if(light_Switch)
                    light_Switch = false;
                else
                    light_Switch = true;
                    
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

    /* VertexShaderObject */
    var VertexShaderSourceCode =
        "#version 300 es                \n" +
        "in vec4 vPosition;" +
		"in vec3 vNormal;" +
		"uniform mat4 u_model_matrix;" +
        "uniform mat4 u_view_matrix;" +
        "uniform mat4 u_projection_matrix;" +
	    "uniform int u_lKey;" +
        "uniform vec3 u_la[2];" +
        "uniform vec3 u_ld[2];" +
        "uniform vec3 u_ls[2];" +
        "uniform vec4 u_light_position[2];" +
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
        "for( int i = 0; i < 2; i++){\n"  +
        "vec3 ambient[2];\n" +
        "vec3 diffuse[2];\n" +
        "vec3 specular[2];\n" +
        "vec3 light_direction[2];\n" +
        "vec3 reflection_vector[2];\n" +
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



    vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShaderObject, VertexShaderSourceCode);
    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if (error.length > 0) {
            console.log("after vertex shader code compile.");

            alert(error)
            unInitilize();
        }
    }
    /* Fragment shader code */

    var fragmentShaderSourceCode =
        "#version 300 es                \n" +
        "precision highp float;         \n" +
        "precision highp int;           \n" +
		"in vec3 fong_ads_light;        \n" +
		"out vec4 FragColor;            \n" +
		"void main(void){               \n" +
		"FragColor = vec4( fong_ads_light, 1.0);\n" +
        "}";

    fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);
    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject);
        if (error.length > 0) {
            alert(error)
            unInitilize();
        }
    }

    /* Shader Linking  */

    ShaderProgramObject = gl.createProgram();
    gl.attachShader(ShaderProgramObject, vertexShaderObject);
    gl.attachShader(ShaderProgramObject, fragmentShaderObject);

    gl.bindAttribLocation(ShaderProgramObject, WebGLMicros.DVM_ATTTRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(ShaderProgramObject, WebGLMicros.DVM_ATTTRIBUTE_NORMAL, "vNormal");


    gl.linkProgram(ShaderProgramObject);
    if (!gl.getProgramParameter(ShaderProgramObject, gl.LINK_STATUS)) {
        var error = gl.getProgramInfoLog(ShaderProgramObject);
        if (error.length > 0) {
            alert(error);
            unInitilize();
        }
    }

    mUniform_DM = gl.getUniformLocation(ShaderProgramObject, "u_model_matrix");
    vUniform_DM = gl.getUniformLocation(ShaderProgramObject, "u_view_matrix");
    pUniform_DM = gl.getUniformLocation(ShaderProgramObject, "u_projection_matrix");

    u_key_Pressed = gl.getUniformLocation( ShaderProgramObject, "u_lKey");
    
    u_LA_Uniform_DM[0] = gl.getUniformLocation( ShaderProgramObject, "u_la[0]");
    u_LD_Uniform_DM[0] = gl.getUniformLocation( ShaderProgramObject, "u_ld[0]");
    u_LS_Uniform_DM[0] = gl.getUniformLocation( ShaderProgramObject, "u_ls[0]");
    u_LPosition_Uniform_DM[0] = gl.getUniformLocation( ShaderProgramObject, "u_light_position[0]");

    u_LA_Uniform_DM[1] = gl.getUniformLocation( ShaderProgramObject, "u_la[1]");
    u_LD_Uniform_DM[1] = gl.getUniformLocation( ShaderProgramObject, "u_ld[1]");
    u_LS_Uniform_DM[1] = gl.getUniformLocation( ShaderProgramObject, "u_ls[1]");
    u_LPosition_Uniform_DM[1] = gl.getUniformLocation( ShaderProgramObject, "u_light_position[1]");
    
    u_KA_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_ka");
    u_KD_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_kd");
    u_KS_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_ks");

    u_Shininess_DM = gl.getUniformLocation( ShaderProgramObject, "u_shininess");
    

    var CubeVertices = new Float32Array([
        0.0, 0.5, 0.0,
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,

        0.0, 0.5, 0.0,
        0.5, -0.5, 0.5,
        0.5, -0.5, -0.5,

        0.0, 0.5, 0.0,
        0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5,

        0.0, 0.5, 0.0,
        -0.5, -0.5, 0.5,
        -0.5, -0.5, -0.5
    ]);

     var CubeColor = new Float32Array([
        0.0, 0.447214, 0.894427,
        0.0, 0.447214, 0.894427,
        0.0, 0.447214, 0.894427,

        0.894427, 0.447214, 0.0,
        0.894427, 0.447214, 0.0,
        0.894427, 0.447214, 0.0,

        0.0, 0.447214, -0.894427,
        0.0, 0.447214, -0.894427,
        0.0, 0.447214, -0.894427,

        -0.894427, 0.447214, 0.0,
        -0.894427, 0.447214, 0.0,
        -0.894427, 0.447214, 0.0

]);


    vao_DM = gl.createVertexArray();
    gl.bindVertexArray(vao_DM);

    Vertices_Vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Vertices_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, CubeVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_VERTEX,
        3,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    Color_Vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Color_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, CubeColor, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_NORMAL,
        3,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_NORMAL);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

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

    gl.useProgram(ShaderProgramObject);

    if(light_Switch)
    {
        a_LA_Uniform_DM [0] = new Float32Array([0.0, 0.0, 0.0]);
        a_LD_Uniform_DM [0] = new Float32Array([1.0, 0.0, 0.0]);
        a_LS_Uniform_DM [0] = new Float32Array([1.0, 0.0, 0.0]);
        a_LPosition_Uniform_DM [0] = new Float32Array([2.0, 0.0, 0.0, 1.0]);
        
        a_LA_Uniform_DM [1] = new Float32Array([0.0, 0.0, 0.0]);
        a_LD_Uniform_DM [1] = new Float32Array([0.0, 0.0, 1.0]);
        a_LS_Uniform_DM [1] = new Float32Array([0.0, 0.0, 1.0]);
        a_LPosition_Uniform_DM [1] = new Float32Array([-2.0, 0.0, 0.0, 1.0]);

        a_KA_Uniform_DM = new Float32Array([0.0, 0.0, 0.0]);
        a_KD_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
        a_KS_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
        a_Shininess_DM = 50.0;

        gl.uniform1i(u_key_Pressed, 1);

        gl.uniform3fv(u_LA_Uniform_DM[0], a_LA_Uniform_DM[0]);
        gl.uniform3fv(u_LD_Uniform_DM[0], a_LD_Uniform_DM[0]);
        gl.uniform3fv(u_LS_Uniform_DM[0], a_LS_Uniform_DM[0]);
        gl.uniform4fv(u_LPosition_Uniform_DM[0], a_LPosition_Uniform_DM[0]);

        gl.uniform3fv(u_LA_Uniform_DM[1], a_LA_Uniform_DM[1]);
        gl.uniform3fv(u_LD_Uniform_DM[1], a_LD_Uniform_DM[1]);
        gl.uniform3fv(u_LS_Uniform_DM[1], a_LS_Uniform_DM[1]);
        gl.uniform4fv(u_LPosition_Uniform_DM[1], a_LPosition_Uniform_DM[1]);

        gl.uniform3fv(u_KA_Uniform_DM, a_KA_Uniform_DM);
        gl.uniform3fv(u_KD_Uniform_DM, a_KD_Uniform_DM);
        gl.uniform3fv(u_KS_Uniform_DM, a_KS_Uniform_DM);
        gl.uniform1f(u_Shininess_DM, a_Shininess_DM);

    }
    else{
        gl.uniform1i(u_key_Pressed, 0);
    }


    var ModelMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    var ViewMatrix = mat4.create();
    var RotationMatrix = mat4.create();
    
    mat4.identity(TranlationMatrix);
    mat4.identity(ModelMatrix);
    mat4.identity(ViewMatrix);
    mat4.identity(RotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 0.0, 0.0, -2.0]);
    mat4.rotateY(RotationMatrix, RotationMatrix, degToRad(angle));
    
    mat4.multiply(ModelMatrix, TranlationMatrix, RotationMatrix);
    
    gl.uniformMatrix4fv(mUniform_DM, false, ModelMatrix);
    gl.uniformMatrix4fv(vUniform_DM, false, ViewMatrix);
    gl.uniformMatrix4fv(pUniform_DM, false, perspectiveProjectionMatrix);

    gl.bindVertexArray(vao_DM);
    
    gl.drawArrays(gl.TRIANGLES, 0, 12);

    gl.bindVertexArray(null);
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
    angle += 0.75;
    if(angle > 360.0)
    {
        angle = 0.0;
    }
}

function unInitilize() {
    if (vao_DM) {
        gl.deleteVertexArray(vao_DM);
        vao_DM = 0;
    }

    if (Vertices_Vbo_DM) {
        gl.deleteBuffer(Vertices_Vbo_DM);
        Vertices_Vbo_DM = 0;
    }
    if (ShaderProgramObject) {
        if (fragmentShaderObject) {
            gl.detachShader(ShaderProgramObject, fragmentShaderObject);
            gl.deleteShader(fragmentShaderObject);
            fragmentShaderObject = null;

        }

        if (vertexShaderObject) {
            gl.detachShader(ShaderProgramObject, vertexShaderObject);
            gl.deleteShader(vertexShaderObject);
            vertexShaderObject = null;
        }
        gl.deleteProgram(ShaderProgramObject);
        ShaderProgramObject = null;
    }
}
