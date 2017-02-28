/*
 * ===================================================
 * ===================================================
 *                SDL Graphics Wrapper
 *                     Revision 1
 *
 *            (C) 2015  Nick Stones-Havas
 *
 * ===================================================
 *
 *  C library for drawing to the screen.
 *
 * ===================================================
 * ===================================================
 */
#include <stdio.h>
#include "gfx.h"

/* Used to identify a texture structure in memory */
#define TEX_TAG 0x55AA

/* Error string buffer */
char errstr[256];

/*
 * SDL textures are stored in VRAM, so we need to manage
 * a RAM-persistent copy of the texture's pixel data that we
 * can write to. Allocated textures are stored in a linked
 * list so we can keep track of them all easily.
 */
typedef struct ManagedTexture_ ManagedTexture_;
struct ManagedTexture_ {
    void* pixelData; /* RAM copy of the texture */
    SDL_Texture* texture;
    Uint32 pitch;
    ManagedTexture_* next;
    ManagedTexture_* prev;
    Uint16 magicTag;
};
ManagedTexture_* managedTextures = NULL;

/* SDL Stuff */
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

unsigned int screenWidth  = -1;
unsigned int screenHeight = -1;

/*========================================================
 * Library debug functions
 *========================================================
 */

const char* gfxGetError() {
    return errstr;
}

void gfxSetError(char* str, char showSdlErr) {
    if(showSdlErr)
        sprintf(errstr, "%s: %s", str, SDL_GetError());
    else
        sprintf(errstr, "%s", str);
}

/*========================================================
 * GFX management functions
 *========================================================
 */

int initGFX(char* title, unsigned int width, unsigned int height) {
    if(window || renderer) return 0;

    screenWidth = width;
    screenHeight = height;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        gfxSetError("Error initializing SDL", 1);
        return 0;
    }

    window = SDL_CreateWindow(title, 50, 50, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if(!window || !renderer) {
        gfxSetError("Could not create SDL window", 1);
        return 0;
    }

    SDL_SetWindowTitle(window, title);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return 1;
}

void* createTexture(unsigned int width, unsigned int height) {
    Uint32* data;
    ManagedTexture_* newmtex;
    if(!width || !height || !renderer) {
        gfxSetError("SDL window has not been initialized yet", 0);
        return NULL;
    }

    newmtex = malloc(sizeof(ManagedTexture_)); if(!newmtex) { return NULL; }
    newmtex->pitch = width * sizeof(Uint32);
    newmtex->next = NULL;
    newmtex->prev = NULL;
    newmtex->magicTag  = TEX_TAG;

    newmtex->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(!(newmtex->texture)) {
        free(newmtex);
        gfxSetError("Could not create texture", 1);
        return NULL;
    }

    data = malloc((sizeof(Uint32) * width * height) + sizeof(ManagedTexture_*)); if(!data) {free(newmtex); return NULL; }

    /*
     * Hide a pointer to the managed struct before the actual pixel data.
     *
     * This allows the caller to only have to worry about the data they
     * want, while still allowing this library to perform more complicated
     * management tasks given only a reference to the pixel data.
     */
    *((ManagedTexture_**)data) = newmtex;
    newmtex->pixelData = ((ManagedTexture_**)data) + 1;

    /* Insert the item into the linked list */
    if(managedTextures) {
        newmtex->next = managedTextures;
        managedTextures->prev = newmtex;
    }
    managedTextures = newmtex;


    return newmtex->pixelData;
}

int destroyTexture(void* ptr) {
    /* Recover the memory management structure before freeing anything */
    ManagedTexture_* mtex = *(((ManagedTexture_**)ptr) - 1);

    /* Don't do anything if it's not actually a managed structure */
    if(mtex->magicTag != TEX_TAG) {
        gfxSetError("Not a valid texture pointer", 0);
        return 0;
    }

    /* Prevent this structure from being somehow sent here again */
    mtex->magicTag = 0;

    /* Actual cleanup */
    free(((ManagedTexture_**)ptr) - 1);

    SDL_DestroyTexture(mtex->texture);

    if(mtex->prev) mtex->prev->next = mtex->next;
    if(mtex->next) mtex->next->prev = mtex->prev;
    if(managedTextures == mtex) managedTextures = mtex->next;
    free(mtex);

    return 1;
}

void displayFullscreenTexture(void* texture) {
    ManagedTexture_* mtex;

    if(!window || !renderer) {
        gfxSetError("SDL window has not been initialized yet", 0);
        return;
    }

    /* Recover the managed texture structure */
    mtex = *(((ManagedTexture_**)texture) - 1);

    /* Don't do anything if it's not actually a managed texture */
    if(mtex->magicTag != TEX_TAG) {
        gfxSetError("Not a valid texture pointer", 0);
        return;
    }

    SDL_UpdateTexture(mtex->texture, NULL, mtex->pixelData, mtex->pitch);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, mtex->texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}


void destroyGFX() {
    /* Destroy all allocated textures */
    while(managedTextures) destroyTexture(managedTextures->pixelData);

    /* Clean everything else up */
    if(window && renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();
    }
}


/*========================================================
 * Drawing functions
 *========================================================
 */

void setDrawColor(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void drawLine(int x1, int y1, int x2, int y2) {
    int xOffset = 0;
    int yOffset = 0;

    if (x2 - x1 > 0) xOffset = -1;
    if (y2 - y1 > 0) yOffset = -1;

    SDL_RenderDrawLine(renderer, x1, y1, x2 + xOffset, y2 + yOffset);
}

void fillRect(int x, int y, int w, int h) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(int x, int y, int w, int h) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_RenderDrawRect(renderer, &rect);
}

void presentRenderer() {
    SDL_RenderPresent(renderer);
}

void clearRenderer() {
    SDL_RenderClear(renderer);
}

/*========================================================
 * Utility functions
 *========================================================
 */

Uint32* generateXorTexture(int size, int redmask, int greenmask, int bluemask) {
    int x, y;
    float factor = 256.0f / (float)size;
    Uint32* texture = createTexture(size, size);

    for(x = 0; x < size; x++)
        for(y = 0; y < size; y++)
            texture[(size * y) + x] = RGBtoABGR((int)((x ^ y) * factor) & redmask, (int)((x ^ y) * factor) & greenmask, (int)((x ^ y) * factor) & bluemask);

    return texture;
}

Uint32* generateRedXorTexture(int size) {
    return generateXorTexture(size, 0xFF, 0x00, 0x00);
}

Uint32* generateGreenXorTexture(int size) {
    return generateXorTexture(size, 0x00, 0xFF, 0x00);
}

Uint32* generateBlueXorTexture(int size) {
    return generateXorTexture(size, 0x00, 0x00, 0xFF);
}

Uint32* generateGrayXorTexture(int size) {
    return generateXorTexture(size, 0xFF, 0xFF, 0xFF);
}
