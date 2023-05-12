#include "InterEnemy.h"

#include "Collision.h"
#include "Player.h"
//Player* InterEnemy::player = nullptr;
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	//行動
	Action();
}
//描画
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

	for (InterBullet* _bullet : Player::GetInstance()->GetBulllet_ghost()) {
		if (Collision::GetLength(_bullet->GetPosition(), m_Position) < 2.f)
		{
			HP--;
			_bullet->SetAlive(false);
			//break;
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
	if (!isAlive) { return; }
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
