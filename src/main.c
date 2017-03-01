#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "raycaster.h"
#include "renderer.h"
#include "player.h"
#include "map.h"

const short MAP[MAP_GRID_HEIGHT][MAP_GRID_WIDTH] = {
    {R,R,R,R,R,R,R,R,R,R},
    {R,B,0,0,0,0,P,0,B,R},
    {R,0,0,0,0,0,0,0,0,R},
    {R,0,0,G,0,0,G,0,0,R},
    {R,0,0,0,0,0,0,0,0,R},
    {R,0,0,0,0,0,0,0,0,R},
    {R,0,0,G,0,0,G,0,0,R},
    {R,0,0,0,0,0,0,0,0,R},
    {R,B,0,0,0,0,0,0,B,R},
    {R,R,R,R,R,R,R,R,R,R}
};


/* Program globals */
Uint32* screenBuffer    = NULL;
Uint32* redXorTexture   = NULL;
Uint32* greenXorTexture = NULL;
Uint32* blueXorTexture  = NULL;
Uint32* grayXorTexture  = NULL;

const Uint32 COLORS[4] = {
    RGBtoABGR(255, 0, 0),
    RGBtoABGR(0, 255, 0),
    RGBtoABGR(0, 0, 255),
    RGBtoABGR(128, 128, 128)
};

Uint32* TEXTURES[4];

/* Program toggles */
char gameIsRunning    = TRUE;
char showMap          = TRUE;
char distortion       = FALSE;
char slowRenderMode   = FALSE;
char rayCastMode      = 0;
char textureMode      = 0;

void render() {
    if(showMap) {
        clearRenderer();
        renderOverheadMap();
    } else { /* Draw projected scene */
        renderProjectedScene();
    }
}

void consumeSDLEvents() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        movingForward = (event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_DOWN:
                        movingBack = (event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_LEFT:
                        turningLeft = (event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_RIGHT:
                        turningRight = (event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        playerIsRunning = (event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_ESCAPE:
                        gameIsRunning = FALSE;
                        break;
                    case SDLK_t:
                        if(event.type == SDL_KEYDOWN) textureMode = (textureMode + 1) % 2;
                        break;
                    case SDLK_m:
                        if(event.type == SDL_KEYDOWN) showMap = !showMap;
                        break;
                    case SDLK_f:
                        if(event.type == SDL_KEYDOWN) distortion = !distortion;
                        break;
                    case SDLK_r:
                        if(event.type == SDL_KEYDOWN) slowRenderMode = !slowRenderMode;
                        break;
                    case SDLK_c:
                        if(event.type == SDL_KEYDOWN) rayCastMode = (rayCastMode + 1) % 3;
                        break;
                    case SDLK_LEFTBRACKET:
                        if(event.type == SDL_KEYDOWN && distFromViewplane - 20.0f > 100.0f) distFromViewplane -= 20.0f;
                        break;
                    case SDLK_RIGHTBRACKET:
                        if(event.type == SDL_KEYDOWN) distFromViewplane += 20.0f;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                gameIsRunning = FALSE;
                break;
            default:
                break;
        }
    }
}

void runGame() {
    long gameTicks = 0;
    long time;

    do {
        time = SDL_GetTicks();

        /* Handle SDL key events */
        consumeSDLEvents();

        /* Update the player */
        updatePlayer();

        /* Update the raycaster */
        updateRaycaster();

        /* Render a frame */
        render();

        /* Fixed delay before next frame */
        SDL_Delay(10);

        /* Print FPS every 500 frames */
        if(!(gameTicks++ % 500))
            fprintf(stderr, "FPS: %.2f\n", 1000.0f / (float)(SDL_GetTicks() - time));
    } while(gameIsRunning);
}

int setupWindow() {
    int x, y;

    if(!initGFX("Raycaster", WINDOW_WIDTH, WINDOW_HEIGHT)) return FALSE;

    screenBuffer = createTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    redXorTexture = generateRedXorTexture(TEXTURE_SIZE);
    greenXorTexture = generateGreenXorTexture(TEXTURE_SIZE);
    blueXorTexture = generateBlueXorTexture(TEXTURE_SIZE);
    grayXorTexture = generateGrayXorTexture(TEXTURE_SIZE);
    TEXTURES[0] = redXorTexture;
    TEXTURES[1] = greenXorTexture;
    TEXTURES[2] = blueXorTexture;
    TEXTURES[3] = grayXorTexture;

    if(!screenBuffer) return FALSE;

    /* Make the texture initially gray */
    for(x = 0; x < WINDOW_WIDTH; x++)
        for(y = 0; y < WINDOW_HEIGHT; y++)
            screenBuffer[(WINDOW_WIDTH * y) + x] = 0xFFAAAAAA;

    return TRUE;
}

int main(int argc, char** argv) {
    if(!setupWindow()) {
        fprintf(stderr, "Could not initialize raycaster!\n");
        return EXIT_FAILURE;
    }
    initPlayer();
    initRaycaster();
    runGame();

    destroyGFX();
    return EXIT_SUCCESS;
}
