﻿#include "EnemyManager.h"
#include "Helper.h"
#include "Input.h"
EnemyManager::EnemyManager(const std::string& sceneName) {

	//シーンによって読み込むボスが違う
	if (sceneName == "FIRSTSTAGE") {
		enemy.reset(new FirstBoss());
		enemy->Initialize();
		for (auto i = 0; i < bulletenemy.size(); i++) {
			bulletenemy[i]=new NormalEnemy();
			bulletenemy[i]->Initialize();
		}

	}
	else if (sceneName == "SECONDSTAGE") {
		enemy.reset(new SecondBoss());
		enemy->Initialize();
		for (auto i = 0; i < bulletenemy.size(); i++) {
			bulletenemy[i]=new NormalEnemy();
			bulletenemy[i]->Initialize();
		}
	}
	else if (sceneName == "TUTORIAL") {
		for (auto i = 0; i < tutorialenemy.size(); i++) {
			tutorialenemy[i].reset(new TutorialEnemy());
			tutorialenemy[i]->Initialize();
		}
		//最初の敵のみ固定湧き
		tutorialenemy[0]->SetPosition({ 0,5.0f ,50.0f });
	}
	else {
		assert(0);
	}

}
//更新
void EnemyManager::Update() {
	enemy->Update();
}
void EnemyManager::TutorialUpdate(int pattern) {
	if (pattern==0) {
		tutorialenemy[0]->Update();
	} else {
		for (auto i = 0; i < tutorialEnemyMax; i++) {
			tutorialenemy[i]->Update();
		}
	}
}
//描画
void EnemyManager::Draw(DirectXCommon* dxCommon) {
	enemy->Draw(dxCommon);

	enemy->SummonEnemyDraw(bulletenemy, dxCommon);
}
void EnemyManager::TutorialDraw(DirectXCommon* dxCommon) {
	for (auto i = 0; i < tutorialEnemyMax; i++) {
		tutorialenemy[i]->Draw(dxCommon);
	}

}
//ImGui
void EnemyManager::ImGuiDraw() {
	//enemy->ImGuiDraw();
	enemy->ImGuiDraw();
}

//敵の死亡処置
bool EnemyManager::BossDestroy() {
	if (enemy->GetHP() <= 0.0f) {
		return true;
	} else {
		return false;
	}

	return false;
}

//敵のチェック終了
void EnemyManager::FinishCheck() {
	enemy->SetCheck(false);
}