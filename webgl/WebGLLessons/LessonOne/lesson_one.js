/**
 * Lesson_one.js
 */

// We make use of the WebGL utility library, which was downloaded from here:
// https://cvs.khronos.org/svn/repos/registry/trunk/public/webgl/sdk/demos/common/webgl-utils.js
//
// It defines two functions which we use here:
//
// // Creates a WebGL context.
// WebGLUtils.setupWebGL(canvas);
//
// // Requests an animation callback. See: https://developer.mozilla.org/en/DOM/window.requestAnimationFrame
// window.requestAnimFrame(callback, node);
//
// We also make use of the glMatrix file which can be downloaded from here:
// http://code.google.com/p/glmatrix/source/browse/glMatrix.js
//

/** Hold a reference to the WebGLContext */
var gl = null;		   

/** Hold a reference to the canvas DOM object. */
var canvas = null;	 

/**
 * Store the model matrix. This matrix is used to move models from object space (where each model can be thought
 * of being located at the center of the universe) to world space.
 */
var modelMatrix = mat4.create();

/**
 * Store the view matrix. This can be thought of as our camera. This matrix transforms world space to eye space;
 * it positions things relative to our eye.
 */
var viewMatrix = mat4.create();

/** Store the projection matrix. This is used to project the scene onto a 2D viewport. */
var projectionMatrix = mat4.create();

/** Allocate storage for the final combined matrix. This will be passed into the shader program. */
var mvpMatrix = mat4.create();

/** Store our model data in a Float32Array buffer. */
var trianglePositions;
var triangle1Colors;
var triangle2Colors;
var triangle3Colors;

/** Store references to the vertex buffer objects (VBOs) that will be created. */
var trianglePositionBufferObject;
var triangleColorBufferObject1;
var triangleColorBufferObject2;
var triangleColorBufferObject3;

/** This will be used to pass in the transformation matrix. */
var mvpMatrixHandle;

/** This will be used to pass in model position information. */
var positionHandle;

/** This will be used to pass in model color information. */
var colorHandle;

/** Size of the position data in elements. */
var positionDataSize = 3;

/** Size of the color data in elements. */
var colorDataSize = 4;	

// Helper function to load a shader
function loadShader(sourceScriptId, type)
{
	var shaderHandle = gl.createShader(type);
	var error;
	
	if (shaderHandle != 0) 
	{				
		// Read the embedded shader from the document.
		var shaderSource = document.getElementById(sourceScriptId);
		
		if (!shaderSource)
		{
			throw("Error: shader script '" + sourceScriptId + "' not found");
		}
		
		// Pass in the shader source.
		gl.shaderSource(shaderHandle, shaderSource.text);		
		
		// Compile the shader.
		gl.compileShader(shaderHandle);

		// Get the compilation status.		
		var compiled = gl.getShaderParameter(shaderHandle, gl.COMPILE_STATUS);		

		// If the compilation failed, delete the shader.
		if (!compiled) 
		{				
			error = gl.getShaderInfoLog(shaderHandle);			
			gl.deleteShader(shaderHandle);
			shaderHandle = 0;
		}
	}

	if (shaderHandle == 0)
	{
		throw("Error creating shader " + sourceScriptId + ": " + error);
	}
	
	return shaderHandle;
}

// Helper function to link a program
function linkProgram(vertexShader, fragmentShader)
{
	// Create a program object and store the handle to it.
	var programHandle = gl.createProgram();
	
	if (programHandle != 0) 
	{
		// Bind the vertex shader to the program.
		gl.attachShader(programHandle, vertexShader);			

		// Bind the fragment shader to the program.
		gl.attachShader(programHandle, fragmentShader);
		
		// Bind attributes
		gl.bindAttribLocation(programHandle, 0, "a_Position");
		gl.bindAttribLocation(programHandle, 1, "a_Color");
		
		// Link the two shaders together into a program.
		gl.linkProgram(programHandle);

		// Get the link status.
		var linked = gl.getProgramParameter(programHandle, gl.LINK_STATUS);

		// If the link failed, delete the program.
		if (!linked) 
		{				
			gl.deleteProgram(programHandle);
			programHandle = 0;
		}
	}
	
	if (programHandle == 0)
	{
		throw("Error creating program.");
	}
	
	return programHandle;
}

// Called when we have the context
function startRendering()
{
	/* Configure viewport */
	
	// Set the OpenGL viewport to the same size as the canvas.
	gl.viewport(0, 0, canvas.clientWidth, canvas.clientHeight);
	
	// Create a new perspective projection matrix. The height will stay the same
	// while the width will vary as per aspect ratio.
	var ratio = canvas.clientWidth / canvas.clientHeight;
	var left = -ratio;
	var right = ratio;
	var bottom = -1.0;
	var top = 1.0;
	var near = 1.0;
	var far = 10.0;
		
	mat4.frustum(left, right, bottom, top, near, far, projectionMatrix);
	
	/* Configure general parameters */
	
	// Set the background clear color to gray.
	gl.clearColor(0.5, 0.5, 0.5, 1.0);		
	
	/* Configure camera */
	// Position the eye behind the origin.
	var eyeX = 0.0;
	var eyeY = 0.0;
	var eyeZ = 1.5;

	// We are looking toward the distance
	var lookX = 0.0;
	var lookY = 0.0;
	var lookZ = -5.0;

	// Set our up vector. This is where our head would be pointing were we holding the camera.
	var upX = 0.0;
	var upY = 1.0;
	var upZ = 0.0;
	
	// Set the view matrix. This matrix can be said to represent the camera position.		
	var eye = vec3.create();
	eye[0] = eyeX; eye[1] = eyeY; eye[2] = eyeZ;
	
	var center = vec3.create();
	center[0] = lookX; center[1] = lookY; center[2] = lookZ;
	
	var up = vec3.create();
	up[0] = upX; up[1] = upY; up[2] = upZ;
	
	mat4.lookAt(eye, center, up, viewMatrix);
	
	/* Configure shaders */
	
	var vertexShaderHandle = loadShader("vertex_shader", gl.VERTEX_SHADER);
	var fragmentShaderHandle = loadShader("fragment_shader", gl.FRAGMENT_SHADER);			
	
	// Create a program object and store the handle to it.
	var programHandle = linkProgram(vertexShaderHandle, fragmentShaderHandle);	
    
    // Set program handles. These will later be used to pass in values to the program.
	mvpMatrixHandle = gl.getUniformLocation(programHandle, "u_MVPMatrix");        
    positionHandle = gl.getAttribLocation(programHandle, "a_Position");
    colorHandle = gl.getAttribLocation(programHandle, "a_Color");        
    
    // Tell OpenGL to use this program when rendering.
    gl.useProgram(programHandle);
    
    // Create buffers in OpenGL's working memory.
    trianglePositionBufferObject = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, trianglePositionBufferObject);
    gl.bufferData(gl.ARRAY_BUFFER, trianglePositions, gl.STATIC_DRAW);
    
    triangleColorBufferObject1 = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleColorBufferObject1);
    gl.bufferData(gl.ARRAY_BUFFER, triangle1Colors, gl.STATIC_DRAW);
    
    triangleColorBufferObject2 = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleColorBufferObject2);
    gl.bufferData(gl.ARRAY_BUFFER, triangle2Colors, gl.STATIC_DRAW);
    
    triangleColorBufferObject3 = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleColorBufferObject3);
    gl.bufferData(gl.ARRAY_BUFFER, triangle3Colors, gl.STATIC_DRAW); 
	
	// Tell the browser we want render() to be called whenever it's time to draw another frame.
	window.requestAnimFrame(render, canvas);
}

// Callback called each time the browser wants us to draw another frame
function render(time)
{           	
	// Clear the canvas
	gl.clear(gl.COLOR_BUFFER_BIT);
	
	// Do a complete rotation every 10 seconds.
    var time = Date.now() % 10000;
    var angleInDegrees = (360.0 / 10000.0) * time;
    var angleInRadians = angleInDegrees / 57.2957795;
    
    var xyz = vec3.create();
    
    // Draw the triangle facing straight on.
    mat4.identity(modelMatrix);
    mat4.rotateZ(modelMatrix, angleInRadians);           
    drawTriangle(triangleColorBufferObject1);
    
    // Draw one translated a bit down and rotated to be flat on the ground.
    mat4.identity(modelMatrix);
    xyz[0] = 0; xyz[1] = -1; xyz[2] = 0;
    mat4.translate(modelMatrix, xyz);
    mat4.rotateX(modelMatrix, 90 / 57.2957795);
    xyz[0] = 0; xyz[1] = 0; xyz[2] = 1;
    mat4.rotate(modelMatrix, angleInRadians, xyz);           
    drawTriangle(triangleColorBufferObject2);
    
    // Draw one translated a bit to the right and rotated to be facing to the left.
    mat4.identity(modelMatrix);
    xyz[0] = 1; xyz[1] = 0; xyz[2] = 0;
    mat4.translate(modelMatrix, xyz);
    mat4.rotateY(modelMatrix, 90 / 57.2957795);
    xyz[0] = 0; xyz[1] = 0; xyz[2] = 1;
    mat4.rotate(modelMatrix, angleInRadians, xyz);     
    drawTriangle(triangleColorBufferObject3);
    
    // Send the commands to WebGL
	gl.flush();
	
	// Request another frame
	window.requestAnimFrame(render, canvas);
}

function checkError()
{
	var error = gl.getError();
	
	if (error)
	{
		throw("error: " + error);
	}
}

// Draws a triangle from the given vertex data.
function drawTriangle(triangleColorBufferObject)
{		
	// Pass in the position information
	gl.enableVertexAttribArray(positionHandle);   
	gl.bindBuffer(gl.ARRAY_BUFFER, trianglePositionBufferObject);
    gl.vertexAttribPointer(positionHandle, positionDataSize, gl.FLOAT, false,
    		0, 0);        
                
    // Pass in the color information
    gl.enableVertexAttribArray(colorHandle);   
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleColorBufferObject);
    gl.vertexAttribPointer(colorHandle, colorDataSize, gl.FLOAT, false,
    		0, 0);              
    
	// This multiplies the view matrix by the model matrix, and stores the result in the modelview matrix
    // (which currently contains model * view).    
    mat4.multiply(viewMatrix, modelMatrix, mvpMatrix);
    
    // This multiplies the modelview matrix by the projection matrix, and stores the result in the MVP matrix
    // (which now contains model * view * projection).
    mat4.multiply(projectionMatrix, mvpMatrix, mvpMatrix);
    
    gl.uniformMatrix4fv(mvpMatrixHandle, false, mvpMatrix);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
}

// Main entry point
function main()
{		
    // Try to get a WebGL context    
    canvas = document.getElementById("canvas");    
    
    // We don't need a depth buffer. 
    // See https://www.khronos.org/registry/webgl/specs/1.0/ Section 5.2 for more info on parameters and defaults.
    gl = WebGLUtils.setupWebGL(canvas, { depth: false });
            
    if (gl != null)
	{    	
    	// Init model data.
    	
    	// Define points for equilateral triangles.		
		trianglePositions = new Float32Array([
				// X, Y, Z, 
	            -0.5, -0.25, 0.0, 	            	            
	            0.5, -0.25, 0.0,	            	            
	            0.0, 0.559016994, 0.0]);
		
		// This triangle is red, green, and blue.
		triangle1Colors = new Float32Array([
  				// R, G, B, A  	            
  	            1.0, 0.0, 0.0, 1.0,  	              	            
  	            0.0, 0.0, 1.0, 1.0,  	              	            
  	            0.0, 1.0, 0.0, 1.0]);				
		
		// This triangle is yellow, cyan, and magenta.
		triangle2Colors = new Float32Array([ 
				// R, G, B, A	            
	            1.0, 1.0, 0.0, 1.0,	            	            
	            0.0, 1.0, 1.0, 1.0,	            	            
	            1.0, 0.0, 1.0, 1.0]);
		
		// This triangle is white, gray, and black.
		triangle3Colors = new Float32Array([ 
				// R, G, B, A	            
	            1.0, 1.0, 1.0, 1.0,	            	            
	            0.5, 0.5, 0.5, 1.0,	            	            
	            0.0, 0.0, 0.0, 1.0]);		
		
    	startRendering();
	}
}

// Execute the main entry point
main();