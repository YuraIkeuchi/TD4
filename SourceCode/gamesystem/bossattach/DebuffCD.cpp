#include "DebuffCD.h"
#include "ModelManager.h"
#include "Helper.h"
#include "Player.h"
#include <random>
DebuffCD::DebuffCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool DebuffCD::Initialize() {
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(20, 50);
	uniform_int_distribution<int> l_distZ(-50, -20);
	m_Position = { float(l_distX(mt)),60.0f,float(l_distZ(mt)) };
	//m_Position = { 40.0f,60.0f,-40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	m_CDState = CD_BIRTH;
	return true;
}
//特有の動き
void DebuffCD::Action() {
	(this->*stateTable[static_cast<size_t>(m_CDState)])();
	Obj_SetParam();
}
//描画
void DebuffCD::Origin_Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void DebuffCD::ImGui_Origin() {
	ImGui::Begin("DEBUFFCD");
	ImGui::Text("CDSTATE:%d", m_CDState);
	ImGui::End();
}

//生成
void DebuffCD::BirthCD() {
	//飛ぶような感じにするため重力を入れる
	m_AddPower -= m_Gravity;
	Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_AddPower);

	if (m_Position.y == 0.0f) {
		m_CDState = CD_STAY;
	}
}

//ステージにいる状態
void DebuffCD::StayCD() {

}

//スルーされたやつ
void DebuffCD::ThroughCD() {

}

//ボスが手に入れた状態
void DebuffCD::CatchCD() {
	m_Position = m_CatchPos;
}

//ボスが投げる
void DebuffCD::ThrowCD() {
	m_ThrowTimer++;
	if (m_ThrowTimer == 1) {
		double sb, sbx, sbz;
		sbx = Player::GetInstance()->GetPosition().x - m_Position.x;
		sbz = Player::GetInstance()->GetPosition().z - m_Position.z;
		sb = sqrt(sbx * sbx + sbz * sbz);
		m_SpeedX = sbx / sb * 0.8;
		m_SpeedZ = sbz / sb * 0.8;
	}
	else if (m_ThrowTimer > 1) {
		//プレイヤーにスピード加算
		m_Position.x += (float)m_SpeedX;
		m_Position.z += (float)m_SpeedZ;

		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -60.0f, 70.0f) || Helper::GetInstance()->CheckNotValueRange(m_Position.z, -65.0f, 65.0f)) {
			m_CDState = CD_DEATH;
			m_ThrowTimer = 0;
		}
	}
}

//消えた
void DebuffCD::DeathCD() {

}

//復活
void DebuffCD::ResPornCD() {
	m_ResPornTimer++;
	const int LimitTimer = 10;

	if (m_ResPornTimer == LimitTimer) {
		//乱数指定
		mt19937 mt{ std::random_device{}() };
		uniform_int_distribution<int> l_distX(20, 50);
		uniform_int_distribution<int> l_distZ(-50, -20);
		m_Position = { float(l_distX(mt)),60.0f,float(l_distZ(mt)) };
		//m_Position = { 40.0f,60.0f,-40.0f };
		m_AddPower = {};
		m_CDState = CD_BIRTH;
	}
}