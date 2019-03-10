#include <../include/SDL2/SDL.h>
#include <../include/SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


// Key press constants
typedef enum class KeyInput {K_DEFAULT, K_UP, K_DOWN, K_W, K_S, K_P, K_QUIT} KeyInput;


// Defines a point on the window
struct Point { double x; double y; };


// Player Number
typedef enum class Player {P_ONE, P_TWO} Player;


// The window we'll be rendering to
SDL_Window* gWindow = NULL;


// The window renderer
SDL_Renderer* gRenderer = NULL;


// Starts up SDL and creates window
bool init();


// Loads media
bool loadMedia();


// Frees media and shuts down SDL
void close();


// Loads individual image as texture
SDL_Texture* loadTexture( std::string path );


// Get key press input
std::vector<KeyInput> processInput(SDL_Event e);


bool init() {
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
      printf( "Warning: Linear texture filtering not enabled!" );
    }

    //Create window
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      //Create renderer for window
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
      if( gRenderer == NULL )
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
      else
      {
        //Initialize renderer color
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
          printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
          success = false;
        }
      }
    }
  }

  return success;
}


bool loadMedia() {
  //Loading success flag
  bool success = true;

  //Nothing to load
  return success;
}


void close() {
  //Destroy window  
  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;

  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}


SDL_Texture* loadTexture( std::string path ) {
  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else
  {
    //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL )
    {
      printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
  }

  return newTexture;
}
 
 
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
  if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
      q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
     return true;

  return false;
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


double bound(double val, double min_v, double max_v) {
  return std::max(min_v, std::min(val, max_v));
}


inline double modulo(double a, double b) {
  if (a < 0) {
    return b - std::fmod(a,b);
  }
  return std::fmod(a,b);
}


class Entity {
  public:
    Entity()
    : pt_{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}
    {}

    virtual ~Entity() {}
    virtual void update() = 0;

    double x() const { return pt_.x; }
    double y() const { return pt_.y; }
    Point& pt() { return pt_; }

    void setX(double x) { pt_.x = x; }
    void setY(double y) { pt_.y = y; }
    void setXY(double x, double y) { pt_.x = x; pt_.y = y; }

  private:
    Point pt_;
};


class Paddle : public Entity {
  public:
    Paddle(Player pl)
    : w_(SCREEN_WIDTH / 64),
    h_(SCREEN_HEIGHT / 8),
    pl_(pl),
    speed_(SCREEN_HEIGHT / 40) {
      double offset = SCREEN_WIDTH / 32;
      if (pl == Player::P_ONE) {
        this->setX(offset);
      } else {
        this->setX(SCREEN_WIDTH - offset - SCREEN_WIDTH / 64);
      }
      this->setY(SCREEN_HEIGHT / 2 - 0.5 * this->h_);
    }
    
    double w() const { return w_; }
    double h() const { return h_; }
    Player pl() { return pl_; }
    
    std::vector<Point> insideCorners() {
      Point c1, c2;
      if (this->pl_ == Player::P_ONE) {
        c1 = { this->x() + this->w_, this->y() };
        c2 = { this->x() + this->w_, this->y() + this->h_ };
      } else {
        c1 = { this->x(), this->y() };
        c2 = { this->x(), this->y() + this->h_ };
      }
      std::vector<Point> c = { c1, c2 };
      return c;
    }
    
    void update(KeyInput& k_in) {
      if (k_in == KeyInput::K_UP && this->pl_ == Player::P_TWO) {
        this->setY(bound(this->y()-this->speed_, 0, SCREEN_HEIGHT-this->h_));
      } else if (k_in == KeyInput::K_DOWN && this->pl_ == Player::P_TWO) {
        this->setY(bound(this->y()+this->speed_, 0, SCREEN_HEIGHT-this->h_));
      } else if (k_in == KeyInput::K_W && this->pl_ == Player::P_ONE) {
        this->setY(bound(this->y()-this->speed_, 0, SCREEN_HEIGHT-this->h_));
      } else if (k_in == KeyInput::K_S && this->pl_ == Player::P_ONE) {
        this->setY(bound(this->y()+this->speed_, 0, SCREEN_HEIGHT-this->h_));
      }
    }
    
    virtual void update() { }
    
  private:
    double const w_;
    double const h_;
    Player const pl_;
    double const speed_;
};


class Ball : public Entity {
  public:
    Ball()
    : ang_((rand()%4) * 90 + 22.5 + (rand()%45)),
    speed_(SCREEN_HEIGHT / 40.),
    l_(SCREEN_HEIGHT / 80.),
    hitPaddle_(false),
    hitWall_(false)
    {
      this->setXY(
        SCREEN_WIDTH / 2 - 0.5 * this->l_,
        SCREEN_HEIGHT / 2 - 0.5 * this->l_
      );
    }
    
    double l() const { return this->l_; }
    double ang() const { return this->ang_; }
    double speed() const { return this->speed_; }
    
    void setSpeed(double sp) { this->speed_ = sp; }
    
    double moveX(double x_val) {
      return x_val + cos(this->ang_*M_PI/180) * this->speed_;
    }
    
    double moveY(double y_val) {
      return y_val + sin(this->ang_*M_PI/180) * this->speed_;
    }
    
    void newGame() {
      this->ang_ = (rand()%4) * 90 + 22.5 + (rand()%45);
      this->speed_ = SCREEN_HEIGHT / 40.;
      this->hitPaddle_ = false;
      this->hitWall_ = false;
      this->setXY(
        SCREEN_WIDTH / 2 - 0.5 * this->l_,
        SCREEN_HEIGHT / 2 - 0.5 * this->l_
      );
    }
    
    void update(Paddle& p1, Paddle& p2) {
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
          this->speed_ *= 1.05;
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
          this->speed_ *= 1.05;
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
        if (y2 <= 0) {
          y2 = (-1) * y2;
          this->ang_ = modulo(180 + (180 - this->ang_), 360);
          hitWall_ = true;
        }
        // Detect collisions on lower wall
        else if (y2 >= SCREEN_HEIGHT - this->l_) {
          y2 = SCREEN_HEIGHT - this->l_ - (y2 - (SCREEN_HEIGHT - this->l_));
          this->ang_ = modulo(360 - this->ang_, 360);
          hitWall_ = true;
        }
      } else {
        hitWall_ = false;
      }
      
      this->setXY( x2, y2 );
    }
    
    virtual void update() { }
    
    std::vector<Point> corners() {
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
    
    std::vector<Point> translatedCorners() {
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
  
  private:
    double const l_;
    double ang_;
    double speed_;
    bool hitPaddle_;
    bool hitWall_;
};


std::vector<KeyInput> processInput(SDL_Event e) {
  KeyInput k_cmd = KeyInput::K_DEFAULT;
  KeyInput k_p1 = KeyInput::K_DEFAULT;
  KeyInput k_p2 = KeyInput::K_DEFAULT;
  std::vector<KeyInput> keys = { k_cmd, k_p1, k_p2 };
  
  const Uint8* kbState = SDL_GetKeyboardState(NULL);
  
  //Handle events on queue
  while( SDL_PollEvent( &e ) != 0 ) {
    //User requests quit
    if( e.type == SDL_QUIT ) {
      keys[0] = KeyInput::K_QUIT;
      return keys;
    }
    // User presses a key
    else if( e.type == SDL_KEYDOWN ) {
      //Select surfaces based on key press
      switch( e.key.keysym.sym ) {
        case SDLK_p:
          keys[0] = KeyInput::K_P;
        break;
      }
    }
  }
  
  if (kbState[SDL_SCANCODE_W]) {
    keys[1] = KeyInput::K_W;
  }
  if (kbState[SDL_SCANCODE_S]) {
    keys[1] = KeyInput::K_S;
  }
  if (kbState[SDL_SCANCODE_UP]) {
    keys[2] = KeyInput::K_UP;
  }
  if (kbState[SDL_SCANCODE_DOWN]) {
    keys[2] = KeyInput::K_DOWN;
  }
  
  // std::cout << keys << "\n";
  
  return keys;
}


void render(Ball& b, Paddle& p1, Paddle& p2) {
  //Clear screen
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( gRenderer );

  //Render red filled quad
  SDL_Rect bkgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
  SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );    
  SDL_RenderFillRect( gRenderer, &bkgRect );

  //Render green outlined quad
  SDL_Rect ballRect = {(int)b.x(), (int)b.y(), (int)b.l(), (int)b.l()};
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );    
  SDL_RenderDrawRect( gRenderer, &ballRect );
  
  //Render green outlined quad
  SDL_Rect p1Rect = {(int)p1.x(), (int)p1.y(), (int)p1.w(), (int)p1.h()};
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );    
  SDL_RenderDrawRect( gRenderer, &p1Rect );
  
  //Render green outlined quad
  SDL_Rect p2Rect = {(int)p2.x(), (int)p2.y(), (int)p2.w(), (int)p2.h()};
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );    
  SDL_RenderDrawRect( gRenderer, &p2Rect );

  //Draw vertical line of yellow dots
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
  for( int i = 0; i < SCREEN_HEIGHT; i += 4 ) {
    SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
  }

  //Update screen
  SDL_RenderPresent( gRenderer );
  
}


int main( int argc, char* args[] ) {
  //Start up SDL and create window
  if( !init() ) {
    printf( "Failed to initialize!\n" );
  }
  else {
    //Load media
    if( !loadMedia() ) {
      printf( "Failed to load media!\n" );
    }
    else { 
      //Main loop flag
      
      // SDL Event Handler
      SDL_Event e;
      
      /* initialize random seed: */
      srand (time(NULL));
      
      bool quit = false;
      bool pause = false;
      bool newGame = true;
      bool timerOngoing = false;
      auto timerStart = std::chrono::system_clock::now();
      auto timerEnd = std::chrono::system_clock::now();
      double timerElapsed = 0.;
      double timerCount = 0;
      
      Ball b = Ball();
      Paddle p1 = Paddle(Player::P_ONE);
      Paddle p2 = Paddle(Player::P_TWO);
      
      double MS_PER_UPDATE = 1000./500.;
      auto previous = std::chrono::system_clock::now();
      auto current = std::chrono::system_clock::now();
      double elapsed = (current - previous).count() / 4000;
      double lag = 0.0;

      //While application is running
      while( !quit ) {
        // Loop Maintenance Variables
        elapsed = (current - previous).count() / 4000;
        previous = current;
        lag += elapsed;
        
        std::vector<KeyInput> k = processInput(e);
        
        while (lag >= MS_PER_UPDATE) {
          k = processInput(e);
          if (newGame) {
            if ( !timerOngoing ) {
              timerStart = std::chrono::system_clock::now();
              timerOngoing = true;
            }
            timerEnd = std::chrono::system_clock::now();
            timerElapsed = (timerEnd - timerStart).count() / 1000;
            if (timerElapsed > 3000) {
              newGame = false;
              timerOngoing = false;
            }
          }
          if (k[0] == KeyInput::K_P) {
            pause = !pause;
          }
          if (newGame || !pause) {
            p1.update(k[1]);
            p2.update(k[2]);
          }
          if (!newGame && !pause) {
            b.update(p1, p2);
          }
          lag -= MS_PER_UPDATE;
        }
        
        if (b.x() <= 0 || b.x() >= SCREEN_WIDTH) {
          b.newGame();
          pause = false;
          newGame = true;
        }
        
        if (k[0] == KeyInput::K_QUIT) {
          quit = true;
        }
        
        render(b, p1, p2);
        
        current = std::chrono::system_clock::now();
      }
    }
  }

  //Free resources and close SDL
  close();

  return 0;
}