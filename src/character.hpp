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
    NON_CANCELABLE_HITBOX = 0x0100U, /**< An attack that cannot be canceled into special moves or supers. */
    SPECIAL_CANCELABLE_HITBOX = 0x0101U, /**< An attack that can be canceled into special moves, but not supers. */
    SUPER_CANCELABLE_HITBOX = 0x0102U, /**< An attack that can be canceled into supers, but not special moves. */
    SPECIAL_SUPER_CANCELABLE_HITBOX = 0x0103U /**< An attack that can be canceled into special moves or supers. */
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
    STAND_LIGHT_PUNCH = 0x0100U, /**< Standing light punch (5LP). */
    STAND_HEAVY_PUNCH = 0x0101U, /**< Standing heavy punch (5HP). */
    STAND_LIGHT_KICK = 0x0102U, /**< Standing light kick (5LK). */
    STAND_HEAVY_KICK = 0x0103U, /**< Standing heavy kick (5HK). */
    CROUCH_LIGHT_PUNCH = 0x0104U, /**< Crouching light punch (2LP). */
    CROUCH_HEAVY_PUNCH = 0x0105U, /**< Crouching heavy punch (2HP), universal anti-air. */
    CROUCH_LIGHT_KICK = 0x0106U, /**< Crouching light kick (2LK). */
    CROUCH_HEAVY_KICK = 0x0107U, /**< Crouching heavy kick (2HK), aka sweep. */
    JUMP_LIGHT_PUNCH = 0x0108U, /**< Jumping light punch (j.LP). */
    JUMP_HEAVY_PUNCH = 0x0109U, /**< Jumping heavy punch (j.HP). */
    JUMP_LIGHT_KICK = 0x010AU, /**< Jumping light kick (j.LK). */
    JUMP_HEAVY_KICK = 0x010BU, /**< Jumping heavy kick (j.HK). */
    FORWARD_LIGHT_KICK = 0x010CU, /**< Forward light kick (6LK), universal overhead. */
    FORWARD_THROW = 0x0180U, /**< Throwing the opponent forwards. */
    BACKWARD_THROW = 0x0181U, /**< Throwing the opponent backwards. */
    SPECIAL_1 = 0x0200U, /**< First special move. */
    SPECIAL_2 = 0x0201U, /**< Second special move. */
    SPECIAL_3 = 0x0202U, /**< Third special move. */
    SPECIAL_4 = 0x0203U, /**< Fourth special move. */
    SPECIAL_5 = 0x0204U, /**< Fifth special move. */
    SPECIAL_6 = 0x0205U, /**< Sixth special move. */
    SPECIAL_7 = 0x0206U, /**< Seventh special move. */
    SPECIAL_8 = 0x0207U, /**< Eighth special move. */
    SPECIAL_9 = 0x0208U, /**< Nineth special move. */
    SPECIAL_10 = 0x0209U, /**< Tenth special move. */
    SUPER = 0x0300U, /**< The character's Super Art. */
    MISC_ASSET_1 = 0x0400U, /**< First miscellaneous asset. */
    MISC_ASSET_2 = 0x0401U, /**< Second miscellaneous asset. */
    MISC_ASSET_3 = 0x0402U, /**< Third miscellaneous asset. */
    MISC_ASSET_4 = 0x0403U, /**< Fourth miscellaneous asset. */
    MISC_ASSET_5 = 0x0404U, /**< Fifth miscellaneous asset. */
    MISC_ASSET_6 = 0x0405U, /**< Sixth miscellaneous asset. */
    MISC_ASSET_7 = 0x0406U, /**< Seventh miscellaneous asset. */
    MISC_ASSET_8 = 0x0407U, /**< Eighth miscellaneous asset. */
    MISC_ASSET_9 = 0x0408U, /**< Nineth miscellaneous asset. */
    MISC_ASSET_10 = 0x0409U, /**< Tenth miscellaneous asset. */
    CHARACTER_METER_ASSET_1 = 0x0500U, /**< First character-specific meter asset. */
    CHARACTER_METER_ASSET_2 = 0x0501U, /**< Second character-specific meter asset. */
    CHARACTER_METER_ASSET_3 = 0x0502U, /**< Third character-specific meter asset. */
    CHARACTER_METER_ASSET_4 = 0x0503U, /**< Fourth character-specific meter asset. */
    CHARACTER_METER_ASSET_5 = 0x0504U, /**< Fifth character-specific meter asset. */
    CHARACTER_METER_ASSET_6 = 0x0505U, /**< Sixth character-specific meter asset. */
    CHARACTER_METER_ASSET_7 = 0x0506U, /**< Seventh character-specific meter asset. */
    CHARACTER_METER_ASSET_8 = 0x0507U, /**< Eighth character-specific meter asset. */
    CHARACTER_METER_ASSET_9 = 0x0508U, /**< Nineth character-specific meter asset. */
    CHARACTER_METER_ASSET_10 = 0x0509U, /**< Tenth character-specific meter asset. */
    ALT_STANCE_IDLE = 0x1000U, /**< Not doing anything (alt stance). */
    ALT_STANCE_WALK_FORWARD = 0x1001U, /**< Walking forward (alt stance). */
    ALT_STANCE_WALK_BACKWARD = 0x1002U, /**< Walking backward (alt stance). */
    ALT_STANCE_CROUCH_TRANSITION = 0x1003U, /**< Transitioning from standing to crouching, or vice versa (alt stance). */
    ALT_STANCE_CROUCH = 0x1004U, /**< Crouching (alt stance). */
    ALT_STANCE_STAND_BLOCK = 0x1005U, /**< Blocking an attack while standing (alt stance). */
    ALT_STANCE_CROUCH_BLOCK = 0x1006U, /**< Blocking an attack while crouching (alt stance). */
    ALT_STANCE_PRE_JUMP = 0x1007U, /**< Frames before jumping (alt stance). */
    ALT_STANCE_JUMP_FORWARD = 0x1008U, /**< Jumping forward (alt stance). */
    ALT_STANCE_JUMP_NEUTRAL = 0x1009U, /**< Jumping vertically (alt stance). */
    ALT_STANCE_JUMP_BACKWARD = 0x100AU, /**< Jumping backward (alt stance). */
    ALT_STANCE_STAND_GETTING_HIT = 0x100BU, /**< Getting hit while standing (alt stance). */
    ALT_STANCE_CROUCH_GETTING_HIT = 0x100CU, /**< Getting hit while crouching (alt stance). */
    ALT_STANCE_AIR_GETTING_HIT = 0x100DU, /**< Getting hit in midair (alt stance). */
    ALT_STANCE_AIR_RESET = 0x100EU, /**< The character landing on their feet after getting hit in midair (alt stance). */
    ALT_STANCE_KNOCKDOWN = 0x100FU, /**< Getting knocked down (alt stance). */
    ALT_STANCE_GET_UP = 0x1010U, /**< Getting up (alt stance). */
    ALT_STANCE_STAND_LIGHT_PUNCH = 0x1100U, /**< Standing light punch (5LP) (alt stance). */
    ALT_STANCE_STAND_HEAVY_PUNCH = 0x1101U, /**< Standing heavy punch (5HP) (alt stance). */
    ALT_STANCE_STAND_LIGHT_KICK = 0x1102U, /**< Standing light kick (5LK) (alt stance). */
    ALT_STANCE_STAND_HEAVY_KICK = 0x1103U, /**< Standing heavy kick (5HK) (alt stance). */
    ALT_STANCE_CROUCH_LIGHT_PUNCH = 0x1104U, /**< Crouching light punch (2LP) (alt stance). */
    ALT_STANCE_CROUCH_HEAVY_PUNCH = 0x1105U, /**< Crouching heavy punch (2HP), universal anti-air (alt stance). */
    ALT_STANCE_CROUCH_LIGHT_KICK = 0x1106U, /**< Crouching light kick (2LK) (alt stance). */
    ALT_STANCE_CROUCH_HEAVY_KICK = 0x1107U, /**< Crouching heavy kick (2HK), aka sweep (alt stance). */
    ALT_STANCE_JUMP_LIGHT_PUNCH = 0x1108U, /**< Jumping light punch (j.LP) (alt stance). */
    ALT_STANCE_JUMP_HEAVY_PUNCH = 0x1109U, /**< Jumping heavy punch (j.HP) (alt stance). */
    ALT_STANCE_JUMP_LIGHT_KICK = 0x110AU, /**< Jumping light kick (j.LK) (alt stance). */
    ALT_STANCE_JUMP_HEAVY_KICK = 0x110BU, /**< Jumping heavy kick (j.HK) (alt stance). */
    ALT_STANCE_FORWARD_LIGHT_KICK = 0x110CU, /**< Forward light kick (6LK), universal overhead (alt stance). */
    ALT_STANCE_FORWARD_THROW = 0x1180U, /**< Throwing the opponent forwards (alt stance). */
    ALT_STANCE_BACKWARD_THROW = 0x1181U, /**< Throwing the opponent backwards (alt stance). */
    ALT_STANCE_SPECIAL_1 = 0x1200U, /**< First special move (alt stance). */
    ALT_STANCE_SPECIAL_2 = 0x1201U, /**< Second special move (alt stance). */
    ALT_STANCE_SPECIAL_3 = 0x1202U, /**< Third special move (alt stance). */
    ALT_STANCE_SPECIAL_4 = 0x1203U, /**< Fourth special move (alt stance). */
    ALT_STANCE_SPECIAL_5 = 0x1204U, /**< Fifth special move (alt stance). */
    ALT_STANCE_SPECIAL_6 = 0x1205U, /**< Sixth special move (alt stance). */
    ALT_STANCE_SPECIAL_7 = 0x1206U, /**< Seventh special move (alt stance). */
    ALT_STANCE_SPECIAL_8 = 0x1207U, /**< Eighth special move (alt stance). */
    ALT_STANCE_SPECIAL_9 = 0x1208U, /**< Nineth special move (alt stance). */
    ALT_STANCE_SPECIAL_10 = 0x1209U, /**< Tenth special move (alt stance). */
    ALT_STANCE_SUPER = 0x1300U, /**< The character's Super Art (alt stance). */
    ALT_STANCE_MISC_ASSET_1 = 0x1400U, /**< First miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_2 = 0x1401U, /**< Second miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_3 = 0x1402U, /**< Third miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_4 = 0x1403U, /**< Fourth miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_5 = 0x1404U, /**< Fifth miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_6 = 0x1405U, /**< Sixth miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_7 = 0x1406U, /**< Seventh miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_8 = 0x1407U, /**< Eighth miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_9 = 0x1408U, /**< Nineth miscellaneous asset (alt stance). */
    ALT_STANCE_MISC_ASSET_10 = 0x1409U, /**< Tenth miscellaneous asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_1 = 0x1500U, /**< First character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_2 = 0x1501U, /**< Second character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_3 = 0x1502U, /**< Third character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_4 = 0x1503U, /**< Fourth character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_5 = 0x1504U, /**< Fifth character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_6 = 0x1505U, /**< Sixth character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_7 = 0x1506U, /**< Seventh character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_8 = 0x1507U, /**< Eighth character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_9 = 0x1508U, /**< Nineth character-specific meter asset (alt stance). */
    ALT_STANCE_CHARACTER_METER_ASSET_10 = 0x1509U, /**< Tenth character-specific meter asset (alt stance). */
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

/**
 * Represents a frame of an animation.
 */
class Sprite {
private:
    Buffer<unsigned short> spriteSheetBuffer; /**< A @c Buffer holding the data for where on the sprite sheet the sprite is located. */
    Buffer<signed short> boxBuffer; /**< A @c Buffer holding the data for a box's coordinates and dimensions. */
    SDL_Texture* texture; /**< The texture used in rendering the frame. */
    unsigned short length; /**< How many frames (1/60 of a second) to show the sprite for. */
    SDL_FRect* spriteSheetArea; /**< The area of the sprite sheet where the sprite's image is located. */
public:
    signed short xOffset = 0x0000; /**< The horizontal offset of this asset. */
    signed short yOffset = 0x0000; /**< The vertical offset of this asset. */
    std::map<BoxType, std::vector<SDL_FRect*>> boxes; /**< The sprite's boxes, along with their type, updated to the location of the sprite on the screen. */
    std::map<BoxType, std::vector<SDL_FRect*>> boxesWithAbsoluteLocation; /**< The sprite's boxes, along with their type, with the absolute location given by the data. */
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
    std::string name; /**< The character's name. */
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