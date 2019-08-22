#ifndef COMMAND_H
#define COMMAND_H

///////////////////////////////////////////////////////////////////////////////

#include "object.h"
#include "game.h"
#include "app.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Command Interfaces //////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*  Interface for commands that execute once and independently. */
class Command {
  public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void execute(GameObject& gObj) = 0;
};

/*  Interface for commands that happen in a sequence in time, containing info
    about the command, which can later be used to undo/redo its execution. */
class SequenceCommand : public Command {
  public:
    virtual ~SequenceCommand() {}
    virtual void execute() = 0;
    virtual void execute(GameObject& gObj) = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

///////////////////////////////////////////////////////////////////////////////
////////////////////////// Game Specific Subclasses ///////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////// Forward Declarations ////////////////////////////

class AppState;
class GameState;
class WorldState;

//////////////////////////////// Implementation ///////////////////////////////

/*  Closes the window. TODO: Make this into an AppState instead. */
class CloseWindowCommand : public Command {
  public:
    CloseWindowCommand(App& app);
    virtual void execute();
    virtual void execute(GameObject& gObj);
  private:
    App& app_;
};

/* Transitions the app's AppState to newAppState. */
class SetAppStateCommand : public Command {
  public:
    SetAppStateCommand(App& App, AppState* newAppState);
    virtual void execute();
    virtual void execute(GameObject& gObj);
  private:
    App& app_;
    AppState* newAppState_;
};

/* Transitions the game's GameState to newGameState. */
class SetGameStateCommand : public Command {
  public:
    SetGameStateCommand(Game& game, GameState* newGameState);
    virtual void execute();
    virtual void execute(GameObject& gObj);
  private:
    Game& game_;
    GameState* newGameState_;
};

/* Transitions the world's WorldState to newWorldState. */
class SetWorldStateCommand : public Command {
  public:
    SetWorldStateCommand(World& world, WorldState* newWorldState);
    virtual void execute();
    virtual void execute(GameObject& gObj);
  private:
    World& world_;
    WorldState* newWorldState_;
};

/* Transitions the paddle's PaddleState to newPaddleState. */
class SetPaddleStateCommand : public Command {
  public:
    SetPaddleStateCommand(Paddle& paddle, PaddleState* newPaddleState);
    virtual void execute();
    virtual void execute(GameObject& gObj);
  private:
    Paddle& paddle_;
    PaddleState* newPaddleState_;
};

/*  Moves paddle_ upwards. */
class PaddleUpCommand : public Command {
  public:
    virtual void execute();
    virtual void execute(GameObject& gObj);
};

/*  Moves paddle_ downwards. */
class PaddleDownCommand : public Command {
  public:
    virtual void execute();
    virtual void execute(GameObject& gObj);
};


///////////////////////////////////////////////////////////////////////////////

#endif