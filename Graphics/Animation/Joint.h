#pragma once

#include <string>
#include "Math\simdmath.h"

class Joint
{
public:
	Joint(const Matrix4& matrix);
	~Joint();

	void setCurrMatrix(const Matrix4& matrix);
	const Matrix4& getCurrMatrix() const;

private:
	Matrix4 m_currMatrix;
};

