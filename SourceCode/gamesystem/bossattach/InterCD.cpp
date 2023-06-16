#include "InterCD.h"
#include "Collision.h"
void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//生成
	&InterCD::StayCD, //放置
	&InterCD::ThroughCD,//スルー
	&InterCD::CatchCD,//所持
	&InterCD::ThrowCD,//投げる
	&InterCD::DeathCD,//なくなった
	&InterCD::ResPornCD,//りすぽーん
};

void InterCD::Update() {
	Action();
}
void InterCD::Draw(DirectXCommon* dxCommon) {
	if(m_CDState != CD_DEATH)
	Obj_Draw();
}
void InterCD::ImGuiDraw() {
	
	ImGui_Origin();
}

//弾との当たり判定
void InterCD::CollideBul(vector<InterBullet*> bullet)
{
	if (m_CDState != CD_STAY)return;
	const float l_Radius = 1.0f;
	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr && _bullet->GetAlive()) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_Radius, m_Position.x, m_Position.z, l_Radius)) {
				m_CDState = CD_DEATH;
				_bullet->SetAlive(false);
			}
		}
	}	
}