#pragma once
#include "Common.h"
#include "Texture.h"
#include "vertexBufferObject.h"

class CCarShape {
public:
	CCarShape();
	~CCarShape();

	void Initialise();
	void Render();
	void CreateCar();
	void Release();
private:

	void CreateVBO(vector<glm::vec3> points, vector<glm::vec2> texCoord);

	CTexture m_BodyTexture;
	CTexture m_WindowTexture;
	CTexture m_TopTexture;

public:

	static glm::vec3 _dummy_vector;

	GLuint m_vaoBase;
	GLuint m_vaoTop;
	GLuint m_vaoSide;
	vector<glm::vec3> m_vertexPointsBase;
	vector<glm::vec3> m_vertexPointsTop;
	vector<glm::vec3> m_vertexPointsSide;
	vector<glm::vec2> m_TexCoordsBase;
	vector<glm::vec2> m_TexCoordsTop;
	vector<glm::vec2> m_TexCoordsSide;


};
