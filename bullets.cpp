#include <cmath>
#include <iostream>
#include "ioManager.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "bullets.h"



CollisionStrategy* getStrategy(const std::string& name)
{
	std::string sName=Gamedata::getInstance().getXmlStr(name+"strategy");
	if(sName=="midpoint") return new MidPointCollisionStrategy;
	if(sName=="rectangular") return new RectangularCollisionStrategy;
	if(sName=="perpixel") return new PerPixelCollisionStrategy;
	throw std::string("No strategy in getStrategy");

}
Bullets::~Bullets()
{
	//SDL_FreeSurface(bulletSurface);
	//delete bulletFrame;
	SDL_FreeSurface(bulletSurface);
	delete strategy;
}
Bullets::Bullets(const std::string& n):
name(n),
strategy(getStrategy(name)),
bulletSurface(IOManager::getInstance().loadAndSet(
	Gamedata::getInstance().getXmlStr(name+"/file"),true)
             ),
//bulletFrame(new Frame())
bulletFrame(FrameFactory::getInstance().getFrame(name)),
frameInterval(Gamedata::getInstance().getXmlInt(name+"/BulletInterval")),
timeSinceLastFrame(0.0),
bulletList(),
freeList()
{}

Bullets::Bullets(const Bullets& b):
name(b.name),
strategy(b.strategy),
bulletSurface(b.bulletSurface),
//bulletFrame(new Frame())
bulletFrame(b.bulletFrame),
frameInterval(b.frameInterval),
timeSinceLastFrame(b.timeSinceLastFrame),
bulletList(b.bulletList),
freeList(b.freeList)
{}

bool Bullets::collideWith(const Drawable* obj) const
{
	std::list<Bullet>::iterator ptr=bulletList.begin();
	while(ptr!=bulletList.end())
	{
		if(strategy->execute(*ptr,*obj))
		{
			freeList.push_back(*ptr);
			ptr=bulletList.erase(ptr);
			return true;
		}
		++ptr;
	}
	return false;
}

void Bullets::shoot(const Vector2f& position,const Vector2f& velocity)
{
	if(timeSinceLastFrame>frameInterval)
	{
		if(freeList.empty())
		{
			Bullet b(name,position,velocity,bulletFrame);
			bulletList.push_back(b);
		}
		else
		{
			Bullet b=freeList.front();
			freeList.pop_front();
			b.reset();
			b.setVelocity(velocity);
			b.setPosition(position);
			bulletList.push_back(b);
		}
		timeSinceLastFrame=0;
	}
}
void Bullets::draw() const
{
	
	std::list<Bullet>::const_iterator ptr=bulletList.begin();
	while(ptr!=bulletList.end())
	{
		ptr->draw();
		++ptr;
	}
}

void Bullets::update(Uint32 ticks)
{
	timeSinceLastFrame +=ticks;
	std::list<Bullet>::iterator ptr=bulletList.begin();
	while(ptr!=bulletList.end())
	{
		ptr->update(ticks);
		if(ptr->goneTooFar())
		{
			freeList.push_back(*ptr);
			ptr=bulletList.erase(ptr);
		}
		++ptr;
	}
}
