#include "character.hpp"

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
std::string format_number(T number, const bool hex) {
    std::stringstream ss;
    if (hex) {
        ss << std::hex << std::uppercase << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2);
    }
    ss << number;
    if (!std::is_signed_v<T>) {
        ss << 'U';
    }
    if (std::is_same_v<T, size_t>) {
        ss << 'Z';
    } else {
        if (sizeof(T) >= 4) {
            ss << 'L';
        }
        if (sizeof(T) == 8) {
            ss << 'L';
        }
    }
    return ss.str();
}

void setCoordinatesRect(SDL_FRect*& rect, const float x, const float y,
                    const float width, const float height) {
    if (rect != nullptr) {
        rect->x = x;
        rect->y = y;
        rect->w = width;
        rect->h = height;
    }
}

void multiplySizeRect(SDL_FRect*& rect, const float factor) {
    if (rect != nullptr) {
        rect->x *= factor;
        rect->y *= factor;
        rect->w *= factor;
        rect->h *= factor;
    }
}

void changeLocationRect(SDL_FRect*& rect, const float x, const float y) {
    if (rect != nullptr) {
        rect->x = x;
        rect->y = y;
    }
}

void changeDimensionsRect(SDL_FRect*& rect, const float width, const float height) {
    if (rect != nullptr) {
        rect->w = width;
        rect->h = height;
    }
}

void moveRect(SDL_FRect*& rect, const float dx, const float dy) {
    if (rect != nullptr) {
        rect->x += dx;
        rect->y += dy;
    }
}

template <std::integral T>
DataException<T>::DataException(const char* origin, const T data, const char* message) : origin{origin}, data{data}, message {message} {}

template <std::integral T>
const char* DataException<T>::what() const noexcept {
    std::stringstream ss;
    ss << "Origin: " << this->origin << std::endl << "SDL Error: " << this->message << std::endl << "Data: " << format_number(this->data, true) << std::endl;
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
}

Box::Box(const float x, const float y, const float width, const float height, const BoxType boxType) : boxType{boxType} {
    this->buffer = Buffer<unsigned short>();
    this->rect = new SDL_FRect(x, y, width, height);
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
    this->length = 0U;
    SDL_ReadU16BE(stream, &this->length);
    if (this->length == 0xFFFFU) {
        unsigned short animationIndex, frameIndex;
        SDL_ReadU16BE(stream, &animationIndex);
        SDL_ReadU16BE(stream, &frameIndex);
        throw std::pair(animationIndex, frameIndex);
    }
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
            this->boxes.emplace_back(stream, static_cast<BoxType>(boxType));
        }
    }
}


Frame::Frame(Frame& reference,
    SDL_IOStream*& stream,
    SDL_Renderer*& renderer,
    const SDL_Surface*& spriteSheet) {
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
    unsigned short frameCount;
    SDL_ReadU16BE(stream, &frameCount);
    if (frameCount == 0x0000U) {
        this->length = reference.length;
    } else {
        this->length = frameCount;
    }
    unsigned short copySpriteSheetLocation;
    SDL_ReadU16BE(stream, &copySpriteSheetLocation);
    if (copySpriteSheetLocation == 0x0001U) {
        this->spriteSheetArea = reference.spriteSheetArea;
    } else {
        unsigned short coordinate;
        for (int i = 0; i < 4; ++i) {
            SDL_ReadU16BE(stream, &coordinate);
            this->buffer.assign(coordinate);
        }
        this->spriteSheetArea = new SDL_FRect(this->buffer.toFRect());
        this->buffer.clear();
    }
    unsigned short copyBoxes;
    SDL_ReadU16BE(stream, &copyBoxes);
    if (copyBoxes == 0x0001U) {
        for (const Box& box : reference.boxes) {
            this->boxes.emplace_back(box.rect->x, box.rect->y, box.rect->w, box.rect->h, box.getBoxType());
        }
    } else {
        unsigned short boxType;
        unsigned short count;
        for (unsigned short i = 0U; i < BOX_TYPE_COUNT; ++i) {
            if (!SDL_ReadU16BE(stream, &boxType)) {
                throw DataException("Frame::Frame(SDL_IOStream*&, SDL_Renderer*&, const SDL_Surface*&) while assigning to boxType", boxType, SDL_GetError());
            }
            if (!SDL_ReadU16BE(stream, &count)) {
                throw DataException("Frame::Frame(SDL_IOStream*&, SDL_Renderer*&, const SDL_Surface*&) while assigning to count", count, SDL_GetError());
            }
            if (count == 0xFFFFU) {
                for (const Box& box : reference.boxes) {
                    if (i == box.getBoxType()) {
                        this->boxes.emplace_back(box.rect->x, box.rect->y, box.rect->w, box.rect->h, box.getBoxType());
                    }
                }
            } else {
                for (unsigned short j = 0U; j < count; ++j) {
                    this->boxes.emplace_back(stream, static_cast<BoxType>(boxType));
                }
            }
        }
    }
}

SDL_FRect* Frame::getSpriteSheetArea() const {
    return this->spriteSheetArea;
}

unsigned short Frame::getLength() const {
    return this->length;
}

void Frame::destroyTexture() const {
    if (this->texture != nullptr) {
        SDL_DestroyTexture(this->texture);
    }
}


void Frame::render(SDL_Renderer*& renderer, const SDL_FRect* location) const {
    if (!SDL_RenderTexture(renderer, this->texture, this->spriteSheetArea, location)) {
        throw SDL_GetError();
    }
#if DEBUG_RENDER_BOXES
    for (const Box& box : this->boxes) {
        if (!boxTypeToColor(renderer, box.getBoxType(), false)) {
            throw SDL_GetError();
        }
        if (!SDL_RenderRect(renderer, box.rect)) {
            throw SDL_GetError();
        }
        if (!boxTypeToColor(renderer, box.getBoxType(), true)) {
            throw SDL_GetError();
        }
        if (!SDL_RenderFillRect(renderer, box.rect)) {
            throw SDL_GetError();
        }
    }
#endif
}

const SDL_FRect* Character::ground;

Character::Character(const char* name, SDL_Renderer*& renderer, const BaseCommandInputParser& controller, const SDL_FRect*& groundBox, const float size) :
    name{name}, size{size}, inputs{InputHistory()}, controller{controller} {
    Character::ground = groundBox;
    unsigned short data;
    SDL_IOStream* sprites = SDL_IOFromFile(
    (std::string(SDL_GetBasePath()) + "../data/characters/" + this->name +
     ".png").c_str(), "rb");
    SDL_IOStream* ffFile = SDL_IOFromFile(
    (std::string(SDL_GetBasePath()) + "../data/characters/" + this->name +
     ".ff").c_str(), "rb");
    SDL_ReadU16BE(ffFile, &data);
    if (data != 0xF055U) {
        throw DataException(
            "Character::Character(SDL_Renderer*&, SDL_IOStream*, SDL_IOStream*)",
            data, "Invalid header");
    }
    this->spriteSheet = IMG_Load_IO(sprites, true);
    if (this->spriteSheet == nullptr) {
        throw SDL_GetError();
    }
    unsigned short animationIndex;
    unsigned short numberOfFrames;
    while (SDL_GetIOStatus(ffFile) != SDL_IO_STATUS_EOF) {
        if (!SDL_ReadU16BE(ffFile, &animationIndex)) {
            break;
        }
        SDL_ReadU16BE(ffFile, &numberOfFrames);
        for (unsigned short i = 0x0000U; i < numberOfFrames; ++i) {
            try {
                this->animations[animationIndex].emplace_back(
                    ffFile, renderer, this->spriteSheet);
            } catch (const std::pair<unsigned short, unsigned short>& e) {
                this->animations[animationIndex].emplace_back(
                    this->animations[e.first].at(e.second), ffFile, renderer,
                    this->spriteSheet);
            }
        }
    }
    this->coordinates = new SDL_FRect();
    setCoordinatesRect(this->coordinates,
        400.0f,
        Character::ground->y - this->animations.at(0).at(0).getSpriteSheetArea()->h * this->size,
        this->animations.at(0).at(0).getSpriteSheetArea()->w * this->size,
        this->animations.at(0).at(0).getSpriteSheetArea()->h * this->size);
    for (std::vector<Frame>& frames : this->animations | std::views::values) {
        for (Frame& frameItem : frames) {
            for (Box& box : frameItem.boxes) {
                multiplySizeRect(box.rect, this->size);
                changeLocationRect(box.rect, 400.0f, Character::ground->y - box.rect->h);
            }
        }
    }
}

Character::~Character() {
    for (std::vector<Frame>& frames : this->animations | std::views::values) {
        for (const Frame& frameItem : frames) {
            frameItem.destroyTexture();
        }
    }
}

size_t Character::processInputs() {
    if (this->midair) {
        switch (this->jumpArc) {
            case UP_LEFT:
                moveRect(this->coordinates, -1.0f * this->jumpXVelocity, this->currentYVelocity);
                for (std::vector<Frame>& frames : this->animations | std::views::values) {
                    for (Frame& fr : frames) {
                        for (Box& box : fr.boxes) {
                            moveRect(box.rect, -1.0f * this->jumpXVelocity, this->currentYVelocity);
                        }
                    }
                }
                break;
            case UP_RIGHT:
                moveRect(this->coordinates, this->jumpXVelocity, this->currentYVelocity);
                for (std::vector<Frame>& frames : this->animations | std::views::values) {
                    for (Frame& fr : frames) {
                        for (Box& box : fr.boxes) {
                            moveRect(box.rect, this->jumpXVelocity, this->currentYVelocity);
                        }
                    }
                }
                break;
            case UP:
                moveRect(this->coordinates, 0.0f, this->currentYVelocity);
                for (std::vector<Frame>& frames : this->animations | std::views::values) {
                    for (Frame& fr : frames) {
                        for (Box& box : fr.boxes) {
                            moveRect(box.rect, 0.0f, this->currentYVelocity);
                        }
                    }
                }
                break;
            default:
                break;
        }
        this->currentYVelocity += this->gravity;
        const auto it = std::ranges::find_if(this->animations.at(this->currentAnimation).at(this->sprite).boxes,
                                       [](const Box& box) {
                                           return box.getBoxType() == THROW_PUSH;
                                       });
        if (it != this->animations.at(this->currentAnimation).at(this->sprite).boxes.end()) {
            if (SDL_HasRectIntersectionFloat(it->rect, Character::ground)) {
                this->currentYVelocity = 0.0f;
                changeLocationRect(this->coordinates, this->coordinates->x, Character::ground->y - this->coordinates->h);
                for (std::vector<Frame>& frames : this->animations | std::views::values) {
                    for (Frame& fr : frames) {
                        for (Box& box : fr.boxes) {
                            changeLocationRect(box.rect, box.rect->x, ground->y - box.rect->h);
                        }
                    }
                }
                this->midair = false;
            }
        }
    } else {
        switch (this->controller.inputToDirection()) {
            case DOWN_LEFT:
            case DOWN:
            case DOWN_RIGHT:
                break;
            case LEFT:
                moveRect(this->coordinates, this->walkBackSpeed, 0.0f);
                for (std::vector<Frame>& frames : this->animations | std::views::values) {
                    for (Frame& fr : frames) {
                        for (Box& box : fr.boxes) {
                            changeLocationRect(box.rect, box.rect->x, ground->y - box.rect->h);
                            moveRect(box.rect, this->walkBackSpeed, 0.0f);
                        }
                    }
                }
                return 2UZ;
            case NEUTRAL:
                break;
            case RIGHT:
                moveRect(this->coordinates, this->speed, 0.0f);
                for (std::vector<Frame>& frames : this->animations | std::views::values) {
                    for (Frame& fr : frames) {
                        for (Box& box : fr.boxes) {
                            changeLocationRect(box.rect, box.rect->x, ground->y - box.rect->h);
                            moveRect(box.rect, this->speed, 0.0f);
                        }
                    }
                }
                return 1UZ;
            case UP_LEFT:
            case UP:
            case UP_RIGHT:
                this->midair = true;
                this->jumpArc = this->controller.inputToDirection();
                this->currentYVelocity = -1.0f * this->initialJumpVelocity;
                break;
        }
    }
    return 0UZ;
}


void Character::render(SDL_Renderer*& renderer) {
    this->currentAnimation = this->processInputs();
    if (this->previousAnimation != this->currentAnimation) {
        this->sprite = 0U;
        this->frame = 0UZ;
    }
    if (this->frame >= this->animations.at(this->currentAnimation).at(this->sprite).getLength()) {
        ++this->sprite;
        this->frame = 0UZ;
    }
    if (this->sprite >= this->animations.at(this->currentAnimation).size()) {
        this->sprite = 0U;
    }
    changeDimensionsRect(this->coordinates,
        this->animations.at(this->currentAnimation).at(this->sprite).getSpriteSheetArea()->w * this->size,
        this->animations.at(this->currentAnimation).at(this->sprite).getSpriteSheetArea()->h * this->size);
    this->animations.at(this->currentAnimation).at(this->sprite).render(renderer, this->coordinates);
    this->previousAnimation = this->currentAnimation;
    ++this->frame;
}