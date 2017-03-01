#ifndef PLAYER_H
#define PLAYER_H

#include "linalg.h"

/* Global data */
extern Vector3f playerPos;
extern Vector3f playerDir;

/* Global toggles */
extern char movingForward;
extern char movingBack;
extern char turningLeft;
extern char turningRight;
extern char playerIsRunning;

/* Functions */

/**
 * Initialize the player.
 */
void initPlayer();

/**
 * Update the player for the current frame.
 */
void updatePlayer();

/**
 * Move the player by a given movement vector.
 *
 * dx: The x component of the movement vector.
 * dy: The y component of the movement vector.
 */
void movePlayer(float dx, float dy);

/**
 * Check if a given movement vector intersects with the world
 * and should be clipped.
 *
 * dx: The x component of the movement vector to check.
 * dy: The y component of the movement vector to check.
 *
 * Returns: Zero if the vector should not be clipped, non-zero otherwise.
 */
int clipMovement(float dx, float dy);

#endif /* PLAYER_H */
