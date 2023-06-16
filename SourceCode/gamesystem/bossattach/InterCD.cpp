#include "InterCD.h"

void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//生成
	&InterCD::StayCD, //放置
	&InterCD::CatchCD,//所持
	&InterCD::ThrowCD,//投げる
	&InterCD::DeathCD,//なくなった
};

void InterCD::Update() {
	Action();
}
void InterCD::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
void InterCD::ImGuiDraw() {
	ImGui::Begin("CD");
	ImGui::Text("POSY:%f", m_Position.y);
	ImGui::Text("CDSTATE:%d", m_CDState);
	ImGui::End();
	ImGui_Origin();
}