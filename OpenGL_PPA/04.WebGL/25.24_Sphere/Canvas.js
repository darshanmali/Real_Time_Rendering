
var canvas = null;
var gl = null;

var original_width;
var original_height;

var angle = 0.05;
var a = 1;

var x;
var y;
var w;
var h;
var iterator_Color;

const WebGLMacros =
{
    DVM_ATRIBUTE_VERTEX : 0,
    DVM_ATRIBUTE_NORMALS : 1,
    DVM_ATRIBUTE_COLOR : 2,
    DVM_ATRIBUTE_TEXTURE0 : 3,

}
var a_LPosition_Uniform_DM = [];

var u_LA_Uniform_DM_PF;
var u_LD_Uniform_DM_PF;
var u_LS_Uniform_DM_PF;
var u_LPosition_Uniform_DM_PF;

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
var X_Rotation = false;
var Y_Rotation = false;
var Z_Rotation = false;

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

    var material_Ambient = [ 
        [0.0215, 0.1745, 0.0215],
       [0.135, 0.2225, 0.1575],
       
       [0.05375, 0.05, 0.06625],
       [0.25, 0.20725, 0.20725],
       
       [0.1745, 0.01175, 0.01175],
       [1.0, 0.18725, 0.1745],
       
       [0.329412, 0.223529, 0.027451],
       [0.2125, 0.1275, 0.054],
       
       [0.25, 0.25, 0.25],
       [0.19125, 0.0735, 0.0225],
       
       [0.24725, 0.1995, 0.0745],
       [0.19225, 0.19225, 0.19225],
       
       //1st - 3rd
       [0.0, 0.0, 0.0],
       [0.0,0.1,0.06],
       
       [0.0, 0.0, 0.0],
       [0.0, 0.0, 0.0],
       
       [0.0, 0.0, 0.0],
       [0.0, 0.0, 0.0],
       
       //1st - 4rd
       [0.02, 0.02, 0.02],
       [0.0, 0.05, 0.05],
       
       [0.0, 0.05, 0.0],
       [0.05, 0.0, 0.0],
       
       [0.05, 0.05, 0.05],
       [0.05, 0.05, 0.0],
       
    ];
       
    var material_Diffuse = [
           [0.07568, 0.61424, 0.07568,],
           [0.54, 0.89, 0.63,],
       [0.18275, 0.17, 0.22525],
       [1.0, 0.829, 0.829],
       
       [0.61424, 0.04136, 0.04136],
      [0.396, 0.74151, 0.69102], 
       
       [0.780392, 0.568627, 0.113725],
       [0.714, 0.4284, 0.18144],
       
       [0.4, 0.4, 0.4],
       [0.7038, 0.27048, 0.0828],
       
       [0.75164, 0.60648, 0.22648],
       [0.50754, 0.50754,0.50754],
       
       //1st - 3rd
       [0.01, 0.01, 0.01],
       [0.0, 0.50980392, 0.50980392],
       
       [0.1, 0.35, 0.1],
       [0.5, 0.0, 0.0],
       
       [0.55, 0.55, 0.55],
       [0.5, 0.5, 0.0],
       
       //1st - 4rd
       [0.01, 0.01, 0.01],
       [0.4, 0.5, 0.5],
       
       [0.4, 0.5, 0.4],
       [0.5, 0.4, 0.4],
       
       [0.5, 0.5, 0.5],
       [0.5, 0.5, 0.4]
    ];
       
    var material_Specular = [
           
        [0.633, 0.727811, 0.633,],
        [0.316228, 0.316228, 0.316228,],
       
       [0.332741, 0.328634, 0.346435],
       [0.296648, 0.296648, 0.296648],
       
       [0.727811, 0.626959, 0.626959],
       [0.297254, 0.30829, 0.306678],
       
       [0.992157, 0.941176, 0.807843],
       [0.393548, 0.271906, 0.166721],
       
       [0.774597, 0.774597, 0.774597],
       [0.256777, 0.137622, 0.086014],
       
       [0.628281, 0.555802, 0.366065],
       [0.508273, 0.508273, 0.508273],
       
       //1st - 3rd
       [0.50, 0.50, 0.50],
       [0.50196078, 0.50196078, 0.50196078],
       
       [0.45, 0.55, 0.45],
       [0.7, 0.6, 0.6],
       
       [0.70, 0.70, 0.70],
       [0.60, 0.60, 0.50],
       
       //1st - 4rd
       [0.4, 0.4, 0.4],
       [0.04, 0.7, 0.7],
       
       [0.04, 0.7, 0.04],
       [0.7, 0.04, 0.04],
       
       [0.7, 0.7, 0.7],
       [0.7, 0.7, 0.04]
       
    ];
       
    var Shininess = [
           [0.6 * 128.0,],
           [0.1 * 128.0,],
        
           [0.3 * 128.0],
           [0.088 * 128.0],
           
           [0.6 * 128.0],
           [0.1 * 128.0],
           
           [0.21794872 *128.0],
           [0.2 * 128.0],
           
           [0.6 * 128.0],
           [0.1 * 128.0],
       
           [0.4 * 128.0],
           [0.4 * 128.0],
       
           //1st - 3rd
           [0.25 * 128.0],
           [0.25 * 128.0],
       
           [0.25 * 128.0],
           [0.25 * 128.0],
           
           [0.25 * 128.0],
           [0.25 * 128.0],
       
           //1st - 4rd
           [0.078125 * 128.0],
           [0.078125 * 128.0],
       
           [0.078125 * 128.0],
          [0.078125 * 128.0],
           
           [0.078125 * 128.0],
           [0.078125 * 128.0],
       
    ];

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
            case 88:
                X_Rotation = true;
                Y_Rotation = false;
                Z_Rotation = false;
            break;
            case 89:
                X_Rotation = false;
                Y_Rotation = true;
                Z_Rotation = false;
            break;
            case 90:
                X_Rotation = false;
                Y_Rotation = false;
                Z_Rotation = true;
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
        "uniform vec3 u_la;\n" +
        "uniform vec3 u_ld;\n" +
        "uniform vec3 u_ls;\n" +
        "uniform vec4 u_light_position;\n" +
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
        "vec3 ambient;\n" +
        "vec3 diffuse;\n" +
        "vec3 specular;\n" +
        "vec3 light_direction;\n" +
        "vec3 reflection_vector;\n" +
        "light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" +
        "reflection_vector = reflect(-light_direction, transformed_normal);\n" +
        "ambient = u_la * u_ka;\n" +
        "diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);\n" +
        "specular = u_ls * u_ks * pow( max(dot(reflection_vector, view_vector), 0.0), u_shininess);\n" +
        "fong_ads_light = fong_ads_light + ambient + diffuse + specular;\n" +
        "FragColor = vec4(fong_ads_light, 1.0f);\n"+
        "}\n" +
        "else{\n" +
        "FragColor = vec4(1.0, 1.0, 1.0, 1.0f);\n"+
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
    
    u_LA_Uniform_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_la");
    u_LD_Uniform_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ld");
    u_LS_Uniform_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_ls");
    u_LPosition_Uniform_DM_PF = gl.getUniformLocation( ShaderProgramObject_DM_PF, "u_light_position");
    
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

    //gl.viewport(0, 0, canvas.width, canvas.height);
    
    mat4.perspective(perspectiveProjectionMatrix, 
        45.0, 
        parseFloat(canvas.width) / parseFloat(canvas.height), 
        0.1, 
        100.0);
}

function display() {
    

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    var ModelMatrix = mat4.create();
    var ViewMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    
    gl.useProgram(ShaderProgramObject_DM_PF);

    if(light_Switch)
        {
            if(X_Rotation)
            {
                a_LPosition_Uniform_DM = new Float32Array([0.0, Math.sin(angle), Math.cos(angle), 1.0]);
            } 
            if(Y_Rotation)
            {
                a_LPosition_Uniform_DM = new Float32Array([ Math.sin(angle), 0.0, Math.cos(angle), 1.0]);
            }
            if(Z_Rotation)
            {
                a_LPosition_Uniform_DM = new Float32Array([ Math.sin(angle), Math.cos(angle), 0.0, 1.0]);
            }

            a_LA_Uniform_DM = new Float32Array([0.0, 0.0, 0.0]);
            a_LD_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
            a_LS_Uniform_DM = new Float32Array([1.0, 1.0, 1.0]);
                
            gl.uniform1i(u_key_Pressed_DM_PF, 1);
        
            gl.uniform3fv(u_LA_Uniform_DM_PF, a_LA_Uniform_DM);
            gl.uniform3fv(u_LD_Uniform_DM_PF, a_LD_Uniform_DM);
            gl.uniform3fv(u_LS_Uniform_DM_PF, a_LS_Uniform_DM);
            gl.uniform4fv(u_LPosition_Uniform_DM_PF, a_LPosition_Uniform_DM);
    }
    else{
            gl.uniform1i(u_key_Pressed_DM_PF, 0);
    }

    for (var i = 0; i < 4; i++) {
        for (var j = 0; j < 6; j++) {
     
            x = j * (canvas.width / 6);
            y = i * (canvas.width / 6);
            w = canvas.width / 6;
            h = canvas.height / 6;
            iterator_Color++;

            gl.viewport(x, y, w, h);
    
            mat4.identity(TranlationMatrix);
            mat4.identity(ModelMatrix);
            mat4.identity(ViewMatrix);

            a_KA_Uniform_DM = new Float32Array(material_Ambient[i*j]);
            a_KD_Uniform_DM = new Float32Array(material_Diffuse[i*j]);
            a_KS_Uniform_DM = new Float32Array(material_Specular[i*j]);
            a_Shininess_DM = Shininess[i*j];
            
            gl.uniform3fv(u_KA_Uniform_DM_PF, a_KA_Uniform_DM);
            gl.uniform3fv(u_KD_Uniform_DM_PF, a_KD_Uniform_DM);
            gl.uniform3fv(u_KS_Uniform_DM_PF, a_KS_Uniform_DM);
            gl.uniform1f(u_Shininess_DM_PF, a_Shininess_DM);
            
            mat4.translate(TranlationMatrix, TranlationMatrix, [ 0.0, 0.0, -1.0]);
            
            mat4.multiply(ModelMatrix, ModelMatrix, TranlationMatrix);
            
            gl.uniformMatrix4fv(mUniform_DM_PF, false, ModelMatrix);
            gl.uniformMatrix4fv(vUniform_DM_PF, false, ViewMatrix);
            gl.uniformMatrix4fv(pUniform_DM_PF, false, perspectiveProjectionMatrix);
            
            Sphere_DM.draw();

        }
    }

    
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
    if (ShaderProgramObject_DM_PF) {
        if (fragmentShaderObject_DM_PF) {
            gl.detachShader(ShaderProgramObject_DM_PF, fragmentShaderObject_DM_PF);
            gl.deleteShader(fragmentShaderObject_DM_PF);
            fragmentShaderObject_DM_PF = null;

        }

        if (vertexShaderObject_DM_PF) {
            gl.detachShader(ShaderProgramObject_DM_PF, vertexShaderObject_DM_PF);
            gl.deleteShader(vertexShaderObject_DM_PF);
            vertexShaderObject_DM_PF = null;
        }
        gl.deleteProgram(ShaderProgramObject_DM_PF);
        ShaderProgramObject_DM_PF = null;
    }
}
