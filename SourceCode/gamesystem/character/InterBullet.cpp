#include "InterBullet.h"

//�X�V
void InterBullet::Update() {
	Action();//�e���Ƃ̏���
}

//�`��
void InterBullet::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void InterBullet::ImGuiDraw() {
	ImGui_Origin();//���ꂼ���ImGui
}