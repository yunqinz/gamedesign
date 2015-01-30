#ifndef MULTISPRITE__H
#define MULTISPRITE__H

#include <string>
#include <vector>
#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  virtual void Right();
  virtual void Left();
  int getWorldWidth(){return worldWidth;}
  int getFrameWidth(){return frameWidth;}
  
  int getARandomSpeedFactor(const std::string);
protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  bool frameRight;


  void advanceFrame(Uint32 ticks);
};
#endif
