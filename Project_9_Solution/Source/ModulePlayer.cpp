#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

#include <stdio.h>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	// idle animation - just one sprite
	idleAnim.PushBack({ 0, 0, 16, 24 });
	MovablePiece = true;

}


ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/PinkTetramino.png");
	currentAnimation = &idleAnim;

	laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 56;
	position.y = 37;

	gameOver = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 16, 28 }, Collider::Type::PLAYER, this);

	// TODO 0: Notice how a font is loaded and the meaning of all its arguments 
	//char lookupTable[] = { "!  ,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	//scoreFont = App->fonts->Load("Assets/Fonts/rtype_font.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);

	// TODO 4: Try loading "rtype_font3.png" that has two rows to test if all calculations are correct
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789:R =+-&([]|/_)!" };
	scoreFont = App->fonts->Load("Assets/Fonts/FontRed.png", lookupTable, 1);
	HiscoreFont = App->fonts->Load("Assets/Fonts/FontBlue.png", lookupTable, 1);
	linesLeftFont = App->fonts->Load("Assets/Fonts/FontWhite.png", lookupTable, 1);

	return ret;
}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll

	if (MovablePiece == true) {

		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT)
		{
			spdx = -1;
			position.x += (spdx);

		}

		if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT)
		{
			spdx = 1;
			position.x += (spdx);
		}

		if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			spdy = 2;
			position.y += (spdy);

		}

		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
		{
			

		}

		if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_IDLE) {

			position.y += (spdy);

		}

	}

	collider->SetPos(position.x, position.y);

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
	if (!gameOver)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%7d", score);
	sprintf_s(linesText, 10, "%7d", lines);
	sprintf_s(linesLeftText, 10, "%7d", linesLeft);
	sprintf_s(HiscoreText, 10, "%7d", hiscore);
	sprintf_s(roundText, 10, "%7d", round);

	// TODO 3: Blit the text of the score in at the bottom of the screen
	App->fonts->BlitText(65, 215, scoreFont, scoreText);
	App->fonts->BlitText(65, 223, scoreFont, linesText);
	App->fonts->BlitText(96, 111, scoreFont, linesLeftText);
	App->fonts->BlitText(128, 191, HiscoreFont, HiscoreText);
	App->fonts->BlitText(128, 207, HiscoreFont, roundText);

	App->fonts->BlitText(25, 215, scoreFont, "score");
	App->fonts->BlitText(25, 223, scoreFont, "lines");
	App->fonts->BlitText(151, 127, linesLeftFont, "lines");
	App->fonts->BlitText(151, 143, linesLeftFont, "left");
	App->fonts->BlitText(128, 183, HiscoreFont, "high score");
	App->fonts->BlitText(128, 207, HiscoreFont, "round");

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && gameOver == false)
	{
		
		spdy = 0;
		spdx = 0;

		MovablePiece = false;

	}

	if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	{
		score += 23;
	}
}