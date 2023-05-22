#include "PlayerAttach.h"
#include "ModelManager.h"
#include "Helper.h"
#include "Player.h"
#include "CsvLoader.h"
#include <Easing.h>
PlayerAttach::PlayerAttach() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Sutopon);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//èâä˙âª
bool PlayerAttach::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed4")));
	return true;
}
//çXêV
void PlayerAttach::Update() {
	float l_AddAngle = 2.0f;

	m_Position.x += m_Angle.x * m_AddSpeed;
	m_Position.z += m_Angle.y * m_AddSpeed;

	m_SinAngle += l_AddAngle;
	m_SinAngle2 = m_SinAngle * (3.14f / 180.0f);
	m_Position.y = (sin(m_SinAngle2) * 1.0f + 1.0f);

	m_Angle = {
		Ease(In,Cubic,0.75f,m_Angle.x,m_AfterAngle.x),
		Ease(In,Cubic,0.75f,m_Angle.y,m_AfterAngle.y)
	};

	//m_Rotation.y = Ease(In, Cubic, 0.5f, m_Rotation.y, m_AfterRotY);
	Obj_SetParam();
}
//ï`âÊ
void PlayerAttach::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void PlayerAttach::ImGuiDraw() {
	ImGui::Begin("Attach");
	ImGui::Text("PosY:%f", m_Position.y);
	ImGui::End();
}