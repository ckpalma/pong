#include "command.h"

#include "constants.h"
#include "object.h"
#include "state.h"
#include "game.h"
#include <cmath>

CloseWindowCommand::CloseWindowCommand(App& app)
: app_(app)
{}

void CloseWindowCommand::execute() {
  app_.closeWindow();
}

void CloseWindowCommand::execute(GameObject& gObj) {
  
}

SetAppStateCommand::SetAppStateCommand(App& app, AppState* newAppState)
: app_(app),
  newAppState_(newAppState)
{}

void SetAppStateCommand::execute() {
  app_.setState(newAppState_);
}

void SetAppStateCommand::execute(GameObject& gObj) {
  
}

SetGameStateCommand::SetGameStateCommand(Game& game, GameState* newGameState)
: game_(game),
  newGameState_(newGameState)
{}

void SetGameStateCommand::execute() {
  game_.setState(newGameState_);
}

void SetGameStateCommand::execute(GameObject& gObj) {
  
}

SetWorldStateCommand::SetWorldStateCommand(World& world, WorldState* newWorldState)
: world_(world),
  newWorldState_(newWorldState)
{}

void SetWorldStateCommand::execute() {
  world_.setState(newWorldState_);
}

void SetWorldStateCommand::execute(GameObject& gObj) {
  
}

SetPaddleStateCommand::SetPaddleStateCommand(Paddle& paddle, PaddleState* newPaddleState)
: paddle_(paddle),
  newPaddleState_(newPaddleState)
{}

void SetPaddleStateCommand::execute() {
  paddle_.setState(newPaddleState_);
}

void SetPaddleStateCommand::execute(GameObject& gObj) {
  
}

void PaddleUpCommand::execute() {
  
}

void PaddleUpCommand::execute(GameObject& gObj) {
  gObj.setY(std::max(0., std::min(gObj.y()-gObj.speed(), SCREEN_HEIGHT-gObj.h())));
}

void PaddleDownCommand::execute() {
  
}

void PaddleDownCommand::execute(GameObject& gObj) {
  gObj.setY(std::max(0., std::min(gObj.y()+gObj.speed(), SCREEN_HEIGHT-gObj.h())));
}