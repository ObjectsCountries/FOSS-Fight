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
std::string format_number(T number, const bool hex, const bool uppercase) {
    std::stringstream ss;
    if (hex) {
        ss << std::hex << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2);
    }
    if (uppercase) {
        ss << std::uppercase;
    }
    if (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>) {
        ss << static_cast<short>(number);
    } else {
        ss << number;
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
DataException<T>::DataException(std::string origin, std::string error, const T data) : origin{std::move(origin)}, error{std::move(error)}, data{data} {
    std::stringstream ss;
    ss << "Origin: " << this->origin << std::endl << "Data: " << format_number<T>(this->data) << std::endl << "Error: " << this->error;
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
            throw DataException<unsigned char>(
                std::string(__PRETTY_FUNCTION__) +
                " while determining box color",
                std::string("Unknown box type"),
                static_cast<unsigned char>(boxType));
    }
}

Box::Box(SDL_IOStream*& stream, const BoxType boxType, SDL_FRect*& copy) : boxType{boxType} {
    unsigned short data;
    for (int i = 0; i < 4; ++i) {
        if (SDL_ReadU16BE(stream, &data)) {
            if (data == 0xFFFFU) {
                if (copy == nullptr) {
                    throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying sprite box", std::string("copy is a nullptr"), SDL_TellIO(stream));
                } else {
                    this->buffer.clear();
                    this->rect = new SDL_FRect(copy->x, copy->y, copy->w, copy->h);
                    return;
                }
            } else {
                this->buffer.assign(data);
            }
        } else {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to data for buffer", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
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
    if (this->length >= 0xFFF0U) {
        unsigned short animationIndex;
        unsigned short frameIndex;
        if (!SDL_ReadU16BE(stream, &animationIndex)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while beginning to copy other frame and assigning to animationIndex", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
        if (!SDL_ReadU16BE(stream, &frameIndex)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while beginning to copy other frame and assigning to frameIndex", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
        throw std::pair(animationIndex, frameIndex);
    }
    unsigned short data;
    for (int i = 0; i < 4; ++i) {
        if (SDL_ReadU16BE(stream, &data)) {
            this->buffer.assign(data);
        } else {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to data for buffer", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
    }
    this->spriteSheetArea = new SDL_FRect(this->buffer.toFRect());
    this->buffer.clear();
    unsigned short count;
    for (unsigned short i = 0x0000U; i < BOX_TYPE_COUNT; ++i) {
        if (!SDL_ReadU16BE(stream, &count)) {
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to count", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
        }
        for (unsigned short j = 0U; j < count; ++j) {
            this->boxes.emplace_back(stream, static_cast<BoxType>(i), this->spriteSheetArea);
        }
    }
}


Frame::Frame(const Frame& reference,
    SDL_IOStream*& stream,
    SDL_Renderer*& renderer,
    const SDL_Surface*& spriteSheet) {
    this->texture = SDL_CreateTexture(renderer, spriteSheet->format, SDL_TEXTUREACCESS_TARGET, spriteSheet->w, spriteSheet->h);
    if (this->texture == nullptr) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while copying a frame and assigning to texture", std::string(SDL_GetError()));
    }
    if (!SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST)) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while copying a frame and setting texture scale mode", std::string(SDL_GetError()), SDL_SCALEMODE_NEAREST);
    }
    if (!SDL_UpdateTexture(this->texture, nullptr, spriteSheet->pixels, spriteSheet->pitch)) {
        throw DataException<int>(std::string(__PRETTY_FUNCTION__) + " while copying a frame and updating texture", std::string(SDL_GetError()), spriteSheet->pitch);
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
        unsigned short count;
        for (unsigned short i = 0U; i < BOX_TYPE_COUNT; ++i) {
            if (!SDL_ReadU16BE(stream, &count)) {
                const std::string error(SDL_GetError());
                throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while copying a frame and assigning to count", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(stream));
            }
            if (count == 0xFFFFU) {
                for (const Box& box : reference.boxes) {
                    if (i == box.getBoxType()) {
                        this->boxes.emplace_back(box.rect->x, box.rect->y, box.rect->w, box.rect->h, box.getBoxType());
                    }
                }
            } else {
                for (unsigned short j = 0U; j < count; ++j) {
                    this->boxes.emplace_back(stream, static_cast<BoxType>(i), this->spriteSheetArea);
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
        throw DataException<unsigned int>(std::string(__PRETTY_FUNCTION__) + " while rendering frame texture", std::string(SDL_GetError()));
    }
#if DEBUG_RENDER_BOXES
    for (const Box& box : this->boxes) {
        if (!boxTypeToColor(renderer, box.getBoxType(), false)) {
            throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while setting box outline color", std::string(SDL_GetError()));
        }
        if (!SDL_RenderRect(renderer, box.rect)) {
            throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while rendering box outline", std::string(SDL_GetError()));
        }
        if (!boxTypeToColor(renderer, box.getBoxType(), true)) {
            throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while setting box color", std::string(SDL_GetError()));
        }
        if (!SDL_RenderFillRect(renderer, box.rect)) {
            throw DataException<unsigned char>(std::string(__PRETTY_FUNCTION__) + " while rendering box", std::string(SDL_GetError()));
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
        throw DataException<unsigned short>(
            std::string(__PRETTY_FUNCTION__) + " while checking header", std::string("Invalid header"), data);
    }
    this->spriteSheet = IMG_Load_IO(sprites, true);
    if (this->spriteSheet == nullptr) {
        throw DataException<int>(
            std::string(__PRETTY_FUNCTION__) + " while loading sprite sheet", std::string(SDL_GetError()));
    }
    unsigned short animationIndex = 0x0000U;
    unsigned short numberOfFrames;
    while (SDL_GetIOStatus(ffFile) != SDL_IO_STATUS_EOF) {
        if (!SDL_ReadU16BE(ffFile, &numberOfFrames)) {
            /*
            const std::string error(SDL_GetError());
            throw DataException<long>(std::string(__PRETTY_FUNCTION__) + " while assigning to numberOfFrames", error.empty() ? std::string("Reached EOF") : error, SDL_TellIO(ffFile));
            */
            break;
        }
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
        ++animationIndex;
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
                moveRect(this->coordinates, this->backJumpXVelocity, this->currentYVelocity);
                for (std::vector<Frame>& frames : this->animations | std::views::values) {
                    for (Frame& fr : frames) {
                        for (Box& box : fr.boxes) {
                            moveRect(box.rect, this->backJumpXVelocity, this->currentYVelocity);
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
                                           return box.getBoxType() == HURTBOX;
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
    changeLocationRect(this->coordinates,
        this->animations.at(this->currentAnimation).at(this->sprite).boxes.at(0).rect->x,
        this->animations.at(this->currentAnimation).at(this->sprite).boxes.at(0).rect->y);
    changeDimensionsRect(this->coordinates,
        this->animations.at(this->currentAnimation).at(this->sprite).getSpriteSheetArea()->w * this->size,
        this->animations.at(this->currentAnimation).at(this->sprite).getSpriteSheetArea()->h * this->size);
    this->animations.at(this->currentAnimation).at(this->sprite).render(renderer, this->coordinates);
    this->previousAnimation = this->currentAnimation;
    ++this->frame;
}