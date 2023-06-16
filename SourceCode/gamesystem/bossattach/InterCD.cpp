#include "InterCD.h"

void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//¶¬
	&InterCD::StayCD, //•ú’u
	&InterCD::ThroughCD,//ƒXƒ‹[
	&InterCD::CatchCD,//Š
	&InterCD::ThrowCD,//“Š‚°‚é
	&InterCD::DeathCD,//‚È‚­‚È‚Á‚½
};

void InterCD::Update() {
	Action();
}
void InterCD::Draw(DirectXCommon* dxCommon) {
	if(m_CDState != CD_DEATH)
	Obj_Draw();
}
void InterCD::ImGuiDraw() {
	
	ImGui_Origin();
}