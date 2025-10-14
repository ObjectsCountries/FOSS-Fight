#include "command_input_parser.hpp"

#include "input_history.hpp"

#include <SDL3/SDL.h>

BaseCommandInputParser::BaseCommandInputParser(const bool verticalSOCDIsUp,
                           const SDL_Scancode leftKey,
                           const SDL_Scancode rightKey,
                           const SDL_Scancode upKey,
                           const SDL_Scancode downKey,
                           const SDL_Scancode lightPunchKey,
                           const SDL_Scancode heavyPunchKey,
                           const SDL_Scancode lightKickKey,
                           const SDL_Scancode heavyKickKey)
: verticalSOCDIsUp{verticalSOCDIsUp}, buttons{ButtonGroup()}, recentInputs{InputHistory()},
  left{false}, right{false}, up{false}, down{false},
  leftKey{leftKey}, rightKey{rightKey}, upKey{upKey}, downKey{downKey},
  lightPunchKey{lightPunchKey}, heavyPunchKey{heavyPunchKey}, lightKickKey{lightKickKey}, heavyKickKey{heavyKickKey} {}

Direction BaseCommandInputParser::inputToDirection() {
    if (this->left && this->right) {
        this->left = false;
        this->right = false;
    }

    if (this->up && this->down) {
        if (this->getSOCD()) {
            this->up = true;
            this->down = false;
        } else {
            this->up = false;
            this->down = false;
        }
    }
    if (this->left && !this->right && !this->up && this->down) {
        return DOWN_BACK;
    } else if (!this->left && !this->right && !this->up && this->down) {
        return DOWN;
    } else if (!this->left && this->right && !this->up && this->down) {
        return DOWN_FORWARD;
    } else if (this->left && !this->right && !this->up && !this->down) {
        return BACK;
    } else if (!this->left && !this->right && !this->up && !this->down) {
        return NEUTRAL;
    } else if (!this->left && this->right && !this->up && !this->down) {
        return FORWARD;
    } else if (this->left && !this->right && this->up && !this->down) {
        return UP_BACK;
    } else if (!this->left && !this->right && this->up && !this->down) {
        return UP;
    } else if (!this->left && this->right && this->up && !this->down) {
        return UP_FORWARD;
    } else {
        return NEUTRAL;
    }
}

void BaseCommandInputParser::updateInput() {
    this->left = SDL_GetKeyboardState(nullptr)[this->leftKey];
    this->right = SDL_GetKeyboardState(nullptr)[this->rightKey];
    this->up = SDL_GetKeyboardState(nullptr)[this->upKey];
    this->down = SDL_GetKeyboardState(nullptr)[this->downKey];
}

ButtonGroup& BaseCommandInputParser::getButton() { return this->buttons; }

void BaseCommandInputParser::setButtons() {
    this->buttons.setLightPunch(SDL_GetKeyboardState(nullptr)[this->lightPunchKey]);
    this->buttons.setHeavyPunch(SDL_GetKeyboardState(nullptr)[this->heavyPunchKey]);
    this->buttons.setLightKick(SDL_GetKeyboardState(nullptr)[this->lightKickKey]);
    this->buttons.setHeavyKick(SDL_GetKeyboardState(nullptr)[this->heavyKickKey]);
}

bool BaseCommandInputParser::getSOCD() const { return this->verticalSOCDIsUp; }

bool BaseCommandInputParser::getLeft() const { return this->left; }

void BaseCommandInputParser::setLeft(const bool newLeft) { this->left = newLeft; }

bool BaseCommandInputParser::getRight() const { return this->right; }

void BaseCommandInputParser::setRight(const bool newRight) { this->right = newRight; }

bool BaseCommandInputParser::getUp() const { return this->up; }

void BaseCommandInputParser::setUp(const bool newUp) { this->up = newUp; }

bool BaseCommandInputParser::getDown() const { return this->down; }

void BaseCommandInputParser::setDown(const bool newDown) { this->down = newDown; }

InputHistory BaseCommandInputParser::updateRecentInputs() {
    Direction mostRecentDirection = this->inputToDirection();
    ButtonGroup currentButton = this->getButton();
    this->recentInputs.addEntry(InputHistoryEntry(mostRecentDirection, currentButton));
    return this->recentInputs;
}

ControllerCommandInputParser::ControllerCommandInputParser(SDL_Gamepad* controller,
                                                           const bool verticalSOCDIsUp,
                                                           const SDL_GamepadButton lightPunchButton,
                                                           const SDL_GamepadButton heavyPunchButton,
                                                           const SDL_GamepadButton lightKickButton,
                                                           const SDL_GamepadButton heavyKickButton)
    : controller{controller},
      lightPunchButton{lightPunchButton},
      heavyPunchButton{heavyPunchButton},
      lightKickButton{lightKickButton},
      heavyKickButton{heavyKickButton},
      BaseCommandInputParser(verticalSOCDIsUp,
                             SDL_SCANCODE_COUNT,
                             SDL_SCANCODE_COUNT,
                             SDL_SCANCODE_COUNT,
                             SDL_SCANCODE_COUNT,
                             SDL_SCANCODE_COUNT,
                             SDL_SCANCODE_COUNT,
                             SDL_SCANCODE_COUNT,
                             SDL_SCANCODE_COUNT)
       {}

ControllerCommandInputParser::~ControllerCommandInputParser() { SDL_CloseGamepad(controller); }

Direction ControllerCommandInputParser::inputToDirection() {

    if (this->left && this->right) {
        this->left = false;
        this->right = false;
    }

    if (this->up && this->down) {
        if (this->getSOCD()) {
            this->up = true;
            this->down = false;
        } else {
            this->up = false;
            this->down = false;
        }
    }
    if (this->left && !this->right && !this->up && this->down) {
        return DOWN_BACK;
    } else if (!this->left && !this->right && !this->up && this->down) {
        return DOWN;
    } else if (!this->left && this->right && !this->up && this->down) {
        return DOWN_FORWARD;
    } else if (this->left && !this->right && !this->up && !this->down) {
        return BACK;
    } else if (!this->left && !this->right && !this->up && !this->down) {
        return NEUTRAL;
    } else if (!this->left && this->right && !this->up && !this->down) {
        return FORWARD;
    } else if (this->left && !this->right && this->up && !this->down) {
        return UP_BACK;
    } else if (!this->left && !this->right && this->up && !this->down) {
        return UP;
    } else if (!this->left && this->right && this->up && !this->down) {
        return UP_FORWARD;
    } else {
        return NEUTRAL;
    }
}

void ControllerCommandInputParser::updateInput() {
    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTX) >=
            thresholdPositive ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_LEFT)) {
        this->left = true;
    }
    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTX) <=
            thresholdNegative ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) {
        this->right = true;
    }
    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTY) <=
            thresholdNegative ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_UP)) {
        this->up = true;
    }
    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTY) <=
            thresholdPositive ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_DOWN)) {
        this->down = true;
    }
}

void ControllerCommandInputParser::setButtons() {
    this->buttons.setLightPunch(SDL_GetGamepadButton(this->controller, this->lightPunchButton));
    this->buttons.setHeavyPunch(SDL_GetGamepadButton(this->controller, this->heavyPunchButton));
    this->buttons.setLightKick(SDL_GetGamepadButton(this->controller, this->lightKickButton));
    this->buttons.setHeavyKick(SDL_GetGamepadButton(this->controller, this->heavyKickButton));
}
