#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#define TITLE_SCREEN_NFRAMES 11
#define TITLE_SCREEN_FRAME_DELAY 10
#define TITLE_SCREEN_START_COUTNDOWN 240

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:
	//Constructor
	SceneIntro(bool startEnabled);

	//Destructor
	~SceneIntro();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	Update_Status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	Update_Status PostUpdate() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture[TITLE_SCREEN_NFRAMES] = { nullptr };
	unsigned int frame;
	unsigned int delay;
	unsigned int countdown;
	bool anim = false; //determines if day-night animation starts
};

#endif