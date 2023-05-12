#include "InterEnemy.h"

#include "Collision.h"
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

	for (InterBullet* _bullet : Player::GetInstance()->GetBulllet_attack()) {
		if (Collision::GetLength(_bullet->GetPosition(), m_Position) < 2.f)
		{
			//�̗�
			HP--;
			//�e����
			_bullet->SetAlive(false);
		}
	}
	if(HP<=0)
	{
		isAlive = false;
	}
}

void InterEnemy::ColPlayer()
{
	constexpr int damage = 5;

	//�v���C���[�_���[�W����
	if (Collision::CircleCollision(Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z,2.f,m_Position.x, m_Position.z,1.f ))
		{
		Player::GetInstance()->RecvDamage(damage);
		isAlive = false;
		}
	if (HP <= 0)
	{
		isAlive = false;
	}
	
}
