#include "Player.h"
#include <any>
#include "CsvLoader.h"
#include"Helper.h"
#include"ModelManager.h"
#include "VariableCommon.h"
#include "HungerGauge.h"
#include "Collision.h"
#include "Input.h"

//�R���X�g���N�^
Player::Player(XMFLOAT3 StartPos)
{
	m_Position = StartPos;
	//�������Ԃ�����
	Initialize();
	//�ړ������p
	velocity /= 5.0f;
	//�傫��
	m_Scale = { 2.5f,2.5f,2.5f };
}
//�f�X�g���N�^
Player::~Player()
{
	m_fbxObject.reset(nullptr);
}
//������
bool Player::Initialize()
{
	//���f���������Ɠǂݍ���
	m_fbxObject.reset(new IKEFBXObject3d());
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::PLAYER));
	m_fbxObject->LoadAnimation();

	/*CSV�ǂݍ���(CSV�t�@�C����,�ǂݍ��ރp�����[�^�̖��O,�󂯎��l)�@���͒P��̕��̂ݑΉ�(int float double char�Ƃ�)*/

	//sp����ԐړI�ɃA�N�Z�X������@ (Update()���Ő�p�̕ϐ��ɑ������K�v����)
	/*�@*/LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "speed1", sp);/*m_AddSpeed��sp����*/

	//�֐��̖߂�l���璼�ڒl�������@(�������̂ق����y�ł͂���@�����s�������������Ȃ�)
	/*�A*/m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "speed2")));

	m_TargetInterVal = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "InterVal")));
	m_TargetRigidityTime = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "Rigidity")));

	//�Q��Q�[�W�̓v���C���[�ŊǗ�����
	HungerGauge::GetInstance()->Initialize();

	viewbullet.reset(new ViewBullet());
	viewbullet->Initialize();
	return true;
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (Player::* Player::stateTable[])() = {
	&Player::Idle,//�ҋ@
	&Player::Walk,//�ړ�
};
//�X�V����
void Player::Update()
{
	//any_cast��double�^�Ȃ̂ł����static_cast��float��
	//double��atof()�֐��̖߂�l�Ȃ̂ŕύX�ł��Ȃ�<any_cast<float>�ŏ����ʂ�Ȃ�����>
	//�܂�Ƃ���std::any_cast<double>(�Z�Z)�͌Œ�(static_cast�ŕϊ�)
	/*�@*/m_AddSpeed= static_cast<float>(std::any_cast<double>(sp));

	Input* input = Input::GetInstance();
	/*FBX�̃J�E���^double�ɂ����ق������������₷����*/


	/*--------�L�����̊�{����-------*/
	/*-----------------------------*/
	//�X�e�B�b�N��������Ă�Ԃ͈ړ�����
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		_charaState = CharaState::STATE_RUN;
	}
	//�����A�N�V�������Ȃ�������A�C�h�����
	else
	{
		_charaState = CharaState::STATE_IDLE;
	}
	//�e�̊Ǘ�
	Bullet_Management();

	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_charaState])();

	//��b�p�����[�^�ݒ�
	Fbx_SetParam();
	
	//�ǂ����g���΂�����������Ȃ���������ۗ�
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);

	//State�ɓ���Ȃ��Ă������
	//�U���̃C���^�[�o��
	InterVal();
	//�Q��Q�[�W�X�V
	HungerGauge::GetInstance()->Update();
}
//�`��
void Player::Draw(DirectXCommon* dxCommon)
{
	//�L�����N�^�[
	Fbx_Draw(dxCommon);
	//�e�̕`��
	BulletDraw(ghostbullets, dxCommon);
	BulletDraw(attackbullets, dxCommon);
}
//�e�̕`��
void Player::BulletDraw(std::vector<InterBullet*> bullets, DirectXCommon* dxCommon) {
	for (InterBullet* bullet : bullets) {
		if (bullet != nullptr) {
			bullet->Draw(dxCommon);
		}
	}

	//viewbullet->Draw(dxCommon);
}
//ImGui
void Player::ImGuiDraw() {
}
//FBX�̃A�j���[�V�����Ǘ�(�A�j���[�V�����̖��O,���[�v���邩,�J�E���^���x)
void Player::AnimationControl(AnimeName name, const bool& loop, int speed)
{
	//�A�j���[�V�����������ɍ��킹��
	if (_animeName != name)
	{
		m_fbxObject->PlayAnimation(static_cast<int>(name));
	}

	//�e��p�����[�^���f
	_animeName = name;
	m_LoopFlag =loop;
	m_AnimationSpeed = speed;
	
}
//����(�R���g���[���[)
void Player::Walk()
{
	XMFLOAT3 rot = m_Rotation;

	float AddSpeed=2.f;
	Input* input = Input::GetInstance();

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float STICK_MAX = 32768.0f;
	
		//�����
		if (input->TiltPushStick(Input::L_UP, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, velocity, 0 }, angle);

		//������
		if (input->TiltPushStick(Input::L_DOWN, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, -velocity, 0 }, angle);

		//�E����
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ velocity, 0, 0, 0 }, angle);

		//������
		if (input->TiltPushStick(Input::L_LEFT, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -velocity, 0, 0, 0 }, angle);
		

		const float rnd_vel = 0.1f;

		XMFLOAT3 vel{};

		vel.x = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		rot.y = angle + atan2f(StickX, StickY) * (PI_180 / PI);

		//�v���C���[�̉�]�p�����
		m_Rotation = { rot.x, rot.y, rot.z };

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
		move = XMVector3TransformNormal(move, matRot);

		//���~�b�g����
		Helper::GetInstance()->FloatClamp(m_Position.x, -55.0f, 65.0f);
		Helper::GetInstance()->FloatClamp(m_Position.z, -60.0f, 60.0f);
		//�����������ɐi��
		if (m_RigidityTime == m_ResetNumber) {
			m_Position.x += move.m128_f32[0] * m_AddSpeed;
			m_Position.z += move.m128_f32[2] * m_AddSpeed;
		}
		AnimationControl(AnimeName::WALK, true, 1);
}
//VECTOR
XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}
//�e�̍X�V
void Player::Bullet_Management() {
	const float l_TargetCount = 1.0f;
	const int l_Limit = 20;//�V���b�g�̃`���[�W����
	/*-----------------------------*/
	//RB||LB�������ꂽ��e������(����)
	if (((Input::GetInstance()->TriggerButton(Input::RB)) || (Input::GetInstance()->TriggerButton(Input::LB))) && (m_InterVal == 0))
	{
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Shot_Normal.wav",VolumManager::GetInstance()->GetSEVolum());
		if (Input::GetInstance()->TriggerButton(Input::RB)) {
			m_BulletType = BULLET_FORROW;
		}
		else if (Input::GetInstance()->TriggerButton(Input::LB)) {
			m_BulletType = BULLET_SEARCH;
		}
		ResetBullet();
		BirthShot("Ghost", false);
	}

	//�U��
	//B�������ꂽ��e�̃`���[�W
	if (Input::GetInstance()->PushButton(Input::B) && m_InterVal == 0 && HungerGauge::GetInstance()->GetCatchCount() >= l_TargetCount)
	{
		m_ShotTimer++;
		viewbullet->SetAlive(true);
	}

	//�`���[�W���Ԃ����𒴂�����Q��Q�[�W�̌��鑬�x���オ��
	if (m_ShotTimer > l_Limit) {
		viewbullet->SetCharge(true);
		HungerGauge::GetInstance()->SetSubVelocity(2.0f);
		//�`���[�W���ɋQ��Q�[�W���؂ꂽ�ꍇ�e�������ŕ������
		if (HungerGauge::GetInstance()->GetNowHunger() == 0.0f) {
			BirthShot("Attack", true);
			HungerGauge::GetInstance()->SetSubVelocity(1.0f);
			ResetBullet();
		}
	}

	if (!Input::GetInstance()->PushButton(Input::B) && m_ShotTimer != 0) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Shot_Charge.wav", VolumManager::GetInstance()->GetSEVolum());
		if (m_ShotTimer < l_Limit) {
			BirthShot("Attack", false);
		}
		else {
			BirthShot("Attack", true);
			HungerGauge::GetInstance()->SetSubVelocity(1.0f);
		}
		ResetBullet();
	}

	//�e�̍폜(����)
	for (int i = 0; i < ghostbullets.size(); i++) {
		if (ghostbullets[i] == nullptr) {
			continue;
		}

		if (!ghostbullets[i]->GetAlive()) {
			ghostbullets.erase(cbegin(ghostbullets) + i);
		}
	}

	//�e�̍폜(����)
	for (int i = 0; i < attackbullets.size(); i++) {
		if (attackbullets[i] == nullptr) {
			continue;
		}

		if (!attackbullets[i]->GetAlive()) {
			attackbullets.erase(cbegin(attackbullets) + i);
		}
	}
	//�e�̍X�V
	BulletUpdate(ghostbullets);
	BulletUpdate(attackbullets);
	//�e�����������Z�o���邽�߂ɉ�]�����߂�
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	XMFLOAT2 l_Angle;
	l_Angle.x = move.m128_f32[0];
	l_Angle.y = move.m128_f32[2];


	//�����̒e�֌W
	viewbullet->Update();
	viewbullet->SetAngle(l_Angle);
	viewbullet->SetPosition(m_Position);
}
void Player::BulletUpdate(std::vector<InterBullet*> bullets) {
	//�e�̍X�V
	for (InterBullet* bullet : bullets) {
		if (bullet != nullptr) {
			bullet->Update();
		}
	}
	
}
//�e�̐���
void Player::BirthShot(const std::string& bulletName, bool Super) {
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	XMFLOAT2 l_Angle;
	l_Angle.x = move.m128_f32[0];
	l_Angle.y = move.m128_f32[2];
	//�U���̒e
	if (bulletName == "Attack") {
		InterBullet* newbullet;
		newbullet = new AttackBullet();
		newbullet->Initialize();
		newbullet->SetPosition(viewbullet->GetPosition());
		//�`���[�W�V���b�g���ǂ���
		if (Super) {
			newbullet->SetScale(viewbullet->GetScale());
		}
		else {
			newbullet->SetScale({ 1.0f,1.0f,1.0f });
		}
		newbullet->SetAngle(l_Angle);
		attackbullets.push_back(newbullet);
	}
	//����
	else if(bulletName == "Ghost") {
		//�e�̐���
		GhostBullet* newbullet;
		newbullet = new GhostBullet();
		newbullet->Initialize();
		newbullet->SetPosition(m_Position);
		newbullet->SetBulletType(m_BulletType);
		newbullet->SetAngle(l_Angle);
		ghostbullets.push_back(newbullet);
	}
	else {
		assert(0);
	}
}
//�ҋ@���[�V����
void Player::Idle()
{
	//�����������������̂Ō�ŏC��
	if (_animeName == AnimeName::IDLE)return;
	AnimationControl(AnimeName::IDLE, true, 1);
}
//�C���^�[�o��
void Player::InterVal() {
	Helper::GetInstance()->CheckMaxINT(m_InterVal, 0, -1);
	Helper::GetInstance()->CheckMaxINT(m_RigidityTime, 0, -1);
}
//�e�Ƃ̓����蔻��
bool Player::BulletCollide(const XMFLOAT3& pos,const bool Catch) {
	float l_Radius = 1.3f;//������͈�
	//�e�̍X�V
	for (InterBullet* bullet : ghostbullets) {
		if (bullet != nullptr) {
			if (Collision::CircleCollision(bullet->GetPosition().x, bullet->GetPosition().z, l_Radius, pos.x, pos.z, l_Radius) && (bullet->GetAlive()) && (!Catch)) {
				//bullet->SetAlive(false);
				return true;
			}
			else {
				return false;
			}
		}
	}

	return false;
}
//�v���C���[�Ƃ̓����蔻��
bool Player::PlayerCollide(const XMFLOAT3& pos) {
	float l_Radius = 3.1f;//������͈�
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, pos.x, pos.z, l_Radius)) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
//�e�̃��Z�b�g
void Player::ResetBullet() {
	m_InterVal = m_TargetInterVal;
	m_RigidityTime = m_TargetRigidityTime;
	viewbullet->SetAlive(false);
	viewbullet->SetCharge(false);
	m_ShotTimer = {};
}