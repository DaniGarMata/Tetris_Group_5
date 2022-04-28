#ifndef __ENEMY_BROWNSHIP_H__
#define __ENEMY_BROWNSHIP_H__

#include "Tetramino.h"
#include "Path.h"

class Pink_Tetramino : public Tetramino
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Pink_Tetramino(int x, int y);

	// The enemy is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

private:
	
	SDL_Rect* piece[4];

};

#endif // __ENEMY_BROWNSHIP_H__