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

	bossstuneffect.reset(new BossStunEffect());
	bossstuneffect->Initialize();
}
//������
bool SevenBoss::Initialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 2.2f;

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
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
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
	&SevenBoss::BirthAvatar,//�U���̃{�X
	&SevenBoss::BulletCatch,//�e���z��
	&SevenBoss::Stun,//�X�^��
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
	if (_charaState != STATE_CATCH) {
		CollideBul(_playerBulA, Type::CIRCLE);
	}
	//�v���C���[�̓����蔻��
	ColPlayer();
	//OBJ�̃X�e�[�^�X�̃Z�b�g
	Obj_SetParam();
	//���~�b�g����
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
	Helper::GetInstance()->Clamp(m_HP, -1.0f, m_MaxHp);

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

	//�U���̃{�X
	for (InterBoss* newboss : avatarboss) {
		if (newboss != nullptr) {
			newboss->Update();
		}
	}

	//�U���̃{�X�̍폜
	for (int i = 0; i < avatarboss.size(); i++) {
		if (avatarboss[i] == nullptr) {
			continue;
		}

		if (avatarboss[i]->GetHP() <= 0.0f) {
			avatarboss.erase(cbegin(avatarboss) + i);
			m_AvatarCount--;
		}
	}

	//�z���G�t�F�N�g
	for (AbsorptionEffect* neweffect : abseffect) {
		if (neweffect != nullptr) {
			neweffect->Update();
		}
	}

	//�z���G�t�F�N�g
	for (int i = 0; i < abseffect.size(); i++) {
		if (abseffect[i] == nullptr) {
			continue;
		}

		if (!abseffect[i]->GetAlive()) {
			abseffect.erase(cbegin(abseffect) + i);
		}
	}

	bossstuneffect->SetBasePos(m_Position);
	bossstuneffect->Update();
}
//�|�[�Y
void SevenBoss::Pause() {
}
//�G�t�F�N�g�`��
void SevenBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;
	bossstuneffect->Draw(dxCommon);

	//�z���G�t�F�N�g
	for (AbsorptionEffect* neweffect : abseffect) {
		if (neweffect != nullptr) {
			neweffect->Draw(dxCommon);
		}
	}
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

		//�U���̃{�X
		for (InterBoss* newboss : avatarboss) {
			if (newboss != nullptr) {
				newboss->Draw(dxCommon);
			}
		}

		EffecttexDraw(dxCommon);
	}
}
//ImGui
void SevenBoss::ImGui_Origin() {
	ImGui::Begin("Seven");
	ImGui::Text("HP:%f", m_HP);
	ImGui::Text("Absorption::%d", m_Absorption);
	ImGui::Text("MoveTimer::%d", m_MoveTimer);
	ImGui::Text("Rand::%d", int(_charaState));
	ImGui::End();
}
//�C���^�[�o��
void SevenBoss::InterValMove() {
	const int l_LimitTimer = 100;
	m_InterVal++;
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 2);
	if (m_InterVal == l_LimitTimer) {
		//�s�������߂Ď��̍s���Ɉڂ�
		m_AttackRand = int(l_RandomMove(mt));

		if (m_AttackRand == 0) {
			_charaState = STATE_BOUND;
			m_InterVal = {};
		}
		else if(m_AttackRand == 1) {
			_charaState = STATE_POLTER;
			m_InterVal = {};
		}
		else {
			if (m_AvatarCount == 0) {
				_charaState = STATE_AVATAR;
				m_InterVal = {};
			}
			else {
				m_InterVal = l_LimitTimer - 1;
			}
		}
	}
}
//�|���^�[�K�C�X�g
void SevenBoss::Polter() {
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	const int l_LimitTimer = 200;
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Normal");
		m_MoveTimer = {};
		m_AttackCount++;
		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 2) {
			_charaState = STATE_INTER;
		}
		else {
			_charaState = STATE_CATCH;
			m_Color = { 1.0f,0.0f,0.0f,1.0f };
		}
	}
}
//�o�E���h�e
void SevenBoss::ThrowBound() {
	const int l_LimitTimer = 200;
	m_Color = { 0.0f,1.0f,0.0f,1.0f };
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		BirthPolter("Bound");
		m_MoveTimer = {};
		m_AttackCount++;

		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 2) {
			_charaState = STATE_INTER;
		}
		else {
			_charaState = STATE_CATCH;
			m_Color = { 1.0f,0.0f,0.0f,1.0f };
		}
	}
}
//�U���̃{�X�𐶂�
void SevenBoss::BirthAvatar() {
	const int l_LimitTimer = 100;
	m_Color = { 0.0f,0.0f,1.0f,1.0f };
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		for (int i = 0; i < AVATAR_NUM; i++) {
			InterBoss* boss;
			boss = new AvatarBoss();
			boss->Initialize();
			if (i == 0) {
				boss->SetPosition({ 20.0f,3.0f,10.0f });
			}
			else {
				boss->SetPosition({ -20.0f,3.0f,10.0f });
			}
			avatarboss.push_back(boss);
			m_AvatarCount++;
		}
		m_AttackCount++;
		m_MoveTimer = {};
		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 2) {
			_charaState = STATE_INTER;
		}
		else {
			_charaState = STATE_CATCH;
		}
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
//�e���z��
void SevenBoss::BulletCatch() {
	const int l_LimitTimer = 500;
	m_Color = { 1.0f,1.0f,0.0f,1.0f };
	//�e�ƃ{�X�̓����蔻��
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CatchBul(_playerBulA);
	m_MoveTimer++;
	m_Absorption = true;
	if (m_MoveTimer == l_LimitTimer) {
		m_Color = { 0.0f,0.0f,1.0f,1.0f };
		m_MoveTimer = {};
		m_AttackCount = {};
		m_Absorption = false;
		_charaState = STATE_INTER;
	}
	if (m_MoveTimer % 6 == 0) {
		BirthParticle();
	}
}
//�X�^��������
void SevenBoss::Stun() {
	const int l_LimitTimer = 500;
	m_MoveTimer++;
	m_Absorption = false;
	m_Color = { 0.0f,1.0f,1.0f,1.0f };
	if (m_MoveTimer == l_LimitTimer) {
		m_Stun = false;
		m_MoveTimer = {};
		m_InterVal = {};
		_charaState = STATE_INTER;
		bossstuneffect->SetAlive(false);
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
//�e�̋z������
void SevenBoss::CatchBul(vector<InterBullet*> bullet)
{
	const float l_CatchRadius = 10.0f;
	const float l_StunRadius = 1.0f;
	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr && _bullet->GetAlive()) {
			_bullet->SetTargetPos(m_Position);
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_CatchRadius, m_Position.x, m_Position.z, l_CatchRadius)) {
				_bullet->SetBossCatch(true);
			}

			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_StunRadius, m_Position.x, m_Position.z, l_StunRadius) && (!m_Stun)) {
				BirthExplosion();
				bossstuneffect->SetAlive(true);
				m_Stun = true;
				m_AttackCount = {};
				_bullet->SetAlive(false);
				m_Absorption = false;
				_charaState = STATE_STUN;
				m_MoveTimer = {};
			}
		}
	}
}
//�����G�t�F�N�g
void SevenBoss::BirthExplosion() {
	float l_AddSize = 2.5f;
	const float RandScale = 3.0f;
	float s_scale = 0.3f * l_AddSize;
	float e_scale = (4.0f + (float)rand() / RAND_MAX * RandScale - RandScale / 2.0f) * l_AddSize;

	//�F
	const float RandRed = 0.2f;
	const float red = 0.2f + (float)rand() / RAND_MAX * RandRed;
	const XMFLOAT4 s_color = { 0.9f, red, 0.1f, 1.0f }; //�Z����
	const XMFLOAT4 e_color = { 0, 0, 0, 1.0f }; //���F

	//�����w��
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Randlife(10, 40);
	int l_Life = int(l_Randlife(mt));

	ParticleEmitter::GetInstance()->Explosion(l_Life, m_Position, l_AddSize, s_scale, e_scale, s_color, e_color);
}
//�p�[�e�B�N��
void SevenBoss::BirthParticle() {
	//�m�[�c�̔���
	AbsorptionEffect* neweffect;
	neweffect = new AbsorptionEffect();
	neweffect->Initialize();
	neweffect->SetBasePos(m_Position);
	abseffect.push_back(neweffect);
}