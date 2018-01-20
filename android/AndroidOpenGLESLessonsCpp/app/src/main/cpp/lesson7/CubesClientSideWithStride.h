//
// Created by biezhihua on 2017/7/23.
//

#ifndef OPENGLLESSON_CUBESCLIENTSIDEWITHSTRIDE_H
#define OPENGLLESSON_CUBESCLIENTSIDEWITHSTRIDE_H


#include "Cubes.h"

class CubesClientSideWithStride : public Cubes {

private :
    vector<float> *mCubeBuffer;
public:

    CubesClientSideWithStride(vector<float> *cubePositions, vector<float> *cubeNormals,
                              vector<float> *cubeTextureCoordinates, int generatedCubeFactor) {
        mCubeBuffer = getInterleavedBuffer(cubePositions, cubeNormals, cubeTextureCoordinates,
                                           generatedCubeFactor);
    }

    ~CubesClientSideWithStride() {

    }

    virtual void renderer() override;

    virtual void release() override;
};


#endif //OPENGLLESSON_CUBESCLIENTSIDEWITHSTRIDE_H
