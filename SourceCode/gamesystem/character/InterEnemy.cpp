#include "InterEnemy.h"
//Player* InterEnemy::player = nullptr;
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	//行動
	Action();
}
//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}
//void InterEnemy::PlayerGet(Player* _player) {
//
//	//InterEnemy::player = _player;
//
//}