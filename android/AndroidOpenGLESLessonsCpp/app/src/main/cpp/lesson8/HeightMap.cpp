//
// Created by biezhihua on 2017/7/30.
//

#include <exception>
#include "HeightMap.h"
#include "graphics/Matrix.h"

#define LOG_TAG "Lesson8"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

const float HeightMap::MIN_POSITION = -5.0f;
const float HeightMap::POSITION_RANGE = 10.0f;

HeightMap::HeightMap() : heightMapVertexData(nullptr), heightMapIndexData(nullptr) {
//    try {
//
    const int floatsPerVertex = POSITION_DATA_SIZE_IN_ELEMENTS + NORMAL_DATA_SIZE_IN_ELEMENTS
                                + COLOR_DATA_SIZE_IN_ELEMENTS;

    const int xLength = SIZE_PER_SIZE;
    const int yLength = SIZE_PER_SIZE;

    heightMapVertexData = new vector<float>();

    // First, build the data for the vertex buffer.
    for (int y = 0; y < yLength; y++) {
        for (int x = 0; x < xLength; x++) {
            // Build our heightmap from the top down,
            // so that triangles are counter-clockwise.
            float xRatio = x / (float) (xLength - 1);
            float yRatio = 1.0f - (y / (float) (yLength - 1));

            float xPosition = MIN_POSITION + (xRatio * POSITION_RANGE);
            float yPosition = MIN_POSITION + (yRatio * POSITION_RANGE);;
            float zPosition = ((xPosition * xPosition) + (yPosition * yPosition)) / 10.0f;

            // Position
            heightMapVertexData->push_back(xPosition);
            heightMapVertexData->push_back(yPosition);
            heightMapVertexData->push_back(zPosition);

            // Cheap normal using a derivative of the function.
            // The slope for X will be 2X, for Y will be 2Y.
            // Divide by 10 since the position's Z is also divided by 10.
            float xSlope = (2 * xPosition) / 10.0f;
            float ySlope = (2 * yPosition) / 10.0f;

            // Calculate the normal using the cross product of the slopes.
            float planeVectorX[3] = {1.0f, 0.0f, xSlope};
            float planeVectorY[3] = {0.0f, 1.0, ySlope};
            float normalVector[3] = {
                    (planeVectorX[1] * planeVectorY[2]) - (planeVectorX[2] * planeVectorY[1]),
                    (planeVectorX[2] * planeVectorY[0]) - (planeVectorX[0] * planeVectorY[2]),
                    (planeVectorX[0] * planeVectorY[1]) - (planeVectorX[1] * planeVectorY[0])};

            // Normalize the normal
            float length = Matrix::length(normalVector[0], normalVector[1], normalVector[2]);

            heightMapVertexData->push_back(normalVector[0] / length);
            heightMapVertexData->push_back(normalVector[1] / length);
            heightMapVertexData->push_back(normalVector[2] / length);

            // Add some fancy colors.
            heightMapVertexData->push_back(xRatio);
            heightMapVertexData->push_back(yRatio);
            heightMapVertexData->push_back(0.5f);
            heightMapVertexData->push_back(1.0f);
        }
    }

    // Now, build the index data.
    int numStripsRequired = yLength - 1;
    int numDegensRequired = 2 * (numStripsRequired - 1);
    int verticesPerStrip = 2 * xLength;

    heightMapIndexData = new vector<short>();

    for (int y = 0; y < yLength - 1; y++) {
        if (y > 0) {
            // Degenerate begin: repeat first vertex
            heightMapIndexData->push_back((short) (y * yLength));
        }

        for (int x = 0; x < xLength; x++) {
            // One part of the strip
            heightMapIndexData->push_back((short) ((y * yLength) + x));
            heightMapIndexData->push_back((short) (((y + 1) * yLength) + x));
        }

        if (y < yLength - 2) {
            // Degenerate end: repeat last vertex
            heightMapIndexData->push_back((short) (((y + 1) * yLength) + (xLength - 1)));
        }
    }

    indexCount = (GLsizei) heightMapIndexData->size();

    glGenBuffers(1, vbo);
    glGenBuffers(1, ibo);

    if (vbo[0] > 0 && ibo[0] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, heightMapVertexData->size() * BYTES_PER_FLOAT,
                     heightMapVertexData->data(), GL_STATIC_DRAW);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, heightMapIndexData->size()
                                              * BYTES_PER_SHORT,
                     heightMapIndexData->data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

//    } catch (const std::exception &exception) {
//        LOGE("Exception %s", exception.what());
//    }
}

HeightMap::~HeightMap() {
    heightMapVertexData->clear();
    vector<float>().swap(*heightMapVertexData);

    heightMapIndexData->clear();
    vector<short>().swap(*heightMapIndexData);

    delete[] heightMapVertexData;
    delete[] heightMapIndexData;
    heightMapVertexData = nullptr;
    heightMapIndexData = nullptr;
}

void HeightMap::render() {
    if (vbo[0] > 0 && ibo[0] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

        // Bind attributes
        glVertexAttribPointer(positionAttribute,
                              POSITION_DATA_SIZE_IN_ELEMENTS, GL_FLOAT, GL_FALSE, STRIDE, 0
        );
        glEnableVertexAttribArray(positionAttribute);

        glVertexAttribPointer(normalAttribute,
                              NORMAL_DATA_SIZE_IN_ELEMENTS,
                              GL_FLOAT, GL_FALSE, STRIDE,
                              (const void *) (POSITION_DATA_SIZE_IN_ELEMENTS * BYTES_PER_FLOAT)
        );
        glEnableVertexAttribArray(normalAttribute);

        glVertexAttribPointer(colorAttribute, COLOR_DATA_SIZE_IN_ELEMENTS, GL_FLOAT, GL_FALSE,
                              STRIDE,
                              (const void *) (
                                      (POSITION_DATA_SIZE_IN_ELEMENTS +
                                       NORMAL_DATA_SIZE_IN_ELEMENTS) *
                                      BYTES_PER_FLOAT)
        );
        glEnableVertexAttribArray(colorAttribute);

        // Draw
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_SHORT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void HeightMap::release() {
    if (vbo[0] > 0) {
        glDeleteBuffers(1, vbo);
        vbo[0] = 0;
    }
    if (ibo[0] > 0) {
        glDeleteBuffers(1, ibo);
        ibo[0] = 0;
    }
}

void HeightMap::setPositionAttribute(GLuint positionAttribute) {
    HeightMap::positionAttribute = positionAttribute;
}

void HeightMap::setNormalAttribute(GLuint normalAttribute) {
    HeightMap::normalAttribute = normalAttribute;
}

void HeightMap::setColorAttribute(GLuint colorAttribute) {
    HeightMap::colorAttribute = colorAttribute;
}
