#include "Heart.h"
#include "VariableCommon.h"
#include "Collision.h"
#include "Helper.h"
#include <random>
#include "Player.h"
Heart::Heart() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Heart);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool Heart::Initialize() {
	//乱数指定
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Alive = false;
	m_AliveTimer = 0;
	return true;
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Heart::* Heart::stateTable[])() = {
	&Heart::HeartJump,//飛ばす
	&Heart::HeartSet,//ストップ
};
//更新
void Heart::Update() {
	//状態移行(charastateに合わせる)
	(this->*stateTable[_heartState])();
	//タイプによって色を一旦変えてる
	Obj_SetParam();
	//パーティクル
	Particle();
	//当たり判定(プレイヤー)
	PlayerCollision();
}
//描画
void Heart::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void Heart::ImGuiDraw() {

}
//パーティクル
void Heart::Particle() {

}
//当たり判定(プレイヤー)
bool Heart::PlayerCollision() {
	if (Player::GetInstance()->PlayerCollide(m_Position) && (_heartState == HeartState::HEART_SET)) {
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return true;
}

//ハートを飛ばす
void Heart::HeartJump() {

}

//ハートの着地後
void Heart::HeartSet() {

}