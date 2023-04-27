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
//������
bool Bullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.5f,0.5f,0.5f };

	//CSV����ǂݍ���
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "speed2")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet.csv", "Timer")));
	return true;
}

//�X�V
void Bullet::Update() {
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	if (m_BulletType == BULLET_FORROW) {
		m_Color = { 1.0f,0.0f,0.0f,1.0f };
	}
	else {
		m_Color = { 0.0f,1.0f,0.0f,1.0f };
	}
	//�e�̈ړ�����
	Move();
	if (m_Alive) {
		Obj_SetParam();
	}
}

//�`��
void Bullet::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui�`��
void Bullet::ImGuiDraw() {
	ImGui::Begin("Bullet");
	ImGui::Text("Pos.X:%f", m_Position.x);
	ImGui::Text("Timer:%d", m_Timer);
	if (m_Alive) {
		
	}
	ImGui::End();
}
//�e�̈ړ�����
void Bullet::Move() {
	if (m_Alive) {
		//�ړ������Z
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		//��莞�ԗ��Ə�����
		if (Helper::GetInstance()->CheckMinINT(m_Timer,m_TargetTimer,1)) {
			m_Timer = 0;
			m_Alive = false;
		}
	}
}