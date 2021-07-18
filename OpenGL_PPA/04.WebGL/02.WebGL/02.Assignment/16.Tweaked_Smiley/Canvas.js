
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
    DVM_ATTTRIBUTE_TEXTURE : 3,

}
var Image_Switch = 1;

var vertexShaderObject;
var fragmentShaderObject;
var ShaderProgramObject;

var Square_Vbo_DM;
var Square_Vertices_Vbo_DM;
var Square_Texture_Vbo_DM;

var mvpUniform_DM;

/* Texture Variables */
var Smiley_Texture;

var texture_Sampler_Uniform;

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
        case 49:
            Image_Switch = 1;
        break;
        case 50:
            Image_Switch = 2;
        break;
        case 51:
            Image_Switch = 3;
            break;
        case 52:
            Image_Switch = 4;
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
        "in vec2 vTexCoord;             \n" +
		"out vec2 out_TexCoord;         \n" +
		"uniform mat4 u_mvp_matrix;     \n" +
		"void main(void){               \n" +
		"gl_Position = u_mvp_matrix * vPosition;\n" +
		"out_TexCoord = vTexCoord;             \n" +
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
        "uniform highp sampler2D u_Texture_Sampler;\n" +
        "in vec2 out_TexCoord;          \n" +
		"out vec4 FragColor;            \n" +
		"void main(void){               \n" +
		"FragColor = texture(u_Texture_Sampler, out_TexCoord);\n" +
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
    gl.bindAttribLocation(ShaderProgramObject, WebGLMicros.DVM_ATTTRIBUTE_TEXTURE, "vTexCoord");


    gl.linkProgram(ShaderProgramObject);
    if (!gl.getProgramParameter(ShaderProgramObject, gl.LINK_STATUS)) {
        var error = gl.getProgramInfoLog(ShaderProgramObject);
        if (error.length > 0) {
            alert(error);
            unInitilize();
        }
    }

    mvpUniform_DM = gl.getUniformLocation(ShaderProgramObject, "u_mvp_matrix");
    texture_Sampler_Uniform = gl.getUniformLocation(ShaderProgramObject,"u_Texture_Sampler");

    var Square_Vertices = new Float32Array([
    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0, -1.0, 1.0,
    1.0, -1.0, 1.0,
     
    ]);

   
    /* Vertex VBO for Smiley */
    Square_Vbo_DM = gl.createVertexArray();
    gl.bindVertexArray(Square_Vbo_DM);

    Square_Vertices_Vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Square_Vertices_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, Square_Vertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_VERTEX,
        3,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    /* Texture VBO for Smiley */
    Square_Texture_Vbo_DM = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, Square_Texture_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, 8, gl.DYNAMIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE,
        2,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    gl.enable(gl.DEPTH_TEST);
    
    gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
    
    /* SMILEY CREATION */
    //createtexture == genTexture();
    Smiley_Texture = gl.createTexture();
    Smiley_Texture.image = new Image();
    Smiley_Texture.image.src = "Res/smiley.png";
    /*lamda : fuction pointer la function cha address na deta aapan directally fuction chi body lihat aahot.
    aata aapan hya tikani onload chya samor fuction ch naav dyayala hav hot but aapan aata fuctionchi body liat aahot directally
    */
    Smiley_Texture.image.onload = function (){
        /* this is similar to loadGLTexcture() from previous platform like windows, linux(soil).*/
        gl.bindTexture(gl.TEXTURE_2D, Smiley_Texture);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, Smiley_Texture.image);

        gl.bindTexture(gl.TEXTURE_2D, null);

    }

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

    if(Image_Switch == 1)
    {
        
        var Square_TexCoords = new Float32Array([
            1.0, 1.0,
		    0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0
        ]);
    }
    else if(Image_Switch == 2)
    {
        
        var Square_TexCoords = new Float32Array([
            0.5, 0.5,
            0.0, 0.5,
		    0.0, 0.0,
		    0.5, 0.0
        ]);
    }
    else if(Image_Switch == 3)
    {
        
        var Square_TexCoords = new Float32Array([
            2.0, 2.0,
		    0.0, 2.0,
		    0.0, 0.0,
		    2.0, 0.0
        ]);
    }
    else if(Image_Switch == 4)
    {
        
        var Square_TexCoords = new Float32Array([
            0.5, 0.5,
		    0.5, 0.5,
		    0.5, 0.5,
		    0.5, 0.5
        ]);
    }


    gl.useProgram(ShaderProgramObject);

    gl.bindBuffer(gl.ARRAY_BUFFER, Square_Texture_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, Square_TexCoords, gl.DYNAMIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE,
        2,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    var XRotationMatrix = mat4.create();

    mat4.identity(TranlationMatrix);
    mat4.identity(modelViewMatrix);
    mat4.identity(modelViewProjectionMatrix);
    mat4.identity(XRotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 0.0, 0.0, -4.0]);

    mat4.rotateY(XRotationMatrix, XRotationMatrix, angle);

    mat4.multiply(modelViewMatrix, TranlationMatrix, XRotationMatrix);
    
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    
    gl.uniformMatrix4fv(mvpUniform_DM, false, modelViewProjectionMatrix);
    
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, Smiley_Texture);
    gl.uniform1i(texture_Sampler_Uniform, 0);

    gl.bindVertexArray(Square_Vbo_DM);
    
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);

    gl.bindVertexArray(null);
    gl.useProgram(null);

    requestAnimationFrame(display, canvas);
}

function degToRad(Degrees)
{
    return (Degrees * Math.PI / 180.0);
}

function unInitilize() {
    if (Square_Vbo_DM) {
        gl.deleteVertexArray(Square_Vbo_DM);
        Square_Vbo_DM = 0;
    }

    if (Square_Vertices_Vbo_DM) {
        gl.deleteBuffer(Square_Vertices_Vbo_DM);
        Square_Vertices_Vbo_DM = 0;
    }
    if (Square_Texture_Vbo_DM) {
        gl.deleteBuffer(Square_Texture_Vbo_DM);
        Square_Texture_Vbo_DM = 0;
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
