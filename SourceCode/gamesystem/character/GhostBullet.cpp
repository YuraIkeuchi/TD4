#include "GhostBullet.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Easing.h"
#include "Helper.h"
GhostBullet::GhostBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool GhostBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.0f,0.0f,0.0f,1.0f };
	//CSVから読み込み
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "Timer")));
	return true;
}
//ImGui描画
void GhostBullet::ImGui_Origin() {
	ImGui::Begin("Ghost");
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::End();
}
//弾の特有処理
void GhostBullet::Action() {
	//タイプによって色を一旦変えてる
	if (m_BulletType == BULLET_FORROW) {
		m_Color.x = 1.0f;
	}
	else {
		m_Color.y = 1.0f;
	}
	if (m_Alive) {
		//移動を加算
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
	}

	VanishBullet();

	if (m_Alive) {
		Obj_SetParam();
	}
}
//弾が徐々に消える処理
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
	}
}