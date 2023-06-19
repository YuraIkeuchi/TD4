#include "FourthBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Player.h"
#include "VariableCommon.h"

void (FourthBoss::* FourthBoss::stateTable[])() = {
	&FourthBoss::InterValMove,//動きの合間
	&FourthBoss::Choice,//選択
	&FourthBoss::LineSet,//ダメージエリアのセット
	&FourthBoss::Debuff,//デバフ
	&FourthBoss::Confu,//混乱
	&FourthBoss::Barrage,//弾幕
	&FourthBoss::Throw,//投げる
	&FourthBoss::EndMove,//行動の終わり
};
//生成
FourthBoss::FourthBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DJ);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	//CDの初期化
	cd[CD_BARRA].reset(new BarrangeCD);
	cd[CD_BARRA]->Initialize();

	cd[CD_LINE].reset(new LineCD);
	cd[CD_LINE]->Initialize();

	cd[CD_CONFU].reset(new ConfuCD);
	cd[CD_CONFU]->Initialize();

	cd[CD_DEBUFF].reset(new DebuffCD);
	cd[CD_DEBUFF]->Initialize();

	confueffect.reset(new ConfuEffect());
	confueffect->Initialize();

	noteeffect.reset(new NoteEffect());
	noteeffect->Initialize();
}
//初期化
bool FourthBoss::Initialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 5.2f;

	_charaState = STATE_INTER;
	m_AreaState = AREA_SET;
	//CSVロード
	CSVLoad();
	return true;
}
//スキップ時の初期化
void FourthBoss::SkipInitialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
}
//CSV
void FourthBoss::CSVLoad() {
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "HeartTarget")));
	m_ChoiceInterval = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "ChoiceInterVal")));

	m_MaxHp = m_HP;
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
	//プレイヤーの当たり判定
	ColPlayer();
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
		
	//CDの更新
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->SetCatchPos({ m_Position.x,((m_Position.y + 2.0f) + (0.5f * i)),m_Position.z });
		cd[i]->Update();

		if (cd[i]->GetBreakCD()) {
			m_EndCount++;
			cd[i]->SetBreakCD(false);
		}
	}

	//攻撃の音符
	for (AttackNote* newnote : attacknotes) {
		if (newnote != nullptr) {
			newnote->Update();
		}
	}

	//攻撃の音符の削除
	for (int i = 0; i < attacknotes.size(); i++) {
		if (attacknotes[i] == nullptr) {
			continue;
		}

		if (!attacknotes[i]->GetAlive()) {
			attacknotes.erase(cbegin(attacknotes) + i);
		}
	}

	if (damagearea != nullptr) {
		damagearea->Update();
	}

	confueffect->Update();
	noteeffect->Update();
	//HPが半分切ったら強化
	if (m_HP < m_MaxHp / 2) {
		isStrong = true;
	}
}
//ポーズ
void FourthBoss::Pause() {
	
}
//エフェクト描画
void FourthBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.1f)return;

	confueffect->Draw(dxCommon);
	noteeffect->Draw(dxCommon);
}
//描画
void FourthBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	//CDの更新
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->Draw(dxCommon);
	}
	//攻撃の音符
	for (AttackNote* newnote : attacknotes) {
		if (newnote != nullptr) {
			newnote->Draw(dxCommon);
		}
	}
	if (damagearea != nullptr) {
		damagearea->Draw(dxCommon);
	}
	EffecttexDraw(dxCommon);
}
//ImGui
void FourthBoss::ImGui_Origin() {
	ImGui::Begin("Fourth");
	ImGui::Text("Attack:%d", m_CheckTimer);
	ImGui::Text("CircleSpeed:%f", m_CircleSpeed);
	ImGui::Text("CircleScale:%f", m_CircleScale);
	ImGui::End();
}
//インターバル
void FourthBoss::InterValMove() {
	m_MoveInterVal++;
	m_AreaState = AREA_SET;
	m_ThrowState = THROW_SET;
	//すべてが消えていたら復活させる
	if ((cd[CD_LINE]->GetCDState() == CD_DEATH) && (cd[CD_DEBUFF]->GetCDState() == CD_DEATH) &&
		(cd[CD_CONFU]->GetCDState() == CD_DEATH) && (cd[CD_BARRA]->GetCDState() == CD_DEATH)) {
		for (int i = 0; i < cd.size(); i++) {
			cd[i]->SetCDState(CD_RESPORN);
		}
		m_EndCount = 0;
		_charaState = STATE_END;
	}

	//
	if (m_MoveInterVal == 50) {
		//上から順にCDを回る
		for (int i = 0; i < cd.size(); i++) {
			if (cd[i]->GetCDState() != CD_STAY) {
				continue;
			}
			else {
				m_AfterPos = cd[i]->GetPosition();
				m_AfterRot.y = Helper::GetInstance()->DirRotation(m_Position, cd[i]->GetPosition(), -PI_180);
				break;
			}
		}
		m_Frame = {};
		m_MoveInterVal = 0;
		_charaState = STATE_CHOICE;
	}
}
//動きの選択
void FourthBoss::Choice() {
	const float l_AddAngle = 5.0f;
	float l_AddFrame = 0.001f;
	const float l_FollowSpeed = 0.4f;
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 90);
	int l_SelectRand = 0;

	//二点間の距離計算
	m_Length = Helper::GetInstance()->ChechLength({ m_Position.x,0.0f,m_Position.z }, { m_AfterPos.x,0.0f,m_AfterPos.z });
	//次のCDを狙う
	if (m_Length > 0.5f) {
		Helper::GetInstance()->FollowMove(m_Position, m_AfterPos, l_FollowSpeed);
	}
	else {
		//行動を決めて次の行動に移る
		l_SelectRand = int(l_RandomMove(mt));
		m_StopTimer++;
		if (m_StopTimer > 50) {
			for (int i = 0; i < cd.size(); i++) {
				if (cd[i]->GetCDState() != CD_STAY) {
					_charaState = STATE_INTER;
					m_StopTimer = {};
					m_Frame = {};
					m_Angle = {};
				}
				if (cd[i]->GetCDState() != CD_STAY) {
					continue;
				}
				else {
					cd[i]->SetAttackSetCD(true);
					//攻撃をするかスルーか行動をするかCDを取るか決める
					if (l_SelectRand < 91) {
						_charaState = i + 2;
						cd[i]->SetCDState(CD_CATCH);
						m_EndCount++;
					}
					else{
						m_CatchCount++;
						m_EndCount++;
						cd[i]->SetCDState(CD_CATCH);
						if ((l_SelectRand >= 41 && l_SelectRand < 71) && m_EndCount < 4) {
							_charaState = STATE_INTER;
						}
						else {
							_charaState = STATE_THROW;
						}
					}
					m_StopTimer = 0;
					m_Frame = {};
					m_Angle = {};
					break;
				}
			}
		}
	}

	//sin波によって上下に動く
	m_Angle += l_AddAngle;
	m_Angle2 = m_Angle * (3.14f / 180.0f);
	m_Position.y = (sin(m_Angle2) * 2.0f + 2.0f);

	m_AfterRot.y = Helper::GetInstance()->DirRotation(m_Position, m_AfterPos, PI_90);

	m_Rotation.y = Ease(In, Cubic, 0.5f, m_Rotation.y, m_AfterRot.y);
}
//ダメージエリアのセット
void FourthBoss::LineSet() {
	int l_BirthNum = {};
	const int l_StartLimit = 120;

	//HPの削り具合によって出す線の数を変える
	if (isStrong) {
		l_BirthNum = 4;
	}
	else {
		l_BirthNum = 2;
	}

	m_CheckTimer++;

	//最初にSE鳴らす
	if (m_CheckTimer == 50) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Shot.wav", VolumManager::GetInstance()->GetSEVolum());
	}

	if (m_CheckTimer > l_StartLimit) {
		if (m_AreaState == AREA_SET) {
			damagearea.reset(new DamageArea(l_BirthNum));
			damagearea->Initialize();
			m_AreaState = AREA_STOP;
		}
		else if (m_AreaState == AREA_STOP) {
			cd[CD_LINE]->SetCDState(CD_DEATH);
			_charaState = STATE_INTER;
			m_CheckTimer = {};
		}
	}
}
//プレイヤーのデバフ
void FourthBoss::Debuff() {
	const int l_StartLimit = 150;
	m_CheckTimer++;
	if (m_CheckTimer == 50) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Shot.wav", VolumManager::GetInstance()->GetSEVolum());
	}
	else if (m_CheckTimer == 80) {
		noteeffect->SetAlive(true);
	}
	else if (m_CheckTimer == 110) {
		m_Check = true;
	}
	else if (m_CheckTimer == l_StartLimit) {
		cd[CD_DEBUFF]->SetCDState(CD_DEATH);
		if ((cd[CD_CONFU]->GetCDState() == CD_DEATH) && (cd[CD_BARRA]->GetCDState() == CD_DEATH) && m_CatchCount != 0) {
			_charaState = STATE_THROW;
		}
		else {
			_charaState = STATE_INTER;
		}
		m_CheckTimer = {};
	}
}
//プレイヤー混乱
void FourthBoss::Confu() {
	m_CheckTimer++;
	const int l_LimitConfu = 80;
	const int l_EndConfu = 120;
	int l_ConfuTimer = {};
	if (m_CheckTimer == 50) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Shot.wav", VolumManager::GetInstance()->GetSEVolum());
	}

	if (m_CheckTimer == l_LimitConfu) {
		confueffect->SetAlive(true);
		Player::GetInstance()->SetConfu(true);
		if (isStrong) {
			l_ConfuTimer = 500;
		}
		else {
			l_ConfuTimer = 200;
		}
		Player::GetInstance()->SetConfuTimer(l_ConfuTimer);
	}
	else if (m_CheckTimer == l_EndConfu) {
		cd[CD_CONFU]->SetCDState(CD_DEATH);
		if ((cd[CD_BARRA]->GetCDState() == CD_DEATH) && m_CatchCount != 0) {
			_charaState = STATE_THROW;
		}
		else {
			_charaState = STATE_INTER;
		}
		m_CheckTimer = {};
	}
}
//拡散(ふつう)
void FourthBoss::Barrage() {
	const int l_StartLimit = 100;
	m_CheckTimer++;
	if (m_CheckTimer == 50) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Shot.wav", VolumManager::GetInstance()->GetSEVolum());
	}
	else if (m_CheckTimer > l_StartLimit) {
		m_Rotation.y += 2.0f;
		m_RotTimer++;
		if (m_RotTimer % 5 == 0) {
			BirthNote("NORMAL");
		}

		//一定フレームで終了
		if (m_RotTimer == 600) {
			cd[CD_BARRA]->SetCDState(CD_DEATH);
			m_RotTimer = {};
			m_CheckTimer = {};
			m_CircleScale = 30.0f;
			m_CircleSpeed = {};
			if (m_CatchCount != 0) {
				_charaState = STATE_THROW;
			}
			else {
				_charaState = STATE_INTER;
			}
		}

		if (m_Rotation.y > 360.0f) {
			m_Rotation.y = 0.0f;
		}

		//強さによって回転が変わる
		if (isStrong) {
			m_CircleSpeed += 2.0f;
			m_CircleScale += 1.0f;
		}
		else {
			m_CircleSpeed += 1.0f;
			m_CircleScale += 0.2f;
		}
		m_AfterPos = Helper::GetInstance()->CircleMove({}, m_CircleScale, m_CircleSpeed);

		m_Position = {
			Ease(In,Cubic,0.2f,m_Position.x,m_AfterPos.x),
			m_Position.y,
			Ease(In,Cubic,0.2f,m_Position.z,m_AfterPos.z),
		};
	}
}
//投げる
void FourthBoss::Throw() {
	int l_LimitTimer = {};
	if (m_ThrowState == THROW_SET) {
		m_ThrowTimer++;
		if (m_ThrowTimer == 50) {
			m_ThrowState = THROW_NOW;
			m_ThrowTimer = {};
			m_CatchCount--;
		}
	}
	else if (m_ThrowState == THROW_NOW) {
		//上から順にCDを投げる
		for (int i = 0; i < cd.size(); i++) {
			if (cd[i]->GetCDState() != CD_CATCH) {
				continue;
			}
			else {
				cd[i]->SetCDState(CD_THROW);
				break;
			}
		}

		//CDを持っているか検索してなかった場合は次に移動
		for (int i = 0; i < cd.size(); i++) {
			if (cd[i]->GetCDState() == CD_CATCH) {
				m_ThrowState = THROW_SET;
				break;
			}
			else {
				m_ThrowState = THROW_END;
			}
		}
	}
	else {
		//投げ終わった後のインターバル
		m_ThrowTimer++;
		if (m_EndCount < 4) {
			l_LimitTimer = 50;
		}
		else {
			l_LimitTimer = 150;
		}


		if (m_ThrowTimer == l_LimitTimer) {
			m_ThrowTimer = 0;
			_charaState = STATE_INTER;
		}
	}
}
//行動の終わり(プレイヤーから逃げる)
void FourthBoss::EndMove() {
	const int l_EndLimit = 300;
	const float l_AddAngle = 5.0f;
	float l_AddSpeed = {};
	m_EndTimer++;

	m_Angle += l_AddAngle;
	m_Angle2 = m_Angle * (3.14f / 180.0f);
	m_CircleScale = (sin(m_Angle2) * 200.0f + 50.0f);
	l_AddSpeed = (sin(m_Angle2) * 1.0f + 2.0f);
	m_CircleSpeed += l_AddSpeed;
	
	//追従
	m_AfterPos = Helper::GetInstance()->CircleMove({}, m_CircleScale, m_CircleSpeed);

	m_Position = {
		Ease(In,Cubic,0.2f,m_Position.x,m_AfterPos.x),
		m_Position.y,
		Ease(In,Cubic,0.2f,m_Position.z,m_AfterPos.z),
	};
	if (m_EndTimer == l_EndLimit) {
		_charaState = STATE_INTER;
		m_EndTimer = {};
		m_Frame = {};
		m_Angle = {};
		m_CircleScale = 30.0f;
		m_CircleSpeed = {};
	}

}
//ノーツの生成
void FourthBoss::BirthNote(const std::string& BarrageName) {
	//ヒトツずつ生成されるパターン
	if (BarrageName == "NORMAL" || BarrageName == "RANDOM") {
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = {};
		if (BarrageName == "RANDOM") {		//ランダム
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_RandomRot(0, 360);
			matRot = XMMatrixRotationY(XMConvertToRadians(float(l_RandomRot(mt))));
		}
		else if (BarrageName == "NORMAL") {	//ボスの向き依存
			matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
		}
		move = XMVector3TransformNormal(move, matRot);
		XMFLOAT2 l_Angle;
		l_Angle.x = move.m128_f32[0];
		l_Angle.y = move.m128_f32[2];
		//ノーツの発生
		AttackNote* newnote;
		newnote = new AttackNote();
		newnote->Initialize();
		newnote->SetPosition(m_Position);
		newnote->SetAngle(l_Angle);
		attacknotes.push_back(newnote);
	}
	else if (BarrageName == "ALTER") {		//同時に弾を出す
		for (int i = 0; i < BULLET_NUM; i++) {
			XMVECTOR move2 = { 0.0f, 0.0f, 0.1f, 0.0f };

			XMMATRIX matRot2;
			if (i == 0) {
				matRot2 = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
			}
			else if (i == 1) {
				matRot2 = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 90.0f));
			}
			else if (i == 2) {
				matRot2 = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 180.0f));
			}
			else {
				matRot2 = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 270));
			}

			move2 = XMVector3TransformNormal(move2, matRot2);
			XMFLOAT2 l_Angle2;
			l_Angle2.x = move2.m128_f32[0];
			l_Angle2.y = move2.m128_f32[2];

			//ノーツの発生
			AttackNote* newnote;
			newnote = new AttackNote();
			newnote->Initialize();
			newnote->SetPosition(m_Position);
			newnote->SetAngle(l_Angle2);
			attacknotes.push_back(newnote);
		}
	}
}
//登場シーン
void FourthBoss::AppearAction() {
	Obj_SetParam();
}