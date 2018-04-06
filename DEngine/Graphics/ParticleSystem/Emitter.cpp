#include "Emitter.h"
#include "Graphics\MeshData.h"
#include "ParticleSystem.h"

#include <Windows.h>

#define PT_EMITTER 0
#define PT_FLARE 1

namespace DE
{
Emitter::Emitter() : Component()
	, m_pTransform(new Matrix4())
{
	m_ID = ComponentID;
	*m_pTransform = Matrix4::Identity;
	m_pVSGSPSCBuffer = new VSGSPSPerFrameCBuffer;
}

Emitter::Emitter(char* id, int type, float size, Vector3& emitPos, Vector3& emitDir) : Component()
	, m_pTransform(new Matrix4())
{
	*m_pTransform = Matrix4::Identity;
	m_ID = ComponentID;
	m_fAge = 0.0f;
	m_fFlareAge = 0.0f;
	m_fTimeStep = 0.0f;
	m_iMaxParticles = 20;
	m_vEyePosW = Vector3(0.0f, 0.0f, 0.0f);
	m_EffectType = type;
	m_vEmitPosW = emitPos;
	m_vEmitDirW = emitDir;
	m_pVSGSPSCBuffer = new VSGSPSPerFrameCBuffer;
	m_Flag = true;
	m_FirstRun = true;
	m_fSize = size;
	m_fRandMin = 0.0f;
	m_fRandMax = 0.0f;
	m_fDisableTime = 100.0f;

	

	// init data
	Particle p;
	ZeroMemory(&p, sizeof(Particle));
	p.InitialPos = Vector3(0.0, 0.0, 0.0);
	p.InitialVel = Vector3(0.0, 0.0, 0.0);
	p.Size = 10.0f;
	p.Age = 0.0f;
	p.Type = PT_EMITTER;
	p.NoData = 0.0f;
	unsigned int index[1], index1[2];
	index[0] = 0;
	index1[0] = 0;
	index1[1] = 1;
	*m_pTransform = Matrix4::Identity;

	Particle particles[20];
	//std::vector<Particle> particles;

	m_InitMesh = new MeshData(&p, 1, index, 1, sizeof(Particle), false);
	m_DrawMesh = new MeshData(&particles, m_iMaxParticles, index1, m_iMaxParticles, sizeof(Particle), true);
	m_StreamOutMesh = new MeshData(&particles, m_iMaxParticles, index1, m_iMaxParticles, sizeof(Particle), true);

	emitterPass = new RenderPass;
	emitterPass->SetVertexShader("../DEngine/Shaders/VS_stream_out.hlsl");
	emitterPass->SetGeometryShader("../DEngine/Shaders/GS_stream_out.hlsl");
	emitterPass->SetTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	drawPass = new RenderPass;
	drawPass->SetVertexShader("../DEngine/Shaders/VS_fire.hlsl");
	drawPass->SetGeometryShader("../DEngine/Shaders/GS_fire.hlsl");
	drawPass->SetPixelShader("../DEngine/Shaders/PS_fire.hlsl");
	drawPass->SetTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	drawPass->SetRasterizerState(State::CULL_NONE_RS);
	drawPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);

	drawPass->SetBlendState(State::ALPHA_BS);

	Handle hTexture(sizeof(Texture));
	if (type == YELLOW_LIGHT)
	{
		new (hTexture) Texture(Texture::SHADER_RESOURCES, 1, "yellow_light.dds");
		drawPass->AddTexture(hTexture);
	}
	else if (type == BLUE_LIGHT)
	{
		new (hTexture) Texture(Texture::SHADER_RESOURCES, 1, "blue_light.dds");
		drawPass->AddTexture(hTexture);
	}
	else if (type == BLEEDING)
	{
		new (hTexture) Texture(Texture::SHADER_RESOURCES, 1, "blood.dds");
		drawPass->AddTexture(hTexture);
	}
	else if (type == FIRE)
	{
		new (hTexture) Texture(Texture::SHADER_RESOURCES, 1, "fire.dds");
		drawPass->AddTexture(hTexture);
	}
	else
	{
		static wchar_t t[64];
		swprintf(t, 64, L"the texture of type /i is missing.\n", type);
		OutputDebugStringW(t);
	}

	ParticleSystem::GetInstance()->AddComponent(id, this);
}

Emitter::~Emitter()
{
}

float Emitter::GetAge()
{
	return m_fAge;
}

void Emitter::Draw()
{
}

void Emitter::SetEyePosW(const Vector3 & eyePosW)
{
	m_vEyePosW = eyePosW;
}

void Emitter::SetEmitPosW(const Vector3 & emitPosW)
{
	m_vEmitPosW = emitPosW;
}

void Emitter::SetEmitDirW(const Vector3 & emitDirW)
{
	m_vEmitDirW = emitDirW;
}

void Emitter::SetMaxPartiples(const unsigned int maxParticles)
{
	if (maxParticles < 7)
		m_iMaxParticles = 7;
	else
		m_iMaxParticles = maxParticles;
}

void Emitter::Update(const float dt)
{
	m_fFlareAge += dt;
	m_fTimeStep = dt;
	m_fDisableTime += dt;
}

void Emitter::SetOwner(GameObject * ptr)
{
	Component::SetOwner(ptr);
	m_pTransform = m_pOwner->GetTransform();
}

void Emitter::SetSize(float size)
{
	m_fSize = size;
}

void Emitter::SetRandomRange(float min, float max)
{
	m_fRandMin = min;
	m_fRandMax = max;
}

void Emitter::ResetDisableTime()
{
	m_fDisableTime = 0.0f;
}

};