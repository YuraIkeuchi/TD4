#include "FourthBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Shake.h"
#include "Player.h"

void (FourthBoss::* FourthBoss::stateTable[])() = {
	&FourthBoss::Choice,//選択
	&FourthBoss::Follow, //追従
	&FourthBoss::Diffusion,//拡散
	&FourthBoss::Confusion//混乱
};


//生成
FourthBoss::FourthBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Tyuta);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	note.reset(new Note);
}

bool FourthBoss::Initialize() {
	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 1.0f,1.4f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = -90.f;
	
	m_MaxHp = m_HP;

	ActionTimer = 1;

	m_Radius = 5.0f;

	note->Initialize();

	_charaState = STATE_CHOICE;

	//CSVロード
	CSVLoad();
	return true;
}

void FourthBoss::SkipInitialize() {
	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 1.0f,1.4f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
}

void FourthBoss::CSVLoad() {
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "HeartTarget")));
	m_ChoiceInterval = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "ChoiceInterVal")));
}
//行動
void FourthBoss::Action() {
	//状態移行(charastateに合わせる)
	if (m_HP > 0.0f) {
		(this->*stateTable[_charaState])();
	}

	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);

	note->Update();
}
//ポーズ
void FourthBoss::Pause() {


}

void FourthBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.1f)return;
}
//描画
void FourthBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	note->Draw(dxCommon);
	EffecttexDraw(dxCommon);
}

//ImGui
void FourthBoss::ImGui_Origin() {
	ImGui::Begin("Fourth");
	ImGui::Text("Stop:%d", m_StopTimer);
	ImGui::Text("Choice:%d", m_ChoiceInterval);
	ImGui::End();
}
//動きの選択
void FourthBoss::Choice() {
	m_StopTimer++;
	if (m_StopTimer > m_ChoiceInterval) {

	}
}

//追従
void FourthBoss::Follow() {

}
//拡散
void FourthBoss::Diffusion() {

}

//混乱
void FourthBoss::Confusion() {

}

//登場シーン
void FourthBoss::AppearAction() {
	Obj_SetParam();
}