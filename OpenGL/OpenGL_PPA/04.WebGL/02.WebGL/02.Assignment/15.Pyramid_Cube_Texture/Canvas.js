
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

var vertexShaderObject;
var fragmentShaderObject;
var ShaderProgramObject;

var Cube_Vbo_DM;
var Cube_Vertices_Vbo_DM;
var Cube_Texture_Vbo_DM;

var Pyramid_vao_DM;
var Pyramid_Vertices_Vbo_DM;
var Pyramid_Texture_Vbo_DM;

var mvpUniform_DM;

/* Texture Variables */
var kundali_Texture;
var STONE_Texture;

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

    var PyramidVertices = new Float32Array([
        0.0, 1.0, 0.0,
    	-1.0, -1.0, 1.0,
    	1.0, -1.0, 1.0,
        
        0.0, 1.0, 0.0,
	    1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,
        
        0.0, 1.0, 0.0,
        1.0, -1.0, -1.0,
	    -1.0, -1.0, -1.0,
        
        0.0, 1.0, 0.0,
    	-1.0, -1.0, 1.0,
	    -1.0, -1.0, -1.0
    ]);

    var PyramidTexture = new Float32Array([  
        0.5, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        0.5, 1.0,
        1.0, 0.0,
        0.0, 0.0,

        0.5, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        0.5, 1.0,
        1.0, 0.0,
        0.0, 0.0
    
    ]);

    var CubeVertices = new Float32Array([
    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0, -1.0, 1.0,
    1.0, -1.0, 1.0,
     
    1.0, 1.0, -1.0,
     1.0, 1.0, 1.0,
     1.0, -1.0, 1.0,
     1.0, -1.0, -1.0,

     1.0, 1.0, -1.0,
     -1.0, 1.0, -1.0,
     -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,

     -1.0, 1.0, -1.0,
     -1.0, 1.0, 1.0,
     -1.0, -1.0, 1.0,
     -1.0, -1.0, -1.0,

     1.0, 1.0, 1.0,
     -1.0, 1.0, 1.0,
     -1.0, 1.0, -1.0,
     1.0, 1.0, -1.0,

     1.0, -1.0, 1.0,
     -1.0, -1.0, 1.0,
     -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0
    ]);

     var CubeTexCoords = new Float32Array([
        0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,

		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,

		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,

		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,

		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,

		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0
    
     ]);

     /* Pyramid VBO  */
    Pyramid_vao_DM = gl.createVertexArray();
    gl.bindVertexArray(Pyramid_vao_DM);

    Pyramid_Vertices_Vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Pyramid_Vertices_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, PyramidVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_VERTEX,
        3,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    Pyramid_Color_Vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Pyramid_Color_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, PyramidTexture, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE,
        2,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

      /* Cube VBO  */
    Cube_Vbo_DM = gl.createVertexArray();
    gl.bindVertexArray(Cube_Vbo_DM);

    Cube_Vertices_Vbo_DM = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Cube_Vertices_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, CubeVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_VERTEX,
        3,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    Cube_Texture_Vbo_DM = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, Cube_Texture_Vbo_DM);
    gl.bufferData(gl.ARRAY_BUFFER, CubeTexCoords, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE,
        2,
        gl.FLOAT,
        false, 0, 0);

    gl.enableVertexAttribArray(WebGLMicros.DVM_ATTTRIBUTE_TEXTURE);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    gl.enable(gl.DEPTH_TEST);
    
    gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
    
    /* STONE CREATION */

    //createtexture == genTexture();
    STONE_Texture = gl.createTexture();
    STONE_Texture.image = new Image();
    STONE_Texture.image.src = "Res/stone.png";
    /*lamda : fuction pointer la function cha address na deta aapan directally fuction chi body lihat aahot.
    aata aapan hya tikani onload chya samor fuction ch naav dyayala hav hot but aapan aata fuctionchi body liat aahot directally
*/
STONE_Texture.image.onload = function (){
    /* this is similar to loadGLTexcture() from previous platform like windows, linux(soil).*/
        gl.bindTexture(gl.TEXTURE_2D, STONE_Texture);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, STONE_Texture.image);

        gl.bindTexture(gl.TEXTURE_2D, null);

    }
    

    /* KUNDALI CREATION */
    //createtexture == genTexture();
    kundali_Texture = gl.createTexture();
    kundali_Texture.image = new Image();
    kundali_Texture.image.src = "Res/Vijay_Kundali.png";
    /*lamda : fuction pointer la function cha address na deta aapan directally fuction chi body lihat aahot.
    aata aapan hya tikani onload chya samor fuction ch naav dyayala hav hot but aapan aata fuctionchi body liat aahot directally
*/
    kundali_Texture.image.onload = function (){
        /* this is similar to loadGLTexcture() from previous platform like windows, linux(soil).*/
        gl.bindTexture(gl.TEXTURE_2D, kundali_Texture);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, kundali_Texture.image);

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

    gl.useProgram(ShaderProgramObject);

    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var TranlationMatrix = mat4.create();
    var XRotationMatrix = mat4.create();

    mat4.identity(TranlationMatrix);
    mat4.identity(modelViewMatrix);
    mat4.identity(modelViewProjectionMatrix);
    mat4.identity(XRotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ -2.0, 0.0, -5.0]);

    mat4.rotateY(XRotationMatrix, XRotationMatrix, angle);

    mat4.multiply(modelViewMatrix, TranlationMatrix, XRotationMatrix);
    
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    
    gl.uniformMatrix4fv(mvpUniform_DM, false, modelViewProjectionMatrix);
    
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, STONE_Texture);
    gl.uniform1i(texture_Sampler_Uniform, 0);

    gl.bindVertexArray(Pyramid_vao_DM);
    
    gl.drawArrays(gl.TRIANGLES, 0, 12);

    gl.bindVertexArray(null);

    mat4.identity(TranlationMatrix);
    mat4.identity(modelViewMatrix);
    mat4.identity(modelViewProjectionMatrix);
    mat4.identity(XRotationMatrix);

    mat4.translate(TranlationMatrix, TranlationMatrix, [ 2.0, 0.0, -5.0]);

    mat4.rotateY(XRotationMatrix, XRotationMatrix, angle);

    mat4.multiply(modelViewMatrix, TranlationMatrix, XRotationMatrix);
    
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    
    gl.uniformMatrix4fv(mvpUniform_DM, false, modelViewProjectionMatrix);
    
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, kundali_Texture);
    gl.uniform1i(texture_Sampler_Uniform, 0);

    gl.bindVertexArray(Cube_Vbo_DM);
    
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
    angle += 0.025;
    if(angle > 360.0)
    {
        angle = 0.0;
    }
}

function unInitilize() {
    if (Cube_Vbo_DM) {
        gl.deleteVertexArray(Cube_Vbo_DM);
        Cube_Vbo_DM = 0;
    }

    if (Cube_Vertices_Vbo_DM) {
        gl.deleteBuffer(Cube_Vertices_Vbo_DM);
        Cube_Vertices_Vbo_DM = 0;
    }
    if (Cube_Texture_Vbo_DM) {
        gl.deleteBuffer(Cube_Texture_Vbo_DM);
        Cube_Texture_Vbo_DM = 0;
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
