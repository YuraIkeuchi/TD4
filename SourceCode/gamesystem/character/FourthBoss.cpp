#include "FourthBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Player.h"
#include "VariableCommon.h"

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
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 5.0f;

	note->Initialize();

	_charaState = STATE_CHOICE;
	m_FollowState = FOLLOW_SET;
	m_DiffuState = DIFFU_SET;
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

	note->Update();

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
	//攻撃の音符
	for (AttackNote* newnote : attacknotes) {
		if (newnote != nullptr) {
			newnote->Draw(dxCommon);
		}
	}
	EffecttexDraw(dxCommon);
}

//ImGui
void FourthBoss::ImGui_Origin() {
	ImGui::Begin("Fourth");
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("Follor:%d", m_DiffuState);
	ImGui::Text("ROTY:%f", m_Rotation.y);
	if (ImGui::Button("RESET")) {
		_charaState = STATE_CHOICE;
	}
	ImGui::End();

	//攻撃の音符
	for (AttackNote* newnote : attacknotes) {
		if (newnote != nullptr) {
			newnote->ImGuiDraw();
		}
	}
}
//動きの選択
void FourthBoss::Choice() {
	m_StopTimer++;	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 2);
	int l_RandState = 0;
	if (m_StopTimer > m_ChoiceInterval) {
		m_Frame = 0.0f;
		l_RandState = int(l_RandomMove(mt));
		m_StopTimer = 0;
		_charaState = STATE_DIFF;
		m_FollowState = FOLLOW_SET;
		m_DiffuState = DIFFU_SET;
		////ランダムで攻撃を選ぶ
		//if (l_RandState == 0) {
		//	
		//}
		//else if (l_RandState == 1) {
		//	_charaState = STATE_DIFF;
		//}
		//else {
		//	_charaState = STATE_CONFU;
		//}
	}
}

//追従
void FourthBoss::Follow() {
	float l_AddFrame = 0.01f;
	m_Scale = { 0.5f,0.5f,0.5f };

	if (m_FollowState == FOLLOW_SET) {
		m_AfterPos = { 40.0f,0.0f,40.0f };
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = {};
			m_FollowState = FOLLOW_BIRTH;
		}

		m_Position = {
	Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
	Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
		Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
	}else if(m_FollowState == FOLLOW_BIRTH){
		m_AfterRot.y = 720.0f;
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = {};
			m_Rotation.y = 0.0f;
			if (m_RotCount != 3) {
				BirthNote(SET_FOLLOW);
				m_RotCount++;
			}
			else {
				m_FollowState = FOLLOW_END;
			}
		}

		m_Rotation.y = Ease(In, Cubic, m_Frame, m_Rotation.y, m_AfterRot.y);
	}
	else {
		m_AfterPos = { 0.0f,0.0f,30.0f };
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
//拡散
void FourthBoss::Diffusion() {
	float l_AddFrame = 0.01f;
	m_Scale = { 0.5f,0.5f,0.5f };

	if (m_DiffuState == DIFFU_SET) {
		m_AfterPos = { -40.0f,0.0f,40.0f };
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = {};
			m_DiffuState = DIFFU_BIRTH;
		}

		m_Position = {
	Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
	Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
		Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
		};
	}
	else if (m_DiffuState == DIFFU_BIRTH) {
		m_Rotation.y += 2.0f;
		m_RotTimer++;
		if (m_RotTimer % 5 == 0) {
			BirthNote(SET_DIFF);
		}

		if (m_RotTimer == 600) {
			m_RotTimer = 0;
			m_DiffuState = DIFFU_END;
		}

		if (m_Rotation.y > 360.0f) {
			m_Rotation.y = 0.0f;
		}
	}
	else {
		m_AfterPos = { 0.0f,0.0f,30.0f };
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

//混乱
void FourthBoss::Confusion() {
	float l_AddFrame = 0.01f;
	m_AfterPos = { 0.0f,0.0f,-40.0f };
	m_Scale = { 0.5f,0.5f,0.5f };

	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		m_Frame = m_FrameMax;
	}

	m_Position = {
Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
	Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
	};
}

//ノーツの生成
void FourthBoss::BirthNote(const int NoteType) {
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	XMFLOAT2 l_Angle;
	l_Angle.x = move.m128_f32[0];
	l_Angle.y = move.m128_f32[2];
	//衝撃波の発生
	AttackNote* newnote;
	newnote = new AttackNote();
	newnote->Initialize();
	newnote->SetPosition(m_Position);
	newnote->SetType(NoteType);
	newnote->SetAngle(l_Angle);
	attacknotes.push_back(newnote);
}
//登場シーン
void FourthBoss::AppearAction() {
	Obj_SetParam();
}