#ifndef LINALG_H
#define LINALG_H

#include <math.h>

#define MIN(A, B)      ((A) < (B) ? (A) : (B))
#define MAX(A, B)      ((A) > (B) ? (A) : (B))

#define IDENTITY_M     {{1,0,0},{0,1,0},{0,0,1}}
#define HOMOGENEOUS_V3 {0, 0, 1}

/* Types */
typedef struct {
    float x;
    float y;
    float z;
} Vector3f;

typedef float Matrix3f[3][3];

Vector3f vectorAdd(Vector3f* vec1, Vector3f* vec2);
Vector3f vectorSubtract(Vector3f* vec1, Vector3f* vec2);
Vector3f homogeneousVectorScale(Vector3f* vec, float scalar);
Vector3f normalizeVector(Vector3f* vec);
Vector3f vectorProjection(Vector3f* vec1, Vector3f* vec2);
float homogeneousVectorMagnitude(Vector3f* vec);
float vectorDotProduct(Vector3f* vec1, Vector3f* vec2);

void matrixVectorMultiply(Matrix3f* mat, Vector3f* vec);
void matrixMatrixMultiply(Matrix3f* mat1, Matrix3f* mat2);

void    matrix3fCopy(Matrix3f* dst, Matrix3f* src);


#endif /* LINALG_H */
