#include "Tiles.h"

Tiles::Tiles(int x0, int y0) {

	x = x0;

	y = y0;

}

void Tiles::Occupy(SDL_Rect* OccupTile) {

	OccupTile->x = x;

	OccupTile->y = y;

	tile = OccupTile;

	isOccupied = true;

}

void Tiles::Unoccupy(Tiles* tiletoOccupy) {

	tiletoOccupy->Occupy(tile);

	isOccupied = false; 

	tile = nullptr;

}
