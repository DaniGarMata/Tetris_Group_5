#include "SceneLevel2.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleTetramino.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"

SceneLevel2::SceneLevel2(bool startEnabled) : Module(startEnabled)
{

}

SceneLevel2::~SceneLevel2()
{

}

// Load assets
bool SceneLevel2::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/Play_Screen.png");
	App->audio->PlayMusic("Assets/Music/04-Bradinsky.ogg", 1.0f);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->tetramino->Enable();

	App->player->linesLeft = 10;
	App->player->round++;

	return ret;
}

Update_Status SceneLevel2::Update()
{

	App->player->TetraminoCheck();

	if (App->player->gameOver == true) {

		App->fade->FadeToBlack(this, (Module*)App->sceneGameOver, 90);

	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel2::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLevel2::CleanUp()
{
	App->player->Disable();
	App->tetramino->Disable();

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)

	return true;
}