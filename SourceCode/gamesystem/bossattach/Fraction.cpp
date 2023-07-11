#include "Fraction.h"
#include"Player.h"
#include"Collision.h"



Fraction::~Fraction()
{

}

void Fraction::Init(const XMFLOAT3& BossPos)
{
	boss_pos_ = BossPos;
	m_Object.reset(new IKEObject3d);
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::GLASS1));
	m_Scale = { 2.f,2.f,2.f };
}

void Fraction::Obj_Set()
{
	m_Object->SetPosition(m_Position);
	m_Object->SetScale(m_Scale);
	m_Object->SetRotation(m_Rotation);
	m_Object->SetColor(m_Color);
	m_Object->Update();
}

void Fraction::Drop()
{
	m_Position = boss_pos_;
}


void Fraction::Update()
{
	ColPlayer();

	Obj_Set();
}

void Fraction::Draw(DirectXCommon* dxCommon)
{
	IKEObject3d::PreDraw();
	m_Object->Draw();
	IKEObject3d::PostDraw();
}

void Fraction::ColPlayer()
{
	//ƒ‰ƒbƒVƒ…’†”»’è‚ ‚è
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_Radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, 1.f) &&
		(Player::GetInstance()->GetDamageInterVal() == 0))
	{
		Player::GetInstance()->RecvDamage(1.0f);
		Player::GetInstance()->PlayerHit(m_Position);
	}
}
