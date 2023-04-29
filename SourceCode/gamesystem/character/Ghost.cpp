#include "Ghost.h"
#include "ModelManager.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "ParticleEmitter.h"
#include "Collision.h"
#include "HungerGauge.h"
#include "Helper.h"
#include <random>
Ghost::Ghost() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Cube);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool Ghost::Initialize() {
	//�����w��
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(-41, 50);
	uniform_int_distribution<int> l_distZ(-45, 45);
	m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	_charaState = CharaState::STATE_NONE;
	return true;
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (Ghost::* Ghost::stateTable[])() = {
	&Ghost::None,//�ҋ@
	&Ghost::Follow,//�ړ�
	&Ghost::Search,//�U��
};
//�X�V
void Ghost::Update() {
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_charaState])();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();
	//�H������
	//BirthGhost();
	//�p�[�e�B�N��
	Particle();
	//�����蔻��(�e)
	BulletCollision();
	//�����蔻��(�v���C���[)
	PlayerCollision();
}
//�`��
void Ghost::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui�`��
void Ghost::ImGuiDraw() {
	ImGui::Begin("Ghost");
	ImGui::Text("State:%d", m_ma);
	ImGui::End();
}
//�p�[�e�B�N��
void Ghost::Particle() {
	XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color, e_color);
}
//�����蔻��(�e)
bool Ghost::BulletCollision() {
	float l_AddHungerMax = 5.0f;//���Z�����ő�Q��Q�[�W
	if (player->BulletCollide(m_Position) && m_Alive) {
		m_Alive = false;
		m_Catch = true;
		if (player->GetBulletType() == BULLET_FORROW) {
			HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() + l_AddHungerMax);
			_charaState = CharaState::STATE_FOLLOW;
		}
		else {
			m_BasePos = m_Position;
			_charaState = CharaState::STATE_SEARCH;
		}
		return true;
	}
	else {
		return false;
	}
	return true;
}
//�����蔻��(�v���C���[)
bool Ghost::PlayerCollision() {
	if (player->PlayerCollide(m_Position) && (_charaState == CharaState::STATE_FOLLOW)) {
		m_Position = m_OldPos;
		return true;
	}
	else {
		return false;
	}

	return true;
}
//�����蔻��(�S�[�X�g���m)
bool Ghost::GhostCollision(const XMFLOAT3& pos) {
	return true;
}
//�H������
void Ghost::BirthGhost() {
	if (!m_Alive) {
		m_Timer++;
		//�`��o�O�N���邩���ɍ��W�Z�b�g
		if (m_Timer == 20) {
			//�����w��
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_distX(-41, 50);
			uniform_int_distribution<int> l_distZ(-45, 45);
			m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
		}
		//��莞�ԂŐ��������
		if (m_Timer == 100) {
			_charaState = CharaState::STATE_NONE;
			m_Alive = true;
			m_Timer = 0;
		}
	}
}
//�����Ȃ����
void Ghost::None() {

}
//�Ǐ]
void Ghost::Follow() {
	m_OldPos = m_Position;
	XMFLOAT3 l_player = player->GetPosition();
	float l_Vel = 0.15f;//���x
	//�Ǐ]
	if (!m_ma) {
		Helper::GetInstance()->FollowMove(m_Position, l_player, l_Vel);
	}
}
//�T��
void Ghost::Search() {
	float l_Vel = 0.15f;
	//�Ǐ]
	if (m_Search) {
		Helper::GetInstance()->FollowMove(m_Position, m_SearchPos, l_Vel);
	}
}
//�T���X�^�[�g
void Ghost::StartSearch(const XMFLOAT3& pos) {
	m_Search = true;
	m_SearchPos = pos;
}