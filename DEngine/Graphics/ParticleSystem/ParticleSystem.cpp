#include "ParticleSystem.h"

namespace DE
{
ParticleSystem* ParticleSystem::m_pInstance;

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

Emitter * ParticleSystem::AddParticles(char* id, int type, Vector3& emitPos, Vector3& emitDir)
{
	m_Velocity = emitDir;
	Vector3 partDir = Vector3(0.0f, 0.7f, 0.0f);
	if (type == 1)
	{
		partDir = Vector3(0.0f, 0.7f, 0.0f);
	}
	else if (type == 2)
	{
		partDir = Vector3(0.0f, 1.3f, 0.7f);
	}

	Emitter* particles = new Emitter(type, emitPos + m_Velocity, partDir);

	m_Particles[id] = particles;

	return particles;
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
