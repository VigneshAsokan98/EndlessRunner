#include "Bullet.h"

void CBullet::Init(CCatmullRom *_spline)
{
	m_spline = _spline;
	m_pModel = new CPentagonPyramid;
	m_pModel->Create();
}

void CBullet::Disable()
{
	m_position = glm::vec3(0.0f);
	isDisabled = true;
}
void CBullet::Update(double dt)
{
	if (!isFired || isDisabled)
	{
		//m_position = glm::vec3(900.f);
		return;
	}

	m_distance += dt * .3f;
	glm::vec3 pNext;
	m_spline->Sample(m_distance, m_position);
	m_spline->Sample(m_distance + 1, pNext);

	//Calculate TNB
	glm::vec3 yAxis = glm::vec3(0, 1, 0);
	glm::vec3 T = glm::normalize(pNext - m_position);
	glm::vec3 N = glm::normalize(glm::cross(T, yAxis));
	glm::vec3 B = glm::normalize(glm::cross(N, T));

	m_Orientation = glm::mat4(glm::mat3(N, B, -T));
	m_position -= N * (float)m_lane * 10.f;

	m_resetTimer += dt;
}

void CBullet::Render(glutil::MatrixStack& _modelViewMatrixStack, CShaderProgram* _shader, glm::mat3 _normalMatrix, glm::vec3 _cameraPos, glm::mat4 _viewMat)
{
	if (!isFired || isDisabled)
		return;

	_modelViewMatrixStack.Push();
	_modelViewMatrixStack.Translate(m_position);
	_modelViewMatrixStack.ApplyMatrix(m_Orientation);
	_modelViewMatrixStack.Scale(1.0f);
	_shader->SetUniform("matrices.modelViewMatrix", _modelViewMatrixStack.Top());
	_shader->SetUniform("matrices.normalMatrix", _normalMatrix);
	_shader->SetUniform("ReflectFactor", 0.65f);
	_shader->SetUniform("WorldCameraPosition", _cameraPos);
	glm::mat4 inverseViewMatrix = glm::inverse(_viewMat);
	_shader->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	m_pModel->Render();
	_modelViewMatrixStack.Pop();
}

void CBullet::Fire(float playerDistance, int lane)
{
	m_distance = playerDistance;
	m_lane = lane;
	isFired = true;
	isDisabled = false;
	m_resetTimer = 0.f;
}