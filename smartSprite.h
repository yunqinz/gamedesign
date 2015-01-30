#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H

#include <iostream>
#include "sprite.h"
#include "ioManager.h"

class SmartSprite : public Sprite{
public:
  SmartSprite(const std::string, const Vector2f& pos, int w, int h);
  void draw()const;
  void update(Uint32);
  void goUp();
  void goDown();
  void goLeft();
  void goRight();

  void incrSafeDistance(){safeDistance++;}
  void decrSafeDistance(){safeDistance--;}
  float getSafeDistance(){return safeDistance;}
  void setPlayerPos(const Vector2f& pos){playerPos = pos;}

private:
  enum MODE{NORMAL,EVADE};
  MODE 	currentMode;
  IOManager& io;
  Vector2f Vel;
  Vector2f playerPos;
  float safeDistance;
  int playerWidth;
  int playerHeight;
  SmartSprite(const SmartSprite&);
  SmartSprite& operator=(const SmartSprite&);
};

#endif
