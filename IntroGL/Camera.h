#pragma once

namespace ord12929
{

enum MOUSE_BUTTON {MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT};
	
class Camera
{

public :
	Camera();

	void Init();
	void Animate(int frameTime);
	void Render();

	void SetPosition(float x, float z);

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	void MouseMove(unsigned int x, unsigned int y, bool leftButton, bool rightButton, bool control, bool shift);
	void MouseDown(MOUSE_BUTTON button, unsigned int x, unsigned int y);
	void MouseUp(MOUSE_BUTTON button, unsigned int x, unsigned int y);

	float getCameraAngleX() {return m_CameraAngleX;}

protected:
	float m_CameraPositionX;
	float m_CameraPositionY;
	float m_CameraPositionZ;

	float m_CameraAngleX;
	float m_CameraAngleY;

	int m_MouseLastX;
	int m_MouseLastY;

	int m_FrontSpeed;
	int m_SideSpeed;
	int m_UpSpeed;
};

}