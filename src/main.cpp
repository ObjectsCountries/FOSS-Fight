#include "command_input_parser.hpp"

#include <iostream>
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

    SDL_Texture* idleTex =
        IMG_LoadTexture(renderer, "data/characters/Debuggy.png");
    SDL_SetTextureScaleMode(idleTex, SDL_SCALEMODE_NEAREST);

    bool running = true;

    SDL_FRect* srcRect = new SDL_FRect{4.0f, 5.0f, 30.0f, 48.0f};
    SDL_FRect* dstRect = new SDL_FRect{0.0f, 0.0f, 60.0f, 96.0f};

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
    KeyboardCommandInputParser kip = KeyboardCommandInputParser(true);
#endif

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
#if !DEBUG_CONTROLLER
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    kip.setLeft(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]);
                    kip.setRight(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]);
                    kip.setUp(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_SPACE]);
                    kip.setDown(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]);
                    kip.setButton(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_U], SDL_GetKeyboardState(NULL)[SDL_SCANCODE_I], SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J], SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]);
                    break;
#endif
                /*
                case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                case SDL_EVENT_WINDOW_RESIZED:
                */
                default:
                    break;
            }
        }
#if DEBUG_CONTROLLER
        for (CommandInputParser cip : controllers) {
            std::cout << cip.updateRecentInputs();
        }
#else
        std::cout << kip.updateRecentInputs().getHistory().back() << std::endl;
#endif
        SDL_SetRenderDrawColor(renderer, 255U, 255U, 255U, 255U);

        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, idleTex, srcRect, dstRect);

        SDL_RenderPresent(renderer);
    }

#if DEBUG_CONTROLLER
    SDL_free(gamepads);
#endif
    SDL_DestroyTexture(idleTex);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
