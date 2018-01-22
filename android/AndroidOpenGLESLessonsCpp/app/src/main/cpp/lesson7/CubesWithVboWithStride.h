//
// Created by biezhihua on 2017/7/23.
//

#ifndef OPENGLLESSON_CUBESWITHVBOWITHSTRIDE_H
#define OPENGLLESSON_CUBESWITHVBOWITHSTRIDE_H


#include "Cubes.h"

class CubesWithVboWithStride : public Cubes {

private:
    GLuint mCubeBufferIdx;
public:

    CubesWithVboWithStride(vector<float> *cubePositions, vector<float> *cubeNormals,
                           vector<float> *cubeTextureCoordinates, int generatedCubeFactor) {
        vector<float> *cubeBuffer = getInterleavedBuffer(cubePositions, cubeNormals,
                                                         cubeTextureCoordinates,
                                                         generatedCubeFactor);

        // Second, copy these buffers into OpenGL's memory. After, we don't need to keep the client-side buffers around.
        GLuint buffers[1];
        glGenBuffers(1, buffers);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, cubeBuffer->size() * BYTES_PER_FLOAT, cubeBuffer->data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mCubeBufferIdx = buffers[0];

        delete (cubeBuffer);
        cubeBuffer = nullptr;
    }

    virtual void renderer() override;

    virtual void release() override;


};


#endif //OPENGLLESSON_CUBESWITHVBOWITHSTRIDE_H
