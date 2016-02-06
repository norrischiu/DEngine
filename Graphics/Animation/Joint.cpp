#include "Joint.h"

Joint::Joint(const Matrix4& matrix) : m_currMatrix(matrix)
{
}


Joint::~Joint()
{
}

const Matrix4& Joint::getCurrMatrix() const {
	return m_currMatrix;
}

void Joint::setCurrMatrix(const Matrix4& matrix) {
	m_currMatrix = matrix;
}
