
#pragma once

#include "Common.h"
#include "Texture.h"
#include "VertexBufferObjectIndexed.h"

class CPentagonPyramid
{
public:
	CPentagonPyramid();
	~CPentagonPyramid();
	
	
private:
	void Init();
	void CreateVBO(vector<glm::vec3> vertices, vector<glm::vec2> texCoords);

	CTexture m_Texture;
public:
	void Create();
	void Render();

	GLuint m_vao;

	vector<glm::vec3> m_vertices;
	vector<glm::vec2> m_TexCoord;
	vector<unsigned int> m_indices;


};