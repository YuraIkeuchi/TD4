#include "InterCD.h"

void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//����
	&InterCD::StayCD, //���u
	&InterCD::ThroughCD,//�X���[
	&InterCD::CatchCD,//����
	&InterCD::ThrowCD,//������
	&InterCD::DeathCD,//�Ȃ��Ȃ���
	&InterCD::ResPornCD,//�肷�ہ[��
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