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
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("State:%d",int(_DeathState));
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
void PlayerAttach::AppearUpdate(int Timer) {
	m_Scale = { 0.7f,0.7f,0.7f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	if (_AppearState == APPEAR_SET) {
		if (Timer == 1) {
			m_Position = { 0.0f,3.0f,-35.0f };
			m_Rotation = { 0.0f,90.0f,0.0f };
			_AppearState = APPEAR_WALK;
		}
	}
	else if (_AppearState == APPEAR_WALK) {
		m_Position.z += 0.2f;
		if (Helper::GetInstance()->CheckMin(m_Position.z, 10.0f, 0.025f)) {
			_AppearState = APPEAR_STOP;
		}
	}
	else if(_AppearState == APPEAR_WALK2) {
		if (Helper::GetInstance()->CheckMin(m_Position.z, 15.0f, 0.025f)) {
			_AppearState = APPEAR_STOP;
		}
	}
	else {
		if (Timer == 2500) {
			_AppearState = APPEAR_WALK2;
		}
	}
	//sinîgÇ…ÇÊÇ¡Çƒè„â∫Ç…ìÆÇ≠
	m_SinAngle += 3.5f;
	m_SinAngle2 = m_SinAngle * (3.14f / 180.0f);
	m_Position.y = (sin(m_SinAngle2) * 0.5f + 3.0f);

	Obj_SetParam();
}
void PlayerAttach::LastDeadUpdate(int Timer) {
	m_Scale = { 0.7f,0.7f,0.7f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	const float l_AddFrame = 0.01f;
	if (_DeathState == DEATH_SET) {
		if (Timer == 1) {
			m_Position = { 100.0f,3.0f,-60.0f };
		}
		else if (Timer == 810) {
			m_AfterPos = { 0.0f,3.0f,-60.0f };
			_DeathState = DEATH_MOVE;
		}
	}
	else {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
		}

		m_Position = {
	Ease(In,Cubic,m_Frame,m_Position.x,m_AfterPos.x),
	Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y),
	Ease(In,Cubic,m_Frame,m_Position.z,m_AfterPos.z),
		};
	}

	//sinîgÇ…ÇÊÇ¡Çƒè„â∫Ç…ìÆÇ≠
	m_SinAngle += 3.0f;
	m_SinAngle2 = m_SinAngle * (3.14f / 180.0f);
	m_Position.y = (sin(m_SinAngle2) * 0.5f + 3.0f);
	Obj_SetParam();
}