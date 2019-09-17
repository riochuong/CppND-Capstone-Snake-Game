#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <sstream>
#include "game.h"
#include "renderer.h"

const uint32_t NUM_MENU = 3;
const char *FONT_FILE = "../font/FreeSans.ttf";
const char *MENU_TEXTS[NUM_MENU] = {"Single Player", "Dual Player", "Quit"};
const SDL_Color TEXT_COLORS[2] = {{255, 255, 255}, {255, 0, 0}};
const SDL_Color TEXT_GUIDE_COLORS[2] = {{64, 129, 226, 0xFF}, {28, 192, 39, 0xFF}};
const int BG_COLOR[3] = {0x0, 0x0, 0x0};
const float EPSILON_X = 300.;
const float EPSILON_Y = 20.;
const int MAX_PLAYERS = 2;
const int DELAY_B4_GAME = 10;

const char *PLAYER_1_GUIDE_STR[7] = {
    "PLAYER 1 KEYS", "Up: ARROW_UP", "Down: ARROW_DOWN", "Left: ARROW_LEFT", "Right: ARROW_RIGHT",
    "Increase speed: RIGHT SHIFT", "Decrease Speed: RIGHT CTRL"};

const char *PLAYER_2_GUIDE_STR[7] = {
    "PLAYER 2 KEYS", "Up: W", "Down: S", "Left: A", "Right: D",
    "Increase speed: SPACE BAR", "Decrease Speed: RIGHT ALT"};

/**
 * Show player guide before start the game ! 
*/
void ShowPlayerGuidePage(SDL_Surface *screen, SDL_Renderer *renderer, MenuOpt game_mode, int frame_duration)
{
  int iteration = DELAY_B4_GAME * 1000 / frame_duration;
  int num_iter_per_one_sec = 1000 / frame_duration;
  uint32_t time = 0;
  std::ostringstream buff;
  SDL_Surface *guide_p1[7];
  SDL_Surface *guide_p2[7];
  SDL_Rect pos[7];
  SDL_Rect timeCountPos;
  timeCountPos.y = 300;
  timeCountPos.x = 50;
  SDL_Event pollEvent;
  TTF_Font *MENUFONT = TTF_OpenFont(FONT_FILE, 20);
  if (!MENUFONT)
  {
    std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
  }
  // set black back ground
  SDL_FillRect(screen,
               &screen->clip_rect,
               SDL_MapRGB(screen->format, BG_COLOR[0], BG_COLOR[1], BG_COLOR[2]));
  // start render text guide
  for (int i = 0; i < 7; i++)
  {
    guide_p1[i] = TTF_RenderText_Solid(MENUFONT, PLAYER_1_GUIDE_STR[i], TEXT_GUIDE_COLORS[0]);
    pos[i].x = 15;
    pos[i].y = 25 + i * 25;
    SDL_BlitSurface(guide_p1[i], NULL, screen, &pos[i]);
  }

  // add second player guides if need to
  if (game_mode == DUAL_PLAYERS)
  {
    for (int i = 0; i < 7; i++)
    {
      guide_p2[i] = TTF_RenderText_Solid(MENUFONT, PLAYER_2_GUIDE_STR[i], TEXT_GUIDE_COLORS[1]);
      pos[i].x = screen->clip_rect.x + 350;
      pos[i].y = 25 + i * 25;
      SDL_BlitSurface(guide_p2[i], NULL, screen, &pos[i]);
    }
  }
  buff << "This screen will be close in " << DELAY_B4_GAME << "second(s). Press ESC to skip";
  const std::string tmpStr = buff.str();
  SDL_Surface *timeSurface = TTF_RenderText_Solid(MENUFONT, tmpStr.c_str(), TEXT_COLORS[1]);
  SDL_BlitSurface(timeSurface, NULL, screen, &timeCountPos);
  SDL_Texture *info_texture = SDL_CreateTextureFromSurface(renderer, screen);
  SDL_RenderCopy(renderer, info_texture, NULL, NULL);
  SDL_RenderPresent(renderer);
  // // // // create texture to be put on surface
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, screen);
  if (texture == NULL)
  {
    fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
  // wait for 10s

  bool break_out = false;
  for (int i = 0; i < iteration; i++)
  {
    // if user press enter
    if (break_out)
    {
      break;
    };
    time = SDL_GetTicks();
    while (SDL_PollEvent(&pollEvent))
    {
      if (SDL_KEYDOWN == pollEvent.type && pollEvent.key.keysym.sym == SDLK_ESCAPE)
      {
        break_out = true;
      }
    }
    //return DUAL_PLAYERS;
    if (frame_duration > (SDL_GetTicks() - time))
    {
      SDL_Delay(frame_duration - (SDL_GetTicks() - time));
    }
  }

  // cleanup surfaces
  for (int i = 0; i < 7; i++)
  {
    SDL_FreeSurface(guide_p1[i]);
  }
  if (game_mode == DUAL_PLAYERS)
  {
    for (int i = 0; i < 7; i++)
    {
      SDL_FreeSurface(guide_p2[i]);
    }
  }
  if (!timeSurface)
  {
    SDL_FreeSurface(timeSurface);
  };
  SDL_FreeSurface(screen);
}

void ShowFinalScores(SDL_Surface *screen,
                     SDL_Renderer *renderer,
                     MenuOpt game_mode,
                     std::vector<Snake> &snakes,
                     int frame_duration)
{
  uint32_t time = 0;
  int iteration = DELAY_B4_GAME * 1000 / frame_duration;
  std::ostringstream buff;
  SDL_Rect timeCountPos;
  timeCountPos.y = 300;
  timeCountPos.x = 50;
  SDL_Surface *timeSurface = NULL;
  SDL_Texture *info_texture = NULL;
  TTF_Font *MENUFONT = TTF_OpenFont(FONT_FILE, 30);
  SDL_Event pollEvent;
  if (!MENUFONT)
  {
    std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
  }
  // set black back ground
  SDL_FillRect(screen,
               &screen->clip_rect,
               SDL_MapRGB(screen->format, BG_COLOR[0], BG_COLOR[1], BG_COLOR[2]));
  if (game_mode == SINGLE_PLAYER)
  {
    buff << "Final Scores: " << snakes[0].score;
    const std::string tmpStr = buff.str();
    timeSurface = TTF_RenderText_Solid(MENUFONT, tmpStr.c_str(), TEXT_GUIDE_COLORS[0]);
    SDL_BlitSurface(timeSurface, NULL, screen, &timeCountPos);
    info_texture = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_RenderCopy(renderer, info_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
  else
  { // should be dual players
    
    if ((snakes[0].alive && !snakes[1].alive) ||  (!snakes[0].alive && !snakes[1].alive &&
                                                snakes[0].score > snakes[1].score))
    {
      buff << "Player 1 is the Winner !!!";
      const std::string tmpStr = buff.str();
      timeSurface = TTF_RenderText_Solid(MENUFONT, tmpStr.c_str(), TEXT_GUIDE_COLORS[0]);
    }
    else if ((snakes[1].alive && !snakes[0].alive) || (!snakes[0].alive && !snakes[1].alive 
                                                      && snakes[0].score < snakes[1].score))
    {
      buff << "Player 2 is the Winner !!!";
      const std::string tmpStr = buff.str();
      timeSurface = TTF_RenderText_Solid(MENUFONT, tmpStr.c_str(), TEXT_GUIDE_COLORS[1]);
    }
    else
    {
      buff << "The game is tied !!!";
      const std::string tmpStr = buff.str();
      timeSurface = TTF_RenderText_Solid(MENUFONT, tmpStr.c_str(), TEXT_COLORS[1]);
    }
    SDL_BlitSurface(timeSurface, NULL, screen, &timeCountPos);
    info_texture = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_RenderCopy(renderer, info_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
  
  for (int i = 0; i < iteration; i++){
     time = SDL_GetTicks();
      while (SDL_PollEvent(&pollEvent))
      {
          if (pollEvent.type == SDL_QUIT){
             if (timeSurface) { SDL_FreeSurface(timeSurface);};
             SDL_FreeSurface(screen);
             exit(EXIT_SUCCESS);
          }
      }
      //return DUAL_PLAYERS;
    if (frame_duration > (SDL_GetTicks() - time))
    {
      SDL_Delay(frame_duration - (SDL_GetTicks() - time));
    }
  }

  if (timeSurface) { SDL_FreeSurface(timeSurface);};
  SDL_FreeSurface(screen);
}

MenuOpt ShowMenuAndWait(SDL_Surface *screen, SDL_Renderer *renderer, int frame_duration)
{
  // show simple menu
  uint32_t time = 0;
  SDL_Surface *menus[NUM_MENU];
  SDL_Rect pos[NUM_MENU];
  SDL_Event pollEvent;
  SDL_Rect orig = {0, 0};
  int x = 0;
  int y = 0;
  MenuOpt result = QUIT;
  if (TTF_Init() == -1)
  {
    std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }
  TTF_Font *MENUFONT = TTF_OpenFont(FONT_FILE, 30);
  if (!MENUFONT)
  {
    std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
  }
  // render font
  for (int i = 0; i < NUM_MENU; i++)
  {
    menus[i] = TTF_RenderText_Solid(MENUFONT, MENU_TEXTS[i], TEXT_COLORS[0]);
    pos[i].x = screen->clip_rect.w / NUM_MENU - menus[i]->clip_rect.w / NUM_MENU;
    pos[i].y = screen->clip_rect.h / NUM_MENU - menus[i]->clip_rect.h / NUM_MENU + i * 50;
  }
  // fill rect with colors
  SDL_FillRect(screen,
               &screen->clip_rect,
               SDL_MapRGB(screen->format, BG_COLOR[0], BG_COLOR[1], BG_COLOR[2]));

  for (int i = 0; i < NUM_MENU; i += 1)
  {
    SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, screen);
  if (texture == NULL)
  {
    fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
  // BLIT SURFACE
  while (1)
  {
    time = SDL_GetTicks();
    // keep checking for options selected from users
    while (SDL_PollEvent(&pollEvent))
    {
      switch (pollEvent.type)
      {
      case SDL_QUIT:
        result = QUIT;
        break;
      case SDL_MOUSEBUTTONDOWN:
        x = pollEvent.button.x;
        y = pollEvent.button.y;
        for (int opt = 0; opt < NUM_MENU; opt++)
        {
          std::cout << "x: " << x << "pos x: " << pos[opt].x << std::endl;
          if ((abs(x - pos[opt].x) <= EPSILON_X) && (abs(y - pos[opt].y) <= EPSILON_Y))
          {
            switch (opt)
            {
            case SINGLE_PLAYER:
              result = SINGLE_PLAYER;
              goto clean_and_return;
            case DUAL_PLAYERS:
              result = DUAL_PLAYERS;
              goto clean_and_return;
            case QUIT:
              result = QUIT;
              goto clean_and_return;
            default:
              throw std::runtime_error("invalid options for game mode selected !");
            }
          }
        }
      }
    }
    //return DUAL_PLAYERS;
    if (frame_duration > (SDL_GetTicks() - time))
    {
      SDL_Delay(frame_duration - (SDL_GetTicks() - time));
    }
  }
// clean up surfaces
clean_and_return:
  for (SDL_Surface *sptr : menus)
  {
    SDL_FreeSurface(sptr);
  }
  SDL_FreeSurface(screen);
  // quit the game
  if (result == QUIT)
  {
    std::cout << "Exit game" << std::endl;
    exit(EXIT_SUCCESS);
  }
  return result;
}

int main()
{
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  SDL_Surface *screen = SDL_GetWindowSurface(renderer.GetWindowSurface());
  MenuOpt game_mode = ShowMenuAndWait(screen, renderer.GetSDLRenderer(), kMsPerFrame);
  ShowPlayerGuidePage(screen, renderer.GetSDLRenderer(), game_mode, kMsPerFrame);
  Game game(kGridWidth, kGridHeight, &renderer, game_mode, kMsPerFrame);
  game.Run();
  ShowFinalScores(screen, renderer.GetSDLRenderer(), game_mode, game.snakes, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  return 0;
}