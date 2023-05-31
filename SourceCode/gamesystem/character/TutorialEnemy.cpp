#include "TutorialEnemy.h"
#include "Player.h"
#include "Collision.h"
#include "ModelManager.h"
#include "Helper.h"
#include "ParticleEmitter.h"
#include <random>
#include <Easing.h>


void (TutorialEnemy::* TutorialEnemy::commandTable[])() = {
	&TutorialEnemy::SpawnUpdate,//要素0
	&TutorialEnemy::WaitUpdate, //要素1
	&TutorialEnemy::LockOnUpdate,
	&TutorialEnemy::JumpUpdate,
};


//モデル読み込み
TutorialEnemy::TutorialEnemy() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::MobUsa);
}
//初期化
bool TutorialEnemy::Initialize() {

	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	m_Scale = { scale_,scale_,scale_ };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Rotation.x = 90.f;
	m_Position = {};
	ret = true;
	HP = 1;
	isAlive = true;
	return true;
}
//行動
void TutorialEnemy::Action() {
	//if (!isStop) {
		//関数ポインタで状態管理
	(this->*commandTable[static_cast<size_t>(commandState)])();
	//}
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
	//ImGui::Text("RotOld %f", RottoPlayer);

	ImGui::Text("EnePosX:%f", m_Position.x);
	ImGui::Text("EnePosY:%f", m_Position.y);
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
	ParticleEmitter::GetInstance()->FireEffect(10, m_Position, m_Scale.x, e_scale, s_color, e_color);
}

void TutorialEnemy::SpawnUpdate() {
	spawnTimer += 1.0f / kSpawnTimeMax;
	Helper::GetInstance()->Clamp(spawnTimer, 0.0f, 1.0f);

	float scale = Ease(Out, Quart, spawnTimer, 0.0f, scale_);
	m_Scale = { scale ,scale ,scale };

	GetRotation2Player();
	if (spawnTimer == 1.0f) {
		commandState = CommandState::WaitState;
		spawnTimer = 0.0f;
	}
}

void TutorialEnemy::WaitUpdate() {
	GetRotation2Player();

	moveTimer += scaleCount * move;

	if (moveTimer > 1.0f) {
		moveTimer = 1.0f;
		move = subNum;
	}
	if (moveTimer < 0.0f) {
		moveTimer = 0.0f;
		move = addNum;
	}
	float scale = Ease(Out, Quart, moveTimer, 0.3f, 1.0f);
	m_Scale = { scale ,scale ,scale };
	
	spawnTimer += 1.0f / kWaitTimeMax;
	Helper::GetInstance()->Clamp(spawnTimer, 0.0f, 1.0f);

	if (spawnTimer==1.0f) {
		m_Scale = { 1.f,1.f,1.f };
		rot = m_Rotation.y;
		s_pos = m_Position;
		e_pos = { m_Position.x+sinf(RottoPlayer) *-10.0f,0.f, m_Position.z + cosf(RottoPlayer) * -10.0f };
		commandState = CommandState::JumpState;
		spawnTimer = 0.0f;
	}
}

void TutorialEnemy::LockOnUpdate() {
	GetRotation2Player();
	spawnTimer += 1.0f / kLockOnTimeMax;
	Helper::GetInstance()->Clamp(spawnTimer, 0.0f, 1.0f);

	if (spawnTimer == 1.0f) {
		jumpCount++;
		spawnTimer = 0.0f;
		rot = m_Rotation.y;
		s_pos = m_Position;
		e_pos = { m_Position.x + sinf(RottoPlayer) * -(8.f*(float)jumpCount),0.f, m_Position.z + cosf(RottoPlayer) * -(15.0f * (float)jumpCount) };
		commandState = CommandState::JumpState;
	}
}

void TutorialEnemy::JumpUpdate() {

	spawnTimer += 1.0f / kJumpTimeMax;
	Helper::GetInstance()->Clamp(spawnTimer, 0.0f, 1.0f);	


	float hight = Ease(In, Quad, spawnTimer, 1.0f, 0.0f);

	m_Position = {
	Ease(Out, Quart, spawnTimer, s_pos.x, e_pos.x),
	5.0f,
	Ease(Out, Quart, spawnTimer, s_pos.z, e_pos.z),
	};
	if (spawnTimer == 1.0f) {
		if (jumpCount<kJumpCountMax) {
			commandState = CommandState::LockOnState;
		} else {
			jumpCount = 1;
			commandState = CommandState::WaitState;
		}
		spawnTimer = 0.0f;
	}
}

void TutorialEnemy::GetRotation2Player() {
	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	m_Rotation.y = RottoPlayer * 60.0f + (PI_90+PI_180);
}


