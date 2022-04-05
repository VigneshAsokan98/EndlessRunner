#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"
#include "Cubemap.h"

// This is a class for creating and rendering a skybox
class CSkybox
{
public:
	CSkybox();
	~CSkybox();
	void Create(float size);
	void Render(int textureUnit);
	void Release();
	const CCubemap GetCubeMap() { return m_currentCubemap; };
	void SwitchTexture(int idx = 0);
private:
	UINT m_vao;
	CVertexBufferObject m_vbo;
	CCubemap m_cubemapTexture[2];
	CCubemap m_currentCubemap;
	
};