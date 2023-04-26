#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include  "imgui.h"
Player* CameraWork::player = nullptr;
CameraWork::CameraWork(XMFLOAT3 eye, XMFLOAT3 target) {
	m_eyePos = eye;
	m_targetPos = target;
	
	Shake* shake_ = new Shake();
	shake.reset(shake_);
}
//XV
void CameraWork::Update(DebugCamera* camera) {
	m_eyePos.x = player->GetPosition().x;
	m_eyePos.z = player->GetPosition().z - 2.0f;
	m_targetPos.x = player->GetPosition().x;
	m_targetPos.z = player->GetPosition().z;
	camera->SetEye(m_eyePos);
	camera->SetTarget(m_targetPos);
	camera->Update();
}

void CameraWork::EditorCamera()
{
	m_eyePos.y = 30.f;
	m_targetPos.z= m_eyePos.z+30.0f;
	m_targetPos.x = m_eyePos.x;
}

//ImGui
void CameraWork::ImGuiDraw() {
	ImGui::Begin("Camera");
	ImGui::Text("eyeX:%f", m_eyePos.x);
	ImGui::SliderFloat("PosX", &m_eyePos.x, -200.f, 200.f);
	ImGui::SliderFloat("PosY", &m_eyePos.y, 0.f, 20.f);
	ImGui::SliderFloat("PosZ", &m_eyePos.z, -200.f, 200.f);
	ImGui::Text("targetX:%f", m_targetPos.x);
	ImGui::SliderFloat("tPosX", &m_targetPos.x, -200.f, 200.f);
	ImGui::SliderFloat("tPosY", &m_targetPos.y, 0.0f, 20.0f);
	ImGui::SliderFloat("tPosZ", &m_targetPos.z, -200.0f, 200.0f);
	ImGui::End();
}

void CameraWork::LoadActorUpdate(DebugCamera* camera) {
	camera->SetEye(m_eyePos);
	camera->SetTarget(m_targetPos);
	camera->Update();
}
