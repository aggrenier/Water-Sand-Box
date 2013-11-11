#include "StdAfx.h"
#include "GLUtils.h"
#include "..\glm\gtc\type_ptr.hpp"
#include "..\SOIL\SOIL.h"

#include <string>
#include <sstream>

namespace Utils
{
	GLint GLUtils::ActivateContext(HDC hDC)
	{
		static	PIXELFORMATDESCRIPTOR pfd=				// pfd décrit a Windows notre format de pixel
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Taille
			1,											// Version
			PFD_DRAW_TO_WINDOW |						// Le format doit Supporter Window
			PFD_SUPPORT_OPENGL |						// Le format doit Supporter OpenGL
			PFD_DOUBLEBUFFER,							// Le format doit Supporter le tampon double
			PFD_TYPE_RGBA,								// Nous voulons un format RGBA
			16,										
			0, 0, 0, 0, 0, 0,						
			0,										
			0,										
			0,											
			0, 0, 0, 0,								
			16,										
			0,										
			0,											
			PFD_MAIN_PLANE,							
			0,										
			0, 0, 0									
		};
			
		GLint  iPixelFormat; 
		 
		// On obtient le format le plus près disponible avec le matériel courant.
		if((iPixelFormat = ChoosePixelFormat(hDC, &pfd)) == 0)
		{
			//ChoosePixelFormat Failed
			return 1;
		}
			 
		// on l'applique
		if(SetPixelFormat(hDC, iPixelFormat, &pfd) == FALSE)
		{
			//SetPixelFormat Failed
			return 2;
		}

		HGLRC hglrc = 0;
		// création d'un contexte OpenGL
		if((hglrc = wglCreateContext(hDC)) == NULL)
		{
			//wglCreateContext Failed
			return 3;
		}

		// on active le contexte
		if((wglMakeCurrent(hDC, hglrc)) == NULL)
		{
			//wglMakeCurrent Failed
			return 4;
		}

		return 0;
	}

	void GLUtils::DebugString(const std::string& s)
	{
		std::string tmp = s + "\n";
		std::wstring str(tmp.begin(), tmp.end());
		OutputDebugStringW(str.c_str());
	}

	void GLUtils::DebugString(int i)
	{
		std::stringstream ss;
		ss << i;
		DebugString(ss.str());
	}

	void GLUtils::DebugString(float f)
	{
		std::stringstream ss;
		ss << f;
		DebugString(ss.str());
	}

	float GLUtils::Deg2Rad(float in)
	{
		return in * 0.0174532925f;
	}

	float GLUtils::Rad2Deg(float in)
	{
		return in * 57.2957795f;
	}

	void GLUtils::DumpMatrix(const glm::mat4& m) 
	{
		std::string out = "matrix : ";
		glm::mat4 m2 = glm::transpose(m);
		for (int i = 0; i < 16; ++i)
		{
			out += glm::value_ptr(m2)[i];
			out += ((i+1)%4 == 0) ? "\n         " : ", ";
		}
		DebugString(out);
	}

	void GLUtils::DumpVector(const glm::vec3& v) 
	{
		std::string out = "vector : ";
		out += v[0];
		out += ", ";
		out += v[1];
		out += ", ";
		out += v[2];
		DebugString(out);
	}

		void GLUtils::DumpVector(const glm::vec4& v) 
	{
		std::string out = "vector : ";
		out += v[0];
		out += ", ";
		out += v[1];
		out += ", ";
		out += v[2];
		out += ", ";
		out += v[3];
		DebugString(out);
	}

	GLuint GLUtils::LoadTexture(const std::string& textureFileName)
	{
		GLuint id =  SOIL_load_OGL_texture(
				textureFileName.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

		if(id == 0)
		{
			GLUtils::DebugString("Erreur au chargement de la texture.");
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return id;
	}
}