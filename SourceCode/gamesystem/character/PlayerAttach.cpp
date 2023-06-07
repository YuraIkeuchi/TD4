#include "PlayerAttach.h"
#include "Helper.h"
#include "Player.h"
#include "CsvLoader.h"
#include <Easing.h>
PlayerAttach::PlayerAttach() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Sutopon);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//èâä˙âª
bool PlayerAttach::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,0.0f };
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet/bullet.csv", "Stopon")));
	return true;
}
//çXêV
void PlayerAttach::Update() {

	m_Position.x += m_Angle.x * m_AddSpeed;
	m_Position.z += m_Angle.y * m_AddSpeed;

	Obj_SetParam();
	Particle();
	BirthObj();
}
//ï`âÊ
void PlayerAttach::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	if (m_Alive) {

	}
}
//ImGui
void PlayerAttach::ImGuiDraw() {
	ImGui::Begin("Attach");
	ImGui::Text("ScaleX:%f", m_Scale.x);
	ImGui::Text("Timer:%d", m_AliveTimer);
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::End();
}
void PlayerAttach::Particle() {
	XMFLOAT4 s_color = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	if (m_Particle) {
		for (int i = 0; i < 3; i++) {
			ParticleEmitter::GetInstance()->FireEffect(50, m_Position, s_scale, e_scale, s_color, e_color);
		}
	}
}
//èeÇÃèoåª
void PlayerAttach::BirthObj() {
	float l_AfterAlpha = 0.0f;
	float l_AfterScale = 0.0f;
	//èeÇ™å©Ç¶ÇÈ
	if (m_Alive) {
		m_AliveTimer++;
		if (m_AliveTimer > 10) {
			l_AfterAlpha = 0.0f;
			l_AfterScale = 0.0f;
		} else {
			l_AfterAlpha = 1.0f;
			l_AfterScale = 0.7f;
		}

		if (m_AliveTimer == 20) {
			m_Alive = false;
			m_AliveTimer = 0;
			m_Particle = true;
		}
	}

	if (m_Particle) {
		if (Helper::GetInstance()->CheckMin(m_ParticleTimer, 5, 1)) {
			m_Particle = false;
			m_ParticleTimer = 0;
		}
	}

	m_Color.w = Ease(In, Cubic, 0.75f, m_Color.w, l_AfterAlpha);

	m_Scale = { Ease(In,Cubic,0.75f,m_Scale.x,l_AfterScale),
		Ease(In,Cubic,0.75f,m_Scale.y,l_AfterScale),
		Ease(In,Cubic,0.75f,m_Scale.z,l_AfterScale),
	};
}