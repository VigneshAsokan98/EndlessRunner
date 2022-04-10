#pragma once
#include"EnemyManager.h"
#include"common.h"
#include "CatmullRom.h"
#include "Shaders.h"
#include "CarShape.h"
#include "Player.h"
#include "Bullet.h"

class CEnemy
{
public:
	CEnemy() {};
	~CEnemy() {};
	void Init(CCatmullRom* _spline, EnemyType type);
	void Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat);
	bool Update(double dt, glm::vec3 _playerPos);

	const glm::vec3 GetPosition() { return m_Position; }

	void Disable() { isDisabled = true; }
private:

	void handleCollision(glm::vec3 _playerPos);
	EnemyType m_type = EnemyType::Static;
	glm::vec3 m_Position;
	glm::mat4 m_Orientation;
	Lane m_currentLane;
	CCatmullRom* m_pSpline;
	glm::mat3 TNB;
	CCarShape* m_carmodel;

	float RespawnTimer=0.f;
	bool isDisabled = false;
	float m_currentDistance;
	float m_speed;
	bool isHit;
};
