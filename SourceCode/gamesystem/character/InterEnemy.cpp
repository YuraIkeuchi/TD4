#include "InterEnemy.h"

#include "Collision.h"
#include "Helper.h"
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
	DeathAction();
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
	//攻撃の弾との判定
	for (InterBullet* _bullet : Player::GetInstance()->GetBulllet_attack()) {
		if (_bullet->GetAlive()) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z,1.f, m_Position.x,m_Position.z,1.3f))
			{
				//体力
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
