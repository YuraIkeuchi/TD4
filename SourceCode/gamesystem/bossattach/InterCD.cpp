#include "InterCD.h"

void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//ê∂ê¨
	&InterCD::StayCD, //ï˙íu
	&InterCD::CatchCD,//èäéù
	&InterCD::ThrowCD,//ìäÇ∞ÇÈ
	&InterCD::DeathCD,//Ç»Ç≠Ç»Ç¡ÇΩ
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