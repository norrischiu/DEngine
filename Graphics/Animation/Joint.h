#pragma once

#include <string>
#include "Math\simdmath.h"

class Joint
{
public:
	Joint(const Matrix4& matrix);
	~Joint();

	const Matrix4& getCurrMatrix() const;
	void setCurrMatrix(const Matrix4& matrix);

private:
	Matrix4 m_currMatrix;
};

