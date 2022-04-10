#pragma once
#include "Player.h"
#include "CatmullRom.h"
#include "Shaders.h"
#include "Game.h"
#include "Pentagon_pyramid.h"

class CBullet
{
public:
	CBullet() {}
	~CBullet() {}

	void Init(CCatmullRom *_spline);
	void Update(double dt);
	void Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat);
	void Fire(float playerDistance, int lane);
	const glm::vec3 GetPosition() { return m_position; }
	void Disable();
private:

	bool isDisabled = false;
	CCatmullRom* m_spline;
	float m_distance;
	bool isFired = false;
	glm::vec3 m_position;
	glm::mat4 m_Orientation;
	CPentagonPyramid* m_pModel;
	float m_resetTimer = 0.f;
	int m_lane;
};
