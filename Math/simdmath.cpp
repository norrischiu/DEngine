#include "simdmath.h"

// Matrix4 constant declaration
float identityMatrix[4][4] = { 
	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
};
const SIMDMatrix4 SIMDMatrix4::Identity(identityMatrix);

// Vector3 constant declaration
const SIMDVector3 SIMDVector3::Zero(0.0f, 0.0f, 0.0f);
const SIMDVector3 SIMDVector3::UnitX(1.0f, 0.0f, 0.0f);
const SIMDVector3 SIMDVector3::UnitY(0.0f, 1.0f, 0.0f);
const SIMDVector3 SIMDVector3::UnitZ(0.0f, 0.0f, 1.0f);
const SIMDVector3 SIMDVector3::NegativeUnitX(-1.0f, 0.0f, 0.0f);
const SIMDVector3 SIMDVector3::NegativeUnitY(0.0f, -1.0f, 0.0f);
const SIMDVector3 SIMDVector3::NegativeUnitZ(0.0f, 0.0f, -1.0f);

// Matrix4 methods
void SIMDMatrix4::CreateTranslation(const SIMDVector3& translation)
{
	_rows[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, translation.GetX());
	_rows[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, translation.GetY());
	_rows[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, translation.GetZ());
	_rows[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

void SIMDMatrix4::CreateLookAt(const SIMDVector3& vEye, const SIMDVector3& vAt, const SIMDVector3& vUp)
{
	SIMDVector3 x = vEye;
	SIMDVector3 y = vAt;
	SIMDVector3 z = vUp;
	__m128 temp = _mm_set_ss(0.0f); //
	x._data = _mm_insert_ps(x._data, temp, 0x30); //
	y._data = _mm_insert_ps(y._data, temp, 0x30); //
	z._data = _mm_insert_ps(z._data, temp, 0x30); //

	DirectX::XMMATRIX result = DirectX::XMMatrixLookAtLH(x._data, y._data, z._data);
	_rows[0] = result.r[0];
	_rows[1] = result.r[1];
	_rows[2] = result.r[2];
	_rows[3] = result.r[3];
	_MM_TRANSPOSE4_PS(_rows[0], _rows[1], _rows[2], _rows[3]);
}

void SIMDMatrix4::CreatePerspectiveFOV(float fFOVy, float fAspectRatio, float fNear, float fFar)
{
	float fYScale = tanf(1.57079633f - (fFOVy / 2)); // cot(x) is the same as tan(pi/2 - x)
	float fXScale = fYScale / fAspectRatio;

	_rows[0] = _mm_setr_ps(fXScale, 0.0f, 0.0f, 0.0f);
	_rows[1] = _mm_setr_ps(0.0f, fYScale, 0.0f, 0.0f);
	_rows[2] = _mm_setr_ps(0.0f, 0.0f, fFar / (fFar - fNear), -fNear*fFar / (fFar - fNear));
	_rows[3] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
}