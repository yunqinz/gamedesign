#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include <list>
#include "hud.h"
#include "player.h"
#include "scaledSprite.h"
#include "smartSprite.h"
#include "sprite.h"
#include "explodingSprite.h"
#include "health.h"
#include "bullets.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void pause() { clock.pause(); }
  void unpause() { clock.unpause(); }
  void setNumberOfSprites(int);
  void restart();
  void makeSprites();
  

private:

  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDL_Surface * const screenWin;
  SDL_Surface * const screenLose;
  

  SDLSound sound;
  //World world;
   World worldBack;
   World worldFront;
   //World worldTree;
   World worldGrass;
   
  Viewport& viewport;
  Hud hud;
  SDL_Surface* orbSurface;
  std::vector<ScaledSprite*> orbs;

  //std::list<Sprite*> targets;
 // std::list<Sprite*> explodingTargets;
  Player player;
  Health bar;
  //std::string bulletName;
  //Bullets bullets;
  int f1count;
  int collisionCount;
  int spritesnumber;

  std::vector <Drawable*> sprites;
  std::vector <ExplodingSprite*> explodingSprites;
  std::vector <SmartSprite*> smarts;

  int currentSprite;
  int Number;

  bool makeVideo;
  bool Lose;
  bool Win;

  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  void draw() const;
  void update();
  //void makeOrbs();
  //void printOrbs() const;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
/////////////////
  void makeSmarts();
};
