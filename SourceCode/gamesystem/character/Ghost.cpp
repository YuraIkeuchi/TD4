#include "Ghost.h"
#include "ModelManager.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "ParticleEmitter.h"
#include "Collision.h"
#include "HungerGauge.h"
#include "Helper.h"
#include <random>
Ghost::Ghost() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Cube);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool Ghost::Initialize() {
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(-41, 50);
	uniform_int_distribution<int> l_distZ(-45, 45);
	m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	_charaState = CharaState::STATE_NONE;
	return true;
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Ghost::* Ghost::stateTable[])() = {
	&Ghost::None,//待機
	&Ghost::Follow,//移動
	&Ghost::Search,//攻撃
};
//更新
void Ghost::Update() {
	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();
	//タイプによって色を一旦変えてる
	Obj_SetParam();
	//食料生成
	//BirthGhost();
	//パーティクル
	Particle();
	//当たり判定
	Collision();
}
//描画
void Ghost::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui描画
void Ghost::ImGuiDraw() {
	ImGui::Begin("Ghost");
	ImGui::Text("State:%d", _charaState);
	ImGui::End();
}
//パーティクル
void Ghost::Particle() {
	XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color, e_color);
}
//当たり判定
bool Ghost::Collision() {
	float l_AddHungerMax = 5.0f;//加算される最大飢餓ゲージ
	if (player->BulletCollide(m_Position) && m_Alive) {
		HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() + l_AddHungerMax);
		m_Alive = false;
		m_Catch = true;
		if (player->GetBulletType() == BULLET_FORROW) {
			_charaState = CharaState::STATE_FOLLOW;
		}
		else {
			m_BasePos = m_Position;
			_charaState = CharaState::STATE_SEARCH;
		}
		return true;
	}
	else {
		return false;
	}
	return true;
}
//食料生成
void Ghost::BirthGhost() {
	if (!m_Alive) {
		m_Timer++;
		//描画バグ起きるから先に座標セット
		if (m_Timer == 20) {
			//乱数指定
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_distX(-41, 50);
			uniform_int_distribution<int> l_distZ(-45, 45);
			m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
		}
		//一定時間で生成される
		if (m_Timer == 100) {
			_charaState = CharaState::STATE_NONE;
			m_Alive = true;
			m_Timer = 0;
		}
	}
}
//何もない状態
void Ghost::None() {

}
//追従
void Ghost::Follow() {
	XMFLOAT3 l_player = player->GetPosition();
	float l_Vel = 0.15f;
	//追従
	Helper::GetInstance()->FollowMove(m_Position, l_player, l_Vel);
}
//探索
void Ghost::Search() {
	m_CircleSpeed += 1.0f;
	//円運動
	m_Position = Helper::GetInstance()->CircleMove(m_BasePos, m_CircleScale, m_CircleSpeed);
}