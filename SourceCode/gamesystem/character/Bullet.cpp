#include "Bullet.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
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
	//弾の移動処理
	Move();
	Obj_SetParam();
}

//描画
void Bullet::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void Bullet::ImGuiDraw() {
	ImGui::Begin("Bullet");
	ImGui::Text("Pos.X:%f", m_Position.x);
	ImGui::End();
}
//弾の移動処理
void Bullet::Move() {
	m_Position.x += m_Angle.x * m_AddSpeed;
	m_Position.z += m_Angle.y * m_AddSpeed;
}