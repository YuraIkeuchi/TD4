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
	uniform_int_distribution<int> l_distX(-50, 60);
	uniform_int_distribution<int> l_distZ(-55, 55);
	m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	_charaState = CharaState::STATE_NONE;
	_searchState = SearchState::SEARCH_NO;
	_followState = FollowState::Follow_NO;
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
	m_OldPos = m_Position;
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_charaState])();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();
	//�H������
	BirthGhost();
	//�p�[�e�B�N��
	Particle();
	//�����蔻��(�e)
	BulletCollision();
	//�����蔻��(�v���C���[)
	PlayerCollision();
	//�H�ו����͂���
	CarryFood();
}
//�`��
void Ghost::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui�`��
void Ghost::ImGuiDraw() {
	//ImGui::Begin("Ghost");
	//ImGui::End();
}
//�p�[�e�B�N��
void Ghost::Particle() {
	XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 s_color2 = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 e_color2 = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 s_color3 = { 0.0f,1.0f,0.0f,1.0f };
	XMFLOAT4 e_color3 = { 0.0f,1.0f,0.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	if (m_Alive) {
		if (_charaState == CharaState::STATE_NONE) {
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color, e_color);
		}
		else if (_charaState == CharaState::STATE_FOLLOW) {
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color2, e_color2);
		}
		else {
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color3, e_color3);
		}
	}
}
//�����蔻��(�e)
bool Ghost::BulletCollision() {
	float l_AddHungerMax = HungerGauge::m_Hungervalue;//���Z�����ő�Q��Q�[�W
	if (player->BulletCollide(m_Position,m_Catch) && (m_Alive)) {
		m_Catch = true;
		if (player->GetBulletType() == BULLET_FORROW) {
			HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() + l_AddHungerMax);
			HungerGauge::GetInstance()->SetNowHunger(HungerGauge::GetInstance()->GetHungerMax());
			HungerGauge::GetInstance()->SetCatchCount(HungerGauge::GetInstance()->GetCatchCount() + 1.0f);
			_charaState = CharaState::STATE_FOLLOW;
			_followState = FollowState::Follow_START;
			m_Follow = true;
		}
		else {
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
void Ghost::GhostCollision(const XMFLOAT3& pos) {
	m_Position.x += sin(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
	m_Position.z += cos(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
}
//�H������
void Ghost::BirthGhost() {
	if (!m_Alive) {
		m_ResPornTimer++;
		//�`��o�O�N���邩���ɍ��W�Z�b�g
		if (m_ResPornTimer == 20) {
			//�����w��
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_distX(-50, 60);
			uniform_int_distribution<int> l_distZ(-55, 55);
			m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
		}
		//��莞�ԂŐ��������
		if (m_ResPornTimer == 100) {
			_charaState = CharaState::STATE_NONE;
			_searchState = SearchState::SEARCH_NO;
			m_Alive = true;
			m_Catch = false;
			m_Search = false;
			m_Follow = false;
			m_ResPornTimer = 0;
		}
	}
}
//�����Ȃ����
void Ghost::None() {

}
//�Ǐ]
void Ghost::Follow() {
	float l_Vel = 0.35f;//���x
	XMFLOAT3 l_playerPos = player->GetPosition();
	Helper::GetInstance()->FollowMove(m_Position, l_playerPos, l_Vel);
}
//�T��
void Ghost::Search() {
	float l_Vel = 0.3f;
	XMFLOAT3 l_playerPos = player->GetPosition();
	//�Ǐ]
	if (_searchState == SearchState::SEARCH_START) {
		Helper::GetInstance()->FollowMove(m_Position, m_SearchPos, l_Vel);
	}
	else if (_searchState == SearchState::SEARCH_END) {
		Helper::GetInstance()->FollowMove(m_Position, l_playerPos, l_Vel);
	}
}
//�T���X�^�[�g
void Ghost::StartSearch(const XMFLOAT3& pos) {
	_searchState = SearchState::SEARCH_START;
	m_Search = true;
	m_SearchPos = pos;
}
//�T���I��
void Ghost::EndSearch() {
	_searchState = SearchState::SEARCH_END;
}
//�H�ו����^��
void Ghost::CarryFood() {
	float l_Radius = 1.0f;//�����蔻��
	float l_AddHunger = HungerGauge::m_Hungervalue;//���Z�����C���Q�[�W
	XMFLOAT3 l_playerPos = player->GetPosition();
	if ((_searchState == SearchState::SEARCH_END) && (m_Alive)) {
		if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_playerPos.x, l_playerPos.z, l_Radius)) {
			m_Alive = false;
			m_Search = false;
			m_Catch = false;
			m_Limit = {};
			HungerGauge::GetInstance()->SetNowHunger(HungerGauge::GetInstance()->GetNowHunger() + l_AddHunger);
		}
	}
}