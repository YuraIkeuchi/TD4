#include "InterBoss.h"
#include"Collision.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
//更新
void InterBoss::Update() {
	//行動
	Action();
}
//描画
void InterBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
