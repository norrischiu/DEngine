#pragma once
#include "Graphics\MeshComponent.h"
#include "Memory\Handle.h"
#include "ProgressBar.h"
#include "Utilities\MyHashMap.h"

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

	MyHashMap<Handle> m_cache;
};

}