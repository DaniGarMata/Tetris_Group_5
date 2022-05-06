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

	Update_Status PreUpdate() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	Update_Status Update() override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	Update_Status PostUpdate() override;

	void TetraminoCheck();

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
	bool MovablePiece = true;
	bool gameOver = false;

	// Sound effects indices
	uint laserFx = 0;
	uint explosionFx = 0;

	//Tetramino_Orientation orientation = Tetramino_Orientation::RIGHT;

	// Font score index
	uint score = 000;
	uint hiscore = 000;
	uint linesLeft = 005;
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
	bool lineIsMade;

	int ctr = 120;

	int Tctr = 0;
	int prov;

	int col = 5;
	int row = 0; //@@ la columna y la fila en la que se encuentra el bloque, siendo la columna 5 la inicial, son variables internas de player pero est疣 sinncronizadas con la matriz
	int Map[20][10] = { {0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {0,0,0,0,0,0,0,0,0,9},
					  {9,9,9,9,9,9,0,9,9,9},
					  {9,9,9,9,9,9,0,9,9,9},
					  {9,9,9,9,9,9,0,9,9,9} };
	//@@ la matriz que hace de mapa, intent・hacerla en una clase externa en la que se cambiasen los valores desde las funciones desde las funciones de Player y hubiese funciones que 
	//@@limpiasen una fila, que comprobasen si alguna fila est・completa, que desplazasen todas las casillas ocupadas, que comprobasen si hay algun bloque en la fila superior... pero no 
	//@@ me aclar・ 0 equivale a una casilla vac僘, 1 ser僘 la posici en la que se encuentra el bloque (cosa que pasa en partida) y 9 a una casilla ocupada(si cambias 0 por 9 habr・  
	//@@ obstaculos en la partida), los numeros han sido elegidos arbitrariamente y no hace falta usar estos

	//Current Tetramino
	//Tetramino* current;
	//Tiles* board[10][20];
};

#endif //!__MODULE_PLAYER_H__