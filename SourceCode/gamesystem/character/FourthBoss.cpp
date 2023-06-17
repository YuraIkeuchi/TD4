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
}
//������
bool FourthBoss::Initialize() {
	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Rotation.y = -90.f;

	ActionTimer = 1;

	m_Radius = 5.0f;

	_charaState = STATE_INTER;
	m_AreaState = AREA_SET;
	//CSV���[�h
	CSVLoad();
	return true;
}
//�X�L�b�v���̏�����
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
	//OBJ�̃X�e�[�^�X�̃Z�b�g
	Obj_SetParam();
	//���~�b�g����
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
		
	//CD�̍X�V
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->SetCatchPos({ m_Position.x,m_Position.y + 3.0f,m_Position.z });
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
}
//�|�[�Y
void FourthBoss::Pause() {
	
}

void FourthBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.1f)return;
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
	ImGui::Text("m_MoveInterVal:%d", m_MoveInterVal);
	ImGui::Text("AfterPosX:%f", m_AfterPos.x);
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("EndCount:%d", m_EndCount);
	ImGui::Text("CatchCount:%d", m_CatchCount);
	ImGui::Text("STATE:%d", (int)_charaState);
	ImGui::End();

	//CD�̍X�V
	for (size_t i = 0; i < cd.size(); i++) {
		cd[i]->ImGuiDraw();
	}
	////�U���̉���
	//for (AttackNote* newnote : attacknotes) {
	//	if (newnote != nullptr) {
	//		newnote->ImGuiDraw();
	//	}
	//}
	if (damagearea != nullptr) {
		damagearea->ImGuiDraw();
	}
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
		m_AfterPos = { 0.0f,20.0f,30.0f };
	}

	//
	if (m_MoveInterVal == 100) {
		//�ォ�珇��CD�����
		for (int i = 0; i < cd.size(); i++) {
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
//�����̑I��
void FourthBoss::Choice() {
	float l_AddFrame = 0.01f;
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 90);
	int l_SelectRand = 0;

	m_StopTimer++;	//�����w��
	if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		m_Frame = 1.0f;
		m_StopTimer++;

		if (m_StopTimer > 100) {
			for (int i = 0; i < cd.size(); i++) {
				if (m_Position.x == cd[i]->GetPosition().x && cd[i]->GetCDState() == CD_STAY) {
					l_SelectRand = int(l_RandomMove(mt));
				}
				else {
					_charaState = STATE_INTER;
					m_StopTimer = 0;
					m_Frame = {};
				}
				if (cd[i]->GetCDState() != CD_STAY) {
					continue;
				}
				else {
					//�U�������邩�X���[���s�������邩CD����邩���߂�
					if (l_SelectRand < 41) {
						_charaState = i + 2;
						cd[i]->SetCDState(CD_DEATH);
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
					break;
				}
			}
		}
	}

	m_Position = {
Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
	Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z)
	};
}
//�_���[�W�G���A�̃Z�b�g
void FourthBoss::LineSet() {
	if (m_AreaState == AREA_SET) {
		damagearea.reset(new DamageArea(4));
		damagearea->Initialize();
		m_AreaState = AREA_STOP;
	}
	else if (m_AreaState == AREA_STOP) {
		_charaState = STATE_INTER;
	}
	else {

	}
}
//�v���C���[�̃f�o�t
void FourthBoss::Debuff() {
	m_CheckTimer++;
	if (m_CheckTimer == 1) {
		m_Check = true;
	}
	else if (m_CheckTimer == 50) {
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
	m_ConfuTimer++;
	const int l_LimitConfu = 20;
	const int l_EndConfu = 50;
	if (m_ConfuTimer == l_LimitConfu) {
		Player::GetInstance()->SetConfu(true);
		Player::GetInstance()->SetConfuTimer(200);
	}
	else if (m_ConfuTimer == l_EndConfu) {
		if ((cd[CD_BARRA]->GetCDState() == CD_DEATH) && m_CatchCount != 0) {
			_charaState = STATE_THROW;
		}
		else {
			_charaState = STATE_INTER;
		}
	}
}
//�g�U(�ӂ�)
void FourthBoss::Barrage() {
	m_Rotation.y += 2.0f;
	m_RotTimer++;
	if (m_RotTimer % 5 == 0) {
		BirthNote("NORMAL");
	}

	//���t���[���ŏI��
	if (m_RotTimer == 600) {
		m_RotTimer = 0;
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
	const float l_FolloSpeed = 0.0f;
	const int l_EndLimit = 300;
	const float l_AddFrame = 0.05f;
	const float l_AfterSpeed = 0.0f;
	m_EndTimer++;

	/*if (m_Frame < m_FrameMax) {
		m_Frame += l_AddFrame;
	}
	else {
		m_Frame = {};
		m_FollowSpeed = -0.3f;
	}*/
	
	m_FollowSpeed = Ease(In, Cubic, m_Frame, m_FollowSpeed, l_FolloSpeed);
	//�Ǐ]
	Helper::GetInstance()->FollowMove(m_Position, Player::GetInstance()->GetPosition(), l_FolloSpeed);
	if (m_EndTimer == l_EndLimit) {
		_charaState = STATE_INTER;
		m_FollowSpeed = {};
		m_EndTimer = {};
		m_Frame = {};
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