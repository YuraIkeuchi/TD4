#include "AttackNote.h"
#include "VariableCommon.h"
#include "Collision.h"
#include "HungerGauge.h"
#include "CsvLoader.h"
#include "Player.h"
AttackNote::AttackNote() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//èâä˙âª
bool AttackNote::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "Speed")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "Timer")));
	return true;
}
//èÛë‘ëJà⁄
/*CharaStateÇÃStateï¿Ç—èáÇ…çáÇÌÇπÇÈ*/
void (AttackNote::* AttackNote::stateTable[])() = {
	&AttackNote::Follow,
	&AttackNote::Diffusion,
};
//çXêV
void AttackNote::Update() {
	//èÛë‘à⁄çs(charastateÇ…çáÇÌÇπÇÈ)
	(this->*stateTable[_charaState])();
	if (m_SetType == SET_FOLLOW) {
		_charaState = STATE_FOLLOW;
	}
	else {
		_charaState = STATE_DIFF;
	}
	//É^ÉCÉvÇ…ÇÊÇ¡ÇƒêFÇàÍíUïœÇ¶ÇƒÇÈ
	Obj_SetParam();

	Collide();
}
//ï`âÊ
void AttackNote::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGuiï`âÊ
void AttackNote::ImGuiDraw() {
	ImGui::Begin("Note");
	ImGui::Text("Timer:%d", m_Timer);
	ImGui::Text("m_TargetTimer:%d", m_TargetTimer);
	ImGui::End();
}

//ÉpÅ[ÉeÉBÉNÉã
void AttackNote::Particle() {
	
}

//í«è]
void AttackNote::Follow() {
	m_Timer++;
	float l_Vel = 0.35f;//ë¨ìx
	XMFLOAT3 l_playerPos = Player::GetInstance()->GetPosition();
	if (m_Timer < m_TargetTimer) {
		Helper::GetInstance()->FollowMove(m_Position, l_playerPos, l_Vel);
	}
	else {
		m_Timer = {};
		m_Alive = false;
	}
}

//ägéUíe
void AttackNote::Diffusion() {
	m_Position.x += m_Angle.x * m_AddSpeed;
	m_Position.z += m_Angle.y * m_AddSpeed;

	if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -55.0f, 65.0f) || Helper::GetInstance()->CheckNotValueRange(m_Position.z, -60.0f, 60.0f)) {
		m_Alive = false;
	}
}

//ìñÇΩÇËîªíË
bool AttackNote::Collide() {
	XMFLOAT3 l_PlayerPos = Player::GetInstance()->GetPosition();

	const float l_Radius = 1.0f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (Player::GetInstance()->GetDamageInterVal() == 0)) {
		Player::GetInstance()->RecvDamage(0.5f);
		Player::GetInstance()->PlayerHit(m_Position);
		return true;
	}
	else {
		return false;
	}

	return false;
}