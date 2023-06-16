#include "InterCD.h"
#include "Collision.h"
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

//�e�Ƃ̓����蔻��
void InterCD::CollideBul(vector<InterBullet*> bullet)
{
	if (m_CDState != CD_STAY)return;
	const float l_Radius = 1.0f;
	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr && _bullet->GetAlive()) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_Radius, m_Position.x, m_Position.z, l_Radius)) {
				m_CDState = CD_DEATH;
				_bullet->SetAlive(false);
			}
		}
	}	
}