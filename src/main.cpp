#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include "game.h"
#include "renderer.h"

const uint32_t NUM_MENU = 3;
const char *FONT_FILE = "./FreeSans.ttf";
const char *MENU_TEXTS[NUM_MENU] = {"Single Player", "Dual Player", "Quit"};
const SDL_Color color[2] = {{255, 255, 0}, {255, 0, 0}};
const int BG_COLOR[3] = {0x0, 0x0, 0x0};
const float EPSILON_X = 100.;
const float EPSILON_Y = 20.;

const char *PLAYER_1_GUIDE_STR = \
"PLAYER1 KEYS:\nUP: ARROW_UP\nDOWN: ARROW_DOWN\
\nLEFT:ARROW_LEFT\nRIGHT:ARROW_RIGHT\nINCREASE SPEED: RIGHT SHIFT\nDECREASE SPEED: RIGHT CTRL\n";

const char *PLAYER_2_GUIDE_STR = \
"PLAYER1 KEYS:\nUP: W\nDOWN: S\
\nLEFT:A\nRIGHT:D\nINCREASE SPEED: SPACE BAR\nDECREASE SPEED: RIGHT ALT\n";



void ShowDualPlayerGuidePage(SDL_Surface *screen, SDL_Renderer *renderer, int frame_duration) {

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
    menus[i] = TTF_RenderText_Solid(MENUFONT, MENU_TEXTS[i], color[0]);
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
        std::cout << "Mouse button down x: " << x << " y: " << y << std::endl;
        for (int opt = 0; opt < NUM_MENU; opt++)
        { 
          std::cout << "Pos x: " << pos[opt].x << " y:" << pos[opt].y << std::endl;
          if ((abs(x - pos[opt].x) <= EPSILON_X) && (abs(y - pos[opt].y) <= EPSILON_Y))
          {
            switch(opt){
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
    // quit the game 
    if (result == QUIT){
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
  Game game(kGridWidth, kGridHeight, &renderer, game_mode, kMsPerFrame);
  game.Run();

  std::cout << "Game has terminated successfully!\n";
  // std::cout << "Score: " << game.GetScores() << "\n";
  // std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}