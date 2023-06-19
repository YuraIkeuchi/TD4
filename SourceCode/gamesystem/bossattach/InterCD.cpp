#include "InterCD.h"
#include "Collision.h"
#include "Player.h"
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
	if(m_CDState != CD_DEATH && m_CDState != CD_RESPORN)
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
				m_BreakCD = true;
				_bullet->SetAlive(false);
			}
		}
	}	
}

//�v���C���[��CD�̓����蔻��
bool InterCD::PlayerCollide() {
	if (m_CDState != CD_THROW)return false;
	const float l_Radius = 1.0f;
	if (Collision::CircleCollision(Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, l_Radius, m_Position.x, m_Position.z, l_Radius)
		&& Player::GetInstance()->GetDamageInterVal() == 0) {
		Player::GetInstance()->PlayerHit(m_Position);
		Player::GetInstance()->RecvDamage(0.5f);
		return true;
	}
	else {
		return false;
	}

	return false;
}

void InterCD::SetCD() {
	if (m_AttackSetCD) {
		if (m_CatchState == CATCH_SET) {
			m_AddPower = 0.5f;
			m_CatchState = CATCH_MOVE;
		}
		else if (m_CatchState == CATCH_MOVE) {
			m_AddPower -= m_Gravity;
			if (Helper::GetInstance()->CheckMax(m_Position.y, m_CatchPos.y, m_AddPower) && m_AddPower < -1.0f) {
				m_CatchState = CATCH_END;
				m_AttackSetCD = false;
			}
		}
	}
}