#include "Player.h"

#include <any>

#include "CsvLoader.h"
#include"Helper.h"
#include"ModelManager.h"

/*-----------------*/
/*���{�G���W�������p*/
/*-----------------*/

//�R���X�g���N�^
Player::Player(XMFLOAT3 StartPos)
	:CharactorManager(StartPos)//�V�[���܂��������������W�Ƃ��ݒ�p(���΂ɃC���X�^���X��ŉ񂳂Ȃ��Ǝv���̂�)
{
	//�������Ԃ�����
	Initialize();
	//�ړ������p
	velocity /= 5.0f;
	//�傫��
	m_Scale = { 2.f,2.f,2.f };
}

//�f�X�g���N�^
Player::~Player()
{
	Model.reset(nullptr);
}

//������
void Player::Initialize()
{
	//���f���������Ɠǂݍ���
	Model.reset(new IKEFBXObject3d());
	Model->Initialize();
	Model->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::PLAYER));
	Model->LoadAnimation();

	/*CSV�ǂݍ���(CSV�t�@�C����,�ǂݍ��ރp�����[�^�̖��O,�󂯎��l)�@���͒P��̕��̂ݑΉ�(int float double char�Ƃ�)*/

	//sp����ԐړI�ɃA�N�Z�X������@ (Update()���Ő�p�̕ϐ��ɑ������K�v����)
	/*�@*/LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "speed1", sp);/*m_AddSpeed��sp����*/

	//�֐��̖߂�l���璼�ڒl�������@(�������̂ق����y�ł͂���@�����s�������������Ȃ�)
	/*�A*/m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "speed2")));
	
}

//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (Player::* Player::stateTable[])() = {
	&Player::Idle,//�ҋ@
	&Player::Walk,//�ړ�
	&Player::Attack,//�U��
};

//�X�V����
void Player::Update()
{
	//any_cast��double�^�Ȃ̂ł����static_cast��float��
	//double��atof()�֐��̖߂�l�Ȃ̂ŕύX�ł��Ȃ�<any_cast<float>�ŏ����ʂ�Ȃ�����>
	//�܂�Ƃ���std::any_cast<double>(�Z�Z)�͌Œ�(static_cast�ŕϊ�)
	/*�@*/m_AddSpeed= static_cast<float>(std::any_cast<double>(sp));

	input = Input::GetInstance();
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
	//Y�������ꂽ��U�����(�܂����Q�[���U������̂��ǂ���������񂩂畁�ʂɌ�ŏ�������)
	else if (Input::GetInstance()->TriggerButton(Input::Y))
	{
		_charaState = CharaState::STATE_ATTACK;
	}
	//�����A�N�V�������Ȃ�������A�C�h�����
	else
	{
		_charaState = CharaState::STATE_IDLE;
	}
	/*-----------------------------*/



	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_charaState])();

	//��b�p�����[�^�ݒ�
	Model->SetPosition(m_Position);
	Model->SetRotation(m_Rotation);
	Model->SetScale(m_Scale);

	//�ǂ����g���΂�����������Ȃ���������ۗ�
	Model->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}

//�`��
void Player::Draw(DirectXCommon* dxCommon)
{
	Model->Draw(dxCommon->GetCmdList());
}

//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::Text("PosZ:%f", m_Position.z);
}
//FBX�̃A�j���[�V�����Ǘ�(�A�j���[�V�����̖��O,���[�v���邩,�J�E���^���x)
void Player::AnimationControl(AnimeName name, const bool& loop, int speed)
{
	//�A�j���[�V�����������ɍ��킹��
	if (_animeName != name)
	{
		Model->PlayAnimation(static_cast<int>(name));
	}

	//�e��p�����[�^���f
	_animeName = name;
	m_LoopFlag =loop;
	m_AnimationSpeed = speed;
	
}

//����(�R���g���[���[)
void Player::Walk()
{
	XMFLOAT3 pos = m_Position;
	XMFLOAT3 rot = m_Rotation;

	float AddSpeed=2.f;


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
		Helper::GetInstance()->FloatClamp(m_Position.x, -41.0f, 50.0f);
		Helper::GetInstance()->FloatClamp(m_Position.z, -45.0f, 45.0f);
		//�����������ɐi��
		m_Position.x += move.m128_f32[0] *m_AddSpeed;
		m_Position.z += move.m128_f32[2] * m_AddSpeed;

		AnimationControl(AnimeName::WALK, true, 1);
}

XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}

//�U���A�N�V����
void Player::Attack()
{
	//�A�j���[�V�������U����
	AnimationControl(AnimeName::ATTACK, false, 1);
}

//�ҋ@�V����
void Player::Idle()
{
	//�����������������̂Ō�ŏC��
	if (_animeName == AnimeName::IDLE)return;

		//�U���[���X�e�B�b�N����
		if (_animeName == AnimeName::ATTACK) {
			//FBX�̃^�C�����ŏI�t���[�����B������A�C�h����Ԃ�
			if (Model->GetFbxTime_Current() >= Model->GetFbxTime_End())
			{
				AnimationControl(AnimeName::IDLE, true, 1);
			}
		}
		//�����[���X�e�B�b�N��������~�܂�
		else
		{
			AnimationControl(AnimeName::IDLE, true, 1);
		}
}

