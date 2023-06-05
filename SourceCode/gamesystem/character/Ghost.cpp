#include "Ghost.h"
#include "VariableCommon.h"
#include "Collision.h"
#include "HungerGauge.h"
#include <random>
#include "Player.h"
Ghost::Ghost() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Ghost);
	model_follow = ModelManager::GetInstance()->GetModel(ModelManager::Buddy);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool Ghost::Initialize() {
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(-50, 60);
	uniform_int_distribution<int> l_distZ(-55, 55);
	m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
	m_Rotation.y = -PI_90;
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Color = { 1.0f,1.0f,1.0f,0.7f };
	uniform_int_distribution<int> spawn(60, 90);
	kSpawnTimerMax = float(spawn(mt));
	_charaState = CharaState::STATE_SPAWN;
	_searchState = SearchState::SEARCH_NO;
	_followState = FollowState::Follow_NO;
	return true;
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Ghost::* Ghost::stateTable[])() = {
	&Ghost::None,//待機
	&Ghost::Spawm,
	&Ghost::Follow,//移動
	&Ghost::Search,//攻撃
};
//更新
void Ghost::Update() {
	float l_AddScale = 1.5f;//OBB用に少し大きめのスケールを取る
	m_OldPos = m_Position;
	m_OBBScale = { m_Scale.x + l_AddScale,m_Scale.y + l_AddScale, m_Scale.z + l_AddScale };
	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();
	//タイプによって色を一旦変えてる
	Obj_SetParam();
	//食料生成
	BirthGhost();
	//当たり判定(弾)
	BulletCollision();
	//当たり判定(プレイヤー)
	PlayerCollision();
	//食べ物をはこぶ
	CarryFood();
	Particle();
}
//描画
void Ghost::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void Ghost::ImGuiDraw() {
	ImGui::Begin("Ghost");
	ImGui::Text("%d", m_SearchTimer);
	ImGui::End();
}
//当たり判定(弾)
bool Ghost::BulletCollision() {
	float l_AddHungerMax = HungerGauge::m_Hungervalue;//加算される最大飢餓ゲージ
	if (Player::GetInstance()->BulletCollide({ m_Position.x,0.0f,m_Position.z }, m_Object->GetMatrot(), m_OBBScale, m_Catch) && (m_Alive)) {
		m_Catch = true;
		if (Player::GetInstance()->GetBulletType() == BULLET_FORROW) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Get_Follower.wav", VolumManager::GetInstance()->GetSEVolum());
			HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() + l_AddHungerMax);
			HungerGauge::GetInstance()->SetNowHunger(HungerGauge::GetInstance()->GetNowHunger() + l_AddHungerMax);
			HungerGauge::GetInstance()->SetCatchCount(HungerGauge::GetInstance()->GetCatchCount() + 1);
			_charaState = CharaState::STATE_FOLLOW;
			_followState = FollowState::Follow_START;
			m_Object->SetModel(model_follow);
			m_Follow = true;
		}
		else {
			_charaState = CharaState::STATE_SEARCH;
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Get_Searcher.wav", VolumManager::GetInstance()->GetSEVolum());
		}
		return true;
	}
	else {
		return false;
	}
	return true;
}
//パーティクル
void Ghost::Particle() {
	XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 s_color2 = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 e_color2 = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 s_color3 = { 0.0f,1.0f,0.0f,1.0f };
	XMFLOAT4 e_color3 = { 0.0f,1.0f,0.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	if (m_Alive) {
		if (_charaState == CharaState::STATE_NONE) {
			m_Color = { 1.0f,1.0f,1.0f,0.7f };
			m_Scale = { 0.5f,0.5f,0.5f };
			//ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color, e_color);
		}
		else if (_charaState == CharaState::STATE_FOLLOW) {
			m_Color = { 1.0f,1.0f,1.0f,1.0f };
			m_Scale = { 0.6f,0.6f,0.6f };
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color2, e_color2);
		}
		else {
			//ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color3, e_color3);
		}
	}
}
//当たり判定(プレイヤー)
bool Ghost::PlayerCollision() {
	if (Player::GetInstance()->PlayerCollide(m_Position) && (_charaState == CharaState::STATE_FOLLOW)) {
		m_Position = m_OldPos;
		return true;
	}
	else {
		return false;
	}

	return true;
}
//当たり判定(ゴースト同士)
void Ghost::GhostCollision(const XMFLOAT3& pos) {
	m_Position.x += sin(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
	m_Position.z += cos(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
}
//食料生成
void Ghost::BirthGhost() {
	if (!m_Alive) {
		m_ResPornTimer++;
		//描画バグ起きるから先に座標セット
		if (m_ResPornTimer == 20) {
			_charaState = CharaState::STATE_SPAWN;
			_searchState = SearchState::SEARCH_NO;
			//乱数指定
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_distX(-50, 60);
			uniform_int_distribution<int> l_distZ(-55, 55);
			m_Object->SetModel(m_Model);
			m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
			uniform_int_distribution<int> spawn(30, 45);
			kSpawnTimerMax = float(spawn(mt));
			m_Color = { 1.0f,1.0f,1.0f,0.7f };
			m_Catch = false;
			m_Search = false;
			m_Follow = false;
			m_SearchTimer = 0;
			m_SpawnTimer = 0;
		}
		//一定時間で生成される
		if (m_ResPornTimer == 100) {
			m_Alive = true;
			m_ResPornTimer = 0;
			m_Scale = { 0.5f,0.5f,0.5f };
		}
	}
}
//何もない状態
void Ghost::None() {
	noneTimer += 0.05f;

	float size = sinf(noneTimer) * 0.05f;
	m_Position.x += cosf(m_Rotation.y * (PI_180 / XM_PI)) * size;
	m_Position.y = sinf(noneTimer) * 1.2f;
	m_Position.z += sinf(m_Rotation.y * (PI_180 / XM_PI)) * size;

}
//生まれる状態
void Ghost::Spawm() {
	m_SpawnTimer += 1.0f / kSpawnTimerMax;

	m_Rotation.y = Ease(In, Quad, m_SpawnTimer, -(PI_360 + PI_90), -PI_90);

	float scale = Ease(Out, Elastic, m_SpawnTimer, 0.0f, 0.5f);
	m_Scale = { scale,scale,scale };

	Helper::GetInstance()->Clamp(m_SpawnTimer, 0.0f, 1.0f);
	if (m_SpawnTimer == 1.0f) {
		_charaState = CharaState::STATE_NONE;
	}
}
//追従
void Ghost::Follow() {
	float l_Vel = 0.35f;//速度
	XMFLOAT3 l_playerPos = Player::GetInstance()->GetPosition();
	Helper::GetInstance()->FollowMove(m_Position, l_playerPos, l_Vel);
	m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, l_playerPos, -PI_90);
}
//探索
void Ghost::Search() {
	const int l_LimitTimer = 300;
	const float l_Vel = 0.3f;
	XMFLOAT3 l_playerPos = Player::GetInstance()->GetPosition();
	//追従
	if (_searchState == SearchState::SEARCH_START) {
		Helper::GetInstance()->FollowMove(m_Position, m_SearchPos, l_Vel);
		m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, m_SearchPos, -PI_90);
		//サーチ状態から一定時間立つと存在消去
		m_SearchTimer++;
		if (m_SearchTimer >= l_LimitTimer) {
			m_Scale = { 0.0f,0.0f,0.0f };
			m_Alive = false;
		}
	}
	else if (_searchState == SearchState::SEARCH_END) {
		Helper::GetInstance()->FollowMove(m_Position, l_playerPos, l_Vel);
		m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, l_playerPos, -PI_90);
	}
}
//探索スタート
void Ghost::StartSearch(const XMFLOAT3& pos) {
	_searchState = SearchState::SEARCH_START;
	m_Search = true;
	m_SearchPos = pos;
}
//探索終了
void Ghost::EndSearch() {
	_searchState = SearchState::SEARCH_END;
}
//食べ物を運ぶ
void Ghost::CarryFood() {
	float l_Radius = 1.0f;//当たり判定
	float l_AddHunger = HungerGauge::m_Hungervalue;//加算される気がゲージ
	XMFLOAT3 l_playerPos = Player::GetInstance()->GetPosition();
	if ((_searchState == SearchState::SEARCH_END) && (m_Alive)) {
		if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_playerPos.x, l_playerPos.z, l_Radius)) {
			m_Scale = { 0.0f,0.0f,0.0f };
			m_Alive = false;
			m_Search = false;
			m_Catch = false;
			m_Limit = {};
			HungerGauge::GetInstance()->RecoveryNowHunger(HungerGauge::GetInstance()->GetNowHunger() + l_AddHunger);
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Get_Food.wav", VolumManager::GetInstance()->GetSEVolum());
		}
	}
}