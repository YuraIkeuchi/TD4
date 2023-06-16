#include "InterCD.h"

void InterCD::Update() {
	Action();
}
void InterCD::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
void InterCD::ImGuiDraw() {
	ImGui_Origin();
}