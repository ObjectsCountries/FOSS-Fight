#include "frect_helpers.hpp"

#include <SDL3/SDL.h>

void setCoordinatesRect(SDL_FRect*& rect, const float x, const float y,
                    const float width, const float height) {
    if (rect != nullptr) {
        rect->x = x;
        rect->y = y;
        rect->w = width;
        rect->h = height;
    }
}

void multiplySizeRect(SDL_FRect*& rect, const float factor) {
    if (rect != nullptr) {
        rect->x *= factor;
        rect->y *= factor;
        rect->w *= factor;
        rect->h *= factor;
    }
}

void changeLocationRect(SDL_FRect*& rect, const float x, const float y) {
    if (rect != nullptr) {
        rect->x = x;
        rect->y = y;
    }
}

void changeDimensionsRect(SDL_FRect*& rect, const float width, const float height) {
    if (rect != nullptr) {
        rect->w = width;
        rect->h = height;
    }
}

void moveRect(SDL_FRect*& rect, const float dx, const float dy) {
    if (rect != nullptr) {
        rect->x += dx;
        rect->y += dy;
    }
}