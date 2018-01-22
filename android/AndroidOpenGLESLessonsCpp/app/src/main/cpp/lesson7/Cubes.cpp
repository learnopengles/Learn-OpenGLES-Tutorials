//
// Created by biezhihua on 2017/7/21.
//

#include "Cubes.h"

int Cubes::POSITION_DATA_SIZE = 3;

int Cubes::NORMAL_DATA_SIZE = 3;

int Cubes::TEXTURE_COORDINATE_DATA_SIZE = 2;

int Cubes::BYTES_PER_FLOAT = 4;

vector<vector<float> *> Cubes::getBuffers(vector<float> *cubePositions, vector<float> *cubeNormals,
                                          vector<float> *cubeTextureCoordinates,
                                          int generatedCubeFactor) {

    mActualCubeFactor = generatedCubeFactor;

    vector<float> *cubeNormalsBuffer = new vector<float>();
    for (int i = 0;
         i < (generatedCubeFactor * generatedCubeFactor * generatedCubeFactor); i++) {
        cubeNormalsBuffer->insert(cubeNormalsBuffer->end(), cubeNormals->begin(),
                                  cubeNormals->end());
    }

    vector<float> *cubeTextureCoordinatesBuffer = new vector<float>();
    for (int i = 0;
         i < (generatedCubeFactor * generatedCubeFactor * generatedCubeFactor); i++) {
        cubeTextureCoordinatesBuffer->insert(cubeTextureCoordinatesBuffer->end(),
                                             cubeTextureCoordinates->begin(),
                                             cubeTextureCoordinates->end());
    }

    vector<vector<float> *> results;
    results.push_back(cubePositions);
    results.push_back(cubeNormalsBuffer);
    results.push_back(cubeTextureCoordinatesBuffer);

    return results;
}

vector<float> *Cubes::getInterleavedBuffer(vector<float> *cubePositions, vector<float> *cubeNormals,
                                           vector<float> *cubeTextureCoordinates,
                                           int generatedCubeFactor) {

    mActualCubeFactor = generatedCubeFactor;

    int cubePositionOffset = 0;
    int cubeNormalOffset = 0;
    int cubeTextureOffset = 0;

    vector<float> *cubeBuffer = new vector<float>();

    for (int i = 0; i < generatedCubeFactor * generatedCubeFactor * generatedCubeFactor; i++) {
        for (int v = 0; v < 36; v++) {
            cubeBuffer->insert(cubeBuffer->end(), cubePositions->begin() + cubePositionOffset,
                               cubePositions->begin() + cubePositionOffset + POSITION_DATA_SIZE);
            cubePositionOffset += POSITION_DATA_SIZE;

            cubeBuffer->insert(cubeBuffer->end(), cubeNormals->begin() + cubeNormalOffset,
                               cubeNormals->begin() + cubeNormalOffset + NORMAL_DATA_SIZE);
            cubeNormalOffset += NORMAL_DATA_SIZE;

            cubeBuffer->insert(cubeBuffer->end(),
                               cubeTextureCoordinates->begin() + cubeTextureOffset,
                               cubeTextureCoordinates->begin() + cubeTextureOffset +
                               TEXTURE_COORDINATE_DATA_SIZE);
            cubeTextureOffset += TEXTURE_COORDINATE_DATA_SIZE;
        }

        // The normal and texture data is repeated for each cube.
        cubeNormalOffset = 0;
        cubeTextureOffset = 0;
    }

    return cubeBuffer;
}


void Cubes::setPositionHandle(GLuint positionHandle) {
    Cubes::mPositionHandle = positionHandle;
}

void Cubes::setNormalHandle(GLuint normalHandle) {
    Cubes::mNormalHandle = normalHandle;
}

void Cubes::setTextureCoordinateHandle(GLuint textureCoordinateHandle) {
    Cubes::mTextureCoordinateHandle = textureCoordinateHandle;
}



