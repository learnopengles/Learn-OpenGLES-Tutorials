//
// Created by biezhihua on 2017/7/30.
//

#ifndef OPENGLLESSON_HEIGHTMAP_H
#define OPENGLLESSON_HEIGHTMAP_H

#include <vector>
#include <GLES2/gl2.h>
#include "graphics/Matrix.h"

using namespace std;

class HeightMap {

private:
    /**
     * Additional constants.
     */
    static const int POSITION_DATA_SIZE_IN_ELEMENTS = 3;
    static const int NORMAL_DATA_SIZE_IN_ELEMENTS = 3;
    static const int COLOR_DATA_SIZE_IN_ELEMENTS = 4;

    static const int BYTES_PER_FLOAT = 4;
    static const int BYTES_PER_SHORT = 2;

    static const int STRIDE = (POSITION_DATA_SIZE_IN_ELEMENTS + NORMAL_DATA_SIZE_IN_ELEMENTS +
                               COLOR_DATA_SIZE_IN_ELEMENTS) * BYTES_PER_FLOAT;


    static const int SIZE_PER_SIZE = 32;
    static const float MIN_POSITION;
    static const float POSITION_RANGE;

    GLuint vbo[1] = {0};
    GLuint ibo[1] = {0};

    GLsizei indexCount = 0;

    vector<float> *heightMapVertexData;

    vector<short> *heightMapIndexData;

    /**
    * OpenGL handles to our program attributes.
    */
    GLuint positionAttribute;
    GLuint normalAttribute;
    GLuint colorAttribute;

public:

    HeightMap();

    ~HeightMap();

    void render();

    void release();

    void setPositionAttribute(GLuint positionAttribute);

    void setNormalAttribute(GLuint normalAttribute);

    void setColorAttribute(GLuint colorAttribute);
};


#endif //OPENGLLESSON_HEIGHTMAP_H
