#include "LineCD.h"
#include "ModelManager.h"
#include "Helper.h"
LineCD::LineCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool LineCD::Initialize() {
	m_Position = { 40.0f,60.0f,40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	m_CDState = CD_BIRTH;
	return true;
}
//特有の鼓動
void LineCD::Action() {
	(this->*stateTable[static_cast<size_t>(m_CDState)])();
	Obj_SetParam();
}
//描画
void LineCD::Origin_Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void LineCD::ImGui_Origin() {
	ImGui::Begin("LINECD");
	ImGui::Text("CDSTATE:%d", m_CDState);
	ImGui::End();
}

//生成
void LineCD::BirthCD() {
	//飛ぶような感じにするため重力を入れる
	m_AddPower -= m_Gravity;
	Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_AddPower);

	if (m_Position.y == 0.0f) {
		m_CDState = CD_STAY;
	}
}

//ステージにいる状態
void LineCD::StayCD() {

}

//スルーされたやつ
void LineCD::ThroughCD() {

}

//ボスが手に入れた状態
void LineCD::CatchCD() {
	m_Position = m_CatchPos;
}

//ボスが投げる
void LineCD::ThrowCD() {

}

//消えた
void LineCD::DeathCD() {

}