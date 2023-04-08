#include "Player.h"

#include "Input.h"
#include"ModelManager.h"


/*-----------------*/
/*���{�G���W�������p*/
/*-----------------*/

//�R���X�g���N�^
Player::Player()
{
	//�������Ԃ�����
	Init();
}

//�f�X�g���N�^
Player::~Player()
{
	
}

//������
void Player::Init()
{
	Model.reset(new IKEFBXObject3d());
	Model->Initialize();
	Model->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::PLAYER));
	Model->LoadAnimation();
}

//�X�V����
void Player::Upda()
{
	/*FBX�̃J�E���^double�ɂ����ق������������₷����*/

	//�U���A�N�V����
	if(Input::GetInstance()->TriggerButton(Input::Button_Y))
		AnimationControl(AnimeName::ATTACK, false, 1);

	//�A�N�V������
	if (_animeName != AnimeName::IDLE)
	{
		//FBX�̃^�C�����ŏI�t���[�����B������A�C�h����Ԃ�
		if (Model->GetFbxTime_Current() >= Model->GetFbxTime_End())
		{
			AnimationControl(AnimeName::IDLE, true, 1);
		}
	}

	//��b�p�����[�^�ݒ�
	Model->SetPosition({ 0.0f,8.0f,0.0f });
	Model->SetRotation({ 0.0f,0.0f,0.0f });
	Model->SetScale({ 2.02f,2.02f,2.02f });

	//�ǂ����g���΂�����������Ȃ���������ۗ�
	Model->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
	Model->FollowUpdate(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}

//�`��
void Player::Draw(DirectXCommon* dxCommon)
{
	Model->Draw(dxCommon->GetCmdList());
}

//FBX�̃A�j���[�V�����Ǘ�(�A�j���[�V�����̖��O,���[�v���邩,�J�E���^���x)
void Player::AnimationControl(AnimeName name, const bool& loop, int speed)
{
	if (_animeName != name)
	{
		Model->PlayAnimation(static_cast<int>(name));
	}

	//�e��p�����[�^���f
	_animeName = name;
	m_LoopFlag =loop;
	m_AnimationSpeed = speed;
	
}



