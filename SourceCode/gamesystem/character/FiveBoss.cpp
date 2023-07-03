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

	shot->Init();
	shot->SetBoss(this);
	
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
	//CSVロード
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
};

void FiveBoss::Action()
{
	////状態移行(charastateに合わせる)
	//if (m_HP > 0.0f) {
		(this->*attackTable[_aPhase])();
	//}
	if(Input::GetInstance()->TriggerButton(Input::Y))
	{
		_aPhase = ATTACK_SHOT;
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

	//どっち使えばいいか分からなかったから保留
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
	shot->Draw();
	Fbx_Draw(dxCommon);

}
