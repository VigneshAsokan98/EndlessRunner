#pragma once
#include "Common.h"
#include "MatrixStack.h"

class CCatmullRom;
class CShaderProgram;
class COpenAssetImportMesh;
class CCamera;
class CCubemap;

enum class Lane {
	Right = -1,
	Center,
	Left
};

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	void Init(CCatmullRom* _spline);
	void Update(double dt);
	void MovePlayer(Lane _lane);
	void Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat);
	const glm::vec3 GetPosition() { return m_playerPosition; };
	const glm::mat3 GetTNBFrame() { return m_TNB; };
	const float GetDistance() { return m_currentDistance; }
	const int GetLaps() { return m_Laps; }
	const int GetLane() { return (int)m_currentLane; }
private:

	CCatmullRom *m_pSpline;
	COpenAssetImportMesh* m_pModel;


	float m_currentDistance;
	glm::mat4 m_PlayerOrientation;
	glm::vec3 m_playerPosition;
	glm::mat3 m_TNB;
	Lane m_currentLane = Lane::Center;
	float m_timer = 0.f;
	float m_playerSpeed = 0.15f;
	int m_Laps = 0.f;
};