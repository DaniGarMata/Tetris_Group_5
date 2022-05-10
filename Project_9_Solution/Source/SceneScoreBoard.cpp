#include "SceneScoreBoard.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

SceneScoreBoard::SceneScoreBoard(bool startEnabled) : Module(startEnabled)
{

}

SceneScoreBoard::~SceneScoreBoard()
{

}

// Load assets
bool SceneScoreBoard::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/Screen Void.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->hiscore = App->player->score;
	App->player->score = 0;
	App->player->round = 1;

	return ret;
}

Update_Status SceneScoreBoard::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneGameOver, 90);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneScoreBoard::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}

