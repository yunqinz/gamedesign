#include <iostream>
#include <cmath>
#include "SDL/SDL_rotozoom.h"
#include "rotscaledMultisprite.h"
#include "extractSurface.h"
#include "gamedata.h"
#include "ioManager.h"

RotateScaledMultisprite::~RotateScaledMultisprite() {
  SDL_Surface* surface = NULL;
  for (unsigned i = 0; i < frames.size(); ++i) {
    surface = const_cast<Frame*>(frames[i])->getSurface();
    SDL_FreeSurface(surface);
    delete frames[i];
  }
}

RotateScaledMultisprite::RotateScaledMultisprite(const std::string& name) :
  Drawable(name, 
    Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y")+
             rand()%Gamedata::getInstance().getXmlInt(name+"/cushion")), 
    Vector2f(
       Gamedata::getInstance().
       getRandFloat(Gamedata::getInstance().getXmlInt(name+"/minSpeed/x"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/x")),
       Gamedata::getInstance().
       getRandFloat(Gamedata::getInstance().getXmlInt(name+"/minSpeed/y"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/y")))
    ),
  scale(
       Gamedata::getInstance().
       getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
                    Gamedata::getInstance().getXmlFloat(name+"/scale/max"))
  ),
  frames(),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  rotating(false),
  angle(0.0),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( scale*
    Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth( scale*Gamedata::getInstance().getXmlInt(name+"/width") ),
  frameHeight( scale*Gamedata::getInstance().getXmlInt(name+"/height") )
{ 
  IOManager& io = IOManager::getInstance();
  SDL_Surface* surface = 
    io.loadAndSet(Gamedata::getInstance().getXmlStr(name+"/file"), true);
  double oldWidth = surface->w;
  // need to subtract 0.5 because rotozoom will ceil it!
  double newWidth = 
    numberOfFrames*ceil((oldWidth*scale-0.5)/numberOfFrames);
  //Since scale*width0 = width1, scale = width1/width0
  scale = newWidth/oldWidth;
  frameInterval *= scale;
  SDL_Surface*
  scaledSurface = rotozoomSurface(surface, 0, scale, SMOOTHING_ON);
  SDL_FreeSurface( surface );
  makeFrames(scaledSurface);
  SDL_FreeSurface( scaledSurface );
}

RotateScaledMultisprite::
RotateScaledMultisprite(const RotateScaledMultisprite& s) :
  Drawable(s.getName(),
           s.getPosition(), s.getVelocity()), 
  scale(s.scale),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  rotating( s.rotating ),
  angle( s.angle ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
{ }

void RotateScaledMultisprite::makeFrames(SDL_Surface* surface) {
  const Gamedata& gdata = Gamedata::getInstance();

  unsigned numberOfFrames = gdata.getXmlInt(getName()+"/frames");
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(getName()+"/src/x");
  Uint16 srcY = gdata.getXmlInt(getName()+"/src/y");
  Uint16 width = surface->w/numberOfFrames;
  Uint16 height = surface->h;
  frameWidth = width;
  frameHeight = height;

  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
    frames.push_back( 
      new Frame(surface, frameX, srcY, width, height) 
    );
  }

  // Stagger the vertical speed by the scaling factor;
  // this makes smaller objects fly slower:
  // velocityY( scale*velocityY() );
  //setVelocity( scale*getVelocity() );
  //if ( rand() % 2 ) {
    //velocityX( -velocityX() );
  //}
}

void RotateScaledMultisprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

void RotateScaledMultisprite::rotate() { 
  if ( isRotating() ) return;
  rotating = true;
  ++angle;
}

void RotateScaledMultisprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if ( isRotating() ) {
    frames[currentFrame]->draw(x, y, angle);
  }
  else {
    frames[currentFrame]->draw(x, y);
  }
}

void RotateScaledMultisprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  if ( isRotating() ) {
    if ( angle >= 360 ) {
      angle = 0;
      rotating = false;
    }
    if (angle) angle += 1;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X()+frameWidth < 0) {
    //velocityX( abs( velocityX() ) );
    X(worldWidth);
  }
  //if ( X() > worldWidth-frameWidth) {
    //velocityX( -abs( velocityX() ) );
  //}  

}
