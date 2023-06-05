#include "Heart.h"
#include "VariableCommon.h"
#include "Collision.h"
#include "CsvLoader.h"
#include <random>
#include "Player.h"
Heart::Heart() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Heart);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool Heart::Initialize() {
	const int l_Division = 80;
	//�����w��
	m_Scale = { 0.8f,0.8f,0.8f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Alive = true;
	m_AliveTimer = 0;
	_heartState = HeartState::HEART_JUMP;
	//�����͈̔�(-50����50)
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_bounddist(-80, 80);
	uniform_int_distribution<int> l_bounddist2(50, 80);

	m_AddPower = {
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist2(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
	};

	m_LimitTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/heart/heart.csv", "Limit")));
	return true;
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (Heart::* Heart::stateTable[])() = {
	&Heart::HeartJump,//��΂�
	&Heart::HeartSet,//�X�g�b�v
};
//�X�V
void Heart::Update() {
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_heartState])();
	m_Rotation.y += 1.0f;
	//���~�b�g����
	Helper::GetInstance()->Clamp(m_Position.x, -50.0f, 60.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -55.0f, 55.0f);
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();
	//�p�[�e�B�N��
	Particle();
	//�����蔻��(�v���C���[)
	PlayerCollision();
}
//�`��
void Heart::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui�`��
void Heart::ImGuiDraw() {
}
//�p�[�e�B�N��
void Heart::Particle() {
	XMFLOAT4 s_color = { 0.5f,1.0f,0.1f,1.0f };
	XMFLOAT4 e_color = { 0.5f,1.0f,0.1f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	if (m_Alive) {
		ParticleEmitter::GetInstance()->HealEffect(40, m_Position, s_scale, e_scale, s_color, e_color);
	}
}
//�����蔻��(�v���C���[)
bool Heart::PlayerCollision() {
	if (!m_Alive) { return false; }
	if (Player::GetInstance()->PlayerCollide(m_Position) && (_heartState == HeartState::HEART_SET)) {
		m_Alive = false;
		Player::GetInstance()->SetHP(Player::GetInstance()->GetHP() + 1.0f);
		return true;
	}
	else {
		return false;
	}

	return true;
}

//�n�[�g���΂�
void Heart::HeartJump() {
	m_AddPower.y -= m_Gravity;
	m_Position = Helper::GetInstance()->Float3AddFloat3(m_Position, m_AddPower);

	if (m_Position.y <= 0.0f) {
		_heartState = HeartState::HEART_SET;
	}
}

//�n�[�g�̒��n��
void Heart::HeartSet() {
	const float l_AddFrame = 0.05f;
	m_AliveTimer++;

	if (m_AliveTimer >= m_LimitTimer) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
			m_AliveTimer = {};
			m_Frame = {};
		}
	}

	m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.0f),
	Ease(In,Cubic,m_Frame,m_Scale.y,0.0f),
	Ease(In,Cubic,m_Frame,m_Scale.z,0.0f)
	};
}