#pragma once

#include <windows.h>
#include <GL/gl.h>
#include "..\glm\glm.hpp"

#include <string>

namespace Utils 
{

	class GLUtils
	{
	public:
		static GLint ActivateContext(HDC hDC);

		static void DebugString(const std::string& s);
		static void DebugString(int i);
		static void DebugString(float f);

		static float Deg2Rad(float in);
		static float Rad2Deg(float in);

		// GLM
		static void DumpMatrix(const glm::mat4& m);
		static void DumpVector(const glm::vec3& v);
		static void DumpVector(const glm::vec4& v);

		//SOIL
		static GLuint LoadTexture(const std::string& textureFileName);
	};
}