#include "ParticleEmitter.h"
#include "VariableCommon.h"
#include "ImageManager.h"
#include "Helper.h"
#include <random>
ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//テクスチャ全読み込み
	LoadTexture();
	//パーティクルマネージャー生成
	circleParticle.reset(ParticleManager::Create(ImageManager::Normal));
	healParticle.reset(ParticleManager::Create(ImageManager::Heal));
	deathParticle.reset(ParticleManager::Create(ImageManager::Normal));
}

void ParticleEmitter::Update()
{
	//パーティクルマネージャー更新
	circleParticle->Update();
	healParticle->Update();
	deathParticle->Update();
}

void ParticleEmitter::FlontDrawAll() {
	circleParticle->Draw(AddBlendType);
	healParticle->Draw(AddBlendType);
}

void ParticleEmitter::DeathDrawAll() {
	deathParticle->Draw(AddBlendType);
}

void ParticleEmitter::FireEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor)
{
	XMFLOAT3 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 2.0f;// -rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	circleParticle->Add(life, { pos.x,pos.y,pos.z }, vel, {}, startscale, endscale, startcolor, endcolor,{});
}

//爆発
void ParticleEmitter::Explosion(const int life, const XMFLOAT3& pos, const float size, const float startscale,
	const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor) {
	for (int j = 0; j < 3; j++) {
		//X,Y,Z全て[-2.0f, +2.0f]でランダムに分布
		const float RandPos = 2.0f;
		XMFLOAT3 l_pos = pos;
		l_pos.x += ((float)rand() / RAND_MAX * RandPos - RandPos / 2.0f) * size;
		l_pos.y += ((float)rand() / RAND_MAX * RandPos - RandPos / 2.0f) * size;
		l_pos.z += ((float)rand() / RAND_MAX * RandPos - RandPos / 2.0f) * size;

		for (int i = 0; i < 10; i++) {
			//X,Y,Z全て[-0.8f, +0.8f]でランダムに分布
			const float RandPos2 = 0.8f;
			l_pos.x += ((float)rand() / RAND_MAX * RandPos2 - RandPos2 / 2.0f) * size;
			l_pos.y += ((float)rand() / RAND_MAX * RandPos2 - RandPos2 / 2.0f) * size;
			l_pos.z += ((float)rand() / RAND_MAX * RandPos2 - RandPos2 / 2.0f) * size;

			//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
			const float RandVel = 0.05f;
			XMFLOAT3 vel{};
			vel.x = ((float)rand() / RAND_MAX * RandVel - RandVel / 2.0f) * size;
			vel.y = ((float)rand() / RAND_MAX * RandVel - RandVel / 2.0f) * size;
			vel.z = ((float)rand() / RAND_MAX * RandVel - RandVel / 2.0f) * size;


			//追加
			circleParticle->Add(life, l_pos, vel, {}, startscale, endscale, startcolor, endcolor,{});
		}
	}
}


//ダメージ時のエフェクト
void ParticleEmitter::Break(const int life, const XMFLOAT3& pos,const float startscale, const float endscale,
	const XMFLOAT4& startcolor, const XMFLOAT4& endcolor,const float Gravity,float divi) {
	const int l_Division = 80;
	float l_AddPowerY = 0.0f;
	//乱数の範囲(-50から50)
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_bounddist(-50, 50);

	XMFLOAT3 l_pos = pos;

	XMFLOAT3 vel{};
	
	vel = {
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
	};

	vel.y += l_AddPowerY;

	circleParticle->Add(life, l_pos, vel, {}, startscale, endscale, startcolor, endcolor,Gravity,divi);
}


void ParticleEmitter::DeathEffect(int life, XMFLOAT3 l_pos, float startscale, float endscale, XMFLOAT4 startcolor, XMFLOAT4 endcolor, float velocity) {
	XMFLOAT3 pos = l_pos;
	float angle = (float)rand() / RAND_MAX * 360.0f;
	const float rnd_vel = 0.1f;
	XMFLOAT3 vel{};
	vel.x = velocity * sinf(angle);
	vel.y = velocity * cosf(angle);
	vel.z = 0.0f;
	deathParticle->Add(life, pos, vel, XMFLOAT3(), startscale, endscale, startcolor, endcolor,{});
}

void ParticleEmitter::HealEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor) {
	XMFLOAT3 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 2.0f;// -rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	healParticle->Add(life, { pos.x,pos.y,pos.z }, vel, {}, startscale, endscale, startcolor, endcolor, {});
}
void ParticleEmitter::AllDelete()
{
	//全パーティクルの削除
	circleParticle->AllDelete();
	healParticle->AllDelete();
	deathParticle->AllDelete();
}

void ParticleEmitter::LoadTexture() {
	ImageManager::GetInstance()->LoadParticle();
}