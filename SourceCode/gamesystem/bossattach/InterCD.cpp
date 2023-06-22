#include "InterCD.h"
#include "Collision.h"
#include "Player.h"
#include "CsvLoader.h"
#include <random>
void (InterCD::* InterCD::stateTable[])() = {
	&InterCD::BirthCD,//����
	&InterCD::StayCD, //���u
	&InterCD::CatchCD,//����
	&InterCD::DeathCD,//�Ȃ��Ȃ���
	&InterCD::ResPornCD,//�肷�ہ[��
};

//CDV�ǂݍ���
void InterCD::CsvLoad() {
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/CD.csv", "hp")));
}

void InterCD::Update() {
	Action();

	//�G�t�F�N�g
	for (InterEffect* effect : effects) {
		if (effect != nullptr) {
			effect->Update();
		}
	}

	//�}�[�N�̍폜
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
	//ImGui::Begin("CD");
	//ImGui::Text("Count:%d", m_BoundCount);
	//ImGui::End();
	ImGui_Origin();
}

//�e�Ƃ̓����蔻��
void InterCD::CollideBul(vector<InterBullet*> bullet)
{
	if (m_CDState != CD_STAY)return;
	const float l_Radius = 1.0f;
	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr && _bullet->GetAlive()) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_Radius, m_Position.x, m_Position.z, l_Radius)) {
				BirthEffect();
				_bullet->SetAlive(false);
				m_HP -= _bullet->GetPower();
				if (m_HP <= 0.0f) {
					m_CDState = CD_DEATH;
					m_BreakCD = true;
				}
			}
		}
	}	
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

//�G�t�F�N�g�̔���
void InterCD::BirthEffect() {
	InterEffect* neweffect;
	neweffect = new BreakEffect();
	neweffect->Initialize();
	neweffect->SetPosition(m_Position);
	neweffect->SetDiviSpeed(1.0f);
	neweffect->SetLife(50);
	effects.push_back(neweffect);
}

//���񂾂Ƃ��̓���
void InterCD::DeathMove(const int Timer, const int TargetTimer) {
	m_CDState = CD_STAY;
	const int l_Division = 50;
	//
	if (!m_DeathMove) {
		m_DeathTimer++;
		if (m_DeathTimer == 1) {
			m_AudioPlay = true;
		}
		if (Timer < TargetTimer) {
			m_Position = m_CatchPos;
		}
		else{
			//��������(���Z�͂Ƒ傫��)
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_bounddist(-80, 80);
			uniform_int_distribution<int> l_bounddist2(30, 60);
			m_BoundPower = {
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist2(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
			};
			m_DeathMove = true;
			_AudioState = AUDIO_END;
			m_DeathTimer = {};
		}
	}
	else {
		//��Ԃ悤�Ȋ����ɂ��邽�ߏd�͂�����
		m_BoundPower.y -= m_Gravity;
		Helper::GetInstance()->Float3AddFloat3(m_Position, m_BoundPower);
		if (Helper::GetInstance()->CheckMax(m_Position.y, 0.0f, m_BoundPower.y)) {
			m_BoundPower = {};
		}
	}

	AudioAction();
	Obj_SetParam();
}