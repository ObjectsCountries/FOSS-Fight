#include "character.hpp"
#include "command_input_parser.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#define DEBUG_CONTROLLER false

#define CHAR_NAME Debuggy

#if DEBUG_CONTROLLER
#define CHAR_CONSTRUCT(name, nameLiteral) \
        name = new Character(#nameLiteral, renderer, \
        &controller, \
        ground);
#else
#define CHAR_CONSTRUCT(name,  nameLiteral) \
        name = new Character(#nameLiteral, renderer, \
        &kip, \
        ground);
#endif

constexpr int height = 720;
constexpr int width = height * 16 / 9;
constexpr int groundLength = 150;

constexpr unsigned int fpsDelay = 1000 / 60;

typedef char boxConstructionError;
typedef unsigned char boxRenderError;
typedef short paletteReadingError;
typedef unsigned short headerError;
typedef int frameConstructionError;
typedef unsigned int frameRenderError;
typedef long dataReadingError;

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow("FOSS Fight", width, height, SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        std::cerr << "Error initializing window: " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::cerr << "Error initializing renderer: " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    SDL_SetRenderLogicalPresentation(renderer, width, height,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND_PREMULTIPLIED);

    bool running = true;

#if DEBUG_CONTROLLER
    std::vector<ControllerCommandInputParser> controllers;
    int gamepadCount;
    SDL_JoystickID* gamepads = SDL_GetGamepads(&gamepadCount);
    std::vector<SDL_Gamepad*> pads;
    if (gamepads == nullptr) {
        std::cerr << "Error getting gamepads: " << SDL_GetError() << std::endl;
    } else if (gamepadCount == 0) {
        std::cerr << "Error: no gamepads detected (set DEBUG_CONTROLLER to false to use keyboard)" << std::endl;
        return 1;
    } else {
        for (int i = 0; i < gamepadCount; ++i) {
            pads.push_back(SDL_OpenGamepad(gamepads[i]));
            if (pads.at(i) == nullptr) {
                std::cerr << "Error opening gamepad " << i << ": " << SDL_GetError() << std::endl;
            }
            controllers.emplace_back(pads.at(i), true,
                    SDL_GAMEPAD_BUTTON_WEST,
                    SDL_GAMEPAD_BUTTON_NORTH,
                    SDL_GAMEPAD_BUTTON_SOUTH,
                    SDL_GAMEPAD_BUTTON_EAST);
        }
    }
    ControllerCommandInputParser controller = controllers.at(0);
#else
    BaseCommandInputParser kip(true,
        SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_SPACE, SDL_SCANCODE_S,
        SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K);
#endif

    const SDL_FRect* ground = new SDL_FRect(-1000, height - groundLength, width + 2000, groundLength + 1000);

    Character* CHAR_NAME = nullptr;
    try {
CHAR_CONSTRUCT(CHAR_NAME, Debuggy)
    } catch (const DataException<boxConstructionError>& e) {
        std::cerr << "ERROR constructing " << CHAR_NAME->name << "! (Box Construction Error)" << std::endl << e.what() << std::endl;
        return 1;
    } catch (const DataException<boxRenderError>& e) {
        std::cerr << "ERROR constructing " << CHAR_NAME->name << "! (Box Rendering Error)" << std::endl << e.what() << std::endl;
        return 1;
    } catch (const DataException<headerError>& e) {
        std::cerr << "ERROR constructing " << CHAR_NAME->name << "! (Header Error)" << std::endl << e.what() << std::endl;
        return 1;
    } catch (const DataException<frameConstructionError>& e) {
        std::cerr << "ERROR constructing " << CHAR_NAME->name << "! (Frame Construction Error)" << std::endl << e.what() << std::endl;
        return 1;
    } catch (const DataException<frameRenderError>& e) {
        std::cerr << "ERROR constructing " << CHAR_NAME->name << "! (Frame Rendering Error)" << std::endl << e.what() << std::endl;
        return 1;
    } catch (const DataException<dataReadingError>& e) {
        std::cerr << "ERROR constructing " << CHAR_NAME->name << "! (Data Reading Error)" << std::endl << e.what() << std::endl;
        return 1;
    } catch (const DataException<paletteReadingError>& e) {
        std::cerr << "ERROR constructing " << CHAR_NAME->name << "! (Palette Reading Error)" << std::endl << e.what() << std::endl;
        return 1;
    }


    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
#if DEBUG_CONTROLLER
                case SDL_EVENT_JOYSTICK_AXIS_MOTION:
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    dynamic_cast<ControllerCommandInputParser*>(debuggy->controller)->updateInput();
                    dynamic_cast<ControllerCommandInputParser*>(debuggy->controller)->setButtons();
                    break;
#else
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    CHAR_NAME->controller->updateInput();
                    CHAR_NAME->controller->setButtons();
                    break;
#endif
                default:
                    break;
            }
        }
        SDL_RenderClear(renderer);
        try {
            CHAR_NAME->render(renderer);
        } catch (const char* e) {
            std::cerr << "ERROR rendering: " << e << std::endl;
            return 1;
        }
        SDL_SetRenderDrawColor(renderer, 0xFFU, 0xFFU, 0xFFU, 0xFFU);
        SDL_RenderPresent(renderer);

        SDL_Delay(fpsDelay);
    }

#if DEBUG_CONTROLLER
    SDL_free(gamepads);
#endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
