#pragma once

#include "input_history.hpp"

#include <SDL3/SDL.h>

constexpr short thresholdPositive = 200;
constexpr short thresholdNegative = -1 * thresholdPositive;

class BaseCommandInputParser {
private:
    bool verticalSOCDIsUp;
    Button button;
    InputHistory recentInputs;
public:
    virtual Direction inputToDirection();
    virtual void updateInput();
    bool left, right, up, down;
    BaseCommandInputParser(bool verticalSOCDIsUp);
    virtual ~BaseCommandInputParser();

    Button& getButton();
    void setButton(bool lightPunch, bool heavyPunch, bool lightKick, bool heavyKick);

    bool getSOCD() const;
    void setSOCD(bool verticalSOCDIsUp);

    bool getLeft() const;
    void setLeft(bool newLeft);

    bool getRight() const;
    void setRight(bool newRight);

    bool getUp() const;
    void setUp(bool newUp);

    bool getDown() const;
    void setDown(bool newDown);

    InputHistory updateRecentInputs();
};

class ControllerCommandInputParser : public BaseCommandInputParser {
private:
    SDL_Gamepad* controller;
    Direction inputToDirection() override;
public:
    void updateInput() override;
    ControllerCommandInputParser(SDL_Gamepad* controller, bool verticalSOCDIsUp);
    ~ControllerCommandInputParser() override;
};
