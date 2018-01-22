//
// Created by biezhihua on 2017/7/8.
//

#include "Native2Lesson.h"
#include "../graphics/GLUtils.h"

#include <android/log.h>

#define LOG_TAG "Lesson"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

static GLint POSITION_DATA_SIZE = 3;
static GLint COLOR_DATA_SIZE = 4;
static GLint NORMAL_DATA_SIZE = 3;

static const char *POINT_VERTEX_SHADER_CODE =
        "uniform mat4 u_MVPMatrix;      \n"
                "attribute vec4 a_Position;     \n"
                "void main()                    \n"
                "{                              \n"
                "   gl_Position = u_MVPMatrix   \n"
                "               * a_Position;   \n"
                "   gl_PointSize = 5.0;         \n"
                "}                              \n";

static const char *POINT_FRAGMENT_SHADER_CODE =
        "precision mediump float;       \n"
                "void main()                    \n"
                "{                              \n"
                "   gl_FragColor = vec4(1.0,    \n"
                "   1.0, 1.0, 1.0);             \n"
                "}                              \n";

static const char *VERTEX_SHADER_CODE =
        "uniform mat4 u_MVPMatrix;      \n"     // A constant representing the combined model/view/projection matrix.
                "uniform mat4 u_MVMatrix;       \n"     // A constant representing the combined model/view matrix.
                "uniform vec3 u_LightPos;       \n"     // The position of the light in eye space.
                "attribute vec4 a_Position;     \n"     // Per-vertex position information we will pass in.
                "attribute vec4 a_Color;        \n"     // Per-vertex color information we will pass in.
                "attribute vec3 a_Normal;       \n"     // Per-vertex normal information we will pass in.
                "varying vec4 v_Color;          \n"     // This will be passed into the fragment shader.
                "void main()                    \n"     // The entry point for our vertex shader.
                "{                              \n"
                // Transform the vertex into eye space.
                "   vec3 modelViewVertex = vec3(u_MVMatrix * a_Position);              \n"
                // Transform the normal's orientation into eye space.
                "   vec3 modelViewNormal = vec3(u_MVMatrix * vec4(a_Normal, 0.0));     \n"
                // Will be used for attenuation.
                "   float distance = length(u_LightPos - modelViewVertex);             \n"
                // Get a lighting direction vector from the light to the vertex.
                "   vec3 lightVector = normalize(u_LightPos - modelViewVertex);        \n"
                // Calculate the dot product of the light vector and vertex normal.
                // If the normal and light vector are
                // pointing in the same direction then it will get max illumination.
                "   float diffuse = max(dot(modelViewNormal, lightVector), 0.1);       \n"
                // Attenuate the light based on distance.
                "   diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));  \n"
                // Multiply the color by the illumination level. It will be interpolated across the triangle.
                "   v_Color = a_Color * diffuse;                                       \n"
                // gl_Position is a special variable used to store the final position.
                // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
                "   gl_Position = u_MVPMatrix * a_Position;                            \n"
                "}";

static const char *FRAGMENT_SHADER_CODE =
        // Set the default precision to medium. We don't need as high of a
        // precision in the fragment shader.
        "precision mediump float;       \n"
                // This is the color from the vertex shader interpolated across the
                // triangle per fragment.
                "varying vec4 v_Color;          \n"
                // The entry point for our fragment shader.
                "void main()                    \n"
                "{                              \n"
                // Pass the color directly through the pipeline.
                "   gl_FragColor = v_Color;     \n"
                "}                              \n";


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


Native2Lesson::Native2Lesson() {

    mWidth = 0;
    mHeight = 0;

    mViewMatrix = NULL;
    mModelMatrix = NULL;
    mProjectionMatrix = NULL;
    mMVPMatrix = NULL;
    mLightModelMatrix = NULL;

    mMVPMatrixHandle = 0;
    mMVMatrixHandle = 0;
    mLightPosHandle = 0;
    mPositionHandle = 0;
    mColorHandle = 0;
    mNormalHandle = 0;

    mPerVertexProgramHandle = 0;
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

    LOGD("Create Native2Lesson instance successful");
}

void Native2Lesson::create() {
    LOGD("Native2Lesson create");

    // Use culling to remove back face.
    glEnable(GL_CULL_FACE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set program handles
    mPerVertexProgramHandle = GLUtils::createProgram(&VERTEX_SHADER_CODE, &FRAGMENT_SHADER_CODE);
    if (!mPerVertexProgramHandle) {
        LOGD("Could not create program");
        return;
    }

    // Set Point program handle
    mPointProgramHandle = GLUtils::createProgram(&POINT_VERTEX_SHADER_CODE,
                                                 &POINT_FRAGMENT_SHADER_CODE);
    if (!mPointProgramHandle) {
        LOGD("Could not create program");
        return;
    }

    mLightModelMatrix = new Matrix();
    mModelMatrix = new Matrix();
    mMVPMatrix = new Matrix();

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
}

void Native2Lesson::change(int width, int height) {
    LOGD("Native2Lesson change");

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

void Native2Lesson::draw() {
    // Set the OpenGL viewport to same size as the surface.

    glClearColor(0, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    // Set out pre-vertex lighting program.
    glUseProgram(mPerVertexProgramHandle);

    // Set program handle for cube drawing.
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mPerVertexProgramHandle, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(mPerVertexProgramHandle, "u_MVMatrix");
    mLightPosHandle = (GLuint) glGetUniformLocation(mPerVertexProgramHandle, "u_LightPos");
    mPositionHandle = (GLuint) glGetAttribLocation(mPerVertexProgramHandle, "a_Position");
    mColorHandle = (GLuint) glGetAttribLocation(mPerVertexProgramHandle, "a_Color");
    mNormalHandle = (GLuint) glGetAttribLocation(mPerVertexProgramHandle, "a_Normal");

    // Calculate position of the light
    // Rotate and then push into the distance.
    mLightModelMatrix->identity();
    mLightModelMatrix->translate(0, 0, -5);
    mLightModelMatrix->rotate(angleInDegrees, 0, 1, 0);
    mLightModelMatrix->translate(0, 0, 2);

    Matrix::multiplyMV(mLightPosInWorldSpace, mLightModelMatrix->mData, mLightPosInModelSpace);
    Matrix::multiplyMV(mLightPosInEyeSpace, mViewMatrix->mData, mLightPosInWorldSpace);

    // right
    mModelMatrix->identity();
    mModelMatrix->translate(4.0f, 0.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 1.0f, 0.0f, 0.0f);
    drawCube();

    // left
    mModelMatrix->identity();
    mModelMatrix->translate(-4.0f, 0.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 1.0f, 0.0f);
    drawCube();

    // top
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, 4.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 1.0f, 0.0f);
    drawCube();

    // bottom
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, -4.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 1.0f, 0.0f);
    drawCube();

    // center
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, 0.0f, -5.0f);
    mModelMatrix->rotate(angleInDegrees, 1.0f, 1.0f, 1.0f);
    drawCube();

    // Draw a pint to indicate the light
    glUseProgram(mPointProgramHandle);
    drawLight();
}

void Native2Lesson::destroy() {
    delete mModelMatrix;
    mModelMatrix = NULL;
    delete mViewMatrix;
    mViewMatrix = NULL;
    delete mProjectionMatrix;
    mProjectionMatrix = NULL;
    delete mLightModelMatrix;
    mLightModelMatrix = NULL;
}

Native2Lesson::~Native2Lesson() {
    destroy();
}

void Native2Lesson::drawCube() {

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

    // Pass in the color info
    glVertexAttribPointer(
            mColorHandle,
            COLOR_DATA_SIZE,
            GL_FLOAT,
            GL_FALSE,
            0,
            CUBE_COLOR_DATA
    );
    glEnableVertexAttribArray(mColorHandle);

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

void Native2Lesson::drawLight() {

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


///=======

static Native2Lesson *native2Lesson;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s \n", name, v);
}

static void checkGlError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson2_LessonTwoNativeRenderer_nativeSurfaceCreate(JNIEnv *env,
                                                                                   jclass type) {
    if (native2Lesson) {
        delete native2Lesson;
        native2Lesson = NULL;
    }

    // Print some OpenGL info
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    native2Lesson = new Native2Lesson();
    native2Lesson->create();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson2_LessonTwoNativeRenderer_nativeSurfaceChange(JNIEnv *env,
                                                                                   jclass type,
                                                                                   jint width,
                                                                                   jint height) {
    if (native2Lesson) {
        native2Lesson->change(width, height);
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson2_LessonTwoNativeRenderer_nativeDrawFrame(JNIEnv *env,
                                                                               jclass type) {
    if (native2Lesson) {
        native2Lesson->draw();
    }
}