#include "world.h"

#include "object.h"
#include "constants.h"
#include "state.h"
#include <../include/SDL2/SDL.h>

World::World() {}

World::~World() {
  delete state_;
}

WorldState* World::getState() {
  return state_;
}

Ball& World::getBall() {
  return b;
}
    
Paddle& World::getP1Paddle() {
  return p1;
}

Paddle& World::getP2Paddle() {
  return p2;
}

void World::setState(WorldState* state) {
  if (state_ == NULL) {
    state_ = new PongState(*this);
  }
  if (state != NULL) {
    delete state_;
    state_ = state;
  }
}

void World::update() {
  if (state_ == NULL) {
    state_ = new PongState(*this);
  }
  state_->update(*this);
}

void World::render(double lag, SDL_Window* gWindow, SDL_Renderer* gRenderer) {
  //Clear screen
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( gRenderer );

  //Render red filled quad
  SDL_Rect bkgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
  SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );    
  SDL_RenderFillRect( gRenderer, &bkgRect );

  //Render green outlined quad
  SDL_Rect ballRect = {(int)b.x(), (int)b.y(), (int)b.l(), (int)b.l()};
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );    
  SDL_RenderDrawRect( gRenderer, &ballRect );
  
  //Render green outlined quad
  SDL_Rect p1Rect = {(int)p1.x(), (int)p1.y(), (int)p1.w(), (int)p1.h()};
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );    
  SDL_RenderDrawRect( gRenderer, &p1Rect );
  
  //Render green outlined quad
  SDL_Rect p2Rect = {(int)p2.x(), (int)p2.y(), (int)p2.w(), (int)p2.h()};
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );    
  SDL_RenderDrawRect( gRenderer, &p2Rect );

  //Draw vertical line of yellow dots
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
  for( int i = 0; i < SCREEN_HEIGHT; i += 4 ) {
    SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
  }

  //Update screen
  SDL_RenderPresent( gRenderer );
  
}