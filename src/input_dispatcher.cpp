#include <iostream>
#include <SDL.h>
#include "input_dispatcher.h"


/**
 * Create map to dispatch correct input key to correct snakes depending on 
 * game mode
 */
InputDispatcher::InputDispatcher(MenuOpt menuOpt, std::vector<Snake> &snakes) {
    this->snakes = std::vector<Snake>(snakes);
    this->game_mode = menuOpt;
    Snake* arrow_snake = NULL;
    Snake* awsd_snake = NULL;

    if (menuOpt == SINGLE_PLAYER) {
        if (this->snakes.size() != 1){
            throw std::runtime_error("The number of snakes should be exactly 1");

        }
        this->consumer_map = {
            {SDLK_UP, &snakes[0]},
            {SDLK_DOWN, &snakes[0]},
            {SDLK_LEFT, &snakes[0]},
            {SDLK_RIGHT, &snakes[0]},
            {SDLK_RSHIFT, &snakes[0]}, // increase speed 
            {SDLK_RCTRL, &snakes[0]} // decrease speed 
        };
        
    } else if (menuOpt == DUAL_PLAYERS) {   
        // initialize two snakes wih both arrow and awsd constrols
        if (this->snakes.size() != 2){
            throw std::runtime_error("The number of snakes should be exactly 2");

        }
        // get figure out snake types
        for (Snake &snake: snakes) {
            if (snake.control_mode == ARROW) {
                arrow_snake = &snake;
            }
            else if (snake.control_mode == AWSD){
                awsd_snake = &snake;
            }
        }
        if (!arrow_snake || !awsd_snake) {
            throw std::runtime_error("Wrong snake control initialization !!");
        }

        this->consumer_map = {
            {SDLK_UP, arrow_snake},
            {SDLK_DOWN, arrow_snake},
            {SDLK_LEFT, arrow_snake},
            {SDLK_RIGHT, arrow_snake},
            {SDLK_RSHIFT, arrow_snake}, // increase speed 
            {SDLK_RCTRL, arrow_snake}, // decrease speed 
            {SDLK_w, awsd_snake},
            {SDLK_s, awsd_snake},
            {SDLK_a, awsd_snake},
            {SDLK_d, awsd_snake},
            {SDLK_SPACE, awsd_snake}, // increase speed 
            {SDLK_RALT, awsd_snake} // deccrease speed 
        };
    } else {
        throw std::runtime_error("Invalid Mode to Initialize Dispatcher");
    }
}

void InputDispatcher::DispatchInput() {
    SDL_Event e;
    InputConsumer* consumer = NULL; 
    while(SDL_PollEvent(&e)) {
        consumer = NULL; 
        if (e.type == SDL_KEYDOWN){
            if (consumer_map.count(e.key.keysym.sym) != 1){
                std::cout << "Key " << e.key.keysym.sym << " is not supported" << std::endl;
                continue;
            }
            consumer = consumer_map[e.key.keysym.sym];
            consumer->ConsumeInput(e);
        }
        else if (e.type == SDL_QUIT){
            exit(EXIT_SUCCESS);
        }       
    }
}