#include "NormalEnemy.h"
#include "ModelManager.h"
#include "Helper.h"
//ÉÇÉfÉãì«Ç›çûÇ›
NormalEnemy::NormalEnemy() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Cube);
}
//èâä˙âª
bool NormalEnemy::Initialize() {
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Position.y = 5.0f;
	return true;
}
//çsìÆ
void NormalEnemy::Action() {
	m_CircleSpeed += 1.0f;
	m_Position = Helper::GetInstance()->CircleMove({ 0.0f,5.0f,0.0f }, m_CircleScale, m_CircleSpeed);
	Obj_SetParam();
}
//ï`âÊ
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	Obj_Draw();
}
//ImGuiï`âÊ
void NormalEnemy::ImGuiDraw() {
	ImGui::Begin("Enemy");
	ImGui::Text("PosX:%f",m_Position.x);
	ImGui::Text("PosY:%f", m_Position.y);
	ImGui::Text("PosZ:%f", m_Position.z);
	ImGui::End();
}
//äJï˙
void NormalEnemy::Finalize() {

}