#ifndef LINALG_H
#define LINALG_H

#include <math.h>

#define MIN(A, B)      ((A) < (B) ? (A) : (B))
#define MAX(A, B)      ((A) > (B) ? (A) : (B))

/* A 3D identity matrix */
#define IDENTITY_M     {{1,0,0},{0,1,0},{0,0,1}}

/* A 2D homogeneous matrix */
#define HOMOGENEOUS_V3 {0, 0, 1}

/* Types */
typedef struct {
    float x;
    float y;
    float z;
} Vector3f;

typedef float Matrix3f[3][3];

/**
 * Add two 2D homogeneous vectors.
 * Since the output is homogeneous, its z component
 * is fixed to '1'.
 *
 * vec1: The first vector.
 * vec2: The second vector.
 *
 * Returns: The addition of vec1 and vec2.
 */
Vector3f vectorAdd(Vector3f* vec1, Vector3f* vec2);

/**
 * Subtract one 2D homogeneous vector from another.
 * Since the output is homogeneous, its z component
 * is fixed to '1'.
 *
 * vec1: The first vector.
 * vec2: The second vector.
 *
 * Returns: The subtraction of vec2 from vec1 (vec1 - vec2).
 */
Vector3f vectorSubtract(Vector3f* vec1, Vector3f* vec2);

/**
 * Scale a homogeneous 2D vector by a scalar.
 * Since the output is homogeneous, its z component
 * is fixed to '1'.
 *
 * vec:    The vector to scale.
 * scalar: The scalar to use.
 *
 * Returns: The scaled vector.
 */
Vector3f homogeneousVectorScale(Vector3f* vec, float scalar);

/**
 * Normalize a homogeneous 2D vector (set its length to 1)
 * Since the output is homogeneous, its z component
 * is fixed to '1'.
 *
 * vec: The vector to normalize.
 *
 * Returns: The normalized vector.
 */
Vector3f normalizeVector(Vector3f* vec);

/**
 * Project a 2D vector onto another.
 * Since the output is homogeneous, its z component
 * is fixed to '1'.
 *
 * vec1: The vector to project.
 * vec2: The vector to project on to.
 *
 * Returns: The projected vector.
 */
Vector3f vectorProjection(Vector3f* vec1, Vector3f* vec2);

/**
 * Get the magnitude of a homogeneous 2D vector.
 *
 * vec: The vector to find the magnitude of.
 *
 * Returns: The magnitude (length) of the vector.
 */
float homogeneousVectorMagnitude(Vector3f* vec);

/**
 * Find the dot product of two homogeneous 2D vectors.
 *
 * vec1: The first vector.
 * vec2: The second vector.
 *
 * Returns: Dot product of the first and second vectors.
 */
float vectorDotProduct(Vector3f* vec1, Vector3f* vec2);

/**
 * Multiply a 3D vector by a 3D square matrix.
 *
 * mat: The transformation matrix.
 * vec: The vector to be multiplied.
 */
void matrixVectorMultiply(Matrix3f* mat, Vector3f* vec);

/**
 * Multiply a matrix by another matrix.
 * The first matrix will hold the result.
 *
 * mat1: The first matrix.
 * mat2: The second matrix.
 */
void matrixMatrixMultiply(Matrix3f* mat1, Matrix3f* mat2);

/**
 * Copy the values of one matrix into another.
 *
 * dst: The destination matrix.
 * src: The source matrix.
 */
void matrix3fCopy(Matrix3f* dst, Matrix3f* src);

#endif /* LINALG_H */
