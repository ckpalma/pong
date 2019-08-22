#include "app.h"

#include "state.h"
#include "tools.h"
#include "game.h"
#include <../include/SDL2/SDL.h>
#include <iostream>

App::App()
: fps_(60.),
  closeWindow_(false)
{
  timer.start();
}

App::~App() {
  delete state_;
}

double App::getFPS() {
  return fps_;
}

GameState* App::getGameState() {
  GameState* gState = game_.getState();
  if (gState == NULL) {
    game_.setState(NULL);
  }
  return game_.getState();
}

void App::setState(AppState* state) {
  if (state_ == NULL) {
    state_ = new PlayState(*this);
  }
  if (state != NULL) {
    delete state_;
    state_ = state;
  }
}

void App::closeWindow() {
  closeWindow_ = true;
}

void App::handleInput() {
  if (state_ == NULL) {
    state_ = new PlayState(*this);
  }
  state_->handleInput(*this);
}

void App::update() {
  if (state_ == NULL) {
    state_ = new PlayState(*this);
  }
  state_->update(*this);
}

void App::render(double lag, SDL_Window* window, SDL_Renderer* renderer) {
  game_.render(lag,window,renderer);
}

void App::run(SDL_Window* window, SDL_Renderer* renderer) {     
  double SEC_PER_UPDATE = 1. / this->getFPS(); // 1 sec / fps
  
  double previous = timer.getTicks() / 1000.; // in seconds
  double current;
  double elapsed;
  double lag = 0.0;
  
  while (!closeWindow_) {
    current = timer.getTicks() / 1000.; // in seconds
    elapsed = current - previous;
    previous = current;
    lag += elapsed;
    
    this->handleInput();
    
    while (lag >= SEC_PER_UPDATE) {
      this->update();
      lag -= SEC_PER_UPDATE;
    }
    
    this->render(lag / SEC_PER_UPDATE, window, renderer);
  }
}