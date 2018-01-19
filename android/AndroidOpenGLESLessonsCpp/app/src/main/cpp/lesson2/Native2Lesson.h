//
// Created by biezhihua on 2017/7/8.
//

#ifndef OPENGLLESSON_NATIVE2LESSON_H
#define OPENGLLESSON_NATIVE2LESSON_H


#include <GLES2/gl2.h>
#include "../graphics/Matrix.h"

class Native2Lesson {

public:

    Native2Lesson();

    ~Native2Lesson();

    void create();

    void change(int width, int height);

    void draw();

    void destroy();

private:

    GLsizei mWidth;
    GLsizei mHeight;

    Matrix *mViewMatrix;
    Matrix *mModelMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVPMatrix;
    Matrix *mLightModelMatrix;

    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mLightPosHandle;
    GLuint mPositionHandle;
    GLuint mColorHandle;
    GLuint mNormalHandle;

    GLuint mPerVertexProgramHandle;
    GLuint mPointProgramHandle;

    float mLightPosInModelSpace[4];
    float mLightPosInWorldSpace[4];
    float mLightPosInEyeSpace[4];

    void drawCube();

    void drawLight();
};

#endif //OPENGLLESSON_NATIVE2LESSON_H
