#include "InterBoss.h"
#include"Collision.h"
#include "Helper.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
#include <Helper.h>
//更新
void InterBoss::Update() {
	//行動
	Action();
	
}
//描画
void InterBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	EffecttexDraw(dxCommon);
}
//ImGui描画
void InterBoss::ImGuiDraw() {
	ImGui_Origin();//それぞれのImGui
}

float InterBoss::HpPercent() {

	float temp = m_HP / m_MaxHp;
	Helper::GetInstance()->FloatClamp(temp, 0.0f, 1.0f);
	return temp;
}

//弾との当たり判定
void InterBoss::CollideBul(vector<InterBullet*> bullet)
{
	if (ColChangeEaseT>0.f)return;
	constexpr float BulRad = 1.f;

	constexpr float BossRad = 3.f;

	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, BulRad, m_Position.x, m_Position.z, BossRad))
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Attack_Normal.wav", VolumManager::GetInstance()->GetSEVolum());
				ActionTimer++;
				Recv = true;
				_bullet->SetAlive(false);
				if (_bullet->GetScale().x == 1.0f) {
					m_HP--;
				} else {
					m_HP -= 2.0f;
				}
			}
		}
	}
}


void InterBoss::SummonEnemyInit(InterEnemy* enemy)
{

}

void InterBoss::isRespawn(std::vector<InterEnemy*> enemy)
{
	if(ResF)
	{
		for(auto i=0;i<3;i++)
		{
			NextActionInteval = 0;
			enemy[i]->SetHP(1);
			enemy[i]->SetScale({ 0,0,0});
			enemy[i]->SetColor({ 1,1,1,1 });
			enemy[i]->SetShotF(false);
			enemy[i]->SetcanRotandRush(false);
			enemy[i]->SetAlive(true);
			
		}
		ResF = false;
	}
}


void InterBoss::SummonEnemyUpda(std::vector<InterEnemy*> enemy)
{

	std::vector<XMVECTOR> move(enemy.size());
	std::vector<XMMATRIX>matRot(enemy.size());

	for (auto i = 0; i < enemy.size(); i++) {
		move[i] = { 0.f,0.f, 0.1f, 0.0f };
	}
	if (!SummonF) {
		for (auto i = 0; i < enemy.size(); i++) {
			move[i] = { 0.f,0.f, 0.1f, 0.0f };
			matRot[i] = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + (0.f + static_cast<float>(i) * 45.f)));
			move[i] = XMVector3TransformNormal(move[i], matRot[i]);

			enemy[i]->SetPosition({ m_Position.x + move[i].m128_f32[0] * 90.f,m_Position.y,	m_Position.z + move[i].m128_f32[2] * 90.f });
		}
	}

	if (SummonF) {
		for (auto i = 0; i < enemy.size(); i++)
		{
			enemy[i]->Update();
		}
		for (auto i = 0; i < enemy.size(); i++) {
				enemy[i]->SetShotF(true);
		}
		
	}
	EndSummon(enemy);
}

void InterBoss::EndSummon(std::vector<InterEnemy*> enemy)
{
	//仮の格納配列
	bool tempList[3];
	//全部発射状態なら
	for (auto i = 0; i < _countof(tempList); i++)
		tempList[i] = enemy[i]->GetShotF();

	if (SummobnStop) {
		//召喚状態解除　円運動再開
		if (Helper::GetInstance()->All_Of(tempList, _countof(tempList))) {
			
			NextActionInteval++;
			if (NextActionInteval > 180) {
				ActionTimer++;
				SummobnStop = false;
			}
		}
	}
	else
	{
		NextActionInteval = 0;
	}
	bool tempList2[3];
	for (auto i = 0; i < _countof(tempList2); i++)
		tempList2[i] = !enemy[i]->GetisAlive()&&enemy[i]->GEtAlpha()<=0.f;

	if(Helper::GetInstance()->All_Of(tempList2,_countof(tempList2)))
	{
		SummonF = false;
	}
}

void InterBoss::SummonEnemyDraw(std::vector<InterEnemy*> enemy, DirectXCommon* dxcomn)
{
	for (auto i = 0; i < enemy.size(); i++)
	{
		enemy[i]->Draw(dxcomn);
	}
}