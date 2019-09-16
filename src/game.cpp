#include <iostream>
#include "game.h"

const int MAX_FOOD_COUNT = 2;

Game::Game(std::size_t grid_width,
               std::size_t grid_height,
               Renderer *renderer,
               MenuOpt game_mode,
               std::size_t frame_duration)
{
  // check game modes to initialize correct number of snake
  if (game_mode == SINGLE_PLAYER)
  {
    this->snakes.push_back(
        Snake(grid_width, grid_height, ARROW));
  }
  else if (game_mode == DUAL_PLAYERS)
  {
    this->snakes.push_back(
        Snake(grid_width, grid_height, ARROW));
    this->snakes.push_back(
        Snake(grid_width, grid_height, AWSD));
  }
  this->renderer = renderer;
  this->input_dispatcher = InputDispatcher(game_mode, snakes);
  this->engine = std::mt19937(dev());
  this->target_frame_duration = frame_duration;
  random_w = std::uniform_int_distribution<int>(0, static_cast<int>(grid_width));
  random_h = std::uniform_int_distribution<int>(0, static_cast<int>(grid_height));
}

// main loop running the game
void Game::Run()
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    input_dispatcher.DispatchInput();
    std::cout << "Dispatch input complete" << std::endl;
    Update();
    this->renderer->Render(snakes, foods);
    std::cout << "Render complete" << std::endl;
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // // After every second, update the window title.
    // if (frame_end - title_timestamp >= 1000)
    // {
    //   renderer->UpdateWindowTitle(scores, frame_count);
    //   frame_count = 0;
    //   title_timestamp = frame_end;
    // }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood(int num_food)
{
  int x, y;
  bool valid_loc = true;
  this->foods.clear();
  while (foods.size() < num_food)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    for (Snake &snake : snakes)
    {
      valid_loc &= !snake.SnakeCell(x, y);
    }
    if (valid_loc)
    {
      foods.push_back(SDL_Point{x, y});
    }
  }
}

void Game::Update()
{
  for (Snake &snake : snakes)
  {
    if (!snake.alive)
    {
      return;
    }
  }
  int food_count = 0;
  this->scores.clear();
  for (int i = 0; i < snakes.size(); i++)
  {
    snakes[i].Update();
    int new_x = static_cast<int>(snakes[i].head_x);
    int new_y = static_cast<int>(snakes[i].head_y);

    // Check if there's food over here
    for (SDL_Point &food : foods)
    {
      if (food.x == new_x && food.y == new_y)
      {
        snakes[i].score += 1;
        snakes[i].GrowBody();
        food_count += 1;
      }
    }
    // update scores
    scores.push_back(snakes[i].score);
  }
  std::cout << "Place " << food_count << " Food" << std::endl;
  PlaceFood(food_count);
}
