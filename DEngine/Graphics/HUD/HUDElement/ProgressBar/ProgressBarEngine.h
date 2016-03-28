#pragma once
#include <unordered_map>
#include "Graphics\MeshComponent.h"
#include "Memory\Handle.h"
#include "ProgressBar.h"

namespace DE
{

class ProgressBarEngine
{
public:
	ProgressBarEngine();
	~ProgressBarEngine();

	static ProgressBarEngine* getInstance();

	MeshComponent* makeProgress(ProgressBar* progressBar);
	void removeCacheByID(const char* id);
	void destructAndCleanUp();

private:
	static ProgressBarEngine* m_instance;

	std::unordered_map<const char*, Handle> m_cache;
};

}