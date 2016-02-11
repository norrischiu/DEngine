#include "Joint.h"

Joint::Joint(const Matrix4& matrix, const int parentIndex)
	: m_mBindPoseInv(matrix)
	, m_iParent(parentIndex)
{
}


Joint::~Joint()
{
}

const Matrix4& Joint::getCurrMatrix() const {
	return m_mBindPoseInv;
}

void Joint::setCurrMatrix(const Matrix4& matrix) {
	m_mBindPoseInv = matrix;
}
