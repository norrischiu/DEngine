#include "Joint.h"

Joint::Joint(const Matrix4& matrix) : m_currMatrix(matrix)
{
}


Joint::~Joint()
{
}

void Joint::setCurrMatrix(const Matrix4& matrix) {
	m_currMatrix = matrix;
}

const Matrix4& Joint::getCurrMatrix() const {
	return m_currMatrix;
}