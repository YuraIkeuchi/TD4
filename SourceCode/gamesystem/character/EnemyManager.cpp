#include "EnemyManager.h"
#include "Helper.h"
#include "Input.h"
EnemyManager::EnemyManager(const std::string& sceneName) {

	//シーンによって読み込むボスが違う
	if (sceneName == "FIRSTSTAGE") {
		enemy.reset(new FirstBoss());
		enemy->Initialize();

		bulletenemy.resize(3);
		for (auto i = 0; i < bulletenemy.size(); i++)
		{
			bulletenemy[i] = (new NormalEnemy());
			bulletenemy[i]->Initialize();
		}
	} else if (sceneName == "SECONDSTAGE") {

		enemy.reset(new SecondBoss());
		enemy->Initialize();
	} else {
		assert(0);
	}

}
//更新
void EnemyManager::Update() {
	enemy->Update();

	if (Input::GetInstance()->TriggerButton(Input::A))Shot_3 = true;
	if (Input::GetInstance()->TriggerButton(Input::X))Shot_8 = true;

	enemy->SummonEnemyUpda(bulletenemy);
	enemy->isRespawn(bulletenemy);

}
//描画
void EnemyManager::Draw(DirectXCommon* dxCommon) {
	enemy->Draw(dxCommon);
	enemy->SummonEnemyDraw(bulletenemy, dxCommon);

}
//ImGui
void EnemyManager::ImGuiDraw() {
	//enemy->ImGuiDraw();
	enemy->ImGuiDraw();
}

void EnemyManager::ShotAttack_B()
{
	
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