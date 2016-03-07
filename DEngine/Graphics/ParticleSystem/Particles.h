
#ifndef PARTICLES_H_
#define PARTICLES_H_

#include "Particle.h"
#include <vector>
#include <d3d11.h>
#include "Graphics\D3D11Renderer.h"
#include "Graphics\Render\VSGSPSPerFrameCBuffer.h"
#include "Graphics\Render\Texture.h"
#include "Random.h"

class Emitter
{
public:

	enum
	{
		TORCH_FLAME = 1,
		SMOKE = 2,
		ROCKET_TRAIL = 3,
		FIRE = 4,
	};

	Emitter();

	Emitter(int Type, Vector3& emitPos, Vector3& emitDir);

	~Emitter();

	float	GetAge();

	void	AddParticle(int type, Vector3& emitPos, Vector3& emitDir);

	void	Draw();

	void	SetEyePosW(const Vector3& eyePosW);

	void	SetEmitPosW(const Vector3& emitPosW);

	void	SetEmitDirW(const Vector3& emitDirW);

	void	SetMaxPartiples(const unsigned int max);

	void	Update(const float delta_time);

private:

	// Maximum number of particles
	unsigned						m_iMaxParticles;

	// Age of particles
	float							m_fAge;

	float							m_fFlareAge;

	float							m_fGameTime;

	float							m_fTimeStep;

	unsigned int					m_EffectType;

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

#endif // !PARTICLES_H_
