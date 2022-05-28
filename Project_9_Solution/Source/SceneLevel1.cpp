#include "SceneLevel1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleTetramino.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"

SceneLevel1::SceneLevel1(bool startEnabled) : Module(startEnabled)
{

}

SceneLevel1::~SceneLevel1()
{

}

// Load assets
bool SceneLevel1::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/Play_Screen.png");
	App->audio->PlayMusic("Assets/Music/02-Loginska.ogg", 1.0f);

	//Bottomside collider
	//App->collisions->AddCollider({ 0, 198, 335, 200 }, Collider::Type::WALL);

	// Enemies --- 
	/*App->enemies->AddEnemy(Enemy_Type::REDBIRD, 600, 80);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 625, 80);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 640, 80);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 665, 80);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 735, 120);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 750, 120);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 775, 120);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 790, 120);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 830, 100);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 850, 100);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 870, 100);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 890, 100);

	App->enemies->AddEnemy(Enemy_Type::MECH, 900, 195);*/

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->tetramino->Enable();

	return ret;
}

Update_Status SceneLevel1::Update()
{

	App->player->TetraminoCheck();

	if(App->player->gameOver == true) {

		App->fade->FadeToBlack(this, (Module*)App->sceneGameOver, 90);

	}

	if (App->player->nextLevel == true) {

		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_2, 90);

	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLevel1::CleanUp()
{
	App->player->Disable();
	App->tetramino->Disable();

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)

	return true;
}