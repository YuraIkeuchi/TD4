#include "Food.h"
#include "ModelManager.h"
#include "imgui.h"
#include "CsvLoader.h"
#include "ParticleEmitter.h"
#include "Collision.h"
#include "HungerGauge.h"
#include <random>
Food::Food() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Cube);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool Food::Initialize() {
	//�����w��
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(-41, 50);
	uniform_int_distribution<int> l_distZ(-45, 45);
	m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	return true;
}

//�X�V
void Food::Update() {
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();
	//�H������
	BirthFood();
	//�p�[�e�B�N��
	Particle();
	//�����蔻��
	Collision();
}

//�`��
void Food::Draw(DirectXCommon* dxCommon) {
	if (m_Alive) {
		Obj_Draw();
	}
}
//ImGui�`��
void Food::ImGuiDraw() {
	//ImGui::Begin("Food");
	//ImGui::Text("Pos.X:%f", m_Position.x);
	//ImGui::Text("Timer:%d", m_Timer);
	//if (m_Alive) {

	//}
	//ImGui::End();
}
//�p�[�e�B�N��
void Food::Particle() {
	XMFLOAT4 s_color = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 e_color = { 0.0f,0.0f,1.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	if (m_Alive) {
		ParticleEmitter::GetInstance()->FireEffect(10, m_Position, s_scale, e_scale, s_color, e_color);
	}
}
//�����蔻��
bool Food::Collision() {
	float l_Radius = 1.5f;
	XMFLOAT3 m_PlayerPos = player->GetPosition();
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, m_PlayerPos.x, m_PlayerPos.z, l_Radius) && m_Alive) {
		m_Alive = false;
		HungerGauge::GetInstance()->SetNowHunger(HungerGauge::GetInstance()->GetNowHunger() + 1.0f);
		return true;
	}
	else {
		return false;
	}

	return true;
}
//�H������
void Food::BirthFood() {
	if (!m_Alive) {
		m_Timer++;
		//�`��o�O�N���邩���ɍ��W�Z�b�g
		if (m_Timer == 20) {
			//�����w��
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_distX(-41, 50);
			uniform_int_distribution<int> l_distZ(-45, 45);
			m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
		}
		//��莞�ԂŐ��������
		if (m_Timer == 100) {
			m_Alive = true;
			m_Timer = 0;
		}
	}
}