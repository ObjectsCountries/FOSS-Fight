#pragma once

#include "input_history.hpp"

#include <SDL3/SDL_gamepad.h>

constexpr short thresholdPositive = 200;
constexpr short thresholdNegative = -1 * thresholdPositive;

class BaseCommandInputParser {
private:
    bool verticalSOCDIsUp;
    Button button;
    InputHistory recentInputs;
    virtual Direction inputToDirection();
public:
    BaseCommandInputParser(bool verticalSOCDIsUp);
    ~BaseCommandInputParser();

    Button getButton();
    void setButton(bool lightPunch, bool heavyPunch, bool lightKick, bool heavyKick);

    bool getSOCD();
    void setSOCD(bool verticalSOCDIsUp);

    InputHistory updateRecentInputs();
};

class ControllerCommandInputParser : public BaseCommandInputParser {
private:
    SDL_Gamepad* controller;
    Direction inputToDirection() override;
public:
    ControllerCommandInputParser(SDL_Gamepad* controller, bool verticalSOCDIsUp);
    ~ControllerCommandInputParser();
};

class KeyboardCommandInputParser : public BaseCommandInputParser {
private:
    bool left, right, up, down;
    Direction inputToDirection() override;
public:
    KeyboardCommandInputParser(bool verticalSOCDIsUp);
    ~KeyboardCommandInputParser();

    bool getLeft();
    void setLeft(bool left);

    bool getRight();
    void setRight(bool right);

    bool getUp();
    void setUp(bool up);

    bool getDown();
    void setDown(bool down);
};
