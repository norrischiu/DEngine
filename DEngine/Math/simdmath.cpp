#include "simdmath.h"
#include "simdmath.h"

namespace DE
{

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

void SIMDMatrix4::CreateOrthographicProj(unsigned int width, unsigned int height, float zNear, float zFar)
{
	_rows[0] = _mm_setr_ps(2.0f / width, 0.0f, 0.0f, 0.0f);
	_rows[1] = _mm_setr_ps(0.0f, 2.0f / height, 0.0f, 0.0f);
	_rows[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f / (zFar - zNear), zNear / (zNear - zFar));
	_rows[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

SIMDMatrix4 SIMDMatrix4::OrthographicProjection(unsigned int width, unsigned int height, float zNear, float zFar)
{
	SIMDMatrix4 mat;
	mat.CreateOrthographicProj(width, height, zNear, zFar);
	return mat;
}

void SIMDMatrix4::Invert()
{
	__m128 minor0, minor1, minor2, minor3;
	__m128 row0, row1, row2, row3;
	__m128 det, tmp1;

	// tranpose and arrange as
	row0 = _mm_setr_ps(_rows[0].m128_f32[0], _rows[1].m128_f32[0], _rows[2].m128_f32[0], _rows[3].m128_f32[0]); // 1 2 3 4
	row1 = _mm_setr_ps(_rows[2].m128_f32[1], _rows[3].m128_f32[1], _rows[0].m128_f32[1], _rows[1].m128_f32[1]); // 7 8 5 6
	row2 = _mm_setr_ps(_rows[0].m128_f32[2], _rows[1].m128_f32[2], _rows[2].m128_f32[2], _rows[3].m128_f32[2]); // 9 10 11 12
	row3 = _mm_setr_ps(_rows[2].m128_f32[3], _rows[3].m128_f32[3], _rows[0].m128_f32[3], _rows[1].m128_f32[3]); // 15 16 13 14

	// calculate cofactor
	tmp1 = _mm_mul_ps(row2, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0 = _mm_mul_ps(row1, tmp1); // add
	minor1 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0); // minus
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
	
	tmp1 = _mm_mul_ps(row1, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0); // add
	minor3 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1)); // minus
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
	
	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);
	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0); // add
	minor2 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1)); // minus
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
	
	tmp1 = _mm_mul_ps(row0, row1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
	
	tmp1 = _mm_mul_ps(row0, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
	
	tmp1 = _mm_mul_ps(row0, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E); 
	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

	det = _mm_mul_ps(row0, minor0);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
	tmp1 = _mm_rcp_ss(det);
	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
	det = _mm_shuffle_ps(det, det, 0x00);

	_rows[0] = _mm_mul_ps(det, minor0);
	_rows[1] = _mm_mul_ps(det, minor1);
	_rows[2] = _mm_mul_ps(det, minor2);
	_rows[3] = _mm_mul_ps(det, minor3);
}

SIMDMatrix4 SIMDMatrix4::Inverse()
{
	SIMDMatrix4 result = *this;
	result.Invert();
	return result;
}

SIMDMatrix4 SIMDMatrix4::Transpose()
{
	SIMDMatrix4 result = *this;
	_MM_TRANSPOSE4_PS(result._rows[0], result._rows[1], result._rows[2], result._rows[3]);
	return result;
}

};