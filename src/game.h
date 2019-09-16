#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "SDL.h"
#include "renderer.h"
#include "snake.h"
#include "input_dispatcher.h"

const int MAX_NUMBER_OF_SNAKE = 2;



class Game {
 public:

  Game(std::size_t grid_width, 
       std::size_t grid_height, 
       Renderer* renderer,
       MenuOpt game_mode,
       std::size_t frame_duration
       );

  void init(MenuOpt mode);
  void Run();
  std::vector<int> GetScore() { return scores; }
  int GetSize() const;


 private:
  std::vector<Snake> snakes;
  std::vector<SDL_Point> foods;
  InputDispatcher input_dispatcher;
  Renderer* renderer;
  std::size_t target_frame_duration;
  std::vector<int> scores; 
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  void PlaceFood(int num_food);
  void Update();
};

#endif