
var canvas  = null;
var gl = null;

var original_width;
var original_height;

var bfullscreen;

var requestAnimation = window.requestAnimationFrame ||
                    window.webkitRequestAnimationFrame ||
                    windows.oRequestAnimationFrame ||
                    window.msRequestAnimationFrame;
                    

function main() {

    canvas = document.getElementById("DVM");

    if (!canvas) {
        console.log("obtaining canvas faild.");
    }
    else {
        console.log("obtaining canvas succeded.");
    }

    console.log("canvas Width = " + canvas.clientWidth + " Canvas Height : " + canvas.clientHeight + "\n");

    original_width = canvas.clientWidth;
    original_height = canvas.clientHeight;

    
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("resize", resize, false);

    init();
    resize();//warm up
    display();//warm up cause repaint nahi hot.


}

function init() {

    //this is the most imp change 2d to webgl2
    gl = canvas.getContext("webgl");

    if (!gl) {
        console.log("obtaining WebGL2 faild.");
    }
    else {
        console.log("obtaining WebGL2 succeded.");
    }

    gl.viewportWidth = canvas.clientWidth;
    gl.viewportHeight = canvas.clientHeight;

    gl.clearColor(0.0, 0.0, 1.0, 1.0);

    resize();
}

function resize() {
    
    if (bfullscreen == true) {
        
        canvas.clientWidth = window.innerWidth;
        canvas.clientHeight = window.innerHeight;
    }
    else{
        canvas.clientWidth = original_width;
        canvas.clientHeight = original_height;
    }

    gl.viewport(0, 0, canvas.clientWidth, canvas.clientHeight);
}


function display() {
    
    gl.clear(gl.COLOR_BUFFER_BIT);

    requestAnimation = requestAnimationFrame(display, canvas);
}

function ToggleFullscreen(params) {
    
    //variable dec
    var fullscreen_element = document.fullscreenElement || // chrome, opera mini
                                document.webkitFullscreenElement || // (apple)safari
                                document.mozFullScreenElement || //modzilla fire fox
                                document.msFullscreenElement || //MS edge
                                null;

    // aapan screen la full screen aata kartoy.
    if(fullscreen_element == null)
    {
        canvas.requestFullscreen();
        bfullscreen = true;
    }
    else if(fullscreen_element == document.msFullscreenElement)
    {
        canvas.msFullscreenElement();
        bfullscreen = true;
    }
    else
    {
        if(document.exitFullscreen)
        {
            document.exitFullscreen();
        }
        bfullscreen = false;
    }

}

function keyDown(event) {
    switch (event.keyCode) {
        case 70:
            ToggleFullscreen();
            break;
    
        default:
            break;
    }
}
