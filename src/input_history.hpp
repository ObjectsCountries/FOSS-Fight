#pragma once

#include <ostream>
#include <vector>

/**
 * The current direction being held, using numpad notation.
 */
enum Direction {
    DOWN_BACK = 1,
    DOWN,
    DOWN_FORWARD,
    BACK,
    NEUTRAL,
    FORWARD,
    UP_BACK,
    UP,
    UP_FORWARD
};

/**
 * Represents the current buttons being pressed.
 */
class ButtonGroup {
private:
    bool lightPunch = false; /**< The light punch button. */
    bool heavyPunch = false; /**< The heavy punch button. */
    bool lightKick = false; /**< The light kick button. */
    bool heavyKick = false; /**< The heavy kick button. */

    /**
     * Converts buttons to numbers.
     * @return A number representing the current buttons being held.
     */
    unsigned char toBitfield() const;
public:
    /**
     * Constructs a button with all buttons as @c false .
     */
    ButtonGroup();
    /**
     * Destructs a button.
     */
    ~ButtonGroup() = default;
    /**
     * Sets the status of the light punch button.
     * @param lightPunch The new light punch value.
     */
    void setLightPunch(bool lightPunch);
    /**
     * Gets the value of the light punch button.
     * @return @c true if it's being pressed, @c false if not.
     */
    bool getLightPunch() const;

    /**
     * Sets the status of the heavy punch button.
     * @param heavyPunch The new heavy punch value.
     */
    void setHeavyPunch(bool heavyPunch);
    /**
     * Gets the value of the heavy punch button.
     * @return @c true if it's being pressed, @c false if not.
     */
    bool getHeavyPunch() const;

    /**
     * Sets the status of the light kick button.
     * @param lightKick The new light kick value.
     */
    void setLightKick(bool lightKick);
    /**
     * Gets the value of the light kick button.
     * @return @c true if it's being pressed, @c false if not.
     */
    bool getLightKick() const;

    /**
     * Sets the status of the heavy kick button.
     * @param heavyKick The new heavy kick value.
     */
    void setHeavyKick(bool heavyKick);
    /**
     * Gets the value of the heavy kick button.
     * @return @c true if it's being pressed, @c false if not.
     */
    bool getHeavyKick() const;

    /**
     * Converts the bitfield of buttons to a string.
     * @return A string representation of the current buttons being pressed.
     */
    const char* toString() const;
};

/**
 * Compares two button groups.
 * @param lhs The first button.
 * @param rhs The second button.
 * @return Whether these buttons are identical in which buttons are being pressed.
 */
bool operator==(const ButtonGroup& lhs, const ButtonGroup& rhs);

/**
 * One entry of input history.
 */
class InputHistoryEntry {
public:
    /**
     * Creates an entry in the input history.
     * @param direction The direction of this input.
     * @param button The button of this input.
     */
    InputHistoryEntry(Direction direction, ButtonGroup button);
    /**
     * Destroys an input history entry.
     */
    ~InputHistoryEntry() = default;
    /**
     * Adds 1 frame to the duration of this entry.
     */
    void incrementDuration();
    /**
     * Gets this entry's direction.
     * @return The direction associated with this entry.
     */
    Direction getDirection() const;
    /**
     * Gets this entry's duration.
     * @return The duration of this entry.
     */
    unsigned short getDuration() const;
    /**
     * Returns the current buttons.
     * @return This entry's buttons.
     */
    ButtonGroup getButton() const;
private:
    const Direction direction; /**< The direction of this entry. */
    unsigned short duration; /**< The duration of this entry. */
    const ButtonGroup button; /**< The buttons of this entry. */
    /**
     * Outputs the entry to a @c std::ostream& .
     * @param os The @c std::ostream& to output to.
     * @param ihe This entry.
     * @return The modified @c std::ostream& .
     */
    friend std::ostream& operator<<(std::ostream& os, const InputHistoryEntry& ihe);
};

bool operator==(const InputHistoryEntry& lhs, const InputHistoryEntry& rhs);

/**
 * A collection of @c InputHistoryEntry s.
 */
class InputHistory {
public:
    /**
     * Initializes input history.
     */
    InputHistory();
    /**
     * Destroys input history.
     */
    ~InputHistory() = default;
    /**
     * Adds an entry, incrementing the previous one if identical.
     * @param entry The entry to add.
     */
    void addEntry(InputHistoryEntry entry);
    /**
     * Gets the input history.
     * @return The history of the inputs.
     */
    std::vector<InputHistoryEntry> getHistory();
private:
    std::vector<InputHistoryEntry> history; /**< The history of the inputs. */
};