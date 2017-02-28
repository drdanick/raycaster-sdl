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
void setupPlayer();
void updatePlayer();
static void movePlayer(float dx, float dy);
static int clipMovement(float dx, float dy);

#endif /* PLAYER_H */
