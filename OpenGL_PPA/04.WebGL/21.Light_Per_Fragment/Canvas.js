
var canvas = null;
var gl = null;

var original_width;
var original_height;

var angle = 0.05;

var bfullscreen;

const WebGLMacros =
{
    DVM_ATRIBUTE_VERTEX : 0,
    DVM_ATRIBUTE_NORMALS : 1,
    DVM_ATRIBUTE_COLOR : 2,
    DVM_ATRIBUTE_TEXTURE0 : 3,

}

var Sphere_DM = null;

var vertexShaderObject;
var fragmentShaderObject;
var ShaderProgramObject;

var u_LA_Uniform_DM;
var u_LD_Uniform_DM;
var u_LS_Uniform_DM;
var u_LPosition_Uniform_DM;

var u_KA_Uniform_DM;
var u_KD_Uniform_DM;
var u_KS_Uniform_DM;
var u_Shininess_DM;

var u_key_Pressed;

var a_LA_Uniform_DM = [];
var a_LD_Uniform_DM = [];
var a_LS_Uniform_DM = [];
var a_LPosition_Uniform_DM = [];

var a_KA_Uniform_DM = [];
var a_KD_Uniform_DM = [];
var a_KS_Uniform_DM = [];
var a_Shininess_DM;


var bAnimation = false;
var bLight_sWitch = false;

var mUniform_DM;
var vUniform_DM;
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
        case 76:
            if(bLight_sWitch)
            {
                bLight_sWitch = false;
            }
            else
            {
                bLight_sWitch = true;
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

    /* VertexShaderObject */
    var VertexShaderSourceCode =
        "#version 300 es                \n" +
        "precision highp float;\n" +
        "precision highp int;\n" +
        "in vec4 vPosition;" +
        "in vec3 vNormal;" +
        "uniform mat4 u_model_matrix;" +
        "uniform mat4 u_view_matrix;" +
        "uniform mat4 u_projection_matrix;" +
        "uniform int u_lKey;" +
        "uniform vec4 u_light_position;" +
        "out vec3 transformed_normal;"  +
        "out vec3 light_direction;\n" +
        "out vec3 view_vector;\n" +
        "void main(void){\n" +
        "if(u_lKey == 1){\n" +
        "vec4 eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" +
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" +
        "light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" +
        "view_vector = normalize(-eye_cordinates.xyz );\n" +
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
        "uniform int u_lKey;" +
        "uniform vec3 u_la;" +
        "uniform vec3 u_ld;" +
        "uniform vec3 u_ls;" +
        "uniform vec3 u_ka;" +
        "uniform vec3 u_kd;" +
        "uniform vec3 u_ks;" +
        "uniform float u_shininess;\n" +
        "in vec3 transformed_normal;\n"  +
        "in vec3 light_direction;\n" +
        "in vec3 view_vector;\n" +
        "out vec4 FragColor;\n" +
        "void main(void){\n" +
        "if(u_lKey == 1){\n" +
        "vec3 normalized_transformed_normal = transformed_normal;\n" +
        "vec3 normalized_light_direction = light_direction;\n" +
        "vec3 normalized_view_vector = view_vector;\n" +
        "vec3 reflection_vector = reflect(-normalized_light_direction , normalized_transformed_normal);\n" +
        "vec3 ambient = u_la * u_ka;\n" +
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normal), 0.0f);\n" +
        "vec3 specular = u_ls * u_ks * pow( max(dot(reflection_vector, normalized_view_vector), 0.0f), u_shininess);\n" +
        "vec3 fong_ads_light = ambient + diffuse + specular;\n" +
        "FragColor = vec4(fong_ads_light, 1.0f);\n"+
        "}\n" +
        "else{\n" +
        "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"+
        "}\n" +
        "}";

    fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);
    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject);
        if (error.length > 0) {
            alert(error);
            unInitilize();
        }
    }

    /* Shader Linking  */

    ShaderProgramObject = gl.createProgram();
    gl.attachShader(ShaderProgramObject, vertexShaderObject);
    gl.attachShader(ShaderProgramObject, fragmentShaderObject);

    gl.bindAttribLocation(ShaderProgramObject, WebGLMacros.DVM_ATRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(ShaderProgramObject, WebGLMacros.DVM_ATRIBUTE_NORMALS, "vNormal");
    
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

    u_key_Pressed = gl.getUniformLocation(ShaderProgramObject, "u_lKey");

    u_LA_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_la");
    u_LD_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_ld");
    u_LS_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_ls");
    u_LPosition_Unform_DM = gl.getUniformLocation( ShaderProgramObject, "u_light_position");
    
    u_KA_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_ka");
    u_KD_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_kd");
    u_KS_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_ks");
    u_Shininess_DM = gl.getUniformLocation( ShaderProgramObject, "u_shininess");
    


    Sphere_DM = new Mesh();
    makeSphere(Sphere_DM, 1.0, 50, 50);
    
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

    if(bLight_sWitch)
    {
        a_LA_Uniform_DM = new Float32Array([0.0, 0.0, 0.0]);
        a_LD_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
        a_LS_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
        a_LPosition_Uniform_DM = new Float32Array([100.0, 100.0, 100.0, 1.0]);
        
        a_KA_Uniform_DM = new Float32Array([0.0, 0.0, 0.0]);
        a_KD_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
        a_KS_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
        a_Shininess_DM = 180.0;

        gl.uniform1i(u_key_Pressed, 1);

        gl.uniform3fv(u_LA_Uniform_DM, a_LA_Uniform_DM);
        gl.uniform3fv(u_LD_Uniform_DM, a_LD_Uniform_DM);
        gl.uniform3fv(u_LS_Uniform_DM, a_LS_Uniform_DM);
        gl.uniform4fv(u_LPosition_Unform_DM, a_LPosition_Uniform_DM);

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

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 0.0, 0.0, -3.0]);
    mat4.rotateY(RotationMatrix, RotationMatrix, degToRad(angle));
    
    mat4.multiply(ModelMatrix, TranlationMatrix, RotationMatrix);
    
    gl.uniformMatrix4fv(mUniform_DM, false, ModelMatrix);
    gl.uniformMatrix4fv(vUniform_DM, false, ViewMatrix);
    gl.uniformMatrix4fv(pUniform_DM, false, perspectiveProjectionMatrix);

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
    angle += 0.5;
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
