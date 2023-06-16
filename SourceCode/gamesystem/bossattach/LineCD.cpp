#include "LineCD.h"
#include "ModelManager.h"
#include "Helper.h"
LineCD::LineCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool LineCD::Initialize() {
	m_Position = { 40.0f,60.0f,40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	m_CDState = CD_BIRTH;
	return true;
}
//���L�̌ۓ�
void LineCD::Action() {
	(this->*stateTable[static_cast<size_t>(m_CDState)])();
	Obj_SetParam();
}
//�`��
void LineCD::Origin_Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void LineCD::ImGui_Origin() {
	ImGui::Begin("LINECD");
	ImGui::Text("CDSTATE:%d", m_CDState);
	ImGui::End();
}

//����
void LineCD::BirthCD() {
	//��Ԃ悤�Ȋ����ɂ��邽�ߏd�͂�����
	m_AddPower -= m_Gravity;
	Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_AddPower);

	if (m_Position.y == 0.0f) {
		m_CDState = CD_STAY;
	}
}

//�X�e�[�W�ɂ�����
void LineCD::StayCD() {

}

//�X���[���ꂽ���
void LineCD::ThroughCD() {

}

//�{�X����ɓ��ꂽ���
void LineCD::CatchCD() {
	m_Position = m_CatchPos;
}

//�{�X��������
void LineCD::ThrowCD() {

}

//������
void LineCD::DeathCD() {

}