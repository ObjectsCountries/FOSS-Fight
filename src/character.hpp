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
    HURTBOX = 0x0001U,
    GRAB = 0x0002U,
    COMMAND_GRAB = 0x0003U,
    THROW_PUSH = 0x0004U,
    PROXIMITY_GUARD = 0x0005U,
    NON_CANCELABLE_HITBOX = 0x0100U,
    SPECIAL_CANCELABLE_HITBOX = 0x0101U,
    SUPER_CANCELABLE_HITBOX = 0x0102U,
    SPECIAL_SUPER_CANCELABLE_HITBOX = 0x0103U,
};

enum AnimationType : unsigned short {
    IDLE = 0x0000U,
    WALKING_FORWARD = 0x0001U,
    WALKING_BACKWARD = 0x0002U,
    CROUCH_TRANSITION = 0x0003U,
    CROUCH = 0x0004U,
    STAND_BLOCK = 0x0005U,
    CROUCH_BLOCK = 0x0006U,
    PRE_JUMP = 0x0007U,
    JUMP_FORWARD = 0x0008U,
    JUMP_NEUTRAL = 0x0009U,
    JUMP_BACKWARD = 0x000AU,
    STAND_GETTING_HIT = 0x000BU,
    CROUCH_GETTING_HIT = 0x000CU,
    AIR_GETTING_HIT = 0x000DU,
    AIR_RESET = 0x000EU,
    KNOCKDOWN = 0x000FU,
    GETTING_UP = 0x0010U,
    STANDING_LIGHT_PUNCH = 0x0100U,
    STANDING_HEAVY_PUNCH = 0x0101U,
    STANDING_LIGHT_KICK = 0x0102U,
    STANDING_HEAVY_KICK = 0x0103U,
    CROUCHING_LIGHT_PUNCH = 0x0104U,
    CROUCHING_HEAVY_PUNCH = 0x0105U,
    CROUCHING_LIGHT_KICK = 0x0106U,
    CROUCHING_HEAVY_KICK = 0x0107U,
    JUMPING_LIGHT_PUNCH = 0x0108U,
    JUMPING_HEAVY_PUNCH = 0x0109U,
    JUMPING_LIGHT_KICK = 0x010AU,
    JUMPING_HEAVY_KICK = 0x010BU,
    FORWARD_LIGHT_KICK = 0x010CU,
    FORWARD_THROW = 0x0180U,
    BACKWARD_THROW = 0x0181U,
    SPECIAL_1 = 0x0200U,
    SPECIAL_2 = 0x0201U,
    SPECIAL_3 = 0x0202U,
    SPECIAL_4 = 0x0203U,
    SPECIAL_5 = 0x0204U,
    SPECIAL_6 = 0x0205U,
    SPECIAL_7 = 0x0206U,
    SPECIAL_8 = 0x0207U,
    SPECIAL_9 = 0x0208U,
    SPECIAL_10 = 0x0209U,
    SUPER = 0x0300U,
    ALT_STANCE_IDLE = 0x1000U,
    ALT_STANCE_WALKING_FORWARD = 0x1001U,
    ALT_STANCE_WALKING_BACKWARD = 0x1002U,
    ALT_STANCE_CROUCH_TRANSITION = 0x1003U,
    ALT_STANCE_CROUCH = 0x1004U,
    ALT_STANCE_STAND_BLOCK = 0x1005U,
    ALT_STANCE_CROUCH_BLOCK = 0x1006U,
    ALT_STANCE_PRE_JUMP = 0x1007U,
    ALT_STANCE_JUMP_FORWARD = 0x1008U,
    ALT_STANCE_JUMP_NEUTRAL = 0x1009U,
    ALT_STANCE_JUMP_BACKWARD = 0x100AU,
    ALT_STANCE_STAND_GETTING_HIT = 0x100BU,
    ALT_STANCE_CROUCH_GETTING_HIT = 0x100CU,
    ALT_STANCE_AIR_GETTING_HIT = 0x100DU,
    ALT_STANCE_AIR_RESET = 0x100EU,
    ALT_STANCE_KNOCKDOWN = 0x100FU,
    ALT_STANCE_GETTING_UP = 0x1010U,
    ALT_STANCE_STANDING_LIGHT_PUNCH = 0x1100U,
    ALT_STANCE_STANDING_HEAVY_PUNCH = 0x1101U,
    ALT_STANCE_STANDING_LIGHT_KICK = 0x1102U,
    ALT_STANCE_STANDING_HEAVY_KICK = 0x1103U,
    ALT_STANCE_CROUCHING_LIGHT_PUNCH = 0x1104U,
    ALT_STANCE_CROUCHING_HEAVY_PUNCH = 0x1105U,
    ALT_STANCE_CROUCHING_LIGHT_KICK = 0x1106U,
    ALT_STANCE_CROUCHING_HEAVY_KICK = 0x1107U,
    ALT_STANCE_JUMPING_LIGHT_PUNCH = 0x1108U,
    ALT_STANCE_JUMPING_HEAVY_PUNCH = 0x1109U,
    ALT_STANCE_JUMPING_LIGHT_KICK = 0x110AU,
    ALT_STANCE_JUMPING_HEAVY_KICK = 0x110BU,
    ALT_STANCE_FORWARD_LIGHT_KICK = 0x110CU,
    ALT_STANCE_FORWARD_THROW = 0x1180U,
    ALT_STANCE_BACKWARD_THROW = 0x1181U,
    ALT_STANCE_SPECIAL_1 = 0x1200U,
    ALT_STANCE_SPECIAL_2 = 0x1201U,
    ALT_STANCE_SPECIAL_3 = 0x1202U,
    ALT_STANCE_SPECIAL_4 = 0x1203U,
    ALT_STANCE_SPECIAL_5 = 0x1204U,
    ALT_STANCE_SPECIAL_6 = 0x1205U,
    ALT_STANCE_SPECIAL_7 = 0x1206U,
    ALT_STANCE_SPECIAL_8 = 0x1207U,
    ALT_STANCE_SPECIAL_9 = 0x1208U,
    ALT_STANCE_SPECIAL_10 = 0x1209U,
    ALT_STANCE_SUPER = 0x1300U
};

class CopyInformation : public std::exception {
private:
    std::string result;
public:
    AnimationType animationType;
    unsigned short index;
    bool copyFrameLength;
    bool copySpriteSheetLocation;
    bool copyHurtboxes;
    bool copyGrabBoxes;
    bool copyCommandGrabBoxes;
    bool copyThrowAndPushBoxes;
    bool copyProximityGuardBoxes;
    bool copyHitboxes;
    CopyInformation(unsigned short type, unsigned short index, uint8_t copyInfo);
    const char* what() const noexcept override;
};

bool boxTypeToColor(SDL_Renderer*& renderer, BoxType boxType, bool outline);

class Frame {
private:
    Buffer<unsigned short> buffer;
    SDL_Texture* texture;
    unsigned short length;
    SDL_FRect* spriteSheetArea;
public:
    std::map<BoxType, std::vector<SDL_FRect*>> boxes;
    Frame(SDL_IOStream*& stream, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet);
    Frame(const Frame& reference, SDL_IOStream*& stream, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet, const CopyInformation& copy);
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
    std::map<AnimationType, std::vector<Frame>> animations;
    SDL_FRect* coordinates;
    AnimationType currentAnimation = IDLE;
    AnimationType previousAnimation = currentAnimation;
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
    AnimationType processInputs();
    void render(SDL_Renderer*& renderer);
};