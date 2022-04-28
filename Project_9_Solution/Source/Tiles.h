#ifndef __TILES_H__
#define __TILES_H__

#include "SDL/include/SDL_rect.h"

class Tiles {

private:

	int x, y;

	SDL_Rect* tile = nullptr;

	bool isOccupied = false;

public:

	Tiles(int x0, int y0);

	~Tiles();

	void Occupy(SDL_Rect* occupTile);

	void Unoccupy(Tiles* tiletoOccupy);

};

#endif
