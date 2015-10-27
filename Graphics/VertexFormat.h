// VertexFormat.h: Store all vertex format structure
#ifndef VERTEXFORMAT_H_
#define VERTEXFORMAT_H_

enum eVertexFormat
{
	POSITION,
	POSITION_COLOR,
	POSITION_TEXTURE,
	POSITION_NORMAL_TEXTURE
};

// Vertex: Position
struct Vertex1P
{
	Vector3								m_pos;
};

// Vertex: Position, diffuse color
struct Vertex1P1D
{
	Vector3								m_pos;
	DirectX::XMFLOAT4					m_diffuse; // 16-byte offset
};

// Vertex: Position, texture coordinate
struct Vertex1P1UV
{
	Vector3								m_pos;
	float								m_UV[2]; // 16-byte offset
};

// Vertex: Position, normal, texture coordinate
struct Vertex1P1N1UV
{
	Vector3								m_pos;
	Vector3								m_norm; // 16-byte offset
	float								m_UV[2]; // 32-byte offset
};

#endif



