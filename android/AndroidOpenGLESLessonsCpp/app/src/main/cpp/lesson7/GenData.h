//
// Created by biezhihua on 2017/7/22.
//

#ifndef OPENGLLESSON_GENDATA_H
#define OPENGLLESSON_GENDATA_H

#include <iostream>
#include <vector>
#include "Cubes.h"
#include "Native7Lesson.h"

using namespace std;

class GenData {

public:

    int mLastRequestedCubeFactor = 3;

    bool mUseVBOs = false;

    bool mUseStride = false;

private:

    Native7Lesson *lesson7;

    Cubes *mCubes;

    // X, Y, Z
    // The normal is used in light calculations and is a vector which points
    // orthogonal to the plane of the surface. For a cube model, the normals
    // should be orthogonal to the points of each face.
    static vector<float> CUBE_NORMAL_DATA;

    // S, T (or X, Y)
    // Texture coordinate data.
    // Because images have a Y axis pointing downward (values increase as you move down the image) while
    // OpenGL has a Y axis pointing upward, we adjust for that here by flipping the Y axis.
    // What's more is that the texture coordinates are the same for every face.
    static vector<float> CUBE_TEXTURE_COORDINATE_DATA;

    vector<float> *
    generateCubeData(float *point1, float *point2, float *point3, float *point4, float *point5,
                     float *point6, float *point7, float *point8, int elementsPerPoint);

    vector<float> *generatorCubeData(int requestedCubeFactor);

public:
    GenData(Native7Lesson *pLesson) : lesson7(pLesson) {
        mCubes = nullptr;
    }

    ~GenData() {
        lesson7 = nullptr;
        if (mCubes != nullptr) {
            mCubes->release();
            delete (mCubes);
            mCubes = nullptr;
        }
    }

    Cubes *getCubes() const;

    void genCube(int requestedCubeFactor, bool toggleVbos, bool toggleStride);
};


#endif //INC_21CPLUSPLUS_GENDATARUNNABLE_H
