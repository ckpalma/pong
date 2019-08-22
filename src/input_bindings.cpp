#include "input_bindings.h"

#include <../include/SDL2/SDL.h>
#include "input.h"
#include <map>
#include <vector>

InputBindings::InputBindings() {}

InputBindings::~InputBindings() {}

std::vector<enum Input> InputBindings::retrieveInput() {
  std::vector<enum Input> frameInput;
  // Poll event
  while(SDL_PollEvent(&event_)) {
    switch (event_.type) {
      case SDL_QUIT:
        frameInput.push_back(Input::CLOSE_WINDOW);
      break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:          
        frameInput.push_back(this->getBinding(event_.key.keysym.scancode));
      break;
    }
  }
  
  return frameInput;
}

void InputBindings::bind(Input input, SDL_Scancode scancode) {
  if (!(this->isBinded(scancode))) {
    input_bindings_.insert(std::pair<Input,SDL_Scancode>(input,scancode));
    scancode_bindings_.insert(std::pair<SDL_Scancode,Input>(scancode,input));
  }
}

void InputBindings::bind(std::map<Input,SDL_Scancode> input_bindings) {
  for (auto it = input_bindings.begin(); it != input_bindings.end(); it++) {
    this->bind(it->first, it->second);
  }
}

Input InputBindings::getBinding(SDL_Scancode scancode) {
  try {
    return scancode_bindings_.at(scancode);
  } catch(const std::out_of_range &e) {
    // cerr << "Exception at " << e.what() << endl;
    return Input::UNSPECIFIED;
  }
}

SDL_Scancode InputBindings::getBinding(Input input) {
  try {
    return input_bindings_.at(input);
  } catch(const std::out_of_range &e) {
    // cerr << "Exception at " << e.what() << endl;
    return SDL_SCANCODE_UNKNOWN;
  }
}

bool InputBindings::isBinded(Input input) {
  return  !(this->getBinding(input) == SDL_SCANCODE_UNKNOWN);
}

bool InputBindings::isBinded(SDL_Scancode scancode) {
  return  !(this->getBinding(scancode) == Input::UNSPECIFIED);
}

void InputBindings::resetBindings() {
  this->clearBindings();
  
  std::map<enum Input,SDL_Scancode> input_bindings = {
    {Input::P1_LEFT_JOY_XDIR,   SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_LEFT_JOY_YDIR,   SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    
    {Input::P1_RIGHT_JOY_XDIR,  SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_RIGHT_JOY_YDIR,  SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    
    {Input::P1_DPAD_UP,         SDL_Scancode::SDL_SCANCODE_W},
    {Input::P1_DPAD_DOWN,       SDL_Scancode::SDL_SCANCODE_S},
    {Input::P1_DPAD_LEFT,       SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_DPAD_RIGHT,      SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P1_BUTTON_A,        SDL_Scancode::SDL_SCANCODE_R},
    {Input::P1_BUTTON_B,        SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_BUTTON_X,        SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_BUTTON_Y,        SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P1_BUTTON_START,    SDL_Scancode::SDL_SCANCODE_H},
    {Input::P1_BUTTON_BACK,     SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_BUTTON_GUIDE,    SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P1_LEFT_BUMPER,     SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_LEFT_TRIGGER,    SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P1_RIGHT_BUMPER,    SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P1_RIGHT_TRIGGER,   SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    
    {Input::P2_LEFT_JOY_XDIR,   SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_LEFT_JOY_YDIR,   SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    
    {Input::P2_RIGHT_JOY_XDIR,  SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_RIGHT_JOY_YDIR,  SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    
    {Input::P2_DPAD_UP,         SDL_Scancode::SDL_SCANCODE_UP},
    {Input::P2_DPAD_DOWN,       SDL_Scancode::SDL_SCANCODE_DOWN},
    {Input::P2_DPAD_LEFT,       SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_DPAD_RIGHT,      SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P2_BUTTON_A,        SDL_Scancode::SDL_SCANCODE_RETURN},
    {Input::P2_BUTTON_B,        SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_BUTTON_X,        SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_BUTTON_Y,        SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P2_BUTTON_START,    SDL_Scancode::SDL_SCANCODE_P},
    {Input::P2_BUTTON_BACK,     SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_BUTTON_GUIDE,    SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P2_LEFT_BUMPER,     SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_LEFT_TRIGGER,    SDL_Scancode::SDL_SCANCODE_UNKNOWN},
     
    {Input::P2_RIGHT_BUMPER,    SDL_Scancode::SDL_SCANCODE_UNKNOWN},
    {Input::P2_RIGHT_TRIGGER,   SDL_Scancode::SDL_SCANCODE_UNKNOWN}
  };
  
  this->bind(input_bindings);
}

void InputBindings::clearBindings() {
  input_bindings_.clear();
  scancode_bindings_.clear();
}