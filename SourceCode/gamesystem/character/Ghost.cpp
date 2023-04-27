#include "Ghost.h"
#include "ModelManager.h"
#include "imgui.h"
#include "CsvLoader.h"
#include "ParticleEmitter.h"
#include "Collision.h"
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
	return true;
}

//更新
void Ghost::Update() {
	//タイプによって色を一旦変えてる
	Obj_SetParam();
	//食料生成
	BirthGhost();
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
	ImGui::Text("Pos.X:%f", m_Position.x);
	ImGui::Text("Timer:%d", m_Timer);
	if (m_Alive) {

	}
	ImGui::End();
}
//パーティクル
void Ghost::Particle() {
	XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	if (m_Alive) {
		ParticleEmitter::GetInstance()->FireEffect(10, m_Position, s_scale, e_scale, s_color, e_color);
	}
}
//当たり判定
bool Ghost::Collision() {
	if (player->BulletCollide(m_Position) && m_Alive) {
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}
	/*float l_Radius = 1.0f;
	XMFLOAT3 m_PlayerPos = player->GetPosition();
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, BulletPos.x, BulletPos.z, l_Radius) && m_Alive) {
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}*/

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
			m_Alive = true;
			m_Timer = 0;
		}
	}
}