#include <iostream>
#include <SDL.h>
#include "rage.h"

using namespace std;


void render(GameState *st, SDL_Surface *screen)
{
	int i;
	SDL_Surface *surf;
	
	// dirt layer
	surf = st->map->renderWallFrame(0, false);
	SDL_BlitSurface(surf, NULL, screen, 0);
	SDL_FreeSurface(surf);
	
	// wall layer
	surf = st->map->renderWallFrame(0, true);
	SDL_BlitSurface(surf, NULL, screen, 0);
	SDL_FreeSurface(surf);
	
	// data (testing)
	//surf = st->map->renderDataSurface();
	//SDL_BlitSurface(surf, NULL, screen, 0);
	//SDL_FreeSurface(surf);
	
	for (i = 0; i < st->numUnits(); i++) {
		Player *e = (Player*) st->getUnit(i);
		
		SDL_Rect r;
		r.x = e->x;
		r.y = e->y;
		
		SDL_BlitSurface(e->getSprite(), NULL, screen, &r);
	}
	
	SDL_Flip(screen);
}
