#include <cmath>
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string name, 
                          const Vector2f& pos, int w, int h):

  Sprite(name,pos,
    Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
             Gamedata::getInstance().getXmlInt(name+"/speedY"))),
   currentMode(NORMAL),
  io(IOManager::getInstance()),
  Vel(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX")*1.5, 
             Gamedata::getInstance().getXmlInt(name+"/speedY")*1.5)),
  playerPos(),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance")),
  playerWidth(w),
  playerHeight(h)
{ }  

void SmartSprite::draw()const{
  Sprite::draw();
}

void SmartSprite::goUp(){velocityY(-abs(Vel[1]));}
void SmartSprite::goDown(){velocityY(abs(Vel[1]));}
void SmartSprite::goLeft(){velocityX(-abs(Vel[0]));}
void SmartSprite::goRight(){velocityX(abs(Vel[0]));}

void SmartSprite::update(Uint32 ticks){
  Sprite::update(ticks);
  float x= X()+getFrame()->getWidth()/2;
  float y= Y()+getFrame()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );//!!!

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
    }
  }
}



