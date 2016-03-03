#include "ParticleSystem.h"
#include "Graphics\MeshData.h"

#define PT_EMITTER 0
#define PT_FLARE 1

ParticleSystem* ParticleSystem::m_pInstance;

ParticleSystem::ParticleSystem()
{
	m_fAge = 0.0f;
	m_fFlareAge = 0.0f;
	m_fTimeStep = 0.1f;
	m_iMaxParticles = 20;
	m_vEyePosW = Vector3(0.0f, 0.0f, 0.0f);
	m_vEmitPosW = Vector3(0.0f, 2.5f, 0.0f);
	m_vEmitDirW = Vector3(1.0f, 1.0f, 1.0f);
	m_pVSGSPSCBuffer = new VSGSPSPerFrameCBuffer;
	m_Flag = true;
	m_FirstRun = true;
}

float ParticleSystem::GetAge()
{
	return m_fAge;
}

void ParticleSystem::Init()
{
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
	drawPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
	drawPass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, "flare.dds"));

}

void ParticleSystem::Draw()
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

void ParticleSystem::SetEyePosW(const Vector3 & eyePosW)
{
	m_vEyePosW = eyePosW;
}

void ParticleSystem::SetEmitPosW(const Vector3 & emitPosW)
{
	m_vEmitPosW = emitPosW;
}

void ParticleSystem::SetEmitDirW(const Vector3 & emitDirW)
{
	m_vEmitDirW = emitDirW;
}

void ParticleSystem::SetMaxPartiples(const unsigned int maxParticles)
{
	m_iMaxParticles = maxParticles;
}

void ParticleSystem::Update(float dt)
{
	m_fFlareAge += dt;
	m_fTimeStep = dt;

}
