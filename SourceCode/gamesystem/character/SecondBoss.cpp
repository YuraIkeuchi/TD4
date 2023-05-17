#include "SecondBoss.h"
#include "ModelManager.h"
#include "Helper.h"
#include "Player.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Easing.h"
#include <random>
//生成
SecondBoss::SecondBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Kido);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	shake = make_unique< Shake>();
}
//初期化
bool SecondBoss::Initialize() {

	m_Position = { 0.0f,5.0f,30.0f };
	m_Rotation = { 180.0f,270.0f,0.0f };
	m_Scale = { 4.0f,4.0f,4.0f };
	m_OBBScale = { 6.0f,6.0f,6.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_AddPowerY = 5.0f;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "pos")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "hp2")));
	_charaState = CharaState::STATE_ATTACK;
	m_MoveState = MOVE_ALTER;
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
	m_MatRot = m_Object->GetMatrot();
	//OBJのステータスのセット
	Obj_SetParam();

	//スタンプ更新
	StampUpdate(angerstamps);
	StampUpdate(joystamps);

	//衝撃波
	for (ShockWave* wave : shockwaves) {
		if (wave != nullptr) {
			wave->Update();
		}
	}

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

	//衝撃波
	for (int i = 0; i < shockwaves.size();i++) {
		if (shockwaves[i] == nullptr) {
			continue;
		}

		if (!shockwaves[i]->GetAlive()) {
			shockwaves.erase(cbegin(shockwaves) += i);
		}
	}

	Collide();
}
//ポーズ
void SecondBoss::Pause() {
}
void SecondBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	//スタンプ描画
	StampDraw(angerstamps, dxCommon);
	StampDraw(joystamps, dxCommon);

	for (ShockWave* wave : shockwaves) {
		if (wave != nullptr) {
			wave->Draw(dxCommon);
		}
	}
}
//ダメージ時のリアクション
void SecondBoss::DamAction()
{

}
//ImGui
void SecondBoss::ImGui_Origin() {
	ImGui::Begin("Second");
	ImGui::Text("Timer:%d", m_StopTimer);
	ImGui::Text("RotX:%f", m_Rotation.x);
	ImGui::End();

	for (ShockWave* wave : shockwaves) {
		if (wave != nullptr) {
			wave->ImGuiDraw();
		}
	}
}
//移動
void SecondBoss::Move() {
	//どの移動方法にするか決める
	if (m_MoveState == MOVE_ALTER) {
		AlterMove();//交互のスタンプ
	}
	else if (m_MoveState == MOVE_ANGER) {
		AngerMove();//怒りのみ
	}
	else if(m_MoveState == MOVE_JOY) {
		JoyMove();//喜び
	}
	else {
		ChoiceMove();//選択
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
	float l_AddFrame;
	int l_TargetTimer;
	int l_RandAct;
	if (m_PressType == PRESS_START) {			//上に行く
		l_AddFrame = 0.01f;
		l_TargetTimer = 80;
		m_AfterPos = { m_Position.x,40.0f,m_Position.z };	//真上に飛ぶ
		if (m_Frame < m_FrameMax) {
			m_Frame += 0.01f;
		}
		else {
			m_Frame = 1.0f;
			if (Helper::GetInstance()->CheckMinINT(m_StopTimer,l_TargetTimer,1)) {			//プレイヤーの位置に移動
				m_Position = { Player::GetInstance()->GetPosition().x,
				m_Position.y,
				Player::GetInstance()->GetPosition().z };
				m_StopTimer = 0;
				m_Frame = 0.0f;
				m_PressType = PRESS_SET;
			}
		}

		m_Position = {
			Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
			Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
			Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
	}
	else if (m_PressType == PRESS_SET) {		//一定時間上で待機
		l_TargetTimer = 100;
		
		if (Helper::GetInstance()->CheckMinINT(m_StopTimer, l_TargetTimer, 1)) {
			m_StopTimer = 0;
			m_PressType = PRESS_ATTACK;
			m_AfterRotX = m_Rotation.x + 720.0f;
		}
	}
	else if (m_PressType == PRESS_ATTACK) {			//落下してくる
		l_AddFrame = 0.1f;
		l_TargetTimer = 20;
		m_AfterPos = { m_Position.x,5.0f,m_Position.z };
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = 1.0f;
			if (m_StopTimer == 1) {
				BirthWave();//ウェーブの生成
			}
			if (Helper::GetInstance()->CheckMinINT(m_StopTimer, l_TargetTimer, 1)){			//シェイクが始まる
				m_StopTimer = 0;
				m_Frame = 0.0f;
				m_PressType = PRESS_SHAKE;
				shake->SetShakeStart(true);
			}
		}
		m_Position = {
			Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
			Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
			Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
		m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, m_AfterRotX);
	}
	else if (m_PressType == PRESS_SHAKE) {			//シェイク
		l_TargetTimer = 50;
		shake->ShakePos(m_ShakePos.x, 5, -5, l_TargetTimer, 10);
		shake->ShakePos(m_ShakePos.z, 5, -5, l_TargetTimer, 10);
		if (!shake->GetShakeStart()) {
			m_Rotation.x = m_Rotation.x - 720.0f;
			m_AfterRotX = m_Rotation.x;
			m_ShakePos = { 0.0f,0.0f,0.0f };
			m_PressType = PRESS_END;
			BirthStamp("Anger");
		}

		m_Position.x += m_ShakePos.x;
		m_Position.z += m_ShakePos.z;
	}
	else {
		//乱数指定
		mt19937 mt{ std::random_device{}() };
		uniform_int_distribution<int> l_RandomMove(0, 2);

		l_RandAct = int(l_RandomMove(mt));

		if (l_RandAct == 0) {
			_charaState = STATE_ATTACK;
			m_PressType = PRESS_START;
		}
		else {
			_charaState = STATE_MOVE;
			m_MoveState = MOVE_CHOICE;
		}
	}

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
//交互のスタンプ
void SecondBoss::AlterMove() {

	float l_AddFrame = 0.025f;
	const int l_TargetStopTimer = 50;

	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		if (_InterValState == UpState) {
			m_AfterRotX += 180.0f;
			MoveInit("UPSTATE");
		}
		else {
			m_StopTimer++;
			if (m_StopTimer == 1) {
				m_MoveCount++;
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
				MoveInit("DOWNSTATE");
			}
		}
	}

	if (m_MoveCount == 3) {
		m_MoveState = MOVE_CHOICE;
	}
}
//怒りの動き
void SecondBoss::AngerMove() {
	float l_AddFrame = 0.05f;
	const int l_TargetStopTimer = 10;
	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		if (_InterValState == UpState) {
			m_AfterRotX = 0.0f;
			MoveInit("UPSTATE");
		}
		else {
			m_StopTimer++;
			if (m_StopTimer == 1) {
				m_MoveCount++;
				BirthStamp("Anger");
			}
			else if (m_StopTimer == l_TargetStopTimer) {
				MoveInit("DOWNSTATE");
			}
		}
	}

	if (m_MoveCount == 3) {
		m_MoveState = MOVE_CHOICE;
	}
}
//喜びの動き
void SecondBoss::JoyMove() {
	float l_AddFrame = 0.05f;
	const int l_TargetStopTimer = 10;
	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		if (_InterValState == UpState) {
			m_AfterRotX = 180.0f;
			MoveInit("UPSTATE");
		}
		else {
			m_StopTimer++;
			if (m_StopTimer == 1) {
				m_MoveCount++;
				BirthStamp("Joy");
				m_Check = true;
			}
			else if (m_StopTimer == l_TargetStopTimer) {
				MoveInit("DOWNSTATE");
			}
		}
	}

	if (m_MoveCount == 3) {
		m_MoveState = MOVE_CHOICE;
	}
}
//動きの選択
void SecondBoss::ChoiceMove() {
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 5);

	m_MoveState = int(l_RandomMove(mt));
	m_MoveCount = 0;
	if (m_MoveState <= 2) {
		_charaState = STATE_MOVE;
	}
	else {
		_charaState = STATE_ATTACK;
	}
}
//移動関係の初期化
void SecondBoss::MoveInit(const std::string& HighState) {
	if (HighState == "UPSTATE") {
		_InterValState = DownState;
		m_Frame = 0.0f;
		m_AfterPower = 0.0f;
	}
	else if (HighState == "DOWNSTATE") {
		_InterValState = UpState;
		m_AfterPower = 20.0f;
		m_FollowSpeed = 1.0f;
		m_Frame = 0.0f;
		m_StopTimer = 0;
	}
	else {
		assert(0);
	}
}
//当たり判定
bool SecondBoss::Collide() {

	XMFLOAT3 l_OBBPosition;

	l_OBBPosition = { m_Position.x,
		m_Position.y - 8.0f,
		m_Position.z
	};

	if (!Helper::GetInstance()->CheckMinINT(m_CheckTimer,10,1)) { return false; }
	m_OBB1.SetParam_Pos(l_OBBPosition);
	m_OBB1.SetParam_Rot(m_MatRot);
	m_OBB1.SetParam_Scl(m_OBBScale);

	m_OBB2.SetParam_Pos(Player::GetInstance()->GetPosition());
	m_OBB2.SetParam_Rot(Player::GetInstance()->GetMatRot());
	m_OBB2.SetParam_Scl(Player::GetInstance()->GetScale());

	if ((Collision::OBBCollision(m_OBB1, m_OBB2) && 
		Player::GetInstance()->GetDamageInterVal() == 0)) {
		Player::GetInstance()->PlayerHit(m_Position);
		Player::GetInstance()->RecvDamage(1.0f);

		return true;
	}
	else {
		return false;
	}

	return false;
}
//衝撃波の発生
void SecondBoss::BirthWave() {
	ShockWave* newwave;
	newwave = new ShockWave();
	newwave->Initialize({ m_Position.x,0.0f,m_Position.z });
	shockwaves.push_back(newwave);
}