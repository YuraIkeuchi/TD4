#include "FiveBoss.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Player.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include <random>

#include "Input.h"

FiveBoss::FiveBoss()
{

	//�_�[�N�Վq���f��
		//���f���������Ɠǂݍ���
	m_fbxObject.reset(new IKEFBXObject3d());
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::PLAYER));
	m_fbxObject->LoadAnimation();
	m_fbxObject->PlayAnimation(0);

	confueffect.reset(new ConfuEffect());
	confueffect->Initialize();

	shot = new ShotAttack();
	normal = new NormalAttack();
	smash = new SmashShotAttack();
	slash = new ShadowSlashAttack();

	smash->Init();
	shot->Init();
	slash->Init();
	
	normal->Init();
normal->SetBoss(this);


	noteeffect.reset(new NoteEffect());
	noteeffect->Initialize();

	m_HP = 50;
}

bool FiveBoss::Initialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 1.2f,0.8f,1.2f };
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	//m_Rotation.y = -90.f;

	/*ActionTimer = 1;

	m_Radius = 5.2f;

	_charaState = STATE_INTER;
	m_AreaState = AREA_SET;*/
	//CSV���[�h
	CSVLoad();
	return true;
}

void FiveBoss::SkipInitialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
}

void FiveBoss::Pause()
{
}
void (FiveBoss::* FiveBoss::attackTable[])() = {
	&FiveBoss::Pause,
	&FiveBoss::Shot,
	&FiveBoss::Normal,
	& FiveBoss::Smash,
	& FiveBoss::Slash,

};

void FiveBoss::Action()
{
	smash->SetBoss(this);
	shot->SetBoss(this);
	slash->SetBoss(this);
	////��Ԉڍs(charastate�ɍ��킹��)
	//if (m_HP > 0.0f) {
		(this->*attackTable[_aPhase])();
	//}

	/// <summary>
	/// �U���[�RWAY
	/// </summary>

		if (_aPhase == ATTACK_SHOT)
		{
			RandAction = 0;
			if (shot->GetActionEnd())
			{
				ActionTimer++;
				_aPhase = ATTACK_NORMAL;
			}
		}
		if (_aPhase == ATTACK_IMPACT)
		{
			RandAction = 0;
		if (smash->GetActionEnd())
		{
			ActionTimer++;
			_aPhase = ATTACK_NORMAL;
		}
	}

		if (_aPhase == ATTACK_NORMAL)ActionTimer++;


	mt19937 mt{ std::random_device{}() };
	if (_aPhase == ATTACK_NORMAL&&ActionTimer%120==0) {
		RandAction = rand() % 3;

		if (RandAction==1)
		{
			slash->SetActionEnd(false);
			_aPhase = ATTACK_SLASH;
		}
		if (RandAction==2)
		{
			slash->SetActionEnd(false);
			_aPhase = ATTACK_SLASH;
		}
	}
	/*^^^^�����蔻��^^^^*/
	//�e�ƃ{�X�̓����蔻��
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//�v���C���[�̓����蔻��
	ColPlayer();
	//OBJ�̃X�e�[�^�X�̃Z�b�g
	//Obj_SetParam();
	//���~�b�g����
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);


	//HP�������؂����狭��
	if (m_HP < m_MaxHp / 2) {
		isStrong = true;
	}
	//��b�p�����[�^�ݒ�
	
	Fbx_SetParam();

	//�ǂ����g���΂�����������Ȃ���������ۗ�
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);

}

void FiveBoss::AppearAction()
{
}

void FiveBoss::DeadAction()
{
}

void FiveBoss::DeadAction_Throw()
{
}

void FiveBoss::CSVLoad()
{
}

void FiveBoss::DeathParticle()
{
}

void FiveBoss::ImGui_Origin()
{
}

void FiveBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;

	confueffect->Draw(dxCommon);
	noteeffect->Draw(dxCommon);
}

void FiveBoss::Draw(DirectXCommon* dxCommon)
{
	//Obj_Draw();
	smash->Draw(dxCommon);
	shot->Draw(dxCommon);
	slash->Draw(dxCommon);
	Fbx_Draw(dxCommon);

}
