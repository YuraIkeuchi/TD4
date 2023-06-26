#include "SevenBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Player.h"
#include "VariableCommon.h"

//生成
SevenBoss::SevenBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DJ);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool SevenBoss::Initialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 0.0f,0.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 5.2f;

	//CSVロード
	CSVLoad();
	return true;
}
//スキップ時の初期化
void SevenBoss::SkipInitialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 0.0f,0.0f,1.0f,1.0f };
}
//CSV
void SevenBoss::CSVLoad() {
	
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "HeartTarget")));

	m_MaxHp = m_HP;
}
//行動
void SevenBoss::Action() {
	//状態移行(charastateに合わせる)
	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//プレイヤーの当たり判定
	ColPlayer();
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
}
//ポーズ
void SevenBoss::Pause() {

}
//エフェクト描画
void SevenBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;

}
//描画
void SevenBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void SevenBoss::ImGui_Origin() {
}

//登場シーン
void SevenBoss::AppearAction() {
	Obj_SetParam();
}
//ボス撃破シーン
void SevenBoss::DeadAction() {

	Obj_SetParam();
}
//ボス撃破シーン(スロー)
void SevenBoss::DeadAction_Throw() {
	Obj_SetParam();
}
