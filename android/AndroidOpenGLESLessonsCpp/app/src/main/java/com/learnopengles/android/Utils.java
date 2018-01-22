package com.learnopengles.android;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;

public class Utils {

    private static final String TAG = "Utils";

    /**
     * type:
     * create a vertex shader type {@link GLES20.GL_VERTEX_SHADER}
     * or a fragment shader type {@link GLES20.GL_FRAGMENT_SHADER}
     */
    @SuppressWarnings("JavadocReference")
    public static int compileShader(int type, String shaderCode) {

        // Load in the  shader
        int shader = GLES20.glCreateShader(type);

        if (shader != 0) {
            // Pass in the shader source
            GLES20.glShaderSource(shader, shaderCode);

            // Compile the shader
            GLES20.glCompileShader(shader);

            // Get the compilation status.
            final int[] compileStatus = new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compileStatus, 0);

            // If the compilation failed, delete the shader.
            if (compileStatus[0] == 0) {
                GLES20.glDeleteShader(shader);
                shader = 0;
            }
        }

        if (shader == 0) {
            GLES20.glGetShaderInfoLog(shader);
            throw new RuntimeException("Error creating vertex shader.");
        }

        return shader;
    }

    /**
     * Helper function to compile and link a program.
     */
    public static int createAndLinkProgram(final int vertexShaderHandle,
                                           final int fragmentShaderHandle,
                                           final String[] attributes) {

        int programHandle = GLES20.glCreateProgram();

        if (programHandle != 0) {

            // Bind the vertex shader to the program
            GLES20.glAttachShader(programHandle, vertexShaderHandle);

            // Bind the fragment shader to the program.
            GLES20.glAttachShader(programHandle, fragmentShaderHandle);

            // Bind attribuets
            if (attributes != null) {
                final int size = attributes.length;
                for (int i = 0; i < size; i++) {
                    GLES20.glBindAttribLocation(programHandle, i, attributes[i]);
                }
            }

            // Link the two shader together into a program.
            GLES20.glLinkProgram(programHandle);


            // Get the link status.
            final int[] linkStatus = new int[1];
            GLES20.glGetProgramiv(programHandle, GLES20.GL_LINK_STATUS, linkStatus, 0);

            // If the link failed, delete the program.
            if (linkStatus[0] == 0) {
                Log.e(TAG, "Error compiling program: " + GLES20.glGetProgramInfoLog(programHandle));
                GLES20.glDeleteProgram(programHandle);
                programHandle = 0;
            }
        }

        if (programHandle == 0) {
            throw new RuntimeException("Error creating program.");
        }

        return programHandle;
    }

    public static int loadTexture(final Context context,
                                  final int resourceId) {
        final int[] textureHandle = new int[1];

        GLES20.glGenTextures(1, textureHandle, 0);

        if (textureHandle[0] != 0) {
            final BitmapFactory.Options options = new BitmapFactory.Options();
            options.inScaled = false; // No pre-scaling

            // Read in the resource
            final Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), resourceId, options);

            // Bind to the texture in OpenGL
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);

            // Set filtering
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,
                    GLES20.GL_TEXTURE_MIN_FILTER,
                    GLES20.GL_NEAREST);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,
                    GLES20.GL_TEXTURE_MAG_FILTER,
                    GLES20.GL_NEAREST);

            // Load the bitmap into the bound texture
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);

            // Recycle the bitmap, since its data has been loaded into OpenGL
            bitmap.recycle();
        }

        if (textureHandle[0] == 0) {
            throw new RuntimeException("Error loading texture.");
        }

        return textureHandle[0];
    }

    public static int loadTexture(final AssetManager manager, final String path) {
        InputStream in = null;
        try {
            in = manager.open(path);
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }
        BitmapFactory.Options op = new BitmapFactory.Options();
        op.inPreferredConfig = Bitmap.Config.ARGB_8888;
        Bitmap bmp = BitmapFactory.decodeStream(in, null, op);

        // generate textureID
        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        int textureID = textures[0];

        // create texture
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureID);
        GLES20.glTexParameteri(
                GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
        GLES20.glTexParameteri(
                GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bmp, 0);

        // clean up
        try {
            in.close();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            bmp.recycle();
        }
        return textureID;
    }

    public static float[] generateCubeData(float[] point1,
                                           float[] point2,
                                           float[] point3,
                                           float[] point4,
                                           float[] point5,
                                           float[] point6,
                                           float[] point7,
                                           float[] point8,
                                           int elementsPerPoint) {

        // Given a cube with the points define as follows:
        // front left top, front right top, front left bottom,front right bottom
        // back left top, back right top, back left bottom, front right bottom
        // return an array of 6 sides, 2 triangles per side, 3 vertices per cube.

        final int FRONT = 0;
        final int RIGHT = 1;
        final int BACK = 2;
        final int LEFT = 3;
        final int TOP = 4;
        final int BOTTOM = 5;

        final int size = elementsPerPoint * 6 * 6;
        final float[] cubeData = new float[size];

        for (int face = 0; face < 6; face++) {
            // Relative to the side,
            // p1 = top left
            // p2 = top right
            // p3 = bottom left
            // p4 = bottom right
            final float[] p1, p2, p3, p4;

            // Select the points for this face
            if (face == FRONT) {
                p1 = point1;
                p2 = point2;
                p3 = point3;
                p4 = point4;
            } else if (face == RIGHT) {
                p1 = point2;
                p2 = point6;
                p3 = point4;
                p4 = point8;
            } else if (face == BACK) {
                p1 = point6;
                p2 = point5;
                p3 = point8;
                p4 = point7;
            } else if (face == LEFT) {
                p1 = point5;
                p2 = point1;
                p3 = point7;
                p4 = point3;
            } else if (face == TOP) {
                p1 = point5;
                p2 = point6;
                p3 = point1;
                p4 = point2;
            } else // if (face == BOTTOM)
            {
                p1 = point8;
                p2 = point7;
                p3 = point4;
                p4 = point3;
            }

            // In OpenGL counter-clockwise winding is default.
            // This means that when we look at a triangle,
            // if the points are counter-clockwise we are looking at the "front".
            // If not we are looking at the back.
            // OpenGL has an optimization where all back-facing triangles are culled, since they
            // usually represent the backside of an object and aren't visible anyways.

            // Build the triangles
            //  1---3,6
            //  | / |
            // 2,4--5
            int offset = face * elementsPerPoint * 6;

            for (int i = 0; i < elementsPerPoint; i++) {
                cubeData[offset++] = p1[i];
            }
            for (int i = 0; i < elementsPerPoint; i++) {
                cubeData[offset++] = p3[i];
            }
            for (int i = 0; i < elementsPerPoint; i++) {
                cubeData[offset++] = p2[i];
            }
            for (int i = 0; i < elementsPerPoint; i++) {
                cubeData[offset++] = p3[i];
            }
            for (int i = 0; i < elementsPerPoint; i++) {
                cubeData[offset++] = p4[i];
            }
            for (int i = 0; i < elementsPerPoint; i++) {
                cubeData[offset++] = p2[i];
            }
        }
        return cubeData;
    }
}
