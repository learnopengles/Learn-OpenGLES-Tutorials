#include "CubesClientSideWithStride.h"

void CubesClientSideWithStride::renderer() {
    int stride = (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + TEXTURE_COORDINATE_DATA_SIZE) *
                 BYTES_PER_FLOAT;

    // Pass in the position information
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, Cubes::POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE, stride,
                          mCubeBuffer->data());


    // Pass in the normal information
    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, Cubes::NORMAL_DATA_SIZE, GL_FLOAT, GL_FALSE, stride,
                          mCubeBuffer->data() + Cubes::POSITION_DATA_SIZE);

    // Pass in the texture information
    glEnableVertexAttribArray(mTextureCoordinateHandle);
    glVertexAttribPointer(mTextureCoordinateHandle, TEXTURE_COORDINATE_DATA_SIZE, GL_FLOAT,
                          GL_FALSE,
                          stride, mCubeBuffer->data() + Cubes::POSITION_DATA_SIZE +
                                  Cubes::NORMAL_DATA_SIZE);

    // Draw the mCubes.
    glDrawArrays(GL_TRIANGLES, 0, mActualCubeFactor * mActualCubeFactor * mActualCubeFactor * 36);

}

void CubesClientSideWithStride::release() {
    if (mCubeBuffer != nullptr) {
        mCubeBuffer->clear();
        vector<float>().swap(*mCubeBuffer);
    }
}
