#include "Pentagon_pyramid.h"

CPentagonPyramid::CPentagonPyramid()
{
}

CPentagonPyramid::~CPentagonPyramid()
{
}

void CPentagonPyramid::Init()
{
	m_Texture.Load("resources\\textures\\pyramid.png");
	m_Texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_Texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_vertices.push_back(glm::vec3(.5f, 0.5f, 1.f));			//0
	m_vertices.push_back(glm::vec3(.75f, 0.f, 1.f));		//1
	m_vertices.push_back(glm::vec3(.25f, 0.f, 1.f));		//2
	m_vertices.push_back(glm::vec3(0.f, .5f, 1.f));			//3
	m_vertices.push_back(glm::vec3(.5f, 1.f, 1.f));			//4
	m_vertices.push_back(glm::vec3(1.f, .5f, 1.f));			//5
	m_vertices.push_back(glm::vec3(1.5f, .5f, -1.f));		//6
	m_vertices.push_back(glm::vec3(1.25f, -0.5f, -1.f));		//7
	m_vertices.push_back(glm::vec3(-0.25f, -0.5f, -1.f));		//8
	m_vertices.push_back(glm::vec3(-0.5f, .5f, -1.f));		//9
	m_vertices.push_back(glm::vec3(.5f, 1.5f, -1.f));		//10
	m_vertices.push_back(glm::vec3(.5f, .5f, -1.f));		//11

	m_indices.push_back(0);
	m_indices.push_back(2);
	m_indices.push_back(1);
	m_indices.push_back(0);
	m_indices.push_back(3);
	m_indices.push_back(2);
	m_indices.push_back(0);
	m_indices.push_back(4);
	m_indices.push_back(3);
	m_indices.push_back(0);
	m_indices.push_back(5);
	m_indices.push_back(4);
	m_indices.push_back(0);
	m_indices.push_back(1);
	m_indices.push_back(5);
	m_indices.push_back(5);
	m_indices.push_back(6);
	m_indices.push_back(10);
	m_indices.push_back(5);
	m_indices.push_back(10);
	m_indices.push_back(4);
	m_indices.push_back(4);
	m_indices.push_back(10);
	m_indices.push_back(9);
	m_indices.push_back(4);
	m_indices.push_back(9);
	m_indices.push_back(3);
	m_indices.push_back(2);
	m_indices.push_back(9);
	m_indices.push_back(8);
	m_indices.push_back(2);
	m_indices.push_back(3);
	m_indices.push_back(9);
	m_indices.push_back(1);
	m_indices.push_back(8);
	m_indices.push_back(7);
	m_indices.push_back(1);
	m_indices.push_back(2);
	m_indices.push_back(8);
	m_indices.push_back(5);
	m_indices.push_back(7);
	m_indices.push_back(6);
	m_indices.push_back(5);
	m_indices.push_back(1);
	m_indices.push_back(7);
	m_indices.push_back(11);
	m_indices.push_back(7);
	m_indices.push_back(8);
	m_indices.push_back(11);
	m_indices.push_back(8);
	m_indices.push_back(9);
	m_indices.push_back(11);
	m_indices.push_back(9);
	m_indices.push_back(10);
	m_indices.push_back(11);
	m_indices.push_back(10);
	m_indices.push_back(6);
	m_indices.push_back(11);
	m_indices.push_back(6);
	m_indices.push_back(7);

	m_TexCoord.push_back(glm::vec2(0.5f, 0.5f));
	m_TexCoord.push_back(glm::vec2(0.2f, 0.03f));
	m_TexCoord.push_back(glm::vec2(0.8f, 0.03f));
	m_TexCoord.push_back(glm::vec2(1.0f, 0.605f));
	m_TexCoord.push_back(glm::vec2(0.5f, 0.97f));
	m_TexCoord.push_back(glm::vec2(0.0f, 0.605f));
	m_TexCoord.push_back(glm::vec2(0.2f, 0.03f));
	m_TexCoord.push_back(glm::vec2(0.8f, 0.03f));
	m_TexCoord.push_back(glm::vec2(1.0f, 0.605f));
	m_TexCoord.push_back(glm::vec2(0.5f, 0.97f));
	m_TexCoord.push_back(glm::vec2(0.0f, 0.605f));
	m_TexCoord.push_back(glm::vec2(0.5f, 0.5f));
}

void CPentagonPyramid::Create()
{
	Init();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	CreateVBO(m_vertices, m_TexCoord);
}
void CPentagonPyramid::CreateVBO(vector<glm::vec3> points, vector<glm::vec2> texCoords)
{
	CVertexBufferObjectIndexed vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec3 Color(1.0f, 0.0f, 0.0f);
	int vectorSize = points.size();
	for (unsigned int i = 0; i < vectorSize; i++)
	{
		int iNext = (i + 1) % vectorSize;
		glm::vec3 T = glm::normalize(points[iNext] - points[i]);
		glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
		glm::vec3 B = glm::normalize(glm::cross(N, T));

		vbo.AddVertexData(&points[i], sizeof(glm::vec3));
		vbo.AddVertexData(&texCoords[i], sizeof(glm::vec2));
		vbo.AddVertexData(&B, sizeof(glm::vec3));
		vbo.AddVertexData(&Color, sizeof(glm::vec3));
	}
	for (int idx = 0; idx < m_indices.size(); idx++)
	{
		vbo.AddIndexData(&m_indices[idx], sizeof(unsigned int));
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 3 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// Color vectors
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

}
void CPentagonPyramid::Render()
{
	m_Texture.Bind();
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}
