#include "GhostBullet.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Easing.h"
#include "Helper.h"
GhostBullet::GhostBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::GhostBullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//‰Šú‰»
bool GhostBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.0f,0.0f,0.0f,1.0f };
	//CSV‚©‚ç“Ç‚İ‚İ
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "Timer")));
	return true;
}
//ImGui•`‰æ
void GhostBullet::ImGui_Origin() {
	ImGui::Begin("Ghost");
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::End();
}
//’e‚Ì“Á—Lˆ—
void GhostBullet::Action() {
	//ƒ^ƒCƒv‚É‚æ‚Á‚ÄF‚ğˆê’U•Ï‚¦‚Ä‚é
	if (m_BulletType == BULLET_FORROW) {
		m_Color.x = 1.0f;
	}
	else {
		m_Color.y = 1.0f;
	}
	if (m_Alive) {
		//ˆÚ“®‚ğ‰ÁZ
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		m_MatRot = m_Object->GetMatrot();
	}

	VanishBullet();

	if (m_Alive) {
		Obj_SetParam();
	}
}
//’e‚ª™X‚ÉÁ‚¦‚éˆ—
void GhostBullet::VanishBullet() {
	float l_AddFrame = 0.035f;
	if (m_Alive) {
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = {};
			m_Alive = false;
		}

		m_Color.w = Ease(In, Cubic, m_Frame, 1.0f, 0.0f);
		m_Scale = {
			Ease(In, Cubic, m_Frame, 1.0f, 2.0f),
			Ease(In, Cubic, m_Frame, 1.0f, 2.0f),
			Ease(In, Cubic, m_Frame, 1.0f, 2.0f)
		};
	}
}