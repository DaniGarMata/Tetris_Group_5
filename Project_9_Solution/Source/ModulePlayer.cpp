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
	MovablePiece = true;

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

	//laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	//explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 56;
	position.y = 37;

	gameOver = false;

	/*collider = App->collisions->AddCollider({ position.x, position.y, 16, 24 }, Collider::Type::PLAYER, this);*/

	// TODO 0: Notice how a font is loaded and the meaning of all its arguments 
	//char lookupTable[] = { "!  ,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	//scoreFont = App->fonts->Load("Assets/Fonts/rtype_font.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);

	// TODO 4: Try loading "rtype_font3.png" that has two rows to test if all calculations are correct
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789:R =+-&([]|/_)!" };
	scoreFont = App->fonts->Load("Assets/Fonts/FontRed.png", lookupTable, 1);
	HiscoreFont = App->fonts->Load("Assets/Fonts/FontBlue.png", lookupTable, 1);
	linesLeftFont = App->fonts->Load("Assets/Fonts/FontWhite.png", lookupTable, 1);

	linesLeft = 5;

	return ret;
}

Update_Status ModulePlayer::PreUpdate() {

	if (MovablePiece == false)
	{

		col = 5;

		row = 0;

		MovablePiece = true;

	}

	return Update_Status::UPDATE_CONTINUE;

}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll



		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
		{
			if (MovablePiece == true)
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

		if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
		{

			if (MovablePiece == true)
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

		if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
		{

			if (MovablePiece == true)
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
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 9; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}
				//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo habú} pensado en hacerlo con 4 de estos
				//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
				//@@ que haya 4 activos al mismo tiempo
			}

		}

		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
		{


		}

		if (App->input->keys[SDL_SCANCODE_H] == Key_State::KEY_DOWN)  //@@ para subir el bloque y poder testear cosas
		{

			if (row > 0)
			{
				//row = 0;

				row--;
				position.y = row * 8 + 40;  //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

			}

		}

		if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE) {

			if (MovablePiece == true) {

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
							MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
							score = score + 2;
						}

					}
					else
					{
						Map[row][col] = 9; //@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

					ctr = 120;

				}

			}

		}


	/*collider->SetPos(position.x, position.y);*/

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
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

void ModulePlayer::TetraminoCheck() {

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

								lines = lines + ((uint)1);

							}

						}

					}

				}

			}

		}

		Tctr = 0;

	}

	for (int i = 0; i < 10; i++) {

		if (Map[0][i] == 9) {

			gameOver = true;

		}

	}

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