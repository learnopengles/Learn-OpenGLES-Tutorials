//
// Created by biezhihua on 2017/7/8.
//

#include "GLUtils.h"
#include <android/asset_manager_jni.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/time.h>

#define LOG_TAG "Lesson"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

static JNIEnv *sEnv = NULL;
static jobject sAssetManager = NULL;

static AAsset *loadAsset(const char *path) {
    AAssetManager *nativeManager = AAssetManager_fromJava(sEnv, sAssetManager);
    if (nativeManager == NULL) {
        return NULL;
    }
    return AAssetManager_open(nativeManager, path, AASSET_MODE_UNKNOWN);
}

/**
 * Loads the given source code as a shader of the given type.
 */
static GLuint loadShader(GLenum shaderType, const char **source) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, source, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 0) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                LOGE("Error compiling shader:\n%s\n", infoLog);
                free(infoLog);
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

GLuint GLUtils::createProgram(const char **vertexSource, const char **fragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        // Bind the vertex shader to the program
        glAttachShader(program, vertexShader);

        // Bind the fragment shader to the program.
        glAttachShader(program, fragmentShader);

        GLint linkStatus;
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (!linkStatus) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 0) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(program, infoLen, NULL, infoLog);
                LOGE("Error linking program:\n%s\n", infoLog);
                free(infoLog);
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

long GLUtils::currentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, (struct timezone *) NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

GLuint GLUtils::loadTexture(const char *path) {
    GLuint textureId = 0;
    jclass utilsClass = sEnv->FindClass("com/learnopengles/android/Utils");
    if (utilsClass == NULL) {
        LOGE("Couldn't find utils class");
        return (GLuint) -1;
    }
    jmethodID loadTexture = sEnv->GetStaticMethodID(utilsClass, "loadTexture",
                                                    "(Landroid/content/res/AssetManager;Ljava/lang/String;)I");
    if (loadTexture == NULL) {
        LOGE("Couldn't find loadTexture method");
        return (GLuint) -1;
    }
    jstring pathStr = sEnv->NewStringUTF(path);
    textureId = (GLuint) sEnv->CallStaticIntMethod(utilsClass, loadTexture, sAssetManager, pathStr);
    sEnv->DeleteLocalRef(pathStr);
    return textureId;
}

void GLUtils::setEnvAndAssetManager(JNIEnv *env, jobject assetManager) {
    sEnv = env;
    sAssetManager = assetManager;
}

char *GLUtils::openTextFile(const char *path) {
    AAsset *asset = loadAsset(path);
    if (asset == NULL) {
        LOGE("Couldn't load %s", path);
        return NULL;
    }
    off_t length = AAsset_getLength(asset);
    char *buffer = new char[length + 1];
    int num = AAsset_read(asset, buffer, length);
    AAsset_close(asset);
    if (num != length) {
        LOGE("Couldn't read %s", path);
        delete[] buffer;
        return NULL;
    }
    buffer[length] = '\0';
    return buffer;
    return NULL;
}

GLfloat *GLUtils::generateCubeData(float *point1,
                                   float *point2,
                                   float *point3,
                                   float *point4,
                                   float *point5,
                                   float *point6,
                                   float *point7,
                                   float *point8,
                                   int elementsPerPoint) {

    // Given a cube with the points define as follows:
    // front left top, front right top, front left bottom,front right bottom
    // back left top, back right top, back left bottom, front right bottom
    // return an array of 6 sides, 2 triangles per side, 3 vertices per cube.

    int FRONT = 0;
    int RIGHT = 1;
    int BACK = 2;
    int LEFT = 3;
    int TOP = 4;
//    int BOTTOM = 5;

    int size = elementsPerPoint * 6 * 6;
    float *cubeData = new float[size];

    for (int face = 0; face < 6; face++) {
        // Relative to the side,
        // p1 = top left
        // p2 = top right
        // p3 = bottom left
        // p4 = bottom right
        float *p1, *p2, *p3, *p4;

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
