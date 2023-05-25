﻿#include "InterBoss.h"
#include"Collision.h"
#include "Helper.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
#include <Helper.h>
//譖ｴ譁ｰ
void InterBoss::Update() {
	//陦悟虚
	Action();
	DeathAction();
	
	//エフェクト
	for (InterEffect* effect : effects) {
		if (effect != nullptr) {
			effect->Update();
		}
	}

	//マークの削除
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i] == nullptr) {
			continue;
		}

		if (!effects[i]->GetAlive()) {
			effects.erase(cbegin(effects) += i);
		}
	}
}
//謠冗判
void InterBoss::Draw(DirectXCommon* dxCommon) {
}
//ImGui謠冗判
void InterBoss::ImGuiDraw() {
	ImGui_Origin();
}

float InterBoss::HpPercent() {

	float temp = m_HP / m_MaxHp;
	Helper::GetInstance()->FloatClamp(temp, 0.0f, 1.0f);
	return temp;
}

void InterBoss::CollideBul(vector<InterBullet*> bullet)
{
	if (ColChangeEaseT>0.f)return;

	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr) {
			if (Collision::SphereCollision(_bullet->GetPosition(), m_Radius, m_Position, m_Radius))
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
				BirthEffect();
			}
		}
	}
}

//エフェクトの発生
void InterBoss::BirthEffect() {
	InterEffect* neweffect;
	neweffect = new BreakEffect();
	neweffect->Initialize();
	neweffect->SetPosition(m_Position);
	effects.push_back(neweffect);
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
			if (enemy[i] == nullptr)continue;
			move[i] = { 0.f,0.f, 0.1f, 0.0f };
			matRot[i] = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + (0.f + static_cast<float>(i) * 45.f)));
			move[i] = XMVector3TransformNormal(move[i], matRot[i]);

			enemy[i]->SetPosition({ m_Position.x + move[i].m128_f32[0] * 90.f,m_Position.y,	m_Position.z + move[i].m128_f32[2] * 90.f });
		}
	}

	if (SummonF) {
		
		for (auto i = 0; i < enemy.size(); i++)
		{
			if (enemy[i] == nullptr)continue;
			enemy[i]->Update();
		}
		for (auto i = 0; i < enemy.size(); i++) {
			if (enemy[i] == nullptr)continue;
			enemy[i]->SetShotF(true);
		}
		
	}
	EndSummon(enemy);
}

void InterBoss::EndSummon(std::vector<InterEnemy*> enemy)
{
	bool tempList[3];
	for (auto i = 0; i < _countof(tempList); i++) {
		if (enemy[i] == nullptr)continue;
		tempList[i] = enemy[i]->GetShotF();
	}
	if (SummobnStop) {
		if (Helper::GetInstance()->All_Of(tempList, _countof(tempList))) {
			
			NextActionInteval++;
			if (NextActionInteval > 180) {
				ActionTimer++;
				EndSummonRepos = true;
				SummobnStop = false;
			}
		}
	}
	else
	{
		NextActionInteval = 0;
	}
	bool tempList2[3];
	for (auto i = 0; i < _countof(tempList2); i++) {
		if (enemy[i] == nullptr)continue;
		tempList2[i] = !enemy[i]->GetisAlive() && enemy[i]->GEtAlpha() <= 0.f;
	}
	if(Helper::GetInstance()->All_Of(tempList2,_countof(tempList2)))
	{
		SummonF = false;
	}
}

void InterBoss::SummonEnemyDraw(std::vector<InterEnemy*> enemy, DirectXCommon* dxcomn)
{
	for (auto i = 0; i < enemy.size(); i++)
	{
		if (enemy[i] == nullptr)continue;
		enemy[i]->Draw(dxcomn);
	}
}

void InterBoss::DeathAction()
{
	if (isAlive)return;

	DeathSceneF = true;
}
