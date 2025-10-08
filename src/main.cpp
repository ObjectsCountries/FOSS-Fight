#include "character.hpp"
#include "command_input_parser.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>

#define DEBUG_CONTROLLER false

constexpr int height = 720;
constexpr int width = height * 16 / 9;

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow("FOSS Fight", width, height, SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        std::cout << "Error initializing window: " << SDL_GetError()
                  << std::endl;
        return 1;
    }


    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::cout << "Error initializing renderer: " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    SDL_SetRenderLogicalPresentation(renderer, width, height,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND_PREMULTIPLIED);

    SDL_SetRenderVSync(renderer, 60);

    bool running = true;

#if DEBUG_CONTROLLER
    int* gamepadCount = nullptr;
    std::vector<ControllerCommandInputParser> controllers{};
    SDL_JoystickID* gamepads = SDL_GetGamepads(gamepadCount);

    if (gamepads == nullptr) {
        std::cout << "Error getting gamepads: " << SDL_GetError() << std::endl;
    } else {
        for (int i = 0; gamepads[i] != 0U; ++i) {
            controllers.push_back(
                CommandInputParser(SDL_OpenGamepad(gamepads[i]), false));
        }
    }
#else
    auto kip = KeyboardCommandInputParser(true);
#endif

    Character* debuggy = nullptr;
    try {
        debuggy = new Character(renderer,
                             SDL_IOFromFile(
                                 (std::string(SDL_GetBasePath())
                                     + "../data/characters/Debuggy.png").c_str(),
                                     "rb"),
                                   SDL_IOFromFile(
                                 (std::string(SDL_GetBasePath())
                                     + "../data/characters/Debuggy.ff").c_str(),
                                     "rb"),
                                 nullptr, 2.0f);
    } catch (const char* e) {
        std::cout << "ERROR constructing (const char*): " << e << std::endl;
        return 1;
    } catch (DataException<unsigned short>& e) {
        std::cout << "ERROR constructing (DataException&): " << e.what() << std::endl;
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
                case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                case SDL_EVENT_WINDOW_RESIZED:
                    break;
#else
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    kip.setLeft(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_A]);
                    kip.setRight(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_D]);
                    kip.setUp(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE]);
                    kip.setDown(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S]);
                    kip.setButton(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_U], SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_I], SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_J], SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_K]);
                    break;
#endif
                default:
                    break;
            }
        }
#if DEBUG_CONTROLLER
        for (CommandInputParser cip : controllers) {
            std::cout << cip.updateRecentInputs();
        }
#endif
        SDL_RenderClear(renderer);
        try {
            debuggy->render(renderer);
        } catch (const char* e) {
            std::cout << "ERROR rendering: " << e << std::endl;
            return 1;
        }
        SDL_SetRenderDrawColor(renderer, 0xFFU, 0xFFU, 0xFFU, 0xFFU);
        SDL_RenderPresent(renderer);
    }

#if DEBUG_CONTROLLER
    SDL_free(gamepads);
#endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
