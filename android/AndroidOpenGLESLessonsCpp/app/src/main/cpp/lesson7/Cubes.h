//
// Created by biezhihua on 2017/7/21.
//

#ifndef OPENGLLESSON_CUBES_H
#define OPENGLLESSON_CUBES_H

#include <vector>
#include <GLES2/gl2.h>

using namespace std;

class Cubes {

protected:

    /**
     * This will be used to pass in model position information.
     */
    GLuint mPositionHandle = 0;

    /**
     * This will be used to pass in model normal information.
     */
    GLuint mNormalHandle = 0;

    /**
     * This will be used to pass in model texture coordinate information.
     */
    GLuint mTextureCoordinateHandle = 0;

    int mActualCubeFactor = 0;

public:
    void setPositionHandle(GLuint positionHandle);

    void setNormalHandle(GLuint normalHandle);

    void setTextureCoordinateHandle(GLuint textureCoordinateHandle);


public:
    vector<vector<float> *> getBuffers(vector<float> *cubePositions,
                                       vector<float> *cubeNormals,
                                       vector<float> *cubeTextureCoordinates,
                                       int generatedCubeFactor);

    vector<float> *getInterleavedBuffer(vector<float> *cubePositions,
                                        vector<float> *cubeNormals,
                                        vector<float> *cubeTextureCoordinates,
                                        int generatedCubeFactor);

    virtual void renderer()= 0;

    virtual void release()= 0;

    /**
     * Size of the position data in elements.
     */
    static int POSITION_DATA_SIZE;

    /**
     *  Size of the normal data in elements.
     */
    static int NORMAL_DATA_SIZE;

    /**
     * Size of the texture coordinate data in elements.
     */
    static int TEXTURE_COORDINATE_DATA_SIZE;

    /**
     * How many bytes per float.
     */
    static  int BYTES_PER_FLOAT;
};


#endif //OPENGLLESSON_CUBES_H
