#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <unordered_map>
#include "snake.h"

typedef enum _menu_opt {
  SINGLE_PLAYER = 0,
  DUAL_PLAYERS = 1,
  QUIT = 2
} MenuOpt;

class InputDispatcher {

 public:
  InputDispatcher(){};
  InputDispatcher(MenuOpt menuOpt, std::vector<Snake> &snakes);
  void DispatchInput();

 private:
    MenuOpt game_mode; 
    std::vector<Snake> snakes;
    std::unordered_map<int, InputConsumer*> consumer_map; 
};

#endif  