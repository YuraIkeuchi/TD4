#include "InterEnemy.h"
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