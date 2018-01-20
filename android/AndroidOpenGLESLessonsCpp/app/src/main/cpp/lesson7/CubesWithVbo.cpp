//
// Created by biezhihua on 2017/7/23.
//

#include "CubesWithVbo.h"

void CubesWithVbo::renderer() {
    // Pass in the position information
    glBindBuffer(GL_ARRAY_BUFFER, mCubePositionsBufferIdx);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, Cubes::POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the normal information
    glBindBuffer(GL_ARRAY_BUFFER, mCubeNormalsBufferIdx);
    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, Cubes::NORMAL_DATA_SIZE, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the texture information
    glBindBuffer(GL_ARRAY_BUFFER, mCubeTexCoordsBufferIdx);
    glEnableVertexAttribArray(mTextureCoordinateHandle);
    glVertexAttribPointer(mTextureCoordinateHandle, Cubes::TEXTURE_COORDINATE_DATA_SIZE, GL_FLOAT,
                          GL_FALSE,
                          0, 0);

    // Clear the currently bound buffer (so future OpenGL calls do not use this buffer).
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Draw the mCubes.
    glDrawArrays(GL_TRIANGLES, 0, mActualCubeFactor * mActualCubeFactor * mActualCubeFactor * 36);

}

void CubesWithVbo::release() {
    GLuint buffersToDelete[] = {mCubePositionsBufferIdx, mCubeNormalsBufferIdx,
                                mCubeTexCoordsBufferIdx};
    glDeleteBuffers(3, buffersToDelete);
}
