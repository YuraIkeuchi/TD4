#include "InterCD.h"

void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//生成
	&InterCD::StayCD, //放置
	&InterCD::ThroughCD,//スルー
	&InterCD::CatchCD,//所持
	&InterCD::ThrowCD,//投げる
	&InterCD::DeathCD,//なくなった
	&InterCD::ResPornCD,//りすぽーん
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