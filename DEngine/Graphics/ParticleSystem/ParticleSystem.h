
#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include "Particles.h"
#include <unordered_map>

namespace DE
{

class ParticleSystem
{
public:

	enum
	{
		TORCH_FLAME = 1,
		SMOKE = 2,
		ROCKET_TRAIL = 3,
	};

	ParticleSystem();

	~ParticleSystem();

	Emitter* AddParticles(char* id, int type, Vector3& emitPos, Vector3& emitDir);

	void SetVelocity(char* id, Vector3& velocity);

	static ParticleSystem*	GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new ParticleSystem;
		return m_pInstance;
	}

	void Update(float delta_time);

	void Render();

private:
	Vector3						m_Velocity;
	static ParticleSystem*		m_pInstance;
	std::unordered_map<const char*, Emitter*> m_Particles;
};

};

#endif // !PARTICLESYSTEM_H_

