#include "ModuleParticles.h"

#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = App->textures->Load("Assets/Sprites/Fireworks.png");

	//// Fireworks particles
	fireworks1.anim.PushBack({ 0, 0, 48, 48 });
	fireworks1.anim.PushBack({ 48, 0, 48, 48 });
	fireworks1.anim.PushBack({ 96, 0, 48, 48 });
	fireworks1.anim.PushBack({ 144, 0, 48, 48 });
	fireworks1.anim.PushBack({ 192, 0, 48, 48 });
	fireworks1.anim.PushBack({ 0, 48, 48, 48 });
	fireworks1.anim.PushBack({ 48, 48, 48, 48 });
	fireworks1.anim.PushBack({ 96, 48, 48, 48 });
	fireworks1.anim.PushBack({ 144, 48, 48, 48 });
	fireworks1.anim.loop = false;
	fireworks1.anim.speed = 0.1f;

	fireworks2.anim.PushBack({ 192, 48, 48, 48 });
	fireworks2.anim.PushBack({ 0, 96, 48, 48 });
	fireworks2.anim.PushBack({ 48, 96, 48, 48 });
	fireworks2.anim.PushBack({ 96, 96, 48, 48 });
	fireworks2.anim.PushBack({ 144, 96, 48, 48 });
	fireworks2.anim.PushBack({ 192, 96, 48, 48 });
	fireworks2.anim.PushBack({ 0, 144, 48, 48 });
	fireworks2.anim.loop = false;
	fireworks2.anim.speed = 0.1f;

	fireworks3.anim.PushBack({ 48, 144, 48, 48 });
	fireworks3.anim.PushBack({ 96, 144, 48, 48 });
	fireworks3.anim.PushBack({ 144, 144, 48, 48 });
	fireworks3.anim.PushBack({ 192, 144, 48, 48 });
	fireworks3.anim.PushBack({ 48, 192, 48, 48 });
	fireworks3.anim.PushBack({ 96, 192, 48, 48 });
	fireworks3.anim.PushBack({ 144, 192, 48, 48 });
	fireworks3.anim.loop = false;
	fireworks3.anim.speed = 0.1f;

	return true;
}

Update_Status ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			particles[i]->pendingToDelete = true;
			particles[i]->collider->pendingToDelete = true;
			break;
		}
	}
}

Update_Status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			particles[i]->SetToDelete();
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			App->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new Particle(particle);
			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			newParticle->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			newParticle->position.y = y;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE)
				newParticle->collider = App->collisions->AddCollider(newParticle->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}