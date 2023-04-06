#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include  "imgui.h"
CameraWork::CameraWork() {
	m_eyePos = { 0.0f,40.0f,-3.0f };
	m_targetPos = { 0.0f,0.0f,0.0f };
	
	Shake* shake_ = new Shake();
	shake.reset(shake_);
	helper = make_unique< Helper> ();
}
//XV
void CameraWork::Update(DebugCamera* camera) {
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
