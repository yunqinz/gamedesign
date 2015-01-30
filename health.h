#ifndef HEALTH_H
#define HEALTH_H

#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"
#include "player.h"

class Health {

friend class Player;
public:
  Health();
  Health(int sx, int sy, int tl, int cl, 
         float t, int inc, Uint32 c, float sp);
  void draw() const;
  void update();
  void reset() { currentLength = totalLength; }
  void setGodMode();
  bool getCurrentLength()const{return currentLength;}
  void restart(){currentLength = totalLength;}
private:
  SDL_Surface* screen;
  Vector2f start;
  bool god;
  int totalLength;
  int currentLength;
  int thick;
  int increments;
  float interval;
  int deltaTime;
 // Player player;
  const Uint32 RED;
  const Uint32 GRAY;
  const Uint32 BLACK;
  const Uint32 color;
  void drawBox() const;
  Health(const Health&);
  Health& operator=(const Health&);
};
#endif

