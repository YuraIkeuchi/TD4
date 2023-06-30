#include "Poltergeist.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Player.h"
#include "Helper.h"
#include "Easing.h"
Poltergeist::Poltergeist() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::CD);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool Poltergeist::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation.y = 270.0f;
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Six/Sixboss.csv", "BarrangeDamage")));
	m_Alive = true;
	m_AfterPos.y = 5.0f;
	m_ThrowType = THROW_SET;
	return true;
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (Poltergeist::* Poltergeist::stateTable[])() = {
	&Poltergeist::Follow,//�Ǐ]
	&Poltergeist::Bound,//���˒e
};
//�X�V
void Poltergeist::Update() {
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[m_PolterType])();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();

	Collide();
	Particle();
}
//�`��
void Poltergeist::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui�`��
void Poltergeist::ImGuiDraw() {
	ImGui::Begin("Polter");
	ImGui::Text("m_TargetTimer:%d", m_TargetTimer);
	ImGui::Text("m_ThrowType:%d", m_ThrowType);
	ImGui::Text("m_Position.y:%f", m_Position.y);
	ImGui::End();
}

//�p�[�e�B�N��
void Poltergeist::Particle() {
	XMFLOAT4 s_color = { 0.0f,0.4f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	float s_scale = 2.0f;
	float e_scale = 0.0f;
	const int m_Life = 50;
	if (m_Alive) {
		ParticleEmitter::GetInstance()->FireEffect(m_Life, m_Position, s_scale, e_scale, s_color, e_color);
	}
}

//�����蔻��
bool Poltergeist::Collide() {
	XMFLOAT3 l_PlayerPos = Player::GetInstance()->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 1.0f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (Player::GetInstance()->GetDamageInterVal() == 0)) {
		Player::GetInstance()->RecvDamage(l_Damage);
		Player::GetInstance()->PlayerHit(m_Position);
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//�Ǐ]
void Poltergeist::Follow() {
	const float l_AddFrame = 0.01f;
	const int l_BaseTimer = 40;
	//�e�̃Z�b�g(���񂾂񕂂��ш��ӂ���悤�Ȋ���)
	if (m_ThrowType == THROW_SET) {
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		}
		else {
			m_Frame = {};
			m_ThrowType = THROW_INTER;
		}

		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, m_AfterPos.y);
	}
	//�_�����������߂�
	else if (m_ThrowType == THROW_INTER) {
		m_ThrowTimer++;
		if (m_ThrowTimer == l_BaseTimer + m_TargetTimer) {
			double sb, sbx, sbz;
			sbx = Player::GetInstance()->GetPosition().x - m_Position.x;
			sbz = Player::GetInstance()->GetPosition().z - m_Position.z;
			sb = sqrt(sbx * sbx + sbz * sbz);
			m_SpeedX = sbx / sb * 0.8;
			m_SpeedZ = sbz / sb * 0.8;
			m_ThrowTimer = 0;
			m_ThrowType = THROW_PLAY;
		}
	}
	//���ۂɑ_�����Ⴄ
	else {
		//�v���C���[�ɃX�s�[�h���Z
		m_Position.x += (float)m_SpeedX;
		m_Position.z += (float)m_SpeedZ;

		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -70.0f, 80.0f) || Helper::GetInstance()->CheckNotValueRange(m_Position.z, -75.0f, 75.0f)) {
			m_Alive = false;
		}
	}

}
//���˒e
void Poltergeist::Bound() {

}