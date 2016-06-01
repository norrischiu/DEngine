#include "ParticleSystem.h"
#include "Memory\Handle.h"

namespace DE
{

ParticleSystem* ParticleSystem::m_pInstance;

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::AddComponent(char* id, Emitter * emitter)
{
	m_Particles.Add(id, emitter);
}

void ParticleSystem::SetVelocity(char* id, Vector3 & velocity)
{
	m_Velocity = velocity;
}

void ParticleSystem::Update(float delta_time)
{
	m_Particles.ForEachItem([delta_time](Emitter* item)
	{
		item->Update(delta_time);
	});
}

void ParticleSystem::Render()
{
	m_Particles.ForEachItem([](Emitter* item)
	{
		item->Draw();
	});
}

};
