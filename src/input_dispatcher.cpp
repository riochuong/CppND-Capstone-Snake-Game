#include <iostream>
#include <SDL.h>
#include "input_dispatcher.h"



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
            std::cout << "Received Key " << e.key.keysym.sym << " is not supported" << std::endl;
            consumer = consumer_map[e.key.keysym.sym];
            consumer->ConsumeInput(e);
        }
        else if (e.type == SDL_QUIT){
            return;
        }       
    }
}


// void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
//                                  Snake::Direction opposite) const {
//   if (snake.direction != opposite || snake.size == 1) snake.direction = input;
//   return;
// }

// void Controller::HandleInput(bool &running, Snake &snake) const {
//   SDL_Event e;
//   while (SDL_PollEvent(&e)) {
//     if (e.type == SDL_QUIT) {
//       running = false;
//     } else if (e.type == SDL_KEYDOWN) {
//       switch (e.key.keysym.sym) {
//         case SDLK_UP:
//           ChangeDirection(snake, Snake::Direction::kUp,
//                           Snake::Direction::kDown);
//           break;

//         case SDLK_DOWN:
//           ChangeDirection(snake, Snake::Direction::kDown,
//                           Snake::Direction::kUp);
//           break;

//         case SDLK_LEFT:
//           ChangeDirection(snake, Snake::Direction::kLeft,
//                           Snake::Direction::kRight);
//           break;

//         case SDLK_RIGHT:
//           ChangeDirection(snake, Snake::Direction::kRight,
//                           Snake::Direction::kLeft);
//           break;
//       }
//     }
//   }
// }