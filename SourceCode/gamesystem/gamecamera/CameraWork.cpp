#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include  "imgui.h"
Player* CameraWork::player = nullptr;
CameraWork::CameraWork() {
	m_eyePos = { 2.0f,60.0f,2.0f };
	m_targetPos = { 2.0f,0.0f,3.0f };
	
	Shake* shake_ = new Shake();
	shake.reset(shake_);
	helper = make_unique< Helper> ();
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
//ImGui
void CameraWork::ImGuiDraw() {
	ImGui::Begin("Camera");
	ImGui::Text("eyeX:%f", m_eyePos.x);
	ImGui::Text("targetX:%f", m_targetPos.x);
	ImGui::End();
}
