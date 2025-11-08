#pragma once

#include "command_input_parser.hpp"
#include "input_history.hpp"

#include <concepts>
#include <exception>
#include <map>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

/**
 * Determines whether to render the boxes.
 */
#define DEBUG_RENDER_BOXES true

/**
 * Formats a number to look pretty when printed.
 *
 * @tparam T Any kind of whole number.
 * @param number The number to format.
 * @param hex Whether to format in hexadecimal (@c true) or binary/decimal (@c false).
 * @param binary Whether to format in binary (@c true) or decimal (@c false).
 * @param uppercase Whether hex digits and suffixes (U, L, etc.) are uppercase (@c true) or lowercase (@c false).
 * @return The formatted version of the number.
 */
template <std::integral T>
std::string format_number(T number, bool hex = true, bool binary = true, bool uppercase = true);

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
     * Constructs a @c DataException .
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
 * A buffer that holds four items, meant to hold data for a @c SDL_FRect* .
 * @tparam T Any kind of whole number.
 */
template <std::integral T>
class Buffer {
private:
    T* x = nullptr; /**< The x-coordinate of the rectangle, @c nullptr if not assigned yet. */
    T* y = nullptr; /**< The y-coordinate of the rectangle, @c nullptr if not assigned yet. */
    T* width = nullptr; /**< The width of the rectangle, @c nullptr if not assigned yet. */
    T* height = nullptr; /**< The height of the rectangle, @c nullptr if not assigned yet. */
public:
    /**
     * Constructs a @c Buffer with no given values.
     */
    Buffer() = default;
    /**
     * Deletes all values in the @c Buffer before destructing.
     */
    ~Buffer() = default;
    /**
     * Assigns one item to the buffer.
     * @param datum The item to assign to the buffer.
     * @exception std::out_of_range Attempted to assign an item to the buffer while it was full.
     */
    void assign(T datum);
    /**
     * Creates a @c SDL_FRect out of the buffer contents.
     * @return A rectangle with the coordinates and dimensions specified in the buffer.
     * @exception DataException Any item in the buffer is a @c nullptr .
     */
    SDL_FRect toFRect() const;
    /**
     * Deletes the contents of the buffer and assigns @c nullptr to each item.
     */
    void clear();
};

/**
 * The different types of boxes in a character's data.
 */
enum BoxType : unsigned short {
    NULL_TERMINATOR = 0x0000U, /**< Indicates the end of box definition. */
    HURTBOX = 0x0001U, /**< Where the character can be hit by attacks. */
    COMMAND_GRAB = 0x0002U, /**< Where a character grabs the opponent with a command grab. */
    THROW_PUSH_GROUND_COLLISION = 0x0003U, /**< Where a character can be thrown or pushed, as well as ground collision detection. */
    PROXIMITY_GUARD = 0x0004U, /**< If a character is holding back or down-back while overlapping with this kind of box, they are forced to enter the blocking animation. */
    HITBOX_BEGIN = 0x0100U, /**< Begins the definition of hitboxes, or attacks. */
    HITBOX_END = 0x01FFU, /**< Ends the definition of hitboxes, or attacks. */
};

/**
 * The various types of character animations.
 */
enum AnimationType : unsigned short {
    IDLE = 0x0000U, /**< Not doing anything. */
    WALK_FORWARD = 0x0001U, /**< Walking forward. */
    WALK_BACKWARD = 0x0002U, /**< Walking backward. */
    CROUCH_TRANSITION = 0x0003U, /**< Transitioning from standing to crouching, or vice versa. */
    CROUCH = 0x0004U, /**< Crouching. */
    STAND_BLOCK = 0x0005U, /**< Blocking an attack while standing. */
    CROUCH_BLOCK = 0x0006U, /**< Blocking an attack while crouching. */
    PRE_JUMP = 0x0007U, /**< Frames before jumping. */
    JUMP_FORWARD = 0x0008U, /**< Jumping forward. */
    JUMP_NEUTRAL = 0x0009U, /**< Jumping vertically. */
    JUMP_BACKWARD = 0x000AU, /**< Jumping backward. */
    STAND_GETTING_HIT = 0x000BU, /**< Getting hit while standing. */
    CROUCH_GETTING_HIT = 0x000CU, /**< Getting hit while crouching. */
    AIR_GETTING_HIT = 0x000DU, /**< Getting hit in midair. */
    AIR_RESET = 0x000EU, /**< The character landing on their feet after getting hit in midair. */
    KNOCKDOWN = 0x000FU, /**< Getting knocked down. */
    GET_UP = 0x0010U, /**< Getting up. */
    VICTORY = 0x0011U, /**< Animation that plays when the character wins a round. */
    DEFEAT = 0x0012U, /**< Animation that plays when the character loses a round to a time-out. */
    STAND_LIGHT_PUNCH = 0x0100U, /**< Standing light punch (5LP). */
    STAND_HEAVY_PUNCH = 0x0101U, /**< Standing heavy punch (5HP). */
    STAND_LIGHT_KICK = 0x0102U, /**< Standing light kick (5LK). */
    STAND_HEAVY_KICK = 0x0103U, /**< Standing heavy kick (5HK). */
    FORWARD_LIGHT_KICK = 0x0104U, /**< Forward light kick (6LK), universal overhead. */
    CROUCH_LIGHT_PUNCH = 0x0110U, /**< Crouching light punch (2LP). */
    CROUCH_HEAVY_PUNCH = 0x0111U, /**< Crouching heavy punch (2HP), universal anti-air. */
    CROUCH_LIGHT_KICK = 0x0112U, /**< Crouching light kick (2LK). */
    CROUCH_HEAVY_KICK = 0x0113U, /**< Crouching heavy kick (2HK), aka sweep. */
    JUMP_LIGHT_PUNCH = 0x0120U, /**< Jumping light punch (j.LP). */
    JUMP_HEAVY_PUNCH = 0x0121U, /**< Jumping heavy punch (j.HP). */
    JUMP_LIGHT_KICK = 0x0122U, /**< Jumping light kick (j.LK). */
    JUMP_HEAVY_KICK = 0x0123U, /**< Jumping heavy kick (j.HK). */
    COMMAND_NORMALS_START = 0x0124U, /**< Begins the definition of other command normals. */
    COMMAND_NORMALS_END = 0x017FU, /**< Ends the definition of other command normals. */
    FORWARD_THROW = 0x0180U, /**< Throwing the opponent forwards. */
    BACKWARD_THROW = 0x0181U, /**< Throwing the opponent backwards. */
    SPECIALS_START = 0x0200U, /**< Begins the definition of special moves. */
    SPECIALS_END = 0x02FFU, /**< Ends the definition of special moves. */
    SUPER = 0x0300U, /**< The character's Super Art. */
    CHARACTER_SPECIFIC_METER_ASSETS_BEGIN = 0xF000U, /**< Begins the definition of character-specific meter assets. */
    CHARACTER_SPECIFIC_METER_ASSETS_END = 0xF0FFU, /**< Ends the definition of character-specific meter assets. */
    MISC_ASSETS_BEGIN = 0xF100U, /**< Begins the definition of miscellaneous assets. */
    MISC_ASSETS_END = 0xF1FFU, /**< Ends the definition of miscellaneous assets. */
    CHARACTER_SELECTION_IMAGE = 0xFF00U, /**< Image of the character on the character selection screen. */
    CHARACTER_WIN_IMAGE = 0xFF01U, /**< Image shown after a game if the character wins. */
    CHARACTER_LOSS_IMAGE = 0xFF02U, /**< Image shown after a game if the character loses. */
    NOTHING = 0xFFFFU /**< Indicates none of the above. */
};

/**
 * Indicates that a frame is to be copied.
 */
class CopyInformation : public std::exception {
private:
    std::string result; /**< The result of which assets are to be copied. */
public:
    AnimationType animationType; /**< The type of animation to copy from. */
    size_t index; /**< The index of the animation to copy from, out of the given animation type's frames. Starts from 0. */
    bool copyFrameLength; /**< Whether to copy the frame's length or define it explicitly. */
    bool copySpriteSheetLocation; /**< Whether to copy the frame's sprite sheet location or define it explicitly. */
    bool copyOffset; /**< Whether to copy the frame's offset or define it explicitly. */
    bool copyHurtboxes; /**< Whether to copy the frame's hurtboxes or define them explicitly. */
    bool copyCommandGrabBoxes; /**< Whether to copy the frame's command grabi boxes or define them explicitly. */
    bool copyThrowPushAndGroundCollisionBoxes; /**< Whether to copy the frame's throw/push/ground collision boxes or define them explicitly. */
    bool copyProximityGuardBoxes; /**< Whether to copy the frame's proximity guard boxes or define them explicitly. */
    bool copyHitboxes; /**< Whether to copy the frame's hitboxes or define them explicitly. */
    /**
     * Constructs a @c CopyInformation .
     * @param type The type of animation to copy from.
     * @param index The index of the animation to copy from, out of the given animation type's frames. Starts from 0.
     * @param copyInfo The bits of information to determine which attributes to copy.
     */
    CopyInformation(unsigned short type, unsigned short index, uint8_t copyInfo);
    /**
     * Takes the result of the attributes being copied.
     * @return A formatted version of the copying information.
     */
    const char* what() const noexcept override;
};

/**
 * Converts a box type to a color, and modifies the renderer to use it.
 * @param renderer The renderer to change the color of.
 * @param boxType The box type, used for determining the color.
 * @param outline Whether to render an outline (@c true) or a box (@c false).
 * @return Whether changing the renderer's color was a success.
 * @exception DataException When given an unknown box type.
 */
bool boxTypeToColor(SDL_Renderer*& renderer, BoxType boxType, bool outline);

enum KnockbackLevel : uint8_t {
    MILD,
    MEDIUM,
    HEAVY,
    KNOCKS_DOWN
};

struct HitboxProperties {
    bool blockableHigh = true;
    bool blockableLow = true;
    bool specialCancelable = true;
    bool superCancelable = true;
    KnockbackLevel knockback = MILD;
    bool hardKnockdown = false;
    bool airReset = true;
    unsigned short hitStun, blockStun;
    signed short hitPushback, blockPushback;
    unsigned short xKnockback;
    signed short yKnockback;
    explicit HitboxProperties(uint8_t data);
    void updateStatsNoKnockdown(SDL_IOStream*& stream);
    void updateStatsKnockdown(SDL_IOStream*& stream);
    HitboxProperties() = default;
    ~HitboxProperties() = default;
};

struct CharacterBox {
    BoxType boxType = NULL_TERMINATOR;
    HitboxProperties hitboxProperties;
    Buffer<signed short> boxBuffer;
    SDL_FRect* rect = nullptr;
    explicit CharacterBox(SDL_IOStream*& stream, BoxType currentBoxType);
    CharacterBox(BoxType currentBoxType, HitboxProperties hitboxProperties, float x, float y, float width, float height);
    CharacterBox() = default;
    ~CharacterBox() = default;
};

/**
 * Represents a frame of an animation.
 */
class Sprite {
private:
    Buffer<unsigned short> spriteSheetBuffer; /**< A @c Buffer holding the data for where on the sprite sheet the sprite is located. */
    SDL_Texture* texture; /**< The texture used in rendering the frame. */
    unsigned short length; /**< How many frames (1/60 of a second) to show the sprite for. */
    SDL_FRect* spriteSheetArea; /**< The area of the sprite sheet where the sprite's image is located. */
public:
    signed short xOffset = 0x0000; /**< The horizontal offset of this asset. */
    signed short yOffset = 0x0000; /**< The vertical offset of this asset. */
    std::vector<CharacterBox> charBoxes; /**< The sprite's boxes, stored in a struct. */
    std::vector<CharacterBox> charBoxesWithAbsoluteLocation; /**< The sprite's boxes, stored in a struct, with their absolute location. */
    /**
     * Constructs a sprite, reading from the stream of data and the sprite sheet.
     * @param stream The stream of data to read from.
     * @param renderer The renderer to render on.
     * @param spriteSheet The sprite sheet to make a @c SDL_Texture* from.
     * @exception DataException Throws a @c DataException<int> when running into issues creating the texture, and a @c DataException<long> when running into issues reading from the stream.
     * @exception CopyInformation Indicates that the sprite is a copy of a different sprite.
     */
    Sprite(SDL_IOStream*& stream, SDL_Renderer*& renderer, SDL_Surface*& spriteSheet);
    /**
     * Copies data from another sprite, defining any non-copied data explicitly.
     * @param reference The sprite to copy from.
     * @param stream The stream of data to read from.
     * @param renderer The renderer to render on.
     * @param spriteSheet The sprite sheet to make a @c SDL_Texture* from.
     * @param copy The information about what to copy and what to define explicitly.
     * @exception DataException Throws a @c DataException<int> when running into issues creating the texture, and a @c DataException<long> when running into issues reading from the stream.
     */
    Sprite(const Sprite& reference, SDL_IOStream*& stream, SDL_Renderer*& renderer, SDL_Surface*& spriteSheet, const CopyInformation& copy);
    /**
     * Destroys a sprite.
     */
    ~Sprite() = default;
    /**
     * Gets the sprite's sprite sheet area.
     * @return The @c SDL_FRect* of the area on the sprite sheet where the sprite gets its image from.
     */
    SDL_FRect* getSpriteSheetArea() const;
    /**
     * Gets the sprite's duration.
     * @return How many frames (1/60 of a second) to show the sprite for.
     */
    unsigned short getLength() const;
    /**
     * Destroys the sprite's @c SDL_Texture* .
     */
    void destroyTexture() const;
    /**
     * Renders the sprite onto the screen.
     * @param renderer The renderer to render on.
     * @param location The coordinates and dimensions to render to.
     * @exception DataException Throws a <c>DataException<unsigned int></c> when running into issues rendering the texture, and a <c>DataException<unsigned char></c> when running into issues rendering the boxes.
     */
    void render(SDL_Renderer*& renderer, const SDL_FRect* location) const;
};

/**
 * Represents a playable character.
 */
class Character {
private:
    static const SDL_FRect* ground; /**< The ground that the characters stand on. */
    unsigned short maxHealth = 500U; /**< The character's maximum health. */
    unsigned short currentHealth = 500U; /**< The character's current health. */
    SDL_Surface* spriteSheet; /**< The sprite sheet containing all the character's sprites. */
    std::map<AnimationType, std::vector<Sprite>> animations; /**< The character's animations and moves. */
    SDL_FRect* coordinates; /**< The current coordinates of the character. */
    SDL_FRect* renderCoordinates; /**< The current rendering coordinates of the character. */
    AnimationType currentAnimation = IDLE; /**< The current animation that the character is playing. */
    AnimationType previousAnimation = currentAnimation; /**< The previous animation of the character. */
    AnimationType previousAction = previousAnimation; /**< The character's previous action. */
    AnimationType currentAttack = NOTHING; /**< The current attack that the character is executing. */
    unsigned short spriteIndex = 0U; /**< The current sprite of the animation to show. */
    size_t frame = 0UZ; /**< The number of frames that the sprite has been shown. */
    bool midair = false; /**< Whether the character is in the air (@c true) or on the ground (@c false). */
    float size; /**< How much to scale the character. */
    float walkForwardSpeed; /**< The speed at which the character walks forward (pixels/frame).  */
    float walkBackwardSpeed; /**< The speed at which the character walks backward (pixels/frame). */
    float jumpForwardXVelocity; /**< The speed at which the character moves forward when jumping (pixels/frame). */
    float jumpBackwardXVelocity; /**< The speed at which the character moves backward when jumping (pixels/frame). */
    float initialJumpVelocity; /**< The initial velocity at which the character leaves the ground when jumping (pixels/frame). */
    float gravity; /**< The speed at which the character falls (pixels/frame^2). */
    float currentXVelocity = 0.0f; /**< The current x-velocity of the character (pixels/frame). */
    float currentYVelocity = 0.0f; /**< The current y-velocity of the character (pixels/frame). */
    SDL_Palette* basePalette; /**< The base color scheme of the character. */
    std::vector<SDL_Palette*> altPalettes; /**< The alternative color schemes of the character. */
    Direction jumpArc = UP; /**< The direction in which this character is jumping, either @c Direction::UP_BACK, @c Direction::UP or @c Direction::UP_FORWARD . */
public:
    std::string name; /**< The character's name. */
    InputHistory inputs; /**< The input history of the character. */
    BaseCommandInputParser* controller; /**< The command input parser of the character. */
    /**
     * Constructs a character.
     * @param name The name of the character.
     * @param renderer The renderer to render this character onto.
     * @param controller The controller used for this character.
     * @param groundBox The box representing the ground.
     * @param paletteIndex The palette to choose from.
     * @exception DataException Throws a @c DataException<long> when encountering issues reading data, a <c>DataException<unsigned short></c> when the header of the data file is not <c>F0 55</c>, and a @c DataException<int> when encountering issues loading the sprite sheet.
     */
    Character(const char* name, SDL_Renderer*& renderer, BaseCommandInputParser* controller, const SDL_FRect*& groundBox, unsigned short paletteIndex = 0x0000U);
    /**
     * Destroys all the textures.
     */
    ~Character();
    /**
     * Processes button inputs to determine which attack to perform.
     * @return The current kind of attack.
     */
    AnimationType processAttacks();
    /**
     * Processes the current character situation.
     * @return The kind of animation to play.
     */
    AnimationType processInputs();
    /**
     * Renders the character onto the screen.
     * @param renderer The renderer to render on.
     */
    void render(SDL_Renderer*& renderer);
};