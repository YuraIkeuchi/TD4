#include "InterCD.h"
#include "Collision.h"
#include "Player.h"
#include "CsvLoader.h"
#include <random>
void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//生成
	&InterCD::StayCD, //放置
	&InterCD::ThroughCD,//スルー
	&InterCD::CatchCD,//所持
	&InterCD::ThrowCD,//投げる
	&InterCD::DeathCD,//なくなった
	&InterCD::ResPornCD,//りすぽーん
};

//CDV読み込み
void InterCD::CsvLoad() {
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/CD.csv", "hp")));
}

void InterCD::Update() {
	Action();

	//エフェクト
	for (InterEffect* effect : effects) {
		if (effect != nullptr) {
			effect->Update();
		}
	}

	//マークの削除
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i] == nullptr) {
			continue;
		}

		if (!effects[i]->GetAlive()) {
			effects.erase(cbegin(effects) += i);
		}
	}
}
void InterCD::Draw(DirectXCommon* dxCommon) {
	if(m_CDState != CD_DEATH && m_CDState != CD_RESPORN)
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
				BirthEffect();
				_bullet->SetAlive(false);
				m_HP -= 1.0f;
				if (m_HP <= 0.0f) {
					m_CDState = CD_DEATH;
					m_BreakCD = true;
				}
			}
		}
	}	
}

//プレイヤーとCDの当たり判定
bool InterCD::PlayerCollide() {
	if (m_CDState != CD_THROW)return false;
	const float l_Radius = 1.0f;
	if (Collision::CircleCollision(Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, l_Radius, m_Position.x, m_Position.z, l_Radius)
		&& Player::GetInstance()->GetDamageInterVal() == 0) {
		Player::GetInstance()->PlayerHit(m_Position);
		Player::GetInstance()->RecvDamage(0.5f);
		return true;
	}
	else {
		return false;
	}

	return false;
}

void InterCD::SetCD() {
	if (m_AttackSetCD) {
		if (m_CatchState == CATCH_SET) {
			m_AddPower = 0.5f;
			m_CatchState = CATCH_MOVE;
		}
		else if (m_CatchState == CATCH_MOVE) {
			m_AddPower -= m_Gravity;
			if (Helper::GetInstance()->CheckMax(m_Position.y, m_CatchPos.y, m_AddPower) && m_AddPower < -1.0f) {
				m_CatchState = CATCH_END;
				m_AttackSetCD = false;
			}
		}
	}
}

//エフェクトの発生
void InterCD::BirthEffect() {
	InterEffect* neweffect;
	neweffect = new BreakEffect();
	neweffect->Initialize();
	neweffect->SetPosition(m_Position);
	neweffect->SetDiviSpeed(1.0f);
	neweffect->SetLife(50);
	effects.push_back(neweffect);
}

//死んだときの動き
void InterCD::DeathMove(const int Timer, const int TargetTimer) {
	m_CDState = CD_STAY;
	const int l_Division = 50;
	if (!m_DeathMove) {
		if (Timer < TargetTimer) {
			m_Position = m_CatchPos;
		}
		else{
			//乱数生成(加算力と大きさ)
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_bounddist(-80, 80);
			uniform_int_distribution<int> l_bounddist2(30, 60);
			m_BoundPower = {
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist2(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
			};
			m_DeathMove = true;
		}
	}
	else {
		//飛ぶような感じにするため重力を入れる
		m_BoundPower.y -= m_Gravity;
		Helper::GetInstance()->Float3AddFloat3(m_Position, m_BoundPower);
		if (Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_BoundPower.y)) {
			m_BoundPower = {};
		}
	}

	Obj_SetParam();
}