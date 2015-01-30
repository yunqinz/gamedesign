#include "player.h"
Player::~Player() {
  for (unsigned i = 0; i < strategies.size(); ++i) {
    delete strategies[i];
  }
}

Player::Player(const std::string& name) : 
  player(name),
  observers(), 
  playerWidth( player.getFrame()->getWidth() ), 
  playerHeight( player.getFrame()->getHeight() ),
  velocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  lastCollision(0),
  bulletName("bullets"),
  bullets(bulletName),
  minSpeed(300.0),
  bulletInterval(2.0),
  timeSinceLastBullet(10.0),
  strategies(),
  strategy( NULL )
{
  strategies.push_back( new MidPointCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategy = strategies[2];
}

  void Player::stop() { 
    player.velocityX(0); 
    player.velocityY(0); 
  }
void Player::left()  { 
  if ( player.X() > 0) {
    player.Left();
    player.velocityX(velocity[0]);
  }
} 

void Player::right()  { 
  if ( player.X() < worldWidth-playerWidth) {
    player.Right();
    player.velocityX(-velocity[0]);
   // player.update();    
  }
} 
void Player::up()    { 
  if ( player.Y() > 0) {
    player.velocityY(-velocity[1]);
  }
} 
void Player::down()  { 
  if ( player.Y() < worldHeight-playerHeight) {
    player.velocityY(velocity[1]);
  }
} 

void Player::shootLeft()
{
  
  if(timeSinceLastBullet>bulletInterval)
  {
   
    float x= getX()+player.getFrame()->getWidth();
    float y= getY()+player.getFrame()->getHeight()/2;
    //not adding minspeed to y velocity
    bullets.shoot(Vector2f(x,y), Vector2f(minSpeed+player.velocityX(),player.velocityY()));
    //bullets.draw();
    //bullets.update(ticks);
  }
}
void Player::shootRight()
{
  
  if(timeSinceLastBullet>bulletInterval)
  {
   
    float x= getX()+player.getFrame()->getWidth();
    float y= getY()+player.getFrame()->getHeight()/2;
    //not adding minspeed to y velocity
    bullets.shoot(Vector2f(x,y), Vector2f(player.velocityX(),player.velocityY()));
    //bullets.draw();
    //bullets.update(ticks);
  }
}
bool Player::bulletCollideWith(Drawable* obj)
{
 
   return bullets.collideWith(obj);
  
}

void Player::restart()
{
  player.X(Gamedata::getInstance().getXmlInt(player.getName()+"/startLoc/x")); 
  player.Y(Gamedata::getInstance().getXmlInt(player.getName()+"/startLoc/y"));
  
}

void Player::detach(SmartSprite* o){
  std::list<SmartSprite*>::iterator it = observers.begin();
  while(it != observers.end()){
    if((*it) == o){
      it = observers.erase(it);
      return; 
    }
    ++it;
  }
}

void Player::update(Uint32 ticks) 
{
  player.update(ticks);
  bullets.update(ticks);
  lastCollision += ticks;
  std::list<SmartSprite*>::iterator ptr = observers.begin(); 
  while(ptr!=observers.end()){
    (*ptr)->setPlayerPos(getPosition());
    ++ptr; 
  }
}
