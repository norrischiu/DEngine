// simdmath.h: Maths Libraray, defining vector, matrix and quaternion, with Intel SSE4.1 extensions and MSVC++
#ifndef SIMDMATH_H_
#define SIMDMATH_H_

#include <xmmintrin.h> // intrinics
#include <smmintrin.h> // intrinics
#include <math.h> // sin cos
#include <DirectXMath.h> // DirectX helper methods (temp)
#include "Memory\MemoryManager.h"

namespace DE
{

class SQT;

#define PI 3.1415926535f

// 4x4 Matrix with SSE
__declspec(align(16)) class SIMDMatrix4
{
private:
	__m128 _rows[4];

public:
	friend class SIMDVector3;

	static const SIMDMatrix4 Identity;
	static const SIMDMatrix4 Zero;

	// Default constructor
	inline SIMDMatrix4()
	{
		*this = SIMDMatrix4::Identity;
	};

	// Construct with given value
	inline SIMDMatrix4(float other[4][4])
	{
		_rows[0] = _mm_setr_ps(other[0][0], other[0][1], other[0][2], other[0][3]);
		_rows[1] = _mm_setr_ps(other[1][0], other[1][1], other[1][2], other[1][3]);
		_rows[2] = _mm_setr_ps(other[2][0], other[2][1], other[2][2], other[2][3]);
		_rows[3] = _mm_setr_ps(other[3][0], other[3][1], other[3][2], other[3][3]);
	}

	// Construct with given value, assume input is row major matrix
	inline SIMDMatrix4(float other[16])
	{
		_rows[0] = _mm_setr_ps(other[0], other[4], other[8], other[12]);
		_rows[1] = _mm_setr_ps(other[1], other[5], other[9], other[13]);
		_rows[2] = _mm_setr_ps(other[2], other[6], other[10], other[14]);
		_rows[3] = _mm_setr_ps(other[3], other[7], other[11], other[15]);
	}

	inline SIMDMatrix4(double other[16])
	{
		_rows[0] = _mm_setr_ps(other[0], other[4], other[8], other[12]);
		_rows[1] = _mm_setr_ps(other[1], other[5], other[9], other[13]);
		_rows[2] = _mm_setr_ps(other[2], other[6], other[10], other[14]);
		_rows[3] = _mm_setr_ps(other[3], other[7], other[11], other[15]);
	}

	// Construct with given m128 data
	inline SIMDMatrix4(__m128 data[4])
	{
		_rows[0] = data[0];
		_rows[1] = data[1];
		_rows[2] = data[2];
		_rows[3] = data[3];
	}

	// Copy constructor
	inline SIMDMatrix4(const SIMDMatrix4& other)
	{
		_rows[0] = other._rows[0];
		_rows[1] = other._rows[1];
		_rows[2] = other._rows[2];
		_rows[3] = other._rows[3];
	}

	// Assignment operator
	inline SIMDMatrix4& operator=(const SIMDMatrix4& other)
	{
		_rows[0] = other._rows[0];
		_rows[1] = other._rows[1];
		_rows[2] = other._rows[2];
		_rows[3] = other._rows[3];
		return *this;
	}

	// Set data values
	inline void Set(float other[4][4])
	{
		_rows[0] = _mm_setr_ps(other[0][0], other[0][1], other[0][2], other[0][3]);
		_rows[1] = _mm_setr_ps(other[1][0], other[1][1], other[1][2], other[1][3]);
		_rows[2] = _mm_setr_ps(other[2][0], other[2][1], other[2][2], other[2][3]);
		_rows[3] = _mm_setr_ps(other[3][0], other[3][1], other[3][2], other[3][3]);
	}

	// Add another matrix to the matrix, store the result back to this
	inline void Add(SIMDMatrix4& other)
	{
		_rows[0] = _mm_add_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_add_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_add_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_add_ps(_rows[3], other._rows[3]);
	}

	// Overload + operator
	inline SIMDMatrix4 operator+(SIMDMatrix4& other)
	{
		SIMDMatrix4 result;
		result._rows[0] = _mm_add_ps(_rows[0], other._rows[0]);
		result._rows[1] = _mm_add_ps(_rows[1], other._rows[1]);
		result._rows[2] = _mm_add_ps(_rows[2], other._rows[2]);
		result._rows[3] = _mm_add_ps(_rows[3], other._rows[3]);

		return result;
	}

	// Overload += operator
	inline void operator+=(SIMDMatrix4& other)
	{
		_rows[0] = _mm_add_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_add_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_add_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_add_ps(_rows[3], other._rows[3]);
	}

	// Subtract the matrix by another matrix, store the result back to this
	inline void Sub(SIMDMatrix4& other)
	{
		_rows[0] = _mm_sub_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_sub_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_sub_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_sub_ps(_rows[3], other._rows[3]);
	}

	// Overload - operator
	inline SIMDMatrix4 operator-(SIMDMatrix4& other)
	{
		SIMDMatrix4 result;
		result._rows[0] = _mm_sub_ps(_rows[0], other._rows[0]);
		result._rows[1] = _mm_sub_ps(_rows[1], other._rows[1]);
		result._rows[2] = _mm_sub_ps(_rows[2], other._rows[2]);
		result._rows[3] = _mm_sub_ps(_rows[3], other._rows[3]);

		return result;
	}

	// Overload -= operator
	inline void operator-=(SIMDMatrix4& other)
	{
		_rows[0] = _mm_sub_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_sub_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_sub_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_sub_ps(_rows[3], other._rows[3]);
	}

	// Pre-Multiply by another matrix, store the result back to this
	inline void Multiply(const SIMDMatrix4& mat)
	{
		__m128 mat_rows0 = mat._rows[0];
		__m128 mat_rows1 = mat._rows[1];
		__m128 mat_rows2 = mat._rows[2];
		__m128 mat_rows3 = mat._rows[3];
		_MM_TRANSPOSE4_PS(mat_rows0, mat_rows1, mat_rows2, mat_rows3);

		for (int i = 0; i < 4; ++i)
		{
			__m128 x = _mm_dp_ps(_rows[i], mat_rows0, 0xF1);
			__m128 y = _mm_dp_ps(_rows[i], mat_rows1, 0xF2);
			__m128 z = _mm_dp_ps(_rows[i], mat_rows2, 0xF4);
			__m128 w = _mm_dp_ps(_rows[i], mat_rows3, 0xF8);

			_rows[i] = _mm_add_ps(x, y);
			_rows[i] = _mm_add_ps(_rows[i], z);
			_rows[i] = _mm_add_ps(_rows[i], w);
		}
	}

	// Overload * operator
	inline SIMDMatrix4 operator*(const SIMDMatrix4& mat)
	{
		__m128 mat_rows0 = mat._rows[0];
		__m128 mat_rows1 = mat._rows[1];
		__m128 mat_rows2 = mat._rows[2];
		__m128 mat_rows3 = mat._rows[3];
		_MM_TRANSPOSE4_PS(mat_rows0, mat_rows1, mat_rows2, mat_rows3);

		SIMDMatrix4 result;
		for (int i = 0; i < 4; ++i)
		{
			__m128 x = _mm_dp_ps(mat_rows0, _rows[i], 0xF1);
			__m128 y = _mm_dp_ps(mat_rows1, _rows[i], 0xF2);
			__m128 z = _mm_dp_ps(mat_rows2, _rows[i], 0xF4);
			__m128 w = _mm_dp_ps(mat_rows3, _rows[i], 0xF8);

			result._rows[i] = _mm_add_ps(x, y);
			result._rows[i] = _mm_add_ps(result._rows[i], z);
			result._rows[i] = _mm_add_ps(result._rows[i], w);
		}
		return result;
	}

	// Overload *= operator
	inline void operator*=(const SIMDMatrix4& mat)
	{
		__m128 mat_rows0 = mat._rows[0];
		__m128 mat_rows1 = mat._rows[1];
		__m128 mat_rows2 = mat._rows[2];
		__m128 mat_rows3 = mat._rows[3];
		_MM_TRANSPOSE4_PS(mat_rows0, mat_rows1, mat_rows2, mat_rows3);

		for (int i = 0; i < 4; ++i)
		{
			__m128 x = _mm_dp_ps(_rows[i], mat_rows0, 0xF1);
			__m128 y = _mm_dp_ps(_rows[i], mat_rows1, 0xF2);
			__m128 z = _mm_dp_ps(_rows[i], mat_rows2, 0xF4);
			__m128 w = _mm_dp_ps(_rows[i], mat_rows3, 0xF8);

			_rows[i] = _mm_add_ps(x, y);
			_rows[i] = _mm_add_ps(_rows[i], z);
			_rows[i] = _mm_add_ps(_rows[i], w);
		}
	}

	// Set a scale transformation given a uniform scale
	inline void CreateScale(float scalar)
	{
		__m128 scale = _mm_set_ps1(scalar);

		_rows[0] = _mm_insert_ps(_rows[0], scale, 0x0E);
		_rows[1] = _mm_insert_ps(_rows[1], scale, 0x5D);
		_rows[2] = _mm_insert_ps(_rows[2], scale, 0xAB);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	inline void CreateScaleX(float scalar)
	{
		__m128 scale = _mm_set_ps1(scalar);
		__m128 identity = _mm_set_ps1(1.0f);

		_rows[0] = _mm_insert_ps(_rows[0], scale, 0x0E);
		_rows[1] = _mm_insert_ps(_rows[1], identity, 0x5D);
		_rows[2] = _mm_insert_ps(_rows[2], identity, 0xAB);
		_rows[3] = _mm_insert_ps(identity, identity, 0x07);
	}

	inline void CreateScaleY(float scalar)
	{
		__m128 scale = _mm_set_ps1(scalar);
		__m128 identity = _mm_set_ps1(1.0f);

		_rows[0] = _mm_insert_ps(_rows[0], identity, 0x0E);
		_rows[1] = _mm_insert_ps(_rows[1], scale, 0x5D);
		_rows[2] = _mm_insert_ps(_rows[2], identity, 0xAB);
		_rows[3] = _mm_insert_ps(_rows[3], identity, 0x07);
	}

	inline void CreateScaleZ(float scalar)
	{
		__m128 scale = _mm_set_ps1(scalar);
		__m128 identity = _mm_set_ps1(1.0f);

		_rows[0] = _mm_insert_ps(_rows[0], identity, 0x0E);
		_rows[1] = _mm_insert_ps(_rows[1], identity, 0x5D);
		_rows[2] = _mm_insert_ps(_rows[2], scale, 0xAB);
		_rows[3] = _mm_insert_ps(_rows[3], identity, 0x07);
	}

	inline void ScaleXYZ(float scalar)
	{
		__m128 scaleX = _mm_setr_ps(scalar, 1.0f, 1.0f, 1.0f);
		__m128 scaleY = _mm_setr_ps(1.0f, scalar, 1.0f, 1.0f);
		__m128 scaleZ = _mm_setr_ps(1.0f, 1.0f, scalar, 1.0f);
		_rows[0] = _mm_mul_ps(_rows[0], scaleX);
		_rows[1] = _mm_mul_ps(_rows[1], scaleY);
		_rows[2] = _mm_mul_ps(_rows[2], scaleZ);
	}

	// Set a rotation transformation about the X axis given an angle in radian
	inline void CreateRotationX(float radian)
	{
		float cosTheta = cosf(radian);
		float sinTheta = sinf(radian);

		_rows[0] = _mm_set_ps1(1.0f);
		_rows[0] = _mm_insert_ps(_rows[0], _rows[0], 0x0E);
		_rows[1] = _mm_setr_ps(0.0f, cosTheta, -sinTheta, 0.0f);
		_rows[2] = _mm_setr_ps(0.0f, sinTheta, cosTheta, 0.0f);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	// Set a rotation transformation about the Y axis given an angle in radian
	inline void CreateRotationY(float radian)
	{
		float cosTheta = cosf(radian);
		float sinTheta = sinf(radian);

		_rows[0] = _mm_setr_ps(cosTheta, 0.0f, sinTheta, 0.0f);
		_rows[1] = _mm_set_ps1(1.0f);
		_rows[1] = _mm_insert_ps(_rows[1], _rows[1], 0x0D);
		_rows[2] = _mm_setr_ps(-sinTheta, 0.0f, cosTheta, 0.0f);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	// Set a rotation transformation about the Z axis given an angle in radian
	inline void CreateRotationZ(float radian)
	{
		float cosTheta = cosf(radian);
		float sinTheta = sinf(radian);

		_rows[0] = _mm_setr_ps(cosTheta, -sinTheta, 0.0f, 0.0f);
		_rows[1] = _mm_setr_ps(sinTheta, cosTheta, 0.0f, 0.0f);
		_rows[2] = _mm_set_ps1(1.0f);
		_rows[2] = _mm_insert_ps(_rows[2], _rows[2], 0x0B);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	// Interpolate between two matrices with float t, return the resultant matrix
	// i.e. result = a * (1 - t) + b * t
	inline friend SIMDMatrix4 Lerp(const SIMDMatrix4& a, const SIMDMatrix4& b, float t)
	{
		__m128 resultRows[4];
		__m128 oneMinusT = _mm_set_ps1(1.0f - t);
		__m128 oneT = _mm_set_ps1(t);

		resultRows[0] = _mm_mul_ps(a._rows[0], oneMinusT);
		resultRows[0] = _mm_add_ps(resultRows[0], _mm_mul_ps(b._rows[0], oneT));
		resultRows[1] = _mm_mul_ps(a._rows[1], oneMinusT);
		resultRows[1] = _mm_add_ps(resultRows[1], _mm_mul_ps(b._rows[1], oneT));
		resultRows[2] = _mm_mul_ps(a._rows[2], oneMinusT);
		resultRows[2] = _mm_add_ps(resultRows[2], _mm_mul_ps(b._rows[2], oneT));
		resultRows[3] = _mm_mul_ps(a._rows[3], oneMinusT);
		resultRows[3] = _mm_add_ps(resultRows[3], _mm_mul_ps(b._rows[3], oneT));

		return SIMDMatrix4(resultRows);
	}

	// Set a translation transformation given a vector
	void CreateTranslation(const SIMDVector3& translation);

	void SetPosition(const SIMDVector3& translation);

	// Extract elements from matrix
	SIMDVector3 GetPosition();
	SIMDVector3 GetForward();
	SIMDVector3 GetUp();

	// Set a rotation transformation given a quaternion
	// void CreateRotationFromQuaternion(const SIMDQuaternion& q);

	// Set a look-at matrix (vUp MUST be normalized)
	void CreateLookAt(const SIMDVector3& vEye, const SIMDVector3& vAt, const SIMDVector3& vUp);
	static SIMDMatrix4 LookAtMatrix(const SIMDVector3& vEye, const SIMDVector3& vAt, const SIMDVector3& vUp);

	// Set a perspective FOV matrix
	void CreatePerspectiveFOV(float fFOVy, float fAspectRatio, float fNear, float fFar);
	static SIMDMatrix4 PerspectiveProjection(float fFOVy, float fAspectRatio, float fNear, float fFar);

	//Set a orthographic projection matrix // temp
	void CreateOrthographicProj(unsigned int width, unsigned int height, float zNear, float zFar)
	{
		DirectX::XMMATRIX result = DirectX::XMMatrixOrthographicLH(width, height, zNear, zFar);
		_rows[0] = result.r[0];
		_rows[1] = result.r[1];
		_rows[2] = result.r[2];
		_rows[3] = result.r[3];
		//_MM_TRANSPOSE4_PS(_rows[0], _rows[1], _rows[2], _rows[3]);
	}
	static SIMDMatrix4 OrthographicProjection(unsigned int width, unsigned int height, float zNear, float zFar);

	// Inverts the matrix, store the result back to this
	void Invert();
	SIMDMatrix4 Inverse();
};

// 3D Vector with SIMD
__declspec(align(16)) class SIMDVector3
{
private:
	__m128 _data;
	// _data.m128_f32[0] = x
	// _data.m128_f32[1] = y
	// _data.m128_f32[2] = z
	// _data.m128_f32[3] = w
	// right side at multiplication
public:
	friend class SIMDMatrix4;
	friend class SIMDQuaternion;
	friend class SQT;

	static const SIMDVector3 Zero;
	static const SIMDVector3 UnitX;
	static const SIMDVector3 UnitY;
	static const SIMDVector3 UnitZ;
	static const SIMDVector3 NegativeUnitX;
	static const SIMDVector3 NegativeUnitY;
	static const SIMDVector3 NegativeUnitZ;

	// Default Constructor
	inline SIMDVector3() {};

	// Construct with given float values
	inline SIMDVector3(float x, float y, float z, float w = 1.0f)
	{
		_data = _mm_setr_ps(x, y, z, w);
	};

	// Construct with given m128 data
	inline SIMDVector3(__m128 value)
	{
		_data = value;
	}

	// Copy constructor
	inline SIMDVector3(const SIMDVector3& other)
	{
		_data = other._data;
	}

	// Assignment constructor
	inline SIMDVector3& operator=(const SIMDVector3& other)
	{
		_data = other._data;
		return *this;
	}

	// Set data values
	inline void Set(float x, float y, float z)
	{
		_data = _mm_setr_ps(x, y, z, 1.0f);
	}

	inline void SetX(float x)
	{
		__m128 temp = _mm_set_ss(x);
		_data = _mm_insert_ps(_data, temp, 0x00);
	}

	inline void SetY(float y)
	{
		__m128 temp = _mm_set_ss(y);
		_data = _mm_insert_ps(_data, temp, 0x10);
	}

	inline void SetZ(float z)
	{
		__m128 temp = _mm_set_ss(z);
		_data = _mm_insert_ps(_data, temp, 0x20);
	}

	inline void SetW(float w)
	{
		__m128 temp = _mm_set_ss(w);
		_data = _mm_insert_ps(_data, temp, 0x30);
	}

	inline float GetX() const
	{
		return _data.m128_f32[0];
	}

	inline float GetY() const
	{
		return _data.m128_f32[1];
	}

	inline float GetZ() const
	{
		return _data.m128_f32[2];
	}

	inline float GetW() const
	{
		return _data.m128_f32[3];
	}

	// Dot product, return a float
	inline float Dot(const SIMDVector3& other)
	{
		__m128 temp = _mm_dp_ps(_data, other._data, 0x78);
		return	temp.m128_f32[3];
	}

	// Add two vector, store result back to this
	inline void Add(const SIMDVector3& other)
	{
		_data = _mm_add_ps(_data, other._data);
	}

	// Overload + operator
	inline SIMDVector3 operator+(const SIMDVector3& other)
	{
		SIMDVector3 result;
		result._data = _mm_add_ps(_data, other._data);
		return result;
	}

	// Overload += operator
	inline void operator+=(const SIMDVector3& other)
	{
		_data = _mm_add_ps(_data, other._data);
	}

	// Substract the other vector from this, store result back to this
	inline void Substract(const SIMDVector3& other)
	{
		_data = _mm_sub_ps(_data, other._data);
	}

	// Overload - operator
	inline SIMDVector3 operator-(const SIMDVector3& other) const
	{
		SIMDVector3 result;
		result._data = _mm_sub_ps(_data, other._data);
		return result;
	}

	// Overload -= operator
	inline void operator-=(const SIMDVector3& other)
	{
		_data = _mm_sub_ps(_data, other._data);
	}

	// Return negative
	inline SIMDVector3 operator-()
	{
		SIMDVector3 result;
		__m128 zero = _mm_set_ss(0.0f);
		result._data = _mm_sub_ps(zero, _data);
		return result;
	}

	// Multiple the vector by a scalar, stire result back to this
	inline void Multiply(float scalar)
	{
		__m128 temp = _mm_set_ps1(scalar);
		_data = _mm_mul_ps(_data, temp);
	}

	// Overload * operator
	inline SIMDVector3 operator*(float scalar)
	{
		SIMDVector3 result;
		__m128 slr = _mm_set_ps1(scalar);
		result._data = _mm_mul_ps(_data, slr);
		return result;
	}

	// Normalize the vector, store result back to this
	inline SIMDVector3& Normalize()
	{
		__m128 length = _mm_dp_ps(_data, _data, 0xFF);
		length = _mm_rsqrt_ps(length);
		_data = _mm_mul_ps(_data, length);
		return *this;
	}

	// Return the square of the length of vector
	inline float LengthSquared() const
	{
		__m128 length = _mm_dp_ps(_data, _data, 0x78);
		return length.m128_f32[3];
	}

	// Return the length of vector
	inline float Length() const
	{
		__m128 length = _mm_dp_ps(_data, _data, 0x78);
		return sqrtf(length.m128_f32[3]);
	}

	inline bool iszero() const
	{
		return Length() < std::numeric_limits<float>::epsilon();
	}

	// Return the normal of a SIMDVector3
	inline friend SIMDVector3 Normal(const SIMDVector3& vec)
	{
		SIMDVector3 result = vec;
		result.Normalize();
		return result;
	}

	// Return the cross product as SIMDVector3 of two vectors
	inline friend SIMDVector3 Cross(const SIMDVector3& a, const SIMDVector3& b)
	{
		SIMDVector3 result;
		result._data = _mm_mul_ps(_mm_shuffle_ps(a._data, a._data, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b._data, b._data, _MM_SHUFFLE(3, 1, 0, 2)));
		__m128 temp = _mm_mul_ps(_mm_shuffle_ps(a._data, a._data, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b._data, b._data, _MM_SHUFFLE(3, 0, 2, 1)));
		result._data = _mm_sub_ps(result._data, temp);
		return result;
	}

	// Interpolate between two vectors with float t, return the resultant vector
	// i.e. result = a * (1 - t) + b * t
	inline friend SIMDVector3 Lerp(const SIMDVector3& a, const SIMDVector3& b, float t)
	{
		__m128 tempA = _mm_set_ps1(1.0f - t);
		tempA = _mm_mul_ps(a._data, tempA);
		__m128 tempB = _mm_set_ps1(t);
		tempB = _mm_mul_ps(b._data, tempB);
		__m128 result = _mm_add_ps(tempA, tempB);
		return SIMDVector3(result);
	}

	// 4-way blend of vectors(colors), return the resultant vector
	// i.e. result = a * t1 + b * t2 + c * t3 + d * (1 - t1 -t2 - t3)
	inline friend SIMDVector3 Blend(const SIMDVector3& a, const SIMDVector3& b, const SIMDVector3& c, const SIMDVector3& d, float t1, float t2, float t3)
	{
		__m128 tempA = _mm_set_ps1(t1);
		tempA = _mm_mul_ps(a._data, tempA);
		__m128 tempB = _mm_set_ps1(t2);
		tempB = _mm_mul_ps(b._data, tempB);
		__m128 tempC = _mm_set_ps1(t3);
		tempC = _mm_mul_ps(c._data, tempC);
		__m128 tempD = _mm_set_ps1(1.0f - t1 - t2 - t3);
		tempD = _mm_mul_ps(d._data, tempD);

		__m128 result = _mm_add_ps(tempA, tempB);
		result = _mm_add_ps(result, tempC);
		result = _mm_add_ps(result, tempD);
		return SIMDVector3(result);
	}

	// Transform the vector as a point by a 4x4 Matrix, store result back to this
	inline void Transform(const SIMDMatrix4& mat)
	{
		// set w to 1.0f
		__m128 one = _mm_set_ss(1.0f);
		_data = _mm_insert_ps(_data, one, 0x30);

		__m128 x = _mm_dp_ps(_data, mat._rows[0], 0xF1);
		__m128 y = _mm_dp_ps(_data, mat._rows[1], 0xF2);
		__m128 z = _mm_dp_ps(_data, mat._rows[2], 0xF4);
		__m128 w = _mm_dp_ps(_data, mat._rows[3], 0xF8);

		_data = _mm_insert_ps(_data, x, 0x00);
		_data = _mm_insert_ps(_data, y, 0x50);
		_data = _mm_insert_ps(_data, z, 0xA0);
		_data = _mm_insert_ps(_data, w, 0xF0);
	}

	// Transform the vector by a 4x4 Matrix, store result back to this
	inline void TransformAsVector(const SIMDMatrix4& mat)
	{
		// Set w to 0.0f
		_data = _mm_insert_ps(_data, _data, 0x08);

		__m128 x = _mm_dp_ps(_data, mat._rows[0], 0xF1);
		__m128 y = _mm_dp_ps(_data, mat._rows[1], 0xF2);
		__m128 z = _mm_dp_ps(_data, mat._rows[2], 0xF4);
		__m128 w = _mm_dp_ps(_data, mat._rows[3], 0xF8);

		_data = _mm_insert_ps(_data, x, 0x00);
		_data = _mm_insert_ps(_data, y, 0x50);
		_data = _mm_insert_ps(_data, z, 0xA0);
		_data = _mm_insert_ps(_data, w, 0xF0);
	}
};

__declspec(align(16)) class SIMDQuaternion
{
private:
	__m128 _data;
	// _data.m128_f32[0] = x
	// _data.m128_f32[1] = y
	// _data.m128_f32[2] = z
	// _data.m128_f32[3] = w
public:
	friend class SIMDMatrix4;
	friend class SIMDVector3;

	static const SIMDQuaternion Identity;

	// Default constructor
	SIMDQuaternion() {};

	// Construct with given axis and angle in radian
	SIMDQuaternion(SIMDVector3& axis, float radian)
	{
		__m128 sinTheta = _mm_set_ps1(sinf(radian / 2.0f));
		__m128 cosTheta = _mm_set_ps1(cosf(radian / 2.0f));
		axis.Normalize();
		_data = _mm_mul_ps(axis._data, sinTheta);
		_data = _mm_insert_ps(_data, cosTheta, 0xF0);
	}

	// Construct with direct data
	SIMDQuaternion(float data[4])
	{
		_data = _mm_setr_ps(data[0], data[1], data[2], data[3]);
	}

	// Copy constructor
	SIMDQuaternion(const SIMDQuaternion& other)
	{
		_data = other._data;
	}

	// Assignment operator
	inline SIMDQuaternion& operator=(const SIMDQuaternion& other)
	{
		_data = other._data;
		return *this;
	}

	inline float GetX() const
	{
		return _data.m128_f32[0];
	}

	inline float GetY() const
	{
		return _data.m128_f32[1];
	}

	inline float GetZ() const
	{
		return _data.m128_f32[2];
	}

	inline float GetW() const
	{
		return _data.m128_f32[3];
	}

	// Return negative
	inline SIMDQuaternion operator-() const
	{
		SIMDQuaternion result;
		__m128 zero = _mm_set_ss(0.0f);
		result._data = _mm_sub_ps(zero, _data);
		return result;
	}

	// Multiply this quaternion with another quaternion, store result back to this
	inline void Multiply(const SIMDQuaternion& other)
	{
		__m128 result;
		__m128 q2X = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 q2Y = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 q2Z = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 q2W = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(3, 3, 3, 3));

		q2X = _mm_mul_ps(q2X, _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(0, 1, 2, 3)));
		q2Y = _mm_mul_ps(q2X, _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(1, 0, 3, 2)));
		q2Z = _mm_mul_ps(q2X, _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(2, 3, 0, 1)));
		q2W = _mm_mul_ps(q2W, _data);

		result = _mm_addsub_ps(_mm_shuffle_ps(q2W, q2W, _MM_SHUFFLE(2, 3, 0, 1)), _mm_shuffle_ps(q2X, q2X, _MM_SHUFFLE(2, 3, 0, 1)));
		result = _mm_addsub_ps(_mm_shuffle_ps(result, result, _MM_SHUFFLE(2, 0, 3, 1)), _mm_shuffle_ps(q2Y, q2Y, _MM_SHUFFLE(3, 1, 2, 0)));
		result = _mm_addsub_ps(_mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(q2Z, q2Z, _MM_SHUFFLE(2, 3, 1, 0)));
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 2, 0, 1));
		_data = result;
	}

	inline void MultiplyDX(const SIMDQuaternion& other)
	{
		_data = DirectX::XMQuaternionMultiply(_data, other._data);
	}

	SIMDQuaternion operator+(const SIMDQuaternion other)
	{
		// TODO
	}

	SIMDMatrix4 GetRotationMatrix()
	{
		//DirectX::XMMATRIX result = DirectX::XMMatrixRotationQuaternion(_data);
		//result = DirectX::XMMatrixTranspose(result);
		//return SIMDMatrix4(result.r);

		float x, y, z, w;
		x = _data.m128_f32[0];
		y = _data.m128_f32[1];
		z = _data.m128_f32[2];
		w = _data.m128_f32[3];

		__m128 rows[4];
		//rows[0] = _mm_setr_ps(1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y), 0.0f);
		//rows[1] = _mm_setr_ps(2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x), 0.0f);
		//rows[2] = _mm_setr_ps(2 * (x * z + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y), 0.0f);
		//rows[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

		rows[0] = _mm_setr_ps(1 - 2 * (y * y + z * z), 2 * (x * y - w * z), 2 * (x * z + w * y), 0.0f);
		rows[1] = _mm_setr_ps(2 * (x * y + w * z), 1 - 2 * (x * x + z * z), 2 * (y * z - w * x), 0.0f);
		rows[2] = _mm_setr_ps(2 * (x * z - w * y), 2 * (y * z + w * x), 1 - 2 * (x * x + y * y), 0.0f);
		rows[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

		return SIMDMatrix4(rows);
	}

	// Normalize the quaternion, store the result to this
	inline void Normalize()
	{
		__m128 length = _mm_dp_ps(_data, _data, 0xFF);
		length = _mm_rsqrt_ps(length);
		_data = _mm_mul_ps(_data, length);
	}

	// Dot product, return a float
	inline float Dot(const SIMDQuaternion& other) const
	{
		__m128 temp = _mm_dp_ps(_data, other._data, 0xFF);
		return temp.m128_f32[3];
	}

	// TODO: switch to utility function in namespace
	static SIMDQuaternion Lerp(SIMDQuaternion a, SIMDQuaternion b, float t)
	{
		SIMDQuaternion result;
		__m128 aFactor = _mm_set_ps1(1.0f - t);
		__m128 bFactor = _mm_set_ps1(t);
		result._data = _mm_mul_ps(aFactor, a._data);
		result._data = _mm_add_ps(result._data, _mm_mul_ps(bFactor, b._data));
		return result;
	}

	static SIMDQuaternion SLerp(SIMDQuaternion a, SIMDQuaternion b, float t)
	{
		SIMDQuaternion result;
		result._data = DirectX::XMQuaternionSlerp(a._data, b._data, t);
		return result;
	}
};

typedef SIMDVector3 Vector3;
typedef SIMDVector3 Vector4;
typedef SIMDMatrix4 Matrix4;
typedef SIMDQuaternion Quaternion;

}
#endif