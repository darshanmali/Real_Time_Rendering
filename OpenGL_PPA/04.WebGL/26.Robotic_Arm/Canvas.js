
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

var Pyramid_vao_DM;
var Vertices_Vbo_DM;
var Color_Vbo_DM;
var mvpUniform_DM;

var matrix_stack = [];
var PushToStack_count = 0;


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
        "#version 300 es                \n" +
        "in vec4 vPosition;             \n" +
        "out vec4 outColor;             \n" +
		"uniform mat4 u_mvp_matrix;     \n" +
		"void main(void){               \n" +
		"gl_Position = u_mvp_matrix * vPosition;\n" +
		"outColor = vec4(1.0, 1.0, 1.0, 1.0);   \n" +
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

    gl.bindAttribLocation(ShaderProgramObject, WebGLMacros.DVM_ATRIBUTE_VERTEX, "vPosition");

    gl.linkProgram(ShaderProgramObject);
    if (!gl.getProgramParameter(ShaderProgramObject, gl.LINK_STATUS)) {
        var error = gl.getProgramInfoLog(ShaderProgramObject);
        if (error.length > 0) {
            alert(error);
            unInitilize();
        }
    }

    mvpUniform_DM = gl.getUniformLocation(ShaderProgramObject, "u_mvp_matrix");

    Sphere_DM = new Mesh();
    makeSphere(Sphere_DM, 1.0, 30, 30);
    
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
    
    var RotateMatrix = mat4.create();
    var ScaleMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    var modelviewMatrix = mat4.create();
    var mvProjectionMatrix = mat4.create();
    var multipliedMatrix = mat4.create();

    mat4.identity(TranlationMatrix);
    mat4.identity(RotateMatrix);
    mat4.identity(ScaleMatrix);
    
    mat4.identity(modelviewMatrix);
    mat4.identity(mvProjectionMatrix);
    
    mat4.translate(TranlationMatrix, TranlationMatrix, [ 0.0, 0.0, -8.0]);

    mat4.rotateZ(RotateMatrix, RotateMatrix, angle);

    mat4.scale(ScaleMatrix, ScaleMatrix, [2.0, 0.25 ,0.25]);
    
    mat4.multiply(multipliedMatrix, TranlationMatrix, ScaleMatrix);
    
    mat4.multiply(modelviewMatrix, RotateMatrix, multipliedMatrix);
    
    //we are calling first push
    modelviewMatrix = PushMatrix(modelviewMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 2.0, 0.0, 0.0]);
    mat4.multiply(multipliedMatrix, multipliedMatrix, TranlationMatrix);
    
    mat4.multiply(mvProjectionMatrix, perspectiveProjectionMatrix, modelviewMatrix);
    
    gl.uniformMatrix4fv(mvpUniform_DM, false, mvProjectionMatrix);

    Sphere_DM.draw();

    gl.useProgram(null);

    PopMatrix();

    update();

    requestAnimationFrame(display, canvas);
}

function PushMatrix(mat) {
    console.log("Inside pushmatrix...!!!");
    var Temp;
    var return_matrix = mat4.create();

    if(PushToStack_count < 1)
    {
        console.log("Inside 1st If.....!!!");
    
        matrix_stack[PushToStack_count] = mat4.create();
        matrix_stack[PushToStack_count] = mat;
        mat4.multiply(return_matrix, return_matrix, matrix_stack[PushToStack_count]);
        PushToStack_count++;
        return(return_matrix);
    }
    else if(PushToStack_count >= 1)
    {
        matrix_stack[PushToStack_count] = mat4.create();
        
        Temp = matrix_stack[PushToStack_count - 1];
        matrix_stack[PushToStack_count] = Temp * mat;
        return_matrix = matrix_stack[PushToStack_count];
        PushToStack_count++;
        console.log("Pushed matrix to MatrixStack...!!!");
        return(return_matrix);
    }
    else{
        return(mat4.create());
    }
    
}

function PopMatrix() {
    console.log("Inside Popmatrix...!!!");
    
    var Temp;
    if(PushToStack_count < 0)
    {
        console.log("Popmatrix is called when matrix Stack is Empty...!!!");
    }
    else if(PushToStack_count >= 1)
    {
        if(PushToStack_count == 1)
        {
            PushToStack_count--;

        }
        else if(PushToStack_count >1)
        {
            PushToStack_count--;
        }
    }

}

function degToRad(Degrees)
{
    return (Degrees * Math.PI / 180.0);
}
function update()
{
    angle += 0.025;
    if(angle > 360.0)
    {
        angle = 0.0;
    }
}

function unInitilize() {
    if (Pyramid_vao_DM) {
        gl.deleteVertexArray(Pyramid_vao_DM);
        Pyramid_vao_DM = 0;
    }

    if (Vertices_Vbo_DM) {
        gl.deleteBuffer(Vertices_Vbo_DM);
        Vertices_Vbo_DM = 0;
    }
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

