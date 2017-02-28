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
#ifndef GFX_H
#define GFX_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif


/*========================================================
 * Macros
 *========================================================
 */

/**
 * Macro to convert from RGB color components to an ABGR integer
 */
#define RGBtoABGR(R,G,B)   (0xFF000000 | ((B) << 16) | ((G) << 8) | (R))


/*========================================================
 * Library debug functions
 *========================================================
 */

/* Get a string describing the last error encountered by this library */
const char* gfxGetError();


/*========================================================
 * GFX management functions
 *========================================================
 */


/**
 * Initialize the graphics environment
 *
 * title:  The title to use for the displayed window
 * width:  The width of the displayed window in pixels
 * height: The height of the displayed window in pixels
 */
int initGFX(char* title, unsigned int width, unsigned int height);

/**
 * Create a texture buffer
 *
 * width:  The width of the texture buffer
 * height: The height of the texture buffer
 *
 * Returns: A pointer to the texture pixel buffer
 */
void* createTexture(unsigned int width, unsigned int height);

/**
 * Free a texture buffer from memory
 *
 * texture: A pointer to the texture to be destroyed
 *
 * Returns: 1 if the operation was successful, 0 otherwise.
 */
int destroyTexture(void* texture);

/**
 * Draw a texture to the window's entire rendering area.
 *
 * texture: A pointer to the texture to be drawm
 *
 * Returns: 1 if the operation was successful, 0 otherwise.
 */
void displayFullscreenTexture(void* texture);

/**
 * Terminate the graphics environment and free all allocated resources
 */
void destroyGFX();


/*========================================================
 * Drawing functions
 *========================================================
 */

/**
 * Set the current drawing color for primitives
 *
 * r: The red channel of the color
 * g: The green channel of the color
 * b: The blue channel of the color
 * a: The alpha (blending) channel of the color
 */
void setDrawColor(int r, int g, int b, int a);

/**
 * Draw a line between two points on the screen
 *
 * x1: The starting point's x component
 * y1: The starting point's y component
 * x2: The finishing point's x component
 * y2: The finishing point's y component
 */
void drawLine(int x1, int y1, int x2, int y2);

/**
 * Draw a filled rectangle to the screen
 *
 * x: The x component of the top-left corner of the rectangle
 * y: The y component of the top-left corner of the rectangle
 * w: The width of the rectangle
 * h: The height of the rectangle
 */
void fillRect(int x, int y, int w, int h);

/**
 * Draw the outline of a rectangle to the screen
 *
 * x: The x component of the top-left corner of the rectangle
 * y: The y component of the top-left corner of the rectangle
 * w: The width of the rectangle
 * h: The height of the rectangle
 */
void drawRect(int x, int y, int w, int h);

/**
 * Refresh the primitive objects on the screen
 */
void presentRenderer();

/**
 * Clear the primitive objects on the screen
 */
void clearRenderer();


/*========================================================
 * Utility functions
 *========================================================
 */

/**
 * Generate an xor square texture
 *
 * size:      The size of the square texture in pixels
 * redmask:   The bitwise mask used on the red channel when picking colors to use
 * greenmask: The bitwise mask used on the green channel when picking colors to use
 * bluemask:  The bitwise mask used on the blue channel when picking colors to use
 *
 * Returns: A pointer to the generated texture
 */
Uint32* generateXorTexture(int size, int redmask, int greenmask, int bluemask);

/**
 * Generate a red xor square texture
 *
 * size: The size of the square texture in pixels
 *
 * Returns: A pointer to the generated texture
 */
Uint32* generateRedXorTexture(int size);

/**
 * Generate a green xor square texture
 *
 * size: The size of the square texture in pixels
 *
 * Returns: A pointer to the generated texture
 */
Uint32* generateGreenXorTexture(int size);

/**
 * Generate a blue xor square texture
 *
 * size: The size of the square texture in pixels
 *
 * Returns: A pointer to the generated texture
 */
Uint32* generateBlueXorTexture(int size);

/**
 * Generate a gray xor square texture
 *
 * size: The size of the square texture in pixels
 *
 * Returns: A pointer to the generated texture
 */
Uint32* generateGrayXorTexture(int size);


#endif /* GFX_H */
