#pragma once
#include "FlowField.h"
#include "../../Graphics/VertexFormat.h"
#include <vector>

namespace DE
{

class FlowFieldManager
{
public:
	FlowFieldManager();
	~FlowFieldManager();

	static FlowFieldManager* GetInstance();

	void AddFlowField(FlowField* flowField);
	void UpdateFlowFieldDirection(const Vector3& position);

	int GetPositionOwnerId(const Vector3& position);
	void SetPositionOwnerId(const Vector3& position, const int gameObjId);

	void LockPosition(const Vector3& position);
	void UnLockPosition(const Vector3& position);

private:
	static FlowFieldManager* m_pInstance;
	std::vector<FlowField*> m_vFlowFieldList;
};

};