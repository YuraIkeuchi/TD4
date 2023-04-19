#include "NormalEnemy.h"
#include "ModelManager.h"
#include "Helper.h"
#include "ParticleEmitter.h"
//���f���ǂݍ���
NormalEnemy::NormalEnemy() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Cube);
}
//������
bool NormalEnemy::Initialize() {

	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Position.y = 5.0f;
	return true;
}
//�s��
void NormalEnemy::Action() {
	XMFLOAT3 l_player = player->GetPosition();
	m_CircleSpeed += 1.0f;
	m_Position = Helper::GetInstance()->CircleMove({ 0.0f,5.0f,0.0f }, m_CircleScale, m_CircleSpeed);
	Obj_SetParam();
	Particle();
}
//�`��
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	Obj_Draw();
}
//ImGui�`��
void NormalEnemy::ImGuiDraw() {
	ImGui::Begin("Enemy");
	ImGui::Text("PosX:%f", player->GetPosition().x);
	ImGui::Text("PosY:%f", player->GetPosition().y);
	ImGui::Text("PosZ:%f", player->GetPosition().z);
	ImGui::End();
}
//�J��
void NormalEnemy::Finalize() {

}
//�p�[�e�B�N��
void NormalEnemy::Particle() {
	XMFLOAT4 s_color = { 1.0f,0.5f,1.0f,1.5f };
	XMFLOAT4 e_color = { 1.0f,0.5f,1.0f,1.5f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->FireEffect(50, m_Position, s_scale, e_scale, s_color, e_color);
}