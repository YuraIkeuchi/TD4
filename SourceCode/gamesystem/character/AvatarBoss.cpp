#include "AvatarBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Player.h"
#include "Easing.h"
//����
AvatarBoss::AvatarBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DJ);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool AvatarBoss::Initialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.1f,0.1f,0.1f };
	m_Color = { 1.0f,1.0f,1.0f,0.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 2.2f;

	_charaState = STATE_INTER;
	//CSV���[�h
	CSVLoad();
	return true;
}
//�X�L�b�v���̏�����
void AvatarBoss::SkipInitialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.1f,0.1f,0.1f };
	m_Color = { 0.0f,0.0f,1.0f,0.0f };
}
//CSV
void AvatarBoss::CSVLoad() {

	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Avatar/Avatarboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Avatar/Avatarboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Avatar/Avatarboss.csv", "HeartTarget")));

	m_MaxHp = m_HP;
}

void (AvatarBoss::* AvatarBoss::stateTable[])() = {
	&AvatarBoss::InterValMove,//�����̍���
	&AvatarBoss::Polter,//�|���^�[�K�C�X�g
	&AvatarBoss::ThrowBound,//������
};
//�s��
void AvatarBoss::Action() {
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

	m_Color.w = Ease(In, Cubic, 0.2f, m_Color.w, 1.0f);
}
//�|�[�Y
void AvatarBoss::Pause() {

}
//�G�t�F�N�g�`��
void AvatarBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;

}
//�`��
void AvatarBoss::Draw(DirectXCommon* dxCommon) {
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
void AvatarBoss::ImGui_Origin() {
	ImGui::Begin("Avatar");
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
void AvatarBoss::InterValMove() {
	int l_LimitTimer = 100;
	m_InterVal++;
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 1);
	if (m_InterVal == l_LimitTimer) {
		//�s�������߂Ď��̍s���Ɉڂ�
		m_AttackRand = int(l_RandomMove(mt));

		if (m_AttackRand == 0) {
			_charaState = STATE_BOUND;
			m_InterVal = {};
		}
		else{
			_charaState = STATE_POLTER;
			m_InterVal = {};
		}
	}
}
//�|���^�[�K�C�X�g
void AvatarBoss::Polter() {
	const int l_LimitTimer = 300;
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Normal");
		m_MoveTimer = {};
		_charaState = STATE_INTER;
	}
}
//�o�E���h�e
void AvatarBoss::ThrowBound() {
	const int l_LimitTimer = 300;
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Bound");
		m_MoveTimer = {};
		_charaState = STATE_INTER;
	}
}
//�|���^�[�K�C�X�g�̐���
void AvatarBoss::BirthPolter(const std::string& PolterName) {
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
			poltergeist.push_back(newpolter);
		}
	}
	else if (PolterName == "Bound") {
		for (int i = 0; i < POLTER_NUM; i++) {
			//�m�[�c�̔���
			Poltergeist* newpolter;
			newpolter = new Poltergeist();
			newpolter->Initialize();

			newpolter->SetPolterType(TYPE_BOUND);
			newpolter->SetTargetTimer(i * l_LimitTimer2);
			newpolter->SetBasePos(m_Position);
			newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z });
			newpolter->SetCircleSpeed(i * 180.0f);
			poltergeist.push_back(newpolter);
		}
	}
}
//�o��V�[��
void AvatarBoss::AppearAction() {
	Obj_SetParam();
}
//�{�X���j�V�[��
void AvatarBoss::DeadAction() {

	Obj_SetParam();
}
//�{�X���j�V�[��(�X���[)
void AvatarBoss::DeadAction_Throw() {
	Obj_SetParam();
}