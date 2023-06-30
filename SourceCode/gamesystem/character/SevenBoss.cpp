#include "SevenBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Player.h"
//����
SevenBoss::SevenBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DJ);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool SevenBoss::Initialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 0.0f,0.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 5.2f;

	_charaState = STATE_INTER;
	//CSV���[�h
	CSVLoad();
	return true;
}
//�X�L�b�v���̏�����
void SevenBoss::SkipInitialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.1f,0.1f,0.1f };
	m_Color = { 0.0f,0.0f,1.0f,1.0f };
}
//CSV
void SevenBoss::CSVLoad() {
	
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "HeartTarget")));

	m_MaxHp = m_HP;
}

void (SevenBoss::* SevenBoss::stateTable[])() = {
	&SevenBoss::InterValMove,//�����̍���
	&SevenBoss::Polter,//�|���^�[�K�C�X�g
	&SevenBoss::ThrowBound,//������
};
//�s��
void SevenBoss::Action() {
	//��Ԉڍs(charastate�ɍ��킹��)
	if (m_HP > 0.0f) {
		(this->*stateTable[_charaState])();
	}
	/*^^^^�����蔻��^^^^*/
	//�e�ƃ{�X�̓����蔻��
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//�v���C���[�̓����蔻��
	ColPlayer();
	//OBJ�̃X�e�[�^�X�̃Z�b�g
	Obj_SetParam();
	//���~�b�g����
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);

	//��Q��
	for (Poltergeist* newpolter : poltergeist) {
		if (newpolter != nullptr) {
			newpolter->Update();
		}
	}

	//��Q���̍폜
	for (int i = 0; i < poltergeist.size(); i++) {
		if (poltergeist[i] == nullptr) {
			continue;
		}

		if (!poltergeist[i]->GetAlive()) {
			poltergeist.erase(cbegin(poltergeist) + i);
		}
	}
}
//�|�[�Y
void SevenBoss::Pause() {

}
//�G�t�F�N�g�`��
void SevenBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;

}
//�`��
void SevenBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	if (m_HP > 0.0f) {
		//��Q��
		for (Poltergeist* newpolter : poltergeist) {
			if (newpolter != nullptr) {
				newpolter->Draw(dxCommon);
			}
		}
	}
}
//ImGui
void SevenBoss::ImGui_Origin() {
	ImGui::Begin("Seven");
	ImGui::Text("Inter:%d", m_InterVal);
	ImGui::Text("Move:%d", m_MoveTimer);
	ImGui::End();

	for (Poltergeist* newpolter : poltergeist) {
		if (newpolter != nullptr) {
			newpolter->ImGuiDraw();
		}
	}
}
//�C���^�[�o��
void SevenBoss::InterValMove() {
	int l_LimitTimer = 100;
	m_InterVal++;

	if (m_InterVal == l_LimitTimer) {
		_charaState = STATE_BOUND;
		m_InterVal = 0;
	}
}
//�|���^�[�K�C�X�g
void SevenBoss::Polter() {
	const int l_LimitTimer = 300;
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Normal");
		m_MoveTimer = {};
		_charaState = STATE_INTER;
	}
}
//�o�E���h�e
void SevenBoss::ThrowBound() {
	const int l_LimitTimer = 300;
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Bound");
		m_MoveTimer = {};
		_charaState = STATE_INTER;
	}
}
//�|���^�[�K�C�X�g�̐���
void SevenBoss::BirthPolter(const std::string& PolterName) {
	const int l_LimitTimer = 20;//��Q���������܂ł̎���
	const int l_LimitTimer2 = 50;//��Q���������܂ł̎���2
	if (PolterName == "Normal") {
		for (int i = 0; i < POLTER_NUM; i++) {
			//�m�[�c�̔���
			Poltergeist* newpolter;
			newpolter = new Poltergeist();
			newpolter->Initialize();
			
			newpolter->SetPolterType(TYPE_FOLLOW);
			newpolter->SetTargetTimer(i * l_LimitTimer);
			if (i == 0) {
				newpolter->SetPosition({ m_Position.x + 3.0f,m_Position.y - 10.0f,m_Position.z });
			}
			else if (i == 1) {
				newpolter->SetPosition({ m_Position.x - 3.0f,m_Position.y - 10.0f,m_Position.z });
			}
			else if (i == 2) {
				newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z + 3.0f});
			}
			else {
				newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z - 3.0f });
			}
			poltergeist.push_back(newpolter);
		}
	}else if(PolterName == "Bound") {
		for (int i = 0; i < POLTER_NUM; i++) {
			//�m�[�c�̔���
			Poltergeist* newpolter;
			newpolter = new Poltergeist();
			newpolter->Initialize();

			newpolter->SetPolterType(TYPE_BOUND);
			newpolter->SetTargetTimer(i * l_LimitTimer2);
			newpolter->SetBasePos(m_Position);
			newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z });
			newpolter->SetCircleSpeed(i * 90.0f);
			poltergeist.push_back(newpolter);
		}
	}
}
//�o��V�[��
void SevenBoss::AppearAction() {
	Obj_SetParam();
}
//�{�X���j�V�[��
void SevenBoss::DeadAction() {

	Obj_SetParam();
}
//�{�X���j�V�[��(�X���[)
void SevenBoss::DeadAction_Throw() {
	Obj_SetParam();
}