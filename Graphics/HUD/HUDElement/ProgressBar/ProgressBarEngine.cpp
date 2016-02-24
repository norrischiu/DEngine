
#include "ProgressBarEngine.h"
#include "../TextBox/TextEngine.h"
#include "VertexFormat.h"
#include "D3D11Renderer.h"

ProgressBarEngine* ProgressBarEngine::m_instance;

ProgressBarEngine::ProgressBarEngine()
{
}

ProgressBarEngine* ProgressBarEngine::getInstance()
{
	if (!m_instance) {
		m_instance = new ProgressBarEngine();
	}

	return m_instance;
}

MeshComponent* ProgressBarEngine::makeProgress(ProgressBar* progressBar)
{
	char* id = progressBar->getID();

	if (progressBar->hasUpdate()) {
		m_cache.erase(id);
		TextEngine::getInstance()->removeCacheByID(id);
		progressBar->setHasUpdate(false);
	}

	if (!m_cache[id]) {
		const HUDElement::Position position = progressBar->getPosition();
		const HUDElement::Size size = progressBar->getSize();
		const float progress = progressBar->getProgress();
		char progressString[20];
		sprintf(progressString, "%.1f%%", progress);

		Vertex1P* pVerticesProgress = new Vertex1P[4] {
			Vector3(0.0f,									0.0f,				0.0f),
			Vector3(0.0f - progress / 100.0f - 0.003f,		0.0f,				0.0f),
			Vector3(0.0f,									0.0f - 0.5f,		0.0f),
			Vector3(0.0f - progress / 100.0f - 0.003f,		0.0f - 0.5f,		0.0f)
		};

		Vertex1P* pVerticesProgressBorder = new Vertex1P[4] {
			Vector3(0.0f,			0.0f,				0.0f),
			Vector3(0.0f - 1.0f,	0.0f,				0.0f),
			Vector3(0.0f,			0.0f - 0.5f,		0.0f),
			Vector3(0.0f - 1.0f,	0.0f - 0.5f,		0.0f)
		};

		unsigned int* pIndicesProgress = new unsigned int[6] {
			0, 1, 2,
			2, 1, 3
		};

		unsigned int* pIndicesProgressBorder = new unsigned int[8] {
			0, 1,
			2, 3,
			1, 3,
			2, 4
		};

		const int fontSize = size.height / 2.0f;
		const int xOffset = size.width / 2.0f - TextEngine::getInstance()->getTextWidth(progressString) / 2.0f * fontSize;
		const int yOffset = size.height / 4.0f - 1.0f / 3.0f * fontSize;

		HUDElement::Position pos(
			position.x + xOffset,
			position.y + yOffset
		);

		MeshComponent* meshComponent = nullptr;

		if (progressBar->isShowText()) {
			meshComponent = new MeshComponent[3] {
				MeshComponent(new MeshData(pVerticesProgress, 4, pIndicesProgress, 6, sizeof(Vertex1P))),
				MeshComponent(new MeshData(pVerticesProgressBorder, 4, pIndicesProgressBorder, 8, sizeof(Vertex1P))),
				*TextEngine::getInstance()->makeText(
					new TextBox(progressBar->getID(), progressString, pos, fontSize, TextBox::Color::RED)
				)
			};
		} else {
			meshComponent = new MeshComponent[2] {
				MeshComponent(new MeshData(pVerticesProgress, 4, pIndicesProgress, 6, sizeof(Vertex1P))),
				MeshComponent(new MeshData(pVerticesProgressBorder, 4, pIndicesProgressBorder, 8, sizeof(Vertex1P)))
			};
		}


		for (int i = 0; i <= 1; i++) {
			RenderPass* progressPass = new RenderPass;
			progressPass->SetVertexShader("Shaders/VS_vertex1P.hlsl");
			progressPass->SetPixelShader("Shaders/PS_red.hlsl");
			if (i == 1) { progressPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); }
			meshComponent[i].m_pMeshData->m_Material.AddPassToTechnique(progressPass);
			progressPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
			progressPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
			progressPass->SetRasterizerState(State::CULL_NONE_RS);

			Matrix4 scaleX, scaleY, translate;
			scaleX.CreateScaleX(-1.0f * size.width);
			scaleY.CreateScaleY(1.0f * size.height);
			translate.CreateTranslation(Vector3(-1024 / 2.0f + position.x, 768 / 2.0f - position.y, 0.0f));
			*meshComponent[i].m_pTransform = Matrix4::OrthographicProjection(1024, 768, 0.0f, 1.0f) * translate * scaleX * scaleY;
		}

		m_cache[id] = meshComponent;
	}

	return m_cache[id];
}

void ProgressBarEngine::removeCacheByID(const char* id)
{
	m_cache.erase(id);
}

void ProgressBarEngine::destructAndCleanUp()
{
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

ProgressBarEngine::~ProgressBarEngine()
{
}
