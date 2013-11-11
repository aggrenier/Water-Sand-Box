#include "stdafx.h"
#include "Camera.h"

#include "GLUtils.h"

using namespace Utils ;

namespace ord12929
{

Camera::Camera()
{
	m_CameraPositionX = 0.f;
	m_CameraPositionZ = 0.f;
	m_CameraPositionY = 0.f;

	m_CameraAngleX = 0.f;
	m_CameraAngleY = 0.f;

	m_MouseLastX = 0;
	m_MouseLastY = 0;

	m_FrontSpeed = 0;
	m_SideSpeed = 0;
	m_UpSpeed = 0;
}

void Camera::Init()
{
	m_CameraPositionZ = -5.f;
}

void Camera::Animate(int frameTime)
{
	float s = (float)sin(GLUtils::Deg2Rad(m_CameraAngleX)) * 0.25f;
	float c = (float)cos(GLUtils::Deg2Rad(m_CameraAngleX)) * 0.25f;
	
	float speed = (float)frameTime * 0.05;

	if(m_SideSpeed != 0)
	{
		m_CameraPositionX += (float)m_SideSpeed * c * speed;
		m_CameraPositionZ += (float)m_SideSpeed * s * speed;
	}

	if(m_FrontSpeed != 0)
	{
		m_CameraPositionX -= (float)m_FrontSpeed * s * speed;
		m_CameraPositionZ += (float)m_FrontSpeed * c * speed;
	}

	m_CameraPositionY += (float)m_UpSpeed * speed * 0.1f;
}

void Camera::Render()
{
	glRotatef(m_CameraAngleX, 0.f, 1.f, 0.f);
	glRotatef(m_CameraAngleY, (float)cos(GLUtils::Deg2Rad((float)m_CameraAngleX)), 0.f, (float)sin(GLUtils::Deg2Rad((float)m_CameraAngleX)));
	glTranslatef(m_CameraPositionX, m_CameraPositionY, m_CameraPositionZ);
}

void Camera::SetPosition(float x, float z)
{
	m_CameraPositionX = x;
	m_CameraPositionZ = z;
}

void Camera::KeyDown(unsigned int key)
{
	if((char)key == 'W')
	{
		m_FrontSpeed = 1.f;
	}
	if((char)key == 'S')
	{
		m_FrontSpeed = -1.f;
	}
	if((char)key == 'A')
	{
		m_SideSpeed = 1.f;
	}
	if((char)key == 'D')
	{
		m_SideSpeed = -1.f;
	}
	if((char)key == 'E')
	{
		m_UpSpeed = 1.f;
	}
	if((char)key == 'Q')
	{
		m_UpSpeed = -1.f;
	}
}

void Camera::KeyUp(unsigned int key)
{
	if((char)key == 'W' || (char)key == 'S')
	{
		m_FrontSpeed = 0.f;
	}
	if((char)key == 'A' || (char)key == 'D')
	{
		m_SideSpeed = 0.f;
	}
	if((char)key == 'Q' || (char)key == 'E')
	{
		m_UpSpeed = 0.f;
	}
}

void Camera::MouseMove(unsigned int x, unsigned int y, bool leftButton, bool rightButton, bool control, bool shift)
{
	if(leftButton)
	{
		int localX = x - m_MouseLastX;
		m_CameraAngleX += 0.25f * (float)localX;

		int localY = y - m_MouseLastY;
		m_CameraAngleY += 0.25f * (float)localY;
		m_CameraAngleY = (m_CameraAngleY>60.f) ? 60.f : (m_CameraAngleY<-60.f) ? - 60.f : m_CameraAngleY;

		m_MouseLastX = x;
		m_MouseLastY = y;
	}
}

void Camera::MouseDown(MOUSE_BUTTON button, unsigned int x, unsigned int y)
{
	m_MouseLastX = x;
	m_MouseLastY = y;
}

void Camera::MouseUp(MOUSE_BUTTON button, unsigned int x, unsigned int y)
{

}

}