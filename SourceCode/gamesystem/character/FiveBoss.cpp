#include "FiveBoss.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Player.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Easing.h"
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

	noteeffect.reset(new NoteEffect());
	noteeffect->Initialize();
	//m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "hp")));
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
	m_HP =  static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "hp")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "HeartTarget")));

	//ノックバック頻度
	KnockInter = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "KnockInter")));

	//KnockBack
	KnockDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "KnockDam")));


		//MeteoShot
		MeteoDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "MeteoDam")));

		//WayShot
		ShotDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "ShotDam")));

		//Ultimate
		UltDam = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "UltDam")));

		noAction = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "noAction")));

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
	//CSVロード
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

	////状態移行(charastateに合わせる)
	//if (m_HP > 0.0f) {
	if(GhostSize<6)
	(this->*attackTable[_aPhase])();

	KnockTimer++;
	//}
	if(KnockTimer%KnockInter==0)
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
	/// 攻撃ー３WAY
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
	if (shot->GetCanRand() > 30&& shot->GetPhase()==ShotAttack::Phase::END) {
		//通常攻撃
		if (GhostSize > noAction && GhostSize < 4)
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
		} else if (GhostSize >= 5)
		{
			shot->SetActionEnd(true);
			shot->SetIdleDam(false);
			slash->SetActionEnd(false);
			_aPhase = ATTACK_SLASH;
		}
	}
	Helper::GetInstance()->Clamp(GhostSize, 0, 5);

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
	ImGui::Begin("Five");
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::End();
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
	const float l_AddFrame = 0.01f;
	m_EndTimer++;
	if (_EndState2 == END_SET2) {
		m_Position = { 10.0f,5.0f,-25.0f };
		m_Rotation = { 0.0f,180.0f,270.0f };
		m_Scale = { 0.8f,0.4f,0.8f };
		m_fbxObject->PlayAnimation(0);
		if (m_EndTimer == 980) {
			_EndState2 = END_RIGHT;
			m_AfterPos = { 7.0f,5.0f,-25.0f };
			m_View = true;
			_ViewType = VIEW_MOVE;
		}
	}
	else if (_EndState2 == END_RIGHT) {
		if (m_View) {
			if (_ViewType == VIEW_MOVE) {
				if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
					m_Frame = {};
					m_AfterPos = { 10.0f,5.0f,-25.0f };
					_ViewType = VIEW_RETURN;
				}
			}
			else {
				if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
					m_Frame = {};
					_ViewType = VIEW_MOVE;
					m_Position = { -10.0f,5.0f,-25.0f };
					m_AfterPos = { -7.0f,5.0f,-25.0f };
					_EndState2 = END_LEFT;
				}
			}
			SetEasePos();
		}
	}
	else if (_EndState2 == END_LEFT) {
		m_Rotation = { 0.0f,180.0f,90.0f };
		if (m_View) {
			if (_ViewType == VIEW_MOVE) {
				if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
					m_Frame = {};
					m_AfterPos = { -10.0f,5.0f,-25.0f };
					_ViewType = VIEW_RETURN;
				}
			}
			else {
				if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
					m_Frame = {};
					_ViewType = VIEW_MOVE;
					m_Position = { -3.0f,15.0f,-25.0f };
					m_AfterPos = { -3.0f,10.0f,-25.0f };
					_EndState2 = END_TOP;
				}
			}
			SetEasePos();
		}
	}
	else if (_EndState2 == END_TOP) {
		m_Rotation = { 0.0f,180.0f,180.0f };
		if (m_View) {
			if (_ViewType == VIEW_MOVE) {
				if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
					m_Frame = {};
					m_AfterPos = { -3.0f,15.0f,-25.0f };
					_ViewType = VIEW_RETURN;
				}
			}
			else {
				if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
				}
			}
			SetEasePos();
		}

		if (m_EndTimer == 1670) {
			_EndState2 = END_MOVE2;
			m_fbxObject->StopAnimation();
		}
	}
	else {
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_Position = { -3.0f,0.0f,-25.0f };
	}

	Fbx_SetParam();
	//どっち使えばいいか分からなかったから保留
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}

void FiveBoss::SetEasePos() {
	m_Position = {
Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
	Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
	};
}