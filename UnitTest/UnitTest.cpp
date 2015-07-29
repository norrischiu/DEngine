// Testing.cpp : Testing methods for library
//	DEBUG: check functionality
//	RELEASE: check speed
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "gtest\gtest.h"
#include "..\Math\simdmath.h"
#include "..\Memory\poolmemory.h"
#include <vector>
#include <algorithm>

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

/************************************
	Pool Memory test
*************************************/
typedef PoolMemoryManager<32, 64> SmallPool;

TEST(PoolMemory, AllocateAndFree)
{
	SmallPool::Instance()->Construct();
	char* temp = reinterpret_cast<char*>(SmallPool::Instance()->Allocate(32));
	EXPECT_FALSE(temp == 0);
	
	SmallPool::Instance()->Free(temp);
	char* temp2 = reinterpret_cast<char*>(SmallPool::Instance()->Allocate(32));
	EXPECT_EQ(temp, temp2);
	SmallPool::Instance()->Free(temp2);
	SmallPool::Instance()->Destruct();
}

TEST(PoolMemory, Overflow)
{
	void* temp;
	SmallPool::Instance()->Construct();
	for (int i = 0; i < 64; i++)
		temp = SmallPool::Instance()->Allocate(32);

	temp = SmallPool::Instance()->Allocate(32);
	EXPECT_EQ(NULL, temp);
	SmallPool::Instance()->Destruct();
}

TEST(PoolMemory, Reuse)
{
	void* pointers[64];
	void* pointers2[64];
	//void* temp;
	SmallPool::Instance()->Construct();
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 64; j++)
			pointers[j] = SmallPool::Instance()->Allocate(32);

		for (int j = 0; j < 64; j++)
			SmallPool::Instance()->Free(pointers[64 - j - 1]);

		for (int j = 0; j < 64; j++)
		{
			pointers2[j] = SmallPool::Instance()->Allocate(32);
			EXPECT_EQ(pointers[j], pointers2[j]);
		}

		for (int j = 0; j < 64; j++)
		{
			SmallPool::Instance()->Free(pointers[64 - j - 1]);
		}
	}
	SmallPool::Instance()->Destruct();
}

TEST(PoolMemory, RandomReuse)
{
	SmallPool::Instance()->Construct();
	std::vector<void*> blocks;
	for (int i = 0; i < 64; i++)
	{
		blocks.push_back(SmallPool::Instance()->Allocate(32));
	}

	std::srand(static_cast<unsigned>(std::time(0)));
	std::random_shuffle(blocks.begin(), blocks.end());
	for (int i = 0; i < 32; i++)
	{
		void* temp = blocks.back();
		SmallPool::Instance()->Free(temp);
		blocks.pop_back();
	}

	for (int i = 0; i < 32; i++)
	{
		void* temp = SmallPool::Instance()->Allocate(32);
		blocks.push_back(temp);
		EXPECT_TRUE(*(std::find(blocks.begin(), blocks.end(), temp)) == blocks.back());
	}
	SmallPool::Instance()->Destruct();
}

TEST(PoolMemory, Alignment)
{
	SmallPool::Instance()->Construct();
	printf("Size of Vector3: %i\n", sizeof(Vector3));
	char* temp = reinterpret_cast<char*>(SmallPool::Instance()->Allocate(sizeof(Vector3)));
	EXPECT_FALSE(temp == NULL);
	printf("temp = 0x%x\n", temp);
	char* temp2 = reinterpret_cast<char*>(SmallPool::Instance()->Allocate(sizeof(Vector3)));
	printf("temp2 = 0x%x\n", temp2);
	Vector3* vTemp = new(temp) Vector3(1.0f, 2.0f, 3.0f);
	Vector3* vTemp2 = new(temp2) Vector3(2.0f, 4.0f, 6.0f);
	EXPECT_NEAR(28.0f, vTemp->Dot(*vTemp2), 0.01f);
	printf("result = %f\n", vTemp->Dot(*vTemp2));
	SmallPool::Instance()->Destruct();
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

	// SIMD
	std::cout << "Testing multiplication of quaternion" << std::endl;
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

int _tmain(int argc, _TCHAR* argv[])
{
	// Quaternion
	TEST_SPEED_QUAT_MUL();

	std::cin.getline(new char, 1);
}

#endif