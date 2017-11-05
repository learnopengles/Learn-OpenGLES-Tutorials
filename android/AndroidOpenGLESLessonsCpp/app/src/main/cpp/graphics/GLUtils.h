//
// Created by biezhihua on 2017/7/8.
//

#ifndef OPENGLLESSON_GLUTILS_H
#define OPENGLLESSON_GLUTILS_H

#include <jni.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class GLUtils {
public:

    /**
     * Set Environment parameter
     */
    static void setEnvAndAssetManager(JNIEnv *env, jobject assetManager);

    /**
     *  Loads a file from assets/path into a char array.
     */
    static char *openTextFile(const char *path);

    /**
     * Loads a texture from assets/texture/<name>
     */
    static GLuint loadTexture(const char *name);

    /**
     * Create a program with the given vertex and framgent
     * shader source code.
     */
    static GLuint createProgram(const char **vertexSource, const char **fragmentSource);

    /**
     * Current Time Millis
     */
    static long currentTimeMillis();


    static GLfloat *
    generateCubeData(
            float *point1, float *point2, float *point3, float *point4,
            float *point5, float *point6, float *point7, float *point8,
            int elementsPerPoint);
};

#endif //OPENGLLESSON_GLUTILS_H
