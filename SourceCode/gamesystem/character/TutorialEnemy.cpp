﻿#include "TutorialEnemy.h"
#include "Player.h"
#include "Collision.h"
#include "ModelManager.h"
#include "Helper.h"
#include "ParticleEmitter.h"
#include <random>

//モデル読み込み
TutorialEnemy::TutorialEnemy() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BulEnemy);
}
//初期化
bool TutorialEnemy::Initialize() {

	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	m_Scale = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.x = 90.f;
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<int> rnd_pos(-50, 50);

	float posX = (float)rnd_pos(mt);
	float posZ = (float)rnd_pos(mt);
	m_Position = { posX,5.0f ,posZ };
	ret = true;
	HP = 1;
	isAlive = true;
	return true;
}
//行動
void TutorialEnemy::Action() {
	if (!isStop) {
		Appearance();
		RushAction();
	}
	Obj_SetParam();
	OnCollision();
	ColPlayer();
	Particle();
}
//描画
void TutorialEnemy::Draw(DirectXCommon* dxCommon) {

	if (m_Color.w <= 0.f)return;
	IKEObject3d::PreDraw();
	Obj_Draw();
}
//ImGui描画
void TutorialEnemy::ImGuiDraw() {
	ImGui::Begin("Enemy");
	//ImGui::Text("time %f", t);
	ImGui::Text("RotOld %f", old);

	ImGui::Text("EnePosX:%f", m_Position.x);
	ImGui::Text("EnePosZ:%f", m_Position.z);
	ImGui::End();
}
//開放
void TutorialEnemy::Finalize() {

}

//パーティクル
void TutorialEnemy::Particle() {
	if (!isAlive)return;
	XMFLOAT4 s_color = { 1.0f,0.5f,1.0f,1.5f };
	XMFLOAT4 e_color = { 1.0f,0.5f,1.0f,0.1f };
	//float s_scale = 3.0f;
	float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->FireEffect(10, m_Position, s_scale, e_scale, s_color, e_color);
}

void TutorialEnemy::Appearance() {
	//	if (Rush)return;
	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	PositionA = { l_player.x,l_player.y,l_player.z };
	PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	constexpr float AddScaling = 0.03f;

	m_Scale.x += AddScaling;
	m_Scale.y += AddScaling;
	m_Scale.z += AddScaling;

	Helper::GetInstance()->Clamp(m_Scale.x, 0.f, 1.5f);
	Helper::GetInstance()->Clamp(m_Scale.y, 0.f, 1.5f);
	Helper::GetInstance()->Clamp(m_Scale.z, 0.f, 1.5f);

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	m_Rotation.y = RottoPlayer * 60.f + 180.f;

	if (m_Scale.x >= 1.5f || m_Scale.y >= 1.5f || m_Scale.z >= 1.5f) {
		Rush = true;
	}




}

void TutorialEnemy::RushAction() {
	if (!Rush)return;
	s_scale += 0.02f;

	Helper::GetInstance()->Clamp(s_scale, 0.f, 3.f);

	// positionA - positionB;
   //回転軸をプレイヤーの方に
	   //向きかえる
	if (ret) {
		MoveTimer = 0;


		XMFLOAT3 l_player = Player::GetInstance()->GetPosition();
		PositionA = { l_player.x,l_player.y,l_player.z };
		PositionB = { m_Position.x,m_Position.y,m_Position.z };
		//プレイヤーと敵のベクトルの長さ(差)を求める
		SubVector = XMVectorSubtract(PositionB, PositionA);
		RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		//イージングカウンタ＋＋
		t += 0.03f;
		//Rotation反映
		//if (randMove > 50) {
		if (canRot)
			m_Rotation.y = Easing::EaseOut(t, old * 50.f + 180.f, RotY * 50.f + 180.f);
		if (t >= 1.f) {
			canRot = true;
			ret = false;//	} else {
		}	//m_Rotation.y = Easing::EaseOut(t, old * 60.f + 180.f, RotY * 60.f + 180.f - 360.f);
			//}
	} else {

		t = 0.f;
		old = RotY;
		MoveTimer++;
		if (MoveTimer > 120) {
			randMove = rand() % 100;
			ret = true;
		}
	}
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));

	move = XMVector3TransformNormal(move, matRot);

	//向き変えてる最中は動き止める
	bool stopMove = !(t > 0.01f);
	if (stopMove) {
		if (isAlive) {
			m_Position = {
					m_Position.x + move.m128_f32[0] * 4.f,
				m_Position.y,
				m_Position.z + move.m128_f32[2] * 4.f
			};
		}
	}
	Helper::GetInstance()->Clamp(t, 0.f, 1.f);

}


