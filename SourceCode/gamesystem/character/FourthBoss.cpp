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
}

bool FourthBoss::Initialize() {
	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 5.0f;

	_charaState = STATE_INTER;
	m_BarraState = BARRA_SET;
	m_AreaState = AREA_SET;
	//CSVロード
	CSVLoad();
	return true;
}

void FourthBoss::SkipInitialize() {
	m_Position = { 0.0f,20.0f,30.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
}

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
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
		
	//CDの更新
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->Update();
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
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("ROTY:%f", m_Rotation.y);
	ImGui::Text("STATE:%d", (int)_charaState);
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::End();

	//CDの更新
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->ImGuiDraw();
	}
	//攻撃の音符
	for (AttackNote* newnote : attacknotes) {
		if (newnote != nullptr) {
			newnote->ImGuiDraw();
		}
	}
	if (damagearea != nullptr) {
		damagearea->ImGuiDraw();
	}
}
//インターバル
void FourthBoss::InterValMove() {
	m_MoveInterVal++;

	if (m_MoveInterVal == 200) {

		for (size_t i = 0; i < cd.size(); i++) {
			if (cd[i]->GetCDState() != CD_STAY) {
				continue;
			}
			else {
				m_AfterPos = cd[i]->GetPosition();
				break;
			}	
		}

		m_MoveInterVal = 0;
		_charaState = STATE_CHOICE;
	}
}
//動きの選択
void FourthBoss::Choice() {
	float l_AddFrame = 0.01f;
	m_StopTimer++;	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 2);
	int l_RandState = 0;
	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		m_Frame = 1.0f;
	}

	m_Position = {
Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
	Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
	};

	//if (m_StopTimer > m_ChoiceInterval) {
	//	m_Frame = 0.0f;
	//	l_RandState = int(l_RandomMove(mt));
	//	m_StopTimer = 0;
	//	m_BarraState = BARRA_SET;
	//	m_AreaState = AREA_SET;
	//	_charaState = STATE_AREA;
	//	//ランダムで攻撃を選ぶ
	///*	if (l_RandState == 0) {
	//		_charaState = STATE_NORMAL;
	//	}
	//	else if (l_RandState == 1) {
	//		_charaState = STATE_ALTER;
	//	}
	//	else {
	//		_charaState = STATE_RANDOM;
	//	}*/
	//}
}
//ダメージエリアのセット
void FourthBoss::LineSet() {
	if (m_AreaState == AREA_SET) {
		damagearea.reset(new DamageArea(4));
		damagearea->Initialize();
		m_AreaState = AREA_STOP;
	}
	else if (m_AreaState == AREA_STOP) {

	}
	else {

	}
}
//プレイヤーのデバフ
void FourthBoss::Debuff() {
}
//プレイヤー混乱
void FourthBoss::Confu() {
}
//拡散(ふつう)
void FourthBoss::Barrage() {
	float l_AddFrame = 0.01f;

	if (m_BarraState == BARRA_SET) {
		m_AfterPos = { 40.0f,0.0f,40.0f };
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = {};
			m_BarraState = BARRA_BIRTH;
		}

		m_Position = {
	Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
	Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
		Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
	}
	else if (m_BarraState == BARRA_BIRTH) {
		m_Rotation.y += 2.0f;
		m_RotTimer++;
		if (m_RotTimer % 5 == 0) {
			BirthNote("NORMAL");
		}

		if (m_RotTimer == 600) {
			m_RotTimer = 0;
			m_BarraState = BARRA_END;
		}

		if (m_Rotation.y > 360.0f) {
			m_Rotation.y = 0.0f;
		}
	}
	else {
		m_AfterPos = { 0.0f,20.0f,30.0f };
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = {};
			_charaState = STATE_CHOICE;
		}

		m_Position = {
	Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
	Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
		Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
	}

}
//行動の終わり
void FourthBoss::EndMove() {

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