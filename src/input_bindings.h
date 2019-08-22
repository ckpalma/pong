#ifndef INPUT_BINDINGS_H
#define INPUT_BINDINGS_H

///////////////////////////////////////////////////////////////////////////////

#include <../include/SDL2/SDL.h>
#include "input.h"
#include <map>
#include <vector>

/*  Maps the Input enum to event bindings (**using SDL2). 
**  **: Ideally, this should be interchangeable with other input systems. */
class InputBindings {
  public:
    InputBindings();
    ~InputBindings();
    
    /*  Returns an Input depending on the event from the SDL_Event queue. */
    std::vector<enum Input> retrieveInput();
    
    /*  Binds an input and scancode to one another if one does not exist */
    void bind(Input input, SDL_Scancode scancode);
    
    /*  Bind multiple inputs to scancodes after clearing the binding map */
    void bind(std::map<Input,SDL_Scancode> input_bindings);
    
    /*  Returns the Input that scancode is binded to */
    Input getBinding(SDL_Scancode scancode);
    
    /*  Returns the SDL_Scancode that input is binded to */
    SDL_Scancode getBinding(Input input);
    
    /*  Returns true if input is binded to scancode in input_bindings_ */
    bool isBinded(Input input);
    
    /*  Returns true if scancode is binded to input in scancode_bindings_ */
    bool isBinded(SDL_Scancode scancode);
    
    /*  Initializes input_bindings_ to the default bindings */
    void resetBindings();
    
    /*  Clear both binding maps */
    void clearBindings();
    
  private:
    SDL_Event event_;
    SDL_Scancode repeatKey_;
    bool keyDown_;
    
    std::map<Input,SDL_Scancode> input_bindings_;
    std::map<SDL_Scancode,Input> scancode_bindings_;
    
    /*  Update scancode_bindings_ when modifying input_bindings_ */
    void updateScancodeBindings();
};

///////////////////////////////////////////////////////////////////////////////

#endif