#include "SceneIntro2.h"

#include "Application.h"
#include "Collider.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFade.h"

#include <time.h>

SceneIntro2::SceneIntro2(bool startEnabled) : Module(startEnabled)
{

}

SceneIntro2::~SceneIntro2()
{

}

// Load assets
bool SceneIntro2::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	srand(time(NULL));

	for (size_t i = 0; i < 5; ++i) { //Init fireworks delay
		fwTimers[i] = rand() % 300;
	}

	bgTexture = App->textures->Load("Assets/Sprites/Title_screen10.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;
	creditFx = App->audio->LoadFx("Assets/Fx/01_Credit.wav");

	return ret;
}

Update_Status SceneIntro2::Update()
{
	for (size_t i = 0; i < 5; ++i) {
		fwTimers[i]--;
		if (fwTimers[i] < 1) {
			if (i == 1 || i == 5) {
				App->particles->AddParticle(App->particles->fireworks3, fwX[i], fwY[i], Collider::Type::NONE, 0);
			}
			else if (i % 2 == 0) {
				App->particles->AddParticle(App->particles->fireworks1, fwX[i], fwY[i], Collider::Type::NONE, 0);
			}
			else {
				App->particles->AddParticle(App->particles->fireworks2, fwX[i], fwY[i], Collider::Type::NONE, 0);
			}
			fwTimers[i] = (rand() % 300) + 200;
		}
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->audio->PlayFx(creditFx);
		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_1, 90);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneIntro2::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}