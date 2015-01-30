#ifndef ROTATESCALEDMULTISPRITE__H
#define ROTATESCALEDMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class RotateScaledMultisprite : public Drawable {
public:
  RotateScaledMultisprite(const std::string&);
  RotateScaledMultisprite(const RotateScaledMultisprite& s);
  virtual ~RotateScaledMultisprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { return frames[currentFrame]; }
  double getScale() const { return scale;    }
  bool isRotating() const { return rotating; }
  void rotate();

private:
  double scale;
  std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;
  bool rotating;
  float angle;

  unsigned currentFrame;
  unsigned numberOfFrames;
  float frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
  void makeFrames(SDL_Surface*);
  RotateScaledMultisprite& operator=(const RotateScaledMultisprite&);
};
#endif
