#include "Tetramino.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"

Tetramino::Tetramino(int x, int y) : position(x, y)
{
	spawnPos = position;
}

Tetramino::~Tetramino()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

const Collider* Tetramino::GetCollider() const
{
	return collider;
}

void Tetramino::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
}

void Tetramino::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
}

void Tetramino::OnCollision(Collider* collider)
{
	App->particles->AddParticle(App->particles->explosion, position.x, position.y);
	App->audio->PlayFx(destroyedFx);

	SetToDelete();
}

void Tetramino::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;
}