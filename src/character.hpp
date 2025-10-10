#pragma once

#include "command_input_parser.hpp"
#include "input_history.hpp"

#include <concepts>
#include <exception>
#include <map>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#define DEBUG_RENDER_BOXES true

template <std::integral T>
std::string format_number(T number, const bool hex = true, const bool uppercase = true);

void setCoordinatesRect(SDL_FRect*& rect, float x, float y, float width, float height);
void changeLocationRect(SDL_FRect*& rect, float x, float y);
void multiplySizeRect(SDL_FRect*& rect, float factor);
void moveRect(SDL_FRect*& rect, float dx, float dy);

template <std::integral T>
class DataException : public std::exception {
private:
    const std::string origin;
    const std::string error;
    const T data;
    std::string result;
public:
    DataException(std::string origin, std::string error, const T data = {});
    const char* what() const noexcept override;
};

template <std::integral T>
class Buffer {
private:
    T* x = nullptr;
    T* y = nullptr;
    T* width = nullptr;
    T* height = nullptr;
public:
    Buffer() = default;
    ~Buffer() = default;
    void assign(T datum);
    SDL_FRect toFRect() const;
    void clear();
};

enum BoxType : unsigned short {
    HURTBOX = 0x0000U,
    HITBOX = 0x0001U,
    GRAB = 0x0002U,
    COMMAND_GRAB = 0x0003U,
    THROW_PUSH = 0x0004U,
    PROXIMITY_GUARD = 0x0005U,
    BOX_TYPE_COUNT = 0x0006U
};

bool boxTypeToColor(SDL_Renderer*& renderer, BoxType boxType, bool outline);

class Box {
private:
    BoxType boxType;
    Buffer<unsigned short> buffer;
public:
    Box(SDL_IOStream*& stream, const BoxType boxType, SDL_FRect*& copy);
    Box(const float x, const float y, const float width, const float height, const BoxType boxType);
    SDL_FRect* rect;
    BoxType getBoxType() const;
};

class Frame {
private:
    Buffer<unsigned short> buffer;
    SDL_Texture* texture;
    unsigned short length;
    SDL_FRect* spriteSheetArea;
public:
    std::vector<Box> boxes;
    Frame(SDL_IOStream*& stream, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet);
    Frame(const Frame& reference, SDL_IOStream*& stream, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet);
    ~Frame() = default;
    SDL_FRect* getSpriteSheetArea() const;
    unsigned short getLength() const;
    void destroyTexture() const;
    void render(SDL_Renderer*& renderer, const SDL_FRect* location) const;
};


class Character {
private:
    static const SDL_FRect* ground;
    std::string name;
    unsigned short maxHealth = 500U;
    unsigned short currentHealth = 500U;
    const SDL_Surface* spriteSheet;
    std::map<unsigned short, std::vector<Frame>> animations;
    SDL_FRect* coordinates;
    size_t currentAnimation = 0UZ;
    size_t previousAnimation = currentAnimation;
    unsigned short frame = 0U;
    size_t sprite = 0UZ;
    float size;
    bool midair = false;
    const float speed = 4.0f;
    const float walkBackSpeed = -3.0f;
    const float jumpXVelocity = 8.0f;
    const float backJumpXVelocity = -8.0f;
    const float initialJumpVelocity = 20.0f;
    float currentYVelocity = 0.0f;
    float gravity = 1.0f;
    Direction jumpArc = UP;
public:
    InputHistory inputs;
    BaseCommandInputParser controller;
    Character(const char* name, SDL_Renderer*& renderer, const BaseCommandInputParser& controller, const SDL_FRect*& groundBox, const float size = 1.0f);
    ~Character();
    size_t processInputs();
    void render(SDL_Renderer*& renderer);
};