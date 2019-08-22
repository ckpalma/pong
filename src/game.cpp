#include "game.h"

#include "state.h"
#include "world.h"
#include <../include/SDL2/SDL.h>
#include <iostream>

Game::Game() {}

Game::~Game() {
  delete state_;
}

GameState* Game::getState() {
  if (state_ == NULL) {
    state_ = new PlayingState(*this);
  }
  return state_;
}

WorldState* Game::getWorldState() {
  WorldState* wState = world_.getState();
  if (wState == NULL) {
    world_.setState(NULL);
  }
  return world_.getState();
}

void Game::setState(GameState* state) {
  if (state_ == NULL) {
    state_ = new PlayingState(*this);
  }
  if (state != NULL) {
    delete state_;
    state_ = state;
  }
}

void Game::update() {
  if (state_ == NULL) {
    state_ = new PlayingState(*this);
  }
  state_->update(*this);
}

void Game::render(double lag, SDL_Window* window, SDL_Renderer* renderer) {
  world_.render(lag,window,renderer);
}