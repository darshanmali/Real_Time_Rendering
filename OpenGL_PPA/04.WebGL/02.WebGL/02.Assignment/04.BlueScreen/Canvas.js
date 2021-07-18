
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

    console.log("canvas Width = " + canvas.width + " Canvas Height : " + canvas.height + "\n");

    original_width = canvas.width;
    original_height = canvas.height;

    
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("resize", resize, false);

    init();
    resize();//warm up
    display();//warm up cause repaint nahi hot.


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

    gl.clearColor(0.0, 0.0, 1.0, 1.0);

    resize();
}

function resize() {
    
    if (bfullscreen == true) {
        
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else{
        canvas.width = original_width;
        canvas.height = original_height;
    }

    gl.viewport(0, 0, window.width, window.height);
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

        if(fullscreen_element == document.requestFullscreen)
        {
            console.log("Inside the Chrome.");
            canvas.requestFullscreen();
            bfullscreen = true;
        }
        else if(fullscreen_element == document.msFullscreenElement)
        {
            console.log("Inside the MS.");
            canvas.msFullscreenElement();
            bfullscreen = true;
        }
        else if(fullscreen_element == document.webkitRequestFullscreen)
        {
            console.log("Inside the MacOS.");
            canvas.webkitRequestFullscreen();
            bfullscreen = true;
        }
        else if(fullscreen_element == document.mozFullScreenElement)
        {
            console.log("Inside the Moz.");
            canvas.mozFullScreenElement();
            fullscreen = true;
        }

    }
    else
    {
        if(document.exitFullscreen)
        {
            document.exitFullscreen();
            bfullscreen = false;
        }
        else if(document.webkitexitFullscreen)
        {
            document.webkitexitFullscreen();
            bfullscreen = false;
        }
        else if(document.mozCancelFullScreen)
        {
            document.mozCancelFullScreen();
            bfullscreen = false;
        }
        else if(document.msexitFullScreen)
        {
            document.msexitFullScreen();
            bfullscreen = false;
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
