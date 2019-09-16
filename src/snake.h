#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <SDL.h>

// add control type to snake 
typedef enum _snake_ctrl_type {
    ARROW = 0,
    AWSD = 1
  } SnakeCtrlType;

class InputConsumer {
    public:
        virtual void ConsumeInput (SDL_EventType event) = 0;
};

class Snake: public InputConsumer {
 public:
  
  enum class Direction { kUp, kDown, kLeft, kRight };
  
  
  const int DEFAULT_SPEED = 1;

  Snake(int grid_width, int grid_height, SnakeCtrlType control_mode)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2),
        control_mode(control_mode) {}

  void Update();
  
  void ConsumeInput(SDL_EventType event);

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  SnakeCtrlType control_mode;
  int score = 0;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif