#include "InterBoss.h"
#include"Collision.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
//�X�V
void InterBoss::Update() {
	//�s��
	Action();
}
//�`��
void InterBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}