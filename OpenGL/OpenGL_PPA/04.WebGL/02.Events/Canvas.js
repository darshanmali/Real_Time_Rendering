
function main() {

    //step1 : Get Canvas from DOM (Document Object MOdel)
    /*  */ 
    var canvas = document.getElementById("DVM");

    if (!canvas) {
        //Console is inbuild function 
        console.log("obtaining canvas faild.");
    }
    else {
        console.log("obtaining canvas succeded.");
    }

    //step 2 : Get canvas's width and hieght for conformation positional argument 
    console.log("canvas Width = " + canvas.clientWidth + " Canvas Height : " + canvas.clientHeight + "\n");

    //Step 3 : Get drowing Context from the Canvas
    /*canvas.getContext */
    var context = canvas.getContext("2d");

    if (!context) {
        console.log("obtaining Context faild.");
    }
    else {
        console.log("obtaining Context succeded.");
    }

    //Step 4 : paint background by Black Color

    context.fillStyle = "black";
    context.fillRect(0, 0, 800, 600);

    //Step 5 : Center the future comming Text 
    context.textAlign = "center"; // horizontal center
    context.textBaseline = "middle"; // Vertical center
    context.font = "48px serif";

    //Declare the string to display
    var str = "Hello world!!!";

    //Color the text "
    context.fillStyle = "green";
    context.fillText(str, canvas.clientWidth / 2, canvas.clientHeight / 2);

    /*Telling system to handle events
    window : is inbuild variable 
    "keydown" : this is inbuild variable (this is trigered when key down event is called)
    KeyDown : aapan aaplya variable madhe capture kartoy haa event
    */
    window.addEventListener("keydown", keyDown, false);

    //
    window.addEventListener("click", mouseDown, false);

}

//event Handler and listeners 
/* this function automatically come with the event code */ 
function keyDown(event) {
    
    alert("Key is pressed...!!");
}


function mouseDown() {
    
    alert("Mouse Button is Clicked...!!");
}