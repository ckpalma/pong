#ifndef GAME_H
#define GAME_H

///////////////////////////////////////////////////////////////////////////////

#include "state.h"
#include "world.h"
#include <../include/SDL2/SDL.h>

///////////////////////////// Forward Declarations ////////////////////////////

class GameState;

//////////////////////////////// Implementation ///////////////////////////////

class Game {
  public:
    Game();
    ~Game();
    
    GameState* getState();
    
    WorldState* getWorldState();
    
    void setState(GameState* state);
    
    void update();
    
    void render(double lag, SDL_Window* window, SDL_Renderer* renderer); // lag in seconds
    
  private:
    GameState* state_;
    
    World world_;
};

///////////////////////////////////////////////////////////////////////////////

#endif