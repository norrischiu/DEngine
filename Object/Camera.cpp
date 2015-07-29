// Camera.cpp

#include "Camera.h"
#include <Windows.h> // get keyboard state
#include <sstream>
#include <string>

static unsigned int VK_W = 0x57; // W
static unsigned int VK_A = 0x41; // A
static unsigned int VK_S = 0x53; // S
static unsigned int VK_D = 0x44; // D

Camera::Camera(Vector3 m_vPos, Vector3 m_vLookAt, Vector3 m_vUp)
	: m_vPos(m_vPos)
	, m_vLookAt(m_vLookAt)
	, m_vUp(m_vUp)
{
	m_vLookAt.Normalize();
}

Matrix4 Camera::getViewMatrix()
{
	Matrix4 cameraMatrix;
	cameraMatrix.CreateLookAt(m_vPos, m_vLookAt, m_vUp);
	return cameraMatrix;
}

void Camera::update()
{
	// key held
	if (GetAsyncKeyState(VK_W) && 0x8000)
	{
		move(0.05);
		OutputDebugString("W pressed");
	}

	if (GetAsyncKeyState(VK_S) && 0x8000)
	{
		move(-0.05);
		OutputDebugString("S pressed");
	}

}

void Camera::move(float offset)
{
	m_vPos += (m_vLookAt - m_vPos).Normalize() * offset;
	
	// debug text
	std::stringstream ss;
	ss << m_vPos.GetX() << " " << m_vPos.GetY() << " " << m_vPos.GetZ() << "\n";
	std::string str(ss.str());
	OutputDebugString(str.c_str());
}