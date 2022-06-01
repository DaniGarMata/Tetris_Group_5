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

	T11.PushBack({ 96, 88, 16, 24 });
	T12.PushBack({ 96, 72, 24, 16 });
	T13.PushBack({ 80, 88, 16, 24 });
	T10.PushBack({ 96, 56, 24, 16 });


	T21.PushBack({ 32, 112, 16, 24 });
	T22.PushBack({ 96, 128, 24, 16 });
	T23.PushBack({ 72, 112, 16, 24 });
	T20.PushBack({ 96, 112, 24, 16 });

	T30.PushBack({ 0, 120, 24, 16 });
	T31.PushBack({ 0, 80, 16, 24 });

	T40.PushBack({ 25, 56, 24, 16 });
	T41.PushBack({ 48, 56, 16, 24 });
	T42.PushBack({ 0, 56, 24, 16 });
	T43.PushBack({ 64, 56, 16, 24 });

	T50.PushBack({ 32, 80, 24, 16 });
	T51.PushBack({ 56, 80, 16, 24 });

	T60.PushBack({ 0, 72, 32, 8 });
	T61.PushBack({ 72, 80, 8, 32 });

	T70.PushBack({ 80, 56, 16, 16 });
	

	MovablePiece = true;
	nextLevel = false;

}


ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Tetriminosheet.png");
	currentAnimation = &idleAnim;

	//laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	//explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 72;
	position.y = 40;

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

	

	


	return ret;
}

Update_Status ModulePlayer::PreUpdate() {

	if (MovablePiece == false)
	{

		col = 5;

		row = 0;

		MovablePiece = true;

		PieceType = next;
		next = (rand() % 7) + 1;
		PieceState = 0;

		

		position.x = col * 8 + 32;
		position.y = row * 8 + 40;

	}

	return Update_Status::UPDATE_CONTINUE;

}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll
	

	if (PieceType == 0)  //OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	{
		currentAnimation = &idleAnim;
		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
		{
			if (MovablePiece == true)
			{


				if (col > 0)
				{
					//col = 0;
					if (Map[row][col - 1] == 0)  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
					{
						
						col--;
						position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
						
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
					if (Map[row][col + 1] == 0)  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
					{
						
						col++;
						position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
						
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
					if (Map[row + 1][col] == 0)  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
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
				//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
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
						if (Map[row + 1][col] == 0)  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
						{
							
							row++;
							position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
							
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
	}
	else if (PieceType == 1 && PieceState == 1)
	{
		
		currentAnimation = &T11;
		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
		{
			if (MovablePiece == true)
			{


				if (col > 0)
				{
					//col = 0;
					if ((Map[row][col -1 ] == 0)&& (Map[row+1][col - 1] == 0) && (Map[row + 2][col - 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
					{
						
						col--;
						position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
						
					}

				}

			}

		}

		if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
		{

			if (MovablePiece == true)
			{

				if (col < 8)
				{
					//col = 9;
					if ((Map[row][col + 1] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
					{
						
						col++;
						position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
						
					}

				}

			}

		}

		if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
		{

			if (MovablePiece == true)
			{
				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 3][col] == 0) && (Map[row + 3][col +1 ] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 95;
						Map[row+1][col] = 96;
						Map[row+2][col] = 104;
						Map[row+2][col+1] = 94;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 95;
					Map[row + 1][col] = 96;
					Map[row + 2][col] = 104;
					Map[row + 2][col + 1] = 94; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}
				//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
				//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
				//@@ que haya 4 activos al mismo tiempo
			}

		}

		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
		{
			PieceState++;

			if (PieceState > 3)
			{
				PieceState = 0;
			}

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

					if (row < 17)
					{
						//row = 19;
						if ((Map[row + 3][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
						{
							
							row++;
							position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
							
						}
						else
						{
							Map[row][col] = 95;
							Map[row + 1][col] = 96;
							Map[row + 2][col] = 104;
							Map[row + 2][col + 1] = 94;//@@ deja la casilla como ocupada
							MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
							score = score + 2;
						}

					}
					else
					{
						Map[row][col] = 95;
						Map[row + 1][col] = 96;
						Map[row + 2][col] = 104;
						Map[row + 2][col + 1] = 94; //@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

					ctr = TIME_CTR;

				}

			}

		}

		
	}
	else if (PieceType == 1 && PieceState == 2)
	{

	currentAnimation = &T12;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row+1][col - 1] == 0) && (Map[row][col + 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row][col + 3] == 0) && (Map[row + 1][col + 3] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row+1][col] = 92;
					Map[row + 1][col+1] = 93;
					Map[row + 1][col+2] = 103;
					Map[row ][col + 2] = 95;  //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row + 1][col] = 92;
				Map[row + 1][col + 1] = 93;
				Map[row + 1][col + 2] = 103;
				Map[row][col + 2] = 95; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row + 1][col] = 92;
						Map[row + 1][col + 1] = 93;
						Map[row + 1][col + 2] = 103;
						Map[row][col + 2] = 95;  //@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row + 1][col] = 92;
					Map[row + 1][col + 1] = 93;
					Map[row + 1][col + 2] = 103;
					Map[row][col + 2] = 95; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 1 && PieceState == 3)
	{

	currentAnimation = &T13;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col - 1] == 0)  && (Map[row + 1][col] == 0) && (Map[row + 2][col] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 2] == 0) && (Map[row + 1][col + 2] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 17)
			{
				//row = 19;
				if ((Map[row + 1][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row][col] = 92;
					Map[row ][col+1] = 105;
					Map[row + 1][col+1] = 96;
					Map[row + 2][col + 1] = 97;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 92;
				Map[row][col + 1] = 105;
				Map[row + 1][col + 1] = 96;
				Map[row + 2][col + 1] = 97; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 1][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 92;
						Map[row][col + 1] = 105;
						Map[row + 1][col + 1] = 96;
						Map[row + 2][col + 1] = 97;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 92;
					Map[row][col + 1] = 105;
					Map[row + 1][col + 1] = 96;
					Map[row + 2][col + 1] = 97; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 1 && PieceState == 0)
	{

	currentAnimation = &T10;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col - 1] == 0) && (Map[row + 1][col-1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row][col + 3] == 0) && (Map[row + 1][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row][col] = 102;
					Map[row][col + 1] = 93;
					Map[row ][col + 2] = 94;
					Map[row + 1][col] = 97;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 102;
				Map[row][col + 1] = 93;
				Map[row][col + 2] = 94;
				Map[row + 1][col] = 97; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 102;
						Map[row][col + 1] = 93;
						Map[row][col + 2] = 94;
						Map[row + 1][col] = 97;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 102;
					Map[row][col + 1] = 93;
					Map[row][col + 2] = 94;
					Map[row + 1][col] = 97; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 2 && PieceState == 1)
	{

	currentAnimation = &T21;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col -1] == 0) && (Map[row + 1][col -1] == 0) && (Map[row + 2][col-1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 2] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 17)
			{
				//row = 19;
				if ((Map[row + 3][col] == 0) && (Map[row+1][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row][col] = 42;
					Map[row + 1][col] = 36;
					Map[row + 2][col] = 37;
					Map[row][col + 1] = 34;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 42;
				Map[row + 1][col] = 36;
				Map[row + 2][col] = 37;
				Map[row][col + 1] = 34; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 3][col] == 0) && (Map[row + 1][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 42;
						Map[row + 1][col] = 36;
						Map[row + 2][col] = 37;
						Map[row][col + 1] = 34;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 42;
					Map[row + 1][col] = 36;
					Map[row + 2][col] = 37;
					Map[row][col + 1] = 34; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 2 && PieceState == 2)
	{

	currentAnimation = &T22;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row + 1][col - 1] == 0) && (Map[row][col -1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row][col + 1] == 0) && (Map[row + 1][col + 3] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row + 1][col] = 44;
					Map[row + 1][col + 1] = 33;
					Map[row + 1][col + 2] = 34;
					Map[row][col] = 35;  //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row + 1][col] = 44;
				Map[row + 1][col + 1] = 33;
				Map[row + 1][col + 2] = 34;
				Map[row][col] = 35; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row + 1][col] = 44;
						Map[row + 1][col + 1] = 33;
						Map[row + 1][col + 2] = 34;
						Map[row][col] = 35;  //@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row + 1][col] = 44;
					Map[row + 1][col + 1] = 33;
					Map[row + 1][col + 2] = 34;
					Map[row][col] = 35; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 2 && PieceState == 3)
	{

	currentAnimation = &T23;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col ] == 0) && (Map[row + 1][col] == 0) && (Map[row + 2][col-1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 2] == 0) && (Map[row + 1][col + 2] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 17)
			{
				//row = 19;
				if ((Map[row + 3][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row][col+1] = 35;
					Map[row+1][col + 1] = 36;
					Map[row + 2][col ] = 32;
					Map[row + 2][col + 1] = 43;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col + 1] = 35;
				Map[row + 1][col + 1] = 36;
				Map[row + 2][col] = 32;
				Map[row + 2][col + 1] = 43; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 3][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col + 1] = 35;
						Map[row + 1][col + 1] = 36;
						Map[row + 2][col] = 32;
						Map[row + 2][col + 1] = 43;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col + 1] = 35;
					Map[row + 1][col + 1] = 36;
					Map[row + 2][col] = 32;
					Map[row + 2][col + 1] = 43; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 2 && PieceState == 0)
	{

	currentAnimation = &T20;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col - 1] == 0) && (Map[row + 1][col + 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row][col + 3] == 0) && (Map[row + 1][col + 3] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 1][col] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row][col] = 32;
					Map[row][col + 1] = 33;
					Map[row][col + 2] = 45;
					Map[row + 1][col+2] = 37;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 32;
				Map[row][col + 1] = 33;
				Map[row][col + 2] = 45;
				Map[row + 1][col + 2] = 37; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 1][col] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 32;
						Map[row][col + 1] = 33;
						Map[row][col + 2] = 45;
						Map[row + 1][col + 2] = 37;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 32;
					Map[row][col + 1] = 33;
					Map[row][col + 2] = 45;
					Map[row + 1][col + 2] = 37; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 3 && PieceState == 0)
	{

	currentAnimation = &T30;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col ] == 0) && (Map[row + 1][col - 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row][col + 3] == 0) && (Map[row + 1][col + 2] == 0) )  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row+1][col] = 62;
					Map[row ][col+1] = 72;
					Map[row + 1][col+1] = 73;
					Map[row ][col + 2] = 64;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row + 1][col] = 62;
				Map[row][col + 1] = 72;
				Map[row + 1][col + 1] = 73;
				Map[row][col + 2] = 64; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 1)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row + 1][col] = 62;
						Map[row][col + 1] = 72;
						Map[row + 1][col + 1] = 73;
						Map[row][col + 2] = 64;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row + 1][col] = 62;
					Map[row][col + 1] = 72;
					Map[row + 1][col + 1] = 73;
					Map[row][col + 2] = 64; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 3 && PieceState == 1)
	{

	currentAnimation = &T31;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col-1] == 0) && (Map[row + 1][col - 1] == 0) && (Map[row + 2][col] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 1] == 0) && (Map[row + 1][col + 2] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 17)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 3][col + 1] == 0) )  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row ][col] = 65;
					Map[row+1][col ] = 74;
					Map[row + 1][col + 1] = 75;
					Map[row+2][col + 1] = 67;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 65;
				Map[row + 1][col] = 74;
				Map[row + 1][col + 1] = 75;
				Map[row + 2][col + 1] = 67; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 1)
		{
			PieceState = 0;
		}

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

				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 65;
						Map[row + 1][col] = 74;
						Map[row + 1][col + 1] = 75;
						Map[row + 2][col + 1] = 67;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 65;
					Map[row + 1][col] = 74;
					Map[row + 1][col + 1] = 75;
					Map[row + 2][col + 1] = 67; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 4 && PieceState == 0)
	{

	currentAnimation = &T40;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col - 1] == 0) && (Map[row + 1][col] == 0) )  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{
					
					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row][col + 3] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 1][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row][col] = 47;
					Map[row ][col+1] = 55;
					Map[row][col + 2] = 49;
					Map[row + 1][col + 1] = 52;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 47;
				Map[row][col + 1] = 55;
				Map[row][col + 2] = 49;
				Map[row + 1][col + 1] = 52; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 1][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 47;
						Map[row][col + 1] = 55;
						Map[row][col + 2] = 49;
						Map[row + 1][col + 1] = 52;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 47;
					Map[row][col + 1] = 55;
					Map[row][col + 2] = 49;
					Map[row + 1][col + 1] = 52; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 4 && PieceState == 1)
	{

	currentAnimation = &T41;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col - 1] == 0) && (Map[row + 1][col-1] == 0) && (Map[row + 2][col - 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 1] == 0) && (Map[row + 1][col + 2] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 17)
			{
				//row = 19;
				if ((Map[row + 3][col] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row][col] = 50;
					Map[row + 1][col] = 54;
					Map[row + 2][col] = 52;
					Map[row + 1][col + 1] = 49;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 50;
				Map[row + 1][col] = 54;
				Map[row + 2][col] = 52;
				Map[row + 1][col + 1] = 49; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 3][col] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row][col] = 50;
						Map[row + 1][col] = 54;
						Map[row + 2][col] = 52;
						Map[row + 1][col + 1] = 49;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 50;
					Map[row + 1][col] = 54;
					Map[row + 2][col] = 52;
					Map[row + 1][col + 1] = 49; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 4 && PieceState == 2)
	{

	currentAnimation = &T42;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row+1][col - 1] == 0) && (Map[row ][col] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row][col + 2] == 0) && (Map[row + 1][col + 3] == 0) )  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{
					
					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego
					
				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row+1][col] = 47;
					Map[row + 1][col+1] = 53;
					Map[row + 1][col+2] = 49;
					Map[row ][col +1] = 50;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row + 1][col] = 47;
				Map[row + 1][col + 1] = 53;
				Map[row + 1][col + 2] = 49;
				Map[row][col + 1] = 50;//@@ //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row + 1][col] = 47;
						Map[row + 1][col + 1] = 53;
						Map[row + 1][col + 2] = 49;
						Map[row][col + 1] = 50;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row + 1][col] = 47;
					Map[row + 1][col + 1] = 53;
					Map[row + 1][col + 2] = 49;
					Map[row][col + 1] = 50;//@@ //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 4 && PieceState == 3)
	{

	currentAnimation = &T43;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row + 1][col - 1] == 0) && (Map[row][col] == 0) && (Map[row+2][col] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 2] == 0) && (Map[row +1][col + 2] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 17)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{

					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

				}
				else
				{
					Map[row + 1][col] = 47;
					Map[row + 1][col + 1] = 56;
					Map[row + 2][col + 1] = 52;
					Map[row][col + 1] = 50;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row + 1][col] = 47;
				Map[row + 1][col + 1] = 56;
				Map[row + 2][col + 1] = 52;
				Map[row][col + 1] = 50;//@@ //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 3)
		{
			PieceState = 0;
		}

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

				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{

						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

					}
					else
					{
						Map[row + 1][col] = 47;
						Map[row + 1][col + 1] = 56;
						Map[row + 2][col + 1] = 52;
						Map[row][col + 1] = 50;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row + 1][col] = 47;
					Map[row + 1][col + 1] = 56;
					Map[row + 2][col + 1] = 52;
					Map[row][col + 1] = 50;//@@ //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 5 && PieceState == 0)
	{

	currentAnimation = &T50;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col-1] == 0) && (Map[row + 1][col] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 7)
			{
				//col = 9;
				if ((Map[row+1][col + 3] == 0) && (Map[row ][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 1][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{

					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

				}
				else
				{
					Map[row ][col] = 17;
					Map[row][col + 1] = 30;
					Map[row + 1][col + 1] = 29;
					Map[row + 1][col + 2] = 19;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 17;
				Map[row][col + 1] = 30;
				Map[row + 1][col + 1] = 29;
				Map[row + 1][col + 2] = 19; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 1)
		{
			PieceState = 0;
		}

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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 1][col] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 2][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{

						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

					}
					else
					{
						Map[row][col] = 17;
						Map[row][col + 1] = 30;
						Map[row + 1][col + 1] = 29;
						Map[row + 1][col + 2] = 19;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 17;
					Map[row][col + 1] = 30;
					Map[row + 1][col + 1] = 29;
					Map[row + 1][col + 2] = 19; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 5 && PieceState == 1)
	{

	currentAnimation = &T51;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col] == 0) && (Map[row + 1][col - 1] == 0) && (Map[row + 2][col - 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 2] == 0) && (Map[row + 1][col + 2] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 17)
			{
				//row = 19;
				if ((Map[row + 3][col] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{

					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

				}
				else
				{
					Map[row + 1][col] = 27;
					Map[row][col + 1] = 20;
					Map[row + 1][col + 1] = 28;
					Map[row+2][col] = 22;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row + 1][col] = 27;
				Map[row][col + 1] = 20;
				Map[row + 1][col + 1] = 28;
				Map[row + 2][col] = 22; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 1)
		{
			PieceState = 0;
		}

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

				if (row < 17)
				{
					//row = 19;
					if ((Map[row + 3][col] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{

						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

					}
					else
					{
						Map[row + 1][col] = 27;
						Map[row][col + 1] = 20;
						Map[row + 1][col + 1] = 28;
						Map[row + 2][col] = 22;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row + 1][col] = 27;
					Map[row][col + 1] = 20;
					Map[row + 1][col + 1] = 28;
					Map[row + 2][col] = 22; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 6 && PieceState == 0)
	{

	currentAnimation = &T60;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col-1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 6)
			{
				//col = 9;
				if ((Map[row][col + 4] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

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
				if ((Map[row +1][col] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 1][col + 2] == 0) && (Map[row + 1][col + 3] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{

					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

				}
				else
				{
					Map[row][col] = 2;
					Map[row][col + 1] = 3;
					Map[row][col + 2] = 3;
					Map[row][col+3] = 4;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 2;
				Map[row][col + 1] = 3;
				Map[row][col + 2] = 3;
				Map[row][col + 3] = 4; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 1)
		{
			PieceState = 0;
		}

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
					if ((Map[row + 1][col] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 1][col + 2] == 0) && (Map[row + 1][col + 3] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{

						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

					}
					else
					{
						Map[row][col] = 2;
						Map[row][col + 1] = 3;
						Map[row][col + 2] = 3;
						Map[row][col + 3] = 4;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 2;
					Map[row][col + 1] = 3;
					Map[row][col + 2] = 3;
					Map[row][col + 3] = 4; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 6 && PieceState == 1)
	{

	currentAnimation = &T61;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row][col - 1] == 0) && (Map[row+1][col - 1] == 0) && (Map[row + 2][col - 1] == 0) && (Map[row + 3][col - 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

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
				if ((Map[row][col + 1] == 0) && (Map[row + 1][col + 1] == 0) && (Map[row + 2][col + 1] == 0) && (Map[row + 3][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 16)
			{
				//row = 19;
				if ((Map[row + 4][col] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{

					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

				}
				else
				{
					Map[row][col] = 5;
					Map[row + 1][col] = 6;
					Map[row + 2][col]  = 6;
					Map[row + 3][col ] = 7;//@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row][col] = 5;
				Map[row + 1][col] = 6;
				Map[row + 2][col] = 6;
				Map[row + 3][col] = 7; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
			//@@ bloques que funcionasen al mismo tiempoo y la prueba de si se pueden mover o no estubiese unificada para los 4 de alguna forma pero el primer problema es conseguir
			//@@ que haya 4 activos al mismo tiempo
		}

	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		PieceState++;

		if (PieceState > 1)
		{
			PieceState = 0;
		}

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

				if (row < 16)
				{
					//row = 19;
					if ((Map[row + 4][col] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{

						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego

					}
					else
					{
						Map[row][col] = 5;
						Map[row + 1][col] = 6;
						Map[row + 2][col] = 6;
						Map[row + 3][col] = 7;//@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row][col] = 5;
					Map[row + 1][col] = 6;
					Map[row + 2][col] = 6;
					Map[row + 3][col] = 7; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}
	else if (PieceType == 7 && PieceState == 0)
	{

	currentAnimation = &T70;
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (MovablePiece == true)
		{


			if (col > 0)
			{
				//col = 0;
				if ((Map[row ][col - 1] == 0) && (Map[row+1][col - 1] == 0))  //@@ comprueba si la siguiente casilla esta ocupada por un 9 que seria el equivalente a un bloque ocupado
				{

					col--;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{

			if (col < 8)
			{
				//col = 9;
				if ((Map[row][col + 2] == 0) && (Map[row + 1][col + 2] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque ocupado
				{

					col++;
					position.x = col * 8 + 32; //@@ col*8 los pixeles por columna del campo de juego, +32 para que se posicione correctamente en el campo de juego

				}

			}

		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{

		if (MovablePiece == true)
		{
			if (row < 18)
			{
				//row = 19;
				if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
				{
					
					row++;
					position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
					
				}
				else
				{
					Map[row + 1][col] = 89;
					Map[row + 1][col + 1] = 88;
					Map[row][col] = 87;
					Map[row][col + 1] = 90;  //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

			}
			else
			{
				Map[row + 1][col] = 89;
				Map[row + 1][col + 1] = 88;
				Map[row][col] = 87;
				Map[row][col + 1] = 90; //@@ deja la casilla como ocupada
				MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
				score = score + 2;
			}
			//@@ comprobar las posibilidades de movimiento en la matriz permite no hacer uso de colliders, para hacer un tetromino completo hab僘 pensado en hacerlo con 4 de estos
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

				if (row < 18)
				{
					//row = 19;
					if ((Map[row + 2][col] == 0) && (Map[row + 2][col + 1] == 0))  //@@ comprueba si la siguiente casilla est・ocupada por un 9 que ser僘 el equivalente a un bloque desactivado, aunque de momento no haya representaci grafica
					{
						
						row++;
						position.y = row * 8 + 40; //@@ row*8 los pixeles por fila del campo de juego, +40 para que se posicione correctamente en el campo de juego
						
					}
					else
					{
						Map[row + 1][col] = 89;
						Map[row + 1][col + 1] = 88;
						Map[row][col] = 87;
						Map[row][col + 1] = 90;  //@@ deja la casilla como ocupada
						MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
						score = score + 2;
					}

				}
				else
				{
					Map[row + 1][col] = 89;
					Map[row + 1][col + 1] = 88;
					Map[row][col] = 87;
					Map[row][col + 1] = 90; //@@ deja la casilla como ocupada
					MovablePiece = false; //@@ para ponerlo a false y que deje de poder moverse, solo si intenta bajar y no puede
					score = score + 2;
				}

				ctr = TIME_CTR;

			}

		}

	}


	}


	if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN)  //@@ para subir el bloque y poder testear cosas
	{

	

			PieceType = 0;
			

		

	}
	if (App->input->keys[SDL_SCANCODE_R] == Key_State::KEY_DOWN)  //@@ para subir el bloque y poder testear cosas
	{



		PieceType = (rand() % 7) + 1;
		PieceState = 0;

		LineCompleted = false;


	}

	

		


	/*collider->SetPos(position.x, position.y);*/

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
	SDL_Rect* next_piece = new SDL_Rect;
	if (next == 1)
	{
		//SDL_Rect* next_piece = new SDL_Rect;

		next_piece->h = 16;
		next_piece->w = 24;
		next_piece->x = 96;
		next_piece->y = 56;

		App->render->Blit(texture, (8), (24), next_piece);
	}
	else if (next == 2)
	{
		//SDL_Rect* next_piece = new SDL_Rect;

		next_piece->h = 16;
		next_piece->w = 24;
		next_piece->x = 96;
		next_piece->y = 112;

		App->render->Blit(texture, (8), (24), next_piece);
	}
	else if (next == 3)
	{
		//SDL_Rect* next_piece = new SDL_Rect;

		next_piece->h = 16;
		next_piece->w = 24;
		next_piece->x = 0;
		next_piece->y = 120;

		App->render->Blit(texture, (8), (24), next_piece);
	}
	else if (next == 4)
	{
		//SDL_Rect* next_piece = new SDL_Rect;

		next_piece->h = 16;
		next_piece->w = 24;
		next_piece->x = 25;
		next_piece->y = 56;

		App->render->Blit(texture, (8), (24), next_piece);
	}
	else if (next == 5)
	{
		//SDL_Rect* next_piece = new SDL_Rect;

		next_piece->h = 16;
		next_piece->w = 24;
		next_piece->x = 32;
		next_piece->y = 80;

		App->render->Blit(texture, (8), (24), next_piece);
	}
	else if (next == 6)
	{
		//SDL_Rect* next_piece = new SDL_Rect;

		next_piece->h = 8;
		next_piece->w = 32;
		next_piece->x = 0;
		next_piece->y = 72;

		App->render->Blit(texture, (8), (24), next_piece);
	}
	else if (next == 7)
	{
		//SDL_Rect* next_piece = new SDL_Rect;

		next_piece->h = 16;
		next_piece->w = 16;
		next_piece->x = 80;
		next_piece->y = 56;

		App->render->Blit(texture, (8), (24), next_piece);
	}
	
	SDL_Rect* tetramino = new SDL_Rect;

	for (int i = 0; i < 20; i++) {

		for (int j = 0; j < 10; j++) {

			if (Map[i][j] == 9) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;

				tetramino->w = 8;

				tetramino->x = 0;

				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 95) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 32;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 96) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 40;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 104) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 104;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 94) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 24;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 92) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 8;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 93) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 16;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 103) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 96;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 105) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 112;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 97) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 48;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 102) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 88;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 34) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 24;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 36) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 40;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 37) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 48;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 42) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 88;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 35) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 32;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 33) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 16;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 44) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 104;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 32) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 8;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 43) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 96;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 45) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 112;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 62) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 8;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 64) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 24;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 72) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 88;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 73) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 96;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 65) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 32;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 67) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 48;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 74) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 104;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 75) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 112;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 47) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 8;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 55) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 72;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 52) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 48;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 49) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 24;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 56) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 80;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 52) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 48;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 54) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 64;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 50) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 32;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 56) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 80;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 53) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 56;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 17) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 8;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 19) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 24;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 29) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 104;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 30) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 112;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 20) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 32;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 22) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 48;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 28) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 96;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 27) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 88;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 5) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 32;
				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 6) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 40;
				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 7) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 48;
				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 4) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 24;
				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 3) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 16;
				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 2) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 8;
				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 90) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 112;
				tetramino->y = 40;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 89) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 104;
				tetramino->y = 40;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 88) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 96;
				tetramino->y = 40;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 87) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 88;
				tetramino->y = 40;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 200) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 0;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 201) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 8;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 202) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 16;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 203) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 24;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 204) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 32;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 205) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 40;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 206) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 48;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 207) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 56;

				App->render->Blit(texture, (j * 8 + 32), (i * 8 + 40), tetramino);

			}
			if (Map[i][j] == 208) {

				//SDL_Rect* tetramino = new SDL_Rect;

				tetramino->h = 8;
				tetramino->w = 8;
				tetramino->x = 120;
				tetramino->y = 64;

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

	delete next_piece;
	delete tetramino;

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::TetraminoCheck() {

	for (int i = 0; i < 20; i++) {

		for (int j = 0; j < 10; j++) {

			if (Map[i][j] != 0) {

				Tctr++;

				prov = i;

				//if (Tctr == 10 && LineCompleted == true) {
				//
				//	int time2 = SDL_GetTicks();
				//	float seconds = (time2 - time1) / 1000.0f;
				//
				//	if (seconds > 1.03f)
				//	{
				//		int RandomColor = (rand() % 9) + 200;
				//
				//		for (int k = 0; k < 10; k++) {
				//
				//			Map[prov][k] = RandomColor;
				//
				//		}
				//	}
				//
				//	////for (int k = 0; k < 10; k++) {
				//	////
				//	////		Map[prov][k] = 0;
				//	////
				//	////}
				//	////
				//	////prov--;
				//	////
				//	////for (prov; prov >= 0; prov--) {
				//	////
				//	////	for (int k = 0; k < 10; k++) {
				//	////
				//	////		if (Map[prov][k] != 0) {
				//	////
				//	////			int get_piece = Map[prov][k];
				//	////
				//	////			if (get_piece == 105 && prov> i-2)
				//	////			{
				//	////				get_piece = 94;
				//	////			}
				//	////			else if (get_piece == 96 && prov > i - 2)
				//	////			{
				//	////				get_piece = 97;
				//	////			}
				//	////
				//	////			Map[prov][k] = 0;
				//	////			Map[prov + 1][k] = get_piece;
				//	////
				//	////		}
				//	////
				//	////	}
				//	////
				//	////
				//	////}
				//
				//	////lineIsMade = true;
				//	//for (int l = prov; l <= 0; l--) {
				//
				//	//	for (int m = 0; m < 10; m++) {
				//
				//	//		if (Map[(l - 1)][m] == 9) {
				//
				//	//			Map[l][m] = 0;
				//	//			Map[l][m] = 9;
				//
				//	//			lineIsMade = true;
				//
				//	//		}
				//
				//	//	}
				//
				//	//}
				//	Tctr = 0;
				//}
				if (Tctr == 10 && LineCompleted == false) { //**************+++++++++++++ this is the one that places the blocks

					int RandomColor = (rand() % 9) + 200;
					time1 = SDL_GetTicks();
					ctr += 12;

					for (int k = 0; k < 10; k++) {

						Map[prov][k] = RandomColor;

					}

					prov--;
					LineCompleted = true;
					for (prov; prov >= 0; prov--) {

						for (int k = 0; k < 10; k++) {

							//if (Map[prov][k] != 0) {
							//
							//	int get_piece = Map[prov][k];
							//
							//	if (get_piece == 105 && prov > i - 2)
							//	{
							//		get_piece = 94;
							//	}
							//	else if (get_piece == 96 && prov > i - 2)
							//	{
							//		get_piece = 97;
							//	}
							//
							//	Map[prov][k] = 0;
							//	Map[prov + 1][k] = get_piece;
							//
							//}

						}


					}

					//lineIsMade = true;
					//for (int l = prov; l <= 0; l--) {

					//	for (int m = 0; m < 10; m++) {

					//		if (Map[(l - 1)][m] == 9) {

					//			Map[l][m] = 0;
					//			Map[l][m] = 9;

					//			lineIsMade = true;

					//		}

					//	}

					//}
					Tctr = 0;
				}

			}

			if (Map[i][j] > 190)
			{
				Tctr++;

				prov = i;

				if (Tctr == 10 && LineCompleted == true) {

					int time2 = SDL_GetTicks();
					float seconds = (time2 - time1) / 1000.0f;

					if (seconds > 0.03f && seconds < 0.04f)
					{
						int RandomColor = (rand() % 9) + 200;

						for (int k = 0; k < 10; k++) {

							Map[prov][k] = RandomColor;

						}
						Tctr = 0;
					}
					else if (seconds > 0.06f && seconds < 0.07f)
					{
						int RandomColor = (rand() % 9) + 200;

						for (int k = 0; k < 10; k++) {

							Map[prov][k] = RandomColor;

						}
						Tctr = 0;
					}
					else if (seconds > 0.09f && seconds < 0.1f)
					{
						int RandomColor = (rand() % 9) + 200;

						for (int k = 0; k < 10; k++) {

							Map[prov][k] = RandomColor;

						}
						Tctr = 0;
					}
					else if (seconds > 0.12f && seconds < 0.13f)
					{
						int RandomColor = (rand() % 9) + 200;

						for (int k = 0; k < 10; k++) {

							Map[prov][k] = RandomColor;

						}
						Tctr = 0;
					}
					else if (seconds > 0.15f && seconds < 0.16f)
					{
						int RandomColor = (rand() % 9) + 200;

						for (int k = 0; k < 10; k++) {

							Map[prov][k] = RandomColor;

						}
						Tctr = 0;
					}
					else if (seconds > 0.18f && seconds < 0.19f)
					{
						int RandomColor = (rand() % 9) + 200;

						for (int k = 0; k < 10; k++) {

							Map[prov][k] = RandomColor;

						}
						Tctr = 0;
					}
					else if (seconds > 0.21f && seconds < 0.22f)
					{
						
						for (int k = 0; k < 10; k++) {

							Map[prov][k] = 0;

						}

						prov--;

						for (prov; prov >= 0; prov--) {

							for (int k = 0; k < 10; k++) {

								if (Map[prov][k] != 0) {

									int get_piece = Map[prov][k];

									if (get_piece == 105 && prov > i - 2)
									{
										get_piece = 94;
									}
									else if (get_piece == 96 && prov > i - 2)
									{
										get_piece = 97;
									}

									Map[prov][k] = 0;
									Map[prov + 1][k] = get_piece;

								}

							}


						}

						lineIsMade = true;
						LineCompleted = false;


					}
				}
			}

		}
		
		if (lineIsMade == true) {

			lines = lines++;
			linesLeft = linesLeft--;
			score = score + (10 * 2);

			lineIsMade = false;

			if (linesLeft == 0) {

				nextLevel = true;

			}
			
		}

		Tctr = 0;

	}

	for (int i = 0; i < 10; i++) {

		if (Map[0][i] != 0) {

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