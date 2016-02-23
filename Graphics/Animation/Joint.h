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

	// Inverse bind pose, i.e. joint space to model space transform
	Matrix4							m_mBindPoseInv;

	// Array index to parent joint (max: 255)
	int8_t							m_iParent;
};

