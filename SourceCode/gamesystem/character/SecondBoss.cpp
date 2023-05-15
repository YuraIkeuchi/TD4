#include "SecondBoss.h"
#include "ModelManager.h"
#include "Helper.h"
#include <any>
#include "Player.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Easing.h"
//生成
SecondBoss::SecondBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Kido);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool SecondBoss::Initialize() {

	m_Position = { 0.0f,0.0f,30.0f };
	m_Rotation = { 180.0f,0.0f,0.0f };
	m_Scale = { 4.0f,4.0f,4.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_AddPowerY = 5.0f;
	m_Rotation.y = 90.f;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "pos")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "hp2")));
	_charaState = CharaState::STATE_MOVE;
	return true;
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (SecondBoss::* SecondBoss::stateTable[])() = {
	&SecondBoss::Move,//待機
	&SecondBoss::Attack,//移動
};
//行動
void SecondBoss::Action() {
	//当たり判定（弾）
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA);

	//OBJのステータスのセット
	Obj_SetParam();

	//スタンプ更新
	StampUpdate(angerstamps);
	StampUpdate(joystamps);

	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();

	//スタンプの削除
	for (int i = 0; i < angerstamps.size(); i++) {
		if (angerstamps[i] == nullptr) {
			continue;
		}

		if (!angerstamps[i]->GetBirth()) {
			angerstamps.erase(cbegin(angerstamps) + i);
		}
	}

	for (int i = 0; i < joystamps.size(); i++) {
		if (joystamps[i] == nullptr) {
			continue;
		}

		if (!joystamps[i]->GetBirth()) {
			joystamps.erase(cbegin(joystamps) + i);
		}
	}
}
//ポーズ
void SecondBoss::Pause() {


}
void SecondBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	//スタンプ描画
	StampDraw(angerstamps, dxCommon);
	StampDraw(joystamps, dxCommon);
}

//ダメージ時のリアクション
void SecondBoss::DamAction()
{

}

//ImGui
void SecondBoss::ImGui_Origin() {
	ImGui::Begin("Second");
	ImGui::Text("StopTimer:%d", m_StopTimer);
	ImGui::End();
	//StampImGui(angerstamps);
	//StampImGui(joystamps);
}

//移動
void SecondBoss::Move() {
	float l_AddFrame = 0.025f;
	int l_TargetStopTimer = 50;
	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		if (_InterValState == UpState) {
			_InterValState = DownState;
			m_AfterPower = 0.0f;
		}
		else {
			m_StopTimer++;
			if (m_StopTimer == 1) {
				if (m_Rotation.x == 360.0f) {
					BirthStamp("Anger");
					m_Rotation.x = 0.0f;
					m_AfterRotX = 0.0f;
				}
				else {
					BirthStamp("Joy");
					m_Check = true;
				}
			}
			else if (m_StopTimer == l_TargetStopTimer) {
				_InterValState = UpState;
				m_AfterRotX += 180.0f;
				m_AfterPower = 5.0f;
				m_FollowSpeed = 1.0f;
				m_Frame = 0.0f;
				m_StopTimer = 0;
			}
		}
	}

	//イージングで設定する
	m_FollowSpeed = Ease(In, Cubic, m_Frame, m_FollowSpeed, m_AfterFollowSpeed);
	m_AddPowerY = Ease(In, Cubic, m_Frame, m_AddPowerY, m_AfterPower);
	m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, m_AfterRotX);
	//座標取得
	m_Position.y = m_AddPowerY + 5.0f;

	//追従
	Helper::GetInstance()->FollowMove(m_Position, Player::GetInstance()->GetPosition(), m_FollowSpeed);
}
//攻撃
void SecondBoss::Attack() {

}
//スタンプの生成
void SecondBoss::BirthStamp(const std::string& stampName) {
	if (stampName == "Anger") {
		InterStamp* newstamp;
		newstamp = new AngerStamp();
		newstamp->Initialize(m_Position);
		angerstamps.push_back(newstamp);
	}
	else if (stampName == "Joy") {
		InterStamp* newstamp;
		newstamp = new JoyStamp();
		newstamp->Initialize(m_Position);
		joystamps.push_back(newstamp);
	}
	else {
		assert(0);
	}
}

//スタンプの更新
void SecondBoss::StampUpdate(std::vector<InterStamp*> stamps) {
	for (InterStamp* stamp : stamps) {
		if (stamp != nullptr) {
			stamp->Update();
		}
	}
}

//スタンプの描画
void SecondBoss::StampDraw(std::vector<InterStamp*> stamps, DirectXCommon* dxCommon) {
	for (InterStamp* stamp : stamps) {
		if (stamp != nullptr) {
			stamp->Draw(dxCommon);
		}
	}
}

//スタンプのImGui
void SecondBoss::StampImGui(std::vector<InterStamp*> stamps) {
	for (InterStamp* stamp : stamps) {
		if (stamp != nullptr) {
			stamp->ImGuiDraw();
		}
	}
}