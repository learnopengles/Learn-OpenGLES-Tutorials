#ifndef OPENGLLESSON_NATIVE8LESSON_H
#define OPENGLLESSON_NATIVE8LESSON_H


#include <GLES2/gl2.h>
#include <string>
#include "HeightMap.h"
#include "graphics/Matrix.h"

using namespace std;

class Native8Lesson {

public :

    Native8Lesson();

    ~Native8Lesson();

    void create();

    void change(int width, int height);

    void draw();

    void setDelta(float x, float y);

private:
    /**
    * Identifiers for our uniforms and attributes inside the shaders.
    */
    static const string MVP_MATRIX_UNIFORM;
    static const string MV_MATRIX_UNIFORM;
    static const string LIGHT_POSITION_UNIFORM;

    static const string POSITION_ATTRIBUTE;
    static const string NORMAL_ATTRIBUTE;
    static const string COLOR_ATTRIBUTE;

    /**
     * Used to hold a light centered on the origin in model space.
     * We need a 4th coordinate so we can get translations to
     * work when we multiply this by our transformation matrices.
     */
    float lightPosInModelSpace[4];

    /**
     * Used to hold the current position of the light in world space
     * (after transformation via model matrix).
     */
    float lightPosInWorldSpace[4];

    /**
     * Used to hold the transformed position of the light in eye space
     * (after transformation via modelview matrix).
     */
    float lightPosInEyeSpace[4];

    /**
     * Store the model matrix.
     * This matrix is used to move models from
     * object space (where each model can be tought of
     * being located at the center of the universe)
     * to world space.
     */
    Matrix *modelMatrix;

    /**
     * Store the view matrix.
     * This can be tought of as our camera.
     * This matrix transforms world space to eye space.
     * it positions things relative to our eye.
     */
    Matrix *viewMatrix;

    /**
     * Store the projection matrix.
     * This is used to projet the scene onto a 2D viewport.
     */
    Matrix *projectionMatrix;

    /**
     * Allocate storage for the final combined matrix.
     * This will be passed into the shader program.
     */
    Matrix *mvpMatrix;

    Matrix *accumulatedRotation;
    Matrix *currentRotaion;
    Matrix *lightModelMatrix;

    /**
     * OpenGL handles to our program uniforms
     */
    GLint mvpMatrixUniform;
    GLint mvMatrixUniform;
    GLint lightPosUniform;


    /**
     * This is a handle to our cube shading program.
     */
    GLuint program;

    /**
     * Retaion the most recent delta for touch events.
     */
    volatile float deltaX;
    volatile float deltaY;


    /**
     * The current heightmap object.
     */
    HeightMap *heightMap;
};

#endif //OPENGLLESSON_NATIVE8LESSON_H
