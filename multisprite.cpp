#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
int MultiSprite::getARandomSpeedFactor(const std::string name){
  int num = 0;
  if(name == "targetstar"){
  num = Gamedata::getInstance().getRandInRange(
                           Gamedata::getInstance().getXmlInt(name+"/minSpeed"), 
                           Gamedata::getInstance().getXmlInt(name+"/maxSpeed"));
  num *= rand() % 2 ? - 1 : 1;
  return num;
  }
  else {
  num = 0;
  return num; 
  }
}

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % (numberOfFrames/2);
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX")+getARandomSpeedFactor(name),
                    Gamedata::getInstance().getXmlInt(name+"/speedY")+getARandomSpeedFactor(name))
           ),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  frameRight(true)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  frameRight(true)
  { }

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  //frames[currentFrame]->draw(x, y,getAngle());
  frames[currentFrame]->draw(x, y);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    //velocityX( abs( velocityX() ) );
    frameRight=true;
  }
  if ( X() > worldWidth-frameWidth) {
    //velocityX( -abs( velocityX() ) );
    frameRight=false;
  }  
  //
  if(velocityX()<0&&!frameRight)
  {
	  currentFrame =(numberOfFrames/2);
  }
}
void MultiSprite::Right()
{
  frameRight=true;
  velocityX(abs( velocityX()));
}
void MultiSprite::Left()
{
  frameRight=false;
}
