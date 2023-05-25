#include "InterEnemy.h"

#include "Collision.h"
#include "Helper.h"
#include "Player.h"
//Player* InterEnemy::player = nullptr;
//������
bool InterEnemy::Initialize() {
	return true;
}
//�X�V
void InterEnemy::Update() {
	//�s��
	Action();
	DeathAction();
}
//�`��
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}
//void InterEnemy::PlayerGet(Player* _player) {
//
//	//InterEnemy::player = _player;
//
//}

void InterEnemy::OnCollision()
{
	if (!isAlive)return;
	//�U���̒e�Ƃ̔���
	for (InterBullet* _bullet : Player::GetInstance()->GetBulllet_attack()) {
		if (_bullet->GetAlive()) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, 1.f, m_Position.x, m_Position.z, 1.3f))
			{
				//�̗�
				HP--;
			}
		}
	}
	if (HP <= 0)
	{
		isAlive = false;
	}
}

void InterEnemy::DeathAction()
{
	if (isAlive)return;

	m_Color.w -= 0.033f;

	Helper::GetInstance()->FloatClamp(m_Color.w, 0.f, 1.f);
}

void InterEnemy::ColPlayer()
{
	constexpr int damage = 1;
	if (!isAlive) { return; }
	if (Collision::CircleCollision(Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, 2.f, m_Position.x, m_Position.z, 1.f))
	{
		Player::GetInstance()->PlayerHit(m_Position);
		Player::GetInstance()->RecvDamage(damage);
		isAlive = false;
	}
	if (HP <= 0)
	{
		isAlive = false;
	}

}


void InterEnemy::ColEnemy(std::vector<InterEnemy*> enelist)
{
	for (auto i = 0; i <enelist.size(); ++i) {
		for (auto j = 0; j < enelist.size(); ++j) {
			XMFLOAT3 ghostpos = enelist[i]->GetPosition();
			XMFLOAT3 ghostpos2 = enelist[j]->GetPosition();
			if ((i == j)) { continue; }
			if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
				enelist[i]->EnemyColNormal(ghostpos2);
				enelist[j]->EnemyColNormal(ghostpos);
			}
		}
	}
}

void InterEnemy::EnemyColNormal(const XMFLOAT3& pos) {
	m_Position.x += sin(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
	m_Position.z += cos(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
}
