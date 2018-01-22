//
// Created by biezhihua on 2017/7/22.
//

#include "GenData.h"
#include "CubesClientSideWithStride.h"
#include "CubesWithVbo.h"
#include "CubesWithVboWithStride.h"
#include "CubesClientSide.h"

vector<float> GenData::CUBE_TEXTURE_COORDINATE_DATA = {
        // Front face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Right face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Back face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Left face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Top face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Bottom face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
};


vector<float> GenData::CUBE_NORMAL_DATA = {
        // Front face
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Right face
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Back face
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Left face
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // Top face
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Bottom face
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
};


vector<float> *
GenData::generatorCubeData(int requestedCubeFactor) {

    vector<float> *cubePositionData = new vector<float>();

    int cubePositionDataOffset = 0;

    int segments = requestedCubeFactor + (requestedCubeFactor - 1);
    float minPosition = -1.0f;
    float maxPosition = 1.0f;
    float positionRange = maxPosition - minPosition;

    for (int x = 0; x < requestedCubeFactor; x++) {
        for (int y = 0; y < requestedCubeFactor; y++) {
            for (int z = 0; z < requestedCubeFactor; z++) {

                float x1 = minPosition + ((positionRange / segments) * (x * 2));
                float x2 = minPosition + ((positionRange / segments) * ((x * 2) + 1));

                float y1 = minPosition + ((positionRange / segments) * (y * 2));
                float y2 = minPosition + ((positionRange / segments) * ((y * 2) + 1));

                float z1 = minPosition + ((positionRange / segments) * (z * 2));
                float z2 = minPosition + ((positionRange / segments) * ((z * 2) + 1));

                // Define points for a cube.
                // X, Y, Z
                float p1p[] = {x1, y2, z2};
                float p2p[] = {x2, y2, z2};
                float p3p[] = {x1, y1, z2};
                float p4p[] = {x2, y1, z2};
                float p5p[] = {x1, y2, z1};
                float p6p[] = {x2, y2, z1};
                float p7p[] = {x1, y1, z1};
                float p8p[] = {x2, y1, z1};

                vector<float> *thisCubePositionData = generateCubeData(p1p, p2p, p3p, p4p, p5p, p6p,
                                                                       p7p, p8p,
                                                                       (sizeof(p1p) /
                                                                        sizeof(*p1p)));

                cubePositionData->insert(cubePositionData->end(), thisCubePositionData->begin(),
                                         thisCubePositionData->end());
                cubePositionDataOffset += thisCubePositionData->size();
            }
        }
    }

    return cubePositionData;
}

vector<float> *GenData::generateCubeData(float *point1,
                                         float *point2,
                                         float *point3,
                                         float *point4,
                                         float *point5,
                                         float *point6,
                                         float *point7,
                                         float *point8,
                                         int elementsPerPoint) {

    // Given a cube with the points define as follows:
    // front left top, front right top, front left bottom,front right bottom
    // back left top, back right top, back left bottom, front right bottom
    // return an array of 6 sides, 2 triangles per side, 3 vertices per cube.

    int FRONT = 0;
    int RIGHT = 1;
    int BACK = 2;
    int LEFT = 3;
    int TOP = 4;
    int BOTTOM = 5;

    int size = elementsPerPoint * 6 * 6;

    vector<float> *cubeData = new vector<float>();

    for (int face = 0; face < 6; face++) {
        // Relative to the side,
        // p1 = top left
        // p2 = top right
        // p3 = bottom left
        // p4 = bottom right
        float *p1, *p2, *p3, *p4;

        // Select the points for this face
        if (face == FRONT) {
            p1 = point1;
            p2 = point2;
            p3 = point3;
            p4 = point4;
        } else if (face == RIGHT) {
            p1 = point2;
            p2 = point6;
            p3 = point4;
            p4 = point8;
        } else if (face == BACK) {
            p1 = point6;
            p2 = point5;
            p3 = point8;
            p4 = point7;
        } else if (face == LEFT) {
            p1 = point5;
            p2 = point1;
            p3 = point7;
            p4 = point3;
        } else if (face == TOP) {
            p1 = point5;
            p2 = point6;
            p3 = point1;
            p4 = point2;
        } else // if (face == BOTTOM)
        {
            p1 = point8;
            p2 = point7;
            p3 = point4;
            p4 = point3;
        }

        // In OpenGL counter-clockwise winding is default.
        // This means that when we look at a triangle,
        // if the points are counter-clockwise we are looking at the "front".
        // If not we are looking at the back.
        // OpenGL has an optimization where all back-facing triangles are culled, since they
        // usually represent the backside of an object and aren't visible anyways.

        // Build the triangles
        //  1---3,6
        //  | / |
        // 2,4--5
        int offset = face * elementsPerPoint * 6;

        for (int i = 0; i < elementsPerPoint; i++) {
            offset++;
            cubeData->push_back(p1[i]);
        }
        for (int i = 0; i < elementsPerPoint; i++) {
            offset++;
            cubeData->push_back(p3[i]);
        }
        for (int i = 0; i < elementsPerPoint; i++) {
            offset++;
            cubeData->push_back(p2[i]);
        }
        for (int i = 0; i < elementsPerPoint; i++) {
            offset++;
            cubeData->push_back(p3[i]);
        }
        for (int i = 0; i < elementsPerPoint; i++) {
            offset++;
            cubeData->push_back(p4[i]);
        }
        for (int i = 0; i < elementsPerPoint; i++) {
            offset++;
            cubeData->push_back(p2[i]);
        }
    }
    return cubeData;
}

void GenData::genCube(int requestedCubeFactor, bool toggleVbos, bool toggleStride) {

    if (mCubes != nullptr) {
        mCubes->release();
        delete (mCubes);
        mCubes = nullptr;
    }

    GenData::mLastRequestedCubeFactor = requestedCubeFactor;

    vector<float> *cubePositionData = generatorCubeData(requestedCubeFactor);

    bool useVBOs = mUseVBOs;
    bool useStride = mUseStride;

    if (toggleVbos) {
        useVBOs = !useVBOs;
    }

    if (toggleStride) {
        useStride = !useStride;
    }

    if (useStride) {
        if (useVBOs) {
            mCubes = new CubesWithVboWithStride(cubePositionData, &GenData::CUBE_NORMAL_DATA,
                                                &GenData::CUBE_TEXTURE_COORDINATE_DATA,
                                                requestedCubeFactor);
        } else {
            mCubes = new CubesClientSideWithStride(cubePositionData, &GenData::CUBE_NORMAL_DATA,
                                                   &GenData::CUBE_TEXTURE_COORDINATE_DATA,
                                                   requestedCubeFactor);
        }
    } else {
        if (useVBOs) {
            mCubes = new CubesWithVbo(cubePositionData, &GenData::CUBE_NORMAL_DATA,
                                      &GenData::CUBE_TEXTURE_COORDINATE_DATA,
                                      requestedCubeFactor);
        } else {
            mCubes = new CubesClientSide(cubePositionData, &GenData::CUBE_NORMAL_DATA,
                                         &GenData::CUBE_TEXTURE_COORDINATE_DATA,
                                         requestedCubeFactor);
        }
    }

    mUseVBOs = useVBOs;
    mUseStride = useStride;

    if (lesson7 != nullptr) {
        lesson7->updateVboStatus(mUseVBOs);
        lesson7->updateStrideStatus(mUseStride);
    }
}

Cubes *GenData::getCubes() const {
    return mCubes;
}


