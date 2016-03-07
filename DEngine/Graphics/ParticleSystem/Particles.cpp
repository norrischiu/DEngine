#include "Particles.h"
#include "Graphics\MeshData.h"

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

Emitter::Emitter(int type, Vector3 & emitPos, Vector3 & emitDir) : Component()
, m_pTransform(new Matrix4())
{
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
	emitterPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	drawPass = new RenderPass;
	drawPass->SetVertexShader("../DEngine/Shaders/VS_fire.hlsl");
	drawPass->SetGeometryShader("../DEngine/Shaders/GS_fire.hlsl");
	drawPass->SetPixelShader("../DEngine/Shaders/PS_fire.hlsl");
	drawPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	drawPass->SetRasterizerState(State::CULL_NONE_RS);
	drawPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
	drawPass->SetDepthStencilView(D3D11Renderer::GetInstance()->m_depthReadOnly->GetDSV());
	drawPass->SetBlendState(State::ALPHA_BS);

	if (type == TORCH_FLAME)
	{
		drawPass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, "flare.dds"));
	}
	else if (type == SMOKE)
	{
		drawPass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, "smoke.dds"));
	}
	else if (type == ROCKET_TRAIL)
	{
		drawPass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, "smoke.dds"));
	}
	else if (type == FIRE)
	{
		drawPass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, "fire.dds"));
	}
}

Emitter::~Emitter()
{
}

float Emitter::GetAge()
{
	return m_fAge;
}

void Emitter::AddParticle(int type, Vector3& emitPos, Vector3& emitDir)
{
	m_ID = ComponentID;
	m_fAge = 0.0f;
	m_fFlareAge = 0.0f;
	m_fTimeStep = 0.0f;
	m_iMaxParticles = 20;
	m_vEyePosW = Vector3(0.0f, 0.0f, 0.0f);
	m_vEmitPosW = emitPos;
	m_vEmitDirW = emitDir;
	m_Flag = true;
	m_FirstRun = true;
	m_EffectType = type;

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


	Particle particles[20];
	//std::vector<Particle> particles;

	m_InitMesh = new MeshData(&p, 1, index, 1, sizeof(Particle), false);
	m_DrawMesh = new MeshData(&particles, m_iMaxParticles, index1, m_iMaxParticles, sizeof(Particle), true);
	m_StreamOutMesh = new MeshData(&particles, m_iMaxParticles, index1, m_iMaxParticles, sizeof(Particle), true);

	emitterPass = new RenderPass;
	emitterPass->SetVertexShader("../DEngine/Shaders/VS_stream_out.hlsl");
	emitterPass->SetGeometryShader("../DEngine/Shaders/GS_stream_out.hlsl");
	emitterPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	drawPass = new RenderPass;
	drawPass->SetVertexShader("../DEngine/Shaders/VS_fire.hlsl");
	drawPass->SetGeometryShader("../DEngine/Shaders/GS_fire.hlsl");
	drawPass->SetPixelShader("../DEngine/Shaders/PS_fire.hlsl");
	drawPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	drawPass->SetRasterizerState(State::CULL_NONE_RS);
	drawPass->SetBlendState(State::ALPHA_BS);
	drawPass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, "flare.dds"));

}

void Emitter::Draw()
{
	// Pass the game time/step to geometry shader
	m_pVSGSPSCBuffer->BindToRenderer();

	VSGSPSPerFrameCBuffer::VSGSPS_PER_FRAME_CBUFFER* ptr = (VSGSPSPerFrameCBuffer::VSGSPS_PER_FRAME_CBUFFER*) m_pVSGSPSCBuffer->m_Memory._data;

	ptr->gViewProj = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix();
	ptr->gEyePosW = D3D11Renderer::GetInstance()->GetCamera()->GetPosition();
	ptr->gEmitPosW = m_vEmitPosW;
	ptr->gEmitDirW = m_vEmitDirW;
	ptr->gFlareAge = m_fFlareAge;
	ptr->gTimeStep = m_fTimeStep;
	ptr->gMaxParts = m_iMaxParticles;
	ptr->gEffectType = m_EffectType;
	m_pVSGSPSCBuffer->Update();


	// first pass
	// updated particle list is streamed out to vertex buffer
	if (m_FirstRun)
	{
		emitterPass->SetStreamOutTargets(m_DrawMesh->GetVertexBuffer());
		emitterPass->SetRasterizerState(State::CULL_NONE_RS);
		emitterPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
		m_InitMesh->RenderUsingPass(emitterPass);
		D3D11Renderer::GetInstance()->UnBindStreamOutTargets();

		// second pass
		drawPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
		m_DrawMesh->RenderUsingPass(drawPass);
		m_FirstRun = false;
	}
	else
	{
		if (m_Flag)
		{
			emitterPass->SetStreamOutTargets(m_StreamOutMesh->GetVertexBuffer());
			emitterPass->SetRasterizerState(State::CULL_NONE_RS);
			emitterPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
			m_DrawMesh->RenderUsingPass(emitterPass);
			D3D11Renderer::GetInstance()->UnBindStreamOutTargets();
			// second pass
			drawPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
			m_StreamOutMesh->RenderUsingPass(drawPass);
			m_Flag = false;
		}
		else if (!m_Flag)
		{
			emitterPass->SetStreamOutTargets(m_DrawMesh->GetVertexBuffer());
			emitterPass->SetRasterizerState(State::CULL_NONE_RS);
			emitterPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
			m_StreamOutMesh->RenderUsingPass(emitterPass);
			D3D11Renderer::GetInstance()->UnBindStreamOutTargets();
			// second pass
			drawPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
			m_DrawMesh->RenderUsingPass(drawPass);
			m_Flag = true;
		}
	}

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

void Emitter::Update(float dt)
{
	m_fFlareAge += dt;
	m_fTimeStep = dt;
	
	m_vEmitPosW.Transform(*m_pTransform);

}
void Emitter::SetOwner(GameObject * ptr)
{
	Component::SetOwner(ptr);
	m_pTransform = m_pOwner->GetTransform();
}
};