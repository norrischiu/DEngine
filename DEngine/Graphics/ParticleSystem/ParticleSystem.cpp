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
	m_Particles[id] = emitter;
}

void ParticleSystem::SetVelocity(char* id, Vector3 & velocity)
{

	m_Velocity = velocity;
}

void ParticleSystem::Update(float delta_time)
{
	for (auto ptr : m_Particles)
	{
		ptr.second->Update(delta_time);
	}
}

void ParticleSystem::Render()
{
	for (auto ptr : m_Particles)
	{
		ptr.second->Draw();
	}
}

};
