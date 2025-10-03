#include "input_history.hpp"

#include <ostream>
#include <vector>

Button::Button() : lightPunch{false}, heavyPunch{false}, lightKick{false}, heavyKick{false} {}

Button::~Button() {}

unsigned char Button::toBitfield() const {
    return (this->heavyKick) | (this->heavyPunch << 1U) | (this->lightKick << 2U) | (this->lightPunch << 3U);
}

void Button::setLightPunch(bool lightPunch) { this->lightPunch = lightPunch; }
bool Button::getLightPunch() const { return this->lightPunch; }

void Button::setHeavyPunch(bool heavyPunch) { this->heavyPunch = heavyPunch; }
bool Button::getHeavyPunch() const { return this->heavyPunch; }

void Button::setLightKick(bool lightKick) { this->lightKick = lightKick; }
bool Button::getLightKick() const { return this->lightKick; }

void Button::setHeavyKick(bool heavyKick) { this->heavyKick = heavyKick; }
bool Button::getHeavyKick() const { return this->heavyKick; }

const char* Button::toString() const {
    switch (this->toBitfield()) {
        case 1U:
            return "HK";
        case 2U:
            return "HP";
        case 3U:
            return "HP+HK";
        case 4U:
            return "LK";
        case 5U:
            return "KK";
        case 6U:
            return "LK+HP";
        case 7U:
            return "LK+HP+HK";
        case 8U:
            return "LP";
        case 9U:
            return "LP+HK";
        case 10U:
            return "PP";
        case 11U:
            return "LP+HP+HK";
        case 12U:
            return "LP+LK";
        case 13U:
            return "LP+LK+HK";
        case 14U:
            return "LP+LK+HP";
        case 15U:
            return "PPKK";
        default:
            return "";
    }
}

bool operator==(const Button& lhs, const Button& rhs) {
    return lhs.getLightPunch() == rhs.getLightPunch() &&
           lhs.getHeavyPunch() == rhs.getHeavyPunch() &&
           lhs.getLightKick() == rhs.getLightKick() &&
           lhs.getHeavyKick() == rhs.getHeavyKick();
}

InputHistoryEntry::InputHistoryEntry(Direction direction, Button button)
    : direction{direction}, duration{0U}, button{button} {}

InputHistoryEntry::~InputHistoryEntry() {}

void InputHistoryEntry::incrementDuration() { if (this->duration < 9999U) { ++this->duration; } }

Direction InputHistoryEntry::getDirection() const { return this->direction; }

unsigned short InputHistoryEntry::getDuration() const { return this->duration; }

Button InputHistoryEntry::getButton() const { return this->button; }

std::ostream& operator<<(std::ostream& os, const InputHistoryEntry& ihe) {
    os << ihe.direction << ihe.button.toString() << ' ' << ihe.duration;
    return os;
}

InputHistory::InputHistory() : history{} {}

InputHistory::~InputHistory() {}

void InputHistory::addEntry(InputHistoryEntry entry) {
    if (!this->history.empty() && this->history.back() == entry) {
        this->history.back().incrementDuration();
    } else {
        this->history.push_back(entry);
    }
}

std::vector<InputHistoryEntry> InputHistory::getHistory() {
    return this->history;
}

bool operator==(const InputHistoryEntry& lhs, const InputHistoryEntry& rhs) {
    return lhs.getDirection() == rhs.getDirection() && lhs.getButton() == rhs.getButton();
}