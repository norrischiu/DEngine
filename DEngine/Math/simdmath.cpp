#include "simdmath.h"

namespace DE
{

// Matrix4 constant declaration
float identityMatrix[4][4] = {
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
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

void SIMDMatrix4::SetPosition(const SIMDVector3 & translation)
{
	__m128 transX = _mm_setr_ps(0.0f, 0.0f, 0.0f, translation.GetX());
	__m128 transY = _mm_setr_ps(0.0f, 0.0f, 0.0f, translation.GetY());
	__m128 transZ = _mm_setr_ps(0.0f, 0.0f, 0.0f, translation.GetZ());

	_rows[0] = _mm_insert_ps(_rows[0], transX, 0xF0);
	_rows[1] = _mm_insert_ps(_rows[1], transY, 0xF0);
	_rows[2] = _mm_insert_ps(_rows[2], transZ, 0xF0);
}

SIMDVector3 SIMDMatrix4::GetPosition()
{
	return SIMDVector3(_rows[0].m128_f32[3], _rows[1].m128_f32[3], _rows[2].m128_f32[3]);
}

SIMDVector3 SIMDMatrix4::GetForward()
{
	return SIMDVector3(_rows[0].m128_f32[2], _rows[1].m128_f32[2], _rows[2].m128_f32[2]);
}

SIMDVector3 SIMDMatrix4::GetRight()
{
	return SIMDVector3(_rows[0].m128_f32[0], _rows[1].m128_f32[0], _rows[2].m128_f32[0]);
}

SIMDVector3 SIMDMatrix4::GetUp()
{
	return SIMDVector3(_rows[0].m128_f32[1], _rows[1].m128_f32[1], _rows[2].m128_f32[1]);
}

void SIMDMatrix4::CreateLookAt(const SIMDVector3& vEye, const SIMDVector3& vAt, const SIMDVector3& vUp)
{
	SIMDVector3 zAxis = (vAt - vEye).Normalize();
	SIMDVector3 xAxis = Cross(vUp, zAxis).Normalize();
	SIMDVector3 yAxis = Cross(zAxis, xAxis);

	_rows[0] = _mm_setr_ps(xAxis.GetX(), xAxis.GetY(), xAxis.GetZ(), -xAxis.Dot(vEye));
	_rows[1] = _mm_setr_ps(yAxis.GetX(), yAxis.GetY(), yAxis.GetZ(), -yAxis.Dot(vEye));
	_rows[2] = _mm_setr_ps(zAxis.GetX(), zAxis.GetY(), zAxis.GetZ(), -zAxis.Dot(vEye));
	_rows[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

SIMDMatrix4 SIMDMatrix4::LookAtMatrix(const SIMDVector3 & vEye, const SIMDVector3 & vAt, const SIMDVector3 & vUp)
{
	Matrix4 mat;
	mat.CreateLookAt(vEye, vAt, vUp);
	return mat;
}

void SIMDMatrix4::CreatePerspectiveFOV(float fFOVy, float fAspectRatio, float fNear, float fFar)
{
	float fYScale = tanf(PI / 2.0f - (fFOVy / 2)); // cot(x) is the same as tan(pi/2 - x)
	float fXScale = fYScale / fAspectRatio;

	_rows[0] = _mm_setr_ps(fXScale, 0.0f, 0.0f, 0.0f);
	_rows[1] = _mm_setr_ps(0.0f, fYScale, 0.0f, 0.0f);
	_rows[2] = _mm_setr_ps(0.0f, 0.0f, fFar / (fFar - fNear), -fNear*fFar / (fFar - fNear));
	_rows[3] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);

}

SIMDMatrix4 SIMDMatrix4::PerspectiveProjection(float fFOVy, float fAspectRatio, float fNear, float fFar)
{
	SIMDMatrix4 mat;
	mat.CreatePerspectiveFOV(fFOVy, fAspectRatio, fNear, fFar);
	return mat;
}

SIMDMatrix4 SIMDMatrix4::OrthographicProjection(unsigned int width, unsigned int height, float zNear, float zFar)
{
	SIMDMatrix4 mat;
	mat.CreateOrthographicProj(width, height, zNear, zFar);
	return mat;
}

void SIMDMatrix4::Invert()
{
	DirectX::XMMATRIX result = DirectX::XMMATRIX(_rows[0], _rows[1], _rows[2], _rows[3]);
	result = DirectX::XMMatrixInverse(nullptr, result);
	_rows[0] = result.r[0];
	_rows[1] = result.r[1];
	_rows[2] = result.r[2];
	_rows[3] = result.r[3];
}

SIMDMatrix4 SIMDMatrix4::Inverse()
{
	SIMDMatrix4 result = *this;
	result.Invert();
	return result;
}

SIMDMatrix4 SIMDMatrix4::Transpose()
{
	DirectX::XMMATRIX result = DirectX::XMMATRIX(_rows[0], _rows[1], _rows[2], _rows[3]);
	result = DirectX::XMMatrixTranspose(result);
	_rows[0] = result.r[0];
	_rows[1] = result.r[1];
	_rows[2] = result.r[2];
	_rows[3] = result.r[3];
	return *this;
}

};