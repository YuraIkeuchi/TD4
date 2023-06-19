#include "FourthBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "Player.h"
#include "VariableCommon.h"

void (FourthBoss::* FourthBoss::stateTable[])() = {
	&FourthBoss::InterValMove,//�����̍���
	&FourthBoss::Choice,//�I��
	&FourthBoss::LineSet,//�_���[�W�G���A�̃Z�b�g
	&FourthBoss::Debuff,//�f�o�t
	&FourthBoss::Confu,//����
	&FourthBoss::Barrage,//�e��
	&FourthBoss::Throw,//������
	&FourthBoss::EndMove,//�s���̏I���
};
//����
FourthBoss::FourthBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DJ);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	//CD�̏�����
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
//������
bool FourthBoss::Initialize() {
	m_Position = { 0.0f,3.0f,30.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 5.2f;

	_charaState = STATE_INTER;
	m_AreaState = AREA_SET;
	//CSV���[�h
	CSVLoad();
	return true;
}
//�X�L�b�v���̏�����
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
//�s��
void FourthBoss::Action() {
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
		
	//CD�̍X�V
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->SetCatchPos({ m_Position.x,((m_Position.y + 2.0f) + (0.5f * i)),m_Position.z });
		cd[i]->Update();

		if (cd[i]->GetBreakCD()) {
			m_EndCount++;
			cd[i]->SetBreakCD(false);
		}
	}

	//�U���̉���
	for (AttackNote* newnote : attacknotes) {
		if (newnote != nullptr) {
			newnote->Update();
		}
	}

	//�U���̉����̍폜
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
	//HP�������؂����狭��
	if (m_HP < m_MaxHp / 2) {
		isStrong = true;
	}
}
//�|�[�Y
void FourthBoss::Pause() {
	
}
//�G�t�F�N�g�`��
void FourthBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.1f)return;

	confueffect->Draw(dxCommon);
	noteeffect->Draw(dxCommon);
}
//�`��
void FourthBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	//CD�̍X�V
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->Draw(dxCommon);
	}
	//�U���̉���
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
//�C���^�[�o��
void FourthBoss::InterValMove() {
	m_MoveInterVal++;
	m_AreaState = AREA_SET;
	m_ThrowState = THROW_SET;
	//���ׂĂ������Ă����畜��������
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
		//�ォ�珇��CD�����
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
//�����̑I��
void FourthBoss::Choice() {
	const float l_AddAngle = 5.0f;
	float l_AddFrame = 0.001f;
	const float l_FollowSpeed = 0.4f;
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 90);
	int l_SelectRand = 0;

	//��_�Ԃ̋����v�Z
	m_Length = Helper::GetInstance()->ChechLength({ m_Position.x,0.0f,m_Position.z }, { m_AfterPos.x,0.0f,m_AfterPos.z });
	//����CD��_��
	if (m_Length > 0.5f) {
		Helper::GetInstance()->FollowMove(m_Position, m_AfterPos, l_FollowSpeed);
	}
	else {
		//�s�������߂Ď��̍s���Ɉڂ�
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
					//�U�������邩�X���[���s�������邩CD����邩���߂�
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

	//sin�g�ɂ���ď㉺�ɓ���
	m_Angle += l_AddAngle;
	m_Angle2 = m_Angle * (3.14f / 180.0f);
	m_Position.y = (sin(m_Angle2) * 2.0f + 2.0f);

	m_AfterRot.y = Helper::GetInstance()->DirRotation(m_Position, m_AfterPos, PI_90);

	m_Rotation.y = Ease(In, Cubic, 0.5f, m_Rotation.y, m_AfterRot.y);
}
//�_���[�W�G���A�̃Z�b�g
void FourthBoss::LineSet() {
	int l_BirthNum = {};
	const int l_StartLimit = 120;

	//HP�̍���ɂ���ďo�����̐���ς���
	if (isStrong) {
		l_BirthNum = 4;
	}
	else {
		l_BirthNum = 2;
	}

	m_CheckTimer++;

	//�ŏ���SE�炷
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
//�v���C���[�̃f�o�t
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
//�v���C���[����
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
//�g�U(�ӂ�)
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

		//���t���[���ŏI��
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

		//�����ɂ���ĉ�]���ς��
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
//������
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
		//�ォ�珇��CD�𓊂���
		for (int i = 0; i < cd.size(); i++) {
			if (cd[i]->GetCDState() != CD_CATCH) {
				continue;
			}
			else {
				cd[i]->SetCDState(CD_THROW);
				break;
			}
		}

		//CD�������Ă��邩�������ĂȂ������ꍇ�͎��Ɉړ�
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
		//�����I�������̃C���^�[�o��
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
//�s���̏I���(�v���C���[���瓦����)
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
	
	//�Ǐ]
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
//�m�[�c�̐���
void FourthBoss::BirthNote(const std::string& BarrageName) {
	//�q�g�c�����������p�^�[��
	if (BarrageName == "NORMAL" || BarrageName == "RANDOM") {
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = {};
		if (BarrageName == "RANDOM") {		//�����_��
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_RandomRot(0, 360);
			matRot = XMMatrixRotationY(XMConvertToRadians(float(l_RandomRot(mt))));
		}
		else if (BarrageName == "NORMAL") {	//�{�X�̌����ˑ�
			matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
		}
		move = XMVector3TransformNormal(move, matRot);
		XMFLOAT2 l_Angle;
		l_Angle.x = move.m128_f32[0];
		l_Angle.y = move.m128_f32[2];
		//�m�[�c�̔���
		AttackNote* newnote;
		newnote = new AttackNote();
		newnote->Initialize();
		newnote->SetPosition(m_Position);
		newnote->SetAngle(l_Angle);
		attacknotes.push_back(newnote);
	}
	else if (BarrageName == "ALTER") {		//�����ɒe���o��
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

			//�m�[�c�̔���
			AttackNote* newnote;
			newnote = new AttackNote();
			newnote->Initialize();
			newnote->SetPosition(m_Position);
			newnote->SetAngle(l_Angle2);
			attacknotes.push_back(newnote);
		}
	}
}
//�o��V�[��
void FourthBoss::AppearAction() {
	Obj_SetParam();
}