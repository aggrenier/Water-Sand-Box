#include "stdafx.h"

#include "GLMesh.h"

#include "GLUtils.h"
#include "OBJLoader\objLoader.h"

#include <algorithm>

using namespace glm;
using namespace Utils;

namespace ord12929
{

GLMesh::GLMesh(std::string path, std::string file)
{
	m_NbOfTriangles.resize(1);
	m_NbOfTriangles[0] = -1;
	m_Index.resize(1);

	m_Path = path; 
	m_File = file;
}

void GLMesh::Init()
{
		objLoader objData;
		std::string objFile = m_Path + m_File;
		objData.load(const_cast<char*>(objFile.c_str()));

		std::vector<Vertex3D> dataVertex;

		std::vector< std::vector<unsigned short> > dataIndex;
		dataIndex.resize(objData.materialCount);
		m_NbOfTriangles.resize(objData.materialCount);
		m_Index.resize(objData.materialCount);

		for(int i=0; i<objData.materialCount; i++)
		{
			obj_material* mat = objData.materialList[i];
			std::string s = mat->texture_filename;
			s = m_Path + s;
			s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
			m_Tex.push_back(Utils::GLUtils::LoadTexture(s));
		}

		for(size_t i=0; i<objData.faceCount; i++)
		{
			obj_face* o = objData.faceList[i];
			if(o->vertex_count > 3)
			{
				GLUtils::DebugString("************ ERREUR : LE MODELE CONTIENT UN POLYGONE (TRIANGLE SEULEMENT !) **************");
				continue;
			}

			for(int j=0; j<o->vertex_count; j++)
			{
				size_t next = dataVertex.size();
				vec3 vertex;
				vertex[0] = (float)((objData.vertexList[o->vertex_index[j]])->e[0]);
				vertex[1] = (float)((objData.vertexList[o->vertex_index[j]])->e[1]);
				vertex[2] = (float)((objData.vertexList[o->vertex_index[j]])->e[2]);

				vec3 normal(0.f, 0.f, 0.f);
				if(o->normal_index[j] != -1)
				{
					normal[0] = (float)((objData.normalList[o->normal_index[j]])->e[0]);
					normal[1] = (float)((objData.normalList[o->normal_index[j]])->e[1]);
					normal[2] = (float)((objData.normalList[o->normal_index[j]])->e[2]);
				}

				vec2 texture(0.f, 0.f);
				if(o->texture_index[j] != -1)
				{
					texture[0] = (float)((objData.textureList[o->texture_index[j]])->e[0]);
					texture[1] = (float)((objData.textureList[o->texture_index[j]])->e[1]);
				}

				dataIndex[o->material_index].push_back((unsigned short)next);
				dataVertex.push_back(Vertex3D(vertex, normal, texture));
			}
		}

		glGenBuffers(1,&m_Vertex);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D)*dataVertex.size(), &dataVertex[0]._position, GL_STATIC_DRAW);

		for(int i=0; i<objData.materialCount; i++)
		{
			glGenBuffers(1,&m_Index[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*dataIndex[i].size(), &dataIndex[i][0], GL_STATIC_DRAW);
			m_NbOfTriangles[i] = dataIndex[i].size() / 3;
		}
}

void GLMesh::Render()
{		
	glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, m_Vertex);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), 0);

		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(Vertex3D), (void*)sizeof(vec3));

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), (void*)(2*sizeof(vec3))); 

		//for(int i=0; i<m_Index.size(); i++)
		for(int i=m_Index.size()-1; i>=0; i--)
		{
			glBindTexture(GL_TEXTURE_2D, m_Tex[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index[i]);
			glDrawElements(GL_TRIANGLES, 3*m_NbOfTriangles[i], GL_UNSIGNED_SHORT, 0);
		}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY); 
		glDisable(GL_TEXTURE_2D);
}

}

