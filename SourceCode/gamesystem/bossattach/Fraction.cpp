#include "Fraction.h"
#include"Player.h"
#include"Collision.h"
#include"InterBullet.h"
#include"Helper.h"
#include"Easing.h"

Fraction::~Fraction()
{

}

void Fraction::Init(const XMFLOAT3& BossPos)
{
	boss_pos_ = BossPos;
	m_Object.reset(new IKEObject3d);
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::GLASS1));
	m_Scale = { 3.f,3.f,3.f };
}

void Fraction::Obj_Set()
{
	m_Object->SetPosition(m_Position);
	m_Object->SetScale(m_Scale);
	m_Object->SetRotation(m_Rotation);
	m_Object->SetColor(m_Color);
	m_Object->Update();
}

void Fraction::Pop()
{
	if (pop_ != PopState::Before) { return; }
	commandTimer += 1.0f / 60;
	Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);
	float s_pos = boss_pos_.y;
	float e_pos = 15.f;

	m_Position = {
	boss_pos_.x,
	Ease(Out, Quart, commandTimer, s_pos, e_pos),
	boss_pos_.z,
	};

	if (commandTimer >= 1) {
		pop_ = PopState::After;
		pop_pos_ = m_Position;
		commandTimer = 0.f;
	}
}

void Fraction::Spatter()
{
	if (pop_ != PopState::After) { return; }
	if (drop_F != true) { return; }

	commandTimer += 1.0f / 60;
	Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);
	XMFLOAT3 s_pos = pop_pos_;
	XMFLOAT3 e_pos = drop_pos_;
	m_Position = {
	Ease(Out, Quart, commandTimer, s_pos.x, e_pos.x),
	Ease(Out, Quart, commandTimer, s_pos.y, e_pos.y),
	Ease(Out, Quart, commandTimer, s_pos.z, e_pos.z),
	};
	if (commandTimer >= 1) {
		drop_F = false;
	}
}

void Fraction::Drop(const XMFLOAT3& dropposiition)
{
	drop_pos_ = dropposiition;
	drop_F = true;
}


void Fraction::Update(vector<InterBullet*> bullet)
{
	ColPlayer(bullet);
	
	Pop();

	Spatter();

	Obj_Set();
}

void Fraction::Draw(DirectXCommon* dxCommon)
{
	IKEObject3d::PreDraw();
	m_Object->Draw();
	IKEObject3d::PostDraw();
}

void Fraction::ColPlayer(vector<InterBullet*> bullet)
{
	//ƒ‰ƒbƒVƒ…’†”»’è‚ ‚è
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_Radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, 1.f) &&
		(Player::GetInstance()->GetDamageInterVal() == 0))
	{
		Player::GetInstance()->RecvDamage(1.0f);
		Player::GetInstance()->PlayerHit(m_Position);
	}
	
	for (InterBullet* _bullet : bullet) {
		bool JudgColide;
		JudgColide = Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, m_Radius, m_Position.x, m_Position.z, m_Radius);
		if (JudgColide) {
			_bullet->SetAlive(false);
			Isdelete = true;
		}
	}
	
}
