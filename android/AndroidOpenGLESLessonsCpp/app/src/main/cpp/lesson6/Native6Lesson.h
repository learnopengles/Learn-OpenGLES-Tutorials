//
// Created by biezhihua on 2017/7/16.
//

#ifndef OPENGLLESSON_NATIVE6LESSON_H
#define OPENGLLESSON_NATIVE6LESSON_H


#include <graphics/Matrix.h>
#include <GLES2/gl2.h>

class Native6Lesson {

public:
    Native6Lesson();

    ~Native6Lesson();

    void create();

    void change(int width, int height);

    void draw();

    void setDelta(float x, float y);

    void setMinFilter(int filter);

    void setMagFilter(int filter);

private:

    //
    const int BYTES_PER_FLOAT = 4;
    const int POSITION_DATA_SIZE = 3;
    const int NORMAL_DATA_SIZE = 3;
    const int TEXTURE_COORDINATE_DATA_SIZE = 2;

    //
    GLsizei mWidth;
    GLsizei mHeight;

    // model/view/projection matrix
    Matrix *mModelMatrix;
    Matrix *mViewMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVPMatrix;

    //
    Matrix *mAccumulatedRotationMatrix;
    Matrix *mCurrentRotationMatrix;

    //
    Matrix *mLightModelMatrix;

    //
    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mLightPosHandle;
    GLuint mTextureUniformHandle;
    GLuint mPositionHandle;
    GLuint mNormalHandle;
    GLuint mTextureCoordinateHandle;

    GLuint mProgramHandle;
    GLuint mPointProgramHandle;

    GLuint mBrickDataHandle;
    GLuint mGrassDataHandle;

    GLint mQueuedMinFilter;
    GLint mQueuedMagFilter;

    float mLightPosInModelSpace[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mLightPosInWorldSpace[4];
    float mLightPosInEyeSpace[4];

    float mDeltaX;
    float mDeltaY;

    void drawCube();

    void drawLight();
};


#endif //OPENGLLESSON_NATIVE6LESSON_H
