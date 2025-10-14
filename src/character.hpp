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

/**
 * Formats a number to look pretty when printed.
 *
 * @tparam T Any kind of whole number.
 * @param number The number to format.
 * @param hex Whether to format in hexadecimal (@c true) or decimal (@c false).
 * @param uppercase Whether hex digits and suffixes (U, L, etc.) are uppercase (@c true) or lowercase (@c false).
 * @return The formatted version of the number.
 */
template <std::integral T>
std::string format_number(T number, bool hex = true, bool uppercase = true);

/**
 * Changes the coordinates of a <c>SDL_FRect*</c>.
 * @param rect The rectangle to modify.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 * @param width The new width.
 * @param height The new height.
 */
void setCoordinatesRect(SDL_FRect*& rect, float x, float y, float width, float height);

/**
 * Changes the scale of a <c>SDL_FRect*</c>.
 * @param rect The rectangle to change the scaling of.
 * @param factor The factor of scaling.
 */
void multiplySizeRect(SDL_FRect*& rect, float factor);

/**
 * Changes the location of a <c>SDL_FRect*</c>.
 * @param rect The rectangle to change the location of.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 */
void changeLocationRect(SDL_FRect*& rect, float x, float y);

/**
 * Changes the dimensions of a <c>SDL_FRect*</c>.
 * @param rect The rectangle to change the dimensions of.
 * @param width The new width.
 * @param height The new height.
 */
void changeDimensionsRect(SDL_FRect*& rect, float width, float height);

/**
 * Moves a <c>SDL_FRect*</c>.
 * @param rect The rectangle to move.
 * @param dx The change in x-coordinate.
 * @param dy The change in y-coordinate.
 */
void moveRect(SDL_FRect*& rect, float dx, float dy);


/**
 * Exception to be thrown when having issues reading a data file.
 * @tparam T Any kind of whole number.
 */
template <std::integral T>
class DataException : public std::exception {
private:
    const std::string origin; /**< The function that threw the exception. */
    const std::string error; /**< The error in question. */
    const T data; /**< The data surrounding the error. */
    std::string result;  /**< A formatted string of the exception information. */
public:
    /**
     * Constructs a <c>DataException</c>.
     * @param origin The function that threw the exception.
     * @param error The error in question.
     * @param data The data surrounding the error.
     */
    DataException(std::string origin, std::string error, T data = {});
    /**
     * Describes the exception.
     * @return A formatted string of the exception information.
     */
    const char* what() const noexcept override;
};

/**
 * A buffer that holds four items, meant to hold data for a <c>SDL_FRect*</c>.
 * @tparam T Any kind of whole number.
 */
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
    NULL_TERMINATOR = 0x0000U,
    HURTBOX = 0x0001U,
    GRAB = 0x0002U,
    COMMAND_GRAB = 0x0003U,
    THROW_PUSH_GROUND_COLLISION = 0x0004U,
    PROXIMITY_GUARD = 0x0005U,
    NON_CANCELABLE_HITBOX = 0x0100U,
    SPECIAL_CANCELABLE_HITBOX = 0x0101U,
    SUPER_CANCELABLE_HITBOX = 0x0102U,
    SPECIAL_SUPER_CANCELABLE_HITBOX = 0x0103U
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
    ALT_STANCE_SUPER = 0x1300U,
    NOTHING = 0xFFFFU
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
    Buffer<unsigned short> spriteSheetBuffer;
    Buffer<signed short> boxBuffer;
    SDL_Texture* texture;
    unsigned short length;
    SDL_FRect* spriteSheetArea;
public:
    std::map<BoxType, std::vector<SDL_FRect*>> boxes;
    std::map<BoxType, std::vector<SDL_FRect*>> boxesWithLocation;
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
    AnimationType previousAction = previousAnimation;
    AnimationType currentAttack = NOTHING;
    unsigned short frame = 0U;
    size_t sprite = 0UZ;
    bool midair = false;
    float size;
    float walkForwardSpeed;
    float walkBackwardSpeed;
    float jumpForwardXVelocity;
    float jumpBackwardXVelocity;
    float initialJumpVelocity;
    float gravity;
    float currentXVelocity = 0.0f;
    float currentYVelocity = 0.0f;
    Direction jumpArc = UP;
public:
    InputHistory inputs;
    BaseCommandInputParser controller;
    Character(const char* name, SDL_Renderer*& renderer, const BaseCommandInputParser& controller, const SDL_FRect*& groundBox);
    ~Character();
    AnimationType processAttacks();
    AnimationType processInputs();
    void render(SDL_Renderer*& renderer);
};