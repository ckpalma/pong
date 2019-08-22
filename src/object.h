#ifndef OBJECT_H
#define OBJECT_H

///////////////////////////////////////////////////////////////////////////////

#include <../include/SDL2/SDL.h>
#include <cmath>
#include "component.h"
#include <vector>

///////////////////////////// Forward Declarations ////////////////////////////

class PaddleState;

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// GameObject Interfaces ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Defines a point on the window
struct Point { double x; double y; };

// Player Number
typedef enum class Player {P_ONE, P_TWO} Player;

// Superclass of entities within the game. Gives basic info about its position, update method used for frame updates, and setters for attributes.
class GameObject {
  public:
    GameObject() {}

    virtual ~GameObject() {}
    virtual void update() = 0;

    double x() const { return pt_.x; }
    double y() const { return pt_.y; }
    double w() const { return w_; }
    double h() const { return h_; }
    double speed() const { return speed_; }
    Point& pt() { return pt_; }

    void setX(double x) { pt_.x = x; }
    void setY(double y) { pt_.y = y; }
    void setXY(double x, double y) { pt_.x = x; pt_.y = y; }
    void setW(double w) { w_ = w; }
    void setH(double h) { h_ = h; }
    void setSpeed(double sp) { speed_ = sp; }

  private:
    Point pt_;
    double speed_;
    double w_;
    double h_;
};

class Paddle : public GameObject {
  public:
    Paddle(Player pl);
    const Player pl();
    std::vector<Point> insideCorners();
    void update();
    PaddleState* getState();
    void setState(PaddleState* state);
    
  private:
    PaddleState* state_;
    Player const pl_;
};

class Ball : public GameObject {
  public:
    Ball();
    const double l();
    const double ang();
    const double speed();
    double moveX(double x_val);
    double moveY(double y_val);
    void newGame();
    void update();
    void update(Paddle& p1, Paddle& p2);
    std::vector<Point> corners();
    std::vector<Point> translatedCorners();
  
  private:
    double const l_;
    double ang_;
    bool hitPaddle_;
    bool hitWall_;
};

// class Object {
//   public:
//     Object();
//            // InputComponent* input,
//            // PhysicsComponent* physics,
//            // GraphicsComponent* graphics)
//     // : input_(input),
//     //   physics_(physics),
//     //   graphics_(graphics)
//     // {}
    
//     virtual ~Object() {}
    
//     virtual void update() = 0;
    
//   // protected:
//     // InputComponent* input_;
//     // PhysicsComponent* physics_;
//     // GraphicsComponent* graphics_;
// };

// class GameObject : public Object {
//   public:
//     GameObject();
//                // InputComponent* input,
//                // PhysicsComponent* physics,
//                // GraphicsComponent* graphics);
//     virtual ~GameObject() {}

//     virtual void update() = 0;
// };

///////////////////////////////////////////////////////////////////////////////
////////////////////////// Game Specific Subclasses ///////////////////////////
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////

#endif