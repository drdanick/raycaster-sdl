#include <stdio.h>

#include "config.h"
#include "raycaster.h"
#include "player.h"

/* Globals */
Vector3f viewplaneDir = {VIEWPLANE_DIR_X, VIEWPLANE_DIR_Y, 1};
float distFromViewplane;
Matrix3f counterClockwiseRotation = IDENTITY_M;
Matrix3f clockwiseRotation = IDENTITY_M;
RayTuple rays[VIEWPLANE_LENGTH];


void initializeRayDirections() {
    int i;
    Vector3f v1,v2,v3;

    for(i = 0; i < VIEWPLANE_LENGTH; i++) {
        v1 = homogeneousVectorScale(&playerDir, distFromViewplane);
        v2 = homogeneousVectorScale(&viewplaneDir, ((VIEWPLANE_LENGTH / 2) - i));
        v3 = vectorSubtract(&v1, &v2);
        rays[i].hRay = normalizeVector(&v3);
        rays[i].vRay = normalizeVector(&v3);

        if (rayCastMode == ONLY_NORMALIZED) {
            rays[i].hRay = homogeneousVectorScale(&rays[i].hRay, 40);
            rays[i].vRay = homogeneousVectorScale(&rays[i].vRay, 40);
        }
    }
}

void extendRaysToFirstHit(RayTuple* rays) {
    int i;

    for(i = 0; i < VIEWPLANE_LENGTH; i++) {
        Vector3f perpVec = HOMOGENEOUS_V3;

        /* Extend vertical ray */
        if(rays[i].vRay.x < 0) { /* Ray is facing left */
            perpVec.x = ((int)(playerPos.x / (float)WALL_SIZE)) * WALL_SIZE - playerPos.x;
        } else { /* Ray is facing right */
            perpVec.x = ((int)(playerPos.x / (float)WALL_SIZE)) * WALL_SIZE - playerPos.x + WALL_SIZE;
        }
        rays[i].vRay = homogeneousVectorScale(&rays[i].vRay, vectorDotProduct(&perpVec, &perpVec) / MAKE_FLOAT_NONZERO(vectorDotProduct(&perpVec, &rays[i].vRay)));

        /* Extend horizontal ray */
        perpVec.x = 0.0f;
        if(rays[i].hRay.y < 0) { /* Ray is facing up */
            perpVec.y = ((int)(playerPos.y / (float)WALL_SIZE)) * WALL_SIZE - playerPos.y;
        } else { /* Ray is facing down */
            perpVec.y = ((int)(playerPos.y / (float)WALL_SIZE)) * WALL_SIZE - playerPos.y + WALL_SIZE;
        }
        rays[i].hRay = homogeneousVectorScale(&rays[i].hRay, vectorDotProduct(&perpVec, &perpVec) / MAKE_FLOAT_NONZERO(vectorDotProduct(&perpVec, &rays[i].hRay)));
    }

}

Vector3f findVerticalRayStepVector(Vector3f* ray) {
    Vector3f stepVector = HOMOGENEOUS_V3;
    if(ray->x < 0) { /* Ray is facing left */
        stepVector.x = -1 * WALL_SIZE;

    } else { /* Ray is facing right */
        stepVector.x = WALL_SIZE;
    }

    return homogeneousVectorScale(ray, vectorDotProduct(&stepVector, &stepVector) / MAKE_FLOAT_NONZERO(vectorDotProduct(&stepVector, ray)));
}

Vector3f findHorizontalRayStepVector(Vector3f* ray) {
    Vector3f stepVector = HOMOGENEOUS_V3;
    if(ray->y < 0) { /* Ray is facing up */
        stepVector.y = -1 * WALL_SIZE;

    } else { /* Ray is facing down */
        stepVector.y = WALL_SIZE;
    }

    return homogeneousVectorScale(ray, vectorDotProduct(&stepVector, &stepVector) / MAKE_FLOAT_NONZERO(vectorDotProduct(&stepVector, ray)));
}

void raycast(RayTuple* rays) {
    int i;

    for(i = 0; i < VIEWPLANE_LENGTH; i++) {
        Vector3f vnorm = normalizeVector(&rays[i].vRay);
        Vector3f hnorm = normalizeVector(&rays[i].hRay);
        Vector3f vstep = findVerticalRayStepVector(&vnorm);
        Vector3f hstep = findHorizontalRayStepVector(&hnorm);
        Vector3f mapCoord;

        /* Cast the vertical ray until it hits something */
        mapCoord = getTileCoordinateForVerticalRay(&rays[i].vRay);
        while(mapCoord.x > 0 && mapCoord.y > 0 && mapCoord.x < MAP_GRID_WIDTH && mapCoord.y < MAP_GRID_HEIGHT && MAP[(int)mapCoord.y][(int)mapCoord.x] < 1) {
            rays[i].vRay = vectorAdd(&rays[i].vRay, &vstep);
            mapCoord = getTileCoordinateForVerticalRay(&rays[i].vRay);
        }

        /* Cast the horizontal ray until it hits something */
        mapCoord = getTileCoordinateForHorizontalRay(&rays[i].hRay);
        while(mapCoord.x > 0 && mapCoord.y > 0 && mapCoord.x < MAP_GRID_WIDTH && mapCoord.y < MAP_GRID_HEIGHT && MAP[(int)mapCoord.y][(int)mapCoord.x] < 1) {
            rays[i].hRay = vectorAdd(&rays[i].hRay, &hstep);
            mapCoord = getTileCoordinateForHorizontalRay(&rays[i].hRay);
        }
    }
}

void updateRaycaster() {

    /* Update the rays */
    initializeRayDirections();

    if (rayCastMode == ONLY_NORMALIZED)
        return;

    /* Extend the rays to their first hits */
    extendRaysToFirstHit(rays);
    if (rayCastMode == ONLY_FIRST_HIT)
        return;

    /* Perform raycasting */
    raycast(rays);

}

Vector3f getTileCoordinateForVerticalRay(Vector3f* ray) {
    Vector3f pos = vectorAdd(&playerPos, ray);
    Vector3f coord;
    coord.x = (int)(pos.x + ((ray->x < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE;
    coord.y = (int)(pos.y + ((ray->y < 0) ? (-1 * EPS) : (EPS))) / WALL_SIZE;

    return coord;
}

Vector3f getTileCoordinateForHorizontalRay(Vector3f* ray) {
    Vector3f pos = vectorAdd(&playerPos, ray);
    Vector3f coord;
    coord.x = (int)(pos.x + ((ray->x < 0) ? (-1 * EPS) : EPS)) / WALL_SIZE;
    coord.y = (int)(pos.y + ((ray->y < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE;

    return coord;
}

void initRaycaster() {

    /* Infer viewplane distance from a given field of view angle */
    distFromViewplane = (WINDOW_WIDTH / 2.0f) / (float)(tan(FOV / 2.0f));

    /* Setup player rotation matrices */
    counterClockwiseRotation[0][0] = cos(PLAYER_ROT_SPEED);
    counterClockwiseRotation[0][1] = -1.0f * sin(PLAYER_ROT_SPEED);
    counterClockwiseRotation[1][0] = sin(PLAYER_ROT_SPEED);
    counterClockwiseRotation[1][1] = cos(PLAYER_ROT_SPEED);

    clockwiseRotation[0][0] = cos(-1.0f * PLAYER_ROT_SPEED);
    clockwiseRotation[0][1] = -1.0f * sin(-1.0f * PLAYER_ROT_SPEED);
    clockwiseRotation[1][0] = sin(-1.0f * PLAYER_ROT_SPEED);
    clockwiseRotation[1][1] = cos(-1.0f * PLAYER_ROT_SPEED);
}
