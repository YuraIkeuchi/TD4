#include "NormalEnemy.h"

#include <random>

#include "Player.h"
#include "Collision.h"
#include "ModelManager.h"
#include "Helper.h"
#include "ParticleEmitter.h"
//モデル読み込み
NormalEnemy::NormalEnemy() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BulEnemy);
}
//初期化
bool NormalEnemy::Initialize() {

	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	m_Scale = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Position.y = 5.0f;
	ret = true;
	HP = 1;
	isAlive = true;
	return true;
}
//行動
void NormalEnemy::Action() {
	float l_Vel = 0.15f;
	//m_CircleSpeed += 1.0f;
	m_Rotation.x = 90.f;

	if (ShotF) {
		Appearance();
		RushAction();
	}
	else
	{
		t = 0.f;
	}

	Obj_SetParam();	//m_Position = Helper::GetInstance()->CircleMove({ 0.0f,5.0f,0.0f }, m_CircleScale, m_CircleSpeed);
	OnCollision();
	ColPlayer();
	Particle();
	DeathEffect();
}
//描画
void NormalEnemy::Draw(DirectXCommon* dxCommon) {

	if (m_Color.w <= 0.f)return;
	IKEObject3d::PreDraw();
	Obj_Draw();
}
//ImGui描画
void NormalEnemy::ImGuiDraw() {
	ImGui::Begin("Enemy");
	//ImGui::Text("time %f", t);
	ImGui::Text("RotOld %f", old);

	ImGui::Text("EnePosX:%f", m_Position.x);
	ImGui::Text("EnePosZ:%f", m_Position.z);
	ImGui::End();
}
//開放
void NormalEnemy::Finalize() {

}

//パーティクル
void NormalEnemy::Particle() {
	if (!isAlive)return;
	XMFLOAT4 s_color = { 1.0f,0.5f,1.0f,1.5f };
	XMFLOAT4 e_color = { 1.0f,0.5f,1.0f,0.1f };
	//float s_scale = 3.0f;
	float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->FireEffect(10, m_Position, s_scale, e_scale, s_color, e_color);
}


void NormalEnemy::DeathEffect()
{
	if (isAlive)return;

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

void NormalEnemy::Appearance()
{
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

	Helper::GetInstance()->FloatClamp(m_Scale.x, 0.f, 1.5f);
	Helper::GetInstance()->FloatClamp(m_Scale.y, 0.f, 1.5f);
	Helper::GetInstance()->FloatClamp(m_Scale.z, 0.f, 1.5f);

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	m_Rotation.y = RottoPlayer * 60.f + 180.f;


	ShotCount++;

	if (ShotCount>200)
	{
		Rush = true;
	}




}

void NormalEnemy::RushAction()
{
	if (!Rush)return;

	ShotCount = 0;
	s_scale += 0.02f;

	Helper::GetInstance()->FloatClamp(s_scale, 0.f, 5.f);

	 // positionA - positionB;
	//回転軸をプレイヤーの方に
		//向きかえる
	if (ret)
	{
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
		if(canRot)
		m_Rotation.y = Easing::EaseOut(t, old * 50.f + 180.f, RotY * 50.f+180.f);
		if (t >= 1.f) {
			canRot = true; 
			ret = false;//	} else {
		}	//m_Rotation.y = Easing::EaseOut(t, old * 60.f + 180.f, RotY * 60.f + 180.f - 360.f);
			//}
	} else
	{
		t = 0.f;
		old = RotY;
		MoveTimer++;
		if (MoveTimer>120) {
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
					m_Position.x + move.m128_f32[0] * 3.f,
				m_Position.y,
				m_Position.z + move.m128_f32[2] * 3.f
			};
		}
	}
	Helper::GetInstance()->FloatClamp(t, 0.f, 1.f);

}


