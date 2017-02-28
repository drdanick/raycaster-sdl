
#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "config.h"
#include "linalg.h"

/* Constants */
#define RAY_EPS   (WALL_SIZE / 3.0f)

/* Datatypes */
typedef struct {
    Vector3f vRay;
    Vector3f hRay;
} RayTuple;

/* Global data */
extern Vector3f viewplaneDir;
extern float distFromViewplane;
extern Matrix3f counterClockwiseRotation;
extern Matrix3f clockwiseRotation;
extern RayTuple rays[VIEWPLANE_LENGTH];

/* Functions */
static void initializeRayDirections();
static void extendRaysToFirstHit(RayTuple* rays);
static Vector3f findVerticalRayStepVector(Vector3f* ray);
static Vector3f findHorizontalRayStepVector(Vector3f* ray);
static void raycast(RayTuple* rays);
Vector3f getTileCoordinateForVerticalRay(Vector3f* ray);
Vector3f getTileCoordinateForHorizontalRay(Vector3f* ray);
void updateRaycaster();
void setupRaycaster();



#endif /* RAYCASTER_H */
