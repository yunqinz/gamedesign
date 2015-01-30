#include <stdint.h>
#include <SDL.h>

class Hud
{
	public:
	Hud ();
	void drawHUD(SDL_Surface* screen, int x, int y)const;
	
	int getstartX()const;
	int getstartY()const;
	int getHUD_WIDTH()const;
    int getHUD_HEIGHT()const;
	
	private:
	 int HUD_WIDTH;// = 200;
	 int HUD_HEIGHT;// = 100;
     int startX; // = 40;
     int startY; //= 40;
	 void Draw_Pixel(SDL_Surface* s, int x, int y, 
                uint8_t r, uint8_t g, uint8_t b, uint8_t a)const;
	 void Draw_AALine(SDL_Surface* screen, float x0, float y0, 
                 float x1, float y1, float thick,
                 uint8_t r, uint8_t g, uint8_t b, uint8_t a)const;
	 void Draw_AALine(SDL_Surface* screen, float x0, float y0, 
                 float x1, float y1, float thick, uint32_t color)const;
	 
	void Draw_AALine(SDL_Surface* screen, float x0, float y0, 
                 float x1, float y1, uint32_t color)const;
	
};

