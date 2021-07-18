
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

var vertexShaderObject;
var fragmentShaderObject;
var ShaderProgramObject;

var Square_vao_DM;
var Square_Vertex_vbo_DM;
var Square_Color_vbo_DM;

var Triangle_vao_DM;
var Triangle_Vertex_vbo_DM;
var Triangle_Color_vbo_DM;

var mvpUniform_DM;

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
        "#version 300 es            \n" +
        "in vec4 vPosition;         \n" +
        "in vec4 vColor;            \n" +
		"out vec4 outColor;         \n" +
		"uniform mat4 u_mvp_matrix; \n" +
		"void main(void){           \n" +
		"gl_Position = u_mvp_matrix * vPosition;" +
		"outColor = vColor;         \n" +
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
        "in vec4 outColor;              \n" +
		"out vec4 FragColor;            \n" +
		"void main(void){               \n" +
		"FragColor = outColor;          \n" +
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
    gl.bindAttribLocation(ShaderProgramObject, WebGLMicros.DVM_ATTTRIBUTE_COLOR, "vColor");

    gl.linkProgram(ShaderProgramObject);
    if (!gl.getProgramParameter(ShaderProgramObject, gl.LINK_STATUS)) {
        var error = gl.getProgramInfoLog(ShaderProgramObject);
        if (error.length > 0) {
            alert(error);
            unInitilize();
        }
    }

    mvpUniform_DM = gl.getUniformLocation(ShaderProgramObject, "u_mvp_matrix");

    var Triangle_Vertices = new Float32Array([
        0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0]);

    var Triangle_Color = new Float32Array([
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0]);

    var Square_Vertices = new Float32Array([
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0]);

    var Square_Color = new Float32Array([
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0]);


    /* Square Vertex Object */

    Sqaure_vao_DM = gl.createVertexArray();
    gl.bindVertexArray(Sqaure_vao_DM);

    Square_Vertex_vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Square_Vertex_vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, Square_Vertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_VERTEX,
        3,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);


    Square_Color_vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Square_Color_vbo_DM);

    gl.bufferData(gl.ARRAY_BUFFER, Square_Color, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_COLOR,
        3,
        gl.FLOAT,
        false,
        0,
        0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_COLOR);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);

    /* Triangle Vertex Object */

    Triangle_vao_DM = gl.createVertexArray();
    gl.bindVertexArray(Triangle_vao_DM);

    Triangle_Vertex_vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Triangle_Vertex_vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, Triangle_Vertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_VERTEX,
        3,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    
    Triangle_Color_vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Triangle_Color_vbo_DM);

    gl.bufferData(gl.ARRAY_BUFFER, Triangle_Color, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_COLOR,
        3,
        gl.FLOAT,
        false,
        0,
        0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_COLOR);
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

    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    var XRotationMatrix = mat4.create();

    mat4.identity(TranlationMatrix);
    mat4.identity(modelViewMatrix);
    mat4.identity(modelViewProjectionMatrix);
    mat4.identity(XRotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ -2.0, 0.0, -4.0]);

    mat4.rotateY(XRotationMatrix, XRotationMatrix, angle);

    mat4.multiply(modelViewMatrix, TranlationMatrix, XRotationMatrix);
    
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    
    gl.uniformMatrix4fv(mvpUniform_DM, false, modelViewProjectionMatrix);

    gl.bindVertexArray(Triangle_vao_DM);

    gl.drawArrays(gl.TRIANGLE_FAN, 0, 3);
    gl.bindVertexArray(null);

    mat4.identity(TranlationMatrix);
    mat4.identity(modelViewMatrix);
    mat4.identity(modelViewProjectionMatrix);
    mat4.identity(XRotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 2.0, 0.0, -4.0]);

    mat4.rotateY(XRotationMatrix, XRotationMatrix, angle);

    mat4.multiply(modelViewMatrix, TranlationMatrix, XRotationMatrix);
    
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    
    gl.uniformMatrix4fv(mvpUniform_DM, false, modelViewProjectionMatrix);

    gl.bindVertexArray(Sqaure_vao_DM);

    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
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
    if (Triangle_vao_DM) {
        gl.deleteVertexArray(Triangle_vao_DM);
        Triangle_vao_DM = 0;
    }

    if (Square_Vertex_vbo_DM) {
        gl.deleteBuffer(Square_Vertex_vbo_DM);
        Square_Vertex_vbo_DM = 0;
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
