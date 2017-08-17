
#ifndef EMITTER_H_
#define EMITTER_H_

#include "Particle.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\D3D12Renderer.h"
#include "Graphics\Render\VSGSPSPerFrameCBuffer.h"
#include "Graphics\Render\Texture.h"
#include "Random.h"
#include "Object\Component.h"


namespace DE
{
class Emitter : public Component
{
public:

	enum
	{
		YELLOW_LIGHT = 1,
		BLUE_LIGHT = 2,
		BLEEDING = 3,
		FIRE = 4,
		DISABLE = 5,
	};

	static const int ComponentID = ComponentID::PARTICLE_SYSTEM;

	Emitter();

	Emitter(char* id, int Type, float size, Vector3& emitPos, Vector3& emitDir);

	~Emitter();

	float	GetAge();

	void	Draw();

	void	SetEyePosW(const Vector3& eyePosW);

	void	SetEmitPosW(const Vector3& emitPosW);

	void	SetEmitDirW(const Vector3& emitDirW);

	void	SetMaxPartiples(const unsigned int max);

	void	Update(const float delta_time) override;

	void	SetOwner(GameObject* ptr) override;

	void	SetSize(float size);

	void	SetRandomRange(float min, float max);

	void	ResetDisableTime();

private:

	// Maximum number of particles
	unsigned						m_iMaxParticles;

	// Age of particles
	float							m_fAge;

	float							m_fFlareAge;

	float							m_fDisableTime;

	float							m_fTimeStep;
	// size of particles
	float							m_fSize;

	float							m_fRandMin;

	float							m_fRandMax;

	unsigned int					m_EffectType;

	// Position of eye in the world space
	Vector3							m_vEyePosW;

	// Position of emitting in the world space
	Vector3							m_vEmitPosW;

	// Emitting direction in the world space
	Vector3							m_vEmitDirW;

	// transform matrix
	Matrix4*						m_pTransform;

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
#endif // !EMITTER_H_
