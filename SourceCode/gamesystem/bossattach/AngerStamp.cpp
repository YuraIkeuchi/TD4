#include "AngerStamp.h"
#include "Easing.h"
#include "ImageManager.h"
#include <random>
#include "Player.h"
#include "CsvLoader.h"
#include "Collision.h"
AngerStamp::AngerStamp() {
	
}

//初期化
bool AngerStamp::Initialize(const XMFLOAT3& pos) {
	texture.reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	texture->TextureCreate();
	m_Birth = true;
	m_Position = { pos.x,0.0f,pos.z };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_DamagePower = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Third/Thirdboss.csv", "StampDamage")));
	return true;
}

//行動
void AngerStamp::Action() {
	const int l_AliveTimer = 5;
	const int l_LimitTimer = 100;
	const float l_AddFrame = 0.01f;
	if (m_Birth) {
		m_BirthTimer++;

		if (m_BirthTimer == l_AliveTimer) {
			m_Alive = true;
		}

		if (m_BirthTimer >= l_LimitTimer) {
			if (m_Frame < m_FrameMax) {
				m_Frame += l_AddFrame;
			}
			else {
				m_Alive = false;
				m_Birth = false;
			}
		}
		//赤に近づけつつ消す
		m_Color = {
			m_Color.x,
			m_Color.y = Ease(In, Cubic, m_Frame, 1.0f, 0.0f),
			m_Color.z = Ease(In, Cubic, m_Frame, 1.0f, 0.0f),
			m_Color.w = Ease(In, Cubic, m_Frame, 1.0f, 0.0f),
		};

		//一定のラインになったら爆発エフェクト発生
		if (m_Color.w < 0.1f) {
			BirthParticle();
		}
	}

	Collide();
}

//ImGui
void AngerStamp::ImGui_Origin() {
}

void AngerStamp::Origin_Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);

	if (m_Alive) {
		texture->Draw();
	}
	IKETexture::PostDraw();
}

void AngerStamp::BirthParticle() {
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
//当たり判定
bool AngerStamp::Collide() {
	XMFLOAT3 l_PlayerPos = Player::GetInstance()->GetPosition();

	const float l_Radius = 2.3f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (m_Color.w < 0.1f) && (Player::GetInstance()->GetDamageInterVal() == 0)) {
		Player::GetInstance()->RecvDamage(m_DamagePower);
		Player::GetInstance()->PlayerHit(m_Position);
		return true;
	}
	else {
		return false;
	}

	return false;
}