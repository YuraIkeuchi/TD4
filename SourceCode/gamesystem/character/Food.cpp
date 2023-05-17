#include "Food.h"
#include "ModelManager.h"
#include "imgui.h"
#include "CsvLoader.h"
#include "ParticleEmitter.h"
#include "Collision.h"
#include "HungerGauge.h"
#include "Player.h"
#include <random>
Food::Food() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Cube);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool Food::Initialize() {
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(-50, 60);
	uniform_int_distribution<int> l_distZ(-55, 55);
	m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
	m_Scale = { 0.7f,0.7f,0.7f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_LockOn = false;
	m_Alive = true;
	return true;
}

//更新
void Food::Update() {
	//タイプによって色を一旦変えてる
	Obj_SetParam();
	//食料生成
	BirthFood();
	//パーティクル
	Particle();
	//当たり判定
	//Collision();
}

//描画
void Food::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui描画
void Food::ImGuiDraw() {
	ImGui::Begin("Food");
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::End();
}
//パーティクル
void Food::Particle() {
	XMFLOAT4 s_color = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 e_color = { 0.0f,0.0f,1.0f,1.0f };
	float s_scale = 1.5f;
	float e_scale = 0.0f;
	if (m_Alive) {
		ParticleEmitter::GetInstance()->FireEffect(10, m_Position, s_scale, e_scale, s_color, e_color);
	}
}
//当たり判定
bool Food::Collision() {
	float l_Radius = 1.2f;//当たり判定の範囲
	float l_AddHunger = 1.0f;//加算される飢餓ゲージ
	XMFLOAT3 m_PlayerPos = Player::GetInstance()->GetPosition();
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, m_PlayerPos.x, m_PlayerPos.z, l_Radius) && m_Alive) {
		m_Alive = false;
		m_LockOn = false;
		HungerGauge::GetInstance()->AddNowHunger(HungerGauge::GetInstance()->GetNowHunger() + l_AddHunger);
		return true;
	}
	else {
		return false;
	}

	return true;
}
//食料生成
void Food::BirthFood() {
	if (!m_Alive) {
		m_Timer++;
		//描画バグ起きるから先に座標セット
		if (m_Timer == 20) {
			//乱数指定
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_distX(-50, 60);
			uniform_int_distribution<int> l_distZ(-55, 55);
			m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
		}
		//一定時間で生成される
		if (m_Timer == 100) {
			m_Alive = true;
			m_Timer = 0;
		}
	}
}