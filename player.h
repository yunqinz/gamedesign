#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <list>
#include "gamedata.h"
#include "bullets.h"
#include "multisprite.h"
#include "collisionStrategy.h"
#include "health.h"
#include "smartSprite.h"

class Player {
public:
  Player(const std::string& name);
  virtual ~Player();

  virtual void update(Uint32 ticks);
 
  virtual void draw() const { 
      player.draw(); 
      bullets.draw();
      //strategy->draw();
  }
  const Drawable* getSprite() const { return &player;}
  Vector2f getPosition() const { return player.getPosition(); }
  float getX() const { return player.X(); }
  float getY() const { return player.Y(); }

  bool playercollideWith(const Drawable* d) const {
    if(lastCollision > 200){
        if(strategy->execute(player, *d)){
           lastCollision = 0;
           return true;
        }
        else return false;
      }
      else return false;
  }
  void setCollisionStrategy(int index) {
    strategy = strategies[index];
  }

  bool bulletCollideWith(Drawable* obj);
  void stop();// { 
    //player.velocityX(0); 
    //player.velocityY(0); 
  //}
  void right();
  void left();
  void up();
  void down();
  void shootRight();
  void shootLeft();
  //void advanceFrame(Uint32 ticks);
  int bulletCount()const{return bullets.bulletCount();}
  int freeCount()const{return bullets.freeCount();}
  void restart();
  void attach(SmartSprite* o){observers.push_back(o);}
  void detach(SmartSprite* o);

private:
  MultiSprite player;
  std::list<SmartSprite*> observers;
 // Health bar;
  float playerWidth;
  float playerHeight;
  Vector2f velocity;
  float worldWidth;
  float worldHeight;
  mutable float lastCollision;
  //attributes for bullet
  std::string bulletName;
  Bullets bullets;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastBullet;
  

  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy * strategy;
  Player(const Player&);
  Player& operator=(const Player&);
};
#endif


