// Testing.cpp : Testing methods for library
//	DEBUG: check functionality
//	RELEASE: check speed
//

#include <iostream>
#include <Windows.h>
#include "gtest\gtest.h"
#include "..\Math\simdmath.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "..\Memory\MemoryManager.h"


#pragma warning(disable : 4996)

// Change typedef name if needed for testing
typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;
typedef SIMDQuaternion Quat;

#ifdef _DEBUG

/************************************
SIMD Functionality test
*************************************/

TEST(Vector, Normalization)
{
	Vector3 v(0.0f, 3.0f, 4.0f);
	v.Normalize();
	EXPECT_NEAR(0.8f, v.GetZ(), 0.01f);
}

TEST(Vector, DotProduct)
{
	Vector3 v(2.0f, 4.0f, 6.0f);
	EXPECT_NEAR(56.0f, v.Dot(v), 0.01f);
}

TEST(Vector, Addition)
{
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(2.0f, 4.0f, 6.0f);
	v1.Add(v2);
	EXPECT_NEAR(9.0f, v1.GetZ(), 0.01f);
}

TEST(Vector, OperatorAddition)
{
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(2.0f, 4.0f, 6.0f);
	Vector3 v3(2.0f, 4.0f, 6.0f);
	v1 = v1 + v2;
	EXPECT_NEAR(9.0f, v1.GetZ(), 0.01f);
}

TEST(Vector, OperatorEqualAddition)
{
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(2.0f, 4.0f, 6.0f);
	v1 += v1 + v2;
	EXPECT_NEAR(12.0f, v1.GetZ(), 0.01f);
}

TEST(Vector, Substraction)
{
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(2.0f, 4.0f, 6.0f);
	v1.Substract(v2);
	EXPECT_NEAR(-3.0f, v1.GetZ(), 0.01f);
}

TEST(Vector, OperatorSubstraction)
{
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(2.0f, 4.0f, 6.0f);
	v1 = v1 - v2 - v1;
	EXPECT_NEAR(-6.0f, v1.GetZ(), 0.01f);
}

TEST(Vector, OperatorEqualSubstraction)
{
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(2.0f, 4.0f, 6.0f);
	v1 -= v2 - v1;
	EXPECT_NEAR(0.0f, v1.GetZ(), 0.01f);
}

TEST(Vector, SquaredLength)
{
	Vector3 v(3.0f, 1.0f, 4.0f);
	EXPECT_NEAR(26.0f, v.LengthSquared(), 0.01f);
}

TEST(Vector, Length)
{
	Vector3 v(3.0f, 0.0f, 4.0f);
	EXPECT_NEAR(5.0f, v.Length(), 0.01f);
}

TEST(Vector, CrossProduct)
{
	Vector3 v1(3.0f, 2.0f, 5.0f);
	Vector3 v2(4.0f, 6.0f, 1.0f);
	Vector3 v3 = CrossProduct(v1, v2);
	EXPECT_NEAR(10.0f, v3.GetZ(), 0.01f);
}

TEST(Vector, LERP)
{
	Vector3 v1(1.0f, 10.0f, 100.0f);
	Vector3 v2(0.0f, 0.0f, 0.0f);
	Vector3 v3 = Lerp(v1, v2, 0.75f);
	EXPECT_NEAR(25.0f, v3.GetZ(), 0.01f);
}

TEST(Vector, Multiplication)
{
	Vector3 v(1.0f, 1.0f, 1.0f);
	v.Multiply(3.0f);
	EXPECT_NEAR(3.0f, v.GetZ(), 0.01f);
}

TEST(Vector, OperatorMultiplication)
{
	Vector3 v(1.0f, 1.0f, 1.0f);
	v = v * 3.0f * 3.0f;
	EXPECT_NEAR(9.0f, v.GetZ(), 0.01f);
}

TEST(Vector, Blend)
{
	Vector3 v1(1.0f, 1.0f, 1.0f);
	Vector3 v2(4.0f, 4.0f, 4.0f);
	Vector3 v3(8.0f, 8.0f, 8.0f);
	Vector3 v4(16.0f, 16.0f, 16.0f);

	Vector3 vBlendResult = Blend(v1, v2, v3, v4, 0.25f, 0.25f, 0.25f);
	EXPECT_NEAR(7.25f, vBlendResult.GetZ(), 0.01f);
}

TEST(Matrix, TransformPoint)
{
	Vector3 v(Vector3::Zero);
	float trans[4][4] = { 1.0f, 0.0f, 0.0f, -5.0f,
		0.0f, 1.0f, 0.0f, -10.0f,
		0.0f, 0.0f, 1.0f, 4.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 translate(trans);

	v.Transform(translate);
	EXPECT_NEAR(-5.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(-10.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(4.0f, v.GetZ(), 0.01f);

	float trans2[4][4] = { 1.0f, 0.0f, 0.0f, 5.0f,
		0.0f, 1.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 1.0f, -4.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 translate2(trans2);
	v.Transform(translate2);

	EXPECT_NEAR(0.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(0.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(0.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, TransformVector)
{
	Vector3 v(1.0f, 1.0f, 1.0f);
	float trans[4][4] = { 2.0f, 0.0f, 0.0f, -5.0f,
		0.0f, 2.0f, 0.0f, -10.0f,
		0.0f, 0.0f, 2.0f, 4.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 translate(trans);

	v.TransformAsVector(translate);
	EXPECT_NEAR(2.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(2.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(2.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, Multiplication)
{
	Vector3 v(1.0f, 1.0f, 1.0f);

	float trans[4][4] = { 3.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 3.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 3.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 world_transform(trans);

	float trans2[4][4] = { 1.0f, 0.0f, 0.0f, 5.0f,
		0.0f, 1.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 1.0f, -4.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 translate(trans2);

	//world_transform.Multiply(translate);
	world_transform *= translate;

	v.Transform(world_transform);

	EXPECT_NEAR(18.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(33.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(-9.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, Add)
{
	Vector3 v(1.0f, 1.0f, 1.0f);

	float trans[4][4] = { 3.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 3.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 3.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 world_transform(trans);

	float trans2[4][4] = { 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 translate(trans2);

	world_transform.Add(translate);

	v.Transform(world_transform);

	EXPECT_NEAR(4.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(4.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(4.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, Subtraction)
{
	Vector3 v(1.0f, 1.0f, 1.0f);

	float trans[4][4] = { 3.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 3.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 3.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 world_transform(trans);

	float trans2[4][4] = { 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4 translate(trans2);

	world_transform.Sub(translate);

	v.Transform(world_transform);

	EXPECT_NEAR(2.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(2.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(2.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, Scale)
{
	Matrix4 scale;
	scale.CreateScale(3.0f);
	Vector3 v(1.0f, 1.0f, 1.0f);
	v.Transform(scale);

	EXPECT_NEAR(3.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(3.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(3.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, RotateX)
{
	Matrix4 rot;
	rot.CreateRotationX(PI / 2.0f);
	Vector3 v(Vector3::UnitY);
	v.Transform(rot);

	EXPECT_NEAR(0.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(0.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(1.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, RotateY)
{
	Matrix4 rot;
	rot.CreateRotationY(PI / 2.0f);
	Vector3 v(Vector3::UnitX);
	v.Transform(rot);

	EXPECT_NEAR(0.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(0.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(-1.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, RotateZ)
{
	Matrix4 rot;
	rot.CreateRotationZ(PI / 2.0f);
	Vector3 v(Vector3::UnitX);
	v.Transform(rot);

	EXPECT_NEAR(0.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(1.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(0.0f, v.GetZ(), 0.01f);
}

TEST(Matrix, Translate)
{
	Matrix4 trans;
	trans.CreateTranslation(Vector3(5.0f, 10.0f, -4.0f));
	Vector3 v(Vector3::Zero);
	v.Transform(trans);

	EXPECT_NEAR(5.0f, v.GetX(), 0.01f);
	EXPECT_NEAR(10.0f, v.GetY(), 0.01f);
	EXPECT_NEAR(-4.0f, v.GetZ(), 0.01f);
}

TEST(Quaternion, Multiplcation)
{
	Vector3 axis(1.0f, 0.0f, 0.0f);
	Quat q(axis, PI / 4.0f);
	Quat p(axis, PI / 4.0f);

	q.Multiply(p);
	EXPECT_NEAR(0.707f, q.GetX(), 0.01f);
}



#endif
#ifdef NDEBUG

/************************************
SIMD Speed test
*************************************/

void TEST_SPEED_QUAT_MUL()
{
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;

	std::cout << "Testing multiplication of quaternion" << '\n';

	// SIMD
	QueryPerformanceCounter(&perf_start);
	Vector3 SIMDqv(1.0f, 2.0f, 3.0f);
	Quat SIMDq1(SIMDqv, 3);
	for (int i = 0; i < 10000; i++)
	{
		SIMDqv.Multiply(i);
		Quat SIMDq2(SIMDqv, 3);
		SIMDq1.Multiply(SIMDq2);
	}
	QueryPerformanceCounter(&perf_end);
	float elapsedSIMD = (perf_end.QuadPart - perf_start.QuadPart) / freqms;
	std::cout << "SSE SIMD multiplication\n";
	std::cout << "Total duration for 10000 = " << elapsedSIMD << "ms\n";
	std::cout << "Average duration = " << elapsedSIMD / 10000.0f << "ms\n";

	// DirectX
	QueryPerformanceCounter(&perf_start);
	Vector3 DXqv(1.0f, 2.0f, 3.0f);
	Quat DXq1(DXqv, 3);
	for (int i = 0; i < 10000; i++)
	{
		DXqv.Multiply(i);
		Quat DXq2(DXqv, 3);
		DXq1.MultiplyDX(DXq2);
	}
	QueryPerformanceCounter(&perf_end);

	float elapsedDX = (perf_end.QuadPart - perf_start.QuadPart) / freqms;
	std::cout << "DirectX::XMQuaternionMultiply\n";
	std::cout << "Total duration for 10000 = " << elapsedDX << "ms\n";
	std::cout << "Average duration = " << elapsedDX / 10000.0f << "ms\n";
}

void TEST_SPEED_FILE_IO()
{
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;

	std::cout << "Testing file read" << '\n';
	float arr1[10267 * 3];
	float arr2[10267 * 3];
	float arr3[10267 * 3];


	// C style
	QueryPerformanceCounter(&perf_start);
	FILE* fr = fopen("test.bufa", "r");
	for (int i = 0; i < 10267 * 3; i++)
	{
		float temp;
		fscanf(fr, "%f", &temp);	
		arr1[i] = temp;
	}
	QueryPerformanceCounter(&perf_end);
	float elapsedC = (perf_end.QuadPart - perf_start.QuadPart) / freqms;
	std::cout << "C style read\n";
	std::cout << "Total duration = " << elapsedC << "ms\n";
	fclose(fr);

	// C buffer style
	QueryPerformanceCounter(&perf_start);
	fr = fopen("test.bufa", "r");
	fseek(fr, 0, SEEK_END);
	long lSize = ftell(fr);
	rewind(fr);
	char* buffer = (char*)malloc(sizeof(char)*lSize);
	fread(buffer, 1, lSize, fr);
	std::stringstream ss(buffer);
	for (int i = 0; i < 10267 * 3; i++)
	{
		float temp;
		ss >> temp;
		arr2[i] = temp;
	}
	QueryPerformanceCounter(&perf_end);
	float elapsedCbuf = (perf_end.QuadPart - perf_start.QuadPart) / freqms;
	std::cout << "C buffer with stringstream style read\n";
	std::cout << "Total duration = " << elapsedCbuf << "ms\n";
	fclose(fr);

	// C++ style
	std::ifstream f;
	const int N = sizeof(float) * 10267 * 3;
	char buff[N];
	f.rdbuf()->pubsetbuf(buff, N);
	f.open("test.bufa", std::ios::in | std::ios::binary);
	QueryPerformanceCounter(&perf_start);
	for (int i = 0; i < 10267 * 3; i++)
	{
		float temp;
		f >> temp;
		arr3[i] = temp;
	}
	QueryPerformanceCounter(&perf_end);
	float elapsedCpp = (perf_end.QuadPart - perf_start.QuadPart) / freqms;
	std::cout << "C++ style read\n";
	std::cout << "Total duration = " << elapsedCpp << "ms\n";
	f.close();
}

void TEST_POOL_MEMORY()
{
	MemoryManager::getInstance()->Construct();
	MemoryManager::getInstance()->Print();

	Handle hle1 = MemoryManager::getInstance()->Allocate(sizeof(Vector3));
	Handle hle2 = MemoryManager::getInstance()->Allocate(sizeof(Vector3));

	MemoryManager::getInstance()->Print();

	Vector3* temp1 = new(hle1) Vector3(0, 0, 0);
	Vector3* temp2 = new(hle2) Vector3(0, 0, 0);
	std::cout << '\n' << temp1 << " " << temp2 << "\n";
	//std::cout << "size of pool:" << sizeof(MemoryPool) << "\n";
	std::cout << "handle 1: " << (uint32_t) hle1 << " ";
	std::cout << "handle 2: " << (uint32_t) hle2 << "\n";

	MemoryManager::getInstance()->Free(hle1);
	MemoryManager::getInstance()->Print();

	Handle hle3 = MemoryManager::getInstance()->Allocate(sizeof(Vector3));
	Vector3* temp3 = new(hle3) Vector3(0, 0, 0);

	MemoryManager::getInstance()->Print();

	std::cout << '\n' << temp3 << " " << temp2 << "\n";

	MemoryManager::getInstance()->Destruct();
}

int main(int argc, char* argv[])
{
	// Quaternion
	//TEST_SPEED_QUAT_MUL();
	// File IO
	TEST_SPEED_FILE_IO();
	// Pool memory
	//TEST_POOL_MEMORY();

	std::cin.getline(new char, 1);
}

#endif