#include "stdafx.h"
#include "GLApp.h"

#include "GLMesh.h"

#include <cmath>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>

using namespace Utils;

namespace ord12929
{

	GLApp::GLApp(HWND Hwnd)
	{
		m_Hwnd = Hwnd;
		m_LinePolygoneMode = false;

		m_ObjectAngle = 0.f;
		m_Program1 = 0;
		m_Program2 = 0;

        //water simulation code 
		dt = 1.0 / 100.0;
		animate = false;
		iterationCount = 0;
	}

	GLApp::~GLApp()
	{

	}

	void GLApp::Init()
	{
		glewInit();

		RECT rect;
		GetClientRect(m_Hwnd, &rect);
		ReSize(rect.right - rect.left, rect.bottom - rect.top);

		m_Camera.SetPosition(0.f, -5.f);

		glClearColor(0.f, 0.f, 0.f, 1.f);					
		glClearDepth(1.f);									
		glEnable(GL_DEPTH_TEST);							
		glDepthFunc(GL_LEQUAL);								

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
		GLfloat diffuseLight[] = { 1.f, 1.f, 1.f, 1.0f };
		GLfloat specularLight[] = { 1.f, 1.f, 1.f, 1.0f };

		glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
		glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);

		GLfloat lightpos[] = {0.f, 5.f, 10.f, 0.f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);

		glEnable( GL_TEXTURE_2D );

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		m_Program1 = BuildProgram("bump");
		m_Program2 = BuildProgram("multi");

		m_Tex1 = GLUtils::LoadTexture("..\\Textures\\1.jpg");
		m_Tex2 = GLUtils::LoadTexture("..\\Textures\\2.jpg");
		m_Tex3 = GLUtils::LoadTexture("..\\Textures\\m1.jpg");
		m_Tex4 = GLUtils::LoadTexture("..\\Textures\\m2.jpg");
		m_Tex5 = GLUtils::LoadTexture("..\\Textures\\m3.jpg");

		m_Tex6 = GLUtils::LoadTexture("..\\Textures\\s1.jpg");
		m_Tex7 = GLUtils::LoadTexture("..\\Textures\\s2.jpg");
		m_Tex8 = GLUtils::LoadTexture("..\\Textures\\s3.jpg");
	}

	void GLApp::ReSize(GLsizei width, GLsizei height)
	{
		glViewport(0,0,width,height);						
		glMatrixMode(GL_PROJECTION);						
		glLoadIdentity();	

		float aspect = (GLfloat)width/(GLfloat)height;
		gluPerspective(45.f, aspect, 0.1f, 100.f);

		glMatrixMode(GL_MODELVIEW);							
		glLoadIdentity();
	}

	void GLApp::Animate(int frameTime)
	{
		m_Camera.Animate(frameTime);

		m_ObjectAngle += (float)frameTime * 0.025f;
	}

	void GLApp::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		Render3D();

		glLoadIdentity();
		Render2D();
	}

	void GLApp::RenderFloor()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		for(int i=-10; i<11; i++)
		{
			if(i == 0)
				glColor3f(0.5f, 0.5f, 0.5f);
			else
				glColor3f(0.2f, 0.2f, 0.2f);

			glVertex3f((float)i, -1.f, -10.f);
			glVertex3f((float)i, -1.f, 10.f);
			glVertex3f(-10.f, -1.f, (float)i);
			glVertex3f(10.f, -1.f, (float)i);
		}
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}

	void GLApp::RenderQuad(float textureScale)
	{
		glBegin(GL_QUADS);
		glColor4f(1.f,1.f,1.f,1.f);
		glNormal3f(0.f,0.f,1.f);
		glTexCoord2d(textureScale,0.0);
		glVertex3f(1.f,0.f,0.f);
		glTexCoord2d(textureScale,textureScale);
		glVertex3f(1.f,1.f,0.f);
		glTexCoord2d(0.0,textureScale);
		glVertex3f(0.f,1.f,0.f);
		glTexCoord2d(0.0,0.0);
		glVertex3f(0.f,0.f,0.f);
		glEnd();
	}

	void GLApp::Render3D()
	{

		GLfloat lightpos[] = {0.f, 2.f, 10.f, 0.f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

		m_Camera.Render();
		//RenderFloor();

		//RenderBumpMap();
		//RenderTextureBlend();

	}

	void GLApp::RenderBumpMap()
	{
		glUseProgram(m_Program1);

		GLint loc = glGetUniformLocation(m_Program1, "lightPos");
		glUniform3f(loc, 0.f, 2.f, 10.f);

		GLuint t1Location = glGetUniformLocation(m_Program1, "tex1");
		glUniform1i(t1Location, 0);
		GLuint t2Location = glGetUniformLocation(m_Program1, "tex2");
		glUniform1i(t2Location, 1);
		GLuint t3Location = glGetUniformLocation(m_Program1, "tex3");
		glUniform1i(t3Location, 2);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture( GL_TEXTURE_2D, m_Tex8 );
		glActiveTexture(GL_TEXTURE1);
		glBindTexture( GL_TEXTURE_2D, m_Tex7 );
		glActiveTexture(GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, m_Tex6 );

		glRotatef(m_ObjectAngle, 0.f, 1.f, 0.f);
		glutSolidTeapot(1.f);
		glUseProgram(0);
	}

	void GLApp::RenderTextureBlend()
	{
		glUseProgram(m_Program2);

		GLuint t1Location = glGetUniformLocation(m_Program2, "tex1");
		glUniform1i(t1Location, 0);
		GLuint t2Location = glGetUniformLocation(m_Program2, "tex2");
		glUniform1i(t2Location, 1);
		GLuint t3Location = glGetUniformLocation(m_Program2, "tex3");
		glUniform1i(t3Location, 2);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture( GL_TEXTURE_2D, m_Tex8 );
		glActiveTexture(GL_TEXTURE1);
		glBindTexture( GL_TEXTURE_2D, m_Tex7 );
		glActiveTexture(GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, m_Tex6 );

		glRotatef(m_ObjectAngle, 0.f, 1.f, 0.f);

		GLint wallSize = glGetUniformLocation(m_Program2, "wallSize");
		glUniform2f(wallSize, 6.f, 3.f);

		for(int j = 0; j < 3; j++)
		{
			for(int i = -3; i < 3; i++)
			{
				float x = (float)i;
				float y = (float)j;

				GLint loc = glGetUniformLocation(m_Program2, "offset");
				glUniform3f(loc, x, y, 0.f);

				glPushMatrix();	
				glTranslatef(x, y, 0.f);
				RenderQuad(1.f);
				glutSolidTeapot(1.f);
				glPopMatrix();	
			}
		}

		glUseProgram(0);
	}

	void GLApp::Render2D()
	{

	}

	void GLApp::Swap()
	{
		SwapBuffers(GetDC(m_Hwnd));
	}

	void GLApp::KeyDown(unsigned int key)
	{
		m_Camera.KeyDown(key);
	}

	void GLApp::KeyUp(unsigned int key)
	{
		m_Camera.KeyUp(key);

		/*if((char)key == 'L')
		{
		m_LinePolygoneMode = !m_LinePolygoneMode;
		if(m_LinePolygoneMode)
		{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		}*/

		switch (key)
		{
		
		case 'a':
			animate = !animate;
			break;			
		
		case '=':
			particleSystem->surfaceThreshold += 0.1;
			cout << "surface threshold: " << particleSystem->surfaceThreshold << endl;
			break;

		case '-':
			particleSystem->surfaceThreshold -= 0.1;
			cout << "surface threshold: " << particleSystem->surfaceThreshold << endl;
			break;

		case 's':
			particleSystem->toggleSurfaceVisible();
			break;

		case '/':
			particleSystem->toggleGravity();
			break;

		case '.':
			particleSystem->toggleArrows();
			break;

		case 't':
			particleSystem->toggleTumble();
			break;

		case '1':
			particleSystem->loadScenario(SCENARIO_DAM);
			break;

		case '2':
			particleSystem->loadScenario(SCENARIO_CUBE);
			break;
		case '3':
			particleSystem->loadScenario(SCENARIO_FAUCET);
			break;
		}
	}

	void GLApp::MouseMove(unsigned int x, unsigned int y, bool leftButton, bool rightButton, bool control, bool shift)
	{
		m_Camera.MouseMove(x, y, leftButton, rightButton, control, shift);
	}

	void GLApp::MouseDown(MOUSE_BUTTON button, unsigned int x, unsigned int y)
	{
		m_Camera.MouseDown(button, x, y);
	}

	void GLApp::MouseUp(MOUSE_BUTTON button, unsigned int x, unsigned int y)
	{

	}

	GLuint GLApp::BuildProgram(const std::string& name) 
	{
		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);	

		char* vs = TextFileRead("..\\Shaders\\" + name + ".vert");
		char* fs = TextFileRead("..\\Shaders\\" + name + ".frag");

		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(v, 1, &vv,NULL);
		glShaderSource(f, 1, &ff,NULL);

		free(vs);
		free(fs);

		glCompileShader(v);
		glCompileShader(f);

		GLuint p = glCreateProgram();

		glAttachShader(p,v);
		glAttachShader(p,f);
		glLinkProgram(p);

		PrintShaderInfoLog(v);
		PrintShaderInfoLog(f);
		PrintProgramInfoLog(p);

		return p;
	}

	char* GLApp::TextFileRead(const std::string& fileName) 
	{
		const char* fn = fileName.c_str(); 

		FILE *fp;
		char *content = NULL;
		int count = 0;

		if (fn != NULL) 
		{
			fp = fopen(fn,"rt");

			if (fp != NULL) 
			{
				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);

				if (count > 0) 
				{
					content = (char *)malloc(sizeof(char) * (count+1));
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		return content;
	}

	void GLApp::PrintShaderInfoLog(GLuint obj)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			GLUtils::DebugString(infoLog);
			free(infoLog);
		}
	}

	void GLApp::PrintProgramInfoLog(GLuint obj)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			GLUtils::DebugString(infoLog);
			free(infoLog);
		}
	}


	//Water Simulation Code
	/////////////////////////////////////////////////////////////////////
	// draw coordinate axes
	/////////////////////////////////////////////////////////////////////
	void GLApp::drawAxes()
	{
		glDisable(GL_COLOR_MATERIAL);
		//draw coordinate axes
		glPushMatrix();
		glTranslatef(-0.1f, -0.1f, -0.1f);
		glLineWidth(3.0f);
		glBegin(GL_LINES);
		//x axis is red
		glColor4f(10.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor4f(10.0f, 0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);

		//y axis is green 
		glColor4f(0.0f, 10.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor4f(0.0f, 10.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		//z axis is blue
		glColor4f(0.0f, 0.0f, 10.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor4f(0.0f, 0.0f, 10.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
		glLineWidth(1.0f);
		glPopMatrix();
	}

	///////////////////////////////////////////////////////////////////////////////
	// The drawing function
	///////////////////////////////////////////////////////////////////////////////
	void GLApp::displayCallback()
	{
		//glvu.BeginFrame();

		// clear away the previous frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		//drawAxes();

		// draw the particle system and walls
		particleSystem->draw();

		// swap the buffers
		//glutSwapBuffers();

		//glvu.EndFrame();
	}



}