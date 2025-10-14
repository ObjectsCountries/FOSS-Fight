#include "character.hpp"

#include "frect_helpers.hpp"

#include <algorithm>
#include <bitset>
#include <concepts>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

template <std::integral T>
std::string format_number(T number, const bool hex, const bool binary, const bool uppercase) {
    std::stringstream ss;
    if (uppercase) {
        ss << std::uppercase;
    }
    if (hex) {
        ss << std::hex << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2);
        if (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>) {
            ss << static_cast<short>(number);
        } else {
            ss << number;
        }
    } else if (binary) {
        ss << "0b" << std::setfill('0') << std::setw(sizeof(T) * 8);
        std::bitset<sizeof(T) * 8> bits(number);
        ss << bits;
    } else {
        if (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>) {
            ss << static_cast<short>(number);
        } else {
            ss << number;
        }
    }
    if (!std::is_signed_v<T>) {
        ss << (uppercase ? 'U' : 'u');
    }
    if (std::is_same_v<T, long> || std::is_same_v<T, unsigned long>) {
        ss << (uppercase ? 'L' : 'l');
    } else if (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        ss << (uppercase ? 'L' : 'l') << (uppercase ? 'L' : 'l');
    }
    return ss.str();
}

template <std::integral T>
DataException<T>::DataException(std::string origin, std::string error, const T data) : origin{std::move(origin)}, error{std::move(error)}, data{data} {
    std::stringstream ss;
    ss << "Origin: " << this->origin << std::endl << "Error: " << this->error << std::endl << "Data: " << format_number<T>(this->data) << std::endl;
    this->result = ss.str();
}

template <std::integral T>
const char* DataException<T>::what() const noexcept {
    return this->result.c_str();
}

template <std::integral T>
void Buffer<T>::assign(T datum) {
    if (this->height == nullptr) {
        if (this->width == nullptr) {
            if (this->y == nullptr) {
                if (this->x == nullptr) {
                    this->x = new T(datum);
                } else {
                    this->y = new T(datum);
                }
            } else {
                this->width = new T(datum);
            }
        } else {
            this->height = new T(datum);
        }
    } else {
        throw std::out_of_range(std::string("Tried to assign value ") + format_number(datum) + " to full buffer!");
    }
}

template <std::integral T>
SDL_FRect Buffer<T>::toFRect() const {
    if (this->x == nullptr) {
        throw DataException<T>(std::string(__PRETTY_FUNCTION__) + " while reading this->x", std::string("this->x is a nullptr"));
    } else if (this->y == nullptr) {
        throw DataException<T>(std::string(__PRETTY_FUNCTION__) + " while reading this->y", std::string("this->y is a nullptr"));
    } else if (this->width == nullptr) {
        throw DataException<T>(std::string(__PRETTY_FUNCTION__) + " while reading this->width", std::string("this->width is a nullptr"));
    } else if (this->height == nullptr) {
        throw DataException<T>(std::string(__PRETTY_FUNCTION__) + " while reading this->height", std::string("this->height is a nullptr"));
    } else {
        return SDL_FRect(static_cast<float>(*this->x),
                        static_cast<float>(*this->y),
                        static_cast<float>(*this->width),
                        static_cast<float>(*this->height));
    }
}

template <std::integral T>
void Buffer<T>::clear() {
    delete this->x;
    this->x = nullptr;
    delete this->y;
    this->y = nullptr;
    delete this->width;
    this->width = nullptr;
    delete this->height;
    this->height = nullptr;
}

CopyInformation::CopyInformation(unsigned short type, unsigned short index, uint8_t copyInfo) {
    this->animationType = static_cast<AnimationType>(type);
    this->index = index;
    this->copyFrameLength = copyInfo & (1 << 7);
    this->copySpriteSheetLocation = copyInfo & (1 << 6);
    this->copyOffset = copyInfo & (1 << 5);
    this->copyHurtboxes = copyInfo & (1 << 4);
    this->copyGrabAndCommandGrabBoxes = copyInfo & (1 << 3);
    this->copyThrowPushAndGroundCollisionBoxes = copyInfo & (1 << 2);
    this->copyProximityGuardBoxes = copyInfo & (1 << 1);
    this->copyHitboxes = copyInfo & (1 << 0);
    std::stringstream ss;
    ss << "Copying animation " << format_number(static_cast<unsigned short>(this->animationType)) << " at index " << this->index << "." << std::endl;
    ss << "Copying the following attributes (" << format_number(copyInfo, false, true, true) << "):" << std::endl;
    if (copyInfo == 0b00000000U) {
        ss << "None" << std::endl;
    }
    if (this->copyFrameLength) {
        ss << "Frame Length" << std::endl;
    }
    if (this->copySpriteSheetLocation) {
        ss << "Sprite Sheet Location" << std::endl;
    }
    if (this->copyOffset) {
        ss << "Offset" << std::endl;
    }
    if (this->copyHurtboxes) {
        ss << "Hurtboxes" << std::endl;
    }
    if (this->copyGrabAndCommandGrabBoxes) {
        ss << "Grab and Command Grab Boxes" << std::endl;
    }
    if (this->copyThrowPushAndGroundCollisionBoxes) {
        ss << "Throw, Push and Ground Collision Boxes" << std::endl;
    }
    if (this->copyProximityGuardBoxes) {
        ss << "Proximity Guard Boxes" << std::endl;
    }
    if (this->copyHitboxes) {
        ss << "Hitboxes" << std::endl;
    }
    this->result = ss.str();
}

const char* CopyInformation::what() const noexcept {
    return this->result.c_str();
}


bool boxTypeToColor(SDL_Renderer*& renderer, const BoxType boxType, const bool translucent) {
    const uint8_t alpha = translucent ? 0x80U : 0xFFU;
    switch (boxType) {
        case NULL_TERMINATOR:
            return true;
        case HURTBOX:
            return SDL_SetRenderDrawColor(renderer, 0x00U, 0x00U, 0xFFU, alpha); // blue
        case GRAB_COMMAND_GRAB:
            return SDL_SetRenderDrawColor(renderer, 0xFFU, 0xA5U, 0x00U, alpha); // orange
        case THROW_PUSH_GROUND_COLLISION:
            return SDL_SetRenderDrawColor(renderer, 0x00U, 0xFFU, 0x00U, alpha); // green
        case PROXIMITY_GUARD:
            return SDL_SetRenderDrawColor(renderer, 0xFFU, 0xFFU, 0x00U, alpha); // yellow
        case NON_CANCELABLE_HITBOX:
        case SPECIAL_CANCELABLE_HITBOX:
        case SUPER_CANCELABLE_HITBOX:
        case SPECIAL_SUPER_CANCELABLE_HITBOX:
            return SDL_SetRenderDrawColor(renderer, 0xFFU, 0x00U, 0x00U, alpha); // red
        default:
            throw DataException<unsigned char>(
                std::string(__PRETTY_FUNCTION__) +
                " while determining box color",
                std::string("Unknown box type"),
                static_cast<unsigned char>(boxType));
    }
}

Sprite::Sprite(SDL_IOStream*& stream, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet) {
    this->texture = SDL_CreateTexture(renderer, spriteSheet->format, SDL_TEXTUREACCESS_TARGET, spriteSheet->w, spriteSheet->h);
    if (this->texture == nullptr) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while assigning to this->texture", std::string(SDL_GetError()));
    }
    if (!SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST)) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while setting scale mode for this->texture", std::string(SDL_GetError()), SDL_SCALEMODE_NEAREST);
    }
    if (!SDL_UpdateTexture(this->texture, nullptr, spriteSheet->pixels, spriteSheet->pitch)) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while updating this->texture", std::string(SDL_GetError()), spriteSheet->pitch);
    }
    this->length = 0x0000U;
    if (!SDL_ReadU16BE(stream, &this->length)) {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to this->length", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
    }
    if (this->length >= 0xFF00U) {
        unsigned short animationIndex;
        unsigned short spriteIndex;
        if (!SDL_ReadU16BE(stream, &animationIndex)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while beginning to copy other sprite and assigning to animationIndex", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
        if (!SDL_ReadU16BE(stream, &spriteIndex)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while beginning to copy other sprite and assigning to spriteIndex", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
        throw CopyInformation(animationIndex, spriteIndex, static_cast<uint8_t>(this->length % 256));
    }
    unsigned short data;
    for (int i = 0; i < 4; ++i) {
        if (SDL_ReadU16BE(stream, &data)) {
            this->spriteSheetBuffer.assign(data);
        } else {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to data for buffer", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
    }
    this->spriteSheetArea = new SDL_FRect(this->spriteSheetBuffer.toFRect());
    this->spriteSheetBuffer.clear();
    if (!SDL_ReadS16BE(stream, &this->xOffset)) {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to this->xOffset", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
    }
    if (!SDL_ReadS16BE(stream, &this->yOffset)) {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to this->yOffset", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
    }
    unsigned short boxType;
    unsigned short count;
    signed short bufferItem;
    do {
        if (!SDL_ReadU16BE(stream, &boxType)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to boxType", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
        if (boxType != NULL_TERMINATOR) {
            if (!SDL_ReadU16BE(stream, &count)) {
                const std::string error(SDL_GetError());
                throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to count", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
            }
            for (unsigned short j = 0U; j < count; ++j) {
                for (int k = 0; k < 4; ++k) {
                    if (SDL_ReadS16BE(stream, &bufferItem)) {
                        this->boxBuffer.assign(bufferItem);
                    } else {
                        const std::string error(SDL_GetError());
                        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to bufferItem", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
                    }
                }
                this->boxes[static_cast<BoxType>(boxType)].emplace_back(new SDL_FRect(this->boxBuffer.toFRect()));
                this->boxesWithAbsoluteLocation[static_cast<BoxType>(boxType)].emplace_back(new SDL_FRect(this->boxBuffer.toFRect()));
                this->boxBuffer.clear();
            }
        }
    } while (boxType != NULL_TERMINATOR);
}


Sprite::Sprite(const Sprite& reference,
    SDL_IOStream*& stream,
    SDL_Renderer*& renderer,
    const SDL_Surface*& spriteSheet,
    const CopyInformation& copy) {
    this->texture = SDL_CreateTexture(renderer, spriteSheet->format, SDL_TEXTUREACCESS_TARGET, spriteSheet->w, spriteSheet->h);
    if (this->texture == nullptr) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to texture", std::string(SDL_GetError()));
    }
    if (!SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST)) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and setting texture scale mode", std::string(SDL_GetError()), SDL_SCALEMODE_NEAREST);
    }
    if (!SDL_UpdateTexture(this->texture, nullptr, spriteSheet->pixels, spriteSheet->pitch)) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and updating texture", std::string(SDL_GetError()), spriteSheet->pitch);
    }
    if (copy.copyFrameLength) {
        this->length = reference.length;
    } else {
        unsigned short spriteCount;
        if (SDL_ReadU16BE(stream, &spriteCount)) {
            this->length = spriteCount;
        } else {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to spriteCount", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
    }
    if (copy.copySpriteSheetLocation) {
        this->spriteSheetArea = new SDL_FRect(reference.spriteSheetArea->x,
            reference.spriteSheetArea->y,
            reference.spriteSheetArea->w,
            reference.spriteSheetArea->h);
    } else {
        unsigned short coordinate;
        for (int i = 0; i < 4; ++i) {
            if (SDL_ReadU16BE(stream, &coordinate)) {
                this->spriteSheetBuffer.assign(coordinate);
            } else {
                const std::string error(SDL_GetError());
                throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to coordinate", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
            }
        }
        this->spriteSheetArea = new SDL_FRect(this->spriteSheetBuffer.toFRect());
        this->spriteSheetBuffer.clear();
    }
    if (copy.copyOffset) {
        this->xOffset = reference.xOffset;
        this->yOffset = reference.yOffset;
    } else {
        if (!SDL_ReadS16BE(stream, &this->xOffset)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to this->xOffset", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
        if (!SDL_ReadS16BE(stream, &this->yOffset)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to this->yOffset", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
    }
    if (!copy.copyHurtboxes
        || !copy.copyGrabAndCommandGrabBoxes
        || !copy.copyThrowPushAndGroundCollisionBoxes
        || !copy.copyProximityGuardBoxes
        || !copy.copyHitboxes) {
        unsigned short boxType;
        unsigned short count;
        signed short bufferItem;
        do {
            if (!SDL_ReadU16BE(stream, &boxType)) {
                const std::string error(SDL_GetError());
                throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to boxType", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
            }
            if (boxType != NULL_TERMINATOR) {
                if (!SDL_ReadU16BE(stream, &count)) {
                    const std::string error(SDL_GetError());
                    throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to count", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
                }
                for (unsigned short j = 0x0000U; j < count; ++j) {
                    for (int k = 0; k < 4; ++k) {
                        if (SDL_ReadS16BE(stream, &bufferItem)) {
                            this->boxBuffer.assign(bufferItem);
                        } else {
                            const std::string error(SDL_GetError());
                            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a sprite and assigning to bufferItem", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
                        }
                    }
                    this->boxes[static_cast<BoxType>(boxType)].emplace_back(new SDL_FRect(this->boxBuffer.toFRect()));
                    this->boxesWithAbsoluteLocation[static_cast<BoxType>(boxType)].emplace_back(new SDL_FRect(this->boxBuffer.toFRect()));
                    this->boxBuffer.clear();
                }
            }
        } while (boxType != NULL_TERMINATOR);
    }
    bool mustCopy = false;
    for (const auto& [type, allBoxes] : reference.boxes) {
        for (const SDL_FRect* box : allBoxes) {
            switch (type) {
                case NULL_TERMINATOR:
                    mustCopy = false;
                    break;
                case HURTBOX:
                    mustCopy = copy.copyHurtboxes;
                    break;
                case GRAB_COMMAND_GRAB:
                    mustCopy = copy.copyGrabAndCommandGrabBoxes;
                    break;
                case THROW_PUSH_GROUND_COLLISION:
                    mustCopy = copy.copyThrowPushAndGroundCollisionBoxes;
                    break;
                case PROXIMITY_GUARD:
                    mustCopy = copy.copyProximityGuardBoxes;
                    break;
                case NON_CANCELABLE_HITBOX:
                case SPECIAL_CANCELABLE_HITBOX:
                case SUPER_CANCELABLE_HITBOX:
                case SPECIAL_SUPER_CANCELABLE_HITBOX:
                    mustCopy = copy.copyHitboxes;
                    break;
            }
            if (mustCopy) {
                this->boxes[type].emplace_back(new SDL_FRect(box->x, box->y, box->w, box->h));
                this->boxesWithAbsoluteLocation[type].emplace_back(new SDL_FRect(box->x, box->y, box->w, box->h));
            }
        }
    }
}

SDL_FRect* Sprite::getSpriteSheetArea() const {
    return this->spriteSheetArea;
}

unsigned short Sprite::getLength() const {
    return this->length;
}

void Sprite::destroyTexture() const {
    if (this->texture != nullptr) {
        SDL_DestroyTexture(this->texture);
    }
}


void Sprite::render(SDL_Renderer*& renderer, const SDL_FRect* location) const {
    if (!SDL_RenderTexture(renderer, this->texture, this->spriteSheetArea, location)) {
        throw DataException<unsigned int>(std::string(__PRETTY_FUNCTION__) + " while rendering sprite texture", std::string(SDL_GetError()));
    }
#if DEBUG_RENDER_BOXES
    for (const auto& [boxType, allBoxes] : this->boxes) {
        if (!boxTypeToColor(renderer, boxType, false)) {
            throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while setting box outline color", std::string(SDL_GetError()));
        }
        for (const SDL_FRect* box : allBoxes) {
            if (!SDL_RenderRect(renderer, box)) {
                throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while rendering box outline", std::string(SDL_GetError()));
            }
        }
        if (!boxTypeToColor(renderer, boxType, true)) {
            throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while setting box color", std::string(SDL_GetError()));
        }
        for (const SDL_FRect* box : allBoxes) {
            if (!SDL_RenderFillRect(renderer, box)) {
                throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while rendering box", std::string(SDL_GetError()));
            }
        }
    }
#endif
}

const SDL_FRect* Character::ground;

Character::Character(const char* name, SDL_Renderer*& renderer, const BaseCommandInputParser& controller, const SDL_FRect*& groundBox) :
    name{name}, inputs{InputHistory()}, controller{controller} {
    Character::ground = groundBox;
    unsigned short data;
    SDL_IOStream* sprites = SDL_IOFromFile(
    (std::string(SDL_GetBasePath()) + "../data/characters/" + this->name +
     ".png").c_str(), "rb");
    SDL_IOStream* ffFile = SDL_IOFromFile(
    (std::string(SDL_GetBasePath()) + "../data/characters/" + this->name +
     ".ff").c_str(), "rb");
    if (!SDL_ReadU16BE(ffFile, &data)) {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while reading header", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    if (data != 0xF055U) {
        throw DataException<unsigned short>(
            std::string(__PRETTY_FUNCTION__) + " while checking header", std::string("Invalid header"), data);
    }
    this->spriteSheet = IMG_Load_IO(sprites, true);
    if (this->spriteSheet == nullptr) {
        throw DataException<int>(
            std::string(__PRETTY_FUNCTION__) + " while loading sprite sheet", std::string(SDL_GetError()));
    }
    int sizeBits;
    if (SDL_ReadS32BE(ffFile, &sizeBits)) {
        this->size = reinterpret_cast<float&>(sizeBits);
    } else {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to sizeBits", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    int walkForwardSpeedBits;
    if (SDL_ReadS32BE(ffFile, &walkForwardSpeedBits)) {
        this->walkForwardSpeed = reinterpret_cast<float&>(walkForwardSpeedBits);
    } else {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to walkForwardSpeedBits", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    int walkBackwardSpeedBits;
    if (SDL_ReadS32BE(ffFile, &walkBackwardSpeedBits)) {
        this->walkBackwardSpeed = reinterpret_cast<float&>(walkBackwardSpeedBits);
    } else {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to walkBackwardSpeedBits", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    int jumpForwardXVelocityBits;
    if (SDL_ReadS32BE(ffFile, &jumpForwardXVelocityBits)) {
        this->jumpForwardXVelocity = reinterpret_cast<float&>(jumpForwardXVelocityBits);
    } else {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to jumpForwardXVelocityBits", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    int jumpBackwardXVelocityBits;
    if (SDL_ReadS32BE(ffFile, &jumpBackwardXVelocityBits)) {
        this->jumpBackwardXVelocity = reinterpret_cast<float&>(jumpBackwardXVelocityBits);
    } else {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to jumpBackwardXVelocityBits", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    int initialJumpVelocityBits;
    if (SDL_ReadS32BE(ffFile, &initialJumpVelocityBits)) {
        this->initialJumpVelocity = reinterpret_cast<float&>(initialJumpVelocityBits);
    } else {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to initialJumpVelocityBits", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    int gravityBits;
    if (SDL_ReadS32BE(ffFile, &gravityBits)) {
        this->gravity = reinterpret_cast<float&>(gravityBits);
    } else {
        const std::string error(SDL_GetError());
        throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to gravityBits", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
    }
    unsigned short animationIndex;
    unsigned short numberOfFrames;
    while (SDL_GetIOStatus(ffFile) != SDL_IO_STATUS_EOF) {
        if (!SDL_ReadU16BE(ffFile, &animationIndex)) { // reached EOF if function returns false
            break;
        }
        if (!SDL_ReadU16BE(ffFile, &numberOfFrames)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to numberOfFrames", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
        }
        for (unsigned short i = 0x0000U; i < numberOfFrames; ++i) {
            try {
                this->animations[static_cast<AnimationType>(animationIndex)].emplace_back(
                    ffFile, renderer, this->spriteSheet);
            } catch (const CopyInformation& e) {
                this->animations[static_cast<AnimationType>(animationIndex)].emplace_back(
                    this->animations[e.animationType].at(e.index), ffFile, renderer,
                    this->spriteSheet, e);
            }
        }
    }
    this->coordinates = new SDL_FRect(400.0f,
        Character::ground->y - this->animations.at(IDLE).at(0).getSpriteSheetArea()->h * this->size,
        this->animations.at(IDLE).at(0).getSpriteSheetArea()->w * this->size,
        this->animations.at(IDLE).at(0).getSpriteSheetArea()->h * this->size);
    this->renderCoordinates = new SDL_FRect(400.0f,
        Character::ground->y - this->animations.at(IDLE).at(0).getSpriteSheetArea()->h * this->size,
        this->animations.at(IDLE).at(0).getSpriteSheetArea()->w * this->size,
        this->animations.at(IDLE).at(0).getSpriteSheetArea()->h * this->size);
    for (std::vector<Sprite>& allSprites : this->animations | std::views::values) {
        for (Sprite& spriteItem : allSprites) {
            for (std::vector<SDL_FRect*>& boxItems : spriteItem.boxes | std::views::values) {
                for (SDL_FRect*& boxItem : boxItems) {
                    multiplySizeRect(boxItem, this->size);
                    changeLocationRect(boxItem, 400.0f, Character::ground->y - boxItem->h);
                }
            }
        }
    }
}

Character::~Character() {
    for (std::vector<Sprite>& sprites : this->animations | std::views::values) {
        for (const Sprite& spriteItem : sprites) {
            spriteItem.destroyTexture();
        }
    }
}

AnimationType Character::processAttacks() {
    if (this->controller.getButton().getLightPunch()) {
        this->controller.getButton().setLightPunch(false);
        switch (this->currentAnimation) {
            /*
            case CROUCH_TRANSITION:
            case CROUCH:
                return CROUCHING_LIGHT_PUNCH;
            case JUMP_FORWARD:
            case JUMP_NEUTRAL:
            case JUMP_BACKWARD:
                return JUMPING_LIGHT_PUNCH;
            */
            default:
                return STAND_LIGHT_PUNCH;
        }
    } else if (this->controller.getButton().getHeavyPunch()) {
        this->controller.getButton().setHeavyPunch(false);
        switch (this->currentAnimation) {
            /*
            case CROUCH_TRANSITION:
            case CROUCH:
                return CROUCHING_HEAVY_PUNCH;
            case JUMP_FORWARD:
            case JUMP_NEUTRAL:
            case JUMP_BACKWARD:
                return JUMPING_HEAVY_PUNCH;
            */
            default:
                return STAND_HEAVY_PUNCH;
        }
    }
    return NOTHING;
}

AnimationType Character::processInputs() {
    if (this->currentAttack == NOTHING) {
        this->currentAttack = this->processAttacks();
        if (this->currentAttack != NOTHING) {
            return this->currentAttack;
        }
    }
    if (this->currentAnimation == this->currentAttack) {
        if (this->spriteIndex >= this->animations.at(this->currentAnimation).at(this->frame).getLength() - 1
            && this->frame >= this->animations.at(this->currentAnimation).size() - 1) {
            this->currentAttack = NOTHING;
        } else {
            return this->currentAttack;
        }
    }
    if (this->midair) {
        if (this->currentAnimation != PRE_JUMP
            && this->currentAnimation != JUMP_FORWARD
            && this->currentAnimation != JUMP_NEUTRAL
            && this->currentAnimation != JUMP_BACKWARD) {
            return PRE_JUMP;
        }
        if (this->currentAnimation == PRE_JUMP && this->spriteIndex < this->animations.at(this->currentAnimation).at(this->frame).getLength()) {
            return PRE_JUMP;
        }
        AnimationType arc = JUMP_NEUTRAL;
        switch (this->jumpArc) {
            case UP_BACK:
                moveRect(this->coordinates, this->jumpBackwardXVelocity, this->currentYVelocity);
                for (std::vector<Sprite>& sprites : this->animations | std::views::values) {
                    for (Sprite& spr : sprites) {
                        for (std::vector<SDL_FRect*>& boxItems : spr.boxes | std::views::values) {
                            for (SDL_FRect*& boxItem : boxItems) {
                                moveRect(boxItem, this->jumpBackwardXVelocity, this->currentYVelocity);
                            }
                        }
                    }
                }
                this->currentXVelocity = this->jumpBackwardXVelocity;
                arc = JUMP_BACKWARD;
                break;
            case UP_FORWARD:
                moveRect(this->coordinates, this->jumpForwardXVelocity, this->currentYVelocity);
                for (std::vector<Sprite>& sprites : this->animations | std::views::values) {
                    for (Sprite& spr : sprites) {
                        for (std::vector<SDL_FRect*>& boxItems : spr.boxes | std::views::values) {
                            for (SDL_FRect*& boxItem : boxItems) {
                                moveRect(boxItem, this->jumpForwardXVelocity, this->currentYVelocity);
                            }
                        }
                    }
                }
                this->currentXVelocity = this->jumpForwardXVelocity;
                arc = JUMP_FORWARD;
                break;
            case UP:
                moveRect(this->coordinates, 0.0f, this->currentYVelocity);
                for (std::vector<Sprite>& sprites : this->animations | std::views::values) {
                    for (Sprite& spr : sprites) {
                        for (std::vector<SDL_FRect*>& boxItems : spr.boxes | std::views::values) {
                            for (SDL_FRect*& boxItem : boxItems) {
                                moveRect(boxItem, 0.0f, this->currentYVelocity);
                            }
                        }
                    }
                }
                this->currentXVelocity = 0.0f;
                arc = JUMP_NEUTRAL;
                break;
            default:
                break;
        }
        this->currentYVelocity += this->gravity;
        const auto it = std::ranges::find_if(this->animations.at(this->currentAnimation).at(this->frame).boxes,
                                       [](const auto& box) {
                                           return box.first == THROW_PUSH_GROUND_COLLISION;
                                       });
        if (it != (this->animations.at(this->currentAnimation).at(this->frame).boxes).end()) {
            if (std::ranges::any_of(it->second, [](SDL_FRect*& box) {return SDL_HasRectIntersectionFloat(box, Character::ground);})) {
                this->currentXVelocity = 0.0f;
                this->currentYVelocity = 0.0f;
                for (std::vector<Sprite>& sprites : this->animations | std::views::values) {
                    for (Sprite& spr : sprites) {
                        for (std::vector<SDL_FRect*>& boxItems : spr.boxes | std::views::values) {
                            for (SDL_FRect*& boxItem : boxItems) {
                                changeLocationRect(boxItem, boxItem->x, Character::ground->y - boxItem->h);
                            }
                        }
                    }
                }
                this->midair = false;
            }
        }
        return arc;
    } else {
        switch (this->controller.inputToDirection()) {
            case DOWN_BACK:
            case DOWN:
            case DOWN_FORWARD:
                if ((this->currentAnimation == CROUCH_TRANSITION || this->currentAnimation == CROUCH) && this->spriteIndex == this->animations.at(this->currentAnimation).at(this->frame).getLength()) {
                    return CROUCH;
                } else {
                    return CROUCH_TRANSITION;
                }
            case BACK:
                if (this->currentAnimation == CROUCH) {
                    return CROUCH_TRANSITION;
                } else if (this->currentAnimation == CROUCH_TRANSITION) {
                    if (this->spriteIndex == this->animations.at(this->currentAnimation).at(this->frame).getLength()) {
                        return WALK_BACKWARD;
                    } else {
                        return CROUCH_TRANSITION;
                    }
                }
                moveRect(this->coordinates, this->walkBackwardSpeed, 0.0f);
                for (std::vector<Sprite>& sprites : this->animations | std::views::values) {
                    for (Sprite& spr : sprites) {
                        for (std::vector<SDL_FRect*>& boxItems : spr.boxes | std::views::values) {
                            for (SDL_FRect*& boxItem : boxItems) {
                                moveRect(boxItem, this->walkBackwardSpeed, 0.0f);
                            }
                        }
                    }
                }
                this->currentXVelocity = this->walkBackwardSpeed;
                return WALK_BACKWARD;
            case NEUTRAL:
                if (this->currentAnimation == CROUCH) {
                    return CROUCH_TRANSITION;
                } else if (this->currentAnimation == CROUCH_TRANSITION) {
                    if (this->spriteIndex == this->animations.at(this->currentAnimation).at(this->frame).getLength()) {
                        return IDLE;
                    } else {
                        return CROUCH_TRANSITION;
                    }
                }
                this->currentXVelocity = 0.0f;
                return IDLE;
            case FORWARD:
                if (this->currentAnimation == CROUCH) {
                    return CROUCH_TRANSITION;
                } else if (this->currentAnimation == CROUCH_TRANSITION) {
                    if (this->spriteIndex == this->animations.at(this->currentAnimation).at(this->frame).getLength()) {
                        return WALK_BACKWARD;
                    } else {
                        return CROUCH_TRANSITION;
                    }
                }
                moveRect(this->coordinates, this->walkForwardSpeed, 0.0f);
                for (std::vector<Sprite>& spriteVector : this->animations | std::views::values) {
                    for (Sprite& spr : spriteVector) {
                        for (std::vector<SDL_FRect*>& boxItems : spr.boxes | std::views::values) {
                            for (SDL_FRect*& boxItem : boxItems) {
                                moveRect(boxItem, this->walkForwardSpeed, 0.0f);
                            }
                        }
                    }
                }
                this->currentXVelocity = this->walkForwardSpeed;
                return WALK_FORWARD;
            case UP_BACK:
            case UP:
            case UP_FORWARD:
                this->midair = true;
                this->jumpArc = this->controller.inputToDirection();
                this->currentYVelocity = this->initialJumpVelocity;
                break;
        }
    }
    return IDLE;
}


void Character::render(SDL_Renderer*& renderer) {
    SDL_SetRenderDrawColor(renderer, 0x80U, 0x80U, 0x80U, 0xFFU);
    SDL_RenderFillRect(renderer, Character::ground);
    this->currentAnimation = this->processInputs();
    if (this->previousAnimation != this->currentAnimation) {
        this->frame = 0UZ;
        this->spriteIndex = 0U;
        this->previousAction = this->previousAnimation;
    }
    if (this->spriteIndex >= this->animations.at(this->currentAnimation).at(this->frame).getLength()) {
        ++this->frame;
        this->spriteIndex = 0U;
    }
    if (this->frame >= this->animations.at(this->currentAnimation).size()) {
        this->frame = 0UZ;
    }
    changeDimensionsRect(this->coordinates,
        this->animations.at(this->currentAnimation).at(this->frame).getSpriteSheetArea()->w * this->size,
        this->animations.at(this->currentAnimation).at(this->frame).getSpriteSheetArea()->h * this->size);
    for (auto& [boxType, boxes] : this->animations.at(this->currentAnimation).at(this->frame).boxes) {
        for (size_t i = 0UZ; i < boxes.size(); ++i) {
            changeLocationRect(
                boxes.at(i),
                this->coordinates->x + this->animations.at(this->currentAnimation).at(this->frame).boxesWithAbsoluteLocation.at(boxType).at(i)->x * this->size,
                this->coordinates->y + this->animations.at(this->currentAnimation).at(this->frame).boxesWithAbsoluteLocation.at(boxType).at(i)->y * this->size);
        }
    }
    this->renderCoordinates->x = this->coordinates->x + this->animations.at(this->currentAnimation).at(this->frame).xOffset;
    this->renderCoordinates->y = this->coordinates->y + this->animations.at(this->currentAnimation).at(this->frame).yOffset;
    this->renderCoordinates->w = this->coordinates->w;
    this->renderCoordinates->h = this->coordinates->h;
    this->animations.at(this->currentAnimation).at(this->frame).render(renderer, this->renderCoordinates);
    this->previousAnimation = this->currentAnimation;
    ++this->spriteIndex;
}