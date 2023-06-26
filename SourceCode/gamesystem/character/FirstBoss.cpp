#include "FirstBoss.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Player.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include <random>

void (FirstBoss::* FirstBoss::stateTable[])() = {
	&FirstBoss::InterValMove,
	&FirstBoss::Choice,

	&FirstBoss::EndMove,
};

FirstBoss::FirstBoss()
{
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DJ);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	confueffect.reset(new ConfuEffect());
	confueffect->Initialize();

	noteeffect.reset(new NoteEffect());
	noteeffect->Initialize();
}

bool FirstBoss::Initialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	//m_Rotation.y = -90.f;

	/*ActionTimer = 1;

	m_Radius = 5.2f;

	_charaState = STATE_INTER;
	m_AreaState = AREA_SET;*/
	//CSVロード
	CSVLoad();
	return true;
}

void FirstBoss::SkipInitialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
}

void FirstBoss::Pause()
{
}

void FirstBoss::Action()
{
	//状態移行(charastateに合わせる)
	if (m_HP > 0.0f) {
		(this->*stateTable[_charaState])();
	}

	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//プレイヤーの当たり判定
	ColPlayer();
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);


	//HPが半分切ったら強化
	if (m_HP < m_MaxHp / 2) {
		isStrong = true;
	}
}

void FirstBoss::AppearAction()
{
}

void FirstBoss::DeadAction()
{
}

void FirstBoss::DeadAction_Throw()
{
}

void FirstBoss::CSVLoad()
{
}

void FirstBoss::DeathParticle()
{
}

void FirstBoss::ImGui_Origin()
{
}

void FirstBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;

	confueffect->Draw(dxCommon);
	noteeffect->Draw(dxCommon);
}

void FirstBoss::Draw(DirectXCommon* dxCommon)
{
	Obj_Draw();
}

void FirstBoss::InterValMove()
{
}

void FirstBoss::Choice()
{
}

void FirstBoss::RockOnAttack()
{
}

void FirstBoss::RandAttack()
{
}

void FirstBoss::EndMove()
{
}