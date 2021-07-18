
var canvas = null;
var gl = null;

var original_width;
var original_height;

var angle = 0.05;

var bfullscreen;

const WebGLMicros =
{
    DVM_ATTTRIBUTE_VERTEX : 0,
    DVM_ATTTRIBUTE_NORMAL : 1,
    DVM_ATTTRIBUTE_COLOR : 2,
    DVM_ATTTRIBUTE_TEXTURE0 : 3,

}

var L_Button_Down_DM;
var LD_Uniform_DM;
var KD_Uniform_DM;
var LPosition_Uniform_DM;

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
		"uniform mat4 u_model_view_matrix;" +
		"uniform mat4 u_p_matrix;" +
	    "uniform int u_lKey;" +
		"uniform vec3 u_ld;" +
		"uniform vec3 u_kd;" +
		"uniform vec4 u_light_position;" +
		"out vec3 diffuse_light;" +
		"void main(void){\n" +
		"if(u_lKey == 1){\n" +
		"vec4 eye_cordinates = vec4(u_model_view_matrix * vPosition);\n" +
		"mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));\n" +
		"vec3 tnormal = normalize(normal_matrix * vNormal);\n" +
		"vec3 s = normalize(vec3(u_light_position - eye_cordinates));\n" +
		"diffuse_light = u_ld * u_kd * max( dot(s , tnormal), 0.0);\n" +
		"}\n" +
		"gl_Position = u_p_matrix * u_model_view_matrix * vPosition;\n" +
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
        "precision highp int;\n" +
		"in vec3 diffuse_light;\n" +
		"uniform int u_lKey;\n" +
		"out vec4 FragColor;\n" +
		"void main(void){\n" +
		"vec4 color;\n" +
		"if(u_lKey == 1){\n" +
		"color = vec4( diffuse_light, 1.0);\n" +
		"}\n" +
		"else{\n" +
		"color = vec4( 1.0, 1.0, 1.0, 1.0);\n" +
		"}\n" +
		"FragColor = color;\n" +
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
    //mvpUniform_DM = gl.getUniformLocation(ShaderProgramObject, "u_mvp_matrix");
    
    mvUniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_model_view_matrix");
    pUniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_p_matrix");
    L_Button_Down_DM = gl.getUniformLocation( ShaderProgramObject, "u_lKey");
    LD_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_ld");
    KD_Uniform_DM = gl.getUniformLocation( ShaderProgramObject, "u_kd");
    LPosition_Unform_DM = gl.getUniformLocation( ShaderProgramObject, "u_light_position");

    var CubeVertices = new Float32Array([
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
     
     0.5, 0.5, -0.5,
     0.5, 0.5, 0.5,
     0.5, -0.5, 0.5,
     0.5, -0.5, -0.5,

     0.5, 0.5, -0.5,
     -0.5, 0.5, -0.5,
     -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5,

     -0.5, 0.5, -0.5,
     -0.5, 0.5, 0.5,
     -0.5, -0.5, 0.5,
     -0.5, -0.5, -0.5,

     0.5, 0.5, 0.5,
     -0.5, 0.5, 0.5,
     -0.5, 0.5, -0.5,
     0.5, 0.5, -0.5,

     0.5, -0.5, 0.5,
     -0.5, -0.5, 0.5,
     -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5
    ]);

     var CubeColor = new Float32Array([
        0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,

	    1.0, 0.0, 0.0,
	    1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
	    1.0, 0.0, 0.0,

	    0.0, 0.0, -1.0,
	    0.0, 0.0, -1.0,
    	0.0, 0.0, -1.0,
    	0.0, 0.0, -1.0,

    	-1.0, 0.0, 0.0,
    	-1.0, 0.0, 0.0,
	    -1.0, 0.0, 0.0,
	    -1.0, 0.0, 0.0,

	    0.0, 1.0, 0.0,
	    0.0, 1.0, 0.0,
	    0.0, 1.0, 0.0,
	    0.0, 1.0, 0.0,

	    0.0, -1.0, 0.0,
	    0.0, -1.0, 0.0,
	    0.0, -1.0, 0.0,
        0.0, -1.0, 0.0

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
        var Light_D = new Float32Array([1.0, 1.0, 1.0]);
        var Light_P = new Float32Array([0.0, 0.0, 2.0, 1.0]);
        var Material_D = new Float32Array([1.0, 1.0, 1.0]);

        gl.uniform1i(L_Button_Down_DM, 1);
        gl.uniform3fv(LD_Uniform_DM, Light_D);
        gl.uniform3fv(KD_Uniform_DM, Material_D);
        gl.uniform4fv(LPosition_Unform_DM, Light_P);
    }
    else{
        gl.uniform1i(L_Button_Down_DM, 0);
    }


    var modelViewMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    var XRotationMatrix = mat4.create();

    mat4.identity(TranlationMatrix);
    mat4.identity(modelViewMatrix);
    mat4.identity(XRotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 0.0, 0.0, -3.0]);

    mat4.rotateY(XRotationMatrix, XRotationMatrix, angle);

    mat4.multiply(modelViewMatrix, TranlationMatrix, XRotationMatrix);
    
    gl.uniformMatrix4fv(mvUniform_DM, false, modelViewMatrix);
    gl.uniformMatrix4fv(pUniform_DM, false, perspectiveProjectionMatrix);

    gl.bindVertexArray(vao_DM);
    
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);

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
    angle += 0.05;
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
