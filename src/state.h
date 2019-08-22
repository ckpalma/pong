#ifndef STATE_H
#define STATE_H

///////////////////////////////////////////////////////////////////////////////

#include "input_bindings.h"
#include "input.h"
#include "object.h"
#include <map>
#include <vector>

///////////////////////////// Forward Declarations ////////////////////////////

class App;
class Game;
class World;
class Command;

//////////////////////////////// Implementation ///////////////////////////////

///////////////////////////////// InputHandler ////////////////////////////////

class InputHandler {
  public:
    InputHandler();
    virtual ~InputHandler() = 0;
    std::vector<enum Input> retrieveInput();
    Command* getCommand(enum Input input);
    void setCommandMap(std::map<enum Input,Command*> command_map);
    void setNextHandler(InputHandler* nextInLine);
    void resetBindings();
    
  protected:
    std::map<enum Input,Command*> command_map_;
    InputHandler* next_;
  
  private:
    //  TODO: Call Bindings from to-be-implemented Service Locator for Settings
    bool binded_;
    InputBindings bindings_;
    Command* getCommandFromNext(enum Input input);
};

////////////////////////////////// AppState //////////////////////////////////

class AppState : public InputHandler {
  public:
    virtual ~AppState() {}
    virtual void handleInput(App& app) = 0;
    virtual void update(App& app) = 0;
};

class PlayState : public AppState {
  public:
    PlayState(App& app);
    virtual void handleInput(App& app);
    virtual void update(App& app);
};

////////////////////////////////// GameState //////////////////////////////////

class GameState: public InputHandler {
  public:
    virtual ~GameState() {}
    virtual void update(Game& game) = 0;
};

class PlayingState : public GameState {
  public:
    PlayingState(Game& game);
    virtual void update(Game& game);
};

class CountdownState : public GameState {
  public:
    CountdownState(Game& game);
    virtual void update(Game& game);
};

class PauseState : public GameState {
  public:
    PauseState(Game& game);
    virtual void update(Game& game);
};

////////////////////////////////// WorldState /////////////////////////////////

class WorldState : public InputHandler {
  public:
    virtual ~WorldState() {}
    virtual void update(World& world) = 0;
};

class PongState : public WorldState {
  public:
    PongState(World& world);
    virtual void update(World& world);
};

///////////////////////////////// PaddleState /////////////////////////////////

class PaddleState : public InputHandler {
  public:
    virtual ~PaddleState() {}
    virtual void update(Paddle& paddle) = 0;
};

class PaddleIdleState : public PaddleState {
  public:
    PaddleIdleState(Paddle& paddle);
    virtual void update(Paddle& paddle);
};

class PaddleDefaultState : public PaddleState {
  public:
    PaddleDefaultState(Paddle& paddle);
    virtual void update(Paddle& paddle);
};

class PaddleUpState : public PaddleState {
  public:
    PaddleUpState(Paddle& paddle);
    virtual void update(Paddle& paddle);
};

class PaddleDownState : public PaddleState {
  public:
    PaddleDownState(Paddle& paddle);
    virtual void update(Paddle& paddle);
};

///////////////////////////////////////////////////////////////////////////////

#endif