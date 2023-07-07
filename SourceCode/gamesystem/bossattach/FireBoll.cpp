#include "FireBoll.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Easing.h"
#include "Helper.h"
FireBoll::FireBoll() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::NOTE);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool FireBoll::Initialize() {
	m_Position = { 500.0f,5.0f,0.0f };
	m_Scale = { 0.6f,0.6f,0.6f };
	m_Color = { 1.0f,1.0f,1.0f,0.0f };

	m_CircleScale = 10.0f;
	m_CircleSpeed = 0.0f;
	_MoveState = MOVE_SET;
	m_Alive = true;
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "FireDamage")));
	return true;

}
//更新
void FireBoll::Update() {
	Move();
	BirthParticle();
	Collide();
	Obj_SetParam();
}
//描画
void FireBoll::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void FireBoll::ImGuiDraw() {
	ImGui::Begin("Fire");
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("Scale:%f", m_CircleScale);
	ImGui::End();
}
//当たり判定
bool FireBoll::Collide() {
	XMFLOAT3 l_PlayerPos = Player::GetInstance()->GetPosition();

	const float l_Radius = 1.0f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (Player::GetInstance()->GetDamageInterVal() == 0)) {
		Player::GetInstance()->RecvDamage(m_Damage);
		Player::GetInstance()->PlayerHit(m_Position);
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//動き
void FireBoll::Move() {
	XMFLOAT3 l_PlayerPos = Player::GetInstance()->GetPosition();
	const float l_AddSpeed = 2.0f;
	const float l_AddFrame = 0.1f;

	if (_MoveState == MOVE_SET) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			_MoveState = MOVE_PLAY;
			m_Frame = {};
		}
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 1.0f);
	}else if (_MoveState == MOVE_PLAY) {
		m_MoveTimer++;
		m_CircleSpeed += l_AddSpeed;
		m_Position = Helper::GetInstance()->CircleMove(l_PlayerPos, m_CircleScale, m_CircleSpeed);
		if (m_MoveTimer == 100) {
			m_TargetPos = l_PlayerPos;
			_MoveState = MOVE_OMEN;
			m_MoveTimer = {};
			m_Frame = {};
		}
	}
	else if (_MoveState == MOVE_OMEN) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_MoveTimer++;

			if (m_MoveTimer == 50) {
				_MoveState = MOVE_ATTACK;
				m_Frame = {};
				m_MoveTimer = {};
			}
		}

		m_CircleScale = Ease(In, Cubic, m_Frame, m_CircleScale, 20.0f);
		m_Position = Helper::GetInstance()->CircleMove(m_TargetPos, m_CircleScale, m_CircleSpeed);
	}
	else if (_MoveState == MOVE_ATTACK) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_MoveState = MOVE_END;
		}

		m_CircleScale = Ease(In, Cubic, m_Frame, m_CircleScale, 1.0f);
		m_Position = Helper::GetInstance()->CircleMove(m_TargetPos, m_CircleScale, m_CircleSpeed);
	}
	else {
		m_MoveTimer++;

		if (m_MoveTimer == 20) {
			m_MoveTimer = {};
			m_Alive = false;
		}
	}
}
//パーティクル
void FireBoll::BirthParticle() {
	XMFLOAT4 s_color = { 1.0f,0.4f,0.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	float s_scale = 4.0f;
	float e_scale = 0.0f;
	const int m_Life = 50;
	if (m_Alive) {
		ParticleEmitter::GetInstance()->FireEffect(m_Life, m_Position, s_scale, e_scale, s_color, e_color);
	}
}