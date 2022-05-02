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
	idleAnim.PushBack({ 0, 0, 8, 8 });

	
}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Tetramino.png");
	currentAnimation = &idleAnim;

	laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = col*8+32;  //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
	position.y = row*8+40;  //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);

	// TODO 0: Notice how a font is loaded and the meaning of all its arguments 
	//char lookupTable[] = { "!  ,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	//scoreFont = App->fonts->Load("Assets/Fonts/rtype_font.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);

	// TODO 4: Try loading "rtype_font3.png" that has two rows to test if all calculations are correct
	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = App->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	return ret;
}

Update_Status ModulePlayer::PreUpdate() {

	if (enabled == false)
	{

		col = 5;

		row = 0;

		enabled = true;

	}

	return Update_Status::UPDATE_CONTINUE;

}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll
	//App->player->position.x += 1;

	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_DOWN)
	{
	
		if (enabled == true)
		{


			if (col > 0)
			{
				//col = 0;
				if (Map[row][col - 1] != 9)  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					Map[row][col] = 0;
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					Map[row][col] = 1;
				}

			}

		}
		
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_DOWN)
	{
		//position.x += speed;

		if (enabled == true)
		{
			
			if (col < 9)
			{
				//col = 9;
				if (Map[row][col + 1] != 9)  //@@ comprueba si la siguiente casilla estEocupada por un 9 que serú} el equivalente a un bloque ocupado
				{
					Map[row][col] = 0;
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					Map[row][col] = 1;
				}
				
			}
			
		}
		
		
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_DOWN)  //@@ no pude hacer que bajase solo con el tiempo
	{
		if (enabled == true)
		{
			if (row < 19)
			{
				//row = 19;
				if (Map[row + 1][col] != 9)  //@@ comprueba si la siguiente casilla estEocupada por un 9 que serú} el equivalente a un bloque desactivado, aunque de momento no haya representación grafica
				{
					Map[row][col] = 0;
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					Map[row][col] = 1;
				}
				else
				{
					Map[row][col] = 9; //@@ deja la casilla como ocupada
					enabled = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				}
				
			}
			else
			{
				Map[row][col] = 9; //@@ deja la casilla como ocupada
				enabled = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo habú} pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}
		
		
		//if (currentAnimation != &downAnim)
		//{
		//	downAnim.Reset();
		//	currentAnimation = &downAnim;
		//}
	}

	if (App->input->keys[SDL_SCANCODE_H] == Key_State::KEY_DOWN)  //@@ para subir el bloque y poder testear cosas
	{
		
		if (row > 0)
		{
			//row = 0;
			
			row--;
			position.y = row * 8 + 40;  //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
			
		}
		
		//if (currentAnimation != &upAnim)
		//{
		//	upAnim.Reset();
		//	currentAnimation = &upAnim;
		//}
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		//Particle* newParticle = App->particles->AddParticle(App->particles->laser, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
		//newParticle->collider->AddListener(this);
		//App->audio->PlayFx(laserFx);
	}

	// If no up/down movement detected, set the current animation back to idle
	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE) {

		if (enabled == true) {

			if (ctr > 0) {

				ctr--;

			}

			else {

				if (row < 19)
				{
					//row = 19;
					if (Map[row + 1][col] != 9)  //@@ comprueba si la siguiente casilla estEocupada por un 9 que serú} el equivalente a un bloque desactivado, aunque de momento no haya representación grafica
					{
						Map[row][col] = 0;
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						Map[row][col] = 1;
					}
					else
					{
						Map[row][col] = 9; //@@ deja la casilla como ocupada
						enabled = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					}

				}
				else
				{
					Map[row][col] = 9; //@@ deja la casilla como ocupada
					enabled = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				}

				ctr = 120;

			}

		}

	}


	collider->SetPos(position.x, position.y);

	for (int i = 0; i < 20; i++) {

		for (int j = 0; j < 10; j++) {

			if (Map[i][j] == 9) {

				Tctr++;

				prov = i;

				if (Tctr == 10) {

					for (int k = 0; k < 10; k++) {

						Map[prov][k] = 0;

					}

					for (int l = 0; l < prov; l++) {

						for (int m = 0; m < 10; m++) {

							if (Map[l][m] == 9) {

								Map[l][m] = 0;
								Map[l + 1][m] = 9;

							}

						}

					}

				}

			}

		}

		Tctr = 0;

	}

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	for (int i = 0; i < 20; i++) {

		for (int j = 0; j < 10; j++) {

			if (Map[i][j] == 9) {

				SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;

				tetramino->w = 8;

				tetramino->x = 0;

				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}

		}

	}

	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%7d", score);

	// TODO 3: Blit the text of the score in at the bottom of the screen
	App->fonts->BlitText(58, 248, scoreFont, scoreText);

	App->fonts->BlitText(150, 248, scoreFont, "this is just a font test");

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, Collider::Type::NONE, 14);
		App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, Collider::Type::NONE, 40);
		App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, Collider::Type::NONE, 28);
		App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, Collider::Type::NONE, 21);

		App->audio->PlayFx(explosionFx);
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneIntro, 60);

		destroyed = true;
	}

	if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	{
		score += 23;
	}
}