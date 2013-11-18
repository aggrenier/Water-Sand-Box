#pragma once

#include "resource.h"
#include <GL/gl.h>	
#include <GL/glu.h>	
#include <GL/glut.h>	

#include "GLFont\GLFont.h"

#include "Water Simulation\WALL.h"
#include "Water Simulation\PARTICLE.h"
#include "Water Simulation\PARTICLE_SYSTEM.h"

#include <vector>
#include <string>

#include "Camera.h"

namespace ord12929
{

#define BRUTE1

#define SCENARIO_DAM      0
#define SCENARIO_FAUCET   1
#define SCENARIO_CUBE     2

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

	//Functions for Water Simulation
	void drawAxes();
	void displayCallback();
	void reshapeCallback(int width, int height);
	void keyboardCallback(unsigned char key, int x, int y);
	void glutMouseClick(int button, int state, int x, int y);
	void glutMouseMotion(int x, int y);
	void idleCallback();
	void waterSimulation();

	double arUtilTimer(void);

	static int ss, sms;

	// GUI interaction stuff
	//GLVU glvu;

	PARTICLE_SYSTEM * particleSystem;

	double dt;
	bool animate;
	int iterationCount;
	//double arUtilTimer(void);
	void arUtilTimerReset(void);


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