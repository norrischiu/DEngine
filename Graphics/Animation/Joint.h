#pragma once

#include <string>
#include "Math\simdmath.h"

class Joint
{
public:
	Joint(const Matrix4& matrix, const int parentIndex);
	~Joint();

	const Matrix4& getCurrMatrix() const;
	void setCurrMatrix(const Matrix4& matrix);

	Matrix4 m_mBindPoseInv;
	uint8_t m_iParent;
};

