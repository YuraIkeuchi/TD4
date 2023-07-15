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

	noteeffect.reset(new NoteEffect());
	noteeffect->Initialize();

	m_HP = 10;
}

bool FiveBoss::Initialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Radius = 5.2f;
	m_Scale = { 1.9f,1.5f,1.9f };
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	//m_Rotation.y = -90.f;
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "Magnification")));
	m_HP =  static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "HeartTarget")));

	//�m�b�N�o�b�N�p�x
	KnockInter = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "KnockInter")));

	//KnockBack
	KnockDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "KnockDam")));


		//MeteoShot
		MeteoDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "MeteoDam")));

		//WayShot
		ShotDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "ShotDam")));

		//Ultimate
		UltDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "UltDam")));


		shot = new ShotAttack();
		normal = new NormalAttack();
		smash = new SmashShotAttack();
		slash = new ShadowSlashAttack();
		single = new SingleShot();
		guard = new GuardAction();
		knock = new KnockAttack();

		smash->Init();
		shot->Init();
		slash->Init();
		single->Init();
		guard->Init();
		normal->Init();
		knock->Init();


		normal->SetBoss(this);

		knock->SetDam(KnockDam);
		smash->SetDam(MeteoDam);
		slash->SetDam(UltDam);
		single->SetDam(ShotDam);

	MaxHP = m_HP;

	GhostSize = 0;
	_aPhase = ATTACK_SHOT;
	/*ActionTimer = 1;

	
	_charaState = STATE_INTER;
	m_AreaState = AREA_SET;*/
	//CSV���[�h
	bonesize = m_fbxObject->GetBoneSize();
	//{
	bonepos.resize(19);
	bonemat.resize(19);
	s_color.resize(19);
	e_color.resize(19);
	s_scale.resize(19);
	e_scale.resize(19);
	m_Life.resize(19);

	m_HP = 10;
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
	&FiveBoss::Smash,
	&FiveBoss::Slash,
	&FiveBoss::Single
};

void FiveBoss::ActionSet(ActionPhase phase, InterAttack* attack)
{
	if (_aPhase == phase)
	{
		RandAction = 0;
		if (attack->GetActionEnd())
		{
			ActionTimer++;
			shot->SetCanRand(0);

			shot->SetActionEnd(false);
			_aPhase = ATTACK_SHOT;
		}
	}
}

void FiveBoss::Action()
{
	smash->SetBoss(this);
	shot->SetBoss(this);
	slash->SetBoss(this);
	single->SetBoss(this);
	guard->SetBoss(this);
	knock->SetBoss(this);

	////��Ԉڍs(charastate�ɍ��킹��)
	//if (m_HP > 0.0f) {
	if(GhostSize<6)
	(this->*attackTable[_aPhase])();

	
	//}
	if(KnockTimer%KnockInter==0)
	//if (Input::GetInstance()->TriggerButton(Input::X))
		knock->setKnockF(true);
	if (GuardCount==0&& m_HP < MaxHP / 2) {
		guard->SetGuardStart(true);
		GuardCount++;
		//guard->SetGuardStart(true);
	}
	if (guard->GetGuardStart())
		m_Magnification = 0.f;
	else
		m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "Magnification")));


	knock->Upda();
	guard->Upda();

	/// <summary>
	/// �U���[�RWAY
	/// </summary>
	for (auto i = 0; i < ghosts.size(); i++)
	{
		if (slash->GetActionEnd() && GhostSize >= 5)
			ghosts[i]->SetCleanGhost(true);
		else
			ghosts[i]->SetCleanGhost(false);
	}
	////ActionSet(ATTACK_SHOT, shot);
	ActionSet(ATTACK_IMPACT, smash);
	ActionSet(ATTACK_SLASH, slash);
	ActionSet(ATTACK_SINGLESHOT, single);

	if (_aPhase == ATTACK_SHOT)ActionTimer++;

	{
		for (auto i = 0; i < ghosts.size(); i++) {
			if (ghosts[i]->GetState() != Ghost::STATE_DARKOTI)continue;

			ghosts[i]->SetFivePos(m_Position);
		}

		for (auto i = 0; i < ghosts.size(); ++i) {
			for (auto j = 0; j < ghosts.size(); ++j) {
				XMFLOAT3 ghostpos = ghosts[i]->GetPosition();
				XMFLOAT3 ghostpos2 = ghosts[j]->GetPosition();
				if ((i == j)) { continue; }
				if ((!ghosts[i]->GetAlive()) || (!ghosts[j]->GetAlive())) { continue; }
				if ((!ghosts[i]->GetCollide()) || (!ghosts[j]->GetCollide())) { continue; }
				if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
					ghosts[i]->GhostCollision(ghostpos2);
					ghosts[j]->GhostCollision(ghostpos);
				}
			}
		}
	}

	for (auto i = 0; i < ghosts.size(); i++)
	{
		if (ghosts[i]->GetStateSpawn())
		{
			GhostSize--;
			ghosts[i]->SetCollide(false);
			ghosts[i]->SetCleanGhost(false);
			ghosts[i]->SetStateSpawn(false);
		}
	}
	mt19937 mt{ std::random_device{}() };

	//single->Upda();
	if (shot->GetCanRand() > 0&& shot->GetPhase()==ShotAttack::Phase::END) {
		//�ʏ�U��
		if (GhostSize > 0 && GhostSize < 4)
		{
			shot->SetActionEnd(true);
			shot->SetIdleDam(false);
			slash->SetActionEnd(false);
			_aPhase = ATTACK_SLASH;
		} else if (GhostSize == 4)
		{
			shot->SetActionEnd(true);
			shot->SetIdleDam(false);
			smash->SetActionEnd(false);
			_aPhase = ATTACK_IMPACT;
		} else if (GhostSize == 5)
		{
			shot->SetActionEnd(true);
			shot->SetIdleDam(false);
			slash->SetActionEnd(false);
			_aPhase = ATTACK_SLASH;
		}
	}
	Helper::GetInstance()->Clamp(GhostSize, 0, 5);

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

	if (bonesize == 0) {

	}
	for (auto i = 0; i < 19; i++) {
	}


	//�ǂ����g���΂�����������Ȃ���������ۗ�
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
	for (auto i = 0; i < 19; i++)
	{
		m_fbxObject->GetBoneIndexMat(i, bonemat[i]);
		MatTranstoPos(bonemat[i], bonepos[i]);

		s_color[i] = { 1.0f,0.4f,1.0f,0.50f };
		e_color[i] = { 0.0f,0.0f,0.0f,0.0f };
		s_scale[i] = 2.0f;
		e_scale[i] = 0.0f;
		m_Life[i] = 50;

		//ParticleEmitter::GetInstance()->FireEffect(m_Life[i],bonepos[i], s_scale[i], e_scale[i], s_color[i], e_color[i]);

	}
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
	single->Draw(dxCommon);

	shot->Draw(dxCommon);
	knock->Draw(dxCommon);
	slash->Draw(dxCommon);

	Fbx_Draw(dxCommon); guard->Draw(dxCommon);

}

void FiveBoss::MatTranstoPos(XMMATRIX trans, XMFLOAT3& m_Pos)
{
	m_Pos.x = trans.r[3].m128_f32[0]; // GetPosition().x;
	m_Pos.y = trans.r[3].m128_f32[1];
	m_Pos.z = trans.r[3].m128_f32[2];
}
void FiveBoss::InitAwake() {

}
void FiveBoss::EndRollAction()
{

}
