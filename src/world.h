#ifndef WORLD_H
#define WORLD_H

///////////////////////////////////////////////////////////////////////////////

#include "object.h"
#include <../include/SDL2/SDL.h>

///////////////////////////// Forward Declarations ////////////////////////////

class WorldState;

//////////////////////////////// Implementation ///////////////////////////////

class World {
  public:
    World();
    
    ~World();
    
    WorldState* getState();
    
    Ball& getBall();
    
    Paddle& getP1Paddle();
    
    Paddle& getP2Paddle();
    
    void setState(WorldState* state);
    
    void update();
    
    void render(double lag, SDL_Window* gWindow, SDL_Renderer* gRenderer); // lag in seconds
    
  private:
    WorldState* state_;
    
    Ball b = Ball();
    Paddle p1 = Paddle(Player::P_ONE);
    Paddle p2 = Paddle(Player::P_TWO);
};

///////////////////////////////////////////////////////////////////////////////

#endif