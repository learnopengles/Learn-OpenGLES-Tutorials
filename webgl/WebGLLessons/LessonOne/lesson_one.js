/**
 * Lesson_one.js
 */

var context = null;    // Hold the canvas context

// Main entry point
function main()
{
    // Try to get a WebGL context
    var contextsToTry = ["webgl", "experimental-webgl"];

    for (var i = 0; i < names.length; i++) 
    {
        try 
        {
            context = canvas.getContext(names[i]);
        } 
        catch(e) 
        {
            // Ignore
        }
        
        if (context) 
        {
        	break;
        }
    }    
}

// Execute the main entry point
main();