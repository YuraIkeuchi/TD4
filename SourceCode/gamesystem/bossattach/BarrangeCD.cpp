#include "BarrangeCD.h"
#include "ModelManager.h"
#include "Helper.h"
#include "Player.h"
#include <random>
BarrangeCD::BarrangeCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

//������
bool BarrangeCD::Initialize() {
	//�����w��
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(-50, -20);
	uniform_int_distribution<int> l_distZ(20, 50);
	m_Position = { float(l_distX(mt)),60.0f,float(l_distZ(mt)) };
	//m_Position = { -40.0f,60.0f,40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.5f,0.0f,0.5f,1.0f };
	m_CDState = CD_BIRTH;
	return true;
}

//���L�̏���
void BarrangeCD::Action() {
	(this->*stateTable[static_cast<size_t>(m_CDState)])();
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA);
	PlayerCollide();
	Obj_SetParam();
}


void BarrangeCD::Origin_Draw(DirectXCommon* dxCommon) {
	//Obj_Draw();
}
//IMGUI
void BarrangeCD::ImGui_Origin() {
	ImGui::Begin("BARRACD");
	ImGui::Text("CDSTATE:%d", m_CDState);
	ImGui::End();
}
//����
void BarrangeCD::BirthCD() {
	//��Ԃ悤�Ȋ����ɂ��邽�ߏd�͂�����
	m_AddPower -= m_Gravity;
	Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_AddPower);

	if (m_Position.y == 0.0f) {
		m_CDState = CD_STAY;
	}
}

//�X�e�[�W�ɂ�����
void BarrangeCD::StayCD() {

}

//�X���[���ꂽ���
void BarrangeCD::ThroughCD() {

}
//�{�X����ɓ��ꂽ���
void BarrangeCD::CatchCD() {
	m_Position = m_CatchPos;
}

//�{�X��������
void BarrangeCD::ThrowCD() {
	m_ThrowTimer++;
	if (m_ThrowTimer == 1) {
		double sb, sbx, sbz;
		sbx = Player::GetInstance()->GetPosition().x - m_Position.x;
		sbz = Player::GetInstance()->GetPosition().z - m_Position.z;
		sb = sqrt(sbx * sbx + sbz * sbz);
		m_SpeedX = sbx / sb * 0.8;
		m_SpeedZ = sbz / sb * 0.8;
	}
	else if (m_ThrowTimer > 1) {
		//�v���C���[�ɃX�s�[�h���Z
		m_Position.x += (float)m_SpeedX;
		m_Position.z += (float)m_SpeedZ;

		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -60.0f, 70.0f) || Helper::GetInstance()->CheckNotValueRange(m_Position.z, -65.0f, 65.0f)) {
			m_CDState = CD_DEATH;
			m_ThrowTimer = 0;
		}
	}
}
//������
void BarrangeCD::DeathCD() {

}

void BarrangeCD::ResPornCD() {
	m_ResPornTimer++;
	const int l_SetTimer = 5;
	const int l_LimitTimer = 10;

	if (m_ResPornTimer == l_SetTimer) {
		//�����w��
		mt19937 mt{ std::random_device{}() };
		uniform_int_distribution<int> l_distX(-50, -20);
		uniform_int_distribution<int> l_distZ(20, 50);
		m_Position = { float(l_distX(mt)),60.0f,float(l_distZ(mt)) };
		//m_Position = { -40.0f,60.0f,40.0f };
		m_AddPower = {};
	}
	else if (m_ResPornTimer == l_LimitTimer) {
		m_CDState = CD_BIRTH;
		m_ResPornTimer = {};
	}
}