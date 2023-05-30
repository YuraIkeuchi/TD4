﻿#include "SecondBoss.h"
#include "ModelManager.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Player.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Easing.h"
#include <random>
//生成
SecondBoss::SecondBoss() {
	//モデル初期化と読み込み
	m_fbxObject.reset(new IKEFBXObject3d());
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::KIDO));
	m_fbxObject->LoadAnimation();

	shake = make_unique< Shake>();

	mark.reset(IKETexture::Create(ImageManager::MARK, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	mark->TextureCreate();
	mark->SetRotation({ 90.0f,0.0f,0.0f });
	mark->SetScale({ 3.5f,3.5f,3.5f });
	mark->SetColor(m_MarkColor);
}
//初期化
bool SecondBoss::Initialize() {
	m_Position = { 0.0f,30.0f,20.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_OBBScale = { 6.0f,6.0f,6.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Scale = { 0.05f,0.05f,0.05f };
	m_MaxHp = m_HP;
	m_AddPower = 0.8f;
	m_Radius = 5.0f;
	_charaState = CharaState::STATE_STAMP;
	m_MoveState = MOVE_ALTER;
	m_RandomType = RANDOM_START;
	m_RollType = ROLL_ONE;
	m_PressType = PRESS_START;	
	_InterValState = DownState;
	//CSVはこっから
	CSVLoad();
	return true;
}
//CSVロード系
void SecondBoss::CSVLoad() {
	//インターバル系を読み込む
	auto PressSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bossinterval.csv", "PRESS_NUM")));
	auto RandomSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bossinterval.csv", "RANDOM_NUM")));

	m_StampInterval.resize(PressSize);
	m_RandomInterval.resize(RandomSize);

	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/bossinterval.csv", m_StampInterval, "Interval");
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/bossinterval.csv", m_RandomInterval, "RandomInterval");

	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "hp2")));

	m_MoveInterval = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bossinterval.csv", "MoveInterVal")));
	m_QuickMoveInterval = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bossinterval.csv", "MoveInterVal2")));
	m_ChoiceInterval = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bossinterval.csv", "ChoiceInterVal")));
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (SecondBoss::* SecondBoss::stateTable[])() = {
	&SecondBoss::Move,//待機
	&SecondBoss::Stamp,//移動
	&SecondBoss::RandomStamp,//ランダムでスタンプ
	&SecondBoss::Rolling,//転がる攻撃
};
//行動
void SecondBoss::Action() {
	//当たり判定（弾）
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA);
	//m_MatRot = m_Object->GetMatrot();
	//OBJのステータスのセット
	//Obj_SetParam();
	Fbx_SetParam();
	//どっち使えばいいか分からなかったから保留
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
	m_MatRot = m_fbxObject->GetMatrot();
	//スタンプ更新
	StampUpdate(angerstamps);
	StampUpdate(joystamps);

	//衝撃波
	for (ShockWave* wave : shockwaves) {
		if (wave != nullptr) {
			wave->Update();
		}
	}

	//マーク
	for (Predict* predict : predicts) {
		if (predict != nullptr) {
			predict->Update();
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

	//マークの削除
	for (int i = 0; i < predicts.size(); i++) {
		if (predicts[i] == nullptr) {
			continue;
		}

		if (!predicts[i]->GetAlive()) {
			predicts.erase(cbegin(predicts) += i);
		}
	}

	//当たり判定
	Collide();
	//テキスチャ
	MarkUpdate();
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

	//マーク
	for (Predict* predict : predicts) {
		if (predict != nullptr) {
			predict->Draw(dxCommon);
		}
	}

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	mark->Draw();
	IKETexture::PostDraw();
}
//描画
void SecondBoss::Draw(DirectXCommon* dxCommon) {
	EffecttexDraw(dxCommon);
	Fbx_Draw(dxCommon);
}
//ダメージ時のリアクション
void SecondBoss::DamAction()
{
}
//ImGui
void SecondBoss::ImGui_Origin() {
	/*ImGui::Begin("SecondBoss");
	ImGui::End();*/
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
	else if(m_MoveState == MOVE_CHOICE) {
		ChoiceMove();//選択
	}
	
	//イージングで設定する
	m_FollowSpeed = Ease(In, Cubic, m_Frame, m_FollowSpeed, m_AfterFollowSpeed);
	m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, m_AfterRot.x);
	m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, m_AfterPos.y);
	//追従
	Helper::GetInstance()->FollowMove(m_Position, Player::GetInstance()->GetPosition(), m_FollowSpeed);
}
//攻撃
void SecondBoss::Stamp() {
	float l_AddFrame;
	//int l_RandAct;
	if (m_PressType == PRESS_START) {			//上に行く
		l_AddFrame = 0.01f;
		m_AfterPos = { m_Position.x,60.0f,m_Position.z };	//真上に飛ぶ
		if (m_Frame < m_FrameMax) {
			m_Frame += 0.01f;
		}
		else {
			m_Frame = 1.0f;
			if (Helper::GetInstance()->CheckMin(m_StopTimer,m_StampInterval[PRESS_START], 1)) {			//プレイヤーの位置に移動
				m_Position = { Player::GetInstance()->GetPosition().x,
				m_Position.y,
				Player::GetInstance()->GetPosition().z };
				StampInit(PRESS_SET, false);
			}
		}

		m_Position = {
			Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
			Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
			Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
	}
	else if (m_PressType == PRESS_SET) {		//一定時間上で待機
		if (Helper::GetInstance()->CheckMin(m_StopTimer, m_StampInterval[PRESS_SET], 1)) {		//次の行動
			StampInit(PRESS_ATTACK, false);
			m_AfterRot.x = m_Rotation.x + 360.0f;
		}
	}
	else if (m_PressType == PRESS_ATTACK) {			//落下してくる
		l_AddFrame = 0.05f;
		m_AfterPos = { m_Position.x,6.0f,m_Position.z };
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = 1.0f;
			if (m_StopTimer == 1) {
				BirthWave();//ウェーブの生成
			}
			if (Helper::GetInstance()->CheckMin(m_StopTimer, m_StampInterval[PRESS_ATTACK], 1)){			//シェイクが始まる
				StampInit(PRESS_SHAKE, false);
				shake->SetShakeStart(true);
			}
		}
		m_Position = {
			Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
			Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
			Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
		m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, m_AfterRot.x);
	}
	else if (m_PressType == PRESS_SHAKE) {			//シェイク
		int ShakeTimer = 50;
		//50フレーム分シェイクする
		m_StopTimer++;
		shake->ShakePos(m_ShakePos.x, 5, -5, ShakeTimer, 10);
		shake->ShakePos(m_ShakePos.z, 5, -5, ShakeTimer, 10);
		m_Position.x += m_ShakePos.x;
		m_Position.z += m_ShakePos.z;
		//シェイクを止める
		if (!shake->GetShakeStart()) {
			m_ShakePos = { 0.0f,0.0f,0.0f };
			//スタンプを押す
			if (m_StopTimer == 51) {
				BirthStamp("Anger");
			}
		}
		
		//次の行動
		if (Helper::GetInstance()->CheckMin(m_StopTimer, m_StampInterval[PRESS_SHAKE], 1)) {
			m_Rotation.x = 0.0f;
			StampInit(PRESS_RETURN, false);
		}
	
	}
	else if (m_PressType == PRESS_RETURN) {//上に戻る
		l_AddFrame = 0.01f;
		//上に戻る
		m_AfterPos = { m_Position.x,30.0f,m_Position.z };
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			if (Helper::GetInstance()->CheckMin(m_StopTimer, m_StampInterval[PRESS_RETURN], 1)) {
				StampInit(PRESS_END, false);
			}
		}

		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, m_AfterPos.y);
	}
	else {
	//次の行動を決める
		_charaState = STATE_MOVE;
		m_MoveState = MOVE_CHOICE;
	}
}
//ランダム攻撃
void SecondBoss::RandomStamp() {
	float l_AddFrame;//加算されるフレーム
	const float l_MaxPosX = 62.0f;//ランダムで取る座標の最大、最小値
	const float l_MinPosX = -52.0f;
	const float l_MaxPosZ = 57.0f;
	const float l_MinPosZ = -57.0f;

	const int l_RandMax = 20;
	const int l_RandMin = -20;
	if (m_RandomType == RANDOM_START) {			//スタート
		l_AddFrame = 0.01f;
		m_AfterPos = { m_Position.x,60.0f,m_Position.z };	//真上に飛ぶ
		if (m_Frame < m_FrameMax) {
			m_Frame += 0.01f;
		}
		else {
			//乱数指定
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_RandX(l_RandMin, l_RandMax);
			mt19937 mt2{ std::random_device{}() };
			uniform_int_distribution<int> l_RandZ(l_RandMin, l_RandMax);
			//座標をランダムで決める
			if (Helper::GetInstance()->CheckMin(m_StopTimer, m_RandomInterval[RANDOM_START], 1)) {			//プレイヤーの位置に移動
				m_Position.x = (float(l_RandX(mt))) + (Player::GetInstance()->GetPosition().x);
				m_Position.z = (float(l_RandZ(mt2))) + (Player::GetInstance()->GetPosition().z);

				Helper::GetInstance()->Clamp(m_Position.x, l_MinPosX,l_MaxPosX);
				Helper::GetInstance()->Clamp(m_Position.z, l_MinPosZ,l_MaxPosZ);
				StampInit(RANDOM_SET, true);
			}
		}

		m_Position = {
			Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
			Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
			Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
	}
	else if (m_RandomType == RANDOM_SET) {		//一定時間上で待機
		if (m_StopTimer == 1) {
			BirthPredict();//予測位置にマークを出す
		}
		if (Helper::GetInstance()->CheckMin(m_StopTimer, m_RandomInterval[RANDOM_SET], 1)) {		//次の行動
			StampInit(RANDOM_ATTACK, true);
			m_Rotation.x = 0.0f;
		}
	}
	else if (m_RandomType == RANDOM_ATTACK) {
		l_AddFrame = 0.05f;
		m_AfterPos.y = 6.0f;
		const int l_MoveMax = 10;
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = 1.0f;
			if (m_StopTimer == 1) {
				//スタンプと衝撃波の生成
				BirthStamp("Anger");
				BirthWave();
			}
			if (Helper::GetInstance()->CheckMin(m_StopTimer, m_RandomInterval[RANDOM_ATTACK], 1)) {
				if (m_MoveCount < l_MoveMax) {		//何回スタンプを押したかで最初に戻るか別の行動をするか決まる
					m_MoveCount++;
					StampInit(RANDOM_START, true);
				}
				else {
					m_MoveCount = 0;
					StampInit(RANDOM_END, true);
				}
			}
		}
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, m_AfterPos.y);
	}
	else {
		//次の行動を決める
		_charaState = STATE_MOVE;
		m_MoveState = MOVE_CHOICE;
	}
}
//転がる攻撃
void SecondBoss::Rolling() {
	XMFLOAT3 l_AfterPos;
	float l_AfterRotY;
	float l_AddFrame;

	//転がる位置を指定する
	if (m_RollType == ROLL_ONE) {
		l_AfterPos = { 0.0f,m_Position.y,30.0f };
		l_AddFrame = 0.007f;
		l_AfterRotY = 225.0f;
		RollEaseCommn(l_AfterPos, l_AddFrame,l_AfterRotY);
		//飛ぶような感じにするため重力を入れる
		m_AddPower -= m_Gravity;
		Helper::GetInstance()->CheckMax(m_Position.y, 5.0f, m_AddPower);
		//回転を決める
		m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, 90.0f);
	}
	else if (m_RollType == ROLL_SECOND) {
		l_AfterPos = { 55.0f,m_Position.y,-50.0f };
		l_AddFrame = 0.007f;
		l_AfterRotY = 90.0f;
		RollEaseCommn(l_AfterPos, l_AddFrame, l_AfterRotY);
	}
	else if (m_RollType == ROLL_THIRD) {
		l_AfterPos = { 55.0f,m_Position.y,50.0f };
		l_AddFrame = 0.007f;
		l_AfterRotY = 0.0f;
		RollEaseCommn(l_AfterPos, l_AddFrame, l_AfterRotY);
	}
	else if (m_RollType == ROLL_FOURTH) {
		l_AfterPos = { -45.0f,m_Position.y,50.0f };
		l_AddFrame = 0.007f;
		l_AfterRotY = -90.0f;
		RollEaseCommn(l_AfterPos, l_AddFrame, l_AfterRotY);
	}
	else if (m_RollType == ROLL_FIVE) {
		l_AfterPos = { -45.0f,m_Position.y,-50.0f };
		l_AddFrame = 0.007f;
		l_AfterRotY = -225.0f;
		RollEaseCommn(l_AfterPos, l_AddFrame, l_AfterRotY);

		m_AddPower = 0.8f;
	}
	else if (m_RollType == ROLL_SIX) {
		l_AfterPos = { 0.0f,m_Position.y,30.0f };
		l_AddFrame = 0.007f;
		l_AfterRotY = -90.0f;
		RollEaseCommn(l_AfterPos, l_AddFrame, l_AfterRotY);

		//飛ぶような感じにするため重力を入れる
		m_AddPower -= m_Gravity;
		Helper::GetInstance()->CheckMax(m_Position.y, 6.0f, m_AddPower);
		m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, 0.0f);
	}
	else {
		m_Rotation = { 0.0f,90.0f,0.0f };
		//次の行動を決める
		m_AddPower = 0.8f;
		_charaState = STATE_MOVE;
		m_MoveState = MOVE_CHOICE;
		m_fbxObject->PlayAnimation(1);
	}
}
//転がるやつの共通イージング
void SecondBoss::RollEaseCommn(const XMFLOAT3& AfterPos, const float AddFrame,const float AfterRot) {
	float l_CheckFrame = 0.4f;

	//座標のイージング
	if (m_Frame < m_FrameMax) {
		m_Frame += AddFrame;
	}
	else {
		m_Frame = 1.0f;
	}
	
	//回転のイージング
	if (m_Frame > l_CheckFrame) {
		m_ChangeRot = true;
		m_fbxObject->StopAnimation();
	}

	ChangeRot(AfterRot);
	//回転と座標のイージングどっちも終わったら終了
	if (m_Frame == 1.0f && m_RotFrame == 1.0f) {
		m_Frame = {};
		m_RotFrame = {};
		m_RollType++;
		m_AnimationSpeed = 5;
		m_fbxObject->PlayAnimation(0);
	}

	m_Position = {
Ease(In,Cubic,m_Frame,m_Position.x,AfterPos.x),
Ease(In,Cubic,m_Frame,m_Position.y,AfterPos.y),
	Ease(In,Cubic,m_Frame,m_Position.z,AfterPos.z)
	};
}
//回転の共通変数
void SecondBoss::ChangeRot(const float AfterRot) {
	const float l_AddFrame = 0.05f;
	if (m_ChangeRot) {
		if (m_RotFrame < m_FrameMax) {
			m_RotFrame += l_AddFrame;
		}
		else {
			m_RotFrame = 1.0f;
			m_ChangeRot = false;
		}
	}

	m_Rotation.y = Ease(In, Cubic, m_RotFrame, m_Rotation.y, AfterRot);
}
//スタンプの生成
void SecondBoss::BirthStamp(const std::string& stampName) {
	//指定された名前によってスタンプの種類を変える
	if (stampName == "Anger") {//怒りのスタンプ
		InterStamp* newstamp;
		newstamp = new AngerStamp();
		newstamp->Initialize(m_Position);
		angerstamps.push_back(newstamp);
	}
	else if (stampName == "Joy") {//喜びのスタンプ
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
	float l_AddFrame = 0.03f;

	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		//上に上がる
		if (_InterValState == UpState) {
			m_AfterRot.x += 180.0f;
			MoveInit("UPSTATE");
		}
		//下に落ちる
		else {
			m_StopTimer++;
			if (m_StopTimer == 1) {
				m_MoveCount++;
				//回転によってどっちのスタンプが押されるかを選ぶ
				if (m_Rotation.x == 360.0f) {
					BirthStamp("Anger");
					m_Rotation.x = 0.0f;
					m_AfterRot.x = 0.0f;
				}
				else {
					BirthStamp("Joy");
					m_Check = true;
				}
			}
			else if (m_StopTimer == m_MoveInterval) {
				MoveInit("DOWNSTATE");
			}
		}
	}

	//3回行動をしたら動きを選択に戻す
	if (m_MoveCount == 3) {
		m_MoveState = MOVE_CHOICE;
	}
}
//怒りの動き
void SecondBoss::AngerMove() {
	float l_AddFrame = 0.03f;
	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		//上に上がる
		if (_InterValState == UpState) {
			m_AfterRot.x = 0.0f;
			MoveInit("UPSTATE");
		}
		else {
			//下に落ちる(スタンプが押される)
			m_StopTimer++;
			if (m_StopTimer == 1) {
				m_MoveCount++;
				BirthStamp("Anger");
			}
			else if (m_StopTimer == m_QuickMoveInterval) {
				MoveInit("DOWNSTATE");
			}
		}
	}

	//三回押されたら動きを選択する
	if (m_MoveCount == 3) {
		m_MoveState = MOVE_CHOICE;
	}
}
//喜びの動き
void SecondBoss::JoyMove() {
	float l_AddFrame = 0.03f;
	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		//上に上がる
		if (_InterValState == UpState) {
			m_AfterRot.x = 180.0f;
			MoveInit("UPSTATE");
		}
		//下に落ちる(喜びのスタンプが押される)
		else {
			m_StopTimer++;
			if (m_StopTimer == 1) {
				m_MoveCount++;
				BirthStamp("Joy");
				m_Check = true;
			}
			else if (m_StopTimer == m_QuickMoveInterval) {
				MoveInit("DOWNSTATE");
			}
		}
	}

	//三回押されたら動きを選択する
	if (m_MoveCount == 3) {
		m_MoveState = MOVE_CHOICE;
	}
}
//動きの選択
void SecondBoss::ChoiceMove() {
	int l_RandState = 0;
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 50);

	m_StopTimer++;
	//一定時間立ったらランダムで行動選択
	if (m_StopTimer > m_ChoiceInterval) {
		m_Frame = 0.0f;
		m_StopTimer = 0;
		l_RandState = int(l_RandomMove(mt));
		//RandStateが30以下ならそれに応じた移動にする、
		if (l_RandState <= 30) {
			m_MoveCount = 0;
			_InterValState = UpState;
			_charaState = STATE_MOVE;
			m_FollowSpeed = 1.0f;
			m_AfterPos.y = 30.0f;

			if (l_RandState <= 10) {
				m_MoveState = MOVE_ALTER;
			}
			else if (l_RandState >= 11 && l_RandState <= 20) {
				m_MoveState = MOVE_JOY;
			}
			else {
				m_MoveState = MOVE_ANGER;
			}
		}
		else if (l_RandState >= 31 && l_RandState <= 37) {	//スタンプ攻撃
			_charaState = STATE_STAMP;
			m_PressType = PRESS_START;
		}
		else if (l_RandState >= 38 && l_RandState <= 44) {	//ランダム攻撃
			_charaState = STATE_RANDOM;
			m_RandomType = RANDOM_START;
		}
		else {			//ローリング攻撃
			_charaState = STATE_ROLL;
			m_RollType = ROLL_ONE;
		}
	}
}
//移動関係の初期化
void SecondBoss::MoveInit(const std::string& HighState) {
	//上にいるか下にいるかで初期化するものが違う
	if (HighState == "UPSTATE") {
		_InterValState = DownState;
		m_Frame = 0.0f;
		m_AfterPos.y = 6.0f;
	}
	else if (HighState == "DOWNSTATE") {
		_InterValState = UpState;
		m_AfterPos.y = 30.0f;
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

	//本来のポジションより低い位置で取る
	l_OBBPosition = { m_Position.x,
		m_Position.y,
		m_Position.z
	};

	//OBBの当たり判定を取る
	if (!Helper::GetInstance()->CheckMin(m_CheckTimer,10,1)) { return false; }
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
	//衝撃波の発生
	ShockWave* newwave;
	newwave = new ShockWave();
	newwave->Initialize({ m_Position.x,0.0f,m_Position.z });
	shockwaves.push_back(newwave);
}
//テクスチャの更新
void SecondBoss::MarkUpdate() {
	mark->Update();
	mark->SetPosition({ m_Position.x,0.0f,m_Position.z });
	mark->SetColor(m_MarkColor);
	if (_charaState == STATE_STAMP) {
		if (m_PressType == PRESS_SET) {
			m_MarkColor.w = Ease(In, Cubic, 0.5f, m_MarkColor.w, 1.0f);
		}
		else {
			m_MarkColor.w = Ease(In, Cubic, 0.5f, m_MarkColor.w, 0.0f);
		}
	}
	else {
		m_MarkColor.w = Ease(In, Cubic, 0.5f, m_MarkColor.w, 0.0f);
	}


}
//予測テクスチャの生成
void SecondBoss::BirthPredict() {
	//衝撃波の発生
	Predict* newpredict;
	newpredict= new Predict();
	newpredict->Initialize({ m_Position.x,0.0f,m_Position.z });
	predicts.push_back(newpredict);
}
//スタンプ攻撃の初期化(共通のみ)
void SecondBoss::StampInit(const int AttackNumber, const bool Random) {
	m_Frame = 0.0f;
	m_StopTimer = 0;
	if (Random) {
		m_RandomType = AttackNumber;
	}
	else {
		m_PressType = AttackNumber;
	}
}
//ボス登場シーン
void SecondBoss::AppearAction() {
	Input* input = Input::GetInstance();
	float l_AddFrame = 0.0f;
	if (m_AppearState == APPEAR_START) {//初期座標セット
		m_AppearTimer++;
		if (m_AppearTimer == 300) {
			m_AppearState = APPEAR_SET;
			m_Frame = {};
			m_AfterPos.y = 8.0f;
			m_AppearTimer = {};
		}
	}
	else if (m_AppearState == APPEAR_SET) {//落下してくる
		l_AddFrame = 0.1f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_AppearState = APPEAR_LOOK;
		}
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, m_AfterPos.y);
	}
	else if (m_AppearState == APPEAR_LOOK) {		//プレイヤーを見てくる
		m_AppearTimer++;
		if (m_AppearTimer == 50) {
			m_AppearTimer = 0;
			m_AppearState = APPEAR_DIR;
			m_AfterRot = { 180.0f,90.0f,90.0f };
		}
	}
	else if (m_AppearState == APPEAR_DIR) {
		l_AddFrame = 0.01f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 0.0f;
			m_AppearState = APPEAR_STOP;
		}
		m_Rotation = { Ease(In,Cubic,m_Frame,m_Rotation.x,m_AfterRot.x),
			m_Rotation.y,
			Ease(In,Cubic,m_Frame,m_Rotation.z,m_AfterRot.z),
		};
	}
	//テキストが出てる間
	else if (m_AppearState == APPEAR_STOP) {
		if (m_DirEmo == DIR_ANGER) {
			m_AfterRot.y = 90.0f;
		}
		else {
			m_AfterRot.y = 270.0f;
		}
		m_Rotation.y = Ease(In, Cubic, 0.5f, m_Rotation.y, m_AfterRot.y);
		if (m_FinishApp) {
			m_Frame = 0.0f;
			m_AppearState = APPEAR_END;
			m_AfterRot = { 0.0f,90.0f,0.0f };
		}
	}
	else if (m_AppearState == APPEAR_END) {
		l_AddFrame = 0.01f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 0.0f;
			m_FinishAppear = true;
		}
		m_Rotation = { Ease(In,Cubic,m_Frame,m_Rotation.x,m_AfterRot.x),
			Ease(In,Cubic,m_Frame,m_Rotation.y,m_AfterRot.y),
			Ease(In,Cubic,m_Frame,m_Rotation.z,m_AfterRot.z),
		};
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 20.0f);
	}

	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
	Fbx_SetParam();
}
//ボス撃破シーン
void SecondBoss::DeadAction() {

}
//ボス撃破シーン
void SecondBoss::DeadAction_Throw() {

}