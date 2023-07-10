#include "SevenBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Player.h"
#include "Easing.h"
#include "HungerGauge.h"
//����
SevenBoss::SevenBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Ghost);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	bossstuneffect.reset(new BossStunEffect());
	bossstuneffect->Initialize();

	confueffect.reset(new ConfuEffect());
	confueffect->Initialize();
}
//������
bool SevenBoss::Initialize() {
	m_Position = { 0.0f,5.0f,30.0f };
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Scale = { 1.5f,1.5f,1.5f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	ActionTimer = 1;

	m_Radius = 2.2f;
	m_AfterAlpha = 1.0f;
	_charaState = STATE_INTER;
	_ReturnState = RETURN_SET;
	m_CircleSpeed = 90.0f;
	//CSV���[�h
	CSVLoad();
	return true;
}
//�X�L�b�v���̏�����
void SevenBoss::SkipInitialize() {
	m_Position = { 0.0f,5.0f,30.0f };
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Scale = { 1.5f,1.5f,1.5f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
}
//CSV
void SevenBoss::CSVLoad() {
	auto ActSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "ACT_NUM")));
	m_RandAct.resize(ActSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/boss/Seven/Sevenboss.csv", m_RandAct, "RandAct");

	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "HeartTarget")));
	m_VanishTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Seven/Sevenboss.csv", "VanishTarget")));

	m_MaxHp = m_HP;

	m_CircleScale = 30.0f;
}

void (SevenBoss::* SevenBoss::stateTable[])() = {
	&SevenBoss::InterValMove,//�����̍���
	&SevenBoss::Polter,//�|���^�[�K�C�X�g
	&SevenBoss::ThrowBound,//������
	&SevenBoss::BirthAvatar,//�U���̃{�X
	&SevenBoss::Manipulate,//�S�[�X�g�𑀂�
	&SevenBoss::FireAttack,//�����o��
	&SevenBoss::Confu,//����
	&SevenBoss::BlockAttack,//�_���[�W�̍U��
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
		if (m_Color.w > 0.9f) {
			CollideBul(_playerBulA, Type::CIRCLE);
		}
		if (_charaState != STATE_STUN) {
			VanishCollide(_playerBulA);
		}
	}

	//�v���C���[�̓����蔻��
	if (m_Color.w > 0.9f) {
		ColPlayer();
	}
	//OBJ�̃X�e�[�^�X�̃Z�b�g
	Obj_SetParam();
	//�{�X�̏����锻��
	VanishBoss();
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
			newboss->SetTargetPos(m_Position);
			newboss->Update();
		}
	}

	//�U���̃{�X�̍폜
	for (int i = 0; i < avatarboss.size(); i++) {
		if (avatarboss[i] == nullptr) {
			continue;
		}

		if (avatarboss[i]->GetHP() < 1.0f) {
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

	//�΂̋�
	for (FireBoll* newfire : fireboll) {
		if (newfire != nullptr) {
			newfire->Update();
		}
	}

	//�΂̋ʂ̍폜
	for (int i = 0; i < fireboll.size(); i++) {
		if (fireboll[i] == nullptr) {
			continue;
		}

		if (!fireboll[i]->GetAlive()) {
			fireboll.erase(cbegin(fireboll) + i);
		}
	}

	//�_���[�W�u���b�N
	for (DamageBlock* newblock : damageblock) {
		if (newblock != nullptr) {
			newblock->Update();
		}
	}
	//�_���[�W�u���b�N�̍폜
	for (int i = 0; i < damageblock.size(); i++) {
		if (damageblock[i] == nullptr) {
			continue;
		}

		if (!damageblock[i]->GetAlive()) {
			damageblock.erase(cbegin(damageblock) + i);
		}
	}

	bossstuneffect->SetBasePos(m_Position);
	bossstuneffect->Update();

	confueffect->SetBasePos(Player::GetInstance()->GetPosition());
	confueffect->Update();

	//HP�������؂����狭��
	if (m_HP < m_MaxHp / 2) {
		isStrong = true;
	}
}
//�|�[�Y
void SevenBoss::Pause() {
}
//�G�t�F�N�g�`��
void SevenBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;
	confueffect->Draw(dxCommon);
	if (_charaState == STATE_STUN) {
		bossstuneffect->Draw(dxCommon);
	}

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
		//�΂̋�
		for (FireBoll* newfire : fireboll) {
			if (newfire != nullptr) {
				newfire->Draw(dxCommon);
			}
		}
		//�_���[�W�u���b�N
		for (DamageBlock* newblock : damageblock) {
			if (newblock != nullptr) {
				newblock->Draw(dxCommon);
			}
		}
		EffecttexDraw(dxCommon);
	}
}
//ImGui
void SevenBoss::ImGui_Origin() {
	ImGui::Begin("Seven");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::End();

	////�_���[�W�u���b�N
	//for (DamageBlock* newblock : damageblock) {
	//	if (newblock != nullptr) {
	//		newblock->ImGuiDraw();
	//	}
	//}
}
//�C���^�[�o��
void SevenBoss::InterValMove() {
	const int l_LimitTimer = 500;
	m_InterVal++;

	
	if (!m_Return) {
		RandMove();//���t���[���œ�����
	}
	else {
		ReturnBoss();//�{�X����ʒu�ɖ߂�
	}
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 100);
	if (m_InterVal == l_LimitTimer) {
		//�s�������߂Ď��̍s���Ɉڂ�
		m_AttackRand = int(l_RandomMove(mt));
		if (m_AttackRand < m_RandAct[RAND_POLTER]) {
			_charaState = STATE_POLTER;
			m_InterVal = {};
		}
		else if (m_AttackRand >= m_RandAct[RAND_POLTER] && m_AttackRand < m_RandAct[RAND_BOUND]) {
			_charaState = STATE_BOUND;
			m_InterVal = {};
		}
		else if (m_AttackRand >= m_RandAct[RAND_BOUND] && m_AttackRand < m_RandAct[RAND_AVATAR]) {
			if (m_AvatarCount == 0) {
				_charaState = STATE_AVATAR;
				m_InterVal = {};
			}
			else {
				m_InterVal = l_LimitTimer - 1;
			}
		}
		else if(m_AttackRand >= m_RandAct[RAND_AVATAR] && m_AttackRand < m_RandAct[RAND_MANIPULATE]) {
			if (HungerGauge::GetInstance()->GetCatchCount() != 0) {
				_charaState = STATE_MANIPULATE;
				m_InterVal = {};
				m_StartMani = true;
			}
			else {
				m_InterVal = l_LimitTimer - 1;
			}
		}
		else if (m_AttackRand >= m_RandAct[RAND_MANIPULATE] && m_AttackRand < m_RandAct[RAND_FIRE]) {
			_charaState = STATE_FIRE;
			m_InterVal = {};
		}
		else if (m_AttackRand >= m_RandAct[RAND_FIRE] && m_AttackRand < m_RandAct[RAND_CONFU]) {
			_charaState = STATE_CONFU;
			m_InterVal = {};
		}
		else {
			_charaState = STATE_BLOCK;
			m_InterVal = {};
		}
		m_ChangeTimer = {};
	}
}
//�|���^�[�K�C�X�g
void SevenBoss::Polter() {
	const int l_LimitTimer = 200;
	m_MoveTimer++;
	if (m_MoveTimer == 1) {
		BirthPolter("Normal");
	}
	if (m_MoveTimer == l_LimitTimer) {
		m_MoveTimer = {};
		m_AttackCount++;
		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 4) {
			_charaState = STATE_INTER;
			m_Return = true;
		}
		else {
			_charaState = STATE_CATCH;
		}
	}
}
//�o�E���h�e
void SevenBoss::ThrowBound() {
	const int l_LimitTimer = 200;
	m_MoveTimer++;
	if (m_MoveTimer == 1) {
		BirthPolter("Bound");
	}
	if (m_MoveTimer == l_LimitTimer) {
		m_MoveTimer = {};
		m_AttackCount++;

		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 4) {
			_charaState = STATE_INTER;
			m_Return = true;
		}
		else {
			_charaState = STATE_CATCH;
		}
	}
}
//�U���̃{�X�𐶂�
void SevenBoss::BirthAvatar() {
	const int l_LimitTimer = 100;
	m_MoveTimer++;
	if (m_MoveTimer == l_LimitTimer) {
		for (int i = 0; i < AVATAR_NUM; i++) {
			InterBoss* boss;
			boss = new AvatarBoss();
			boss->Initialize();
			boss->SetPosition(m_Position);
			if (i == 0) {
				boss->SetCircleSpeed(0.0f);
			}
			else {
				boss->SetCircleSpeed(180.0f);
			}
			avatarboss.push_back(boss);
			m_AvatarCount++;
		}
		m_AttackCount++;
		m_MoveTimer = {};
		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 4) {
			_charaState = STATE_INTER;
			m_Return = true;
		}
		else {
			_charaState = STATE_CATCH;
		}
	}
}
//�߂܂��Ă���S�[�X�g�𑀂�
void SevenBoss::Manipulate() {
	const int l_LimitTimer = 200;
	if (m_StartMani) {
		m_MoveTimer++;
		if (m_MoveTimer == 50) {
			m_Manipulate = true;
		}
		else if (m_MoveTimer == 52) {
			m_Manipulate = false;
		}

		if (m_MoveTimer == l_LimitTimer) {
			m_MoveTimer = {};
			m_Manipulate = false;
			m_StartMani = false;
			m_AttackCount++;

			//���U��������z���s���Ɉڍs����
			if (m_AttackCount != 4) {
				_charaState = STATE_INTER;
				m_Return = true;
			}
			else {
				_charaState = STATE_CATCH;
			}
		}
	}
}
//�΂̋ʍU��
void SevenBoss::FireAttack() {
	const int l_LimitTimer = 200;
	m_MoveTimer++;
	if (m_MoveTimer == 1) {
		BirthFire();
	}
	if (m_MoveTimer == l_LimitTimer) {
		m_MoveTimer = {};
		m_AttackCount++;

		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 4) {
			_charaState = STATE_INTER;
			m_Return = true;
		}
		else {
			_charaState = STATE_CATCH;
		}
	}
}
void SevenBoss::BirthFire() {
	//�΂̋�
	for (int i = 0; i < FIRE_NUM; i++) {
		FireBoll* newfire;
		newfire = new FireBoll();
		newfire->Initialize();
		newfire->SetCircleSpeed(i * 90.0f);
		fireboll.push_back(newfire);
	}
}
//�v���C���[����
void SevenBoss::Confu() {
	m_MoveTimer++;
	const int l_LimitConfu = 80;
	const int l_EndConfu = 120;
	int l_ConfuTimer = {};

	if (m_MoveTimer == l_LimitConfu) {
		confueffect->SetAlive(true);
		Player::GetInstance()->SetConfu(true);
		if (isStrong) {
			l_ConfuTimer = 600;
		}
		else {
			l_ConfuTimer = 300;
		}
		Player::GetInstance()->SetConfuTimer(l_ConfuTimer);
	}
	else if (m_MoveTimer == l_EndConfu) {
		m_MoveTimer = {};
		m_AttackCount++;

		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 4) {
			_charaState = STATE_INTER;
			m_Return = true;
		}
		else {
			_charaState = STATE_CATCH;
		}
	}
}
//�_���[�W�̃u���b�N
void SevenBoss::BlockAttack() {

	const int l_LimitTimer = 200;
	m_MoveTimer++;
	if (m_MoveTimer == 1) {
		BirthBlock();
	}
	if (m_MoveTimer == l_LimitTimer) {
		m_MoveTimer = {};
		m_AttackCount++;
		//���U��������z���s���Ɉڍs����
		if (m_AttackCount != 4) {
			_charaState = STATE_INTER;
			m_Return = true;
		}
		else {
			_charaState = STATE_CATCH;
		}
	}
}
//�u���b�N�̐���
void SevenBoss::BirthBlock() {
	float l_SetPosX = {};
	float l_SetPosZ = {};
	int l_RandDir = {};
	//�����_���Ői�s�������߂�
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomDir(0, 3);
	uniform_int_distribution<int> l_RandomX(-55, 45);
	uniform_int_distribution<int> l_RandomZ(-60, 40);
	l_SetPosX = float(l_RandomX(mt));
	l_SetPosZ = float(l_RandomZ(mt));
	l_RandDir = int(l_RandomDir(mt));

	for (int i = 0; i < BLOCK_NUM; i++) {
		DamageBlock* newblock;
		newblock = new DamageBlock();
		newblock->Initialize();
		newblock->SetAttackDir(l_RandDir);
		newblock->InitPos(i,{l_SetPosX,0.0f,l_SetPosZ});
		damageblock.push_back(newblock);
	}
}
//�|���^�[�K�C�X�g�̐���
void SevenBoss::BirthPolter(const std::string& PolterName) {
	const int l_LimitTimer = 20;//��Q���������܂ł̎���
	const int l_LimitTimer2 = 50;//��Q���������܂ł̎���2
	if (PolterName == "Normal") {
		for (int i = 0; i < POLTER_NUM; i++) {
			//��Q���̔���
			Poltergeist* newpolter;
			newpolter = new Poltergeist();
			newpolter->Initialize();
			
			newpolter->SetPolterType(TYPE_FOLLOW);
			newpolter->SetTargetTimer(i * l_LimitTimer);
			if (i == 0) {
				newpolter->SetPosition({ m_Position.x + 2.0f,m_Position.y - 10.0f,m_Position.z });
			}
			else if (i == 1) {
				newpolter->SetPosition({ m_Position.x - 2.0f,m_Position.y - 10.0f,m_Position.z });
			}
			else if (i == 2) {
				newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z + 2.0f});
			}
			else {
				newpolter->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z - 2.0f });
			}
			poltergeist.push_back(newpolter);
		}
	}else if(PolterName == "Bound") {
		for (int i = 0; i < POLTER_NUM; i++) {
			//��Q���̔���
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
	//�e�ƃ{�X�̓����蔻��
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CatchBul(_playerBulA);
	m_MoveTimer++;
	m_Absorption = true;
	if (m_MoveTimer == l_LimitTimer) {
		m_MoveTimer = {};
		m_AttackCount = {};
		m_Absorption = false;
		_charaState = STATE_INTER;
		m_Return = true;
	}
	if (m_MoveTimer % 6 == 0) {
		BirthParticle();
	}

	m_Color.w = Ease(In, Cubic, 0.5f, m_Color.w, 1.0f);
}
//�X�^��������
void SevenBoss::Stun() {
	const int l_LimitTimer = 500;
	m_MoveTimer++;
	m_Absorption = false;
	if (m_MoveTimer == l_LimitTimer) {
		m_Stun = false;
		m_MoveTimer = {};
		m_InterVal = {};
		_charaState = STATE_INTER;
		bossstuneffect->SetAlive(false);
		m_Return = true;
	}
}
//�o��V�[��
void SevenBoss::AppearAction() {
	//sin�g�ɂ���ď㉺�ɓ���
	m_SinAngle += 2.0f;
	m_SinAngle2 = m_SinAngle * (3.14f / 180.0f);
	m_Position.y = (sin(m_SinAngle2) * 1.0f + 5.0f);
	m_AppearTimer++;
	if (m_AppearTimer >= 2250) {
		if (Helper::GetInstance()->CheckMax(m_Position.z, 25.0f, -0.05f)) {
		}
	}
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
				//�e�̑傫���ɂ���ė^����_���[�W���Ⴄ
				m_HP -= _bullet->GetPower() * m_Magnification;
				m_Absorption = false;
				_charaState = STATE_STUN;
				m_MoveTimer = {};
				_bullet->SetAlive(false);
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
//�{�X�̏����锻��
void SevenBoss::VanishCollide(vector<InterBullet*> bullet)
{
	int l_RandCount = {};
	XMFLOAT2 m_RandPos = {};
	int l_RandDir = {};
	const float l_VanishRadius = m_Radius + 5.0f;
	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr && _bullet->GetAlive() && !_bullet->GetInArea()) {
		
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, l_VanishRadius, m_Position.x, m_Position.z, l_VanishRadius) && (!m_Stun) && (!m_Vanish)) {
				//�����w��
				mt19937 mt{ std::random_device{}() };
				uniform_int_distribution<int> l_RandomRange(1, 100);
				uniform_int_distribution<int> l_RandomPosX(5, 10);
				uniform_int_distribution<int> l_RandomPosZ(5, 10);
				uniform_int_distribution<int> l_RandomDirX(0, 1);
				uniform_int_distribution<int> l_RandomDirZ(0, 1);
				l_RandCount = int(l_RandomRange(mt));
				_bullet->SetInArea(true);

				//���������̒l���傫��������n�[�g���o��
				if (l_RandCount > m_VanishTarget) {
					m_Vanish = true;
					m_VanishFrame = {};
					m_AfterAlpha = {};
					_vanishState = VANISH_SET;
				}
			}
		}
	}
}
//�{�X����u������
void SevenBoss::VanishBoss() {
	const float l_AddFrame = 0.05f;
	if (m_Vanish) {
		if (_vanishState == VANISH_SET) {
			if (m_VanishFrame < m_FrameMax) {
				m_VanishFrame += l_AddFrame;
			}
			else {
				m_VanishFrame = {};
				m_AfterAlpha = 1.0f;
				_vanishState = VANISH_END;
			}
		}
		else {
			if (m_VanishFrame < m_FrameMax) {
				m_VanishFrame += l_AddFrame;
			}
			else {
				m_VanishFrame = {};
				m_AfterAlpha = 0.0f;
				_vanishState = VANISH_SET;
				m_Vanish = false;
			}
		}
		m_Color.w = Ease(In, Cubic, m_VanishFrame, m_Color.w, m_AfterAlpha);

	}
}
//�����_���̓���
void SevenBoss::RandMove() {
	const int l_Division = 10;
	int l_Dir = {};
	m_ChangeTimer++;

	//���t���[���Ői�s���������߂�
	if (m_ChangeTimer == 50) {
		//��������(���Z�͂Ƒ傫��)
		mt19937 mt{ std::random_device{}() };
		uniform_int_distribution<int> l_RandSpeed(-10, 10);
		uniform_int_distribution<int> l_RandScale(-5, 5);
		uniform_int_distribution<int> l_RandDir(0, 1);
		m_AddScale = float(l_RandScale(mt)) / l_Division;
		
		m_AddSpeed = 0.5f;
		m_ChangeTimer = {};
	}
	
	Helper::GetInstance()->Clamp(m_CircleScale, 20.0f, 30.0f);
	m_CircleScale += m_AddScale;
	m_CircleSpeed += m_AddSpeed;
	m_AfterPos = Helper::GetInstance()->CircleMove(Player::GetInstance()->GetPosition(), m_CircleScale, m_CircleSpeed);
	
	m_Position = {
		Ease(In,Cubic,0.5f,m_Position.x,m_AfterPos.x),
		m_Position.y,
		Ease(In,Cubic,0.5f,m_Position.z,m_AfterPos.z),
	};
	m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, Player::GetInstance()->GetPosition(), -PI_90);
}
void SevenBoss::ReturnBoss() {
	const float l_AddFrame = 0.05f;
	if (_ReturnState == RETURN_SET) {
		m_AfterAlpha = {};
		if (m_VanishFrame < m_FrameMax) {
			m_VanishFrame += l_AddFrame;
		}
		else {
			_ReturnState = RETURN_PLAY;
			m_VanishFrame = {};
		}
	}
	else if (_ReturnState == RETURN_PLAY) {
		m_CircleScale = 30.0f;
		m_CircleSpeed = {};
		m_Position = Helper::GetInstance()->CircleMove(Player::GetInstance()->GetPosition(), m_CircleScale, m_CircleSpeed);
		m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, Player::GetInstance()->GetPosition(), -PI_90);
		_ReturnState = RETURN_END;
		m_AfterAlpha = 1.0f;
	}
	else {
		if (m_VanishFrame < m_FrameMax) {
			m_VanishFrame += l_AddFrame;
		}
		else {
			_ReturnState = RETURN_SET;
			m_VanishFrame = {};
			m_Return = false;
		}
	}

	m_Color.w = Ease(In, Cubic, m_VanishFrame, m_Color.w, m_AfterAlpha);
}
//�o���V�[����̏�����
void SevenBoss::InitAwake() {
	if (m_DeleteObj && !m_AwakeInit) {
		avatarboss.clear();
		poltergeist.clear();
		abseffect.clear();
		damageblock.clear();
		m_Position = { 0.0f,5.0f,30.0f };
		m_Rotation = { 0.0f,270.0f,0.0f };
		m_Scale = { 1.5f,1.5f,1.5f };
		m_Color = { 1.0f,0.0f,0.0f,1.0f };
		m_InterVal = {};
		m_MoveTimer = {};
		//�U����
		m_AttackCount = {};
		//�X�^���������ǂ���
		m_Stun = false;
		//�G���e������邩�ǂ���
		m_Vanish = false;
		_vanishState = VANISH_SET;
		//���������鎞��
		m_VanishFrame = {};
		//���x
		m_AfterAlpha = {};
		m_AfterPos = {};
		_charaState = STATE_INTER;
		_ReturnState = RETURN_SET;
		m_CircleSpeed = 90.0f;
		m_RotTimer = {};
		m_StartMani = false;
		m_DeleteObj = false;
		m_AwakeInit = true;
	}
}
