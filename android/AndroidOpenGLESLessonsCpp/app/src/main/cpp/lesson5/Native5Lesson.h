//
// Created by biezhihua on 2017/7/15.
//

#ifndef OPENGLLESSON_NATIVE5LESSON_H
#define OPENGLLESSON_NATIVE5LESSON_H


#include <graphics/Matrix.h>

class Native5Lesson {

public:

    Native5Lesson();

    ~Native5Lesson();

    void create();

    void change(int width, int height);

    void draw();

    void switchMode();

private:

    //
    float *mCubePositionData;
    float *mCubeColorData;

    //
    GLsizei mWidth;
    GLsizei mHeight;

    // Matrix
    Matrix *mModelMatrix;
    Matrix *mViewMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVPMatrix;

    // Handle
    GLuint mMVPMatrixHandle;
    GLuint mPositionHandle;
    GLuint mColorHandle;

    GLuint mProgramHandle;

    bool mBending;

    void drawCube();

    void createCubeData();
};


#endif //OPENGLLESSON_NATIVE5LESSON_H
