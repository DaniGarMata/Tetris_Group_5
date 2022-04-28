#include "ModuleFade.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include "SDL/include/SDL_render.h"

ModuleFade::ModuleFade(bool startEnabled) : Module(startEnabled)
{
	screenRect = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };

}

ModuleFade::~ModuleFade()
{

}

bool ModuleFade::Start()
{
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	return true;
}

Update_Status ModuleFade::Update()
{
//Fades an image into another

	if (currentStep == Fade_Step::FROM_IMAGE)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = Fade_Step::NONE;
		}
	}

	else{ return Update_Status::UPDATE_CONTINUE; }

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleFade::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return Update_Status::UPDATE_CONTINUE;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_RenderFillRect(App->render->renderer, &screenRect);

	return Update_Status::UPDATE_CONTINUE;
}

bool ModuleFade::Fade(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::FROM_IMAGE;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}