package com.learnopengles.android.lesson8;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

import com.badlogic.gdx.backends.android.AndroidGL20;
import com.learnopengles.android.R;
import com.learnopengles.android.common.RawResourceReader;
import com.learnopengles.android.common.ShaderHelper;
import com.learnopengles.android.lesson8.ErrorHandler.ErrorType;

/**
 * This class implements our custom renderer. Note that the GL10 parameter
 * passed in is unused for OpenGL ES 2.0 renderers -- the static class GLES20 is
 * used instead.
 */
public class LessonEightRenderer implements GLSurfaceView.Renderer {
	/** Used for debug logs. */
	private static final String TAG = "LessonEightRenderer";

	/** References to other main objects. */
	private final LessonEightActivity lessonEightActivity;
	private final ErrorHandler errorHandler;

	/**
	 * Android's OpenGL bindings are broken until Gingerbread, so we use LibGDX
	 * bindings here.
	 */
	private final AndroidGL20 glEs20;

	/**
	 * Store the model matrix. This matrix is used to move models from object
	 * space (where each model can be thought of being located at the center of
	 * the universe) to world space.
	 */
	private final float[] modelMatrix = new float[16];

	/**
	 * Store the view matrix. This can be thought of as our camera. This matrix
	 * transforms world space to eye space; it positions things relative to our
	 * eye.
	 */
	private final float[] viewMatrix = new float[16];

	/**
	 * Store the projection matrix. This is used to project the scene onto a 2D
	 * viewport.
	 */
	private final float[] projectionMatrix = new float[16];

	/**
	 * Allocate storage for the final combined matrix. This will be passed into
	 * the shader program.
	 */
	private final float[] mvpMatrix = new float[16];

	/** Additional matrices. */
	private final float[] accumulatedRotation = new float[16];
	private final float[] currentRotation = new float[16];
	private final float[] lightModelMatrix = new float[16];
	private final float[] temporaryMatrix = new float[16];

	/** OpenGL handles to our program uniforms. */
	private int mvpMatrixUniform;
	private int mvMatrixUniform;
	private int lightPosUniform;

	/** OpenGL handles to our program attributes. */
	private int positionAttribute;
	private int normalAttribute;
	private int colorAttribute;

	/** Identifiers for our uniforms and attributes inside the shaders. */
	private static final String MVP_MATRIX_UNIFORM = "u_MVPMatrix";
	private static final String MV_MATRIX_UNIFORM = "u_MVMatrix";
	private static final String LIGHT_POSITION_UNIFORM = "u_LightPos";

	private static final String POSITION_ATTRIBUTE = "a_Position";
	private static final String NORMAL_ATTRIBUTE = "a_Normal";
	private static final String COLOR_ATTRIBUTE = "a_Color";

	/** Additional constants. */
	private static final int POSITION_DATA_SIZE_IN_ELEMENTS = 3;
	private static final int NORMAL_DATA_SIZE_IN_ELEMENTS = 3;
	private static final int COLOR_DATA_SIZE_IN_ELEMENTS = 4;

	private static final int BYTES_PER_FLOAT = 4;
	private static final int BYTES_PER_SHORT = 2;

	private static final int STRIDE = (POSITION_DATA_SIZE_IN_ELEMENTS + NORMAL_DATA_SIZE_IN_ELEMENTS + COLOR_DATA_SIZE_IN_ELEMENTS)
			* BYTES_PER_FLOAT;

	/**
	 * Used to hold a light centered on the origin in model space. We need a 4th
	 * coordinate so we can get translations to work when we multiply this by
	 * our transformation matrices.
	 */
	private final float[] lightPosInModelSpace = new float[] { 0.0f, 0.0f, 0.0f, 1.0f };

	/**
	 * Used to hold the current position of the light in world space (after
	 * transformation via model matrix).
	 */
	private final float[] lightPosInWorldSpace = new float[4];

	/**
	 * Used to hold the transformed position of the light in eye space (after
	 * transformation via modelview matrix)
	 */
	private final float[] lightPosInEyeSpace = new float[4];

	/** This is a handle to our cube shading program. */
	private int program;

	/** Retain the most recent delta for touch events. */
	// These still work without volatile, but refreshes are not guaranteed to
	// happen.
	public volatile float deltaX;
	public volatile float deltaY;

	/** The current heightmap object. */
	private HeightMap heightMap;

	/**
	 * Initialize the model data.
	 */
	public LessonEightRenderer(final LessonEightActivity lessonEightActivity, ErrorHandler errorHandler) {
		this.lessonEightActivity = lessonEightActivity;
		this.errorHandler = errorHandler;
		glEs20 = new AndroidGL20();
	}

	@Override
	public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
		heightMap = new HeightMap();

		// Set the background clear color to black.
		GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Enable depth testing
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);

		// Position the eye in front of the origin.
		final float eyeX = 0.0f;
		final float eyeY = 0.0f;
		final float eyeZ = -0.5f;

		// We are looking toward the distance
		final float lookX = 0.0f;
		final float lookY = 0.0f;
		final float lookZ = -5.0f;

		// Set our up vector. This is where our head would be pointing were we
		// holding the camera.
		final float upX = 0.0f;
		final float upY = 1.0f;
		final float upZ = 0.0f;

		// Set the view matrix. This matrix can be said to represent the camera
		// position.
		// NOTE: In OpenGL 1, a ModelView matrix is used, which is a combination
		// of a model and view matrix. In OpenGL 2, we can keep track of these
		// matrices separately if we choose.
		Matrix.setLookAtM(viewMatrix, 0, eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

		final String vertexShader = RawResourceReader.readTextFileFromRawResource(lessonEightActivity,
				R.raw.per_pixel_vertex_shader_no_tex);
		final String fragmentShader = RawResourceReader.readTextFileFromRawResource(lessonEightActivity,
				R.raw.per_pixel_fragment_shader_no_tex);

		final int vertexShaderHandle = ShaderHelper.compileShader(GLES20.GL_VERTEX_SHADER, vertexShader);
		final int fragmentShaderHandle = ShaderHelper.compileShader(GLES20.GL_FRAGMENT_SHADER, fragmentShader);

		program = ShaderHelper.createAndLinkProgram(vertexShaderHandle, fragmentShaderHandle, new String[] {
				POSITION_ATTRIBUTE, NORMAL_ATTRIBUTE, COLOR_ATTRIBUTE });

		// Initialize the accumulated rotation matrix
		Matrix.setIdentityM(accumulatedRotation, 0);
	}

	@Override
	public void onSurfaceChanged(GL10 glUnused, int width, int height) {
		// Set the OpenGL viewport to the same size as the surface.
		GLES20.glViewport(0, 0, width, height);

		// Create a new perspective projection matrix. The height will stay the
		// same while the width will vary as per aspect ratio.
		final float ratio = (float) width / height;
		final float left = -ratio;
		final float right = ratio;
		final float bottom = -1.0f;
		final float top = 1.0f;
		final float near = 1.0f;
		final float far = 1000.0f;

		Matrix.frustumM(projectionMatrix, 0, left, right, bottom, top, near, far);
	}

	@Override
	public void onDrawFrame(GL10 glUnused) {
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

		// Set our per-vertex lighting program.
		GLES20.glUseProgram(program);

		// Set program handles for cube drawing.
		mvpMatrixUniform = GLES20.glGetUniformLocation(program, MVP_MATRIX_UNIFORM);
		mvMatrixUniform = GLES20.glGetUniformLocation(program, MV_MATRIX_UNIFORM);
		lightPosUniform = GLES20.glGetUniformLocation(program, LIGHT_POSITION_UNIFORM);
		positionAttribute = GLES20.glGetAttribLocation(program, POSITION_ATTRIBUTE);
		normalAttribute = GLES20.glGetAttribLocation(program, NORMAL_ATTRIBUTE);
		colorAttribute = GLES20.glGetAttribLocation(program, COLOR_ATTRIBUTE);

		// Calculate position of the light. Push into the distance.
		Matrix.setIdentityM(lightModelMatrix, 0);
		Matrix.translateM(lightModelMatrix, 0, 0.0f, 7.5f, -8.0f);

		Matrix.multiplyMV(lightPosInWorldSpace, 0, lightModelMatrix, 0, lightPosInModelSpace, 0);
		Matrix.multiplyMV(lightPosInEyeSpace, 0, viewMatrix, 0, lightPosInWorldSpace, 0);

		// Draw the heightmap.
		// Translate the heightmap into the screen.
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -12f);

		// Set a matrix that contains the current rotation.
		Matrix.setIdentityM(currentRotation, 0);
		Matrix.rotateM(currentRotation, 0, deltaX, 0.0f, 1.0f, 0.0f);
		Matrix.rotateM(currentRotation, 0, deltaY, 1.0f, 0.0f, 0.0f);
		deltaX = 0.0f;
		deltaY = 0.0f;

		// Multiply the current rotation by the accumulated rotation, and then
		// set the accumulated rotation to the result.
		Matrix.multiplyMM(temporaryMatrix, 0, currentRotation, 0, accumulatedRotation, 0);
		System.arraycopy(temporaryMatrix, 0, accumulatedRotation, 0, 16);

		// Rotate the cube taking the overall rotation into account.
		Matrix.multiplyMM(temporaryMatrix, 0, modelMatrix, 0, accumulatedRotation, 0);
		System.arraycopy(temporaryMatrix, 0, modelMatrix, 0, 16);

		// This multiplies the view matrix by the model matrix, and stores
		// the result in the MVP matrix
		// (which currently contains model * view).
		Matrix.multiplyMM(mvpMatrix, 0, viewMatrix, 0, modelMatrix, 0);

		// Pass in the modelview matrix.
		GLES20.glUniformMatrix4fv(mvMatrixUniform, 1, false, mvpMatrix, 0);

		// This multiplies the modelview matrix by the projection matrix,
		// and stores the result in the MVP matrix
		// (which now contains model * view * projection).
		Matrix.multiplyMM(temporaryMatrix, 0, projectionMatrix, 0, mvpMatrix, 0);
		System.arraycopy(temporaryMatrix, 0, mvpMatrix, 0, 16);

		// Pass in the combined matrix.
		GLES20.glUniformMatrix4fv(mvpMatrixUniform, 1, false, mvpMatrix, 0);

		// Pass in the light position in eye space.
		GLES20.glUniform3f(lightPosUniform, lightPosInEyeSpace[0], lightPosInEyeSpace[1], lightPosInEyeSpace[2]);

		// Render the heightmap.
		heightMap.render();
	}

	class HeightMap {
		static final int SIZE_PER_SIDE = 32;
		static final float MIN_POSITION = -5f;
		static final float POSITION_RANGE = 10f;

		final int[] vbo = new int[1];
		final int[] ibo = new int[1];

		int indexCount;

		HeightMap() {
			try {
				final int floatsPerVertex = POSITION_DATA_SIZE_IN_ELEMENTS + NORMAL_DATA_SIZE_IN_ELEMENTS
						+ COLOR_DATA_SIZE_IN_ELEMENTS;
				final int xLength = SIZE_PER_SIDE;
				final int yLength = SIZE_PER_SIDE;

				final float[] heightMapVertexData = new float[xLength * yLength * floatsPerVertex];

				int offset = 0;

				// First, build the data for the vertex buffer
				for (int y = 0; y < yLength; y++) {
					for (int x = 0; x < xLength; x++) {
						final float xRatio = x / (float) (xLength - 1);
						
						// Build our heightmap from the top down, so that our triangles are counter-clockwise.
						final float yRatio = 1f - (y / (float) (yLength - 1));
						
						final float xPosition = MIN_POSITION + (xRatio * POSITION_RANGE);
						final float yPosition = MIN_POSITION + (yRatio * POSITION_RANGE);

						// Position
						heightMapVertexData[offset++] = xPosition;
						heightMapVertexData[offset++] = yPosition;
						heightMapVertexData[offset++] = ((xPosition * xPosition) + (yPosition * yPosition)) / 10f;					

						// Cheap normal using a derivative of the function.
						// The slope for X will be 2X, for Y will be 2Y.
						// Divide by 10 since the position's Z is also divided by 10.
						final float xSlope = (2 * xPosition) / 10f;
						final float ySlope = (2 * yPosition) / 10f;
						
						// Calculate the normal using the cross product of the slopes.
						final float[] planeVectorX = {1f, 0f, xSlope};
						final float[] planeVectorY = {0f, 1f, ySlope};
						final float[] normalVector = {
								(planeVectorX[1] * planeVectorY[2]) - (planeVectorX[2] * planeVectorY[1]),
								(planeVectorX[2] * planeVectorY[0]) - (planeVectorX[0] * planeVectorY[2]),
								(planeVectorX[0] * planeVectorY[1]) - (planeVectorX[1] * planeVectorY[0])};
						
						// Normalize the normal						
						final float length = Matrix.length(normalVector[0], normalVector[1], normalVector[2]);

						heightMapVertexData[offset++] = normalVector[0] / length;
						heightMapVertexData[offset++] = normalVector[1] / length;
						heightMapVertexData[offset++] = normalVector[2] / length;

						// Add some fancy colors.
						heightMapVertexData[offset++] = xRatio;
						heightMapVertexData[offset++] = yRatio;
						heightMapVertexData[offset++] = 0.5f;
						heightMapVertexData[offset++] = 1f;
					}
				}

				// Now build the index data
				final int numStripsRequired = yLength - 1;
				final int numDegensRequired = 2 * (numStripsRequired - 1);
				final int verticesPerStrip = 2 * xLength;

				final short[] heightMapIndexData = new short[(verticesPerStrip * numStripsRequired) + numDegensRequired];

				offset = 0;

				for (int y = 0; y < yLength - 1; y++) {
					if (y > 0) {
						// Degenerate begin: repeat first vertex
						heightMapIndexData[offset++] = (short) (y * yLength);
					}

					for (int x = 0; x < xLength; x++) {
						// One part of the strip
						heightMapIndexData[offset++] = (short) ((y * yLength) + x);
						heightMapIndexData[offset++] = (short) (((y + 1) * yLength) + x);
					}

					if (y < yLength - 2) {
						// Degenerate end: repeat last vertex
						heightMapIndexData[offset++] = (short) (((y + 1) * yLength) + (xLength - 1));
					}
				}

				indexCount = heightMapIndexData.length;

				final FloatBuffer heightMapVertexDataBuffer = ByteBuffer
						.allocateDirect(heightMapVertexData.length * BYTES_PER_FLOAT).order(ByteOrder.nativeOrder())
						.asFloatBuffer();
				heightMapVertexDataBuffer.put(heightMapVertexData).position(0);

				final ShortBuffer heightMapIndexDataBuffer = ByteBuffer
						.allocateDirect(heightMapIndexData.length * BYTES_PER_SHORT).order(ByteOrder.nativeOrder())
						.asShortBuffer();
				heightMapIndexDataBuffer.put(heightMapIndexData).position(0);

				GLES20.glGenBuffers(1, vbo, 0);
				GLES20.glGenBuffers(1, ibo, 0);

				if (vbo[0] > 0 && ibo[0] > 0) {
					GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
					GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, heightMapVertexDataBuffer.capacity() * BYTES_PER_FLOAT,
							heightMapVertexDataBuffer, GLES20.GL_STATIC_DRAW);

					GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
					GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, heightMapIndexDataBuffer.capacity()
							* BYTES_PER_SHORT, heightMapIndexDataBuffer, GLES20.GL_STATIC_DRAW);

					GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
					GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
				} else {
					errorHandler.handleError(ErrorType.BUFFER_CREATION_ERROR, "glGenBuffers");
				}
			} catch (Throwable t) {
				Log.w(TAG, t);
				errorHandler.handleError(ErrorType.BUFFER_CREATION_ERROR, t.getLocalizedMessage());
			}
		}

		void render() {
			if (vbo[0] > 0 && ibo[0] > 0) {				
				GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);

				// Bind Attributes
				glEs20.glVertexAttribPointer(positionAttribute, POSITION_DATA_SIZE_IN_ELEMENTS, GLES20.GL_FLOAT, false,
						STRIDE, 0);
				GLES20.glEnableVertexAttribArray(positionAttribute);

				glEs20.glVertexAttribPointer(normalAttribute, NORMAL_DATA_SIZE_IN_ELEMENTS, GLES20.GL_FLOAT, false,
						STRIDE, POSITION_DATA_SIZE_IN_ELEMENTS * BYTES_PER_FLOAT);
				GLES20.glEnableVertexAttribArray(normalAttribute);

				glEs20.glVertexAttribPointer(colorAttribute, COLOR_DATA_SIZE_IN_ELEMENTS, GLES20.GL_FLOAT, false,
						STRIDE, (POSITION_DATA_SIZE_IN_ELEMENTS + NORMAL_DATA_SIZE_IN_ELEMENTS) * BYTES_PER_FLOAT);
				GLES20.glEnableVertexAttribArray(colorAttribute);

				// Draw
				GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
				glEs20.glDrawElements(GLES20.GL_TRIANGLE_STRIP, indexCount, GLES20.GL_UNSIGNED_SHORT, 0);

				GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
				GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}

		void release() {
			if (vbo[0] > 0) {
				GLES20.glDeleteBuffers(vbo.length, vbo, 0);
				vbo[0] = 0;
			}

			if (ibo[0] > 0) {
				GLES20.glDeleteBuffers(ibo.length, ibo, 0);
				ibo[0] = 0;
			}
		}
	}
}
