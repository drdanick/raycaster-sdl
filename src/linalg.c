
#include "linalg.h"

Vector3f vectorAdd(Vector3f* vec1, Vector3f* vec2) {
    Vector3f retVec = {vec1->x + vec2->x, vec1->y + vec2->y, 1};
    return retVec;
}

Vector3f vectorSubtract(Vector3f* vec1, Vector3f* vec2) {
    Vector3f retVec = {vec1->x - vec2->x, vec1->y - vec2->y, 1};
    return retVec;
}

Vector3f homogeneousVectorScale(Vector3f* vec, float scalar) {
    Vector3f retVec = {vec->x * scalar, vec->y * scalar, 1};
    return retVec;
}

Vector3f normalizeVector(Vector3f* vec) {
    return homogeneousVectorScale(vec, 1.0f/homogeneousVectorMagnitude(vec));
}

Vector3f vectorProjection(Vector3f* vec1, Vector3f* vec2) {
    Vector3f pvec = normalizeVector(vec2);

    return homogeneousVectorScale(&pvec, vectorDotProduct(&pvec, vec1));
}

float homogeneousVectorMagnitude(Vector3f* vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

float vectorDotProduct(Vector3f* vec1, Vector3f* vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y;
}


void matrixVectorMultiply(Matrix3f* mat, Vector3f* vec) {
    Vector3f newVec;
    newVec.x = (*mat)[0][0] * vec->x + (*mat)[0][1] * vec->y + (*mat)[0][2] * vec->z;
    newVec.y = (*mat)[1][0] * vec->x + (*mat)[1][1] * vec->y + (*mat)[1][2] * vec->z;
    newVec.z = (*mat)[2][0] * vec->x + (*mat)[2][1] * vec->y + (*mat)[2][2] * vec->z;

    *vec = newVec;
}

void matrixMatrixMultiply(Matrix3f* mat1, Matrix3f* mat2) {
    int row, col;
    Matrix3f retMat;

    for(row = 0; row < 3; row++) {
        for(col = 0; col < 3; col++) {
            retMat[row][col] =
                (*mat1)[row][0] * (*mat2)[col][0] +
                (*mat1)[row][1] * (*mat2)[col][1] +
                (*mat1)[row][2] * (*mat2)[col][2];
        }
    }

    matrix3fCopy(mat1, &retMat);
}

void matrix3fCopy(Matrix3f* dst, Matrix3f* src) {
    int row, col;

    for(row = 0; row < 3; row++) {
        for(col = 0; col < 3; col++) {
            (*dst)[row][col] = (*src)[row][col];
        }
    }
}
