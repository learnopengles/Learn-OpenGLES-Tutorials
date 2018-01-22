//
// Created by biezhihua on 2017/7/22.
//

#include <graphics/GLUtils.h>
#include "Native7Lesson.h"
#include "GenData.h"
#include <android/log.h>
#include <string.h>

#define LOG_TAG "Lesson"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

// processing callback to handler class
typedef struct context {
    JavaVM *javaVM;
    jclass nativeRendererClz;
    jobject nativeRendererObj;
} Context;

static Native7Lesson *lesson7;
static GenData *genData;
static Context g_ctx;

Native7Lesson::Native7Lesson() {

    mViewMatrix = nullptr;
    mModelMatrix = nullptr;
    mProjectionMatrix = nullptr;
    mMVPMatrix = nullptr;
    mLightModelMatrix = nullptr;

    mMVPMatrixHandle = 0;
    mMVMatrixHandle = 0;
    mLightPosHandle = 0;
    mPointProgramHandle = 0;

    mLightPosInModelSpace[0] = 0.0f;
    mLightPosInModelSpace[1] = 0.0f;
    mLightPosInModelSpace[2] = 0.0f;
    mLightPosInModelSpace[3] = 1.0f;

    mLightPosInWorldSpace[0] = 0.0f;
    mLightPosInWorldSpace[1] = 0.0f;
    mLightPosInWorldSpace[2] = 0.0f;
    mLightPosInWorldSpace[3] = 0.0f;

    mLightPosInEyeSpace[0] = 0.0f;
    mLightPosInEyeSpace[1] = 0.0f;
    mLightPosInEyeSpace[2] = 0.0f;
    mLightPosInEyeSpace[3] = 0.0f;
}

Native7Lesson::~Native7Lesson() {
    delete genData;
    genData = nullptr;
    delete mModelMatrix;
    mModelMatrix = nullptr;
    delete mViewMatrix;
    mViewMatrix = nullptr;
    delete mProjectionMatrix;
    mProjectionMatrix = nullptr;
    delete mMVPMatrix;
    mMVPMatrix = nullptr;
}

void Native7Lesson::create() {

//    genData->setNative7Lesson(this);

    genData->genCube(3, false, false);

    // Set the background clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Use culling to remove back face.
    glEnable(GL_CULL_FACE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = -0.5f;

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

    // Main Program
    const char *vertex = GLUtils::openTextFile("vertex/lesson_seven_vertex_shader.glsl");
    const char *fragment = GLUtils::openTextFile(
            "fragment/lesson_seven_fragment_shader.glsl");

    // Set program handles
    mProgramHandle = GLUtils::createProgram(&vertex, &fragment);
    if (!mProgramHandle) {
        LOGD("Could not create program");
        return;
    }

    // Load the texture
    mAndroidDataHandle = GLUtils::loadTexture("texture/usb_android.png");
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, mAndroidDataHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, mAndroidDataHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    mModelMatrix = new Matrix();
    mMVPMatrix = new Matrix();
    mViewMatrix = new Matrix();
    mProjectionMatrix = new Matrix();
    mCurrentRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix = new Matrix();
    mLightModelMatrix = new Matrix();
}

void Native7Lesson::change(int width, int height) {

    glViewport(0, 0, width, height);

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

void Native7Lesson::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 100000.0f) * ((int) time);

    // Set our per-vertex lighting program.
    glUseProgram(mProgramHandle);

    // Set program handles for cube drawing.
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_MVMatrix");
    mLightPosHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_LightPos");
    mTextureUniformHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_Texture");
    GLuint positionHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_Position");
    GLuint normalHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_Normal");
    GLuint textureCoordinateHandle = (GLuint) glGetAttribLocation(mProgramHandle,
                                                                  "a_TexCoordinate");

    if (genData != nullptr && genData->getCubes() != nullptr) {
        genData->getCubes()->setNormalHandle(normalHandle);
        genData->getCubes()->setPositionHandle(positionHandle);
        genData->getCubes()->setTextureCoordinateHandle(textureCoordinateHandle);
    }

    // Calculate position of the light
    // Rotate and then push into the distance.
    mLightModelMatrix->identity();
    mLightModelMatrix->translate(0, 0, -1);

    Matrix::multiplyMV(mLightPosInWorldSpace, mLightModelMatrix->mData, mLightPosInModelSpace);
    Matrix::multiplyMV(mLightPosInEyeSpace, mViewMatrix->mData, mLightPosInWorldSpace);

    // Draw a cube.
    // Translate the cube into the screen.
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, 0.0f, -3.5f);
    mModelMatrix->rotate(angleInDegrees, 1, 1, 1);

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

    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture to this unit.
    glBindTexture(GL_TEXTURE_2D, mAndroidDataHandle);

    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glUniform1i(mTextureUniformHandle, 0);

    if (genData != nullptr && genData->getCubes() != nullptr) {
        genData->getCubes()->renderer();
    }
}

void Native7Lesson::decreaseCubeCount() {
    if (genData != nullptr && genData->mLastRequestedCubeFactor > 1) {
        genData->genCube(--genData->mLastRequestedCubeFactor, false, false);
    }
}

void Native7Lesson::increaseCubeCount() {
    if (genData != nullptr && genData->mLastRequestedCubeFactor < 16) {
        genData->genCube(++genData->mLastRequestedCubeFactor, false, false);
    }
}

void Native7Lesson::setDelta(float x, float y) {
    mDeltaX += x;
    mDeltaY += y;
}

void Native7Lesson::toggleStride() {
    genData->genCube(genData->mLastRequestedCubeFactor, false, true);
}

void Native7Lesson::toggleVBOs() {
    genData->genCube(genData->mLastRequestedCubeFactor, true, false);
}

void Native7Lesson::updateVboStatus(bool useVbos) {
    LOGD("updateVboStatus %d", useVbos);
    Context *pctx = &g_ctx;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (JNI_OK != res) {
        LOGE("Failed to Get env, ErrorCode = %d", res);
        return;
    }
    jmethodID statusId = env->GetMethodID(pctx->nativeRendererClz, "updateVboStatus", "(Z)V");
    env->CallVoidMethod(pctx->nativeRendererObj, statusId, useVbos);
}

void Native7Lesson::updateStrideStatus(bool useStride) {
    LOGD("updateStrideStatus %d", useStride);
    Context *pctx = &g_ctx;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (JNI_OK != res) {
        LOGE("Failed to Get env, ErrorCode = %d", res);
        return;
    }
    jmethodID statusId = env->GetMethodID(pctx->nativeRendererClz, "updateStrideStatus", "(Z)V");
    env->CallVoidMethod(pctx->nativeRendererObj, statusId, useStride);
}

// ----------------------------------------------------------

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }
    g_ctx.nativeRendererObj = NULL;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeToggleStride(JNIEnv *env,
                                                                                    jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->toggleStride();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeToggleVBOs(JNIEnv *env,
                                                                                  jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->toggleVBOs();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeIncreaseCubeCount(
        JNIEnv *env, jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->increaseCubeCount();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeDecreaseCubeCount(
        JNIEnv *env, jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->decreaseCubeCount();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeSetDelta(JNIEnv *env,
                                                                                jobject instance,
                                                                                jfloat x,
                                                                                jfloat y) {
    if (lesson7 != nullptr) {
        lesson7->setDelta(x, y);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeDrawFrame(JNIEnv *env,
                                                                                 jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->draw();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeSurfaceChange(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jint width,
                                                                                     jint height) {
    if (lesson7 != nullptr) {
        lesson7->change(width, height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeSurfaceCreate(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jobject assetManager) {

    GLUtils::setEnvAndAssetManager(env, assetManager);
    lesson7 = new Native7Lesson();
    genData = new GenData(lesson7);
    if (lesson7 != nullptr) {
        lesson7->create();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeDestroy(JNIEnv *env,
                                                                               jobject instance) {
    env->DeleteGlobalRef(g_ctx.nativeRendererClz);
    env->DeleteGlobalRef(g_ctx.nativeRendererObj);
    g_ctx.nativeRendererClz = NULL;
    g_ctx.nativeRendererObj = NULL;

    if (lesson7 != nullptr) {
        delete (lesson7);
        lesson7 = NULL;
    }
    if (genData != nullptr) {
        delete (genData);
        genData = NULL;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeInit(JNIEnv *env,
                                                                            jobject instance) {
    jclass clz = env->GetObjectClass(instance);
    g_ctx.nativeRendererClz = (jclass) env->NewGlobalRef(clz);
    g_ctx.nativeRendererObj = env->NewGlobalRef(instance);

}
