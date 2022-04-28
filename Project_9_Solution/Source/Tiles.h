#ifndef __TILES_H__
#define __TILES_H__

class Tiles {

private:

	uint x, y;

	SDL_Rect* tile = nullptr;

	bool isOccupied;

public:

	Tiles(uint x0, uint y0, bool isOccupied0);

	~Tiles();

	void Occupy(SDL_Rect* occupTile);

};

#endif
