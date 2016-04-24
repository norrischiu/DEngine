#include "FlowFieldManager.h"
#include "FlowFieldBuilder.h"

namespace DE
{

FlowFieldManager* FlowFieldManager::m_pInstance = nullptr;

FlowFieldManager::FlowFieldManager()
{
}


FlowFieldManager::~FlowFieldManager()
{
}

FlowFieldManager* FlowFieldManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new FlowFieldManager;
	}

	return m_pInstance;
}

int FlowFieldManager::GetPositionOwnerId(const Vector3& position)
{
	for (auto itr : m_vFlowFieldList)
	{
		const int id = itr->getPositionOwnerId(position);

		if (id != -1)
		{
			return id;
		}
	}

	return -1;
}

void FlowFieldManager::SetPositionOwnerId(const Vector3& position, const int gameObjId)
{
	for (auto itr : m_vFlowFieldList)
	{
		itr->setPositionOwnerId(position, gameObjId);
	}
}

void FlowFieldManager::LockPosition(const Vector3& position)
{
	for (auto itr : m_vFlowFieldList)
	{
		itr->setPositionMovable(position, false);
	}
}

void FlowFieldManager::UnLockPosition(const Vector3& position)
{
	for (auto itr : m_vFlowFieldList)
	{
		itr->setPositionMovable(position, true);
	}
}

void FlowFieldManager::UpdateFlowFieldDirection(const Vector3& position)
{
	for (auto itr : m_vFlowFieldList)
	{
		FlowFieldBuilder::getInstance()->updateFlowField(itr, position);
	}
}

void FlowFieldManager::AddFlowField(FlowField* flowField)
{
	m_vFlowFieldList.push_back(flowField);
}

};
