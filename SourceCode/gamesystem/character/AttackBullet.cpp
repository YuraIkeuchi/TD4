#include "AttackBullet.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Helper.h"
AttackBullet::AttackBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//‰Šú‰»
bool AttackBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	//CSV‚©‚ç“Ç‚İ‚İ
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "Timer")));
	return true;
}
//ImGui•`‰æ
void AttackBullet::ImGui_Origin() {
	ImGui::Begin("Attack");
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::End();
}
//’e‚Ì“Á—Lˆ—
void AttackBullet::Action() {
	if (m_Alive) {
		//ˆÚ“®‚ğ‰ÁZ
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		//ˆê’èŠÔ—§‚Â‚ÆÁ‚¦‚é
		if (Helper::GetInstance()->CheckMinINT(m_Timer, m_TargetTimer, 1)) {
			m_Timer = 0;
			m_Alive = false;
		}

		m_MatRot = m_Object->GetMatrot();
	}

	if (m_Alive) {
		Obj_SetParam();
	}
}