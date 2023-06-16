#include "BarrangeCD.h"
#include "ModelManager.h"
#include "Helper.h"
BarrangeCD::BarrangeCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

//初期化
bool BarrangeCD::Initialize() {
	m_Position = { -40.0f,60.0f,40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.5f,0.0f,0.5f,1.0f };
	m_CDState = CD_BIRTH;
	return true;
}

//特有の処理
void BarrangeCD::Action() {
	(this->*stateTable[static_cast<size_t>(m_CDState)])();
	Obj_SetParam();
}


void BarrangeCD::Origin_Draw(DirectXCommon* dxCommon) {
	//Obj_Draw();
}
//IMGUI
void BarrangeCD::ImGui_Origin() {

}
//生成
void BarrangeCD::BirthCD() {
	//飛ぶような感じにするため重力を入れる
	m_AddPower -= m_Gravity;
	Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_AddPower);

	if (m_Position.y == 0.0f) {
		m_CDState = CD_STAY;
	}
}

//ステージにいる状態
void BarrangeCD::StayCD() {

}

//ボスが手に入れた状態
void BarrangeCD::CatchCD() {

}

//ボスが投げる
void BarrangeCD::ThrowCD() {

}
//消えた
void BarrangeCD::DeathCD() {

}