
#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include "Particle.h"
#include <vector>
#include <d3d11.h>
#include "Graphics\D3D11Renderer.h"
#include "Graphics\Render\VSGSPSPerFrameCBuffer.h"
#include "Graphics\Render\Texture.h"
#include "Random.h"

namespace DE
{

class ParticleSystem
{
public:
	ParticleSystem();

	~ParticleSystem();

	float	GetAge();

	void	Init();

	void	Draw();

	void	SetEyePosW(const Vector3& eyePosW);

	void	SetEmitPosW(const Vector3& emitPosW);

	void	SetEmitDirW(const Vector3& emitDirW);

	void	SetMaxPartiples(const unsigned int max);

	static ParticleSystem*	GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new ParticleSystem;
		return m_pInstance;
	}

	void	Update(const float delta_time);

private:
	// singleton instance
	static ParticleSystem*			m_pInstance;

	// Maximum number of particles
	unsigned						m_iMaxParticles;

	// Age of particles
	float							m_fAge;

	float							m_fFlareAge;

	float							m_fGameTime;

	float							m_fTimeStep;

	// Position of eye in the world space
	Vector3							m_vEyePosW;

	// Position of emitting in the world space
	Vector3							m_vEmitPosW;

	// Emitting direction in the world space
	Vector3							m_vEmitDirW;

	// Mesh for emitter
	MeshData*						m_InitMesh;

	// Mesh for all particles
	MeshData*						m_DrawMesh;

	MeshData*						m_StreamOutMesh;
	// 
	VSGSPSPerFrameCBuffer*			m_pVSGSPSCBuffer;

	bool							m_Flag;

	bool							m_FirstRun;

	RenderPass* emitterPass;

	RenderPass*	drawPass;
};

};

#endif // !PARTICLESYSTEM_H_

