//
// Created by biezhihua on 2017/7/16.
//

#include <graphics/GLUtils.h>
#include "Native6Lesson.h"
#include <android/log.h>

#define LOG_TAG "Lesson"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

const static GLfloat CUBE_POSITION_DATA[] = {
        // Front face
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        // Right face
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        // Back face
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // Left face
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        // Top face
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,

        // Bottom face
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
};

static const GLfloat CUBE_COLOR_DATA[] = {
        // R, G, B, A

        // Front face (red)
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        // Right face (green)
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,

        // Back face (blue)
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        // Left face (yellow)
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,

        // Top face (cyan)
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,

        // Bottom face (magenta)
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f
};

// X, Y, Z
// The normal is used in light calculations and is a vector which points
// orthogonal to the plane of the surface. For a cube model, the normals
// should be orthogonal to the points of each face.
static const GLfloat CUBE_NORMAL_DATA[] = {
        // Front face
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Right face
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Back face
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Left face
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // Top face
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Bottom face
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
};

/**
     * S,T (X,Y)
     * Texture coordinate data.
     * Because images have a Y axis pointing downward,
     * while OpenGL has a Y axis pointing upward, we adjust for
     * that here by flipping the Y axis.
     * What's more is that the texture coordinates are the same for every face.
     */
static const GLfloat CUBE_TEXTURE_COORDINATE_DATA[] =
        {
                // Front face
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,

                // Right face
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,

                // Back face
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,

                // Left face
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,

                // Top face
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,

                // Bottom face
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f
        };


// S, T (or X, Y)
// Texture coordinate data.
// Because images have a Y axis pointing downward (values increase as you move down the image) while
// OpenGL has a Y axis pointing upward, we adjust for that here by flipping the Y axis.
// What's more is that the texture coordinates are the same for every face.
static const GLfloat CUBE_TEXTURE_COORDINATE_DATA_FOR_PLANE[] =
        {
                // Front face
                0.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 25.0f,
                25.0f, 0.0f,

                // Right face
                0.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 25.0f,
                25.0f, 0.0f,

                // Back face
                0.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 25.0f,
                25.0f, 0.0f,

                // Left face
                0.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 25.0f,
                25.0f, 0.0f,

                // Top face
                0.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 25.0f,
                25.0f, 0.0f,

                // Bottom face
                0.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 0.0f,
                0.0f, 25.0f,
                25.0f, 25.0f,
                25.0f, 0.0f
        };

Native6Lesson::Native6Lesson() {

}

Native6Lesson::~Native6Lesson() {

}

void Native6Lesson::create() {
    // Use culling to remove back face.
    glEnable(GL_CULL_FACE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Main Program
    const char *vertex = GLUtils::openTextFile("vertex/per_pixel_vertex_shader_tex_and_light.glsl");
    const char *fragment = GLUtils::openTextFile(
            "fragment/per_pixel_fragment_shader_tex_and_light.glsl");

    // Set program handles
    mProgramHandle = GLUtils::createProgram(&vertex, &fragment);
    if (!mProgramHandle) {
        LOGD("Could not create program");
        return;
    }

    // Main Program
    const char *pointVertex = GLUtils::openTextFile(
            "vertex/point_vertex_shader.glsl");
    const char *pointFragment = GLUtils::openTextFile(
            "fragment/point_fragment_shader.glsl");

    // Set program handles
    mPointProgramHandle = GLUtils::createProgram(&pointVertex, &pointFragment);
    if (!mPointProgramHandle) {
        LOGD("Could not create program");
        return;
    }

    // Load the texture
    mBrickDataHandle = GLUtils::loadTexture("texture/stone_wall_public_domain.png");
    glGenerateMipmap(GL_TEXTURE_2D);

    mGrassDataHandle = GLUtils::loadTexture("texture/noisy_grass_public_domain.png");
    glGenerateMipmap(GL_TEXTURE_2D);

    mLightModelMatrix = new Matrix();
    mModelMatrix = new Matrix();
    mMVPMatrix = new Matrix();
    mCurrentRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix = new Matrix();

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 1.5f;

    // We are looking at the origin
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = -5.0f;

    // Set our up vector.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Set the view matrix.
    mViewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

    if (mQueuedMinFilter != 0) {
        setMinFilter(mQueuedMinFilter);
    }

    if (mQueuedMagFilter != 0) {
        setMagFilter(mQueuedMagFilter);
    }
}

void Native6Lesson::change(int width, int height) {

    mWidth = width;
    mHeight = height;

    glViewport(0, 0, mWidth, mHeight);

    // Create a new perspective projection matrix. The height will stay the same
    // while the width will vary as per aspect ratio.
    float ratio = (float) width / height;
    float left = -ratio;
    float right = ratio;
    float bottom = -1.0f;
    float top = 1.0f;
    float near = 1.0f;
    float far = 10.0f;

    mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);

}

void Native6Lesson::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    long slowTime = GLUtils::currentTimeMillis() % 100000L;
    float angleInDegrees = (180.0f / 100000.0f) * ((int) time);
    float slowAngleInDegrees = (180.0f / 1000000.0f) * ((int) slowTime);

    // Set out pre-vertex lighting program.
    glUseProgram(mProgramHandle);

    // Set program handles for cube drawing.
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_MVMatrix");
    mLightPosHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_LightPos");
    mTextureUniformHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_Texture");
    mPositionHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_Position");
    mNormalHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_Normal");
    mTextureCoordinateHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_TexCoordinate");

    // Calculate position of the light
    // Rotate and then push into the distance.
    mLightModelMatrix->identity();
    mLightModelMatrix->translate(0, 0, -2);
    mLightModelMatrix->rotate(angleInDegrees, 0, 1, 0);
    mLightModelMatrix->translate(0, 0, 3.5);

    Matrix::multiplyMV(mLightPosInWorldSpace, mLightModelMatrix->mData, mLightPosInModelSpace);
    Matrix::multiplyMV(mLightPosInEyeSpace, mViewMatrix->mData, mLightPosInWorldSpace);

    // Draw a cube.
    // Translate the cube into the screen.
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, 0.8f, -3.5f);
//    mModelMatrix->rotate(angleInDegrees, 1.0f, 1.0f, 1.0f);

    // Set a matrix that contains the current rotation.
    mCurrentRotationMatrix->identity();
    mCurrentRotationMatrix->rotate(mDeltaX, 0.0f, 1.0f, 0.0f);
    mCurrentRotationMatrix->rotate(mDeltaY, 1.0f, 0.0f, 0.0f);
    mDeltaX = 0.0f;
    mDeltaY = 0.0f;

    Matrix tempMatrix;

    // Multiply the current rotation by the accumulated rotation, and then set the accumulated rotation to the result.
    tempMatrix.identity();
    tempMatrix.multiply(*mCurrentRotationMatrix, *mAccumulatedRotationMatrix);
    mAccumulatedRotationMatrix->loadWith(tempMatrix);

    // Rotate the cube taking the overall rotation into account.
    tempMatrix.identity();
    tempMatrix.multiply(*mModelMatrix, *mAccumulatedRotationMatrix);
    mModelMatrix->loadWith(tempMatrix);

    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture to this unit.
    glBindTexture(GL_TEXTURE_2D, mBrickDataHandle);

    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glUniform1i(mTextureUniformHandle, 0);

    drawCube();

    // Draw a plane
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, -2.0f, -5.0f);
    mModelMatrix->scale(25.0f, 1.0f, 25.0f);
    mModelMatrix->rotate(slowAngleInDegrees, 0.0f, 1.0f, 0.0f);

    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture to this unit.
    glBindTexture(GL_TEXTURE_2D, mGrassDataHandle);

    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glUniform1i(mTextureUniformHandle, 0);

    drawCube();

    // Draw a point to indicate the light
    glUseProgram(mPointProgramHandle);
    drawLight();
}

void Native6Lesson::setMinFilter(int filter) {
    if (mBrickDataHandle != 0 && mGrassDataHandle != 0) {
        glBindTexture(GL_TEXTURE_2D, mBrickDataHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glBindTexture(GL_TEXTURE_2D, mGrassDataHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    } else {
        mQueuedMinFilter = filter;
    }
}

void Native6Lesson::setMagFilter(int filter) {
    if (mBrickDataHandle != 0 && mGrassDataHandle != 0) {
        glBindTexture(GL_TEXTURE_2D, mBrickDataHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glBindTexture(GL_TEXTURE_2D, mGrassDataHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    } else {
        mQueuedMagFilter = filter;
    }
}

void Native6Lesson::drawCube() {

    // Pass in the position info
    glVertexAttribPointer(
            mPositionHandle,
            POSITION_DATA_SIZE,
            GL_FLOAT,
            GL_FALSE,
            0,
            CUBE_POSITION_DATA
    );
    glEnableVertexAttribArray(mPositionHandle);

    // Pass in the texture coordinate information
    glVertexAttribPointer(
            mTextureCoordinateHandle,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            CUBE_TEXTURE_COORDINATE_DATA
    );
    glEnableVertexAttribArray(mTextureCoordinateHandle);

    // Pass in the normal information
    glVertexAttribPointer(
            mNormalHandle,
            NORMAL_DATA_SIZE,
            GL_FLOAT,
            GL_FALSE,
            0,
            CUBE_NORMAL_DATA
    );
    glEnableVertexAttribArray(mNormalHandle);

    // This multiplies the view by the model matrix
    // and stores the result the MVP matrix.
    // which currently contains model * view
    mMVPMatrix->multiply(*mViewMatrix, *mModelMatrix);

    // Pass in the model view matrix
    glUniformMatrix4fv(
            mMVMatrixHandle,
            1,
            GL_FALSE,
            mMVPMatrix->mData
    );

    // This multiplies the model view matrix by the projection matrix
    // and stores the result in the MVP matrix.
    // which no contains model * view * projection
    mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);

    // Pass in the model view projection matrix
    glUniformMatrix4fv(
            mMVPMatrixHandle,
            1,
            GL_FALSE,
            mMVPMatrix->mData
    );

    // Pass in the light position in eye space
    glUniform3f(mLightPosHandle,
                mLightPosInEyeSpace[0],
                mLightPosInEyeSpace[1],
                mLightPosInEyeSpace[2]
    );

    // Draw the cube
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Native6Lesson::drawLight() {

    GLint pointMVPMatrixHandle = glGetUniformLocation(mPointProgramHandle, "u_MVPMatrix");
    GLint pointPositionHandle = glGetAttribLocation(mPointProgramHandle, "a_Position");

    // Pass in the position
    glVertexAttrib3f(
            pointPositionHandle,
            mLightPosInModelSpace[0],
            mLightPosInModelSpace[1],
            mLightPosInModelSpace[2]);

    // Since we are not using a buffer object,
    // disable vertex arrays for the attribute
    glDisableVertexAttribArray(pointPositionHandle);

    // Pass in the transformation matrix.
    mMVPMatrix->identity();
    mMVPMatrix->multiply(*mViewMatrix, *mLightModelMatrix);
    mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);

    glUniformMatrix4fv(
            pointMVPMatrixHandle,
            1,
            GL_FALSE,
            mMVPMatrix->mData
    );

    glDrawArrays(GL_POINTS, 0, 1);
}

void Native6Lesson::setDelta(float x, float y) {
    mDeltaX += x;
    mDeltaY += y;
}

//////////////////////

Native6Lesson *lesson6;

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson6_LessonSixNativeRenderer_nativeSurfaceCreate(JNIEnv *env,
                                                                                   jclass type,
                                                                                   jobject assetManager) {
    GLUtils::setEnvAndAssetManager(env, assetManager);
    if (lesson6) {
        delete lesson6;
        lesson6 = NULL;
    }
    lesson6 = new Native6Lesson();
    lesson6->create();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson6_LessonSixNativeRenderer_nativeSurfaceChange(JNIEnv *env,
                                                                                   jclass type,
                                                                                   jint width,
                                                                                   jint height) {
    if (lesson6) {
        lesson6->change(width, height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson6_LessonSixNativeRenderer_nativeDrawFrame(JNIEnv *env,
                                                                               jclass type) {
    if (lesson6) {
        lesson6->draw();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson6_LessonSixNativeRenderer_nativeSetDelta(JNIEnv *env,
                                                                              jclass type, jfloat x,
                                                                              jfloat y) {
    if (lesson6) {
        lesson6->setDelta(x, y);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson6_LessonSixNativeRenderer_nativeSetMinFilter(JNIEnv *env,
                                                                                  jclass type,
                                                                                  jint filter) {
    if (lesson6) {
        lesson6->setMagFilter(filter);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson6_LessonSixNativeRenderer_nativeSetMagFilter(JNIEnv *env,
                                                                                  jclass type,
                                                                                  jint filter) {
    if (lesson6) {
        lesson6->setMinFilter(filter);
    }
}
