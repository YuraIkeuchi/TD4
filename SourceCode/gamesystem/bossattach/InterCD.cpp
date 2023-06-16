#include "InterCD.h"

void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//����
	&InterCD::StayCD, //���u
	&InterCD::CatchCD,//����
	&InterCD::ThrowCD,//������
	&InterCD::DeathCD,//�Ȃ��Ȃ���
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