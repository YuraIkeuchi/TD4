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

	m_HP = 5.f;
}

bool FirstBoss::Initialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	m_Rotation.y = -90.f;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "pos")));
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "HeartTarget")));
	m_Radius = 5.2f;
	m_MaxHp = m_HP;

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
	Obj_SetParam();
}

void FirstBoss::DeadAction()
{
	const float l_AddAngle = 5.0f;
	m_DeathTimer++;
	const int l_BaseTarget = 50;
	if (m_DeathTimer == 1) {
		m_Position = { 0.0f,30.0f,20.0f };
		m_Rotation = { 0.0f,0.0f,0.0f };
	}
	else if (m_DeathTimer >= 2 && m_DeathTimer < 300) {
		//sin波によって上下に動く
		m_Angle += l_AddAngle;
		m_Angle2 = m_Angle * (3.14f / 180.0f);
		m_Position.x = (sin(m_Angle2) * 15.0f + 15.0f);
		DeathParticle();
	}
	else {
		m_Gravity = 0.05f;
		//飛ぶような感じにするため重力を入れる
		m_AddPower -= m_Gravity;
		Helper::GetInstance()->CheckMax(m_Position.y, 6.0f, m_AddPower);
	}

	Obj_SetParam();
}

void FirstBoss::DeadAction_Throw()
{
	Obj_SetParam();
}

void FirstBoss::DeathParticle()
{
	const XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	const XMFLOAT4 e_color = { 0.0f,0.0f,1.0f,1.0f };
	float s_scale = 5.0f;
	float e_scale = 0.0f;
	float l_velocity = 0.5f;
	for (int i = 0; i < 3; ++i) {
		ParticleEmitter::GetInstance()->DeathEffect(50, { m_Position.x,(m_Position.y - 1.0f),m_Position.z }, s_scale, e_scale, s_color, e_color, l_velocity);
	}
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
//攻撃後のインターバル
void FirstBoss::InterValMove()
{
}
//攻撃の選択
void FirstBoss::Choice()
{
}
//ロックオン突進
void FirstBoss::RockOnAttack()
{
}
//ランダム攻撃
void FirstBoss::RandAttack()
{
}
//当たり屋?
void FirstBoss::Hit()
{
}
//行動の終わり(プレイヤーから逃げる)
void FirstBoss::EndMove()
{
	const int l_EndLimit = 100;
	const float l_AddAngle = 5.0f;
	float l_AddSpeed = {};
	const float l_FollowSpeed = 0.3f;
	m_EndTimer++;

	//二点間の距離計算
	m_Length = Helper::GetInstance()->ChechLength({ m_Position.x,0.0f,m_Position.z }, {});
	//次のCDを狙う
	if (m_Length > 0.5f) {
		Helper::GetInstance()->FollowMove(m_Position, {}, l_FollowSpeed);
		m_AddPower -= m_Gravity;
		if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower)) {
			m_AddPower = 0.5f;
		}
	}
	else {
		_charaState = STATE_INTER;
		m_EndTimer = {};
		m_AddPower = {};
	}
}