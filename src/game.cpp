#include <iostream>
#include <algorithm>
#include "game.h"

const int MAX_FOOD_COUNT = 2;

SDL_Color HEAD_CS[] {
  {64,129,226, 0xFF},
  {28,192,39, 0xFF}
};

SDL_Color BODY_CS[] {
  {255,255,255, 0xFF},
  {255,0,0, 0xFF}
};

void Game::_RandomizeStartPos() {
    if (snakes.size() > MAX_NUMBER_OF_SNAKE) {
        throw std::runtime_error("Only support 2 snakes max");
    }
    std::unordered_map<int, int> pos;
    for (Snake &snake: snakes){
       int x = random_w(engine);
       int y = random_h(engine);
       if ((pos.count(x) > 0) && (pos[x] == y)){
          continue;
       }
       pos[x] = y;
       std::cout << "Start Pos: x: " << x << "y: " << y << std::endl;
       snake.head_x = x;
       snake.head_y = y;
    }
}

Game::Game(std::size_t grid_width,
               std::size_t grid_height,
               Renderer *renderer,
               MenuOpt mode,
               std::size_t frame_duration)
{
  // check game modes to initialize correct number of snake
  if (mode == SINGLE_PLAYER)
  {
    this->snakes.push_back(
        Snake(grid_width, grid_height, ARROW, HEAD_CS[0], BODY_CS[0]));
  }
  else if (mode == DUAL_PLAYERS)
  {
    this->snakes.push_back(
        Snake(grid_width, grid_height, ARROW, HEAD_CS[0], BODY_CS[0]));
    this->snakes.push_back(
        Snake(grid_width, grid_height, AWSD, HEAD_CS[1], BODY_CS[1]));
  }
  this->renderer = renderer;
  this->input_dispatcher = InputDispatcher(mode, snakes);
  this->engine = std::mt19937(dev());
  this->target_frame_duration = frame_duration;
  this->game_mode = mode;
  random_w = std::uniform_int_distribution<int>(0, static_cast<int>(grid_width));
  random_h = std::uniform_int_distribution<int>(0, static_cast<int>(grid_height));
  this->_RandomizeStartPos();
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
    //std::cout << "Dispatch input complete" << std::endl;
    Update();
    this->renderer->Render(snakes, foods);
    //std::cout << "Render complete" << std::endl;
    frame_end = SDL_GetTicks();

    if (game_mode == SINGLE_PLAYER && (!snakes[0].alive)){
        running = false;
        std::cout << "Game over" << std::endl;
        continue;
    } 

    if (game_mode == DUAL_PLAYERS && ((!snakes[0].alive) || (!snakes[1].alive))){
        running = false;
        std::cout << "Game over" << std::endl;
        continue;
    } 
    
    frame_count++;
    frame_duration = frame_end - frame_start;
  
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
  for(int i = 0; i < num_food; i++)
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

bool Game::_CheckSnakesCollied() {
    if (snakes.size() != MAX_NUMBER_OF_SNAKE) {
       throw std::runtime_error("Cannot not support not equal 2 snakes");
    } 
    //TODO: Naive methods for checking collide. Not supper efficient but should work for small games
    for (SDL_Point &p: snakes[0].body){
       int new_x = static_cast<int>(snakes[1].head_x);
       int new_y = static_cast<int>(snakes[1].head_y); 
       if (new_x == p.x && new_y == p.y) { return true;}
    }
    for (SDL_Point &p: snakes[1].body){
       int new_x = static_cast<int>(snakes[0].head_x);
       int new_y = static_cast<int>(snakes[0].head_y); 
       if (new_x == p.x && new_y == p.y) { return true;}
    }
    return static_cast<int>(snakes[1].head_y) == static_cast<int>(snakes[0].head_y) 
            && static_cast<int>(snakes[0].head_x) == static_cast<int>(snakes[1].head_x);
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
  // first time foods get placed in the map
  if (this->foods.size() == 0) {
     PlaceFood(MAX_FOOD_COUNT);
     return;
  }
  int eaten_food = 0;
  this->scores.clear();
  int remaining_food = 0;
  std::vector<SDL_Point> food_to_remove;


  for (int i = 0; i < snakes.size(); i++)
  {
    food_to_remove.clear();
    int new_x = static_cast<int>(snakes[i].head_x);
    int new_y = static_cast<int>(snakes[i].head_y);
    // Check if there's food over here
    for (int j = 0; j < foods.size(); j++)
    {  
      if (foods[j].x == new_x && foods[j].y == new_y)
      {
        snakes[i].score += 1;
        snakes[i].GrowBody();
        eaten_food += 1;
        food_to_remove.push_back(foods[j]);
      }
    }
    // update food lists 
    for(SDL_Point &food: food_to_remove){
         foods.erase(std::remove_if(foods.begin(), foods.end(), 
                      [food](SDL_Point i){return food.x == i.x && food.y == i.y;}));
    }
   
    snakes[i].Update();
    scores.push_back(snakes[i].score);
  }
  
  remaining_food = MAX_FOOD_COUNT - eaten_food;
  if (remaining_food < MAX_FOOD_COUNT){
    PlaceFood(MAX_FOOD_COUNT - remaining_food);
  }

  // check if two snakes collide 
  if (this->game_mode == DUAL_PLAYERS && this->_CheckSnakesCollied()) {
     //stop the game as there is a winner 
     std::cout<< "There is a winner" << std::endl;
     snakes[0].alive = false;
     snakes[1].alive = false;
  }
  
}
