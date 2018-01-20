//
// Created by biezhihua on 2017/7/30.
//

#include <jni.h>
#include "Native8Lesson.h"
#include <android/log.h>
#include "graphics/GLUtils.h"

#define LOG_TAG "Lesson8"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

const string Native8Lesson::MVP_MATRIX_UNIFORM = "u_MVPMatrix";
const string Native8Lesson::MV_MATRIX_UNIFORM = "u_MVMatrix";
const string Native8Lesson::LIGHT_POSITION_UNIFORM = "u_LightPos";

const string Native8Lesson::POSITION_ATTRIBUTE = "a_Position";
const string Native8Lesson::NORMAL_ATTRIBUTE = "a_Normal";
const string Native8Lesson::COLOR_ATTRIBUTE = "a_Color";

Native8Lesson::Native8Lesson() : modelMatrix(nullptr),
                                 viewMatrix(nullptr),
                                 projectionMatrix(nullptr),
                                 mvpMatrix(nullptr),
                                 accumulatedRotation(nullptr),
                                 currentRotaion(nullptr),
                                 lightModelMatrix(nullptr),
                                 heightMap(nullptr) {
    LOGD("Native8Lesson");

    lightPosInModelSpace[0] = 0.0f;
    lightPosInModelSpace[1] = 0.0f;
    lightPosInModelSpace[2] = 0.0f;
    lightPosInModelSpace[3] = 1.0f;

    lightPosInWorldSpace[0] = 0.0f;
    lightPosInWorldSpace[1] = 0.0f;
    lightPosInWorldSpace[2] = 0.0f;
    lightPosInWorldSpace[3] = 0.0f;

    lightPosInEyeSpace[0] = 0.0f;
    lightPosInEyeSpace[1] = 0.0f;
    lightPosInEyeSpace[2] = 0.0f;
    lightPosInEyeSpace[3] = 0.0f;

}

Native8Lesson::~Native8Lesson() {

    delete (modelMatrix);
    delete (viewMatrix);
    delete (projectionMatrix);
    delete (mvpMatrix);
    delete (accumulatedRotation);
    delete (currentRotaion);
    delete (lightModelMatrix);

    modelMatrix = nullptr;
    viewMatrix = nullptr;
    projectionMatrix = nullptr;
    mvpMatrix = nullptr;
    accumulatedRotation = nullptr;
    currentRotaion = nullptr;
    lightModelMatrix = nullptr;

    heightMap->release();
    delete (heightMap);
    heightMap = nullptr;

    LOGD("~Native8Lesson");
}

void Native8Lesson::create() {
    heightMap = new HeightMap();

    // Set the background clear color th black.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = -0.5f;

    // We are looking toward the distance
    float lookX = 0.0f;
    float lookY = 0.0f;
    float lookZ = -5.0f;

    // Set our up vector. This is where our head would be pointing were we
    // holding the camera.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Set the view matrix. This matrix can be said to represent the camera
    // position.
    // NOTE: In OpenGL 1, a ModelView matrix is used, which is a combination
    // of a model and view matrix. In OpenGL 2, we can keep track of these
    // matrices separately if we choose.
    viewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

    // Main Program
    const char *vertex = GLUtils::openTextFile("vertex/per_pixel_vertex_shader_no_tex.glsl");
    const char *fragment = GLUtils::openTextFile("fragment/per_pixel_fragment_shader_no_tex.glsl");

    // Set the program.
    program = GLUtils::createProgram(&vertex, &fragment);
    if (!program) {
        LOGD("Could not create program");
        return;
    }

    // Init matrix
    modelMatrix = new Matrix();
    mvpMatrix = new Matrix();
    accumulatedRotation = new Matrix();
    currentRotaion = new Matrix();
    lightModelMatrix = new Matrix();

    LOGD("create");
}

void Native8Lesson::change(int width, int height) {
    // Set the OpenGL viewport to the same size as the surface.
    glViewport(0, 0, width, height);

    // Create a new perspective projection matrix.
    // The height wil stay the same while the width
    // vary as per aspect ratio.
    float ratio = (float) width / height;
    float left = -ratio;
    float right = ratio;
    float bottom = -1.0f;
    float top = 1.0f;
    float near = 1.0f;
    float far = 1000.0f;

    projectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);

    LOGD("change");
}

void Native8Lesson::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set our per-vertex lighting program.
    glUseProgram(program);
    mvpMatrixUniform = glGetUniformLocation(program, MVP_MATRIX_UNIFORM.c_str());
    mvMatrixUniform = glGetUniformLocation(program, MV_MATRIX_UNIFORM.c_str());
    lightPosUniform = glGetAttribLocation(program, LIGHT_POSITION_UNIFORM.c_str());

    GLint positionAttribute = glGetAttribLocation(program, POSITION_ATTRIBUTE.c_str());
    GLint normalAttribute = glGetAttribLocation(program, NORMAL_ATTRIBUTE.c_str());
    GLint colorAttribute = glGetAttribLocation(program, COLOR_ATTRIBUTE.c_str());

    if (heightMap != nullptr) {
        heightMap->setPositionAttribute((GLuint) positionAttribute);
        heightMap->setNormalAttribute((GLuint) normalAttribute);
        heightMap->setColorAttribute((GLuint) colorAttribute);
    }

    // Calculate position of the light. Push into the distance.
    lightModelMatrix->identity();
    lightModelMatrix->translate(0.0f, 7.5f, -8.0f);

    Matrix::multiplyMV(lightPosInWorldSpace, lightModelMatrix->mData, lightPosInModelSpace);
    Matrix::multiplyMV(lightPosInEyeSpace, viewMatrix->mData, lightPosInWorldSpace);

    // Draw the heightmap
    // Translate the heightmap into the screen.
    modelMatrix->identity();
    modelMatrix->translate(0.0f, 0.0f, -12.0f);

    // Set a matrix that contains the current rotation.
    currentRotaion->identity();
    currentRotaion->rotate(deltaX, 0.0f, 1.0f, 0.0f);
    currentRotaion->rotate(deltaY, 1.0f, 0.0f, 0.0f);
    deltaX = 0;
    deltaY = 0;

    Matrix tempMatrix;

    // Multiply the current rotation by the accumulated rotation, and then set the accumulated rotation to the result.
    tempMatrix.identity();
    tempMatrix.multiply(*currentRotaion, *accumulatedRotation);
    accumulatedRotation->loadWith(tempMatrix);

    // Rotate the cube taking the overall rotation into account.
    tempMatrix.identity();
    tempMatrix.multiply(*modelMatrix, *accumulatedRotation);
    modelMatrix->loadWith(tempMatrix);

    // This multiplies the view by the model matrix
    // and stores the result the MVP matrix.
    // which currently contains model * view
    mvpMatrix->multiply(*viewMatrix, *modelMatrix);
    glUniformMatrix4fv(mvMatrixUniform, 1, GL_FALSE, mvpMatrix->mData);

    // Pass in the combined matrix.
    mvpMatrix->multiply(*projectionMatrix, *mvpMatrix);
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrix->mData);

    glUniform3f(lightPosUniform, lightPosInEyeSpace[0], lightPosInEyeSpace[1],
                lightPosInEyeSpace[2]);

    // Renderer the heightmap;
    heightMap->render();
}

void Native8Lesson::setDelta(float x, float y) {
    deltaX += x;
    deltaY += y;
}


// ------------------------------------------------------------------

Native8Lesson native8Lesson;

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson8_LessonEightNativeRenderer_nativeSurfaceCreate(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jobject assetManager) {

    GLUtils::setEnvAndAssetManager(env, assetManager);
    native8Lesson.create();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson8_LessonEightNativeRenderer_nativeSurfaceChange(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jint width,
                                                                                     jint height) {

    native8Lesson.change(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson8_LessonEightNativeRenderer_nativeDrawFrame(JNIEnv *env,
                                                                                 jobject instance) {
    native8Lesson.draw();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson8_LessonEightNativeRenderer_nativeSetDelta(JNIEnv *env,
                                                                                jobject instance,
                                                                                jfloat x,
                                                                                jfloat y) {
    native8Lesson.setDelta(x, y);
}