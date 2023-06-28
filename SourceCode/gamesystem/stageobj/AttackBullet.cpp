#include "AttackBullet.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Helper.h"
AttackBullet::AttackBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//èâä˙âª
bool AttackBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	//CSVÇ©ÇÁì«Ç›çûÇ›
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet/bullet.csv", "AttackSpeed")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet/bullet.csv", "AttackLimit")));
	return true;
}
//ImGuiï`âÊ
void AttackBullet::ImGui_Origin() {

}
//íeÇÃì¡óLèàóù
void AttackBullet::Action() {
	if (m_Alive) {
		if (m_PowerState == POWER_NONE) {
			m_Power = 1.0f;
			m_Color = { 1.0f,1.0f,1.0f,1.0f };
		}
		else if (m_PowerState == POWER_MIDDLE) {
			m_Power = 2.0f;
			m_Color = { 1.0f,0.0f,0.0f,1.0f };
		}
		else if (m_PowerState == POWER_STRONG) {
			m_Power = 3.0f;
			m_Color = { 0.0f,1.0f,0.0f,1.0f };
		}
		else {
			m_Power = 5.0f;
			m_Color = { 1.0f,0.0f,0.0f,1.0f };
		}
		//à⁄ìÆÇâ¡éZ
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		//àÍíËéûä‘óßÇ¬Ç∆è¡Ç¶ÇÈ
		if (Helper::GetInstance()->CheckMin(m_Timer, m_TargetTimer, 1)) {
			m_Timer = 0;
			m_Alive = false;
		}

		m_MatRot = m_Object->GetMatrot();
	}

	if (m_Alive) {
		Obj_SetParam();
	}
}