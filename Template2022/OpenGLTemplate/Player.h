#pragma once
#include "Common.h"
#include "MatrixStack.h"

class CCatmullRom;
class CShaderProgram;
class COpenAssetImportMesh;
class CCamera;
enum class PlayerLane {
	Right = -1,
	Center,
	Left
};

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	void Init(CCatmullRom *_spline);
	void Update(double dt);
	void UpdateLane();
	void MovePlayer(PlayerLane _lane);
	void Render(glutil::MatrixStack &_modelViewMatrixStack, CShaderProgram *_shader, glm::mat3 _normalMatrix);
	const glm::vec3 GetPosition() { return m_playerPosition; };
	const glm::mat3 GetTNBFrame() { return m_TNB; };
private:

	CCatmullRom *m_pSpline;
	COpenAssetImportMesh* m_pModel;
	float m_currentDistance;
	glm::mat4 m_PlayerOrientation;
	glm::vec3 m_playerPosition;
	glm::mat3 m_TNB;
	PlayerLane m_currentLane = PlayerLane::Center;
	float m_timer = 0.f;
};