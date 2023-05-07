#include "InterBoss.h"
#include"Collision.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
//çXêV
void InterBoss::Update() {
	//çsìÆ
	Action();
}
//ï`âÊ
void InterBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	EffecttexDraw(dxCommon);
}
