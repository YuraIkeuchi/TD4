#include "InterEnemy.h"

#include "Collision.h"
#include "Player.h"
//Player* InterEnemy::player = nullptr;
//‰Šú‰»
bool InterEnemy::Initialize() {
	return true;
}
//XV
void InterEnemy::Update() {
	//s“®
	Action();
}
//•`‰æ
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
	//UŒ‚‚Ì’e‚Æ‚Ì”»’è
	for (InterBullet* _bullet : Player::GetInstance()->GetBulllet_attack()) {
		if (Collision::GetLength(_bullet->GetPosition(), m_Position) < 2.f)
		{
			//‘Ì—Í
			HP--;
			//’eÁ‚µ
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
