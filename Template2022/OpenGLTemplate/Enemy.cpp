#include"Enemy.h"
#include "Shaders.h"

void CEnemy::Init(CCatmullRom* _spline, EnemyType type)
{
	m_type = type;
	m_pSpline = _spline;
	int min = 25.f, max = 10000.f;
	m_currentLane = (Lane)((((rand() % (max - min)) + min) % 3) - 1);
	m_carmodel = new CCarShape;
	m_carmodel->CreateCar();

	RespawnTimer = 0.f;
	isDisabled = false;
	
	m_speed = 0.4f + (float)(rand()) / ((float)(RAND_MAX / (0.08f - 0.04f)));
	
	m_currentDistance = (float)((rand() % (max - min)) + min);
}
void CEnemy::handleCollision(glm::vec3 _playerPos)
{
	float dis = glm::distance(_playerPos, m_Position);
	if (dis < 3.f)
		isHit = true;
}

void CEnemy::Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat)
{
	if (isDisabled)
		return;

	_modelViewMatrixStack.Push();
	_modelViewMatrixStack.Translate(m_Position);
	_modelViewMatrixStack.ApplyMatrix(m_Orientation);
	_modelViewMatrixStack.Scale(1.0f);
	_shader->SetUniform("matrices.modelViewMatrix", _modelViewMatrixStack.Top());
	_shader->SetUniform("matrices.normalMatrix", _normalMatrix);
	_shader->SetUniform("ReflectFactor", 0.5f);
	_shader->SetUniform("WorldCameraPosition", _cameraPos);
	glm::mat4 inverseViewMatrix = glm::inverse(_viewMat);
	_shader->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
	m_carmodel->Render();
	_modelViewMatrixStack.Pop();
}

bool CEnemy::Update(double dt, glm::vec3 _playerPos)
{
	if (isDisabled)
	{
		RespawnTimer += 0.01f;
		if (RespawnTimer >= 5.f)
			isDisabled = false;

		return false;
	}
	m_currentDistance += dt * m_speed * 0.1f;
	glm::vec3 yAxis = glm::vec3(0, 1, 0);

	glm::vec3 pNext;
	m_pSpline->Sample(m_currentDistance, m_Position);
	m_pSpline->Sample(m_currentDistance + 1.f, pNext);
	glm::vec3 T = glm::normalize(pNext - m_Position);
	glm::vec3 N = glm::normalize(glm::cross(T, yAxis));
	glm::vec3 B = glm::normalize(glm::cross(N, T));

	m_Position -= N * (float)m_currentLane * 10.f;
	m_Orientation = glm::mat4(glm::mat3(N, B, -T));

	handleCollision(_playerPos);
	return isHit;
}
