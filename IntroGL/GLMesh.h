#pragma once

#include <GL/gl.h>
#include "../glm/glm.hpp"

#include <vector>
#include <string>

namespace ord12929
{

class GLMesh
{
	struct Vertex3D
	{
		Vertex3D() : _position(glm::vec3()), _normal(glm::vec3()), _textureCoordinate(glm::vec2()) {}
		Vertex3D(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinate)
			: _position(position), _normal(normal), _textureCoordinate(textureCoordinate)
		{
		}

		glm::vec3 _position;
		glm::vec3 _normal;
		glm::vec2 _textureCoordinate;
	};

public:
	GLMesh(std::string path, std::string file);

	void Init();
	void Render();

	glm::vec3 GetPosition() {return m_Position;}
	void SetPosition(glm::vec3 pos) {m_Position = pos;}

private:
	GLuint m_Vertex;
	std::vector<GLuint> m_NbOfTriangles;
	std::vector<GLuint> m_Index;
	std::vector<GLuint> m_Tex;

	std::string m_Path; 
	std::string m_File;

	glm::vec3 m_Position;
};


}