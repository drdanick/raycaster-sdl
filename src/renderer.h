#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.h"
#include "linalg.h"

/* Macros */
#define XY_TO_SCREEN_INDEX(X, Y)   (((Y) * WINDOW_WIDTH) + (X))
#define XY_TO_TEXTURE_INDEX(X, Y)   (((Y) * TEXTURE_SIZE) + (X))
#define DARKEN_COLOR(C)     ((((C) >> 1) & 0x7F7F7F7F) | 0xFF000000)

/* Enums */
typedef enum {HORIZONTAL_RAY, VERTICAL_RAY} RayType;

/* Functions */

/**
 * Calculate the draw height of a pixel column for a given
 * ray length.
 *
 * rayLength: The ray length to use.
 *
 * Returns: The pixel height of a vertical column to draw.
 */
float calculateDrawHeight(float rayLength);

/**
 * Draw a textured pixel column on the screen.
 *
 * x:          The x coordinate of the column.
 * wallYStart: The starting y coordinate of the pixel column.
 * length:     The length of the column.
 * textureX:   The texture column number to use for the strip.
 * texture:    The texture to use.
 * darken:     Non-zero if the strip should be darkened, zero otherwise.
 */
void drawTexturedStrip(int x, float wallYStart, float length, int textureX, Uint32* texture, char darken);

/**
 * Draw an un-textured pixel column on the screen.
 *
 * x:          The x coordinate of the column.
 * wallYStart: The starting y coordinate of the pixel column.
 * length:     The length of the column.
 * ABGRColor:  The color (ABGR) to use.
 * darken:     Non-zero if the strip should be darkened, zero otherwise.
 */
void drawUntexturedStrip(int x, float wallYStart, float length, Uint32 ABGRColor, char darken);

/**
 * Find the texture column number to use for a given ray.
 *
 * ray:   The ray to use.
 * rtype: The type of ray intersection (see above definition of RayType)
 *
 * Returns: The texture column number to use.
 */
int getTextureColumnNumberForRay(Vector3f* ray, RayType rtype);

/**
 * Get the barrel-distortion corrected ray length for a given ray.
 *
 * ray: The ray to undistort.
 *
 * Returns: The undistorted length of the ray.
 */
float getUndistortedRayLength(Vector3f* ray);

/**
 * Render the scene.
 * This assumes that rays have already been cast.
 */
void renderProjectedScene();

#endif /* RENDERER_H */
