#pragma once

// Engine include
#include "Math\simdmath.h"

namespace DE
{

class Joint
{
public:

	/********************************************************************************
	*	--- Constructor:
	*	Joint(const Matrix4&, const int)
	*	This constructor will construct a joint by storing the inverse bind pose
	*	matrix and parent index
	*
	*	--- Parameters:
	*	@ matrix: the inverse bind pose matrix of this joint
	*	@ parentIndex: the parent joint index in array stored in skeleton
	********************************************************************************/
	Joint(const Matrix4& matrix, const int parentIndex);

	/********************************************************************************
	*	--- Destructor:
	*	~Joint()
	*	This destructor will do nothing
	********************************************************************************/
	~Joint();

	const Matrix4& getCurrMatrix() const;
	void setCurrMatrix(const Matrix4& matrix);

	const char*						m_sName;		// name of this joint, for debug purpose
	Matrix4							m_mBindPoseInv;		// inverse bind pose, i.e. joint space to model space transform
	int8_t							m_iParent;		// array index to parent joint (max: 255)
};

};

