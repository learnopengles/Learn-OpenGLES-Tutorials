//
// Created by biezhihua on 2017/7/23.
//

#include "CubesWithVboWithStride.h"

void CubesWithVboWithStride::renderer() {
    int stride = (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + TEXTURE_COORDINATE_DATA_SIZE) *
                 BYTES_PER_FLOAT;

    // Pass in the position information
    glBindBuffer(GL_ARRAY_BUFFER, mCubeBufferIdx);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE, stride, 0);

    // Pass in the normal information
    glBindBuffer(GL_ARRAY_BUFFER, mCubeBufferIdx);
    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, NORMAL_DATA_SIZE, GL_FLOAT, GL_FALSE, stride,
                          (const GLvoid *) (POSITION_DATA_SIZE * BYTES_PER_FLOAT));

    // Pass in the texture information
    glBindBuffer(GL_ARRAY_BUFFER, mCubeBufferIdx);
    glEnableVertexAttribArray(mTextureCoordinateHandle);
    glVertexAttribPointer(mTextureCoordinateHandle, TEXTURE_COORDINATE_DATA_SIZE, GL_FLOAT,
                          GL_FALSE,
                          stride, (const GLvoid *) ((POSITION_DATA_SIZE + NORMAL_DATA_SIZE) *
                                                    BYTES_PER_FLOAT));

    // Clear the currently bound buffer (so future OpenGL calls do not use this buffer).
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Draw the mCubes.
    glDrawArrays(GL_TRIANGLES, 0, mActualCubeFactor * mActualCubeFactor * mActualCubeFactor * 36);
}

void CubesWithVboWithStride::release() {
    GLuint buffersToDelete[] = {mCubeBufferIdx};
    glDeleteBuffers(1, buffersToDelete);
}
