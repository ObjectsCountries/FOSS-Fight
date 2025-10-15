#pragma once

#include "input_history.hpp"

#include <SDL3/SDL.h>

/**
 * Determines how much of an analog stick input is needed to register a positive direction.
 */
constexpr short thresholdPositive = 15000;
/**
 * Determines how much of an analog stick input is needed to register a negative direction.
 */
constexpr short thresholdNegative = -1 * thresholdPositive;

/**
 * Base class for parsing inputs.
 */
class BaseCommandInputParser {
protected:
    const bool verticalSOCDIsUp; /**< If @c true, an input of down+up at the same time will result in up. If @c false, it will result in neutral. */
    ButtonGroup buttons; /**< The current buttons being pressed. */
    InputHistory recentInputs; /**< The inputs so far. */

    bool left; /**< Whether a left input is being sent. */
    bool right; /**< Whether a right input is being sent. */
    bool up; /**< Whether an up input is being sent. */
    bool down; /**< Whether a down input is being sent. */
private:
    const SDL_Scancode leftKey = SDL_SCANCODE_COUNT; /**< The key used for left inputs. */
    const SDL_Scancode rightKey = SDL_SCANCODE_COUNT; /**< The key used for right inputs. */
    const SDL_Scancode upKey = SDL_SCANCODE_COUNT; /**< The key used for up inputs. */
    const SDL_Scancode downKey = SDL_SCANCODE_COUNT; /**< The key used for down inputs. */

    const SDL_Scancode lightPunchKey = SDL_SCANCODE_COUNT; /**< The key used for light punch inputs. */
    const SDL_Scancode heavyPunchKey = SDL_SCANCODE_COUNT; /**< The key used for heavy punch inputs. */
    const SDL_Scancode lightKickKey = SDL_SCANCODE_COUNT; /**< The key used for light kick inputs. */
    const SDL_Scancode heavyKickKey = SDL_SCANCODE_COUNT; /**< The key used for heavy kick inputs. */
public:
    /**
     * Constructs a command input parser.
     * @param verticalSOCDIsUp Whether up+down is meant to be interpreted as up (@c true) or neutral (@c false).
     * @param leftKey The key used for left inputs.
     * @param rightKey The key used for right inputs.
     * @param upKey The key used for up inputs.
     * @param downKey The key used for down inputs.
     * @param lightPunchKey The key used for light punch inputs.
     * @param heavyPunchKey The key used for heavy punch inputs.
     * @param lightKickKey The key used for light kick inputs.
     * @param heavyKickKey The key used for heavy kick inputs.
     */
    BaseCommandInputParser(bool verticalSOCDIsUp,
                           SDL_Scancode leftKey,
                           SDL_Scancode rightKey,
                           SDL_Scancode upKey,
                           SDL_Scancode downKey,
                           SDL_Scancode lightPunchKey,
                           SDL_Scancode heavyPunchKey,
                           SDL_Scancode lightKickKey,
                           SDL_Scancode heavyKickKey);
    /**
     * Determines the current direction based on inputs and SOCD.
     * @return The current direction.
     */
    virtual Direction inputToDirection();
    /**
     * Updates the inputs based on the keys being pressed.
     */
    virtual void updateInput();

    /**
     * Destroys a command input parser.
     */
    virtual ~BaseCommandInputParser() = default;

    /**
     * Gets the current button inputs.
     * @return The buttons being held.
     */
    ButtonGroup& getButton();
    /**
     * Updates the button inputs.
     */
    virtual void setButtons();

    /**
     * Gets the current SOCD.
     * @return Whether up+down is meant to be interpreted as up (@c true) or neutral (@c false).
     */
    bool getSOCD() const;

    /**
     * Gets the left input.
     * @return The current status of the left input.
     */
    bool getLeft() const;
    /**
     * Sets the left input.
     * @param newLeft The new left input.
     */
    void setLeft(bool newLeft);

    /**
     * Gets the right input.
     * @return The current status of the right input.
     */
    bool getRight() const;
    /**
     * Sets the right input.
     * @param newRight The new right input.
     */
    void setRight(bool newRight);

    /**
     * Gets the up input.
     * @return The current status of the up input.
     */
    bool getUp() const;
    /**
     * Sets the up input.
     * @param newUp The new up input.
     */
    void setUp(bool newUp);

    /**
     * Gets the down input.
     * @return The current status of the down input.
     */
    bool getDown() const;
    /**
     * Sets the down input.
     * @param newDown The new down input.
     */
    void setDown(bool newDown);

    /**
     * Updates the input history.
     * @return The input history.
     */
    InputHistory updateRecentInputs();
};

/**
 * An extension of @c BaseCommandInputParser to accommodate for controllers.
 */
class ControllerCommandInputParser : public BaseCommandInputParser {
private:
    SDL_GamepadButton lightPunchButton = SDL_GAMEPAD_BUTTON_INVALID; /**< The button to use for light punch. */
    SDL_GamepadButton heavyPunchButton = SDL_GAMEPAD_BUTTON_INVALID; /**< The button to use for heavy punch. */
    SDL_GamepadButton lightKickButton = SDL_GAMEPAD_BUTTON_INVALID; /**< The button to use for light kick. */
    SDL_GamepadButton heavyKickButton = SDL_GAMEPAD_BUTTON_INVALID; /**< The button to use for heavy kick. */
public:
    /**
     * Creates a controller.
     * @param controller The controller to use.
     * @param verticalSOCDIsUp Whether up+down is meant to be interpreted as up (@c true) or neutral (@c false).
     * @param lightPunchButton The button to use for light punch.
     * @param heavyPunchButton The button to use for heavy punch.
     * @param lightKickButton The button to use for light kick.
     * @param heavyKickButton The button to use for heavy kick.
     */
    ControllerCommandInputParser(SDL_Gamepad*& controller, bool verticalSOCDIsUp,
        SDL_GamepadButton lightPunchButton,
        SDL_GamepadButton heavyPunchButton,
        SDL_GamepadButton lightKickButton,
        SDL_GamepadButton heavyKickButton);
    /**
     * Destroys the controller being used.
     */
    ~ControllerCommandInputParser() override;
private:
    SDL_Gamepad* controller; /**< The controller being used. */
    /**
     * Gets the current direction based on D-pad and left analog stick inputs.
     * @return The current direction.
     */
    Direction inputToDirection() override;
public:
    /**
     * Updates input based on controller buttons.
     */
    void updateInput() override;
    /**
     * Updates the button inputs.
     */
    void setButtons() override;
};
