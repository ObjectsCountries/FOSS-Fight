#pragma once

#include <concepts>
#include <exception>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#define BOX_TYPE_COUNT static_cast<unsigned short>(6U)
#define DEBUG_RENDER_BOXES true

template <std::integral T>
std::string format_number(T number, const bool hex);

void setCoordinates(SDL_FRect** rect, float x, float y, float width, float height);
void changeLocation(SDL_FRect** rect, float x, float y);
void multiplySize(SDL_FRect** rect, float factor);

template <std::integral T>
class DataException : public std::exception {
private:
    std::string origin;
    T data;
    std::string message;
public:
    DataException(const char* origin, const T data, const char* message);
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
    PROXIMITY_GUARD = 0x0005U
};

bool boxTypeToColor(SDL_Renderer*& renderer, BoxType boxType, bool outline);

class Box {
private:
    const BoxType boxType;
    Buffer<unsigned short> buffer;
public:
    Box(SDL_IOStream*& stream, const BoxType boxType);
    ~Box() = default;
    SDL_FRect* rect;
    SDL_FRect* getRect() const;
    BoxType getBoxType() const;
};

class Frame {
private:
    std::vector<Box> boxes;
    Buffer<unsigned short> buffer;
    SDL_Texture* texture;
    unsigned short length;
    SDL_FRect* spriteSheetArea;
public:
    Frame(SDL_IOStream*& stream, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet);
    ~Frame() = default;
    SDL_FRect* getSpriteSheetArea() const;
    unsigned short getLength() const;
    std::vector<Box> getBoxes() const;
    void destroyTexture() const;
    void render(SDL_Renderer*& renderer, SDL_FRect* location, const float size) const;
};

class Animation {
private:
    std::vector<Frame> frames;
public:
    Animation(SDL_IOStream*& stream, unsigned short frameCount, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet);
    ~Animation() = default;
    std::vector<Frame> getFrames() const;
};

class Character {
private:
    std::string name;
    unsigned short maxHealth = 500U;
    unsigned short currentHealth = 500U;
    const SDL_Surface* spriteSheet;
    std::vector<Animation> animations;
    SDL_FRect* coordinates;
    size_t currentAnimation = 0UZ;
    size_t previousAnimation = currentAnimation;
    size_t frame = 0UZ;
    size_t sprite = 0UZ;
    float size;
public:
    Character(SDL_Renderer*& renderer, SDL_IOStream* sprites, SDL_IOStream* ffFile, SDL_FRect* coordinates, float size = 1.0f);
    ~Character();
    void render(SDL_Renderer*& renderer);
};