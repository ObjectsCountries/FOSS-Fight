#include "command_input_parser.hpp"

#include "input_history.hpp"

#include <SDL3/SDL_gamepad.h>

BaseCommandInputParser::BaseCommandInputParser(bool verticalSOCDIsUp)
    : verticalSOCDIsUp{verticalSOCDIsUp},
    button{Button()}, recentInputs{InputHistory()} {}

Direction BaseCommandInputParser::inputToDirection() { return NEUTRAL; }

BaseCommandInputParser::~BaseCommandInputParser() {}

Button BaseCommandInputParser::getButton() { return this->button; }

void BaseCommandInputParser::setButton(bool lightPunch, bool heavyPunch, bool lightKick, bool heavyKick) {
    this->button.setLightPunch(lightPunch);
    this->button.setHeavyPunch(heavyPunch);
    this->button.setLightKick(lightKick);
    this->button.setHeavyKick(heavyKick);
}

InputHistory BaseCommandInputParser::updateRecentInputs() {
    Direction mostRecentDirection = this->inputToDirection();
    Button currentButton = this->getButton();
    this->recentInputs.addEntry(InputHistoryEntry(mostRecentDirection, currentButton));
    return this->recentInputs;
}

bool BaseCommandInputParser::getSOCD() { return this->verticalSOCDIsUp; }
void BaseCommandInputParser::setSOCD(bool verticalSOCDIsUp) { this->verticalSOCDIsUp = verticalSOCDIsUp; }

ControllerCommandInputParser::ControllerCommandInputParser(SDL_Gamepad* controller,
                                       bool verticalSOCDIsUp)
    : controller{controller}, BaseCommandInputParser(verticalSOCDIsUp)
       {}

ControllerCommandInputParser::~ControllerCommandInputParser() { SDL_CloseGamepad(controller); }

Direction ControllerCommandInputParser::inputToDirection() {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTX) >=
            thresholdPositive ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_LEFT)) {
        left = true;
    }
    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTX) <=
            thresholdNegative ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) {
        right = true;
    }
    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTY) <=
            thresholdNegative ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_UP)) {
        up = true;
    }
    if (SDL_GetGamepadAxis(this->controller, SDL_GAMEPAD_AXIS_LEFTY) <=
            thresholdPositive ||
        SDL_GetGamepadButton(this->controller, SDL_GAMEPAD_BUTTON_DPAD_DOWN)) {
        down = true;
    }

    if (left && right) {
        left = false;
        right = false;
    }

    if (up && down) {
        if (this->getSOCD()) {
            up = true;
            down = false;
        } else {
            up = false;
            down = false;
        }
    }
    if (left && !right && !up && down) {
        return DOWN_LEFT;
    } else if (!left && !right && !up && down) {
        return DOWN;
    } else if (!left && right && !up && down) {
        return DOWN_RIGHT;
    } else if (left && !right && !up && !down) {
        return LEFT;
    } else if (!left && !right && !up && !down) {
        return NEUTRAL;
    } else if (!left && right && !up && !down) {
        return RIGHT;
    } else if (left && !right && up && !down) {
        return UP_LEFT;
    } else if (!left && !right && up && !down) {
        return UP;
    } else if (!left && right && up && !down) {
        return UP_RIGHT;
    } else {
        return NEUTRAL;
    }
}

KeyboardCommandInputParser::KeyboardCommandInputParser(bool verticalSOCDIsUp) :
      left{false},
      right{false},
      up{false},
      down{false},
      BaseCommandInputParser(verticalSOCDIsUp) {}

KeyboardCommandInputParser::~KeyboardCommandInputParser() {}

Direction KeyboardCommandInputParser::inputToDirection() {
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
        return DOWN_LEFT;
    } else if (!this->left && !this->right && !this->up && this->down) {
        return DOWN;
    } else if (!this->left && this->right && !this->up && this->down) {
        return DOWN_RIGHT;
    } else if (this->left && !this->right && !this->up && !this->down) {
        return LEFT;
    } else if (!this->left && !this->right && !this->up && !this->down) {
        return NEUTRAL;
    } else if (!this->left && this->right && !this->up && !this->down) {
        return RIGHT;
    } else if (this->left && !this->right && this->up && !this->down) {
        return UP_LEFT;
    } else if (!this->left && !this->right && this->up && !this->down) {
        return UP;
    } else if (!this->left && this->right && this->up && !this->down) {
        return UP_RIGHT;
    } else {
        return NEUTRAL;
    }
}

bool KeyboardCommandInputParser::getLeft() { return this->left; }

void KeyboardCommandInputParser::setLeft(bool left) { this->left = left; }

bool KeyboardCommandInputParser::getRight() { return this->right; }

void KeyboardCommandInputParser::setRight(bool right) { this->right = right; }

bool KeyboardCommandInputParser::getUp() { return this->up; }

void KeyboardCommandInputParser::setUp(bool up) { this->up = up; }

bool KeyboardCommandInputParser::getDown() { return this->down; }

void KeyboardCommandInputParser::setDown(bool down) { this->down = down; }
