#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "rotscaledMultisprite.h"
//#include "bullets.h"
#include "explodingSprite.h"

#include <vector>
#include <algorithm>

class ScaledSpriteCompare{
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs){
    return lhs->getScale() < rhs->getScale();
  }
};



Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  
 
    SDL_FreeSurface(orbSurface);
    SDL_FreeSurface(screenWin);
    SDL_FreeSurface(screenLose);
    


    for(unsigned int i=0;i<smarts.size();++i)
    { 
      player.detach(smarts[i]);
      delete smarts[i];
    }

    for(unsigned int i=0;i<sprites.size();++i)
    {
      delete sprites[i];
    }
    for(unsigned int i = 0; i< orbs.size(); ++i){
    delete orbs[i];
    }
    orbs.clear();

    for(unsigned int i=0;i<explodingSprites.size();++i)
    {
      delete explodingSprites[i];
    }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  screenWin(io.loadAndSet(
            Gamedata::getInstance().getXmlStr("screenWin/file"),
            Gamedata::getInstance().getXmlBool("screenWin/transparency"))),
  screenLose(io.loadAndSet(
            Gamedata::getInstance().getXmlStr("screenLose/file"),
            Gamedata::getInstance().getXmlBool("screenLose/transparency"))),


  sound(),
  //world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  worldBack("back",Gamedata::getInstance().getXmlInt("back/factor")),
  worldFront("front",Gamedata::getInstance().getXmlInt("front/factor")),
  //worldTree("tree",Gamedata::getInstance().getXmlInt("tree/factor")),
  worldGrass("grass",Gamedata::getInstance().getXmlInt("grass/factor")),
  viewport( Viewport::getInstance() ),
  hud(),
  orbSurface(io.loadAndSet(Gamedata::getInstance().getXmlStr("star/file"),
             Gamedata::getInstance().getXmlBool("star/transparency"))
  ),
  orbs(),
  player("player"),
  bar(),
  //bulletName("bullets"),
  //bullets(bulletName),
  f1count(0),
  collisionCount(0),
  spritesnumber(0),
  sprites(),
  explodingSprites(),
////////////////////////
  smarts(),
////////////////////////
  currentSprite(0),
  Number(10),

  makeVideo( false ),
  Lose(false),
  Win(false),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
  {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
     throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);

  //makeOrbs();
  //printOrbs();
////////////////////////////////////////////////
  makeSmarts();
/////////////////////
  
  viewport.setObjectToTrack(player.getSprite());

}
void Manager::makeSprites()
{
  int spritesnumber=Number;//Gamedata::getInstance().getXmlInt("spritesnumber");
  for(int i=0;i<spritesnumber;++i)
  {
    sprites.push_back( new Sprite("greenorb") );
    sprites.push_back( new MultiSprite("targetstar") );
  }
}
/*
void Manager::makeOrbs(){
  unsigned int numberOfOrbs = Gamedata::getInstance().getXmlInt("numberOfOrbs");
  orbs.reserve(numberOfOrbs);
  for(unsigned int i =0; i< numberOfOrbs; ++i){
    orbs.push_back(new ScaledSprite("star",orbSurface));
  }
  sort(orbs.begin(),orbs.end(),ScaledSpriteCompare());  
}

void Manager::printOrbs() const{
  for (unsigned i = 0; i<orbs.size(); ++i){
    std::cout<<orbs[i]->getScale()<<std::endl;
  }
}
*/
/////////////////////////////////////////
void Manager::makeSmarts(){
  int X = Gamedata::getInstance().getXmlInt("smart/startLoc/x");
  int Y = Gamedata::getInstance().getXmlInt("smart/startLoc/y");
  int w = Gamedata::getInstance().getXmlInt("smart/width");
  int h = Gamedata::getInstance().getXmlInt("smart/height");
  unsigned int number = Gamedata::getInstance().getXmlInt("smart/number");
  for(unsigned int i = 0; i < number; i++){
    SmartSprite* s = new SmartSprite("smart",Vector2f(X,Y),w,h);
    smarts.push_back(s);
    player.attach(s);
  }
}
////////////////////////////////////////
void Manager::setNumberOfSprites(int number) {
  Number=number;
  if ( number > static_cast<int>( sprites.size() ) ) {
    number = (number - sprites.size())/2;
    for (int i = 0; i < number; ++i) {
      sprites.push_back( new MultiSprite("targetstar") );
      sprites.push_back( new Sprite("greenorb") );
    }
  }
  else {
    number = sprites.size()-number;
    for (int i = 0; i < number; ++i) {
      delete sprites.back();
      sprites.pop_back();
    }
  }
  
}
void Manager::restart()
{
  player.restart();
  bar.restart();
  std::vector<Drawable*>::iterator it=sprites.begin();
  while(it!=sprites.end())
  {
    delete *it;
    it=sprites.erase(it);
    Lose=false;
    Win=false;
  }
  makeSprites();
}
////////////////////////////////////////

void Manager::draw() const {
 // world.draw();

  worldBack.draw();
  
  
  /*for (unsigned i = 0; i<orbs.size()/2; ++i){
    orbs[i]->draw();
  }*/


  worldFront.draw();
 // worldTree.draw();
  worldGrass.draw();
  

  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  for (unsigned i = 0; i < explodingSprites.size(); ++i) {
    explodingSprites[i]->draw();
  }
///////////////////////////////////////////////////////////
  for (unsigned i = 0; i < smarts.size(); ++i) {
      smarts[i]->draw();

  }
////////////////////////////////////////////////////
  bar.draw();

  if(bar.getCurrentLength()!=0)
  {
     player.draw();
  } 
  
  if(clock.getSeconds()<3||(f1count%2)){
  hud.drawHUD(screen, hud.getstartX(), hud.getstartY());
  io.printMessageAt(title, 10, 10);  
  //io.printMessageValueAt("collisions:",collisionCount,10,25);
  //io.printMessageValueAt("Explosions:",explodingTargets.size(),300,30);
  //io.printMessageValueAt("Targets:",targets.size(),300,50);
  io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 40);
  io.printMessageValueAt("AvgFps: ", clock.getAvgFps(), 10, 55);
  
  io.printMessageValueAt("Bullets: ",player.bulletCount(),10,70);
  io.printMessageValueAt("FreeList: ",player.freeCount(),10,85);
  //io.printMessageAt("Press T to switch sprites", 10, 100);
  io.printMessageAt("Press f1 to reappear the hud", 10,115);
  io.printMessageAt("Press f2 to reset health meter", 10,100);
  io.printMessageAt("Press g to start the God Model", 10,130);
  //io.printMessageAt("Press r to restart the game", 10,145);

  }
if(Lose)
{
  //Lose=false;
  SDL_Rect dest = {0,0,0,0};
  SDL_BlitSurface( screenLose, NULL, screen, &dest );
}
if(Win)
{
  SDL_Rect dest = {50,50,0,0};
  SDL_BlitSurface( screenWin, NULL, screen, &dest );

}
 
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
    for (unsigned int i = 0; i < orbs.size(); ++i) {
    orbs[i]->update(ticks);
   }
   ///////
   if(bar.getCurrentLength()<=0)
   {
    Lose=true;
   }


  std::vector<Drawable*>::iterator spit=sprites.begin();
  while(spit!=sprites.end())
  {

    (*spit)->update(ticks);
    if(player.playercollideWith(*spit))
    {
      //explodingSprites.push_back(new ExplodingSprite(player.getSprite()));
      sound[5];
      bar.update();
      explodingSprites.push_back(new ExplodingSprite(*spit));
      delete (*spit);
      spit=sprites.erase(spit);
    }
    else if(player.bulletCollideWith(*spit))
    {
        explodingSprites.push_back(new ExplodingSprite(*spit));
       // sound[5];
        delete (*spit);
        spit=sprites.erase(spit);
    }
    else {++spit;}
  }

  std::vector<SmartSprite*>::iterator smit=smarts.begin();
  while(smit!=smarts.end())
  {
    (*smit)->update(ticks);
    if(player.playercollideWith(*smit))
    {
        Win=true;
        return;
    }
    ++smit;

  }

   std::vector<ExplodingSprite*>::iterator exsp=explodingSprites.begin();
   while(exsp!=explodingSprites.end())
   {
      (*exsp)->update(ticks);
      if((*exsp)->done())
      {
          delete (*exsp);
          exsp=explodingSprites.erase(exsp);
      }
      else {++exsp;}
   }

  /*std::list<Sprite*>::iterator expIt=explodingTargets.begin();
  while(expIt!=explodingTargets.end())
  {
    (*expIt)->update(ticks);
    if((*expIt)->done())
    {
      delete *expIt;
      expIt=explodingTargets.erase(expIt);
    }
    else
    {
      ++expIt;
    }
  }
  std::list<Sprite*>::iterator it=targets.begin();
  while(it!=targets.end())
  {
    (*it)->update(ticks);
    if(shootingTarget.collideWith(*it))
    {
      explodingTargets.push_back(new explodingTargets(*it));
      delete *it;
      it=targets.erase(it);
      ++collisionCount;
    }
    else  ++it;
  }
  shootingTarget.update(ticks);

*/
///////////////////////////////////////////////////////
/*
std::vector<SmartSprite*>::iterator its=smarts.begin();
  while(its!=smarts.end())
  {
    (*its)->update(ticks);
    if(player.playercollideWith(*its)) {
      //player.detach(*its);
      //bar.update();
      //explodingSprites.push_back(new ExplodingSprite(*its));
      //delete *its;
      //its=smarts.erase(its);
    }
    ++its;
  }
  */
///////////////////////////////////////////////


  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  player.update(ticks);
  //bar.update(ticks);
  player.stop();
  worldBack.update();
  worldFront.update();
  //worldTree.update();
  worldGrass.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  while ( not done ) {

    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]||Lose) {
        done = true;
        break;
      }
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
        currentSprite = (currentSprite+1) % sprites.size();
        viewport.setObjectToTrack(player.getSprite());
       }
       if (keystate[SDLK_p] && !keyCatch) {
        keyCatch = true;
        if ( clock.isPaused() ) clock.unpause();
        else clock.pause();
       }
        /*int i=0; 
        for ( std::list<Drawable*>::iterator it = sprites.begin(); it!=sprites.end(); ++it) {
        if(i==currentSprite)
    	{
			viewport.setObjectToTrack(sprites[it]);
			break;
    	}
    	++i;
  	}*/

          if(keystate[SDLK_F1] && !keyCatch)
          {
            keyCatch = true; 
            ++f1count;
          } 
          if (keystate[SDLK_F2]) 
          {
            bar.reset();
          }
           if (keystate[SDLK_g]) 
          {
            bar.setGodMode();
          }

          if(keystate[SDLK_F4]&&!makeVideo)
          {
            std::cout<<"making video frames"<<std::endl;
            makeVideo=true;
          }
          if(keystate[SDLK_d])
            {
              player.right(); 
            }
          if(keystate[SDLK_a]) 
            {
            player.left(); 
            }
          if(keystate[SDLK_w])
            {
            player.up(); 
            }
          if(keystate[SDLK_s]) 
            {
            player.down(); 
            }

          if(keystate[SDLK_a] && keystate[SDLK_d])
            {
             player.stop();
            }  
          if(keystate[SDLK_SPACE]&& !keyCatch )
          {
            keyCatch = true; 
            //if(keystate[SDLK_a])
             player.shootLeft();
             sound[1];
            //else if(keystate[SDLK_d])
             player.shootRight();
          }
          if(Win||Lose)
          {
            clock.pause();
          }
          

            /*if (keystate[SDLK_b] && !keyCatch) 
            {
              keyCatch = true; 
              static_cast<ScaledSprite*>(sprites[1])->rotate();
            }*/


    /*if (keystate[SDLK_b] && !keyCatch) {
        keyCatch = true; 
        static_cast<ScaledSprite*>(sprites[0])->rotate();
    }
      

      if (keystate[SDLK_c] && !keyCatch) {
      keyCatch = true; 
      static_cast<ScaledSprite*>(sprites[0])->rotate();
    }*/


    
}
    draw();
    update();
  }
 }


