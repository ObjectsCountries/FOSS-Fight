#include "input_history.hpp"

#include <ostream>
#include <vector>

ButtonGroup::ButtonGroup() : lightPunch{false}, heavyPunch{false}, lightKick{false}, heavyKick{false} {}

unsigned char ButtonGroup::toBitfield() const {
    return (this->heavyKick) | (this->heavyPunch << 1U) | (this->lightKick << 2U) | (this->lightPunch << 3U);
}

void ButtonGroup::setLightPunch(bool lightPunch) { this->lightPunch = lightPunch; }
bool ButtonGroup::getLightPunch() const { return this->lightPunch; }

void ButtonGroup::setHeavyPunch(bool heavyPunch) { this->heavyPunch = heavyPunch; }
bool ButtonGroup::getHeavyPunch() const { return this->heavyPunch; }

void ButtonGroup::setLightKick(bool lightKick) { this->lightKick = lightKick; }
bool ButtonGroup::getLightKick() const { return this->lightKick; }

void ButtonGroup::setHeavyKick(bool heavyKick) { this->heavyKick = heavyKick; }
bool ButtonGroup::getHeavyKick() const { return this->heavyKick; }

const char* ButtonGroup::toString() const {
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

bool operator==(const ButtonGroup& lhs, const ButtonGroup& rhs) {
    return lhs.getLightPunch() == rhs.getLightPunch() &&
           lhs.getHeavyPunch() == rhs.getHeavyPunch() &&
           lhs.getLightKick() == rhs.getLightKick() &&
           lhs.getHeavyKick() == rhs.getHeavyKick();
}

InputHistoryEntry::InputHistoryEntry(Direction direction, ButtonGroup button)
    : direction{direction}, duration{0U}, button{button} {}

void InputHistoryEntry::incrementDuration() { if (this->duration < 9999U) { ++this->duration; } }

Direction InputHistoryEntry::getDirection() const { return this->direction; }

unsigned short InputHistoryEntry::getDuration() const { return this->duration; }

ButtonGroup InputHistoryEntry::getButton() const { return this->button; }

std::ostream& operator<<(std::ostream& os, const InputHistoryEntry& ihe) {
    os << ihe.direction << ihe.button.toString() << ' ' << ihe.duration;
    return os;
}

bool operator==(const InputHistoryEntry& lhs, const InputHistoryEntry& rhs) {
    return lhs.getDirection() == rhs.getDirection() && lhs.getButton() == rhs.getButton();
}

InputHistory::InputHistory() : history{} {}

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