#include "Bullet.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Helper.h"
Bullet::Bullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Box);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool Bullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 2.5f,2.5f,2.5f };
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	return true;
}

//更新
void Bullet::Update() {
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
	if (m_Alive) {
		ImGui::Text("Pos.X:%f", m_Position.x);
		ImGui::Text("Type:%d", m_BulletType);
	}
	ImGui::End();
}
//弾の移動処理
void Bullet::Move() {
	if (m_Alive) {
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if ((Helper::GetInstance()->CheckNotValueRange(m_Position.x, -41.0f, 50.0f)) || (Helper::GetInstance()->CheckNotValueRange(m_Position.z, -45.0f, 45.0f))) {
			m_Alive = false;
		}
	}
}