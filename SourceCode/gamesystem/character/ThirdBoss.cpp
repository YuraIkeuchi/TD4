#include "ThirdBoss.h"
#include <any>
#include <random>

#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Shake.h"
#include "Player.h"

void (ThirdBoss::* ThirdBoss::stateTable[])() = {
	&ThirdBoss::WaitUpdate,//要素0
	&ThirdBoss::MoveUpdate, //要素1
	&ThirdBoss::ControlUpdate,
};


//生成
ThirdBoss::ThirdBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Tyuta);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool ThirdBoss::Initialize() {
	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 1.0f,1.4f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = -90.f;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "pos")));
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "HeartTarget")));
	m_MaxHp = m_HP;

	ActionTimer = 1;

	m_Radius = 5.0f;
	return true;
}

void ThirdBoss::SkipInitialize() {

}
//行動
void ThirdBoss::Action() {
	if (m_HP < 0.1) return;
	


	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
}
//ポーズ
void ThirdBoss::Pause() {


}

void ThirdBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.1f)return;
}
//描画
void ThirdBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	EffecttexDraw(dxCommon);
}

void ThirdBoss::WaitUpdate() {
}

void ThirdBoss::MoveUpdate() {
}

void ThirdBoss::ControlUpdate() {
}
