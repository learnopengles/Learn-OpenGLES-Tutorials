package com.learnopengles.android.lesson1

import android.opengl.GLES20
import android.opengl.GLES20.*
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import android.os.SystemClock
import glm_.f
import glm_.mat4x4.Mat4
import glm_.vec3.Vec3
import glm_.glm
import glm_.i
import glm_.rad
import glm_.vec4.Vec4
import uno.buffer.floatBufferOf
import uno.glf.semantic
import uno.gln.glClearColor
import uno.gln.glGetProgram
import uno.gln.glGetShader
import uno.gln.glUniform

import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import java.nio.FloatBuffer

/**
 * This class implements our custom renderer. Note that the GL10 parameter passed in is unused for OpenGL ES 2.0
 * renderers -- the static class GLES20 is used instead.
 */
class LessonOneRendererKt : GLSurfaceView.Renderer {
    /**
     * Store the model matrix. This matrix is used to move models from object space (where each model can be thought
     * of being located at the center of the universe) to world space.
     */
    private val modelMatrix = Mat4()

    /**
     * Store the view matrix. This can be thought of as our camera. This matrix transforms world space to eye space;
     * it positions things relative to our eye.
     */
    private val viewMatrix = Mat4()

    /** Store the projection matrix. This is used to project the scene onto a 2D viewport.  */
    private val projMatrix = Mat4()

    /** Allocate storage for the final combined matrix. This will be passed into the shader program.  */
    private val mvpMatrix = Mat4()

    /** Store our model data in a float buffer.  */
    private val triangle1Vertices: FloatBuffer
    private val triangle2Vertices: FloatBuffer
    private val triangle3Vertices: FloatBuffer

    /** How many elements per vertex.  */
    private val strideBytes = Vec3.size + Vec4.size

    /** Offset of the position data.  */
    private val positionOffset = 0

    /** Offset of the color data.  */
    private val colorOffset = Vec3.length

    /** the program */
    private lateinit var program: Program

    /**
     * Initialize the model data.
     */
    init {
        // Define points for equilateral triangles.

        // This triangle is red, green, and blue.
        triangle1Vertices = floatBufferOf(
                //X,    Y,     Z     R,  G,  B,  A
                -0.5f, -0.25f, 0.0f, 1f, 0f, 0f, 1f,
                +0.5f, -0.25f, 0.0f, 0f, 0f, 1f, 1f,
                +0.0f, +0.56f, 0.0f, 0f, 1f, 0f, 1f)

        // This triangle is yellow, cyan, and magenta.
        triangle2Vertices = floatBufferOf(
                //X,   Y,      Z     R,  G,  B,  A
                -0.5f, -0.25f, 0.0f, 1f, 1f, 0f, 1f,
                +0.5f, -0.25f, 0.0f, 0f, 1f, 1f, 1f,
                +0.0f, -0.56f, 0.0f, 1f, 0f, 1f, 1f)

        // This triangle is white, gray, and black.
        triangle3Vertices = floatBufferOf(
                //X,    Y,     Z     R,    G,    B,    A
                -0.5f, -0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                +0.5f, -0.25f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,
                +0.0f, +0.56f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)
    }

    override fun onSurfaceCreated(glUnused: GL10, config: EGLConfig) {

        // Set the background clear color to gray.
        glClearColor(0.5f)

        // Position the eye behind the origin.
        val eye = Vec3(0f, 0f, 1.5f)

        // We are looking toward the distance
        val look = Vec3(0f, 0f, -5f)

        // Set our up vector. This is where our head would be pointing were we holding the camera.
        val up = Vec3(0f, 1f, 0f)

        // Set the view matrix. This matrix can be said to represent the camera position.
        // NOTE: In OpenGL 1, a ModelView matrix is used, which is a combination of a model and
        // view matrix. In OpenGL 2, we can keep track of these matrices separately if we choose.
        viewMatrix put glm.lookAt(eye, look, up)

        val vertexShader = """

            uniform mat4 mvpMat;        // A constant representing the combined model/view/projection matrix.

            attribute vec4 inPosition;  // Per-vertex position information we will pass in.
            attribute vec4 inColor;     // Per-vertex color information we will pass in.

            varying vec4 color;         // This will be passed into the fragment shader.

            void main()                 // The entry point for our vertex shader.
            {
                color = inColor;        // Pass the color through to the fragment shader.

                /*  It will be interpolated across the triangle.
                    gl_Position is a special variable used to store the final position.
                    Multiply the vertex by the matrix to get the final point in normalized screen coordinates. */

                gl_Position = mvpMat * inPosition;
            }
        """

        val fragmentShader = """

            // Set the default precision to medium. We don't need as high of a precision in the fragment shader.

            precision mediump float;

            // This is the color from the vertex shader interpolated across the triangle per fragment.

            varying vec4 color;

            void main()                 // The entry point for our fragment shader.
            {
                gl_FragColor = color;   // Pass the color directly through the pipeline.
            }
        """

        program = Program(vertexShader, fragmentShader)

        // Tell OpenGL to use this program when rendering.
        glUseProgram(program.name)
    }

    class Program(vertexSrc: String, fragmentSrc: String) {

        val name: Int
        val mvpMatrix: Int

        init {

            fun loadShader(type: Int, src: String): Int {

                val shaderName = glCreateShader(type)

                if (shaderName != 0) {
                    // Pass in the shader source.
                    glShaderSource(shaderName, src)

                    // Compile the shader.
                    glCompileShader(shaderName)

                    // Check the compilation status. If the compilation failed, delete the shader.
                    if (glGetShader(shaderName, GL_COMPILE_STATUS) == GL_FALSE) {
                        glDeleteShader(shaderName)
                        val typeS = if (type == GL_VERTEX_SHADER) "vertex" else "fragment"
                        throw RuntimeException("Error creating $typeS shader.")
                    }
                }
                return shaderName
            }

            // Load in the vertex shader shader.
            val vertexName = loadShader(GL_VERTEX_SHADER, vertexSrc)
            // Load in the fragment shader shader.
            val fragmentName = loadShader(GL_FRAGMENT_SHADER, fragmentSrc)

            // Create a program object and store the handle to it.
            name = glCreateProgram()

            if (name != 0) {
                // Bind the vertex shader to the program.
                glAttachShader(name, vertexName)

                // Bind the fragment shader to the program.
                glAttachShader(name, fragmentName)

                // Bind attributes, must be done before linking the program
                glBindAttribLocation(name, semantic.attr.POSITION, "inPosition")
                glBindAttribLocation(name, semantic.attr.COLOR, "inColor")

                // Link the two shaders together into a program.
                glLinkProgram(name)

                // Check the link status. If the link failed, delete the program.
                if (glGetProgram(name, GL_LINK_STATUS) == GL_FALSE) {
                    glDeleteProgram(name)
                    throw RuntimeException("Error creating program.")
                }
            }

            // Set program handles. These will later be used to pass in values to the program.
            mvpMatrix = glGetUniformLocation(name, "mvpMat")
        }
    }

    override fun onSurfaceChanged(glUnused: GL10, width: Int, height: Int) {

        // Set the OpenGL viewport to the same size as the surface.
        uno.gln.glViewport(width, height)

        // Create a new perspective projection matrix. The height will stay the same
        // while the width will vary as per aspect ratio.
        val ratio = width.f / height
        val left = -ratio
        val right = ratio
        val bottom = -1f
        val top = 1f
        val near = 1f
        val far = 10f

        projMatrix put glm.frustum(left, right, bottom, top, near, far)
    }

    override fun onDrawFrame(glUnused: GL10) {

        glClear(GL_DEPTH_BUFFER_BIT or GL_COLOR_BUFFER_BIT)

        // Do a complete rotation every 10 seconds.
        val time = SystemClock.uptimeMillis() % 10_000L
        val angleDeg = 360f / 10_000f * time.i

        // Draw the triangle facing straight on.
        modelMatrix
                .put(1f)
                .rotate_(angleDeg.rad, 0.0f, 0.0f, 1.0f)
        drawTriangle(triangle1Vertices)

        // Draw one translated a bit down and rotated to be flat on the ground.
        modelMatrix
                .put(1f)
                .translate_(0.0f, -1.0f, 0.0f)
                .rotate_(90.0f, 1.0f, 0.0f, 0.0f)
                .rotate_(angleDeg.rad, 0.0f, 0.0f, 1.0f)
        drawTriangle(triangle2Vertices)

        // Draw one translated a bit to the right and rotated to be facing to the left.
        modelMatrix
                .put(1f)
                .translate_(1.0f, 0.0f, 0.0f)
                .rotate_(90.0f, 0.0f, 1.0f, 0.0f)
                .rotate_(angleDeg.rad, 0.0f, 0.0f, 1.0f)
        drawTriangle(triangle3Vertices)
    }

    /**
     * Draws a triangle from the given vertex data.
     *
     * @param triangleBuffer The buffer containing the vertex data.
     */
    private fun drawTriangle(triangleBuffer: FloatBuffer) {

        // Pass in the position information
        triangleBuffer.position(positionOffset)
        glVertexAttribPointer(semantic.attr.POSITION, Vec3.length, GL_FLOAT, false, strideBytes, triangleBuffer)

        glEnableVertexAttribArray(semantic.attr.POSITION)

        // Pass in the color information
        triangleBuffer.position(colorOffset)
        glVertexAttribPointer(semantic.attr.COLOR, Vec4.length, GL_FLOAT, false, strideBytes, triangleBuffer)

        glEnableVertexAttribArray(semantic.attr.COLOR)

        // This saves the projection matrix into the mvpMatrix
        mvpMatrix put projMatrix

        /*  This multiplies the mvpMatrix (that contains the projection matrix) by the view matrix, and stores the
            result in the mvpMatrix itself.
            mvpMatrix *= viewMatrix
         */
        mvpMatrix times_ viewMatrix

        /*  This multiplies the mvpMatrix (that contains the projection * view matrix) by the mdoel matrix, and stores
            the result in the mvpMatrix itself.
            mvpMatrix *= modelMatrix
         */
        mvpMatrix times_ modelMatrix

        glUniform(program.mvpMatrix, mvpMatrix)
        glDrawArrays(GL_TRIANGLES, 0, 3)
    }
}
