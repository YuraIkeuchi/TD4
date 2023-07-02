#include "SevenBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Player.h"
//生成
SevenBoss::SevenBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DJ);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	bossstuneffect.reset(new BossStunEffect());
	bossstuneffect->Initialize();
}
//初期化
bool SevenBoss::Initialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 2.2f;

	_charaState = STATE_INTER;
	//CSVロード
	CSVLoad();
	return true;
}
//スキップ時の初期化
void SevenBoss::SkipInitialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.1f,0.1f,0.1f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
}
//CSV
void SevenBoss::CSVLoad() {
	
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "HeartTarget")));

	m_MaxHp = m_HP;
}

void (SevenBoss::* SevenBoss::stateTable[])() = {
	&SevenBoss::InterValMove,//動きの合間
	&SevenBoss::Polter,//ポルターガイスト
	&SevenBoss::ThrowBound,//投げる
	&SevenBoss::BirthAvatar,//偽物のボス
	&SevenBoss::BulletCatch,//弾を吸収
	&SevenBoss::Stun,//スタン
};
//行動
void SevenBoss::Action() {
	//状態移行(charastateに合わせる)
	if (m_HP > 0.0f) {
		(this->*stateTable[_charaState])();
	}
	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	if (_charaState != STATE_CATCH) {
		CollideBul(_playerBulA, Type::CIRCLE);
	}
	//プレイヤーの当たり判定
	ColPlayer();
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
	Helper::GetInstance()->Clamp(m_HP, -1.0f, m_MaxHp);

	//障害物
	for (Poltergeist* newpolter : poltergeist) {
		if (newpolter != nullptr) {
			newpolter->Update();
		}
	}

	//障害物の削除
	for (int i = 0; i < poltergeist.size(); i++) {
		if (poltergeist[i] == nullptr) {
			continue;
		}

		if (!poltergeist[i]->GetAlive()) {
			poltergeist.erase(cbegin(poltergeist) + i);
		}
	}

	//偽物のボス
	for (InterBoss* newboss : avatarboss) {
		if (newboss != nullptr) {
			newboss->Update();
		}
	}

	//偽物のボスの削除
	for (int i = 0; i < avatarboss.size(); i++) {
		if (avatarboss[i] == nullptr) {
			continue;
		}

		if (avatarboss[i]->GetHP() <= 0.0f) {
			avatarboss.erase(cbegin(avatarboss) + i);
			m_AvatarCount--;
		}
	}

	//吸収エフェクト
	for (AbsorptionEffect* neweffect : abseffect) {
		if (neweffect != nullptr) {
			neweffect->Update();
		}
	}

	//吸収エフェクト
	for (int i = 0; i < abseffect.size(); i++) {
		if (abseffect[i] == nullptr) {
			continue;
		}

		if (!abseffect[i]->GetAlive()) {
			abseffect.erase(cbegin(abseffect) + i);
		}
	}

	bossstuneffect->SetBasePos(m_Position);
	bossstuneffect->Update();
}
//ポーズ
void SevenBoss::Pause() {
}
//エフェクト描画
void SevenBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;
	bossstuneffect->Draw(dxCommon);

	//吸収エフェクト
	for (AbsorptionEffect* neweffect : abseffect) {
		if (neweffect != nullptr) {
			neweffect->Draw(dxCommon);
		}
	}
}
//描画
void SevenBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	if (m_HP > 0.0f) {
		//障害物
		for (Poltergeist* newpolter : poltergeist) {
			if (newpolter != nullptr) {
				newpolter->Draw(dxCommon);
			}
		}

		//偽物のボス
		for (InterBoss* newboss : avatarboss) {
			if (newboss != nullptr) {
				newboss->Draw(dxCommon);
			}
		}

		EffecttexDraw(dxCommon);
	}
}
//ImGui
void SevenBoss::ImGui_Origin() {
	ImGui::Begin("Seven");
	ImGui::Text("HP:%f", m_HP);
	ImGui::Text("Absorption::%d", m_Absorption);
	ImGui::Text("Rand::%d", int(_charaState));
	ImGui::End();
}
//インターバル
void SevenBoss::InterValMove() {
	const int l_LimitTimer = 100;
	m_InterVal++;
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 2);
	if (m_InterVal == l_LimitTimer) {
		//行動を決めて次の行動に移る
		m_AttackRand = int(l_RandomMove(mt));

		if (m_AttackRand == 0) {
			_charaState = STATE_BOUND;
			m_InterVal = {};
		}
		else if(m_AttackRand == 1) {
			_charaState = STATE_POLTER;
			m_InterVal = {};
		}
		else {
			if (m_AvatarCount == 0) {
				_charaState = STATE_AVATAR;
				m_InterVal = {};
			}
			else {
				m_InterVal = l_LimitTimer - 1;
			}
		}
	}
}
//ポルターガイスト
void SevenBoss::Polter() {
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	const int l_LimitTimer = 200;
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Normal");
		m_MoveTimer = {};
		m_AttackCount++;
		//二回攻撃したら吸収行動に移行する
		if (m_AttackCount != 2) {
			_charaState = STATE_INTER;
		}
		else {
			_charaState = STATE_CATCH;
			m_Color = { 1.0f,0.0f,0.0f,1.0f };
		}
	}
}
//バウンド弾
void SevenBoss::ThrowBound() {
	const int l_LimitTimer = 200;
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Bound");
		m_MoveTimer = {};
		m_AttackCount++;

		//二回攻撃したら吸収行動に移行する
		if (m_AttackCount != 2) {
			_charaState = STATE_INTER;
		}
		else {
			_charaState = STATE_CATCH;
			m_Color = { 1.0f,0.0f,0.0f,1.0f };
		}
	}
}
//偽物のボスを生む
void SevenBoss::BirthAvatar() {
	const int l_LimitTimer = 100;
	m_Color = { 0.0f,0.0f,1.0f,1.0f };
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		for (int i = 0; i < AVATAR_NUM; i++) {
			InterBoss* boss;
			boss = new AvatarBoss();
			boss->Initialize();
			if (i == 0) {
				boss->SetPosition({ 20.0f,3.0f,10.0f });
			}
			else {
				boss->SetPosition({ -20.0f,3.0f,10.0f });
			}
			avatarboss.push_back(boss);
			m_AvatarCount++;
		}
		m_AttackCount++;
		m_MoveTimer = {};
		//二回攻撃したら吸収行動に移行する
		if (m_AttackCount != 2) {
			_charaState = STATE_INTER;
		}
		else {
			_charaState = STATE_CATCH;
		}
	}
}
//ポルターガイストの生成
void SevenBoss::BirthPolter(const std::string& PolterName) {
	const int l_LimitTimer = 20;//障害物が動くまでの時間
	const int l_LimitTimer2 = 50;//障害物が動くまでの時間2
	if (PolterName == "Normal") {
		for (int i = 0; i < POLTER_NUM; i++) {
			//ノーツの発生
			Poltergeist* newpolter;
			newpolter = new Poltergeist();
			newpolter->Initialize();
			
			newpolter->SetPolterType(TYPE_FOLLOW);
			newpolter->SetTargetTimer(i * l_LimitTimer);
			if (i == 0) {
				newpolter->SetPosition({ m_Position.x + 3.0f,m_Position.y - 10.0f,m_Position.z });
			}
			else if (i == 1) {
				newpolter->SetPosition({ m_Position.x - 3.0f,m_Position.y - 10.0f,m_Position.z });
			}
			else if (i == 2) {
				newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z + 3.0f});
			}
			else {
				newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z - 3.0f });
			}
			poltergeist.push_back(newpolter);
		}
	}else if(PolterName == "Bound") {
		for (int i = 0; i < POLTER_NUM; i++) {
			//ノーツの発生
			Poltergeist* newpolter;
			newpolter = new Poltergeist();
			newpolter->Initialize();

			newpolter->SetPolterType(TYPE_BOUND);
			newpolter->SetTargetTimer(i * l_LimitTimer2);
			newpolter->SetBasePos(m_Position);
			newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z });
			newpolter->SetCircleSpeed(i * 90.0f);
			poltergeist.push_back(newpolter);
		}
	}
}
//弾を吸収
void SevenBoss::BulletCatch() {
	const int l_LimitTimer = 500;
	m_Color = { 1.0f,1.0f,0.0f,1.0f };
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CatchBul(_playerBulA);
	m_MoveTimer++;
	m_Absorption = true;
	if (m_MoveTimer == l_LimitTimer) {
		m_Color = { 0.0f,0.0f,1.0f,1.0f };
		m_MoveTimer = {};
		m_AttackCount = {};
		m_Absorption = false;
		_charaState = STATE_INTER;
	}
	if (m_MoveTimer % 6 == 0) {
		BirthParticle();
	}
}
//スタンした時
void SevenBoss::Stun() {
	const int l_LimitTimer = 500;
	m_MoveTimer++;
	m_Absorption = false;
	m_Color = { 0.0f,1.0f,1.0f,1.0f };
	if (m_MoveTimer == l_LimitTimer) {
		m_Stun = false;
		m_MoveTimer = {};
		m_InterVal = {};
		_charaState = STATE_INTER;
		bossstuneffect->SetAlive(false);
	}
}
//登場シーン
void SevenBoss::AppearAction() {
	Obj_SetParam();
}
//ボス撃破シーン
void SevenBoss::DeadAction() {

	Obj_SetParam();
}
//ボス撃破シーン(スロー)
void SevenBoss::DeadAction_Throw() {
	Obj_SetParam();
}
//弾の吸収判定
void SevenBoss::CatchBul(vector<InterBullet*> bullet)
{
	const float l_CatchRadius = 10.0f;
	const float l_StunRadius = 1.0f;
	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr && _bullet->GetAlive()) {
			_bullet->SetTargetPos(m_Position);
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_CatchRadius, m_Position.x, m_Position.z, l_CatchRadius)) {
				_bullet->SetBossCatch(true);
			}

			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_StunRadius, m_Position.x, m_Position.z, l_StunRadius) && (!m_Stun)) {
				BirthExplosion();
				bossstuneffect->SetAlive(true);
				m_Stun = true;
				m_AttackCount = {};
				_bullet->SetAlive(false);
				m_Absorption = false;
				_charaState = STATE_STUN;
			}
		}
	}
}
//爆発エフェクト
void SevenBoss::BirthExplosion() {
	float l_AddSize = 2.5f;
	const float RandScale = 3.0f;
	float s_scale = 0.3f * l_AddSize;
	float e_scale = (4.0f + (float)rand() / RAND_MAX * RandScale - RandScale / 2.0f) * l_AddSize;

	//色
	const float RandRed = 0.2f;
	const float red = 0.2f + (float)rand() / RAND_MAX * RandRed;
	const XMFLOAT4 s_color = { 0.9f, red, 0.1f, 1.0f }; //濃い赤
	const XMFLOAT4 e_color = { 0, 0, 0, 1.0f }; //無色

	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Randlife(10, 40);
	int l_Life = int(l_Randlife(mt));

	ParticleEmitter::GetInstance()->Explosion(l_Life, m_Position, l_AddSize, s_scale, e_scale, s_color, e_color);
}
//パーティクル
void SevenBoss::BirthParticle() {
	//ノーツの発生
	AbsorptionEffect* neweffect;
	neweffect = new AbsorptionEffect();
	neweffect->Initialize();
	neweffect->SetBasePos(m_Position);
	abseffect.push_back(neweffect);
}