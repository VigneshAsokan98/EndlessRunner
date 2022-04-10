#include "EnemyManager.h"
#include "Player.h"
#include "CatmullRom.h"
#include "Shaders.h"
#include "OpenAssetImportMesh.h"
#include "Game.h"
#include "Cubemap.h"
#include "Enemy.h"

CEnemyManager::CEnemyManager()
{
}

CEnemyManager::~CEnemyManager()
{
}

void CEnemyManager::Init(CCatmullRom* _spline, CPlayer* _player)
{
	m_SpawnTimer = 0.f;
	m_pSpline = _spline;
	m_player = _player;
	for (int i = 0; i < EnemyCount; i++)
	{
		Spawn(i);
	}
}
bool CEnemyManager::CheckBulletCollision(glm::vec3 bulletPosition)
{
	for (int i = 0; i < EnemyCount; i++)
	{
		float dis = glm::distance(bulletPosition, m_Enemies[i]->GetPosition());
		if (dis < 3.f)
		{
			m_Enemies[i]->Disable();
			return true;
		}
	}
	return false;
}
bool CEnemyManager::Update(double dt, glm::vec3 _playerPos)
{	
	for (int i = 0; i < EnemyCount; i++)
	{
		bool isHit = m_Enemies[i]->Update(dt, _playerPos);
		if (isHit)
			return true;
	}
	return false;
}

void CEnemyManager::Spawn(int idx)
{
	m_Enemies[idx] = new CEnemy;
	m_Enemies[idx]->Init(m_pSpline, EnemyType::Static);
}
void CEnemyManager::Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat)
{
	for (int i = 0; i < EnemyCount; i++)
	{
		m_Enemies[i]->Render(_modelViewMatrixStack,  _shader, _normalMatrix, _cameraPos, _viewMat);
	}
}
