#include <stdio.h>

#include "SceneIntro.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{

}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	char s[128];

	for (size_t i = 0; i < TITLE_SCREEN_NFRAMES; ++i)
	{
		sprintf_s(s, "Assets/Sprites/Title_screen%d.png", i);
		bgTexture[i] = App->textures->Load(s);
	}
	delay = TITLE_SCREEN_FRAME_DELAY;	//60fps -> 12 : 1/5s
	frame = 0;
	countdown = TITLE_SCREEN_START_COUTNDOWN;

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneIntro::Update()
{

	if (!anim)
	{
		countdown--;
		if (countdown == 0) {
			anim = true;
		}
		/* Use if day-night animation is started by pressing Space instead of 4-sec countdown:
		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN) {
			anim = true;
		}*/
	}
	else
	{
		delay--;
		if (delay == 0)
		{
			if (frame < TITLE_SCREEN_NFRAMES - 1)
			{
				frame++;
				delay = TITLE_SCREEN_FRAME_DELAY;
			}
			else {
				App->fade->FadeToBlack(this, (Module*)App->sceneIntro_2, 90);
			}
		}
	}
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneIntro_2, 90);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture[frame], 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}