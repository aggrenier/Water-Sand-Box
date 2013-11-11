#pragma once

#include "resource.h"
#include <GL/gl.h>	
#include <GL/glu.h>	
#include <GL/glut.h>	

#include "GLFont\GLFont.h"

#include <vector>
#include <string>

#include "Camera.h"

namespace ord12929
{

class GLMesh;

class GLApp
{

public :

	GLApp(HWND Hwnd);
	~GLApp();

	void Init();
	void ReSize(GLsizei width, GLsizei height);
	void Animate(int frameTime);
	void Render();
	void Swap();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	void MouseMove(unsigned int x, unsigned int y, bool leftButton, bool rightButton, bool control, bool shift);
	void MouseDown(MOUSE_BUTTON button, unsigned int x, unsigned int y);
	void MouseUp(MOUSE_BUTTON button, unsigned int x, unsigned int y);

protected:
	void RenderFloor();
	void RenderQuad(float textureScale);
	void Render3D();
	void Render2D();

	void RenderBumpMap();
	void RenderTextureBlend();

	GLuint BuildProgram(const std::string& name);
	char* TextFileRead(const std::string& fileName);
	void PrintShaderInfoLog(GLuint obj);
	void PrintProgramInfoLog(GLuint obj);

protected:

	HWND m_Hwnd;
	Camera m_Camera;
	float m_ObjectAngle;

	bool m_LinePolygoneMode;

	GLuint m_Program1;
	GLuint m_Program2;

	GLuint m_Tex1;
	GLuint m_Tex2;
	GLuint m_Tex3;
	GLuint m_Tex4;
	GLuint m_Tex5;

	GLuint m_Tex6;
	GLuint m_Tex7;
	GLuint m_Tex8;
};


}