// VertexFormat.h: Store all vertex format structure
#ifndef VERTEXFORMAT_H_
#define VERTEXFORMAT_H_

#include "Math/simdmath.h"

enum eVertexFormat
{
	POSITION,
	POSITION_COLOR,
	POSITION_TEXTURE,
	POSITION_NORMAL_TEXTURE,
	POSITION_NORMAL_TANGENT_TEXTURE
};

// Vertex: Position
struct Vertex1P
{
	Vector3								m_pos;
	
	Vertex1P() {};
	Vertex1P(const Vector3& pos) : m_pos(pos){ }
};

// Vertex: Position, diffuse color
struct Vertex1P1D
{
	Vector3								m_pos;
	DirectX::XMFLOAT4					m_diffuse; // 16-byte offset
};

// Vertex: Position, texture coordinate
struct vertex1P1UV
{
	Vector3								m_pos;
	float								m_UV[2]; // 16-byte offset

	vertex1P1UV() { }
	vertex1P1UV(const Vector3& pos, float uv[2]) : m_pos(pos) {
		m_UV[0] = uv[0];
		m_UV[1] = uv[1];
	}
};

// Vertex: Position, normal
struct Vertex1P1N
{
	Vector3								m_pos;
	Vector3								m_norm; // 16-byte offset

	Vertex1P1N() {};
	Vertex1P1N(const Vector3& pos, const Vector3& norm) : m_pos(pos), m_norm(norm) { }
};

// Vertex: Position, normal, texture coordinate
struct Vertex1P1N1UV
{
	Vector3								m_pos;
	Vector3								m_norm; // 16-byte offset
	float								m_UV[2]; // 32-byte offset

	Vertex1P1N1UV() { }
	Vertex1P1N1UV(const Vector3& pos, const Vector3& norm, float uv[2]) : m_pos(pos), m_norm(norm) {
		m_UV[0] = uv[0];
		m_UV[1] = uv[1];
 	}
};

struct Vertex1P1N1T1UV
{
	Vector3								m_pos;
	Vector3								m_norm; // 16-byte offset
	Vector3								m_tangent; // 32-btye offset
	float								m_UV[2]; // 48-byte offset
};

struct Vertex1P1N1T1UV4J
{
	Vector3								m_pos;
	Vector3								m_norm; // 16-byte offset
	Vector3								m_tangent; // 32-btye offset
	float								m_UV[2]; // 48-byte offset
	int									m_jointIndex[4]; //56-byte offset
	Vector4								m_jointWeight; // m_jointWeight[3] = 1 - m_jointWeight[0..2]
};

#endif