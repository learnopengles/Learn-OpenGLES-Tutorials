/**
 * Lesson_two.js
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

/** 
 * Stores a copy of the model matrix specifically for the light position.
 */
var lightModelMatrix = mat4.create();

/** Store our model data in a Float32Array buffer. */
var cubePositions;
var cubeColors;
var cubeNormals;

/** Store references to the vertex buffer objects (VBOs) that will be created. */
var cubePositionBufferObject;
var cubeColorBufferObject;
var cubeNormalBufferObject;

/** This will be used to pass in the transformation matrix. */
var mvpMatrixHandle;

/** This will be used to pass in the modelview matrix. */
var mvMatrixHandle;

/** This will be used to pass in the light position. */
var lightPosHandle;

/** This will be used to pass in model position information. */
var positionHandle;

/** This will be used to pass in model color information. */
var colorHandle;

/** This will be used to pass in model normal information. */
var normalHandle;

/** Size of the position data in elements. */
var positionDataSize = 3;

/** Size of the color data in elements. */
var colorDataSize = 4;	

/** Size of the normal data in elements. */
var normalDataSize = 3;

/** Used to hold a light centered on the origin in model space. We need a 4th coordinate so we can get translations to work when
 *  we multiply this by our transformation matrices. */
var lightPosInModelSpace = new Array(0, 0, 0, 1);

/** Used to hold the current position of the light in world space (after transformation via model matrix). */
var lightPosInWorldSpace = new Array(0, 0, 0, 0);

/** Used to hold the transformed position of the light in eye space (after transformation via modelview matrix) */
var lightPosInEyeSpace = new Array(0, 0, 0, 0);

/** This is a handle to our per-vertex cube shading program. */
var perVertexProgramHandle;
	
/** This is a handle to our light point program. */
var pointProgramHandle;

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
function linkProgram(vertexShader, fragmentShader, attributes)
{
	// Create a program object and store the handle to it.
	var programHandle = gl.createProgram();
	var error;
	
	if (programHandle != 0) 
	{
		// Bind the vertex shader to the program.
		gl.attachShader(programHandle, vertexShader);			

		// Bind the fragment shader to the program.
		gl.attachShader(programHandle, fragmentShader);
		
		// Bind attributes
		if (attributes)
		{
			for (i = 0; i < attributes.length; i++)
			{
				gl.bindAttribLocation(programHandle, i, attributes[i]);
			}
		}
						
		// Link the two shaders together into a program.
		gl.linkProgram(programHandle);

		// Get the link status.
		var linked = gl.getProgramParameter(programHandle, gl.LINK_STATUS);

		// If the link failed, delete the program.
		if (!linked) 
		{	
			error = gl.getProgramInfoLog(programHandle);			
			gl.deleteProgram(programHandle);
			programHandle = 0;
		}
	}
	
	if (programHandle == 0)
	{
		throw("Error creating program:" + error);
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
	
	// Set the background clear color to black.
	gl.clearColor(0.0, 0.0, 0.0, 0.0);	
			
	// Use culling to remove back faces.
	gl.enable(gl.CULL_FACE);
	
	// Enable depth testing
	gl.enable(gl.DEPTH_TEST);
	
	// Enable dithering
	gl.enable(gl.DITHER);
	
	/* Configure camera */
	// Position the eye behind the origin.
	var eyeX = 0.0;
	var eyeY = 0.0;
	var eyeZ = -0.5;

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
	perVertexProgramHandle = linkProgram(vertexShaderHandle, fragmentShaderHandle, new Array("a_Position",  "a_Color", "a_Normal"));
	
    // Define a simple shader program for our point.
	var pointVertexShaderHandle = loadShader("point_vertex_shader", gl.VERTEX_SHADER);
	var pointFragmentShaderHandle = loadShader("point_fragment_shader", gl.FRAGMENT_SHADER);
	pointProgramHandle = linkProgram(pointVertexShaderHandle, pointFragmentShaderHandle, new Array("a_Position"));     	
    
    // Create buffers in OpenGL's working memory.				
	cubePositionBufferObject = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, cubePositionBufferObject);
    gl.bufferData(gl.ARRAY_BUFFER, cubePositions, gl.STATIC_DRAW);
    
    cubeColorBufferObject = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeColorBufferObject);
    gl.bufferData(gl.ARRAY_BUFFER, cubeColors, gl.STATIC_DRAW);
    
    cubeNormalBufferObject = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeNormalBufferObject);
    gl.bufferData(gl.ARRAY_BUFFER, cubeNormals, gl.STATIC_DRAW);        
	
	// Tell the browser we want render() to be called whenever it's time to draw another frame.
	window.requestAnimFrame(render, canvas);
}

// Callback called each time the browser wants us to draw another frame
function render(time)
{           	
	// Clear the canvas
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	// Do a complete rotation every 10 seconds.
    var time = Date.now() % 10000;
    var angleInDegrees = (360.0 / 10000.0) * time;
    var angleInRadians = angleInDegrees / 57.2957795;
    
    // Set our per-vertex lighting program.
    gl.useProgram(perVertexProgramHandle);    
    
    // Set program handles for cube drawing.
    mvpMatrixHandle = gl.getUniformLocation(perVertexProgramHandle, "u_MVPMatrix");
    mvMatrixHandle = gl.getUniformLocation(perVertexProgramHandle, "u_MVMatrix"); 
    lightPosHandle = gl.getUniformLocation(perVertexProgramHandle, "u_LightPos");
    positionHandle = gl.getAttribLocation(perVertexProgramHandle, "a_Position");
    colorHandle = gl.getAttribLocation(perVertexProgramHandle, "a_Color");
    normalHandle = gl.getAttribLocation(perVertexProgramHandle, "a_Normal"); 
        
    var v = vec3.create();
    
    // Calculate position of the light. Rotate and then push into the distance.    
    mat4.identity(lightModelMatrix);
    v[0] = 0; v[1] = 0; v[2] = -5;
    mat4.translate(lightModelMatrix, v);
    mat4.rotateY(lightModelMatrix, angleInRadians);
    v[0] = 0; v[1] = 0; v[2] = 2;
    mat4.translate(lightModelMatrix, v);
    
    mat4.multiplyVec4(lightModelMatrix, lightPosInModelSpace, lightPosInWorldSpace);
    mat4.multiplyVec4(viewMatrix, lightPosInWorldSpace, lightPosInEyeSpace);                                             
    
    // Draw some cubes.              
    mat4.identity(modelMatrix);
    v[0] = 4; v[1] = 0; v[2] = -7;
    mat4.translate(modelMatrix, v);
    mat4.rotateX(modelMatrix, angleInRadians);            
    drawCube();
                    
    mat4.identity(modelMatrix);
    v[0] = -4; v[1] = 0; v[2] = -7;
    mat4.translate(modelMatrix, v);
    mat4.rotateY(modelMatrix, angleInRadians);           
    drawCube();
    
    mat4.identity(modelMatrix);
    v[0] = 0; v[1] = 4; v[2] = -7;
    mat4.translate(modelMatrix, v);    
    mat4.rotateZ(modelMatrix, angleInRadians);           
    drawCube();
    
    mat4.identity(modelMatrix);
    v[0] = 0; v[1] = -4; v[2] = -7;
    mat4.translate(modelMatrix, v);    
    drawCube();
    
    mat4.identity(modelMatrix);
    v[0] = 0; v[1] = 0; v[2] = -5;
    mat4.translate(modelMatrix, v);        
    v[0] = 1; v[1] = 1; v[2] = 0;
    mat4.rotate(modelMatrix, angleInRadians, v);
    drawCube();      
    
    // Draw a point to indicate the light.
    gl.useProgram(pointProgramHandle);        
    drawLight();        
    
    // Send the commands to WebGL
	gl.flush();
	
	// Request another frame
	window.requestAnimFrame(render, canvas);
}

/**
 * Draws a cube.
 */			
function drawCube()
{		
	// Pass in the position information
	gl.enableVertexAttribArray(positionHandle);   
	gl.bindBuffer(gl.ARRAY_BUFFER, cubePositionBufferObject);
    gl.vertexAttribPointer(positionHandle, positionDataSize, gl.FLOAT, false, 0, 0);  
    
    // Pass in the color information
    gl.enableVertexAttribArray(colorHandle);   
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeColorBufferObject);
    gl.vertexAttribPointer(colorHandle, colorDataSize, gl.FLOAT, false, 0, 0); 
    
    // Pass in the normal information
    gl.enableVertexAttribArray(normalHandle);   
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeNormalBufferObject);
    gl.vertexAttribPointer(normalHandle, normalDataSize, gl.FLOAT, false, 0, 0);
     
	// This multiplies the view matrix by the model matrix, and stores the result in the MVP matrix
    // (which currently contains model * view).
    mat4.multiply(viewMatrix, modelMatrix, mvpMatrix);      
    
    // Pass in the modelview matrix.
    gl.uniformMatrix4fv(mvMatrixHandle, false, mvpMatrix);                
    
    // This multiplies the modelview matrix by the projection matrix, and stores the result in the MVP matrix
    // (which now contains model * view * projection).
    mat4.multiply(projectionMatrix, mvpMatrix, mvpMatrix);    

    // Pass in the combined matrix.
    gl.uniformMatrix4fv(mvpMatrixHandle, false, mvpMatrix);
    
    // Pass in the light position in eye space.        
    gl.uniform3f(lightPosHandle, lightPosInEyeSpace[0], lightPosInEyeSpace[1], lightPosInEyeSpace[2]);
    
    // Draw the cube.
    gl.drawArrays(gl.TRIANGLES, 0, 36);    
}

/**
 * Draws a point representing the position of the light.
 */
function drawLight()
{
	var pointMVPMatrixHandle = gl.getUniformLocation(pointProgramHandle, "u_MVPMatrix");
    var pointPositionHandle = gl.getAttribLocation(pointProgramHandle, "a_Position");
    
	// Pass in the position.
	gl.vertexAttrib3f(pointPositionHandle, lightPosInModelSpace[0], lightPosInModelSpace[1], lightPosInModelSpace[2]);

	// Since we are not using a buffer object, disable vertex arrays for this attribute.
    gl.disableVertexAttribArray(pointPositionHandle);  
	
	// Pass in the transformation matrix.
    mat4.multiply(viewMatrix, lightModelMatrix, mvpMatrix);
    mat4.multiply(projectionMatrix, mvpMatrix, mvpMatrix);    	
	gl.uniformMatrix4fv(pointMVPMatrixHandle, false, mvpMatrix);
	
	// Draw the point.
	gl.drawArrays(gl.POINTS, 0, 1);
}

// Main entry point
function main()
{		
    // Try to get a WebGL context    
    canvas = document.getElementById("canvas");    
    
    // We don't need alpha blending.
    // See https://www.khronos.org/registry/webgl/specs/1.0/ Section 5.2 for more info on parameters and defaults.
    gl = WebGLUtils.setupWebGL(canvas, {alpha: false});
            
    if (gl != null)
	{    	
    	// // Define points for a cube.		
		
		// X, Y, Z
		cubePositions = new Float32Array
		([
				// In OpenGL counter-clockwise winding is default. This means that when we look at a triangle, 
				// if the points are counter-clockwise we are looking at the "front". If not we are looking at
				// the back. OpenGL has an optimization where all back-facing triangles are culled, since they
				// usually represent the backside of an object and aren't visible anyways.
				
				// Front face
				-1.0, 1.0, 1.0,				
				-1.0, -1.0, 1.0,
				1.0, 1.0, 1.0, 
				-1.0, -1.0, 1.0, 				
				1.0, -1.0, 1.0,
				1.0, 1.0, 1.0,
				
				// Right face
				1.0, 1.0, 1.0,				
				1.0, -1.0, 1.0,
				1.0, 1.0, -1.0,
				1.0, -1.0, 1.0,				
				1.0, -1.0, -1.0,
				1.0, 1.0, -1.0,
				
				// Back face
				1.0, 1.0, -1.0,				
				1.0, -1.0, -1.0,
				-1.0, 1.0, -1.0,
				1.0, -1.0, -1.0,				
				-1.0, -1.0, -1.0,
				-1.0, 1.0, -1.0,
				
				// Left face
				-1.0, 1.0, -1.0,				
				-1.0, -1.0, -1.0,
				-1.0, 1.0, 1.0, 
				-1.0, -1.0, -1.0,				
				-1.0, -1.0, 1.0, 
				-1.0, 1.0, 1.0, 
				
				// Top face
				-1.0, 1.0, -1.0,				
				-1.0, 1.0, 1.0, 
				1.0, 1.0, -1.0, 
				-1.0, 1.0, 1.0, 				
				1.0, 1.0, 1.0, 
				1.0, 1.0, -1.0,
				
				// Bottom face
				1.0, -1.0, -1.0,				
				1.0, -1.0, 1.0, 
				-1.0, -1.0, -1.0,
				1.0, -1.0, 1.0, 				
				-1.0, -1.0, 1.0,
				-1.0, -1.0, -1.0
		]);	
		
		// R, G, B, A
		cubeColors = new Float32Array
		([				
				// Front face (red)
				1.0, 0.0, 0.0, 1.0,				
				1.0, 0.0, 0.0, 1.0,
				1.0, 0.0, 0.0, 1.0,
				1.0, 0.0, 0.0, 1.0,				
				1.0, 0.0, 0.0, 1.0,
				1.0, 0.0, 0.0, 1.0,
				
				// Right face (green)
				0.0, 1.0, 0.0, 1.0,				
				0.0, 1.0, 0.0, 1.0,
				0.0, 1.0, 0.0, 1.0,
				0.0, 1.0, 0.0, 1.0,				
				0.0, 1.0, 0.0, 1.0,
				0.0, 1.0, 0.0, 1.0,
				
				// Back face (blue)
				0.0, 0.0, 1.0, 1.0,				
				0.0, 0.0, 1.0, 1.0,
				0.0, 0.0, 1.0, 1.0,
				0.0, 0.0, 1.0, 1.0,				
				0.0, 0.0, 1.0, 1.0,
				0.0, 0.0, 1.0, 1.0,
				
				// Left face (yellow)
				1.0, 1.0, 0.0, 1.0,				
				1.0, 1.0, 0.0, 1.0,
				1.0, 1.0, 0.0, 1.0,
				1.0, 1.0, 0.0, 1.0,				
				1.0, 1.0, 0.0, 1.0,
				1.0, 1.0, 0.0, 1.0,
				
				// Top face (cyan)
				0.0, 1.0, 1.0, 1.0,				
				0.0, 1.0, 1.0, 1.0,
				0.0, 1.0, 1.0, 1.0,
				0.0, 1.0, 1.0, 1.0,				
				0.0, 1.0, 1.0, 1.0,
				0.0, 1.0, 1.0, 1.0,
				
				// Bottom face (magenta)
				1.0, 0.0, 1.0, 1.0,				
				1.0, 0.0, 1.0, 1.0,
				1.0, 0.0, 1.0, 1.0,
				1.0, 0.0, 1.0, 1.0,				
				1.0, 0.0, 1.0, 1.0,
				1.0, 0.0, 1.0, 1.0
		]);
		
		// X, Y, Z
		// The normal is used in light calculations and is a vector which points
		// orthogonal to the plane of the surface. For a cube model, the normals
		// should be orthogonal to the points of each face.
		cubeNormals = new Float32Array
		([												
				// Front face
				0.0, 0.0, 1.0,				
				0.0, 0.0, 1.0,
				0.0, 0.0, 1.0,
				0.0, 0.0, 1.0,				
				0.0, 0.0, 1.0,
				0.0, 0.0, 1.0,
				
				// Right face 
				1.0, 0.0, 0.0,				
				1.0, 0.0, 0.0,
				1.0, 0.0, 0.0,
				1.0, 0.0, 0.0,				
				1.0, 0.0, 0.0,
				1.0, 0.0, 0.0,
				
				// Back face 
				0.0, 0.0, -1.0,				
				0.0, 0.0, -1.0,
				0.0, 0.0, -1.0,
				0.0, 0.0, -1.0,				
				0.0, 0.0, -1.0,
				0.0, 0.0, -1.0,
				
				// Left face 
				-1.0, 0.0, 0.0,				
				-1.0, 0.0, 0.0,
				-1.0, 0.0, 0.0,
				-1.0, 0.0, 0.0,				
				-1.0, 0.0, 0.0,
				-1.0, 0.0, 0.0,
				
				// Top face 
				0.0, 1.0, 0.0,			
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,				
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
				
				// Bottom face 
				0.0, -1.0, 0.0,			
				0.0, -1.0, 0.0,
				0.0, -1.0, 0.0,
				0.0, -1.0, 0.0,				
				0.0, -1.0, 0.0,
				0.0, -1.0, 0.0
		]);    	    	
		
    	startRendering();
	}
}

// Execute the main entry point
main();