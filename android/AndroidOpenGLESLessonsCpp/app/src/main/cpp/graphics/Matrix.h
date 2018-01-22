/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */
#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
public:

    static const int MATRIX_SIZE = 16;

    float mData[MATRIX_SIZE];

    Matrix();

    Matrix(const Matrix &src);

    // Returns true if the two matrices have the same values.
    bool equals(const Matrix &src);

    // Loads this matrix with the identity matrix.
    void identity();

    // Loads this matrix with the data from src.
    void loadWith(const Matrix &src);

    // Translates this matrix by the given amounts.
    void translate(float x, float y, float z);

    // Scales this matrix by the given amounts.
    void scale(float x, float y, float z);

    // Rotates this matrix the given angle.
    void rotate(float radians, float x, float y, float z);

    // Sets this matrix to be the result of multiplying the given matrices.
    void multiply(const Matrix &l, const Matrix &r);

    void print(const char *label);

    // Returns a new matrix representing the camera.
    static Matrix *newLookAt(float eyeX, float eyeY, float eyeZ, float centerX,
                             float centerY, float centerZ, float upX, float upY, float upZ);

    // Returns a new matrix representing the perspective matrix.
    static Matrix *newFrustum(float left, float right, float bottom, float top,
                              float near, float far);

    // Returns a new matrix representing the translation.
    static Matrix *newTranslate(float x, float y, float z);

    // Returns a new matrix representing the scaling.
    static Matrix *newScale(float x, float y, float z);

    // Returns a new matrix representing the rotation.
    static Matrix *newRotate(float radians, float x, float y, float z);

    // Sets the given matrix to be the result of multiplying the given matrix by the given vector.
    static void multiplyVector(float *result, const Matrix &lhs,
                               const float *rhs);

    static void multiplyMV(float *r, const float *lhs, const float *rhs);

    /**
    * Computes the length of a vector.
    *
    * @param x x coordinate of a vector
    * @param y y coordinate of a vector
    * @param z z coordinate of a vector
    * @return the length of a vector
    */
    static float length(float x, float y, float z);
};

#endif
