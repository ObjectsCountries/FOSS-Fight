#pragma once

#include <ostream>
#include <vector>

enum Direction {
    DOWN_LEFT = 1,
    DOWN,
    DOWN_RIGHT,
    LEFT,
    NEUTRAL,
    RIGHT,
    UP_LEFT,
    UP,
    UP_RIGHT
};

class Button {
private:
    bool lightPunch = false;
    bool heavyPunch = false;
    bool lightKick = false;
    bool heavyKick = false;

    unsigned char toBitfield() const;
public:
    Button();
    ~Button();
    void setLightPunch(bool lightPunch);
    bool getLightPunch() const;

    void setHeavyPunch(bool heavyPunch);
    bool getHeavyPunch() const;

    void setLightKick(bool lightKick);
    bool getLightKick() const;

    void setHeavyKick(bool heavyKick);
    bool getHeavyKick() const;

    const char* toString() const;
};

bool operator==(const Button& lhs, const Button& rhs);

class InputHistoryEntry {
public:
    InputHistoryEntry(Direction direction, Button button);
    ~InputHistoryEntry();
    void incrementDuration();
    Direction getDirection() const;
    unsigned short getDuration() const;
    Button getButton() const;
private:
    Direction direction;
    unsigned short duration;
    Button button;
    friend std::ostream& operator<<(std::ostream& os, const InputHistoryEntry& ihe);
};

class InputHistory {
public:
    InputHistory();
    ~InputHistory();
    void addEntry(InputHistoryEntry entry);
    std::vector<InputHistoryEntry> getHistory();
private:
    std::vector<InputHistoryEntry> history;
};

bool operator==(const InputHistoryEntry& lhs, const InputHistoryEntry& rhs);