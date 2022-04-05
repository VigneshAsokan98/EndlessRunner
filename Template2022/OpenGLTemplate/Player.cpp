#include "Player.h"
#include "CatmullRom.h"
#include "Shaders.h"
#include "OpenAssetImportMesh.h"
#include "Game.h"
#include "Cubemap.h"

CPlayer::CPlayer()
{

}
CPlayer::~CPlayer()
{

}
void CPlayer::Init(CCatmullRom *_spline, CCubemap _reflectionMap)
{
	m_pSpline = _spline;
	
	//m_reflectionCubeMap = _reflectionMap;

	m_pModel = new COpenAssetImportMesh;
	m_pModel->Load("resources\\models\\Player\\sls_amg.obj");


}

void CPlayer::Update(double dt)
{
	m_timer += dt;
	m_currentDistance += dt * m_playerSpeed;
	glm::vec3 xAxis = glm::vec3(1, 0, 0);
	glm::vec3 yAxis = glm::vec3(0, 1, 0);
	glm::vec3 zAxis = glm::vec3(0, 0, 1);

	glm::vec3 p, pNext;
	m_pSpline->Sample(m_currentDistance, m_playerPosition, (float)m_currentLane);
	m_pSpline->Sample(m_currentDistance + 1.f, pNext, (float)m_currentLane);
	glm::vec3 T = glm::normalize(pNext - p);
	glm::vec3 N = glm::normalize(glm::cross(T, yAxis));
	glm::vec3 B = glm::normalize(glm::cross(N, T)); 

	m_TNB = glm::mat3(T, N, B);
	m_PlayerOrientation = glm::mat4(glm::mat3(T, B, N));

	UpdateLane();
}
void CPlayer::UpdateLane()
{
	/*if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
			MovePlayer(PlayerLane::Left);
	}

	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
			MovePlayer(PlayerLane::Right);
	}

	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		MovePlayer(PlayerLane::Center);
	}*/
}
void CPlayer::MovePlayer(PlayerLane _lane)
{
	if (m_currentLane == _lane)
		return;

	if (m_currentLane == PlayerLane::Center)
		m_currentLane = _lane;
	else
		m_currentLane = PlayerLane::Center;

	m_timer = 0.f;
}
void CPlayer::Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat)
{
	_modelViewMatrixStack.Push();
	_modelViewMatrixStack.Translate(m_playerPosition);
	_modelViewMatrixStack *= m_PlayerOrientation;
	_modelViewMatrixStack.Scale(1.0f);
	_shader->SetUniform("matrices.modelViewMatrix", _modelViewMatrixStack.Top());
	_shader->SetUniform("matrices.normalMatrix", _normalMatrix);
	_shader->SetUniform("ReflectFactor", 0.5f);
	_shader->SetUniform("WorldCameraPosition", _cameraPos);
	glm::mat4 inverseViewMatrix = glm::inverse(_viewMat);
	_shader->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	m_pModel->Render();
	_modelViewMatrixStack.Pop();
}