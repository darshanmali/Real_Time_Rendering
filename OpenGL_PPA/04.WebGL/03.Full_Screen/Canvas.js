
var canvas  = null;
var context = null;

function main() {

    //step1 : Get Canvas from DOM (Document Object MOdel)
    /*  */ 
    canvas = document.getElementById("DVM");

    if (!canvas) {
        //Console is inbuild function 
        console.log("obtaining canvas faild.");
    }
    else {
        console.log("obtaining canvas succeded.");
    }

    //step 2 : Get canvas's width and hieght for conformation positional argument 
    console.log("canvas Width = " + canvas.clientWidth + " Canvas Height : " + canvas.clientHeight + "\n");

    context = canvas.getContext("2d");

    if (!context) {
        console.log("obtaining Context faild.");
    }
    else {
        console.log("obtaining Context succeded.");
    }

    //Step 4 : paint background by Black Color
    context.fillStyle = "black";
    context.fillRect(0, 0, 800, 600);
    drowTex("Hello world.!!!");
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);

}

function drowTex(tex)
{
    context.textAlign = "center"; 
    context.textBaseline = "middle";
    context.font = "48px serif";

    context.fillStyle = "green";
    context.fillText(tex, canvas.clientWidth / 2, canvas.clientHeight / 2);

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
    }
    else if(fullscreen_element == document.msFullscreenElement)
    {
        canvas.msFullscreenElement();
    }
    else
    {
        if(document.exitFullscreen)
        {
            document.exitFullscreen();
        }
    }

}

//event Handler and listeners 
/* this function automatically come with the event code */ 
function keyDown(event) {
    switch (event.keyCode) {
        case 70:
            ToggleFullscreen();
            drowTex("Hello world.!!!");
            break;
    
        default:
            break;
    }
}


function mouseDown() {
    
}