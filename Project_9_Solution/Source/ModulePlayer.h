#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "Tetramino.h"
#include "Tiles.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	// Constructor
	ModulePlayer(bool startEnabled);

	// Destructor
	~ModulePlayer();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	Update_Status Update() override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	Update_Status PostUpdate() override;

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2) override;

public:
	// Position of the player in the map
	iPoint position;

	// The speed in which we move the player (pixels per frame)
	int spdx = 0;
	int spdy = 1;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnim;
	Animation upAnim;
	Animation downAnim;

	// The player's collider
	Collider* collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool MovablePiece;
	bool gameOver = false;

	// Sound effects indices
	uint laserFx = 0;
	uint explosionFx = 0;

	Tetramino_Orientation orientation = Tetramino_Orientation::RIGHT;

	// Font score index
	uint score = 000;
	uint hiscore = 000;
	uint linesLeft = 000;
	uint lines = 000;
	uint round = 001;
	int scoreFont = -1;
	int HiscoreFont = -1;
	int linesLeftFont = -1;
	char scoreText[10] = { "\0" };
	char HiscoreText[10] = { "\0" };
	char linesText[10] = { "\0" };
	char linesLeftText[10] = { "\0" };
	char roundText[10] = { "\0" };

	//Current Tetramino
	Tetramino* current;
	Tiles* board[10][20];
};

#endif //!__MODULE_PLAYER_H__