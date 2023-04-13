#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include  "imgui.h"
CameraWork::CameraWork() {
	m_eyePos = { 2.0f,15.0f,2.0f };
	m_targetPos = { 2.0f,0.0f,3.0f };
	
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
	ImGui::SliderFloat("PosX", &m_targetPos.x, -200.f, 200.f);
	ImGui::SliderFloat("tPosY", &m_targetPos.y, 0.0f, 20.0f);
	ImGui::SliderFloat("tPosZ", &m_targetPos.z, -200.0f, 200.0f);
	ImGui::End();
}
