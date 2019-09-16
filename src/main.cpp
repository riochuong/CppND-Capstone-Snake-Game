#include <iostream>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "renderer.h"



const uint32_t NUM_MENU = 3;
const char *FONT_FILE = "./FreeSans.ttf";
const char* MENU_TEXTS [NUM_MENU] = {"Single Player", "Dual Player", "Quit"};
const SDL_Color color[2] = {{255,255,255}, {255,0,0}};
const int BG_COLOR [3] = {0x0, 0x0, 0x0};


MenuOpt ShowMenuAndWait(SDL_Surface* screen, int frame_rate) {
  // show simple menu 
  uint32_t time = 0;
  SDL_Surface* menus [NUM_MENU];
  SDL_Rect pos[NUM_MENU];
  SDL_Event pollEvent;
  if( TTF_Init() == -1 )
  {
      std::cout<<"SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError()<<std::endl;
      exit(EXIT_FAILURE);
  }
  TTF_Font* MENUFONT = TTF_OpenFont(FONT_FILE,12);
  if (!MENUFONT){
      std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
  }
  // render font 
  for(int i = 0; i < NUM_MENU; i++){
     menus[i] = TTF_RenderText_Solid(MENUFONT, MENU_TEXTS[i], color[0]); 
     pos[i].x = screen->clip_rect.w/NUM_MENU - menus[i]->clip_rect.w / NUM_MENU;
     pos[i].y = screen->clip_rect.h/NUM_MENU - menus[i]->clip_rect.h / NUM_MENU;
  }
  // fill rect with colors
  SDL_FillRect(screen, 
               &screen->clip_rect, 
               SDL_MapRGB(screen->format, BG_COLOR[0], BG_COLOR[1], BG_COLOR[2]));

  while(1) {
    time = SDL_GetTicks();
    // keep checking for options selected from users
    return DUAL_PLAYERS;
  }

  clean_up_surface:
    for (SDL_Surface* sptr: menus){
      SDL_FreeSurface(sptr);
    }
}


int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};


  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  SDL_Surface* screen = SDL_GetWindowSurface(renderer.GetWindowSurface());
  MenuOpt game_mode = ShowMenuAndWait(screen, kMsPerFrame);
  Game game(kGridWidth, kGridHeight, &renderer, game_mode, kMsPerFrame);
  game.Run();
  
  std::cout << "Game has terminated successfully!\n";
  // std::cout << "Score: " << game.GetScores() << "\n";
  // std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}