#include "state.h"

#include "input_bindings.h"
#include "object.h"
#include "command.h"
#include "object.h"
#include "input.h"
#include "game.h"
#include "app.h"
#include "debug.h"
#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////// InputHandler ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

InputHandler::InputHandler() {}

InputHandler::~InputHandler() {
  for (auto it = this->command_map_.begin();
    it != this->command_map_.end();
    it++) {
    delete it->second;
  }
}

std::vector<enum Input> InputHandler::retrieveInput() {
  return bindings_.retrieveInput();
}

Command* InputHandler::getCommand(enum Input input) {
  try {
    Command* command = this->command_map_.at(input);
    if (command) {
      return command;
    } else {
      return getCommandFromNext(input);
    }
  } catch(const std::out_of_range &e) {
    // cerr << "Exception at " << e.what() << endl;
    return getCommandFromNext(input);
  }
}

void InputHandler::setCommandMap(std::map<enum Input,Command*> command_map) {
  std::map<enum Input,Command*> comm_map = {
    {Input::P1_LEFT_JOY_XDIR,   NULL},
    {Input::P1_LEFT_JOY_YDIR,   NULL},
    
    {Input::P1_RIGHT_JOY_XDIR,  NULL},
    {Input::P1_RIGHT_JOY_YDIR,  NULL},
    
    {Input::P1_DPAD_UP,         NULL},
    {Input::P1_DPAD_DOWN,       NULL},
    {Input::P1_DPAD_LEFT,       NULL},
    {Input::P1_DPAD_RIGHT,      NULL},
     
    {Input::P1_BUTTON_A,        NULL},
    {Input::P1_BUTTON_B,        NULL},
    {Input::P1_BUTTON_X,        NULL},
    {Input::P1_BUTTON_Y,        NULL},
     
    {Input::P1_BUTTON_START,    NULL},
    {Input::P1_BUTTON_BACK,     NULL},
    {Input::P1_BUTTON_GUIDE,    NULL},
     
    {Input::P1_LEFT_BUMPER,     NULL},
    {Input::P1_LEFT_TRIGGER,    NULL},
     
    {Input::P1_RIGHT_BUMPER,    NULL},
    {Input::P1_RIGHT_TRIGGER,   NULL},
    
    {Input::P2_LEFT_JOY_XDIR,   NULL},
    {Input::P2_LEFT_JOY_YDIR,   NULL},
    
    {Input::P2_RIGHT_JOY_XDIR,  NULL},
    {Input::P2_RIGHT_JOY_YDIR,  NULL},
    
    {Input::P2_DPAD_UP,         NULL},
    {Input::P2_DPAD_DOWN,       NULL},
    {Input::P2_DPAD_LEFT,       NULL},
    {Input::P2_DPAD_RIGHT,      NULL},
     
    {Input::P2_BUTTON_A,        NULL},
    {Input::P2_BUTTON_B,        NULL},
    {Input::P2_BUTTON_X,        NULL},
    {Input::P2_BUTTON_Y,        NULL},
     
    {Input::P2_BUTTON_START,    NULL},
    {Input::P2_BUTTON_BACK,     NULL},
    {Input::P2_BUTTON_GUIDE,    NULL},
     
    {Input::P2_LEFT_BUMPER,     NULL},
    {Input::P2_LEFT_TRIGGER,    NULL},
     
    {Input::P2_RIGHT_BUMPER,    NULL},
    {Input::P2_RIGHT_TRIGGER,   NULL},
    {Input::CLOSE_WINDOW,       NULL}
  };
  
  for (auto it = command_map.begin(); it != command_map.end(); it++) {
    comm_map[it->first] = it->second;
  }
  this->command_map_ = comm_map;
}

void InputHandler::setNextHandler(InputHandler* nextInLine) {
  this->next_ = nextInLine;
}

void InputHandler::resetBindings() {
  bindings_.resetBindings();
}

Command* InputHandler::getCommandFromNext(enum Input input) {
  if (this->next_ != NULL) {
    return this->next_->getCommand(input);
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// AppState //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// PlayState /////////////////////////////////

PlayState::PlayState(App& app) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(app.getGameState());
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    {Input::CLOSE_WINDOW,       new CloseWindowCommand(app)}
  };
  
  InputHandler::setCommandMap(command_map);
}
    
void PlayState::handleInput(App& app) {
  InputHandler::resetBindings();
  std::vector<enum Input> inputs = InputHandler::retrieveInput();
  for (auto input = inputs.begin(); input != inputs.end(); input++) {
    Command* command = InputHandler::getCommand(*input);
    if (command) {
      command->execute();
    }
  }
}
    
void PlayState::update(App& app) {
  
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////// PongState //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////// PlayingState ////////////////////////////////

PlayingState::PlayingState(Game& game) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(game.getWorldState());
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    
  };
  
  InputHandler::setCommandMap(command_map);
}
    
void PlayingState::update(Game& game) {
      
}

//////////////////////////////// CountdownState ///////////////////////////////

CountdownState::CountdownState(Game& game) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(game.getWorldState());
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    
  };
  
  InputHandler::setCommandMap(command_map);
}
    
void CountdownState::update(Game& game) {
      
}

////////////////////////////////// PauseState /////////////////////////////////

PauseState::PauseState(Game& game) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(NULL);
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    
  };
  
  InputHandler::setCommandMap(command_map);
}
    
void PauseState::update(Game& game) {
      
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////// WorldState /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////// PongState //////////////////////////////////

PongState::PongState(World& world) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(NULL);//world.getP1Paddle().getState());
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
  
  };
  
  InputHandler::setCommandMap(command_map);
}

void PongState::update(World& world) {
  
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////// PaddleState /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

PaddleUpCommand paddleUp = PaddleUpCommand();
PaddleDownCommand paddleDown = PaddleDownCommand();

/////////////////////////////// PaddleIdleState ///////////////////////////////

PaddleIdleState::PaddleIdleState(Paddle& paddle) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(NULL);
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    {Input::P1_DPAD_UP,         &paddleUp},
    {Input::P1_DPAD_DOWN,       &paddleDown},
    {Input::P2_DPAD_UP,         &paddleUp},
    {Input::P2_DPAD_DOWN,       &paddleDown}
  };
  
  InputHandler::setCommandMap(command_map);
}

void PaddleIdleState::update(Paddle& paddle) {
  
}

////////////////////////////// PaddleDefaultState /////////////////////////////

PaddleDefaultState::PaddleDefaultState(Paddle& paddle) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(NULL);
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    {Input::P1_DPAD_UP,         &paddleUp},
    {Input::P1_DPAD_DOWN,       &paddleDown},
    {Input::P2_DPAD_UP,         &paddleUp},
    {Input::P2_DPAD_DOWN,       &paddleDown}
  };
  
  InputHandler::setCommandMap(command_map);
}

void PaddleDefaultState::update(Paddle& paddle) {
  
}

//////////////////////////////// PaddleUpState ////////////////////////////////

PaddleUpState::PaddleUpState(Paddle& paddle) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(NULL);
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    {Input::P1_DPAD_UP,         &paddleUp},
    {Input::P1_DPAD_DOWN,       &paddleDown},
    {Input::P2_DPAD_UP,         &paddleUp},
    {Input::P2_DPAD_DOWN,       &paddleDown}
  };
  
  InputHandler::setCommandMap(command_map);
}

void PaddleUpState::update(Paddle& paddle) {
  
}

/////////////////////////////// PaddleDownState ///////////////////////////////

PaddleDownState::PaddleDownState(Paddle& paddle) {
  //  TODO: Object Pool of States in order to setNextHandler
  InputHandler::setNextHandler(NULL);
  
  //  TODO: Load this data from a json file, for decluttering purposes
  std::map<enum Input,Command*> command_map = {
    {Input::P1_DPAD_UP,         &paddleUp},
    {Input::P1_DPAD_DOWN,       &paddleDown},
    {Input::P2_DPAD_UP,         &paddleUp},
    {Input::P2_DPAD_DOWN,       &paddleDown}
  };
  
  InputHandler::setCommandMap(command_map);
}

void PaddleDownState::update(Paddle& paddle) {
  
}