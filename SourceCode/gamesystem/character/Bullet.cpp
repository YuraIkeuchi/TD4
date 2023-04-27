#include "Bullet.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Helper.h"
Bullet::Bullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool Bullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.5f,0.5f,0.5f };

	//CSVから読み込み
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "Timer")));
	return true;
}

//更新
void Bullet::Update() {
	//タイプによって色を一旦変えてる
	if (m_BulletType == BULLET_FORROW) {
		m_Color = { 1.0f,0.0f,0.0f,1.0f };
	}
	else {
		m_Color = { 0.0f,1.0f,0.0f,1.0f };
	}
	//弾の移動処理
	Move();
	if (m_Alive) {
		Obj_SetParam();
	}
}

//描画
void Bullet::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui描画
void Bullet::ImGuiDraw() {
	ImGui::Begin("Bullet");
	ImGui::Text("Pos.X:%f", m_Position.x);
	ImGui::Text("Timer:%d", m_Timer);
	if (m_Alive) {
		
	}
	ImGui::End();
}
//弾の移動処理
void Bullet::Move() {
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
}