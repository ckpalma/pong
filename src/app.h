#ifndef APP_H
#define APP_H

///////////////////////////////////////////////////////////////////////////////

#include "state.h"
#include "tools.h"
#include "game.h"
#include <../include/SDL2/SDL.h>

///////////////////////////// Forward Declarations ////////////////////////////

class AppState;
class GameState;

//////////////////////////////// Implementation ///////////////////////////////

class App {
  public:
    App();
    ~App();
    
    double getFPS();
    
    GameState* getGameState();
    
    void setState(AppState* state);
    
    void closeWindow();
    
    void handleInput();
    
    void update();
    
    void render(double lag, SDL_Window* window, SDL_Renderer* renderer); // lag in seconds
    
    void run(SDL_Window* window, SDL_Renderer* renderer);
    
  private:
    LTimer timer;
    AppState* state_;
    
    //  TODO: Make this more general for any game of some InGame type class
    Game game_;
    
    double fps_;
    bool closeWindow_;
};

///////////////////////////////////////////////////////////////////////////////

#endif