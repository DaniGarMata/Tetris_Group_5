#include "Pink_Tetramino.h"

#include "Application.h"
#include "ModuleCollisions.h"

Pink_Tetramino::Pink_Tetramino(int x, int y) : Tetramino(x, y)
{

	
	
	collider = App->collisions->AddCollider({0, 0, 24, 24}, Collider::Type::ENEMY, (Module*)App->tetramino);
}

void Pink_Tetramino::Update()
{
	//path.Update();
	/*position = spawnPos + path.GetRelativePosition();*/

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Tetramino::Update();
}
