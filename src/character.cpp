#include "character.hpp"

#include <concepts>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

template <std::integral T>
std::string format_number(T number, const bool hex) {
    std::stringstream ss;
    if (hex) {
        ss << std::hex << std::uppercase << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2);
    }
    ss << number;
    if (!std::is_signed_v<T>) {
        ss << 'U';
    }
    if (sizeof(T) >= 4) {
        ss << 'L';
    }
    if (sizeof(T) == 8) {
        ss << 'L';
    }
    return ss.str();
}

void setCoordinates(SDL_FRect** rect, const float x, const float y,
                    const float width, const float height) {
    (*rect)->x = x;
    (*rect)->y = y;
    (*rect)->w = width;
    (*rect)->h = height;
}

void multiplySize(SDL_FRect** rect, const float factor) {
    if (*rect != nullptr) {
        (*rect)->x *= factor;
        (*rect)->y *= factor;
        (*rect)->w *= factor;
        (*rect)->h *= factor;
    }
}

void changeLocation(SDL_FRect** rect, const float x, const float y) {
    if (*rect != nullptr) {
        (*rect)->x = x;
        (*rect)->y = y;
    }
}

template <std::integral T>
DataException<T>::DataException(const char* origin, const T data, const char* message) : origin{origin}, data{data}, message {message} {}

template <std::integral T>
const char* DataException<T>::what() const noexcept {
    std::stringstream ss;
    ss << "Origin: " << this->origin << std::endl << "SDL Error: " << this->message << std::endl << "Data: " << format_number(this->data, true) << std::endl;
    std::cout << ss.str() << std::endl;
    return ss.str().c_str();
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
        throw std::out_of_range(std::string("Tried to assign value ") + format_number(datum, true) + " to full buffer!");
    }
}

template <std::integral T>
SDL_FRect Buffer<T>::toFRect() const {
    return SDL_FRect(static_cast<float>(*this->x),
                     static_cast<float>(*this->y),
                     static_cast<float>(*this->width),
                     static_cast<float>(*this->height));
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

bool boxTypeToColor(SDL_Renderer*& renderer, BoxType boxType, const bool translucent) {
    const uint8_t alpha = translucent ? 0x80U : 0xFFU;
    switch (boxType) {
        case HURTBOX:
            return SDL_SetRenderDrawColor(renderer, 0x00U, 0x00U, 0xFFU, alpha);
        case HITBOX:
            return SDL_SetRenderDrawColor(renderer, 0xFFU, 0x00U, 0x00U, alpha);
        case GRAB:
            return SDL_SetRenderDrawColor(renderer, 0x00U, 0xFFU, 0x00U, alpha);
        case COMMAND_GRAB:
            return SDL_SetRenderDrawColor(renderer, 0xFFU, 0x00U, 0xFFU, alpha);
        case THROW_PUSH:
            return SDL_SetRenderDrawColor(renderer, 0x00U, 0xFFU, 0xFFU, alpha);
        case PROXIMITY_GUARD:
            return SDL_SetRenderDrawColor(renderer, 0xFFU, 0xFFU, 0x00U, alpha);
        default:
            throw "Invalid enum value!";
    }
}

Box::Box(SDL_IOStream*& stream, const BoxType boxType) : boxType{boxType} {
    unsigned short data;
    for (int i = 0; i < 4; ++i) {
        if (SDL_ReadU16BE(stream, &data)) {
            this->buffer.assign(data);
        } else {
            throw DataException("Box::Box(SDL_IOStream*&, const BoxType)", data, SDL_GetError());
        }
    }
    this->rect = new SDL_FRect(this->buffer.toFRect());
    this->buffer.clear();
}

SDL_FRect* Box::getRect() const {
    return this->rect;
}

BoxType Box::getBoxType() const {
    return this->boxType;
}

Frame::Frame(SDL_IOStream*& stream, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet) {
    this->texture = SDL_CreateTexture(renderer, spriteSheet->format, SDL_TEXTUREACCESS_TARGET, spriteSheet->w, spriteSheet->h);
    if (this->texture == nullptr) {
        throw SDL_GetError();
    }
    if (!SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST)) {
        throw SDL_GetError();
    }
    if (!SDL_UpdateTexture(this->texture, nullptr, spriteSheet->pixels, spriteSheet->pitch)) {
        throw SDL_GetError();
    }
    SDL_ReadU16BE(stream, &this->length);
    unsigned short data;
    for (int i = 0; i < 4; ++i) {
        if (SDL_ReadU16BE(stream, &data)) {
            this->buffer.assign(data);
        } else {
            throw DataException("Frame::Frame(SDL_IOStream*&, SDL_Renderer*&, const SDL_Surface*&) while assigning to data", data, SDL_GetError());
        }
    }
    this->spriteSheetArea = new SDL_FRect(this->buffer.toFRect());
    this->buffer.clear();
    unsigned short boxType;
    unsigned short count;
    for (unsigned short i = 0U; i < BOX_TYPE_COUNT; ++i) {
        if (!SDL_ReadU16BE(stream, &boxType)) {
            throw DataException("Frame::Frame(SDL_IOStream*&, SDL_Renderer*&, const SDL_Surface*&) while assigning to boxType", boxType, SDL_GetError());
        }
        if (!SDL_ReadU16BE(stream, &count)) {
            throw DataException("Frame::Frame(SDL_IOStream*&, SDL_Renderer*&, const SDL_Surface*&) while assigning to count", count, SDL_GetError());
        }
        for (unsigned short j = 0U; j < count; ++j) {
            this->boxes.push_back(Box(stream, static_cast<BoxType>(boxType)));
        }
    }
}

SDL_FRect* Frame::getSpriteSheetArea() const {
    return this->spriteSheetArea;
}

unsigned short Frame::getLength() const {
    return this->length;
}

std::vector<Box> Frame::getBoxes() const {
    return this->boxes;
}
void Frame::destroyTexture() const {
    if (this->texture != nullptr) {
        SDL_DestroyTexture(this->texture);
    }
}


void Frame::render(SDL_Renderer*& renderer, SDL_FRect* location, const float size) const {
    if (!SDL_RenderTexture(renderer, this->texture, this->spriteSheetArea, location)) {
        throw SDL_GetError();
    }
#if DEBUG_RENDER_BOXES
    for (Box box : this->boxes) {
        if (!boxTypeToColor(renderer, box.getBoxType(), false)) {
            throw SDL_GetError();
        }
        if (!SDL_RenderRect(renderer, box.getRect())) {
            throw SDL_GetError();
        }
        if (!boxTypeToColor(renderer, box.getBoxType(), true)) {
            throw SDL_GetError();
        }
        if (!SDL_RenderFillRect(renderer, box.getRect())) {
            throw SDL_GetError();
        }
    }
#endif
}

Animation::Animation(SDL_IOStream*& stream, const unsigned short frameCount, SDL_Renderer*& renderer, const SDL_Surface*& spriteSheet) {
    for (unsigned short i = 0; i < frameCount; ++i) {
        this->frames.push_back(Frame(stream, renderer, spriteSheet));
    }
}

std::vector<Frame> Animation::getFrames() const {
    return this->frames;
}

Character::Character(SDL_Renderer*& renderer, SDL_IOStream* sprites, SDL_IOStream* ffFile, SDL_FRect* coordinates, float size) : size{size} {
    unsigned short data;
    SDL_ReadU16BE(ffFile, &data);
    if (data != 0xF055U) {
        throw DataException("Character::Character(SDL_Renderer*&, SDL_IOStream*, SDL_IOStream*)", data, "Invalid header");
    }
    int8_t nameStr = 1;
    while (nameStr != 0) {
        SDL_ReadS8(ffFile, &nameStr);
        if (nameStr != 0) {
            this->name += static_cast<char>(nameStr);
        }
    }
    this->spriteSheet = IMG_Load_IO(sprites, true);
    if (this->spriteSheet == nullptr) {
        throw SDL_GetError();
    }
    unsigned short frameIndex;
    unsigned short numberOfFrames;
    while (SDL_GetIOStatus(ffFile) != SDL_IO_STATUS_EOF) {
        if (!SDL_ReadU16BE(ffFile, &frameIndex)) {
            break;
        }
        SDL_ReadU16BE(ffFile, &numberOfFrames);
        this->animations.push_back(Animation(ffFile, numberOfFrames, renderer, this->spriteSheet));
    }
    if (coordinates == nullptr) {
        this->coordinates = new SDL_FRect(*this->animations.at(0).getFrames().at(0).getSpriteSheetArea());
    } else {
        this->coordinates = coordinates;
    }
    multiplySize(&this->coordinates, this->size);
    for (const Animation& animation : this->animations) {
        for (Frame& frameItem : animation.getFrames()) {
            for (Box& box : frameItem.getBoxes()) {
                multiplySize(&box.rect, this->size);
            }
        }
    }
}

Character::~Character() {
    for (const Animation& animation : this->animations) {
        for (const Frame& frameItem : animation.getFrames()) {
            frameItem.destroyTexture();
        }
    }
}

void Character::render(SDL_Renderer*& renderer) {
    if (this->frame >= this->animations.at(this->currentAnimation).getFrames().at(this->sprite).getLength()) {
        ++this->sprite;
        this->frame = 0UZ;
    }
    if (this->sprite >= this->animations.at(this->currentAnimation).getFrames().size()) {
        this->sprite = 0UZ;
    }
    if (this->previousAnimation != this->currentAnimation) {
        this->sprite = 0UZ;
        this->frame = 0UZ;
    }
    this->animations.at(this->currentAnimation).getFrames().at(this->sprite).render(renderer, this->coordinates, this->size);
    this->previousAnimation = this->currentAnimation;
    ++this->frame;
}