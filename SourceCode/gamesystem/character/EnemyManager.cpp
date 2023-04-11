#include "EnemyManager.h"
EnemyManager::EnemyManager() {
	enemy.reset(new NormalEnemy());
	enemy->Initialize();
}
//XV
void EnemyManager::Update() {
	enemy->Update();
}
//•`‰æ
void EnemyManager::Draw(DirectXCommon* dxCommon) {
	enemy->Draw(dxCommon);
}
//ImGui
void EnemyManager::ImGuiDraw() {
	enemy->ImGuiDraw();
}