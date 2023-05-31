#include "Heart.h"
#include "VariableCommon.h"
#include "Collision.h"
#include "Helper.h"
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
	//�����w��
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Alive = false;
	m_AliveTimer = 0;
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
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();
	//�p�[�e�B�N��
	Particle();
	//�����蔻��(�v���C���[)
	PlayerCollision();
}
//�`��
void Heart::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui�`��
void Heart::ImGuiDraw() {

}
//�p�[�e�B�N��
void Heart::Particle() {

}
//�����蔻��(�v���C���[)
bool Heart::PlayerCollision() {
	if (Player::GetInstance()->PlayerCollide(m_Position) && (_heartState == HeartState::HEART_SET)) {
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return true;
}

//�n�[�g���΂�
void Heart::HeartJump() {

}

//�n�[�g�̒��n��
void Heart::HeartSet() {

}