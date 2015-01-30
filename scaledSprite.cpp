#include <cmath>
#include "scaledSprite.h"
#include "gamedata.h"
#include "frame.h"
#include "SDL/SDL_rotozoom.h"


int getARandomFactor(const std::string name){
  int num = 0;
  if(name == "star"){
  num = Gamedata::getInstance().getRandInRange(
                           Gamedata::getInstance().getXmlInt(name+"/min"), 
                           Gamedata::getInstance().getXmlInt(name+"/max"));
  num *= rand() % 2 ? - 1 : 1;
  return num;
  }
  else {
  num = 0;
  return num; 
  }
}

float getRandFloat(float a, float b){
  float random = ((float)rand()) / (float) RAND_MAX;
  float diff = b -a ;
  float ans = a + random*diff;
  return ans;
}

ScaledSprite::ScaledSprite(const std::string& name, SDL_Surface* surface) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX")
                                                      +getARandomFactor(name), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY") 
                                                      +getARandomFactor(name)) 
           ),
  scale(getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
                     Gamedata::getInstance().getXmlFloat(name+"/scale/max"))
  ),
  scaledSurface(rotozoomSurface(surface, 0, scale, SMOOTHING_ON)),
  frame( new Frame(scaledSurface, scaledSurface->w, scaledSurface->h,
                   Gamedata::getInstance().getXmlInt(name+"/srcX"), 
                   Gamedata::getInstance().getXmlInt(name+"/srcY"))
  ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
//  angle(0.0)
{ }
/*
ScaledSprite::ScaledSprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel), 
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  angle(0.0)
{ }
*/
/*
ScaledSprite::ScaledSprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  angle(0.0)
{ }
*/
ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()), 
  scale(s.scale),
  scaledSurface(s.scaledSurface),
  frame(s.frame),
/*
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  angle(0.0)
*/

  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
 // angle(s.angle)
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  Drawable::operator=( rhs );
  scale = rhs.scale;
  scaledSurface = rhs.scaledSurface;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
 // angle = rhs.angle;
  return *this;
}

ScaledSprite:: ~ScaledSprite(){
  SDL_FreeSurface(scaledSurface);
  delete frame;
}

void ScaledSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int ScaledSprite::getDistance(const ScaledSprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void ScaledSprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}
