#include "input_history.hpp"

#include <ostream>
#include <string>
#include <vector>

Button::Button() : lightPunch{false}, heavyPunch{false}, lightKick{false}, heavyKick{false} {}

Button::~Button() {}

bool operator==(const Button& lhs, const Button& rhs) {
    return lhs.getLightPunch() == rhs.getLightPunch() &&
           lhs.getHeavyPunch() == rhs.getHeavyPunch() &&
           lhs.getLightKick() == rhs.getLightKick() &&
           lhs.getHeavyKick() == rhs.getHeavyKick();
}

void Button::setLightPunch(bool lightPunch) { this->lightPunch = lightPunch; }
bool Button::getLightPunch() const { return this->lightPunch; }

void Button::setHeavyPunch(bool heavyPunch) { this->heavyPunch = heavyPunch; }
bool Button::getHeavyPunch() const { return this->heavyPunch; }

void Button::setLightKick(bool lightKick) { this->lightKick = lightKick; }
bool Button::getLightKick() const { return this->lightKick; }

void Button::setHeavyKick(bool heavyKick) { this->heavyKick = heavyKick; }
bool Button::getHeavyKick() const { return this->heavyKick; }

std::string Button::toString() const {
    std::string result("");
    if (this->lightPunch) {
        result += "LP";
    } else if (this->heavyPunch) {
        result += "HP";
    } else if (this->lightKick) {
        result += "LK";
    } else if (this->heavyKick) {
        result += "HK";
    }

    if (result.c_str() == "LPHP") {
        result = "PP";
    } else if (result.c_str() == "LKHK") {
        result = "KK";
    }
    return result;
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