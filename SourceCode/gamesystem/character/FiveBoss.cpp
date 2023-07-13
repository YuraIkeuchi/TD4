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

	//ダーク琴子モデル
		//モデル初期化と読み込み
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
	m_Radius = 5.2f;
	m_Scale = { 1.2f,0.8f,1.2f };
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	//m_Rotation.y = -90.f;
_aPhase = ATTACK_SHOT;
	/*ActionTimer = 1;

		
	_charaState = STATE_INTER;
	m_AreaState = AREA_SET;*/
	//CSVロード
	bonesize =  m_fbxObject->GetBoneSize();
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

};

void FiveBoss::ActionSet(ActionPhase phase, InterAttack* attack)
{
	if (_aPhase == phase)
	{
		RandAction = 0;
		if (attack->GetActionEnd())
		{
			ActionTimer++;
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
	////状態移行(charastateに合わせる)
	//if (m_HP > 0.0f) {
	(this->*attackTable[_aPhase])();
	//}

	/// <summary>
	/// 攻撃ー３WAY
	/// </summary>

	//ActionSet(ATTACK_SHOT, shot);
	ActionSet(ATTACK_IMPACT, smash);
	ActionSet(ATTACK_SLASH, slash);

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

	for(auto i=0;i<ghosts.size();i++)
	{
		if(ghosts[i]->GetStateSpawn())
		{
			GhostSize--;
			ghosts[i]->SetStateSpawn(false);
		}
	}
	mt19937 mt{ std::random_device{}() };
	//if (_aPhase == ATTACK_SHOT && ActionTimer % 120 == 0) {
	//	RandAction = rand()%3+1;

	//	if (shot->GetDarkCount()<5)
	//	{
	//		shot->SetActionEnd(false);
	//		_aPhase = ATTACK_SHOT;
	//	}
	//	else {
	//		if (RandAction == 2)
	//		{
	//			smash->SetActionEnd(false);
	//			_aPhase = ATTACK_IMPACT;
	//		}
	//		if (shot->GetDarkCount() >= 5)
	//		{
	//			slash->SetActionEnd(false);
	//			_aPhase = ATTACK_SLASH;
	//		}
	//	}
	//}
	JudgAttack = 100;

	bool PhaseAttack = JudgAttack > 60;
	if (shot->GetIdleDam()) {
		//通常攻撃
		if (GhostSize == 1)
		{
			shot->SetActionEnd(true);
			shot->SetIdleDam(false);
			smash->SetActionEnd(false);
			_aPhase = ATTACK_IMPACT;
		} else if (GhostSize == 2)
		{
			shot->SetActionEnd(true);
			shot->SetIdleDam(false);
			slash->SetActionEnd(false);
			_aPhase = ATTACK_SLASH;
		}
	}

	
	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//プレイヤーの当たり判定
	ColPlayer();
	//OBJのステータスのセット
	//Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);


	//HPが半分切ったら強化
	if (m_HP < m_MaxHp / 2) {
		isStrong = true;
	}
	//基礎パラメータ設定

	Fbx_SetParam();

	if (bonesize == 0) {
	
	}
	for (auto i = 0; i < 19; i++) {
	}

		
	//どっち使えばいいか分からなかったから保留
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

			ParticleEmitter::GetInstance()->FireEffect(m_Life[i],bonepos[i], s_scale[i], e_scale[i], s_color[i], e_color[i]);

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
	shot->Draw(dxCommon);
	slash->Draw(dxCommon);
	Fbx_Draw(dxCommon);

}

void FiveBoss::MatTranstoPos(XMMATRIX trans,XMFLOAT3& m_Pos)
{
	m_Pos.x = trans.r[3].m128_f32[0]; // GetPosition().x;
	m_Pos.y = trans.r[3].m128_f32[1];
	m_Pos.z = trans.r[3].m128_f32[2];
}
void FiveBoss::InitAwake() {

}


void FiveBoss::EndRollAction() {

	Fbx_SetParam();
	//どっち使えばいいか分からなかったから保留
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}