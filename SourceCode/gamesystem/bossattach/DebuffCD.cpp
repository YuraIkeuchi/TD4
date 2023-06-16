#include "DebuffCD.h"
#include "ModelManager.h"
#include "Helper.h"
DebuffCD::DebuffCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool DebuffCD::Initialize() {
	m_Position = { 40.0f,60.0f,-40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	m_CDState = CD_BIRTH;
	return true;
}
//���L�̓���
void DebuffCD::Action() {
	(this->*stateTable[static_cast<size_t>(m_CDState)])();
	Obj_SetParam();
}
//�`��
void DebuffCD::Origin_Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void DebuffCD::ImGui_Origin() {
	ImGui::Begin("DEBUFFCD");
	ImGui::Text("CDSTATE:%d", m_CDState);
	ImGui::End();
}

//����
void DebuffCD::BirthCD() {
	//��Ԃ悤�Ȋ����ɂ��邽�ߏd�͂�����
	m_AddPower -= m_Gravity;
	Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_AddPower);

	if (m_Position.y == 0.0f) {
		m_CDState = CD_STAY;
	}
}

//�X�e�[�W�ɂ�����
void DebuffCD::StayCD() {

}

//�X���[���ꂽ���
void DebuffCD::ThroughCD() {

}

//�{�X����ɓ��ꂽ���
void DebuffCD::CatchCD() {
	m_Position = m_CatchPos;
}

//�{�X��������
void DebuffCD::ThrowCD() {

}

//������
void DebuffCD::DeathCD() {

}