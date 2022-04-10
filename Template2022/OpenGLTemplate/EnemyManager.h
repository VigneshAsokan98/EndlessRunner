#pragma once
#pragma once
#include "Common.h"
#include "Player.h"
#include "MatrixStack.h"

class CCatmullRom;
class CShaderProgram;
class COpenAssetImportMesh;
class CCamera;
class CCubemap;
class CPlayer;
class CEnemy;

enum class EnemyType
{
	Static = 0,
	Shooter,
	PingPong
};

class CEnemyManager
{
public:
	CEnemyManager();
	~CEnemyManager();

	void Init(CCatmullRom* _spline, CPlayer* _player);
	bool Update(double dt, glm::vec3 _playerPos);
	void Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat);
	bool CheckBulletCollision(glm::vec3 bulletPosition);
private:
	void Spawn(int idx);
	CCatmullRom* m_pSpline;
	COpenAssetImportMesh* m_pModel;
	//CCubemap m_reflectionCubeMap;

	float m_currentDistance;
	glm::mat4 m_PlayerOrientation;
	glm::vec3 m_playerPosition;
	glm::mat3 m_TNB;
	Lane m_currentLane = Lane::Center;
	double m_SpawnTimer = 0.f;
	CPlayer* m_player;
	int EnemyCount = 25;
	CEnemy* m_Enemies[25];
};