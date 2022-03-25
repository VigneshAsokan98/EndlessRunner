#include "CarShape.h"


CCarShape::CCarShape() 
{
	m_vertexPointsBase.clear();
	m_vertexPointsTop.clear();
	m_vertexPointsSide.clear();
}

CCarShape::~CCarShape() 
{
	Release();
}

void CCarShape::Initialise()
{
	m_BodyTexture.Load("resources\\textures\\carBase.jpg");
	m_BodyTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_BodyTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_BodyTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_BodyTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_WindowTexture.Load("resources\\textures\\carWindows.jpg");
	m_WindowTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_WindowTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_WindowTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_WindowTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_TopTexture.Load("resources\\textures\\carTop.jpg");
	m_TopTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_TopTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_TopTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_TopTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set Side Vertex Points
	m_vertexPointsBase.push_back(glm::vec3(0, 1, 0));
	m_vertexPointsBase.push_back(glm::vec3(0, 0, 0));
	m_vertexPointsBase.push_back(glm::vec3(0, 1, 5));
	m_vertexPointsBase.push_back(glm::vec3(0, 0, 5));
	m_vertexPointsBase.push_back(glm::vec3(3, 1, 5));
	m_vertexPointsBase.push_back(glm::vec3(3, 0, 5));
	m_vertexPointsBase.push_back(glm::vec3(3, 1, 0));
	m_vertexPointsBase.push_back(glm::vec3(3, 0, 0));
	m_vertexPointsBase.push_back(glm::vec3(0, 1, 0));
	m_vertexPointsBase.push_back(glm::vec3(0, 0, 0));

	//Set Top Vertex Points
	m_vertexPointsTop.push_back(glm::vec3(3, 1, 0));
	m_vertexPointsTop.push_back(glm::vec3(0, 1, 0));
	m_vertexPointsTop.push_back(glm::vec3(3, 1, 2));
	m_vertexPointsTop.push_back(glm::vec3(0, 1, 2));
	m_vertexPointsTop.push_back(glm::vec3(3, 2, 3));
	m_vertexPointsTop.push_back(glm::vec3(0, 2, 3));
	m_vertexPointsTop.push_back(glm::vec3(3, 2, 4.5));
	m_vertexPointsTop.push_back(glm::vec3(0, 2, 4.5));
	m_vertexPointsTop.push_back(glm::vec3(3, 1, 5));
	m_vertexPointsTop.push_back(glm::vec3(0, 1, 5));


	//Set Windows Vertex Points
	m_vertexPointsSide.push_back(glm::vec3(0, 2, 3));
	m_vertexPointsSide.push_back(glm::vec3(0, 1, 2));
	m_vertexPointsSide.push_back(glm::vec3(0, 2, 4.5));
	m_vertexPointsSide.push_back(glm::vec3(0, 1, 5));
	m_vertexPointsSide.push_back(glm::vec3(3, 2, 4.5));
	m_vertexPointsSide.push_back(glm::vec3(3, 1, 5));
	m_vertexPointsSide.push_back(glm::vec3(3, 2, 3));
	m_vertexPointsSide.push_back(glm::vec3(3, 1, 2));


	//Set TexCoords 
	m_TexCoordsBase.push_back(glm::vec2(0, 0));
	m_TexCoordsBase.push_back(glm::vec2(0, 1));
	m_TexCoordsBase.push_back(glm::vec2(0.25, 0));
	m_TexCoordsBase.push_back(glm::vec2(0.25f, 1));
	m_TexCoordsBase.push_back(glm::vec2(0.5f, 0));
	m_TexCoordsBase.push_back(glm::vec2(0.5f, 1));
	m_TexCoordsBase.push_back(glm::vec2(.75f, 0));
	m_TexCoordsBase.push_back(glm::vec2(0.75f, 1));
	m_TexCoordsBase.push_back(glm::vec2(1, 0));
	m_TexCoordsBase.push_back(glm::vec2(1, 1));

	m_TexCoordsTop.push_back(glm::vec2(0, 0));
	m_TexCoordsTop.push_back(glm::vec2(0, 1));
	m_TexCoordsTop.push_back(glm::vec2(0.25f, 0));
	m_TexCoordsTop.push_back(glm::vec2(0.25f, 1));
	m_TexCoordsTop.push_back(glm::vec2(0.5f, 0));
	m_TexCoordsTop.push_back(glm::vec2(0.5f, 1));
	m_TexCoordsTop.push_back(glm::vec2(.75f, 0));
	m_TexCoordsTop.push_back(glm::vec2(0.75f, 1));
	m_TexCoordsTop.push_back(glm::vec2(1, 0));
	m_TexCoordsTop.push_back(glm::vec2(1, 1));

	m_TexCoordsSide.push_back(glm::vec2(0, 0));
	m_TexCoordsSide.push_back(glm::vec2(0, 1));
	m_TexCoordsSide.push_back(glm::vec2((float) 1/3, 0));
	m_TexCoordsSide.push_back(glm::vec2((float) 1/3, 1));
	m_TexCoordsSide.push_back(glm::vec2((float) 2/3, 0));
	m_TexCoordsSide.push_back(glm::vec2((float) 2/3, 1));
	m_TexCoordsSide.push_back(glm::vec2(1, 0));
	m_TexCoordsSide.push_back(glm::vec2(1, 1));

}

void CCarShape::CreateCar()
{
	Initialise();

	glGenVertexArrays(1, &m_vaoBase);
	glBindVertexArray(m_vaoBase);

	CreateVBO(m_vertexPointsBase, m_TexCoordsBase);


	glGenVertexArrays(1, &m_vaoTop);
	glBindVertexArray(m_vaoTop);

	CreateVBO(m_vertexPointsTop, m_TexCoordsTop);

	glGenVertexArrays(1, &m_vaoSide);
	glBindVertexArray(m_vaoSide);

	CreateVBO(m_vertexPointsSide, m_TexCoordsSide);

}

void CCarShape::Render()
{
	m_BodyTexture.Bind();
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoBase);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

	m_TopTexture.Bind();
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoTop);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);

	m_WindowTexture.Bind();
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoSide);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

}

void CCarShape::CreateVBO(vector<glm::vec3> points, vector<glm::vec2> texCoord)
{
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec3 Color(0.0f, 1.0f, 0.0f);
	int vectorSize = points.size();
	int quads = ((vectorSize / 2) - 1);
	for (unsigned int i = 0; i < vectorSize; i++) 
	{
			int iNext = (i + 1) % vectorSize;
			glm::vec3 T = glm::normalize(points[iNext] - points[i]);
			glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
			glm::vec3 B = glm::normalize(glm::cross(N, T));

			vbo.AddData(&points[i], sizeof(glm::vec3));
			vbo.AddData(&texCoord[i], sizeof(glm::vec2));
			vbo.AddData(&B, sizeof(glm::vec3));
			vbo.AddData(&Color, sizeof(glm::vec3));
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
void CCarShape::Release()
{
	m_BodyTexture.Release();
	glDeleteVertexArrays(1, &m_vaoBase);
	glDeleteVertexArrays(1, &m_vaoSide);
	glDeleteVertexArrays(1, &m_vaoTop);
}



glm::vec3 CCarShape::_dummy_vector(0.0f, 0.0f, 0.0f);