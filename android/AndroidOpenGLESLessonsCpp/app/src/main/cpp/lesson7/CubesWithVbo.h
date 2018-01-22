//
// Created by biezhihua on 2017/7/23.
//

#ifndef OPENGLLESSON_CUBESWITHVBO_H
#define OPENGLLESSON_CUBESWITHVBO_H


#include "Cubes.h"

class CubesWithVbo : public Cubes {

private:
    GLuint mCubePositionsBufferIdx;
    GLuint mCubeNormalsBufferIdx;
    GLuint mCubeTexCoordsBufferIdx;
public:

    CubesWithVbo(vector<float> *cubePositions, vector<float> *cubeNormals,
                 vector<float> *cubeTextureCoordinates, int generatedCubeFactor) {

        vector<vector<float> *> floatBuffers = getBuffers(cubePositions, cubeNormals,
                                                          cubeTextureCoordinates,
                                                          generatedCubeFactor);

        vector<float> *mCubePositions = floatBuffers.at(0);
        vector<float> *mCubeNormals = floatBuffers.at(1);
        vector<float> *mCubeTextureCoordinates = floatBuffers.at(2);

        // First, generate as many buffers as we need.
        // This will give us the OpenGL handles for these buffers.
        GLuint buffers[3];
        glGenBuffers(3, buffers);

        // Bind to the buffer. Future commands will affect this buffer specifically.
         glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        // Transfer data from client memory to the buffer.
        // We can release the client memory after this call.
        glBufferData(GL_ARRAY_BUFFER, mCubePositions->size() * BYTES_PER_FLOAT,
                     mCubePositions->data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, mCubeNormals->size() * BYTES_PER_FLOAT, mCubeNormals->data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ARRAY_BUFFER, mCubeTextureCoordinates->size() * BYTES_PER_FLOAT,
                     mCubeTextureCoordinates->data(),
                     GL_STATIC_DRAW);

        // IMPORTANT: Unbind from the buffer when we're done with it.
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mCubePositionsBufferIdx = buffers[0];
        mCubeNormalsBufferIdx = buffers[1];
        mCubeTexCoordsBufferIdx = buffers[2];

        delete (mCubeNormals);
        mCubeNormals = nullptr;

        delete (mCubeTextureCoordinates);
        mCubeTextureCoordinates = nullptr;
    }

    virtual void renderer() override;

    virtual void release() override;
};


#endif //OPENGLLESSON_CUBESWITHVBO_H
