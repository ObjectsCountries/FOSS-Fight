#pragma once

#include <SDL3/SDL.h>

/**
 * Changes the coordinates of a @c SDL_FRect* .
 * @param rect The rectangle to modify.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 * @param width The new width.
 * @param height The new height.
 */
void setCoordinatesRect(SDL_FRect*& rect, float x, float y, float width, float height);

/**
 * Changes the scale of a @c SDL_FRect* .
 * @param rect The rectangle to change the scaling of.
 * @param factor The factor of scaling.
 */
void multiplySizeRect(SDL_FRect*& rect, float factor);

/**
 * Changes the location of a @c SDL_FRect* .
 * @param rect The rectangle to change the location of.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 */
void changeLocationRect(SDL_FRect*& rect, float x, float y);

/**
 * Changes the dimensions of a @c SDL_FRect* .
 * @param rect The rectangle to change the dimensions of.
 * @param width The new width.
 * @param height The new height.
 */
void changeDimensionsRect(SDL_FRect*& rect, float width, float height);

/**
 * Moves a @c SDL_FRect* .
 * @param rect The rectangle to move.
 * @param dx The change in x-coordinate.
 * @param dy The change in y-coordinate.
 */
void moveRect(SDL_FRect*& rect, float dx, float dy);