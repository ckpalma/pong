#include "object.h"

#include "constants.h"
#include "component.h"
#include "state.h"

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
  double crossproduct = (r.y - p.y) * (q.x - p.x) - (r.x - p.x) * (q.y - p.y);

  double epsilon = 1e-6;
  // compare versus epsilon for float values, or != 0 if using integers
  if (std::abs(crossproduct) > epsilon)
      return false;

  double dotproduct = (r.x - p.x) * (q.x - p.x) + (r.y - p.y)*(q.y - p.y);
  if (dotproduct < 0)
      return false;

  double squaredlengthba = (q.x - p.x)*(q.x - p.x) + (q.y - p.y)*(q.y - p.y);
  if (dotproduct > squaredlengthba)
      return false;

  return true;
}


// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
  // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
  // for details of below formula.
  int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);

  if (val == 0) return 0;  // colinear

  return (val > 0)? 1: 2; // clock or counterclock wise
}
 

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
  // Find the four orientations needed for general and
  // special cases
  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);

  // General case
  if (o1 != o2 && o3 != o4)
      return true;

  // Special Cases
  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  if (o1 == 0 && onSegment(p1, p2, q1)) return true;

  // p1, q1 and q2 are colinear and q2 lies on segment p1q1
  if (o2 == 0 && onSegment(p1, q2, q1)) return true;

  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  if (o3 == 0 && onSegment(p2, p1, q2)) return true;

   // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  if (o4 == 0 && onSegment(p2, q1, q2)) return true;

  return false; // Doesn't fall in any of the above cases
}

// Returns point of intersection, if exists, between segments 'p1q1' and 'p2q2'
Point intersection(Point p1, Point q1, Point p2, Point q2) {
  Point pt = { 0, 0 };
  
  double x1 = p1.x; double x2 = q1.x; double x3 = p2.x; double x4 = q2.x;
  double y1 = p1.y; double y2 = q1.y; double y3 = p2.y; double y4 = q2.y;
  
  double det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  // If d is zero, there is no intersection
  if (det == 0) return pt;
   
  // Get the x and y
  double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
  double x = ( pre * (x3 - x4) - (x1 - x2) * post ) / det;
  double y = ( pre * (y3 - y4) - (y1 - y2) * post ) / det;
   
  // Check if the x and y coordinates are within both lines
  if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
  x < std::min(x3, x4) || x > std::max(x3, x4) ) return pt;
  if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
  y < std::min(y3, y4) || y > std::max(y3, y4) ) return pt;
   
  // Return the point of intersection
  pt.x = x;
  pt.y = y;
  return pt;
} 

// Returns val if within min_v and max_v. Otherwise returns either min_v or max_v, depending on which is closer to val.
double bound(double val, double min_v, double max_v) {
  return std::max(min_v, std::min(val, max_v));
}

// Custom modulo function which works as expected from discrete math.
inline double modulo(double a, double b) {
  if (a < 0) {
    return b - std::fmod(a,b);
  }
  return std::fmod(a,b);
}

Paddle::Paddle(Player pl)
: pl_(pl) {
  double offset = SCREEN_WIDTH / 32;
  if (pl == Player::P_ONE) {
    this->setX(offset);
  } else {
    this->setX(SCREEN_WIDTH - offset - SCREEN_WIDTH / 64);
  }
  this->setY(SCREEN_HEIGHT / 2 - 0.5 * this->h());
  
  GameObject::setSpeed(SCREEN_HEIGHT / 80.);
  this->setW(SCREEN_WIDTH / 64);
  this->setH(SCREEN_HEIGHT / 8);
}

const Player Paddle::pl() { return pl_; }

std::vector<Point> Paddle::insideCorners() {
  Point c1, c2;
  if (this->pl_ == Player::P_ONE) {
    c1 = { this->x() + this->w(), this->y() };
    c2 = { this->x() + this->w(), this->y() + this->h() };
  } else {
    c1 = { this->x(), this->y() };
    c2 = { this->x(), this->y() + this->h() };
  }
  std::vector<Point> c = { c1, c2 };
  return c;
}

void Paddle::update() {
}

PaddleState* Paddle::getState() {
  if (state_ == NULL) {
    state_ = new PaddleIdleState(*this);
  }
  return state_;
}

void Paddle::setState(PaddleState* state) {
  if (state_ == NULL) {
    state_ = new PaddleIdleState(*this);
  }
  if (state != NULL) {
    delete state_;
    state_ = state;
  }
}

Ball::Ball()
: ang_((rand()%4) * 90 + 22.5 + (rand()%45)),
l_(SCREEN_HEIGHT / 80.),
hitPaddle_(false),
hitWall_(false)
{
  this->setXY(
    SCREEN_WIDTH / 2 - 0.5 * this->l_,
    SCREEN_HEIGHT / 2 - 0.5 * this->l_
  );
  
  GameObject::setSpeed(SCREEN_HEIGHT / 80.);
}

const double Ball::l() { return this->l_; }
const double Ball::ang() { return this->ang_; }

double Ball::moveX(double x_val) {
  return x_val + cos(this->ang_*M_PI/180) * GameObject::speed();
}

double Ball::moveY(double y_val) {
  return y_val + sin(this->ang_*M_PI/180) * GameObject::speed();
}

void Ball::newGame() {
  this->ang_ = (rand()%4) * 90 + 22.5 + (rand()%45);
  GameObject::setSpeed(SCREEN_HEIGHT / 80.);
  this->hitPaddle_ = false;
  this->hitWall_ = false;
  this->setXY(
    SCREEN_WIDTH / 2 - 0.5 * this->l_,
    SCREEN_HEIGHT / 2 - 0.5 * this->l_
  );
}

void Ball::update() {
  
}

void Ball::update(Paddle& p1, Paddle& p2) {
  std::vector<Point> p1_c = p1.insideCorners();
  std::vector<Point> p2_c = p2.insideCorners();
  
  std::vector<Point> old_pts = corners();
  std::vector<Point> new_pts = translatedCorners();
  
  double x2 = moveX(this->x());
  double y2 = moveY(this->y());
  
  // Detect collisions on paddles
  if (!hitPaddle_) {
    bool p1_b1 = doIntersect(p1_c[0], p1_c[1], old_pts[0], new_pts[0]);
    bool p1_b3 = doIntersect(p1_c[0], p1_c[1], old_pts[2], new_pts[2]);
    bool p2_b2 = doIntersect(p2_c[0], p2_c[1], old_pts[1], new_pts[1]);
    bool p2_b4 = doIntersect(p2_c[0], p2_c[1], old_pts[3], new_pts[3]);
    
    // Detect collisions on left paddle
    if (p1_b1 || p1_b3) {
      x2 = p1_c[0].x + (p1_c[0].x - x2);
      GameObject::setSpeed(GameObject::speed()*1.05);
      this->ang_ = modulo(270. + (270. - this->ang_), 360.);
      
      // Adjust angle for corner shots for left paddle
      Point b1_c = intersection(p1_c[0],p1_c[1],old_pts[0],new_pts[0]);
      Point b3_c = intersection(p1_c[0],p1_c[1],old_pts[2],new_pts[2]);
      if (b1_c.y >= 0.65 * p1.h() + p1.y()) {
        double ratio = (b1_c.y - 0.65 * p1.h() - p1.y()) / (p1.h() / 2) + 0.25;
        if (this->ang_ < 90) {
          this->ang_ = modulo(this->ang_ + ratio * (90-this->ang_), 360);
        } else {
          this->ang_ = modulo(this->ang_ + ratio * (this->ang_-270), 360);
        }
      }
      if (b3_c.y <= 0.35 * p1.h() + p1.y() && b3_c.y > 0) {
        double ratio = (0.35 * p1.h() + p1.y() - b3_c.y) / (p1.h() / 2) + 0.25;
        if (this->ang_ < 90) {
          this->ang_ = modulo(this->ang_ - ratio * (90-this->ang_), 360);
        } else {
          this->ang_ = modulo(this->ang_ - ratio * (this->ang_-270), 360);
        }
      }
      
      if (this->ang_ < 90) {
        this->ang_ = std::min(this->ang_, 75.);
      } else {
        this->ang_ = std::max(this->ang_, 285.);
      }
      
      hitPaddle_ = true;
    }
    
    // Detect collisions on right paddle
    else if (p2_b2 || p2_b4) {
      x2 = p2_c[0].x - (new_pts[1].x - p2_c[0].x) - this->l_;
      GameObject::setSpeed(GameObject::speed()*1.05);
      if (this->ang_ >= 270) {
        this->ang_ = modulo(180. + 360. - this->ang_, 360);
      } else {
        this->ang_ = modulo(180. - this->ang_, 360);
      }
      
      // Adjust angle for corner shots for right paddle
      Point b2_c = intersection(p2_c[0],p2_c[1],old_pts[1],new_pts[1]);
      Point b4_c = intersection(p2_c[0],p2_c[1],old_pts[3],new_pts[3]);
      if (b2_c.y >= 0.65 * p2.h() + p2.y()) {
        double ratio = (b2_c.y - 0.65 * p2.h() - p2.y()) / (p2.h() / 2) + 0.25;
        if (this->ang_ <= 180) {
          this->ang_ = modulo(this->ang_ - ratio * (this->ang_-90), 360);
        } else {
          this->ang_ = modulo(this->ang_ - ratio * (270-this->ang_), 360);
        }
      }
      if (b4_c.y <= 0.35 * p2.h() + p2.y() && b4_c.y > 0) {
        double ratio = (0.35 * p2.h() + p2.y() - b4_c.y) / (p2.h() / 2) + 0.25;
        if (this->ang_ <= 180) {
          this->ang_ = modulo(this->ang_ + ratio * (this->ang_-90), 360);
        } else {
          this->ang_ = modulo(this->ang_ + ratio * (270-this->ang_), 360);
        }
      }
      
      if (this->ang_ <= 180) {
        this->ang_ = std::max(this->ang_, 105.);
      } else {
        this->ang_ = std::min(this->ang_, 255.);
      }
      
      hitPaddle_ = true;
    }
  } else {
    hitPaddle_ = false;
  }
  
  // Detect collisions on upper wall
  if (!hitWall_) {
    if (y2 < 0) {
      y2 = (-1) * y2;
      this->ang_ = modulo(180 + (180 - this->ang_), 360);
      hitWall_ = true;
    }
    // Detect collisions on lower wall
    else if (y2 > SCREEN_HEIGHT - this->l_) {
      y2 = SCREEN_HEIGHT - this->l_ - (y2 - (SCREEN_HEIGHT - this->l_));
      this->ang_ = modulo(360 - this->ang_, 360);
      hitWall_ = true;
    }
  } else {
    hitWall_ = false;
  }
  
  this->setXY( x2, y2 );
}

std::vector<Point> Ball::corners() {
  Point c1, c2, c3, c4;
  c1.x = this->x();
  c1.y = this->y();
  c2.x = this->x() + this->l_;
  c2.y = this->y();
  c3.x = this->x();
  c3.y = this->y() + this->l_;
  c4.x = this->x() + this->l_;
  c4.y = this->y() + this->l_;
  std::vector<Point> c = {
    c1, c2,
    c3, c4
  };
  return c;
}

std::vector<Point> Ball::translatedCorners() {
  Point c1, c2, c3, c4;
  c1.x = moveX(this->x());
  c1.y = moveY(this->y());
  c2.x = moveX(this->x() + this->l_);
  c2.y = moveY(this->y());
  c3.x = moveX(this->x());
  c3.y = moveY(this->y() + this->l_);
  c4.x = moveX(this->x() + this->l_);
  c4.y = moveY(this->y() + this->l_);
  std::vector<Point> c = {
    c1, c2,
    c3, c4
  };
  return c;
}

// GameObject::GameObject() {}
           // InputComponent* input,
           // PhysicsComponent* physics,
           // GraphicsComponent* graphics)
// : input_(input),
//   physics_(physics),
//   graphics_(graphics)
// {}

// void GameObject::update()//World& world, Graphics& graphics)
// {
//   // input_->update(*this);
//   // physics_->update(*this);// , world);
//   // graphics_->update(*this);// , graphics);
// }