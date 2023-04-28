#include "GhostBullet.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Helper.h"
GhostBullet::GhostBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//èâä˙âª
bool GhostBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.5f,0.5f,0.5f };

	//CSVÇ©ÇÁì«Ç›çûÇ›
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "Timer")));
	return true;
}

//ImGuiï`âÊ
void GhostBullet::ImGui_Origin() {
	ImGui::Begin("Bullet");
	ImGui::Text("Pos.X:%f", m_Position.x);
	ImGui::Text("Timer:%d", m_Timer);
	ImGui::End();
}
//íeÇÃì¡óLèàóù
void GhostBullet::Action() {
	//É^ÉCÉvÇ…ÇÊÇ¡ÇƒêFÇàÍíUïœÇ¶ÇƒÇÈ
	if (m_BulletType == BULLET_FORROW) {
		m_Color = { 1.0f,0.0f,0.0f,1.0f };
	}
	else {
		m_Color = { 0.0f,1.0f,0.0f,1.0f };
	}
	if (m_Alive) {
		//à⁄ìÆÇâ¡éZ
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		//àÍíËéûä‘óßÇ¬Ç∆è¡Ç¶ÇÈ
		if (Helper::GetInstance()->CheckMinINT(m_Timer,m_TargetTimer,1)) {
			m_Timer = 0;
			m_Alive = false;
		}
	}

	if (m_Alive) {
		Obj_SetParam();
	}
}