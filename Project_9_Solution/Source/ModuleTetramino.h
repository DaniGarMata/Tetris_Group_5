#ifndef __MODULE_TETRAMINO_H__
#define __MODULE_TETRAMINO_H__

#include "Module.h"


#define MAX_ENEMIES 100

enum class Tetramino_Type
{
	NO_TYPE,
	RED,
	PINK,
	BLUE,
	TEAL,
	ORANGE,
	YELLOW,
	GREEN,
};

struct EnemySpawnpoint
{
	Tetramino_Type type = Tetramino_Type::NO_TYPE;
	int x, y;
};

class Tetramino;
struct SDL_Texture;

class ModuleTetramino : public Module
{
public:
	// Constructor
	ModuleTetramino(bool startEnabled);

	// Destructor
	~ModuleTetramino();

	// Called when the module is activated
	// Loads the necessary textures for the enemies
	bool Start() override;

	// Called at the beginning of the application loop
	// Removes all enemies pending to delete
	Update_Status PreUpdate() override;

	// Called at the middle of the application loop
	// Handles all enemies logic and spawning/despawning
	Update_Status Update() override;

	// Called at the end of the application loop
	// Iterates all the enemies and draws them
	Update_Status PostUpdate() override;

	// Called on application exit
	// Destroys all active enemies left in the array
	bool CleanUp() override;

	// Called when an enemi collider hits another collider
	// The enemy is destroyed and an explosion particle is fired
	void OnCollision(Collider* c1, Collider* c2) override;

	// Add an enemy into the queue to be spawned later
	bool AddTetramino(Tetramino_Type type, int x, int y);

	// Iterates the queue and checks for camera position
	void HandleTetraminoSpawn();

	// Destroys any enemies that have moved outside the camera limits
	void HandleTetraminoDespawn();

private:
	// Spawns a new enemy using the data from the queue
	void SpawnTetramino(const EnemySpawnpoint& info);

private:
	// A queue with all spawn points information
	EnemySpawnpoint spawnQueue[MAX_ENEMIES];

	// All spawned enemies in the scene
	Tetramino* tetramino[MAX_ENEMIES] = { nullptr };

	// The enemies sprite sheet
	SDL_Texture* texture = nullptr;

	// The audio fx for destroying an enemy
	int tetraminoDestroyedFx = 0;
};

#endif // __MODULE_ENEMIES_H__