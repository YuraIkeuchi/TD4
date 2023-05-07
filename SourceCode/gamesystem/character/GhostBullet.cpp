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
//初期化
bool GhostBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.5f,0.5f,0.5f };

	//CSVから読み込み
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "Timer")));
	return true;
}

//ImGui描画
void GhostBullet::ImGui_Origin() {

}
//弾の特有処理
void GhostBullet::Action() {
	//タイプによって色を一旦変えてる
	if (m_BulletType == BULLET_FORROW) {
		m_Color = { 1.0f,0.0f,0.0f,1.0f };
	}
	else {
		m_Color = { 0.0f,1.0f,0.0f,1.0f };
	}
	if (m_Alive) {
		//移動を加算
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		//一定時間立つと消える
		if (Helper::GetInstance()->CheckMinINT(m_Timer,m_TargetTimer,1)) {
			m_Timer = 0;
			m_Alive = false;
		}
	}

	if (m_Alive) {
		Obj_SetParam();
	}
}